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

#include "MainWindow.h"
#include "WidgetCentral.h"
#include "App.h"

#include "Electricity/ZSDrawObjFactoryElectricityCapacitor.h"
#include "Electricity/ZSDrawObjFactoryElectricityDiode.h"
#include "Electricity/ZSDrawObjFactoryElectricityInductor.h"
#include "Electricity/ZSDrawObjFactoryElectricitySwitch.h"
#include "Electricity/ZSDrawObjFactoryElectricityResistor.h"
#include "Electricity/ZSDrawObjFactoryElectricityTransistor.h"
#include "Electricity/ZSDrawObjFactoryElectricityVoltageSource.h"
#include "QtWidgets/ZSDrawObjFactoryWdgtCheckBox.h"
#include "QtWidgets/ZSDrawObjFactoryWdgtComboBox.h"
#include "QtWidgets/ZSDrawObjFactoryWdgtGroupBox.h"
#include "QtWidgets/ZSDrawObjFactoryWdgtLabel.h"
#include "QtWidgets/ZSDrawObjFactoryWdgtLineEdit.h"
#include "QtWidgets/ZSDrawObjFactoryWdgtPushButton.h"

#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/GraphObjFormat/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/Drawing/ZSDrawDlgDrawingViewSetup.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/ZSDrawingView.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/GraphObjWdgts/ZSDrawGraphObjsWdgt.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoriesModel.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoryConnectionLine.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoryConnectionPoint.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoryEllipse.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoryGroup.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoryImage.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoryLine.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoryPoint.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoryPolygon.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoryPolyline.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoryRect.h"
#include "ZSDraw/GraphObjFactories/ZSDrawObjFactoryText.h"
#include "ZSPhysValGUI/ZSPhysUnitsDlg.h"
#include "ZSPhysValGUI/ZSPhysValDlgEditPhysVal.h"
#include "ZSIpcTraceGUI/ZSIpcTrcServerDlg.h"
#include "ZSTestGUI/ZSTestDlg.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSysGUI/ZSSysErrLogDlg.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeDlg.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysVersion.h"

//#include <QtCore/qaction.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qsettings.h>
//#include <QtCore/qtimer.h>

//#include <QtGui/qbitmap.h>
#include <QtGui/qevent.h>
//#include <QtGui/qicon.h>
//#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/qaction.h>
//#include <QtGui/qapplication.h>
//#include <QtGui/qbuttongroup.h>
//#include <QtGui/qcombobox.h>
#include <QtGui/qdockwidget.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qlabel.h>
//#include <QtGui/qlayout.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qmessagebox.h>
//#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
//#include <QtGui/qsplitter.h>
#include <QtGui/qstatusbar.h>
#include <QtGui/qtoolbar.h>
//#include <QtGui/qtoolbutton.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qaction.h>
//#include <QtWidgets/qapplication.h>
//#include <QtWidgets/qbuttongroup.h>
//#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlabel.h>
//#include <QtWidgets/qlayout.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmessagebox.h>
//#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>
//#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qstatusbar.h>
#include <QtWidgets/qtoolbar.h>
//#include <QtWidgets/qtoolbutton.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace::GUI;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::GUI;
using namespace ZS::Draw;
using namespace ZS::Draw::QtWidgets;
using namespace ZS::Draw::Electricity;
using namespace ZS::Apps::Products::Draw;


/*******************************************************************************
class CMainWindow : public QMainWindow
*******************************************************************************/

/*==============================================================================
public: // class members
==============================================================================*/

const QString CMainWindow::c_strMenuNameFile               = "&File";
const QString CMainWindow::c_strMenuNameMode               = "&Mode";
const QString CMainWindow::c_strMenuNameEdit               = "&Edit";
const QString CMainWindow::c_strMenuNameEditRotate         = "Edit:&Rotate";
const QString CMainWindow::c_strMenuNameEditMirror         = "Edit:&Mirror";
const QString CMainWindow::c_strMenuNameDraw               = "&Draw";
const QString CMainWindow::c_strMenuNameDrawSettings       = "Draw:S&ettings";
const QString CMainWindow::c_strMenuNameDrawStandardShapes = "Draw:&Standard Shapes";
const QString CMainWindow::c_strMenuNameDrawGraphics       = "Draw:&Graphics";
const QString CMainWindow::c_strMenuNameDrawConnections    = "Draw:&Connections";
const QString CMainWindow::c_strMenuNameDrawWidgets        = "Draw:&Widgets";
const QString CMainWindow::c_strMenuNameDrawElectricity    = "Draw:&Electricity";
const QString CMainWindow::c_strMenuNameView               = "&View";
const QString CMainWindow::c_strMenuNameDebug              = "Debug";
const QString CMainWindow::c_strMenuNameInfo               = "&Info";

const QString CMainWindow::c_strActionNameFileNew                    = c_strMenuNameFile + ":&New ...";
const QString CMainWindow::c_strActionNameFileOpen                   = c_strMenuNameFile + ":&Open ...";
const QString CMainWindow::c_strActionNameFileSave                   = c_strMenuNameFile + ":&Save ...";
const QString CMainWindow::c_strActionNameFileSaveAs                 = c_strMenuNameFile + ":Save &As ...";
const QString CMainWindow::c_strActionNameFilePageSetup              = c_strMenuNameFile + ":Page Set&up ...";
const QString CMainWindow::c_strActionNameFileQuit                   = c_strMenuNameFile + ":&Quit";
const QString CMainWindow::c_strActionNameModeEdit                   = c_strMenuNameMode + ":&Edit";
const QString CMainWindow::c_strActionNameModeView                   = c_strMenuNameMode + ":&View";
const QString CMainWindow::c_strActionNameEditSelect                 = c_strMenuNameDraw + ":&Select";
const QString CMainWindow::c_strActionNameEditRotateLeft             = c_strMenuNameEdit + ":Rotate &Left by ";
const QString CMainWindow::c_strActionNameEditRotateRight            = c_strMenuNameEdit + ":Rotate &Right by ";
const QString CMainWindow::c_strActionNameEditMirrorVertical         = c_strMenuNameEdit + ":Mirror &Vertical";
const QString CMainWindow::c_strActionNameEditMirrorHorizontal       = c_strMenuNameEdit + ":Mirror &Horizontal";
const QString CMainWindow::c_strActionNameEditGroup                  = c_strMenuNameEdit + ":Group";
const QString CMainWindow::c_strActionNameEditUngroup                = c_strMenuNameEdit + ":Ungroup";
const QString CMainWindow::c_strActionNameDrawSettingsLine           = c_strMenuNameDrawSettings + ":&Line";
const QString CMainWindow::c_strActionNameDrawSettingsFill           = c_strMenuNameDrawSettings + ":&Fill";
const QString CMainWindow::c_strActionNameDrawSettingsText           = c_strMenuNameDrawSettings + ":&Text";
const QString CMainWindow::c_strActionNameDrawStandardShapePoint     = c_strMenuNameDrawStandardShapes + ":&Point";
const QString CMainWindow::c_strActionNameDrawStandardShapeLine      = c_strMenuNameDrawStandardShapes + ":&Line";
const QString CMainWindow::c_strActionNameDrawStandardShapeRect      = c_strMenuNameDrawStandardShapes + ":&Rectangle";
const QString CMainWindow::c_strActionNameDrawStandardShapeEllipse   = c_strMenuNameDrawStandardShapes + ":&Ellipse";
const QString CMainWindow::c_strActionNameDrawStandardShapePolyline  = c_strMenuNameDrawStandardShapes + ":&Polyline";
const QString CMainWindow::c_strActionNameDrawStandardShapePolygon   = c_strMenuNameDrawStandardShapes + ":Poly&gon";
const QString CMainWindow::c_strActionNameDrawStandardShapeText      = c_strMenuNameDrawStandardShapes + ":&Text";
const QString CMainWindow::c_strActionNameDrawGraphicsImage          = c_strMenuNameDrawGraphics + ":&Image";
const QString CMainWindow::c_strActionNameDrawConnectionPoint        = c_strMenuNameDrawConnections + ":Connection &Point";
const QString CMainWindow::c_strActionNameDrawConnectionLine         = c_strMenuNameDrawConnections + ":Connection &Line";
const QString CMainWindow::c_strActionNameDrawWdgtCheckBox           = c_strMenuNameDrawWidgets + ":C&heck Box";
const QString CMainWindow::c_strActionNameDrawWdgtComboBox           = c_strMenuNameDrawWidgets + ":&Combo Box";
const QString CMainWindow::c_strActionNameDrawWdgtGroupBox           = c_strMenuNameDrawWidgets + ":&Group Box";
const QString CMainWindow::c_strActionNameDrawWdgtLabel              = c_strMenuNameDrawWidgets + ":&Label";
const QString CMainWindow::c_strActionNameDrawWdgtLineEdit           = c_strMenuNameDrawWidgets + ":Line &Edit";
const QString CMainWindow::c_strActionNameDrawWdgtPushButton         = c_strMenuNameDrawWidgets + ":&Push Button";
const QString CMainWindow::c_strActionNameDrawElectricityResistor    = c_strMenuNameDrawElectricity + ":&Resistor";
const QString CMainWindow::c_strActionNameDrawElectricityCapacitor   = c_strMenuNameDrawElectricity + ":&Capacitor";
const QString CMainWindow::c_strActionNameDrawElectricityInductor    = c_strMenuNameDrawElectricity + ":&Inductor";
const QString CMainWindow::c_strActionNameDrawElectricitySwitch      = c_strMenuNameDrawElectricity + ":&Switch";
const QString CMainWindow::c_strActionNameDrawElectricityTransistor  = c_strMenuNameDrawElectricity + ":&Transistor";
const QString CMainWindow::c_strActionNameViewZoomIn                 = c_strMenuNameView + ":&Zoom In";
const QString CMainWindow::c_strActionNameViewZoomOut                = c_strMenuNameView + ":&Zoom Out";
const QString CMainWindow::c_strActionNameViewZoom                   = c_strMenuNameView + ":&Zoom";
const QString CMainWindow::c_strActionNameDebugErrLog                = c_strMenuNameDebug + ":&Error Log";
const QString CMainWindow::c_strActionNameDebugUnits                 = c_strMenuNameDebug + ":&Units";
const QString CMainWindow::c_strActionNameDebugTraceServer           = c_strMenuNameDebug + ":&Trace Server";
const QString CMainWindow::c_strActionNameDebugTraceAdminObjs        = c_strMenuNameDebug + ":&Trace Admin Objects";
const QString CMainWindow::c_strActionNameInfoVersion                = c_strMenuNameInfo + ":&Version";

/*==============================================================================
protected: // class members
==============================================================================*/

CMainWindow* CMainWindow::s_pThis = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CMainWindow* CMainWindow::GetInstance()
//------------------------------------------------------------------------------
{
    return s_pThis;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMainWindow::CMainWindow(
    const QString&   i_strWindowTitleAppName,
    ZS::Test::CTest* i_pTest,
    unsigned int     i_uAddObjFactories,
    QWidget*         i_pWdgtParent,
    Qt::WindowFlags  i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent,i_wflags),
    m_strWindowTitleAppName(i_strWindowTitleAppName),
    m_uAddObjFactories(i_uAddObjFactories),
    m_pTest(i_pTest),
    // Object Factories
    m_pObjFactoryPoint(nullptr),
    m_pObjFactoryLine(nullptr),
    m_pObjFactoryRect(nullptr),
    m_pObjFactoryEllipse(nullptr),
    m_pObjFactoryPolygon(nullptr),
    m_pObjFactoryPolyline(nullptr),
    m_pObjFactoryText(nullptr),
    m_pObjFactoryImage(nullptr),
    m_pObjFactoryConnectionPoint(nullptr),
    m_pObjFactoryConnectionLine(nullptr),
    m_pObjFactoryGroup(nullptr),
    m_pObjFactoryWdgtCheckBox(nullptr),
    m_pObjFactoryWdgtComboBox(nullptr),
    m_pObjFactoryWdgtGroupBox(nullptr),
    m_pObjFactoryWdgtLabel(nullptr),
    m_pObjFactoryWdgtLineEdit(nullptr),
    m_pObjFactoryWdgtPushButton(nullptr),
    m_pObjFactoryElectricityCapacitor(nullptr),
    m_pObjFactoryElectricityDiode(nullptr),
    m_pObjFactoryElectricityInductor(nullptr),
    m_pObjFactoryElectricityResistor(nullptr),
    m_pObjFactoryElectricitySwitch(nullptr),
    m_pObjFactoryElectricityTransistor(nullptr),
    m_pObjFactoryElectricityVoltageSource(nullptr),
    // Menus and Actions
    m_pMenuBar(nullptr),
    // Menu - File
    m_pMenuFile(nullptr),
    m_pToolBarFile(nullptr),
    m_pActFileNew(nullptr),
    m_pActFileOpen(nullptr),
    m_pActFileSave(nullptr),
    m_pActFileSaveAs(nullptr),
    m_pActFilePageSetup(nullptr),
    m_pActFilesRecentSeparator(nullptr),
    //m_arpActFilesRecent[EFilesRecentCountMax]
    m_bDrawingChangedSinceLastSave(false),
    m_strCurrentFile(),
    m_pActFileQuit(nullptr),
    // Menu - Mode
    m_pMenuMode(nullptr),
    m_pToolBarMode(nullptr),
    m_pActModeEdit(nullptr),
    m_pActModeView(nullptr),
    // Menu - Edit
    m_pMenuEdit(nullptr),
    // Menu - Edit - Select
    m_pToolBarEditSelect(nullptr),
    m_pActEditSelect(nullptr),
    // Menu - Edit - Rotate and Mirror
    m_pToolBarEditRotateAndMirror(nullptr),
    // Menu - Edit - Rotate
    m_pMenuEditRotate(nullptr),
    m_pActEditRotateLeft(nullptr),
    m_pActEditRotateRight(nullptr),
    m_pEdtEditRotateAngle(nullptr),
    // Menu - Edit - Mirror
    m_pMenuEditMirror(nullptr),
    m_pActEditMirrorVertical(nullptr),
    m_pActEditMirrorHorizontal(nullptr),
    // Menu - Edit - Group
    m_pToolBarEditGroup(nullptr),
    m_pActEditGroup(nullptr),
    m_pActEditUngroup(nullptr),
    // Menu - Draw
    m_pMenuDraw(nullptr),
    m_pActDrawChecked(nullptr),
    // Menu - Draw - Settings
    m_pMenuDrawSettings(nullptr),
    m_pToolBarDrawSettings(nullptr),
    m_pActDrawSettingsLine(nullptr),
    m_pActDrawSettingsFill(nullptr),
    m_pActDrawSettingsText(nullptr),
    // Menu - Draw - Standard Shapes
    m_pMenuDrawStandardShapes(nullptr),
    m_pToolBarDrawStandardShapes(nullptr),
    m_pActDrawStandardShapePoint(nullptr),
    m_pActDrawStandardShapeLine(nullptr),
    m_pActDrawStandardShapeRect(nullptr),
    m_pActDrawStandardShapeEllipse(nullptr),
    m_pActDrawStandardShapePolyline(nullptr),
    m_pActDrawStandardShapePolygon(nullptr),
    m_pActDrawStandardShapeText(nullptr),
    // Menu - Draw - Graphics
    m_pMenuDrawGraphics(nullptr),
    m_pToolBarDrawGraphics(nullptr),
    m_pActDrawGraphicsImage(nullptr),
    // Menu - Draw - Connections
    m_pMenuDrawConnections(nullptr),
    m_pToolBarDrawConnections(nullptr),
    m_pActDrawConnectionPoint(nullptr),
    m_pActDrawConnectionLine(nullptr),
    // Menu - View
    m_pMenuView(nullptr),
    m_pToolBarView(nullptr),
    m_pActViewZoomIn(nullptr),
    m_pActViewZoomOut(nullptr),
    m_pEdtViewZoomFactor_perCent(nullptr),
    m_iViewZoomFactor_perCent(100),
    // Menu - Debug
    m_pMenuDebug(nullptr),
    m_pActDebugErrLog(nullptr),
    m_pActDebugUnits(nullptr),
    m_pActDebugTraceServer(nullptr),
    m_pActDebugTraceAdminObjs(nullptr),
    m_pActDebugTest(nullptr),
    // Menu - Info
    m_pMenuInfo(nullptr),
    m_pActInfoVersion(nullptr),
    m_pActInfoSettingsFile(nullptr),
    // Dock Widgets
    // Dock Widget - Object Factories
    m_pDockWdgtObjFactories(nullptr),
    m_pTreeViewObjFactories(nullptr),
    m_pModelObjFactories(nullptr),
    // Dock Widget - GraphObjs
    m_pDockWdgtGraphObjs(nullptr),
    m_pWdgtGraphObjs(nullptr),
    // Dialogs
    m_pDlgTest(nullptr),
    // Status Bar
    m_pLblStatusBarDrawingSceneEditTool(nullptr),
    m_pLblStatusBarDrawingSceneEditMode(nullptr),
    m_pLblStatusBarDrawingSceneGraphObjEditInfo(nullptr),
    m_pLblStatusBarDrawingSceneRect(nullptr),
    m_pLblStatusBarDrawingSceneMouseCursorPos(nullptr),
    m_pLblStatusBarDrawingViewMouseCursorPos(nullptr),
    m_pLblErrors(nullptr),
    // Central Widget with Drawing
    m_pWdgtCentral(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjMouseEvents(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("MainWindow");

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CMainWindow", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    m_pTrcAdminObjMouseEvents = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CMainWindow", objectName() + "-MouseEvents");

    // Graph Object Factories
    //-----------------------

    createObjFactories();

    // Central Widget
    //---------------

    m_pWdgtCentral = new CWidgetCentral();
    setCentralWidget(m_pWdgtCentral);

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    QObject::connect(
        pDrawingScene, &CDrawingScene::changed,
        this, &CMainWindow::onDrawingSceneChanged );
    QObject::connect(
        pDrawingScene, &CDrawingScene::focusItemChanged,
        this, &CMainWindow::onDrawingSceneFocusItemChanged );
    QObject::connect(
        pDrawingScene, &CDrawingScene::selectionChanged,
        this, &CMainWindow::onDrawingSceneSelectionChanged );
    QObject::connect(
        pDrawingScene, &CDrawingScene::mousePosChanged,
        this, &CMainWindow::onDrawingSceneMousePosChanged );
    QObject::connect(
        pDrawingScene, &CDrawingScene::modeChanged,
        this, &CMainWindow::onDrawingSceneModeChanged );
    QObject::connect(
        pDrawingScene, &CDrawingScene::drawSettingsChanged,
        this, &CMainWindow::onDrawingSceneDrawSettingsChanged );

    setUnifiedTitleAndToolBarOnMac(true);

    CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();

    QObject::connect(
        pDrawingView, &CDrawingView::mousePosChanged,
        this, &CMainWindow::onDrawingViewMousePosChanged );

    // Actions/Menu/StatusBar/ToolBars/DockWidgets
    //--------------------------------------------

    createActions();
    createMenus();
    createStatusBar();
    createToolBars();
    createDockWidgets();

    // Restore Geometry
    //-----------------

    QSettings settings;
    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );
    restoreState( settings.value(objectName()+"/WindowState").toByteArray() );

    // Initialize Status Settings
    //---------------------------

    onDrawingSceneModeChanged();
    onDrawingSceneDrawSettingsChanged(pDrawingScene->getDrawSettings());

} // ctor

//------------------------------------------------------------------------------
CMainWindow::~CMainWindow()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    CDialog::DestroyAllInstances();

    try {
        delete m_pDlgTest;
    }
    catch(...) {
    }
    m_pDlgTest = nullptr;

    try {
        delete m_pModelObjFactories;
    }
    catch(...) {
    }
    m_pModelObjFactories = nullptr;

    try {
        delete m_pObjFactoryPoint;
    }
    catch(...) {
    }
    m_pObjFactoryPoint = nullptr;

    try {
        delete m_pObjFactoryLine;
    }
    catch(...) {
    }
    m_pObjFactoryLine = nullptr;

    try {
        delete m_pObjFactoryRect;
    }
    catch(...) {
    }
    m_pObjFactoryRect = nullptr;

    try {
        delete m_pObjFactoryEllipse;
    }
    catch(...) {
    }
    m_pObjFactoryEllipse = nullptr;

    try {
        delete m_pObjFactoryPolyline;
    }
    catch(...) {
    }
    m_pObjFactoryPolyline = nullptr;

    try {
        delete m_pObjFactoryPolygon;
    }
    catch(...) {
    }
    m_pObjFactoryPolygon = nullptr;

    try {
        delete m_pObjFactoryText;
    }
    catch(...) {
    }
    m_pObjFactoryText = nullptr;

    try {
        delete m_pObjFactoryImage;
    }
    catch(...) {
    }
    m_pObjFactoryImage = nullptr;

    try {
        delete m_pObjFactoryConnectionPoint;
    }
    catch(...) {
    }
    m_pObjFactoryConnectionPoint = nullptr;

    try {
        delete m_pObjFactoryConnectionLine;
    }
    catch(...) {
    }
    m_pObjFactoryConnectionLine = nullptr;

    try {
        delete m_pObjFactoryGroup;
    }
    catch(...) {
    }
    m_pObjFactoryGroup = nullptr;

    try {
        delete m_pObjFactoryWdgtCheckBox;
    }
    catch(...) {
    }
    m_pObjFactoryWdgtCheckBox = nullptr;

    try {
        delete m_pObjFactoryWdgtComboBox;
    }
    catch(...) {
    }
    m_pObjFactoryWdgtComboBox = nullptr;

    try {
        delete m_pObjFactoryWdgtGroupBox;
    }
    catch(...) {
    }
    m_pObjFactoryWdgtGroupBox = nullptr;

    try {
        delete m_pObjFactoryWdgtLabel;
    }
    catch(...) {
    }
    m_pObjFactoryWdgtLabel = nullptr;

    try {
        delete m_pObjFactoryWdgtLineEdit;
    }
    catch(...) {
    }
    m_pObjFactoryWdgtLineEdit = nullptr;

    try {
        delete m_pObjFactoryWdgtPushButton;
    }
    catch(...) {
    }
    m_pObjFactoryWdgtPushButton = nullptr;

    try {
        delete m_pObjFactoryElectricityCapacitor;
    }
    catch(...) {
    }
    m_pObjFactoryElectricityCapacitor = nullptr;

    try {
        delete m_pObjFactoryElectricityDiode;
    }
    catch(...) {
    }
    m_pObjFactoryElectricityDiode = nullptr;

    try {
        delete m_pObjFactoryElectricityInductor;
    }
    catch(...) {
    }
    m_pObjFactoryElectricityInductor = nullptr;

    try {
        delete m_pObjFactoryElectricityResistor;
    }
    catch(...) {
    }
    m_pObjFactoryElectricityResistor = nullptr;

    try {
        delete m_pObjFactoryElectricitySwitch;
    }
    catch(...) {
    }
    m_pObjFactoryElectricitySwitch = nullptr;

    try {
        delete m_pObjFactoryElectricityTransistor;
    }
    catch(...) {
    }
    m_pObjFactoryElectricityTransistor = nullptr;

    try {
        delete m_pObjFactoryElectricityVoltageSource;
    }
    catch(...) {
    }
    m_pObjFactoryElectricityVoltageSource = nullptr;

    // Remove status bar section.
    if( m_pLblStatusBarDrawingSceneEditTool != nullptr ) {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSceneEditTool);
        delete m_pLblStatusBarDrawingSceneEditTool;
        m_pLblStatusBarDrawingSceneEditTool = nullptr;
    }
    if( m_pLblStatusBarDrawingSceneEditMode != nullptr ) {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSceneEditMode);
        delete m_pLblStatusBarDrawingSceneEditMode;
        m_pLblStatusBarDrawingSceneEditMode = nullptr;
    }
    if( m_pLblStatusBarDrawingSceneGraphObjEditInfo != nullptr ) {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSceneGraphObjEditInfo);
        delete m_pLblStatusBarDrawingSceneGraphObjEditInfo;
        m_pLblStatusBarDrawingSceneGraphObjEditInfo = nullptr;
    }
    if( m_pLblStatusBarDrawingSceneRect != nullptr ) {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSceneRect);
        delete m_pLblStatusBarDrawingSceneRect;
        m_pLblStatusBarDrawingSceneRect = nullptr;
    }
    if( m_pLblStatusBarDrawingSceneMouseCursorPos != nullptr ) {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSceneMouseCursorPos);
        delete m_pLblStatusBarDrawingSceneMouseCursorPos;
        m_pLblStatusBarDrawingSceneMouseCursorPos = nullptr;
    }
    if( m_pLblStatusBarDrawingViewMouseCursorPos != nullptr ) {
        statusBar()->removeWidget(m_pLblStatusBarDrawingViewMouseCursorPos);
        delete m_pLblStatusBarDrawingViewMouseCursorPos;
        m_pLblStatusBarDrawingViewMouseCursorPos = nullptr;
    }

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjMouseEvents);
    m_pTrcAdminObjMouseEvents = nullptr;

    // Object Factories
    m_pObjFactoryPoint = nullptr;
    m_pObjFactoryLine = nullptr;
    m_pObjFactoryRect = nullptr;
    m_pObjFactoryEllipse = nullptr;
    m_pObjFactoryPolyline = nullptr;
    m_pObjFactoryPolygon = nullptr;
    m_pObjFactoryText = nullptr;
    m_pObjFactoryImage = nullptr;
    m_pObjFactoryConnectionPoint = nullptr;
    m_pObjFactoryConnectionLine = nullptr;
    m_pObjFactoryGroup = nullptr;
    m_pObjFactoryWdgtCheckBox = nullptr;
    m_pObjFactoryWdgtComboBox = nullptr;
    m_pObjFactoryWdgtGroupBox = nullptr;
    m_pObjFactoryWdgtLabel = nullptr;
    m_pObjFactoryWdgtLineEdit = nullptr;
    m_pObjFactoryWdgtPushButton = nullptr;
    m_pObjFactoryElectricityCapacitor = nullptr;
    m_pObjFactoryElectricityDiode = nullptr;
    m_pObjFactoryElectricityInductor = nullptr;
    m_pObjFactoryElectricityResistor = nullptr;
    m_pObjFactoryElectricitySwitch = nullptr;
    m_pObjFactoryElectricityTransistor = nullptr;
    m_pObjFactoryElectricityVoltageSource = nullptr;
    // Menus and Actions
    m_pMenuBar = nullptr;
    // Menu - File
    m_pMenuFile = nullptr;
    m_pToolBarFile = nullptr;
    m_pActFileNew = nullptr;
    m_pActFileOpen = nullptr;
    m_pActFileSave = nullptr;
    m_pActFileSaveAs = nullptr;
    m_pActFilePageSetup = nullptr;
    m_pActFilesRecentSeparator = nullptr;
    memset( m_arpActFilesRecent, 0x00, _ZSArrLen(m_arpActFilesRecent) );
    m_pActFileQuit = nullptr;
    // Menu - Mode
    m_pMenuMode = nullptr;
    m_pToolBarMode = nullptr;
    m_pActModeEdit = nullptr;
    m_pActModeView = nullptr;
    // Menu - Edit
    m_pMenuEdit = nullptr;
    // Menu - Edit - Select
    m_pToolBarEditSelect = nullptr;
    m_pActEditSelect = nullptr;
    // Menu - Edit - Rotate and Mirror
    m_pToolBarEditRotateAndMirror = nullptr;
    // Menu - Edit - Rotate
    m_pMenuEditRotate = nullptr;
    m_pActEditRotateLeft = nullptr;
    m_pActEditRotateRight = nullptr;
    m_pEdtEditRotateAngle = nullptr;
    // Menu - Edit - Mirror
    m_pMenuEditMirror = nullptr;
    m_pActEditMirrorVertical = nullptr;
    m_pActEditMirrorHorizontal = nullptr;
    // Menu - Edit - Group
    m_pToolBarEditGroup = nullptr;
    m_pActEditGroup = nullptr;
    m_pActEditUngroup = nullptr;
    // Menu - Draw
    m_pMenuDraw = nullptr;
    m_pActDrawChecked = nullptr;
    // Menu - Draw - Settings
    m_pMenuDrawSettings = nullptr;
    m_pToolBarDrawSettings = nullptr;
    m_pActDrawSettingsLine = nullptr;
    m_pActDrawSettingsFill = nullptr;
    m_pActDrawSettingsText = nullptr;
    // Menu - Draw - Standard Shapes
    m_pMenuDrawStandardShapes = nullptr;
    m_pToolBarDrawStandardShapes = nullptr;
    m_pActDrawStandardShapePoint = nullptr;
    m_pActDrawStandardShapeLine = nullptr;
    m_pActDrawStandardShapeRect = nullptr;
    m_pActDrawStandardShapeEllipse = nullptr;
    m_pActDrawStandardShapePolyline = nullptr;
    m_pActDrawStandardShapePolygon = nullptr;
    m_pActDrawStandardShapeText = nullptr;
    // Menu - Draw - Graphics
    m_pMenuDrawGraphics = nullptr;
    m_pToolBarDrawGraphics = nullptr;
    m_pActDrawGraphicsImage = nullptr;
    // Menu - Draw - Connections
    m_pMenuDrawConnections = nullptr;
    m_pToolBarDrawConnections = nullptr;
    m_pActDrawConnectionPoint = nullptr;
    m_pActDrawConnectionLine = nullptr;
    // Menu - View
    m_pMenuView = nullptr;
    m_pToolBarView = nullptr;
    m_pActViewZoomIn = nullptr;
    m_pActViewZoomOut = nullptr;
    m_pEdtViewZoomFactor_perCent = nullptr;
    m_iViewZoomFactor_perCent = 0;
    // Menu - Trace
    m_pMenuDebug = nullptr;
    m_pActDebugErrLog = nullptr;
    m_pActDebugUnits = nullptr;
    m_pActDebugTraceServer = nullptr;
    m_pActDebugTraceAdminObjs = nullptr;
    m_pActDebugTest = nullptr;
    // Menu - Info
    m_pMenuInfo = nullptr;
    m_pActInfoVersion = nullptr;
    m_pActInfoSettingsFile = nullptr;
    // Dock Widgets
    // Dock Widget - Object Factories
    m_pDockWdgtObjFactories = nullptr;
    m_pTreeViewObjFactories = nullptr;
    m_pModelObjFactories = nullptr;
    // Dock Widget - GraphObjs
    m_pDockWdgtGraphObjs = nullptr;
    m_pWdgtGraphObjs = nullptr;
    // Dialogs
    m_pDlgTest = nullptr;
    // Status Bar
    m_pLblStatusBarDrawingSceneEditTool = nullptr;
    m_pLblStatusBarDrawingSceneEditMode = nullptr;
    m_pLblStatusBarDrawingSceneRect = nullptr;
    m_pLblStatusBarDrawingSceneGraphObjEditInfo = nullptr;
    m_pLblStatusBarDrawingSceneMouseCursorPos = nullptr;
    m_pLblStatusBarDrawingViewMouseCursorPos = nullptr;
    m_pLblErrors = nullptr;
    // Central Widget with Drawing
    m_pWdgtCentral = nullptr;
    // Trace
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjMouseEvents = nullptr;

    s_pThis = nullptr;

} // dtor

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "closeEvent",
        /* strAddInfo   */ "" );

    if( m_bDrawingChangedSinceLastSave ) {
        QString strMsgText =
            "The drawing has been modified.\n"
            "Do you want to save your changes?";
        QMessageBox::StandardButton msgBoxBtn = QMessageBox::warning(
            /* pWdgtParent */ this,
            /* strTitle    */ QApplication::applicationName(),
            /* strText     */ strMsgText,
            /* buttons     */ QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel );
        if( msgBoxBtn == QMessageBox::Save ) {
            onActionFileSaveTriggered(true);
            i_pEv->accept();
        }
        else if( msgBoxBtn == QMessageBox::Cancel ) {
            i_pEv->ignore();
        }
        else {
            i_pEv->accept();
        }
    }

    if( i_pEv->isAccepted() ) {
        QSettings settings;
        settings.setValue(objectName() + "/Geometry", saveGeometry());
        settings.setValue(objectName() + "/WindowState", saveState());

        CDialog::HideAllInstances();

        if( m_pDlgTest != nullptr && m_pDlgTest->isVisible() ) {
            m_pDlgTest->hide();
        }

        if( m_pDockWdgtGraphObjs != nullptr ) {
            m_pWdgtGraphObjs->saveState(settings);
            if( m_pDockWdgtGraphObjs->isVisible() ) {
                m_pDockWdgtGraphObjs->hide();
            }
        }
        if( m_pDockWdgtObjFactories != nullptr ) {
            if( m_pDockWdgtObjFactories->isVisible() ) {
                m_pDockWdgtObjFactories->hide();
            }
        }
    }
} // closeEvent

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setWindowTitle()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strAddTrcInfo = "FileName:" + m_strCurrentFile;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setWindowTitle",
        /* strAddInfo   */ strAddTrcInfo );

    QString strWindowTitle = m_strWindowTitleAppName;
    QString strFileName = tr("Unnamed");
    if( !m_strCurrentFile.isEmpty() ) {
        QFileInfo fileInfo(m_strCurrentFile);
        strFileName = fileInfo.fileName();
    }
    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    CEnumMode mode = pDrawingScene->getMode();
    strWindowTitle += " - " + mode.toString();
    strWindowTitle += " - " + strFileName;
    QMainWindow::setWindowTitle(strWindowTitle);
}

/*==============================================================================
protected: // instance methods (for ctor)
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates the graphical object factories used in the application.
*/
void CMainWindow::createObjFactories()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createObjFactories",
        /* strAddInfo   */ "" );

    // <Menu> Edit
    //============

    // <MenuItem> Edit::Group
    //-----------------------

    QPixmap pxmEditGroup16x16(":/ZS/Draw/EditGroup16x16.bmp");
    pxmEditGroup16x16.setMask(pxmEditGroup16x16.createMaskFromColor(Qt::white));
    m_pObjFactoryGroup = new CObjFactoryGroup(pxmEditGroup16x16);

    // <Menu> Draw
    //============

    // <Menu> Draw::Standard Shapes
    //---------------------------------------------

    // <MenuItem> Draw::Standard Shapes::Draw Point
    //---------------------------------------------

    QPixmap pxmDrawStandardShapesPoint16x16(":/ZS/Draw/DrawPoint16x16.bmp");
    pxmDrawStandardShapesPoint16x16.setMask(pxmDrawStandardShapesPoint16x16.createHeuristicMask());
    m_pObjFactoryPoint = new CObjFactoryPoint(pxmDrawStandardShapesPoint16x16);

    // <MenuItem> Draw::Standard Shapes::Draw Line
    //--------------------------------------------

    QPixmap pxmDrawStandardShapesLine16x16(":/ZS/Draw/DrawLine16x16.bmp");
    pxmDrawStandardShapesLine16x16.setMask(pxmDrawStandardShapesLine16x16.createHeuristicMask());
    m_pObjFactoryLine = new CObjFactoryLine(pxmDrawStandardShapesLine16x16);

    // <MenuItem> Draw::Standard Shapes::Draw Rect
    //--------------------------------------------

    QPixmap pxmDrawStandardShapesRect16x16(":/ZS/Draw/DrawRect16x16.bmp");
    pxmDrawStandardShapesRect16x16.setMask(pxmDrawStandardShapesRect16x16.createHeuristicMask());
    m_pObjFactoryRect = new CObjFactoryRect(pxmDrawStandardShapesRect16x16);

    // <MenuItem> Draw::Standard Shapes::Draw Ellipse
    //-----------------------------------------------

    QPixmap pxmDrawStandardShapesEllipse16x16(":/ZS/Draw/DrawEllipse16x16.bmp");
    pxmDrawStandardShapesEllipse16x16.setMask(pxmDrawStandardShapesEllipse16x16.createHeuristicMask());
    m_pObjFactoryEllipse = new CObjFactoryEllipse(pxmDrawStandardShapesEllipse16x16);

    // <MenuItem> Draw::Standard Shapes::Draw Polyline
    //------------------------------------------------

    QPixmap pxmDrawStandardShapesPolyline16x16(":/ZS/Draw/DrawPolyline16x16.bmp");
    pxmDrawStandardShapesPolyline16x16.setMask(pxmDrawStandardShapesPolyline16x16.createHeuristicMask());
    m_pObjFactoryPolyline = new CObjFactoryPolyline(pxmDrawStandardShapesPolyline16x16);

    // <MenuItem> Draw::Standard Shapes::Draw Polygon
    //------------------------------------------------

    QPixmap pxmDrawStandardShapesPolygon16x16(":/ZS/Draw/DrawPolygon16x16.bmp");
    pxmDrawStandardShapesPolygon16x16.setMask(pxmDrawStandardShapesPolygon16x16.createHeuristicMask());
    m_pObjFactoryPolygon = new CObjFactoryPolygon(pxmDrawStandardShapesPolygon16x16);

    // <MenuItem> Draw::Standard Shapes::Draw Text
    //------------------------------------------------

    QPixmap pxmDrawStandardShapesText16x16(":/ZS/Draw/DrawText16x16.bmp");
    pxmDrawStandardShapesText16x16.setMask(pxmDrawStandardShapesText16x16.createHeuristicMask());
    m_pObjFactoryText = new CObjFactoryText(pxmDrawStandardShapesText16x16);

    // <Menu> Draw::Graphics
    //----------------------

    // <MenuItem> Draw::Graphics::Image
    //---------------------------------

    QPixmap pxmDrawGraphicsImage16x16(":/ZS/Draw/DrawImage16x16.bmp");
    pxmDrawGraphicsImage16x16.setMask(pxmDrawGraphicsImage16x16.createHeuristicMask());
    m_pObjFactoryImage = new CObjFactoryImage(pxmDrawGraphicsImage16x16);

    // <Menu> Draw::Connections
    //-------------------------

    // <MenuItem> Draw::Connections::Draw Connection Point
    //----------------------------------------------------

    QPixmap pxmDrawConnectionPoint16x16(":/ZS/Draw/DrawConnectionPoint16x16.bmp");
    pxmDrawConnectionPoint16x16.setMask(pxmDrawConnectionPoint16x16.createHeuristicMask());
    m_pObjFactoryConnectionPoint = new CObjFactoryConnectionPoint(pxmDrawConnectionPoint16x16);

    // <MenuItem> Draw::Connections::Draw Connection Line
    //----------------------------------------------------

    QPixmap pxmDrawConnectionLine16x16(":/ZS/Draw/DrawConnectionLine16x16.bmp");
    pxmDrawConnectionLine16x16.setMask(pxmDrawConnectionLine16x16.createHeuristicMask());
    m_pObjFactoryConnectionLine = new CObjFactoryConnectionLine(pxmDrawConnectionLine16x16);

    // <Menu> Draw::Widgets
    //----------------------

    if( m_uAddObjFactories & EAddObjFactoriesQtWidgets )
    {
        // <MenuItem> Draw::Widgets::CheckBox
        //-----------------------------------

        QPixmap pxmDrawWdgtCheckBox16x16(":/ZS/Draw/QtWidgets/CheckBox16x16.bmp");
        pxmDrawWdgtCheckBox16x16.setMask(pxmDrawWdgtCheckBox16x16.createHeuristicMask());
        m_pObjFactoryWdgtCheckBox = new CObjFactoryWdgtCheckBox(pxmDrawWdgtCheckBox16x16);

        // <MenuItem> Draw::Widgets::ComboBox
        //-----------------------------------

        QPixmap pxmDrawWdgtComboBox16x16(":/ZS/Draw/QtWidgets/ComboBox16x16.bmp");
        pxmDrawWdgtComboBox16x16.setMask(pxmDrawWdgtComboBox16x16.createHeuristicMask());
        m_pObjFactoryWdgtComboBox = new CObjFactoryWdgtComboBox(pxmDrawWdgtComboBox16x16);

        // <MenuItem> Draw::Widgets::GroupBox
        //-----------------------------------

        QPixmap pxmDrawWdgtGroupBox16x16(":/ZS/Draw/QtWidgets/GroupBox16x16.bmp");
        pxmDrawWdgtGroupBox16x16.setMask(pxmDrawWdgtGroupBox16x16.createHeuristicMask());
        m_pObjFactoryWdgtGroupBox = new CObjFactoryWdgtGroupBox(pxmDrawWdgtGroupBox16x16);

        // <MenuItem> Draw::Widgets::Label
        //-----------------------------------

        QPixmap pxmDrawWdgtLabel16x16(":/ZS/Draw/QtWidgets/Label16x16.bmp");
        pxmDrawWdgtLabel16x16.setMask(pxmDrawWdgtLabel16x16.createHeuristicMask());
        m_pObjFactoryWdgtLabel = new CObjFactoryWdgtLabel(pxmDrawWdgtLabel16x16);

        // <MenuItem> Draw::Widgets::LineEdit
        //-----------------------------------

        QPixmap pxmDrawWdgtLineEdit16x16(":/ZS/Draw/QtWidgets/LineEdit16x16.bmp");
        pxmDrawWdgtLineEdit16x16.setMask(pxmDrawWdgtLineEdit16x16.createHeuristicMask());
        m_pObjFactoryWdgtLineEdit = new CObjFactoryWdgtLineEdit(pxmDrawWdgtLineEdit16x16);

        // <MenuItem> Draw::Widgets::PushButton
        //-------------------------------------

        QPixmap pxmDrawWdgtPushButton16x16(":/ZS/Draw/QtWidgets/PushButton16x16.bmp");
        pxmDrawWdgtPushButton16x16.setMask(pxmDrawWdgtPushButton16x16.createHeuristicMask());
        m_pObjFactoryWdgtPushButton = new CObjFactoryWdgtPushButton(pxmDrawWdgtPushButton16x16);

    } // if( m_uAddObjFactories & EAddObjFactoriesQtWidgets )

    // <Menu> Draw::Electricity
    //-------------------------

    if( m_uAddObjFactories & EAddObjFactoriesElectricity )
    {
        // <MenuItem> Draw::Electricity::VoltageSource
        //--------------------------------------------

        QPixmap pxmDrawVoltageSource(":/ZS/Draw/Electricity/VoltageSource16x16.bmp");
        pxmDrawVoltageSource.setMask(pxmDrawVoltageSource.createHeuristicMask());
        m_pObjFactoryElectricityVoltageSource = new CObjFactoryVoltageSource(pxmDrawVoltageSource);

        // <MenuItem> Draw::Electricity::Resistor
        //---------------------------------------

        QPixmap pxmDrawResistor(":/ZS/Draw/Electricity/Resistor16x16.bmp");
        pxmDrawResistor.setMask(pxmDrawResistor.createHeuristicMask());
        m_pObjFactoryElectricityResistor = new CObjFactoryResistor(pxmDrawResistor);

        // <MenuItem> Draw::Electricity::Inductor
        //---------------------------------------

        QPixmap pxmDrawInductor(":/ZS/Draw/Electricity/Inductor16x16.bmp");
        pxmDrawInductor.setMask(pxmDrawInductor.createHeuristicMask());
        m_pObjFactoryElectricityInductor = new CObjFactoryInductor(pxmDrawInductor);

        // <MenuItem> Draw::Electricity::Capacitor
        //---------------------------------------

        QPixmap pxmDrawCapacitor(":/ZS/Draw/Electricity/Capacitor16x16.bmp");
        pxmDrawCapacitor.setMask(pxmDrawCapacitor.createHeuristicMask());
        m_pObjFactoryElectricityCapacitor = new CObjFactoryCapacitor(pxmDrawCapacitor);

        // <MenuItem> Draw::Electricity::Switch
        //---------------------------------------

        QPixmap pxmDrawSwitch(":/ZS/Draw/Electricity/Switch16x16.bmp");
        pxmDrawSwitch.setMask(pxmDrawSwitch.createHeuristicMask());
        m_pObjFactoryElectricitySwitch = new CObjFactorySwitch(pxmDrawSwitch);

        // <MenuItem> Draw::Electricity::Diode
        //-----------------------------------------

        QPixmap pxmDrawDiode(":/ZS/Draw/Electricity/Diode16x16.bmp");
        pxmDrawDiode.setMask(pxmDrawDiode.createHeuristicMask());
        m_pObjFactoryElectricityDiode = new CObjFactoryDiode(pxmDrawDiode);

        // <MenuItem> Draw::Electricity::Transistor
        //-----------------------------------------

        QPixmap pxmDrawTransistor(":/ZS/Draw/Electricity/Transistor16x16.bmp");
        pxmDrawTransistor.setMask(pxmDrawTransistor.createHeuristicMask());
        m_pObjFactoryElectricityTransistor = new CObjFactoryTransistor(pxmDrawTransistor);

    } // if( m_uAddObjFactories & EAddObjFactoriesElectricity )

} // createObjFactories

//------------------------------------------------------------------------------
/*! Creates all the actions used in the menu and in the toolbars.
*/
void CMainWindow::createActions()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createActions",
        /* strAddInfo   */ "" );

    // <Menu> File
    //============

    // <MenuItem> File::New
    //----------------------

    QIcon iconFileNew;

    QPixmap pxmFileNew16x16(":/ZS/Menu/MenuFileNew16x16.bmp");

    pxmFileNew16x16.setMask(pxmFileNew16x16.createHeuristicMask());

    iconFileNew.addPixmap(pxmFileNew16x16);

    m_pActFileNew = new QAction( iconFileNew, c_strActionNameFileNew.section(":",-1,-1), this );
    m_pActFileNew->setShortcuts(QKeySequence::New);
    m_pActFileNew->setStatusTip(tr("Create a new file"));

    QObject::connect(
        m_pActFileNew, &QAction::triggered,
        this, &CMainWindow::onActionFileNewTriggered );

    // <MenuItem> File::Open
    //----------------------

    QIcon iconFileOpen;

    QPixmap pxmFileOpen16x16(":/ZS/Menu/MenuFileOpen16x16.bmp");

    pxmFileOpen16x16.setMask(pxmFileOpen16x16.createHeuristicMask());

    iconFileOpen.addPixmap(pxmFileOpen16x16);

    m_pActFileOpen = new QAction( iconFileOpen, c_strActionNameFileOpen.section(":",-1,-1), this );
    m_pActFileOpen->setShortcuts(QKeySequence::Open);
    m_pActFileOpen->setStatusTip(tr("Open an existing file"));

    QObject::connect(
        m_pActFileOpen, &QAction::triggered,
        this, &CMainWindow::onActionFileOpenTriggered );

    // <MenuItem> File::Save
    //----------------------

    QIcon iconFileSave;

    QPixmap pxmFileSave16x16(":/ZS/Menu/MenuFileSave16x16.bmp");

    pxmFileSave16x16.setMask(pxmFileSave16x16.createHeuristicMask());

    iconFileSave.addPixmap(pxmFileSave16x16);

    m_pActFileSave = new QAction( iconFileSave, c_strActionNameFileSave.section(":",-1,-1), this );
    m_pActFileSave->setShortcuts(QKeySequence::Save);
    m_pActFileSave->setStatusTip(tr("Save current file"));

    QObject::connect(
        m_pActFileSave, &QAction::triggered,
        this, &CMainWindow::onActionFileSaveTriggered );

    // <MenuItem> File::Save As
    //-------------------------

    QIcon iconFileSaveAs;

    QPixmap pxmFileSaveAs16x16(":/ZS/Menu/MenuFileSaveAs16x16.bmp");

    pxmFileSaveAs16x16.setMask(pxmFileSaveAs16x16.createHeuristicMask());

    iconFileSaveAs.addPixmap(pxmFileSaveAs16x16);

    m_pActFileSaveAs = new QAction( iconFileSaveAs, c_strActionNameFileSaveAs.section(":",-1,-1), this );
    m_pActFileSaveAs->setShortcuts(QKeySequence::SaveAs);
    m_pActFileSaveAs->setStatusTip(tr("Save current file choosing a different file name"));

    QObject::connect(
        m_pActFileSaveAs, &QAction::triggered,
        this, &CMainWindow::onActionFileSaveAsTriggered );

    // <MenuItem> File::Page Setup
    //----------------------------

    QIcon   iconFilePageSetup;
    QPixmap pxmFilePageSetup16x16(":/ZS/Draw/FilePageSetup16x16.bmp");

    pxmFilePageSetup16x16.setMask(pxmFilePageSetup16x16.createHeuristicMask());

    iconFilePageSetup.addPixmap(pxmFilePageSetup16x16);

    m_pActFilePageSetup = new QAction( iconFilePageSetup, c_strActionNameFilePageSetup.section(":",-1,-1), this );
    m_pActFilePageSetup->setShortcuts(QKeySequence::Preferences);
    m_pActFilePageSetup->setStatusTip(tr("Set properties of current page"));

    QObject::connect(
        m_pActFilePageSetup, &QAction::triggered,
        this, &CMainWindow::onActionFilePageSetupTriggered );

    // <MenuItem> File::Recent Files
    //------------------------------

    bool bHasActFilesRecent = false;
    int  idxFile;

    for( idxFile = 0; idxFile < _ZSArrLen(m_arpActFilesRecent); idxFile++ )
    {
        if( m_arpActFilesRecent[idxFile] != nullptr )
        {
            bHasActFilesRecent = true;
            break;
        }
    }

    if( bHasActFilesRecent )
    {
        for( idxFile = 0; idxFile < _ZSArrLen(m_arpActFilesRecent); idxFile++ )
        {
            if( m_arpActFilesRecent[idxFile] != nullptr )
            {
                m_arpActFilesRecent[idxFile] = new QAction(this);
                m_arpActFilesRecent[idxFile]->setVisible(false);

                QObject::connect(
                    m_arpActFilesRecent[idxFile], &QAction::triggered,
                    this, &CMainWindow::onActionFileRecentTriggered );
            }
        }

        updateActionsFilesRecent();

    } // if( bHasActFilesRecent )

    // <MenuItem> File::Quit
    //----------------------

    m_pActFileQuit = new QAction( c_strActionNameFileQuit.section(":",-1,-1), this );
    m_pActFileQuit->setStatusTip( tr("Quit application") );

    QObject::connect(
        m_pActFileQuit, &QAction::triggered,
        this, &CApplication::quit );

    // <Menu> Mode
    //============

    // <MenuItem> Mode::Edit
    //----------------------

    QIcon iconModeEdit;

    iconModeEdit.addPixmap( mode2Pixmap(static_cast<int>(EMode::Edit),24) );

    m_pActModeEdit = new QAction( iconModeEdit, c_strActionNameModeEdit.section(":",-1,-1), this );
    m_pActModeEdit->setStatusTip( tr("Activate Edit Mode") );
    m_pActModeEdit->setCheckable(true);
    m_pActModeEdit->setChecked(true);

    QObject::connect(
        m_pActModeEdit, &QAction::triggered,
        this, &CMainWindow::onActionModeEditToggled );

    // <MenuItem> Mode::View
    //-------------------------------

    QIcon iconModeView;

    iconModeView.addPixmap( mode2Pixmap(static_cast<int>(EMode::View),24) );

    m_pActModeView = new QAction( iconModeView, c_strActionNameModeView.section(":",-1,-1), this );
    m_pActModeView->setStatusTip( tr("Activate View Mode") );
    m_pActModeView->setCheckable(true);

    QObject::connect(
        m_pActModeView, &QAction::triggered,
        this, &CMainWindow::onActionModeViewToggled );

    // <Menu> Edit
    //============

    // <MenuItem> Edit::Select
    //-------------------------------

    QIcon   iconEditSelect;
    QPixmap pxmEditSelect16x16(":/ZS/Draw/CursorSelect16x16.bmp");

    pxmEditSelect16x16.setMask(pxmEditSelect16x16.createHeuristicMask());

    iconEditSelect.addPixmap(pxmEditSelect16x16);

    m_pActEditSelect = new QAction( iconEditSelect, c_strActionNameEditSelect.section(":",-1,-1), this );
    m_pActEditSelect->setStatusTip( tr("Select Object(s)") );
    m_pActEditSelect->setCheckable(true);
    m_pActEditSelect->setEnabled(false);

    QObject::connect(
        m_pActEditSelect, &QAction::triggered,
        this, &CMainWindow::onActionEditSelectToggled );

    // <MenuItem> Edit::Rotate::Left
    //------------------------------

    QIcon   iconEditRotateLeft;
    QPixmap pxmEditRotateLeft(":/ZS/Draw/DrawToolRotateLeftBy90Degrees16x16.bmp");

    pxmEditRotateLeft.setMask(pxmEditRotateLeft.createHeuristicMask());

    iconEditRotateLeft.addPixmap(pxmEditRotateLeft);

    m_pActEditRotateLeft = new QAction( iconEditRotateLeft, c_strActionNameEditRotateLeft.section(":",-1,-1), this );
    m_pActEditRotateLeft->setStatusTip( tr("Rotate Selected Object(s) Left (counterclockwise)") );
    m_pActEditRotateLeft->setEnabled(false);

    QObject::connect(
        m_pActEditRotateLeft, &QAction::triggered,
        this, &CMainWindow::onActionEditRotateLeftTriggered );

    // <MenuItem> Edit::Rotate::Right
    //-------------------------------

    QIcon   iconEditRotateRight;
    QPixmap pxmEditRotateRight(":/ZS/Draw/DrawToolRotateRightBy90Degrees16x16.bmp");

    pxmEditRotateRight.setMask(pxmEditRotateRight.createHeuristicMask());

    iconEditRotateRight.addPixmap(pxmEditRotateRight);

    m_pActEditRotateRight = new QAction( iconEditRotateRight, c_strActionNameEditRotateRight.section(":",-1,-1), this );
    m_pActEditRotateRight->setStatusTip( tr("Rotate Selected Object(s) Right (clockwise)") );
    m_pActEditRotateRight->setEnabled(false);

    QObject::connect(
        m_pActEditRotateRight, &QAction::triggered,
        this, &CMainWindow::onActionEditRotateRightTriggered );

    // <MenuItem> Edit::Rotate::Angle
    //-------------------------------

    // The edit field for defining the rotation angle belongs to the tool bar.

    // <MenuItem> Edit::Mirror::Vertical
    //----------------------------------

    QIcon   iconEditMirrorVertical;
    QPixmap pxmEditMirrorVertical(":/ZS/Draw/DrawToolMirrorVertical16x16.bmp");

    pxmEditMirrorVertical.setMask(pxmEditMirrorVertical.createHeuristicMask());

    iconEditMirrorVertical.addPixmap(pxmEditMirrorVertical);

    m_pActEditMirrorVertical = new QAction( iconEditMirrorVertical, c_strActionNameEditMirrorVertical.section(":",-1,-1), this );
    m_pActEditMirrorVertical->setStatusTip( tr("Vertically Mirror Selected Object(s)") );
    m_pActEditMirrorVertical->setEnabled(false);

    QObject::connect(
        m_pActEditMirrorVertical, &QAction::triggered,
        this, &CMainWindow::onActionEditMirrorVerticalTriggered );

    // <MenuItem> Edit::Mirror::Horizontal
    //------------------------------------

    QIcon   iconEditMirrorHorizontal;
    QPixmap pxmEditMirrorHorizontal(":/ZS/Draw/DrawToolMirrorHorizontal16x16.bmp");

    pxmEditMirrorHorizontal.setMask(pxmEditMirrorHorizontal.createHeuristicMask());

    iconEditMirrorHorizontal.addPixmap(pxmEditMirrorHorizontal);

    m_pActEditMirrorHorizontal = new QAction( iconEditMirrorHorizontal, c_strActionNameEditMirrorHorizontal.section(":",-1,-1), this );
    m_pActEditMirrorHorizontal->setStatusTip( tr("Horizontally Mirror Selected Object(s)") );
    m_pActEditMirrorHorizontal->setEnabled(false);

    QObject::connect(
        m_pActEditMirrorHorizontal, &QAction::triggered,
        this, &CMainWindow::onActionEditMirrorHorizontalTriggered );

    // <MenuItem> Edit::Group
    //-----------------------

    QIcon   iconEditGroup;
    QPixmap pxmEditGroup16x16(":/ZS/Draw/EditGroup16x16.bmp");

    pxmEditGroup16x16.setMask(pxmEditGroup16x16.createMaskFromColor(Qt::white));

    iconEditGroup.addPixmap(pxmEditGroup16x16);

    m_pActEditGroup = new QAction(iconEditGroup, c_strActionNameEditGroup.section(":",-1,-1), this);
    m_pActEditGroup->setStatusTip(tr("Group Selected Objects"));
    m_pActEditGroup->setEnabled(false);

    QObject::connect(
        m_pActEditGroup, &QAction::triggered,
        this, &CMainWindow::onActionEditGroupTriggered );

    // <MenuItem> Edit::Ungroup
    //-------------------------

    QIcon   iconEditUngroup;
    QPixmap pxmEditUngroup16x16(":/ZS/Draw/EditUngroup16x16.bmp");

    pxmEditUngroup16x16.setMask(pxmEditUngroup16x16.createMaskFromColor(Qt::white));

    iconEditUngroup.addPixmap(pxmEditUngroup16x16);

    m_pActEditUngroup = new QAction( iconEditUngroup, c_strActionNameEditUngroup.section(":",-1,-1), this );
    m_pActEditUngroup->setStatusTip( tr("Ungroup Selected Group(s)") );
    m_pActEditUngroup->setEnabled(false);

    QObject::connect(
        m_pActEditUngroup, &QAction::triggered,
        this, &CMainWindow::onActionEditUngroupTriggered );

    // <Menu> Draw
    //============

    // <Menu> Draw::Settings
    //----------------------

    // <MenuItem> Draw::Settings::Line
    //--------------------------------

    QIcon   iconEditDrawSettingsLine;
    QPixmap pxmEditDrawSettingsLine(":/ZS/Draw/DrawToolPen16x16.bmp");

    pxmEditDrawSettingsLine.setMask(pxmEditDrawSettingsLine.createHeuristicMask());

    iconEditDrawSettingsLine.addPixmap(pxmEditDrawSettingsLine);

    m_pActDrawSettingsLine = new QAction( iconEditDrawSettingsLine, c_strActionNameDrawSettingsLine.section(":",-1,-1), this );
    m_pActDrawSettingsLine->setStatusTip( tr("Draw Settings Line Style") );

    QObject::connect(
        m_pActDrawSettingsLine, &QAction::triggered,
        this, &CMainWindow::onActionDrawSettingsLineTriggered );

    // <MenuItem> Draw::Settings::Fill
    //---------------------------------

    QIcon   iconEditDrawSettingsFill;
    QPixmap pxmEditDrawSettingsFill(":/ZS/Draw/DrawToolBrush16x16.bmp");

    pxmEditDrawSettingsFill.setMask(pxmEditDrawSettingsFill.createHeuristicMask());

    iconEditDrawSettingsFill.addPixmap(pxmEditDrawSettingsFill);

    m_pActDrawSettingsFill = new QAction( iconEditDrawSettingsFill, c_strActionNameDrawSettingsFill.section(":",-1,-1), this );
    m_pActDrawSettingsFill->setStatusTip( tr("Draw Settings Fill Style") );

    QObject::connect(
        m_pActDrawSettingsFill, &QAction::triggered,
        this, &CMainWindow::onActionDrawSettingsFillTriggered );

    // <MenuItem> Draw::Settings::Text
    //---------------------------------

    QIcon   iconEditDrawSettingsText;
    QPixmap pxmEditDrawSettingsText(":/ZS/Draw/DrawToolText16x16.bmp");

    pxmEditDrawSettingsText.setMask(pxmEditDrawSettingsText.createHeuristicMask());

    iconEditDrawSettingsText.addPixmap(pxmEditDrawSettingsText);

    m_pActDrawSettingsText = new QAction( iconEditDrawSettingsText, c_strActionNameDrawSettingsText.section(":",-1,-1), this );
    m_pActDrawSettingsText->setStatusTip( tr("Draw Settings Text Style") );

    QObject::connect(
        m_pActDrawSettingsText, &QAction::triggered,
        this, &CMainWindow::onActionDrawSettingsTextTriggered );

    // <Menu> Draw::Standard Shapes
    //---------------------------------------------

    // <MenuItem> Draw::Standard Shapes::Draw Point
    //---------------------------------------------

    QIcon iconDrawPoint;
    QPixmap pxmDrawPoint(":/ZS/Draw/DrawPoint16x16.bmp");
    pxmDrawPoint.setMask(pxmDrawPoint.createHeuristicMask());
    iconDrawPoint.addPixmap(pxmDrawPoint);

    m_pActDrawStandardShapePoint = new QAction(iconDrawPoint, c_strActionNameDrawStandardShapePoint.section(":",-1,-1), this);
    m_pActDrawStandardShapePoint->setStatusTip(tr("Draw Points"));
    m_pActDrawStandardShapePoint->setCheckable(true);

    QObject::connect(
        m_pActDrawStandardShapePoint, &QAction::triggered,
        this, &CMainWindow::onActionDrawStandardShapePointToggled );

    // <MenuItem> Draw::Standard Shapes::Draw Line
    //--------------------------------------------

    QIcon iconDrawLine;
    QPixmap pxmDrawLine(":/ZS/Draw/DrawLine16x16.bmp");
    pxmDrawLine.setMask(pxmDrawLine.createHeuristicMask());
    iconDrawLine.addPixmap(pxmDrawLine);

    m_pActDrawStandardShapeLine = new QAction(iconDrawLine, c_strActionNameDrawStandardShapeLine.section(":",-1,-1), this);
    m_pActDrawStandardShapeLine->setStatusTip(tr("Draw Lines"));
    m_pActDrawStandardShapeLine->setCheckable(true);

    QObject::connect(
        m_pActDrawStandardShapeLine, &QAction::triggered,
        this, &CMainWindow::onActionDrawStandardShapeLineToggled );

    // <MenuItem> Draw::Standard Shapes::Draw Rect
    //--------------------------------------------

    QIcon iconDrawRect;
    QPixmap pxmDrawRect(":/ZS/Draw/DrawRect16x16.bmp");
    pxmDrawRect.setMask(pxmDrawRect.createHeuristicMask());
    iconDrawRect.addPixmap(pxmDrawRect);

    m_pActDrawStandardShapeRect = new QAction(iconDrawRect, c_strActionNameDrawStandardShapeRect.section(":",-1,-1), this);
    m_pActDrawStandardShapeRect->setStatusTip(tr("Draw Rectangles"));
    m_pActDrawStandardShapeRect->setCheckable(true);

    QObject::connect(
        m_pActDrawStandardShapeRect, &QAction::triggered,
        this, &CMainWindow::onActionDrawStandardShapeRectToggled );

    // <MenuItem> Draw::Standard Shapes::Draw Ellipse
    //-----------------------------------------------

    QIcon iconDrawEllipse;
    QPixmap pxmDrawEllipse(":/ZS/Draw/DrawEllipse16x16.bmp");
    pxmDrawEllipse.setMask(pxmDrawEllipse.createHeuristicMask());
    iconDrawEllipse.addPixmap(pxmDrawEllipse);

    m_pActDrawStandardShapeEllipse = new QAction(iconDrawEllipse, c_strActionNameDrawStandardShapeEllipse.section(":",-1,-1), this);
    m_pActDrawStandardShapeEllipse->setStatusTip(tr("Draw Ellipses"));
    m_pActDrawStandardShapeEllipse->setCheckable(true);

    QObject::connect(
        m_pActDrawStandardShapeEllipse, &QAction::triggered,
        this, &CMainWindow::onActionDrawStandardShapeEllipseToggled );

    // <MenuItem> Draw::Standard Shapes::Draw Polyline
    //------------------------------------------------

    QIcon iconDrawPolyline;
    QPixmap pxmDrawPolyline(":/ZS/Draw/DrawPolyline16x16.bmp");
    pxmDrawPolyline.setMask(pxmDrawPolyline.createHeuristicMask());
    iconDrawPolyline.addPixmap(pxmDrawPolyline);

    m_pActDrawStandardShapePolyline = new QAction(iconDrawPolyline, c_strActionNameDrawStandardShapePolyline.section(":",-1,-1), this);
    m_pActDrawStandardShapePolyline->setStatusTip(tr("Draw Polylines"));
    m_pActDrawStandardShapePolyline->setCheckable(true);

    QObject::connect(
        m_pActDrawStandardShapePolyline, &QAction::triggered,
        this, &CMainWindow::onActionDrawStandardShapePolylineToggled );

    // <MenuItem> Draw::Standard Shapes::Draw Polygon
    //------------------------------------------------

    QIcon iconDrawPolygon;
    QPixmap pxmDrawPolygon(":/ZS/Draw/DrawPolygon16x16.bmp");
    pxmDrawPolygon.setMask(pxmDrawPolygon.createHeuristicMask());
    iconDrawPolygon.addPixmap(pxmDrawPolygon);

    m_pActDrawStandardShapePolygon = new QAction(iconDrawPolygon, c_strActionNameDrawStandardShapePolygon.section(":",-1,-1), this);
    m_pActDrawStandardShapePolygon->setStatusTip(tr("Draw Polygons"));
    m_pActDrawStandardShapePolygon->setCheckable(true);

    QObject::connect(
        m_pActDrawStandardShapePolygon, &QAction::triggered,
        this, &CMainWindow::onActionDrawStandardShapePolygonToggled );

    // <MenuItem> Draw::Standard Shapes::Draw Text
    //------------------------------------------------

    QIcon iconDrawText;
    QPixmap pxmDrawText(":/ZS/Draw/DrawText16x16.bmp");
    pxmDrawText.setMask(pxmDrawText.createHeuristicMask());
    iconDrawText.addPixmap(pxmDrawText);

    m_pActDrawStandardShapeText = new QAction(iconDrawText, c_strActionNameDrawStandardShapeText.section(":",-1,-1), this);
    m_pActDrawStandardShapeText->setStatusTip(tr("Draw Texts"));
    m_pActDrawStandardShapeText->setCheckable(true);

    QObject::connect(
        m_pActDrawStandardShapeText, &QAction::triggered,
        this, &CMainWindow::onActionDrawStandardShapeTextToggled );

    // <Menu> Draw::Graphics
    //----------------------

    // <MenuItem> Draw::Graphics::Image
    //---------------------------------

    QIcon iconDrawImage;
    QPixmap pxmDrawImage(":/ZS/Draw/DrawImage16x16.bmp");
    pxmDrawImage.setMask(pxmDrawImage.createHeuristicMask());
    iconDrawImage.addPixmap(pxmDrawImage);

    m_pActDrawGraphicsImage = new QAction(iconDrawImage, c_strActionNameDrawGraphicsImage.section(":",-1,-1), this);
    m_pActDrawGraphicsImage->setStatusTip(tr("Insert Images"));
    m_pActDrawGraphicsImage->setCheckable(false);

    QObject::connect(
        m_pActDrawGraphicsImage, &QAction::triggered,
        this, &CMainWindow::onActionDrawGraphicsImageTriggered );

    // <Menu> Draw::Connections
    //-------------------------

    // <MenuItem> Draw::Connections::Draw Connection Point
    //----------------------------------------------------

    QIcon iconDrawConnectionPoint;
    QPixmap pxmDrawConnectionPoint(":/ZS/Draw/DrawConnectionPoint16x16.bmp");
    pxmDrawConnectionPoint.setMask(pxmDrawConnectionPoint.createHeuristicMask());
    iconDrawConnectionPoint.addPixmap(pxmDrawConnectionPoint);

    m_pActDrawConnectionPoint = new QAction(iconDrawConnectionPoint, c_strActionNameDrawConnectionPoint.section(":",-1,-1), this);
    m_pActDrawConnectionPoint->setStatusTip(tr("Draw Connection Point"));
    m_pActDrawConnectionPoint->setCheckable(true);

    QObject::connect(
        m_pActDrawConnectionPoint, &QAction::triggered,
        this, &CMainWindow::onActionDrawConnectionPointToggled );

    // <MenuItem> Draw::Connections::Draw Connection Line
    //----------------------------------------------------

    QIcon iconDrawConnectionLine;
    QPixmap pxmDrawConnectionLine(":/ZS/Draw/DrawConnectionLine16x16.bmp");
    pxmDrawConnectionLine.setMask(pxmDrawConnectionLine.createHeuristicMask());
    iconDrawConnectionLine.addPixmap(pxmDrawConnectionLine);

    m_pActDrawConnectionLine = new QAction(iconDrawConnectionLine, c_strActionNameDrawConnectionLine.section(":",-1,-1), this);
    m_pActDrawConnectionLine->setStatusTip(tr("Draw Connection Line"));
    m_pActDrawConnectionLine->setCheckable(true);

    QObject::connect(
        m_pActDrawConnectionLine, &QAction::triggered,
        this, &CMainWindow::onActionDrawConnectionLineToggled );

    // <Menu> View
    //============

    // <MenuItem> View::ZoomIn
    //------------------------

    QIcon iconViewZoomIn;
    QPixmap pxmViewZoomIn16x16(":/ZS/Button/ButtonZoomIn16x16.bmp");
    pxmViewZoomIn16x16.setMask(pxmViewZoomIn16x16.createHeuristicMask());
    iconViewZoomIn.addPixmap(pxmViewZoomIn16x16);

    m_pActViewZoomIn = new QAction(iconViewZoomIn, c_strActionNameViewZoomIn.section(":",-1,-1), this);
    m_pActViewZoomIn->setStatusTip( tr("Zoom In") );

    QObject::connect(
        m_pActViewZoomIn, &QAction::triggered,
        this, &CMainWindow::onActionViewZoomInTriggered );

    // <MenuItem> View::ZoomOut
    //-------------------------

    QIcon iconViewZoomOut;
    QPixmap pxmViewZoomOut16x16(":/ZS/Button/ButtonZoomOut16x16.bmp");
    pxmViewZoomOut16x16.setMask(pxmViewZoomOut16x16.createHeuristicMask());
    iconViewZoomOut.addPixmap(pxmViewZoomOut16x16);

    m_pActViewZoomOut = new QAction(iconViewZoomOut, c_strActionNameViewZoomOut.section(":",-1,-1), this);
    m_pActViewZoomOut->setStatusTip( tr("Zoom Out") );

    QObject::connect(
        m_pActViewZoomOut, &QAction::triggered,
        this, &CMainWindow::onActionViewZoomOutTriggered );

    // <Menu> Debug & Trace
    //=====================

    // <MenuItem> DebugTrace::Error Log
    //---------------------------------

    if( CErrLog::GetInstance() != nullptr )
    {
        m_pActDebugErrLog = new QAction(c_strActionNameDebugErrLog.section(":",-1,-1),this);

        QObject::connect(
            m_pActDebugErrLog, &QAction::triggered,
            this, &CMainWindow::onActionDebugErrLogTriggered );
    }

    // <MenuItem> DebugTrace::Units
    //---------------------------------

    m_pActDebugUnits = new QAction(c_strActionNameDebugUnits.section(":",-1,-1),this);

    QObject::connect(
        m_pActDebugUnits, &QAction::triggered,
        this, &CMainWindow::onActionDebugUnitsTriggered );

    // <MenuItem> DebugTrace::Trace Server
    //------------------------------------

    if( CTrcServer::GetInstance() != nullptr )
    {
        m_pActDebugTraceServer = new QAction(c_strActionNameDebugTraceServer.section(":",-1,-1),this);

        QObject::connect(
            m_pActDebugTraceServer, &QAction::triggered,
            this, &CMainWindow::onActionDebugTraceServerTriggered );
    }

    // <MenuItem> DebugTrace::Trace Admin Objects
    //-------------------------------------------

    if( CTrcServer::GetInstance() != nullptr )
    {
        m_pActDebugTraceAdminObjs = new QAction(c_strActionNameDebugTraceAdminObjs.section(":",-1,-1),this);

        QObject::connect(
            m_pActDebugTraceAdminObjs, &QAction::triggered,
            this, &CMainWindow::onActionDebugTraceAdminObjsTriggered );
    }

    // <MenuItem> DebugTrace::Test
    //-----------------------------------------------------------------------

    if( m_pTest != nullptr )
    {
        m_pActDebugTest = new QAction( "Subsystem Test", this );

        QObject::connect(
            m_pActDebugTest, &QAction::triggered,
            this, &CMainWindow::onActionDebugTestTriggered );
    }

    // <Menu> Info
    //=====================

    // <MenuItem> Info::Version
    //-------------------------

    QString strActionInfoVersion = "Version: " + ZS::System::c_strSysVersionNr;

    #ifdef _DEBUG
    strActionInfoVersion += " (Debug)";
    #endif

    m_pActInfoVersion = new QAction(strActionInfoVersion,this);

    // <MenuItem> Info::Settings File
    //-------------------------------

    //if( CApplication::GetInstance()->getSettingsFile() != nullptr )
    //{
    //    QString strActionInfoSettingsFile = "Settings File: " + CApplication::GetInstance()->getSettingsFile()->fileName();

    //    m_pActInfoSettingsFile = new QAction(strActionInfoSettingsFile,this);
    //}

} // createActions

//------------------------------------------------------------------------------
/*! Creates the menu items of the application.

    The actions used by the menu items must have been created before.
    @see createActions
*/
void CMainWindow::createMenus()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createMenus",
        /* strAddInfo   */ "" );

    m_pMenuBar = menuBar();

    // <Menu> File
    //------------

    m_pMenuFile = m_pMenuBar->addMenu(c_strMenuNameFile);

    m_pMenuFile->addAction(m_pActFileNew);
    m_pMenuFile->addAction(m_pActFileOpen);
    m_pMenuFile->addSeparator();
    m_pMenuFile->addAction(m_pActFileSave);
    m_pMenuFile->addAction(m_pActFileSaveAs);
    m_pMenuFile->addSeparator();
    m_pMenuFile->addAction(m_pActFilePageSetup);

    bool bHasActFilesRecent = false;
    for( int idxFile = 0; idxFile < _ZSArrLen(m_arpActFilesRecent); idxFile++ ) {
        if( m_arpActFilesRecent[idxFile] != nullptr ) {
            bHasActFilesRecent = true;
            break;
        }
    }
    if( bHasActFilesRecent ) {
        m_pActFilesRecentSeparator = m_pMenuFile->addSeparator();
        for( int idxFile = 0; idxFile < _ZSArrLen(m_arpActFilesRecent); idxFile++ ) {
            if( m_arpActFilesRecent[idxFile] != nullptr ) {
                m_pMenuFile->addAction(m_arpActFilesRecent[idxFile]);
            }
        }
    }

    if( m_pActFileQuit != nullptr ) {
        m_pMenuFile->addSeparator();
        m_pMenuFile->addAction(m_pActFileQuit);
    }

    // <Menu> Mode
    //------------

    m_pMenuMode = m_pMenuBar->addMenu(c_strMenuNameMode);

    if( m_pActModeEdit != nullptr ) {
        m_pMenuMode->addAction(m_pActModeEdit);
    }
    if( m_pActModeView != nullptr ) {
        m_pMenuMode->addAction(m_pActModeView);
    }

    // <Menu> Edit
    //------------

    m_pMenuEdit = m_pMenuBar->addMenu(c_strMenuNameEdit);

    if( m_pActEditSelect != nullptr ) {
        m_pMenuEdit->addAction(m_pActEditSelect);
    }

    m_pMenuEditRotate = m_pMenuEdit->addMenu(c_strMenuNameEditRotate.section(":",-1,-1));

    if( m_pActEditRotateLeft != nullptr ) {
        m_pMenuEditRotate->addAction(m_pActEditRotateLeft);
    }
    if( m_pActEditRotateRight != nullptr ) {
        m_pMenuEditRotate->addAction(m_pActEditRotateRight);
    }

    m_pMenuEditMirror = m_pMenuEdit->addMenu(c_strMenuNameEditMirror.section(":",-1,-1));

    if( m_pActEditMirrorVertical != nullptr ) {
        m_pMenuEditMirror->addAction(m_pActEditMirrorVertical);
    }
    if( m_pActEditMirrorHorizontal != nullptr ) {
        m_pMenuEditMirror->addAction(m_pActEditMirrorHorizontal);
    }

    if( m_pActEditGroup != nullptr ) {
        m_pMenuEdit->addAction(m_pActEditGroup);
    }
    if( m_pActEditUngroup != nullptr ) {
        m_pMenuEdit->addAction(m_pActEditUngroup);
    }

    // <Menu> Draw
    //------------

    m_pMenuDraw = m_pMenuBar->addMenu(c_strMenuNameDraw);

    m_pMenuDrawSettings = m_pMenuDraw->addMenu(c_strMenuNameDrawSettings.section(":",-1,-1));

    if( m_pActDrawSettingsLine != nullptr ) {
        m_pMenuDrawSettings->addAction(m_pActDrawSettingsLine);
    }
    if( m_pActDrawSettingsFill != nullptr ) {
        m_pMenuDrawSettings->addAction(m_pActDrawSettingsFill);
    }
    if( m_pActDrawSettingsText != nullptr ) {
        m_pMenuDrawSettings->addAction(m_pActDrawSettingsText);
    }

    m_pMenuDrawStandardShapes = m_pMenuDraw->addMenu(c_strMenuNameDrawStandardShapes.section(":",-1,-1));

    if( m_pActDrawStandardShapePoint != nullptr ) {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapePoint);
    }
    if( m_pActDrawStandardShapeLine != nullptr ) {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapeLine);
    }
    if( m_pActDrawStandardShapeRect != nullptr ) {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapeRect);
    }
    if( m_pActDrawStandardShapeEllipse != nullptr ) {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapeEllipse);
    }
    if( m_pActDrawStandardShapePolyline != nullptr ) {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapePolyline);
    }
    if( m_pActDrawStandardShapePolygon != nullptr ) {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapePolygon);
    }
    if( m_pActDrawStandardShapeText != nullptr ) {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapeText);
    }

    m_pMenuDrawGraphics = m_pMenuDraw->addMenu(c_strMenuNameDrawGraphics.section(":",-1,-1));

    if( m_pActDrawGraphicsImage != nullptr ) {
        m_pMenuDrawGraphics->addAction(m_pActDrawGraphicsImage);
    }

    m_pMenuDrawConnections = m_pMenuDraw->addMenu(c_strMenuNameDrawConnections.section(":",-1,-1));

    if( m_pActDrawConnectionPoint != nullptr ) {
        m_pMenuDrawConnections->addAction(m_pActDrawConnectionPoint);
    }
    if( m_pActDrawConnectionLine != nullptr ) {
        m_pMenuDrawConnections->addAction(m_pActDrawConnectionLine);
    }

    // <Menu> View
    //------------

    m_pMenuView = m_pMenuBar->addMenu(c_strMenuNameView);

    if( m_pActViewZoomIn != nullptr ) {
        m_pMenuView->addAction(m_pActViewZoomIn);
    }
    if( m_pActViewZoomOut != nullptr ) {
        m_pMenuView->addAction(m_pActViewZoomOut);
    }

    // <Menu> Debug & Trace
    //------------

    if( m_pActDebugErrLog != nullptr || m_pActDebugUnits != nullptr
     || m_pActDebugTraceServer != nullptr || m_pActDebugTraceAdminObjs != nullptr
     || m_pActDebugTest != nullptr )
    {
        m_pMenuDebug = m_pMenuBar->addMenu(c_strMenuNameDebug);

        if( m_pActDebugErrLog != nullptr ) {
            m_pMenuDebug->addAction(m_pActDebugErrLog);
        }
        if( m_pActDebugUnits != nullptr ) {
            m_pMenuDebug->addAction(m_pActDebugUnits);
        }
        if( m_pActDebugTraceServer != nullptr ) {
            m_pMenuDebug->addAction(m_pActDebugTraceServer);
        }
        if( m_pActDebugTraceAdminObjs != nullptr ) {
            m_pMenuDebug->addAction(m_pActDebugTraceAdminObjs);
        }
        if( m_pActDebugTest != nullptr ) {
            if( !m_pMenuDebug->isEmpty() ) {
                m_pMenuDebug->addSeparator();
            }
            m_pMenuDebug->addAction(m_pActDebugTest);
        }
    }

    // <Menu> Info
    //------------

    m_pMenuInfo = m_pMenuBar->addMenu(c_strMenuNameInfo);

    if( m_pActInfoVersion != nullptr ) {
        m_pMenuInfo->addAction(m_pActInfoVersion);
    }
    if( m_pActInfoSettingsFile != nullptr ) {
        m_pMenuInfo->addAction(m_pActInfoSettingsFile);
    }
} // createMenus

//------------------------------------------------------------------------------
/*! Creates the status bar of the application.
*/
void CMainWindow::createStatusBar()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createStatusBar",
        /* strAddInfo   */ "" );

    //CPageSetup* pageSetup = m_pDrawingView->getPageSetup();
    //CUnit unitWidth = pageSetup->unit(EDirection::Horizontal);

    //m_pLblStatusBarDrawingSceneEditTool = new QLabel("Tool: -");
    //m_pLblStatusBarDrawingSceneEditTool->setMinimumWidth(80);
    //statusBar()->addPermanentWidget(m_pLblStatusBarDrawingSceneEditTool);

    //m_pLblStatusBarDrawingSceneEditMode = new QLabel("Mode: ");
    //m_pLblStatusBarDrawingSceneEditMode->setMinimumWidth(100);
    //statusBar()->addPermanentWidget(m_pLblStatusBarDrawingSceneEditMode);

    //m_pLblStatusBarDrawingSceneGraphObjEditInfo = new QLabel("Edit: -");
    //m_pLblStatusBarDrawingSceneGraphObjEditInfo->setMinimumWidth(120);
    //statusBar()->addPermanentWidget(m_pLblStatusBarDrawingSceneGraphObjEditInfo);

    //m_pLblStatusBarDrawingSceneRect = new QLabel("SceneRect: -/-, -/- [" + unitWidth.symbol() + "]");
    //m_pLblStatusBarDrawingSceneRect->setMinimumWidth(160);
    //statusBar()->addPermanentWidget(m_pLblStatusBarDrawingSceneRect);

    //m_pLblStatusBarDrawingSceneMouseCursorPos = new QLabel( "ScenePos: -/- [" + unitWidth.symbol() + "]" );
    //m_pLblStatusBarDrawingSceneMouseCursorPos->setMinimumWidth(140);
    //statusBar()->addPermanentWidget(m_pLblStatusBarDrawingSceneMouseCursorPos);

    //m_pLblStatusBarDrawingViewMouseCursorPos = new QLabel( "ViewPos: -/- [" + unitWidth.symbol() + "]" );
    //m_pLblStatusBarDrawingViewMouseCursorPos->setMinimumWidth(140);
    //statusBar()->addPermanentWidget(m_pLblStatusBarDrawingViewMouseCursorPos);

    // <Label> Errors
    //---------------

    if( CErrLog::GetInstance() != nullptr ) {
        m_pLblErrors = new QLabel("Errors");
        m_pLblErrors->installEventFilter(this);
        statusBar()->addPermanentWidget(m_pLblErrors);
        m_pLblErrors->hide();

        updateErrorsStatus();

        QObject::connect(
            CErrLog::GetInstance(), &CErrLog::entryAdded,
            this, &CMainWindow::onErrLogEntryAdded );
        QObject::connect(
            CErrLog::GetInstance(), &CErrLog::entryChanged,
            this, &CMainWindow::onErrLogEntryChanged );
        QObject::connect(
            CErrLog::GetInstance(), &CErrLog::entryRemoved,
            this, &CMainWindow::onErrLogEntryRemoved );
    }
} // createStatusBar

//------------------------------------------------------------------------------
/*! Creates the tool bars of the application.

    The actions used by the toolbars must have been created before.
    @see createActions
*/
void CMainWindow::createToolBars()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createToolBars",
        /* strAddInfo   */ "" );

    // <Menu> File
    //------------

    m_pToolBarFile = addToolBar("File Operations");
    m_pToolBarFile->setObjectName("File Operations");
    //m_pToolBarFile->setMaximumHeight(24);
    m_pToolBarFile->setIconSize( QSize(16,16) );

    if( m_pActFileNew != nullptr ) {
        m_pToolBarFile->addAction(m_pActFileNew);
    }
    if( m_pActFileOpen != nullptr ) {
        m_pToolBarFile->addAction(m_pActFileOpen);
    }
    if( m_pActFileSave != nullptr ) {
        m_pToolBarFile->addAction(m_pActFileSave);
    }
    if( m_pActFileSaveAs != nullptr ) {
        m_pToolBarFile->addAction(m_pActFileSaveAs);
    }
    if( m_pActFilePageSetup != nullptr ) {
        m_pToolBarFile->addAction(m_pActFilePageSetup);
    }

    // <Menu> Mode
    //------------

    m_pToolBarMode = addToolBar("Switching Between View and Edit Mode");
    m_pToolBarMode->setObjectName("Mode");
    //m_pToolBarMode->setMaximumHeight(24);
    m_pToolBarMode->setIconSize( QSize(16,16) );

    if( m_pActModeEdit != nullptr ) {
        m_pToolBarMode->addAction(m_pActModeEdit);
    }
    if( m_pActModeView != nullptr ) {
        m_pToolBarMode->addAction(m_pActModeView);
    }

    // <Menu> Edit
    //------------

    m_pToolBarEditSelect = addToolBar("Edit Select and Rotate Free");
    m_pToolBarEditSelect->setObjectName("Edit Select and Rotate Free");
    //m_pToolBarEditSelect->setMaximumHeight(24);
    m_pToolBarEditSelect->setIconSize( QSize(16,16) );

    if( m_pActEditSelect != nullptr ) {
        m_pToolBarEditSelect->addAction(m_pActEditSelect);
    }

    m_pToolBarEditRotateAndMirror = addToolBar("Edit Rotate and Mirror");
    m_pToolBarEditRotateAndMirror->setObjectName("Edit Rotate and Mirror");
    m_pToolBarEditRotateAndMirror->setIconSize( QSize(16,16) );

    if( m_pActEditRotateLeft != nullptr ) {
        m_pToolBarEditRotateAndMirror->addAction(m_pActEditRotateLeft);
    }
    if( m_pActEditRotateRight != nullptr ) {
        m_pToolBarEditRotateAndMirror->addAction(m_pActEditRotateRight);
    }

    m_pEdtEditRotateAngle = new QDoubleSpinBox();
    m_pEdtEditRotateAngle->setRange(0.0,360.0);
    m_pEdtEditRotateAngle->setDecimals(1);
    m_pEdtEditRotateAngle->setSingleStep(1.0);
    m_pEdtEditRotateAngle->setSuffix(" " +  ZS::PhysVal::c_strSymbolDegree);
    m_pEdtEditRotateAngle->setValue(90.0);
    m_pToolBarEditRotateAndMirror->addWidget(m_pEdtEditRotateAngle);

    if( m_pActEditMirrorVertical != nullptr ) {
        m_pToolBarEditRotateAndMirror->addAction(m_pActEditMirrorVertical);
    }
    if( m_pActEditMirrorHorizontal != nullptr ) {
        m_pToolBarEditRotateAndMirror->addAction(m_pActEditMirrorHorizontal);
    }

    m_pToolBarEditGroup = addToolBar("Edit Group and Ungroup");
    m_pToolBarEditGroup->setObjectName("Edit Group and Ungroup");
    //m_pToolBarEditGroup->setMaximumHeight(24);
    m_pToolBarEditGroup->setIconSize( QSize(16,16) );

    if( m_pActEditGroup != nullptr ) {
        m_pToolBarEditGroup->addAction(m_pActEditGroup);
    }
    if( m_pActEditGroup != nullptr ) {
        m_pToolBarEditGroup->addAction(m_pActEditUngroup);
    }

    // <Menu> Draw
    //------------

    m_pToolBarDrawSettings = addToolBar("Draw Settings");
    m_pToolBarDrawSettings->setObjectName("Draw Settings");
    //m_pToolBarDrawSettings->setMaximumHeight(24);
    m_pToolBarDrawSettings->setIconSize( QSize(16,16) );

    if( m_pActDrawSettingsLine != nullptr ) {
        m_pToolBarDrawSettings->addAction(m_pActDrawSettingsLine);
    }
    if( m_pActDrawSettingsFill != nullptr ) {
        m_pToolBarDrawSettings->addAction(m_pActDrawSettingsFill);
    }
    if( m_pActDrawSettingsText != nullptr ) {
        m_pToolBarDrawSettings->addAction(m_pActDrawSettingsText);
    }

    m_pToolBarDrawStandardShapes = addToolBar("Draw Standard Shapes");
    m_pToolBarDrawStandardShapes->setObjectName("Draw Standard Shapes");
    //m_pToolBarDrawStandardShapes->setMaximumHeight(24);
    m_pToolBarDrawStandardShapes->setIconSize( QSize(16,16) );

    if( m_pActDrawStandardShapePoint != nullptr ) {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapePoint);
    }
    if( m_pActDrawStandardShapeLine != nullptr ) {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapeLine);
    }
    if( m_pActDrawStandardShapeRect != nullptr ) {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapeRect);
    }
    if( m_pActDrawStandardShapeEllipse != nullptr ) {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapeEllipse);
    }
    if( m_pActDrawStandardShapePolyline != nullptr ) {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapePolyline);
    }
    if( m_pActDrawStandardShapePolygon != nullptr ) {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapePolygon);
    }
    if( m_pActDrawStandardShapeText != nullptr ) {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapeText);
    }

    m_pToolBarDrawGraphics = addToolBar("Draw Graphics");
    m_pToolBarDrawGraphics->setObjectName("Draw Graphics");
    //m_pToolBarDrawGraphics->setMaximumHeight(24);
    m_pToolBarDrawGraphics->setIconSize( QSize(16,16) );

    if( m_pActDrawGraphicsImage != nullptr ) {
        m_pToolBarDrawGraphics->addAction(m_pActDrawGraphicsImage);
    }

    m_pToolBarDrawConnections = addToolBar("Draw Connections");
    m_pToolBarDrawConnections->setObjectName("Draw Connections");
    //m_pToolBarDrawConnections->setMaximumHeight(24);
    m_pToolBarDrawConnections->setIconSize( QSize(16,16) );

    if( m_pActDrawConnectionPoint != nullptr ) {
        m_pToolBarDrawConnections->addAction(m_pActDrawConnectionPoint);
    }
    if( m_pActDrawConnectionLine != nullptr ) {
        m_pToolBarDrawConnections->addAction(m_pActDrawConnectionLine);
    }

    // <Menu> View
    //------------

    m_pToolBarView = addToolBar("View");
    m_pToolBarView->setObjectName("View");
    //m_pToolBarView->setMaximumHeight(24);
    m_pToolBarView->setIconSize( QSize(16,16) );

    if( m_pActViewZoomIn != nullptr ) {
        m_pToolBarView->addAction(m_pActViewZoomIn);
    }
    if( m_pActViewZoomOut != nullptr ) {
        m_pToolBarView->addAction(m_pActViewZoomOut);
    }
    if( m_pActViewZoomIn != nullptr || m_pActViewZoomOut != nullptr ) {
        m_pEdtViewZoomFactor_perCent = new QSpinBox();
        m_pEdtViewZoomFactor_perCent->setMinimumWidth(80);
        m_pEdtViewZoomFactor_perCent->setMinimum(1);
        m_pEdtViewZoomFactor_perCent->setMaximum(10000);
        m_pEdtViewZoomFactor_perCent->setSuffix(" %");
        m_pEdtViewZoomFactor_perCent->setValue(m_iViewZoomFactor_perCent);
        m_pToolBarView->addWidget(m_pEdtViewZoomFactor_perCent);
        QObject::connect(
            m_pEdtViewZoomFactor_perCent, &QSpinBox::editingFinished,
            this, &CMainWindow::onEdtViewZoomFactorEditingFinished );
    }
} // createToolBars

//------------------------------------------------------------------------------
void CMainWindow::createDockWidgets()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createDockWidgets",
        /* strAddInfo   */ "" );

    QSettings settings;

    CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    // <DockWidget> Tree View Object Factories
    //----------------------------------------

    if( m_uAddObjFactories != EAddObjFactoriesNone ) {
        m_pDockWdgtObjFactories = new QDockWidget("Tool Box");
        m_pDockWdgtObjFactories->setObjectName("Object Factories");
        m_pDockWdgtObjFactories->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

        m_pModelObjFactories = new CObjFactoriesModel( CObjFactory::IdxTree() );
        m_pTreeViewObjFactories = new QTreeView(this);

        m_pTreeViewObjFactories->setModel(m_pModelObjFactories);
        m_pTreeViewObjFactories->setSelectionBehavior(QAbstractItemView::SelectItems);
        m_pTreeViewObjFactories->setSelectionMode(QAbstractItemView::SingleSelection);
        //m_pTreeViewObjFactories->setAllColumnsShowFocus(true);
        m_pTreeViewObjFactories->setDragEnabled(true);
        m_pTreeViewObjFactories->setDropIndicatorShown(true);
        // Please note that the first column should not be hidden to allow
        // collapsing and expanding the tree view.
        m_pTreeViewObjFactories->showColumn(CObjFactoriesModel::EColumnNodeName);

        m_pDockWdgtObjFactories->setWidget(m_pTreeViewObjFactories);

        addDockWidget(Qt::LeftDockWidgetArea, m_pDockWdgtObjFactories);

        QObject::connect(
            m_pTreeViewObjFactories, &QTreeView::expanded,
            this, &CMainWindow::onTreeViewObjFactoriesExpanded );
        QObject::connect(
            m_pTreeViewObjFactories->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &CMainWindow::onTreeViewObjFactoriesCurrentChanged );

        if( m_pMenuView != nullptr ) {
            if( !m_pMenuView->isEmpty() ) {
                m_pMenuView->addSeparator();
            }
            m_pMenuView->addAction(m_pDockWdgtObjFactories->toggleViewAction());
        }
    } // if( m_uAddObjFactories != EAddObjFactoriesNone )

    // Dock Widget - GraphObjs
    //------------------------

    m_pDockWdgtGraphObjs = new QDockWidget("Graphical Objects");
    m_pDockWdgtGraphObjs->setObjectName("Graphical Objects");
    m_pDockWdgtGraphObjs->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

    // Tree View with graphics items as in drawing scene's items list
    m_pWdgtGraphObjs = new CWdgtGraphObjs(pDrawingView);
    m_pDockWdgtGraphObjs->setWidget(m_pWdgtGraphObjs);

    addDockWidget(Qt::RightDockWidgetArea, m_pDockWdgtGraphObjs);

    m_pWdgtGraphObjs->restoreState(settings);

    if( m_pMenuView != nullptr ) {
        if( !m_pMenuView->isEmpty() ) {
            m_pMenuView->addSeparator();
        }
        m_pMenuView->addAction(m_pDockWdgtGraphObjs->toggleViewAction());
    }
} // createDockWidgets

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
bool CMainWindow::eventFilter( QObject* i_pObjWatched, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Obj: " + QString(i_pObjWatched == nullptr ? "nullptr" : i_pObjWatched->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "eventFilter",
        /* strAddInfo   */ strMthInArgs );

    bool bHandled = false;

    if( i_pObjWatched == m_pLblErrors )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            onActionDebugErrLogTriggered(false);
            bHandled = true;
        }
    }
    else
    {
        // pass the event on to the parent class
        bHandled = QMainWindow::eventFilter(i_pObjWatched,i_pEv);
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        mthTracer.setMethodReturn(bHandled);
    }

    return bHandled;

} // eventFilter

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionModeEdit( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionModeEdit",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pActModeEdit != nullptr )
    {
        m_pActModeEdit->setChecked(i_bChecked);
    }

} // setCheckedActionModeEdit

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionModeView( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionModeView",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pActModeView != nullptr )
    {
        m_pActModeView->setChecked(i_bChecked);
    }

} // setCheckedActionModeView

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionEditSelect( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionEditSelect",
        /* strAddInfo   */ strMthInArgs );

    if( m_pActEditSelect != nullptr )
    {
        m_pActEditSelect->setChecked(i_bChecked);
    }

} // setCheckedActionEditSelect

//------------------------------------------------------------------------------
void CMainWindow::triggerActionEditRotateLeft()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionEditRotateLeft",
        /* strAddInfo   */ "" );

    if( m_pActEditRotateLeft != nullptr )
    {
        m_pActEditRotateLeft->trigger();
    }

} // triggerActionEditRotateLeft

//------------------------------------------------------------------------------
void CMainWindow::triggerActionEditRotateRight()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionEditRotateRight",
        /* strAddInfo   */ "" );

    if( m_pActEditRotateRight != nullptr )
    {
        m_pActEditRotateRight->trigger();
    }

} // triggerActionEditRotateRight

//------------------------------------------------------------------------------
void CMainWindow::triggerActionEditMirrorVertical()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionEditMirrorVertical",
        /* strAddInfo   */ "" );

    if( m_pActEditMirrorVertical != nullptr )
    {
        m_pActEditMirrorVertical->trigger();
    }

} // triggerActionEditMirrorVertical

//------------------------------------------------------------------------------
void CMainWindow::triggerActionEditMirrorHorizontal()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionEditMirrorHorizontal",
        /* strAddInfo   */ "" );

    if( m_pActEditMirrorHorizontal != nullptr )
    {
        m_pActEditMirrorHorizontal->trigger();
    }

} // triggerActionEditMirrorHorizontal

//------------------------------------------------------------------------------
void CMainWindow::triggerActionEditGroup()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionEditGroup",
        /* strAddInfo   */ "" );

    if( m_pActEditGroup != nullptr )
    {
        m_pActEditGroup->trigger();
    }

} // triggerActionEditGroup

//------------------------------------------------------------------------------
void CMainWindow::triggerActionEditUngroup()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionEditUngroup",
        /* strAddInfo   */ "" );

    if( m_pActEditUngroup != nullptr )
    {
        m_pActEditUngroup->trigger();
    }

} // triggerActionEditUngroup

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapePoint( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapePoint",
        /* strAddInfo   */ strMthInArgs );

    if( m_pActDrawStandardShapePoint != nullptr )
    {
        m_pActDrawStandardShapePoint->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapePoint

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapeLine( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapeLine",
        /* strAddInfo   */ strMthInArgs );

    if( m_pActDrawStandardShapeLine != nullptr )
    {
        m_pActDrawStandardShapeLine->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapeLine

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapeRect( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapeRect",
        /* strAddInfo   */ strMthInArgs );

    if( m_pActDrawStandardShapeRect != nullptr )
    {
        m_pActDrawStandardShapeRect->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapeRect

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapeEllipse( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapeEllipse",
        /* strAddInfo   */ strMthInArgs );

    if( m_pActDrawStandardShapeEllipse != nullptr )
    {
        m_pActDrawStandardShapeEllipse->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapeEllipse

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapePolyline( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapePolyline",
        /* strAddInfo   */ strMthInArgs );

    if( m_pActDrawStandardShapePolyline != nullptr )
    {
        m_pActDrawStandardShapePolyline->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapePolyline

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapePolygon( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapePolygon",
        /* strAddInfo   */ strMthInArgs );

    if( m_pActDrawStandardShapePolygon != nullptr )
    {
        m_pActDrawStandardShapePolygon->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapePolygon

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapeText( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapeText",
        /* strAddInfo   */ strMthInArgs );

    if( m_pActDrawStandardShapeText != nullptr )
    {
        m_pActDrawStandardShapeText->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapeText

//------------------------------------------------------------------------------
void CMainWindow::triggerActionDrawGraphicsImage()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionDrawGraphicsImage",
        /* strAddInfo   */ "" );

    if( m_pActDrawGraphicsImage != nullptr )
    {
        m_pActDrawGraphicsImage->trigger();
    }

} // triggerActionDrawGraphicsImage

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawConnectionPoint( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawConnectionPoint",
        /* strAddInfo   */ strMthInArgs );

    if( m_pActDrawConnectionPoint != nullptr )
    {
        m_pActDrawConnectionPoint->setChecked(i_bChecked);
    }

} // setCheckedActionDrawConnectionPoint

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawConnectionLine( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawConnectionLine",
        /* strAddInfo   */ strMthInArgs );

    if( m_pActDrawConnectionLine != nullptr )
    {
        m_pActDrawConnectionLine->setChecked(i_bChecked);
    }

} // setCheckedActionDrawConnectionLine

/*==============================================================================
public slots: // Menu - File
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionFileNewTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFileNewTriggered",
        /* strAddInfo   */ "" );

    m_pWdgtCentral->drawingScene()->clear();
    setCurrentFile("");
}

//------------------------------------------------------------------------------
void CMainWindow::onActionFileOpenTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFileOpenTriggered",
        /* strAddInfo   */ "" );

    QString strFileName = QFileDialog::getOpenFileName(
        /* pWdgtParent */ this,
        /* strCaption  */ "Choose a file to open",
        /* strDir      */ "",
        /* strFilter   */ "Drawings (*.xml)" );

    if( !strFileName.isEmpty() ) {
        CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

        // Clear drawing on opening a new file.
        pDrawingScene->clear();

        SErrResultInfo errResultInfo = pDrawingScene->load(strFileName);
        if( !errResultInfo.isErrorResult() ) {
            setCurrentFile(strFileName);
        }
        else {
            QString strMsg;
            strMsg  = "Error on reading file \"" + strFileName + "\"";
            strMsg += "\n\nErrorCode:\t" + errResultInfo.getResultStr();
            strMsg += "\n\n" + errResultInfo.getAddErrInfoDscr();

            if( errResultInfo.getSeverity() == EResultSeverityCritical ) {
                QMessageBox::critical(
                    /* pWdgtParent */ this,
                    /* strTitly    */ windowTitle(),
                    /* strText     */ strMsg,
                    /* buttons     */ QMessageBox::Ok );
            }
            else {
                QMessageBox::warning(
                    /* pWdgtParent */ this,
                    /* strTitly    */ windowTitle(),
                    /* strText     */ strMsg,
                    /* buttons     */ QMessageBox::Ok );
            }
        }
    }
} // onActionFileOpenTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionFileSaveTriggered( bool )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "FileName:" + m_strCurrentFile;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFileSaveTriggered",
        /* strAddInfo   */ strMthInArgs );

    if( m_strCurrentFile.isEmpty() ) {
        onActionFileSaveAsTriggered(true);
    }
    else {
        CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
        pDrawingScene->save(m_strCurrentFile);
    }
} // onActionFileSaveTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionFileSaveAsTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFileSaveAsTriggered",
        /* strAddInfo   */ "" );

    QString strFileName = QFileDialog::getSaveFileName(
        /* pWdgtParent */ this,
        /* strCaption  */ "Choose a filename to save under",
        /* strDir      */ "",
        /* strFilter   */ "XML File (*.xml);; Image (*.png);; Image (*.jpg);; Image (*.bmp)" );

    if( !strFileName.isEmpty() ) {
        CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
        CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
        if( strFileName.toLower().endsWith(".xml") ) {
            pDrawingScene->save(strFileName);
            setCurrentFile(strFileName);
        }
        else {
            CDrawingSize drawingSize = pDrawingView->drawingSize();
            QImage img(
                drawingSize.imageSizeInPixels().width(),
                drawingSize.imageSizeInPixels().height(),
                QImage::Format_ARGB32_Premultiplied);
            QPainter painter(&img);
            pDrawingScene->render(&painter);
            painter.end();
            img.save(strFileName);
        }
    }
} // onActionFileSaveAsTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionFilePageSetupTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFilePageSetupTriggered",
        /* strAddInfo   */ "" );

    CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
    QString strDlgTitle = QCoreApplication::applicationName() + ": Page Setup";
    CDlgDrawingViewSetup* pDlg = CDlgDrawingViewSetup::GetInstance(pDrawingView);
    if( pDlg == nullptr ) {
        pDlg = CDlgDrawingViewSetup::CreateInstance(strDlgTitle, pDrawingView);
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

//------------------------------------------------------------------------------
void CMainWindow::onActionFileRecentTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFileRecentTriggered",
        /* strAddInfo   */ "" );

    //QAction* pAct = dynamic_cast<QAction*>(sender());
    //if( m_pDrawPaper != nullptr && pAct != nullptr ) {
    //    m_pDrawPaper->fileOpen( pAct->data().toString() );
    //    setCurrentFile( m_pDrawPaper->getFileName() );
    //}
} // onActionFileRecentTriggered

/*==============================================================================
public slots: // Menu - Mode
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionModeEditToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionModeEditToggled",
        /* strAddInfo   */ "" );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    if( i_bChecked && pDrawingScene->getMode() != EMode::Edit ) {
        pDrawingScene->setCurrentDrawingTool(nullptr);
        pDrawingScene->setMode(EMode::Edit);
    }
    else if( !i_bChecked && pDrawingScene->getMode() != EMode::View ) {
        pDrawingScene->setCurrentDrawingTool(nullptr);
        pDrawingScene->setMode(EMode::View);
    }
} // onActionModeEditToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionModeViewToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionModeViewToggled",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    if( i_bChecked && pDrawingScene->getMode() != EMode::View ) {
        pDrawingScene->setCurrentDrawingTool(nullptr);
        pDrawingScene->setMode(EMode::View);
    }
    else if( !i_bChecked && pDrawingScene->getMode() != EMode::Edit ) {
        pDrawingScene->setCurrentDrawingTool(nullptr);
        pDrawingScene->setMode(EMode::Edit);
    }
} // onActionModeViewToggled

/*==============================================================================
public slots: // Menu - Edit - Select/RotateFree
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionEditSelectToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditSelectToggled",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    if( i_bChecked ) {
        if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActEditSelect ) {
            m_pActDrawChecked->setChecked(false);
        }
        m_pActDrawChecked = m_pActEditSelect;
        pDrawingScene->setCurrentDrawingTool(nullptr);
        pDrawingScene->setMode(
            EMode::Ignore, EEditTool::Select, EEditMode::None, EEditResizeMode::None);
    }
    else {
        if( m_pActDrawChecked == m_pActEditSelect ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getEditTool() == EEditTool::Select ) {
            pDrawingScene->setMode(
                EMode::Ignore, EEditTool::None, EEditMode::None, EEditResizeMode::None);
        }
    }
} // onActionEditSelectToggled

/*==============================================================================
public slots: // Menu - Edit - Rotate
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionEditRotateLeftTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditRotateLeftTriggered",
        /* strAddInfo   */ "" );

    double fAngle_deg = 90.0;
    if( m_pEdtEditRotateAngle != nullptr ) {
        fAngle_deg = m_pEdtEditRotateAngle->value();
    }

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    QList<QGraphicsItem*> arpGraphicsItemsSelected = pDrawingScene->selectedItems();

    // If graphical objects are selected ...
    if( arpGraphicsItemsSelected.size() > 0 ) {
        // .. all selected graphical objects will be changed according to the new settings.
        for( int idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ ) {
            QGraphicsItem* pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
            if( pGraphObj != nullptr ) {
                if( !pGraphObj->isLabel() && !pGraphObj->isSelectionPoint() ) {
                    double fAngleTmp_deg = pGraphObj->getRotationAngleInDegree();
                    pGraphObj->setRotationAngleInDegree(fAngleTmp_deg+fAngle_deg);
                }
            }
        }
    }
} // onActionEditRotateLeftTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionEditRotateRightTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditRotateRightTriggered",
        /* strAddInfo   */ "" );

    double fAngle_deg = -90.0;
    if( m_pEdtEditRotateAngle != nullptr ) {
        fAngle_deg = -m_pEdtEditRotateAngle->value();
    }

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    QList<QGraphicsItem*> arpGraphicsItemsSelected = pDrawingScene->selectedItems();

    // If graphical objects are selected ...
    if( arpGraphicsItemsSelected.size() > 0 ) {
        // .. all selected graphical objects will be changed according to the new settings.
        for( int idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ ) {
            QGraphicsItem* pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
            if( pGraphObj != nullptr ) {
                if( !pGraphObj->isLabel() && !pGraphObj->isSelectionPoint() ) {
                    double fAngleTmp_deg = pGraphObj->getRotationAngleInDegree();
                    pGraphObj->setRotationAngleInDegree(fAngleTmp_deg+fAngle_deg);
                }
            }
        }
    }
} // onActionEditRotateRightTriggered

/*==============================================================================
public slots: // Menu - Edit - Mirror
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionEditMirrorVerticalTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditMirrorVerticalTriggered",
        /* strAddInfo   */ "" );

    //graphicsItem->scale(1,-1);

    QMessageBox::information(
        /* pWdgtParent */ this,
        /* strTitle    */ QApplication::applicationName() + ": Unsupported Feature",
        /* strText     */ "Sorry but mirroring is not yet supported." );

} // onActionEditMirrorVerticalTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionEditMirrorHorizontalTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditMirrorHorizontalTriggered",
        /* strAddInfo   */ "" );

    //graphicsItem->scale(-1,1);

    QMessageBox::information(
        /* pWdgtParent */ this,
        /* strTitle    */ QApplication::applicationName() + ": Unsupported Feature",
        /* strText     */ "Sorry but mirroring is not yet supported." );

} // onActionEditMirrorHorizontalTriggered

/*==============================================================================
public slots: // Menu - Edit - Group
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionEditGroupTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditGroupTriggered",
        /* strAddInfo   */ "" );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    pDrawingScene->groupGraphObjsSelected();

} // onActionEditGroupTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionEditUngroupTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditUngroupTriggered",
        /* strAddInfo   */ "" );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    pDrawingScene->ungroupGraphObjsSelected();

} // onActionEditUngroupTriggered

/*==============================================================================
public slots: // Menu - Draw - Settings
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawSettingsLineTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawSettingsLineTriggered",
        /* strAddInfo   */ "" );

    CDlgFormatGraphObjs* pDlgFormatGraphObjs = nullptr;
    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    // If no graphical object is selected ...
    if( pDrawingScene->selectedItems().size() == 0 ) {
        // .. set default drawing attribute for following draw commands.
        pDlgFormatGraphObjs = new CDlgFormatGraphObjs(pDrawingScene);
    }
    // If one specific graphical object is selected ...
    else if( pDrawingScene->selectedItems().size() == 1 ) {
        // .. set drawing attributes for this object.
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pDrawingScene->selectedItems()[0]);
        if( pGraphObj != nullptr ) {
            pDlgFormatGraphObjs = new CDlgFormatGraphObjs(pDrawingScene,pGraphObj);
        }
    }
    // If several graphical objects are selected ...
    else { // if( m_pDrawingScene->selectedItems().size() > 1 )
        // .. set drawing attributes for those object.
        pDlgFormatGraphObjs = new CDlgFormatGraphObjs(pDrawingScene);
    }

    if( pDlgFormatGraphObjs != nullptr ) {
        pDlgFormatGraphObjs->setCurrentWidget(CDlgFormatGraphObjs::c_strWdgtLineStyle);
        pDlgFormatGraphObjs->exec();
        delete pDlgFormatGraphObjs;
        pDlgFormatGraphObjs = nullptr;
    }
} // onActionDrawSettingsLineTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawSettingsFillTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawSettingsFillTriggered",
        /* strAddInfo   */ "" );

    CDlgFormatGraphObjs* pDlgFormatGraphObjs = nullptr;
    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    // If no graphical object is selected ...
    if( pDrawingScene->selectedItems().size() == 0 ) {
        // .. set default drawing attribute for following draw commands.
        pDlgFormatGraphObjs = new CDlgFormatGraphObjs(pDrawingScene);
    }
    // If one specific graphical object is selected ...
    else if( pDrawingScene->selectedItems().size() == 1 )
    {
        // .. set drawing attributes for this object.
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pDrawingScene->selectedItems()[0]);
        if( pGraphObj != nullptr ) {
            pDlgFormatGraphObjs = new CDlgFormatGraphObjs(pDrawingScene,pGraphObj);
        }
    }
    // If several graphical objects are selected ...
    else { // if( m_pDrawingScene->selectedItems().size() > 1 )
        // .. set drawing attributes for those object.
        pDlgFormatGraphObjs = new CDlgFormatGraphObjs(pDrawingScene);
    }

    if( pDlgFormatGraphObjs != nullptr ) {
        pDlgFormatGraphObjs->setCurrentWidget(CDlgFormatGraphObjs::c_strWdgtFillStyle);
        pDlgFormatGraphObjs->exec();
        delete pDlgFormatGraphObjs;
        pDlgFormatGraphObjs = nullptr;
    }
} // onActionDrawSettingsFillTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawSettingsTextTriggered( bool )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawSettingsTextTriggered",
        /* strAddInfo   */ "" );

    CDlgFormatGraphObjs* pDlgFormatGraphObjs = nullptr;
    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    // If no graphical object is selected ...
    if( pDrawingScene->selectedItems().size() == 0 ) {
        // .. set default drawing attribute for following draw commands.
        pDlgFormatGraphObjs = new CDlgFormatGraphObjs(pDrawingScene);
    }
    // If one specific graphical object is selected ...
    else if( pDrawingScene->selectedItems().size() == 1 ) {
        // .. set drawing attributes for this object.
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pDrawingScene->selectedItems()[0]);
        if( pGraphObj != nullptr ) {
            pDlgFormatGraphObjs = new CDlgFormatGraphObjs(pDrawingScene,pGraphObj);
        }
    }
    // If several graphical objects are selected ...
    else { // if( m_pDrawingScene->selectedItems().size() > 1 )
        // .. set drawing attributes for those object.
        pDlgFormatGraphObjs = new CDlgFormatGraphObjs(pDrawingScene);
    }

    if( pDlgFormatGraphObjs != nullptr ) {
        pDlgFormatGraphObjs->setCurrentWidget(CDlgFormatGraphObjs::c_strWdgtTextStyle);
        pDlgFormatGraphObjs->exec();
        delete pDlgFormatGraphObjs;
        pDlgFormatGraphObjs = nullptr;
    }
} // onActionDrawSettingsTextTriggered

/*==============================================================================
public slots: // Menu - Draw - Standard Shapes
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapePointToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawStandardShapePointToggled",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    if( i_bChecked ) {
        if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapePoint ) {
            m_pActDrawChecked->setChecked(false);
        }
        m_pActDrawChecked = m_pActDrawStandardShapePoint;
        pDrawingScene->setCurrentDrawingTool(m_pObjFactoryPoint);
    }
    else { // if( !i_bChecked )
        if( m_pActDrawChecked == m_pActDrawStandardShapePoint ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPoint ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
} // onActionDrawStandardShapePointToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapeLineToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawStandardShapeLineToggled",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    if( i_bChecked ) {
        if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapeLine ) {
            m_pActDrawChecked->setChecked(false);
        }
        m_pActDrawChecked = m_pActDrawStandardShapeLine;
        pDrawingScene->setCurrentDrawingTool(m_pObjFactoryLine);
    }
    else { // if( !i_bChecked )
        if( m_pActDrawChecked == m_pActDrawStandardShapeLine ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryLine ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
} // onActionDrawStandardShapeLineToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapeRectToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawStandardShapeRectToggled",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    if( i_bChecked ) {
        if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapeRect ) {
            m_pActDrawChecked->setChecked(false);
        }
        m_pActDrawChecked = m_pActDrawStandardShapeRect;
        pDrawingScene->setCurrentDrawingTool(m_pObjFactoryRect);
    }
    else { // if( !i_bChecked )
        if( m_pActDrawChecked == m_pActDrawStandardShapeRect ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryRect ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
} // onActionDrawStandardShapeRectToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapeEllipseToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawStandardShapeEllipseToggled",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    if( i_bChecked ) {
        if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapeEllipse ) {
            m_pActDrawChecked->setChecked(false);
        }
        m_pActDrawChecked = m_pActDrawStandardShapeEllipse;
        pDrawingScene->setCurrentDrawingTool(m_pObjFactoryEllipse);
    }
    else { // if( !i_bChecked )
        if( m_pActDrawChecked == m_pActDrawStandardShapeEllipse ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryEllipse ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
} // onActionDrawStandardShapeEllipseToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapePolylineToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawStandardShapePolylineToggled",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    if( i_bChecked ) {
        if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapePolyline ) {
            m_pActDrawChecked->setChecked(false);
        }
        m_pActDrawChecked = m_pActDrawStandardShapePolyline;
        pDrawingScene->setCurrentDrawingTool(m_pObjFactoryPolyline);
    }
    else { // if( !i_bChecked )
        if( m_pActDrawChecked == m_pActDrawStandardShapePolyline ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPolyline ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
} // onActionDrawStandardShapePolylineToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapePolygonToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawStandardShapePolygonToggled",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    if( i_bChecked ) {
        if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapePolygon ) {
            m_pActDrawChecked->setChecked(false);
        }
        m_pActDrawChecked = m_pActDrawStandardShapePolygon;
        pDrawingScene->setCurrentDrawingTool(m_pObjFactoryPolygon);
    }
    else { // if( !i_bChecked )
        if( m_pActDrawChecked == m_pActDrawStandardShapePolygon ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPolygon ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
} // onActionDrawStandardShapePolygonToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapeTextToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawStandardShapeTextToggled",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    if( i_bChecked ) {
        if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapeText ) {
            m_pActDrawChecked->setChecked(false);
        }
        m_pActDrawChecked = m_pActDrawStandardShapeText;
        pDrawingScene->setCurrentDrawingTool(m_pObjFactoryText);
    }
    else { // if( !i_bChecked )
        if( m_pActDrawChecked == m_pActDrawStandardShapeText ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryText ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
} // onActionDrawStandardShapeTextToggled

/*==============================================================================
public slots: // Menu - Draw - Graphics
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawGraphicsImageTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawGraphicsImageTriggered",
        /* strAddInfo   */ strMthInArgs );

    // Please note that the graphics image button is not a checkable button
    // and i_bChecked is always false.
    if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawGraphicsImage ) {
        m_pActDrawChecked->setChecked(false);
    }
    m_pActDrawChecked = m_pActDrawGraphicsImage;

    QString strFileName = QFileDialog::getOpenFileName(
        /* pWdgtParent */ this,
        /* strCaption  */ "Choose image file",
        /* strDir      */ "",
        /* strFilter   */ "Image Files (*.bmp *.jpg)" );

    if( !strFileName.isEmpty() ) {
        CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
        m_pObjFactoryImage->setFileName(strFileName);
        pDrawingScene->setCurrentDrawingTool(m_pObjFactoryImage);
    }
} // onActionDrawGraphicsImageTriggered

/*==============================================================================
public slots: // Menu - Draw - Connections
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawConnectionPointToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawConnectionPointToggled",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    if( i_bChecked ) {
        if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawConnectionPoint ) {
            m_pActDrawChecked->setChecked(false);
        }
        m_pActDrawChecked = m_pActDrawConnectionPoint;
        pDrawingScene->setCurrentDrawingTool(m_pObjFactoryConnectionPoint);
    }
    else { // if( !i_bChecked )
        if( m_pActDrawChecked == m_pActDrawConnectionPoint ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryConnectionPoint ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
} // onActionDrawConnectionPointToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawConnectionLineToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawConnectionLineToggled",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    if( i_bChecked ) {
        if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawConnectionLine ) {
            m_pActDrawChecked->setChecked(false);
        }
        m_pActDrawChecked = m_pActDrawConnectionLine;
        pDrawingScene->setCurrentDrawingTool(m_pObjFactoryConnectionLine);
    }
    else { // if( !i_bChecked )
        if( m_pActDrawChecked == m_pActDrawConnectionLine ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryConnectionLine ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
} // onActionDrawConnectionLineToggled

/*==============================================================================
public slots: // Menu - View - Zoom
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionViewZoomInTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionViewZoomInTriggered",
        /* strAddInfo   */ strMthInArgs );

    int iZoomFactor_perCent = m_iViewZoomFactor_perCent;

    if( iZoomFactor_perCent == 1 ) {
        iZoomFactor_perCent = 2;
    }
    else if( iZoomFactor_perCent == 2 ) {
        iZoomFactor_perCent = 5;
    }
    else if( iZoomFactor_perCent == 5 ) {
        iZoomFactor_perCent = 10;
    }
    else if( iZoomFactor_perCent == 10 ) {
        iZoomFactor_perCent = 25;
    }
    else if( iZoomFactor_perCent == 25 ) {
        iZoomFactor_perCent = 50;
    }
    else if( iZoomFactor_perCent == 50 ) {
        iZoomFactor_perCent = 100;
    }
    else if( iZoomFactor_perCent == 100 ) {
        iZoomFactor_perCent = 250;
    }
    else if( iZoomFactor_perCent == 250 ) {
        iZoomFactor_perCent = 500;
    }
    else if( iZoomFactor_perCent == 500 ) {
        iZoomFactor_perCent = 1000;
    }
    else if( iZoomFactor_perCent == 1000 ) {
        iZoomFactor_perCent = 2500;
    }
    else if( iZoomFactor_perCent == 2500 ) {
        iZoomFactor_perCent = 5000;
    }
    else if( 2*iZoomFactor_perCent <= 5000 ) {
        iZoomFactor_perCent *= 2;
    }
    else {
        iZoomFactor_perCent = 10000;
    }

    m_pEdtViewZoomFactor_perCent->setValue(iZoomFactor_perCent);

    onEdtViewZoomFactorEditingFinished();

} // onActionViewZoomInTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionViewZoomOutTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionViewZoomOutTriggered",
        /* strAddInfo   */ strMthInArgs );

    int iZoomFactor_perCent = m_iViewZoomFactor_perCent;

    if( iZoomFactor_perCent == 10000 ) {
        iZoomFactor_perCent = 5000;
    }
    else if( iZoomFactor_perCent == 5000 ) {
        iZoomFactor_perCent = 2500;
    }
    else if( iZoomFactor_perCent == 2500 ) {
        iZoomFactor_perCent = 1000;
    }
    else if( iZoomFactor_perCent == 1000 ) {
        iZoomFactor_perCent = 500;
    }
    else if( iZoomFactor_perCent == 500 ) {
        iZoomFactor_perCent = 250;
    }
    else if( iZoomFactor_perCent == 250 ) {
        iZoomFactor_perCent = 100;
    }
    else if( iZoomFactor_perCent == 100 ) {
        iZoomFactor_perCent = 50;
    }
    else if( iZoomFactor_perCent == 50 ) {
        iZoomFactor_perCent = 25;
    }
    else if( iZoomFactor_perCent == 25 ) {
        iZoomFactor_perCent = 10;
    }
    else if( iZoomFactor_perCent == 10 ) {
        iZoomFactor_perCent = 5;
    }
    else if( iZoomFactor_perCent == 5 ) {
        iZoomFactor_perCent = 2;
    }
    else if( iZoomFactor_perCent >= 2 ) {
        iZoomFactor_perCent /= 2;
    }
    else {
        iZoomFactor_perCent = 1;
    }

    m_pEdtViewZoomFactor_perCent->setValue(iZoomFactor_perCent);

    onEdtViewZoomFactorEditingFinished();

} // onActionViewZoomOutTriggered

//------------------------------------------------------------------------------
void CMainWindow::onEdtViewZoomFactorEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    int iZoomFactor_perCent = m_pEdtViewZoomFactor_perCent->value();
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "ZoomFactor: " + QString::number(iZoomFactor_perCent) + " %";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtViewZoomFactorEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    if( m_iViewZoomFactor_perCent != iZoomFactor_perCent ) {
        CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
        double fScale = static_cast<double>(iZoomFactor_perCent) / static_cast<double>(m_iViewZoomFactor_perCent);
        m_iViewZoomFactor_perCent = iZoomFactor_perCent;
        pDrawingView->scale(fScale, fScale);
    }
} // onEdtViewZoomFactorEditingFinished

/*==============================================================================
public slots: // Menu - Debug
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDebugErrLogTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDebugErrLogTriggered",
        /* strAddInfo   */ "" );

    QString strDlgTitle = QCoreApplication::applicationName() + ": Error Log";
    CDlgErrLog* pDlg = dynamic_cast<CDlgErrLog*>(CDlgErrLog::GetInstance(strDlgTitle));
    if( pDlg == nullptr ) {
        pDlg = CDlgErrLog::CreateInstance(strDlgTitle);
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
} // onActionDebugErrLogTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionDebugUnitsTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDebugUnitsTriggered",
        /* strAddInfo   */ "" );

    CDlgPhysUnits* pDlg = CDlgPhysUnits::GetInstance();
    if( pDlg == nullptr ) {
        QString strDlgTitle = QCoreApplication::applicationName() + ": Units";
        pDlg = CDlgPhysUnits::CreateInstance(strDlgTitle);
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
} // onActionDebugUnitsTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionDebugTraceServerTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDebugTraceServerTriggered",
        /* strAddInfo   */ "" );

    CDlgTrcServer* pDlg = CDlgTrcServer::GetInstance();

    if( pDlg == nullptr ) {
        QString strDlgTitle = QCoreApplication::applicationName() + ": Method Trace Server";
        pDlg = CDlgTrcServer::CreateInstance(strDlgTitle);
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
} // onActionDebugTraceServerTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionDebugTraceAdminObjsTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDebugTraceAdminObjsTriggered",
        /* strAddInfo   */ "" );

    if( CTrcServer::GetTraceAdminObjIdxTree() != nullptr )
    {
        QString strDlgTitle = QCoreApplication::applicationName() + ": Trace Admin Objects";

        CDlgIdxTreeTrcAdminObjs* pDlg =
            CDlgIdxTreeTrcAdminObjs::GetInstance(
                CTrcServer::GetTraceAdminObjIdxTree()->objectName());

        if( pDlg == nullptr ) {
            pDlg = CDlgIdxTreeTrcAdminObjs::CreateInstance(
                strDlgTitle, CTrcServer::GetTraceAdminObjIdxTree());
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
} // onActionDebugTraceAdminObjsTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionDebugTestTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDebugTestTriggered",
        /* strAddInfo   */ "" );

    if( m_pDlgTest == nullptr ) {
        m_pDlgTest = new ZS::Test::GUI::CDlgTest(
            /* strObjName  */ "Test",
            /* pTest       */ m_pTest );
            ///* pWdgtParent */ nullptr,
            ///* wFlags      */ Qt::WindowMinMaxButtonsHint );
    }

    m_pDlgTest->show();
    m_pDlgTest->activateWindow();
    m_pDlgTest->raise();

} // onActionDebugTestTriggered

/*==============================================================================
public slots: // Drawing Scene
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneChanged( const QList<QRectF>& i_region )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "RectsCount:" + QString::number(i_region.size());
        for( int idx = 0; idx < i_region.size(); idx++ ) {
            QRectF rct = i_region[idx];
            if( idx == 0 ) {
                strMthInArgs += " " + QString::number(idx);
            } else {
                strMthInArgs += ", " + QString::number(idx);
            }
            strMthInArgs += ":(" + QString::number(rct.left(),'f',1);
            strMthInArgs += "," + QString::number(rct.top(),'f',1);
            strMthInArgs += "," + QString::number(rct.width(),'f',1);
            strMthInArgs += "," + QString::number(rct.height(),'f',1) + ")";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneChanged",
        /* strAddInfo   */ strMthInArgs );

    if( !m_bDrawingChangedSinceLastSave ) {
        if( !m_strCurrentFile.isEmpty() ) {
            m_bDrawingChangedSinceLastSave = true;
        }
        else if( i_region.size() != 1 ) {
            m_bDrawingChangedSinceLastSave = true;
        }
        else { // if( i_region.size() == 1 )
            QRectF rct = i_region[0];
            CDrawingScene* pDrawingScene = CWidgetCentral::GetInstance()->drawingScene();
            if( rct.size() == pDrawingScene->sceneRect().size() ) {
                if( pDrawingScene->items().size() > 0 ) {
                    m_bDrawingChangedSinceLastSave = true;
                }
            }
        }
    }

    updateActions();

} // onDrawingSceneChanged

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneFocusItemChanged(
    QGraphicsItem* i_pNewFocusItem,
    QGraphicsItem* i_pOldFocusItem,
    Qt::FocusReason reason )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        CGraphObj* pGraphObjNew = dynamic_cast<CGraphObj*>(i_pNewFocusItem);
        CGraphObj* pGraphObjOld = dynamic_cast<CGraphObj*>(i_pOldFocusItem);
        strMthInArgs  = "NewItem:" + QString(pGraphObjNew == nullptr ? "nullptr" : pGraphObjNew->path());
        strMthInArgs += ", OldItem:" + QString(pGraphObjOld == nullptr ? "nullptr" : pGraphObjOld->path());
        strMthInArgs += ", Reason: " + qFocusReason2Str(reason);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneFocusItemChanged",
        /* strAddInfo   */ strMthInArgs );

} // onDrawingSceneFocusItemChanged

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneSelectionChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneSelectionChanged",
        /* strAddInfo   */ "" );

    updateActions();

} // onDrawingSceneSelectionChanged

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneMousePosChanged( const QPointF& i_ptMousePos )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObjMouseEvents, EMethodTraceDetailLevel::ArgsNormal) )  {
        strMthInArgs = "Pos:" + point2Str(i_ptMousePos);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneMousePosChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_pLblStatusBarDrawingSceneMouseCursorPos != nullptr ) {
        //CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
        //CPageSetup* pageSetup = pDrawingView->getPageSetup();
        //CUnit unitWidth = pageSetup->unit(EDirection::Horizontal);
        //QString strMouseCursorPos;
        //strMouseCursorPos += QString("ScenePos: ");
        //strMouseCursorPos += QString::number(i_ptMousePos.x());
        //strMouseCursorPos += QString("/");
        //strMouseCursorPos += QString::number(i_ptMousePos.y());
        //strMouseCursorPos += QString(" [");
        //strMouseCursorPos += QString(unitWidth.symbol());
        //strMouseCursorPos += QString("]");

        //if( m_drawArea.isValid() && m_drawArea.m_physValWidth.getPhysSize() != &Geometry::GraphDevice() )
        //{
        //    strMouseCursorPos += QString("=");
        //    strMouseCursorPos += QString("(");
        //    strMouseCursorPos += QString::number(getXPhysVal(m_fXPosMouseMoveEvent).getVal(),'f',2);
        //    strMouseCursorPos += QString("[");
        //    strMouseCursorPos += QString(m_drawArea.m_physValWidth.getPhysUnit()->getSymbol());
        //    strMouseCursorPos += QString("]");
        //    strMouseCursorPos += QString("/");
        //    strMouseCursorPos += QString::number(getYPhysVal(m_fYPosMouseMoveEvent).getVal(),'f',2);
        //    strMouseCursorPos += QString("[");
        //    strMouseCursorPos += QString(m_drawArea.m_physValHeight.getPhysUnit()->getSymbol());
        //    strMouseCursorPos += QString("]");
        //    strMouseCursorPos += QString(")");
        //}

        //m_pLblStatusBarDrawingSceneMouseCursorPos->setText(strMouseCursorPos);

    } // if( m_pLblStatusBarDrawingSceneMouseCursorPos != nullptr )

} // onDrawingSceneMousePosChanged

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneModeChanged()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneModeChanged",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
        int iObjFactoryType = pDrawingScene->getCurrentDrawingToolGraphObjType();
        CGraphObj* pGraphObjCreating = pDrawingScene->getGraphObjCreating();

        strAddTrcInfo  = "Mode:" + pDrawingScene->getMode().toString();
        strAddTrcInfo += ", EditTool:" + pDrawingScene->getEditTool().toString();
        strAddTrcInfo += ", EditMode:" + pDrawingScene->getEditMode().toString();
        strAddTrcInfo += ", ResizeMode:" + pDrawingScene->getEditResizeMode().toString();
        strAddTrcInfo += ", ObjFactory:" + graphObjType2Str(iObjFactoryType);
        strAddTrcInfo += ", GraphObjCreating: " + QString(pGraphObjCreating == nullptr ? "nullptr" : pGraphObjCreating->name());
    }

    setWindowTitle();
    updateActions();
    updateStatusBar();

} // onDrawingSceneModeChanged

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneDrawSettingsChanged( const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneDrawSettingsChanged",
        /* strAddInfo   */ "" );

    if( m_pActDrawSettingsLine != nullptr )
    {
        QIcon   iconEditDrawSettingsLine;
        QPixmap pxmEditDrawSettingsLine(":/ZS/Draw/DrawToolPen16x16.bmp");

        pxmEditDrawSettingsLine.setMask(pxmEditDrawSettingsLine.createHeuristicMask());

        QPainter painter(&pxmEditDrawSettingsLine);
        QLine    lineBtnLineColor1( QPoint(0,2), QPoint(8,2) );
        QLine    lineBtnLineColor2( QPoint(0,2), QPoint(0,10) );
        QLine    lineBtnLineColor3( QPoint(0,10), QPoint(5,10) );
        QRect    rctBtnLineColor(0,12,16,4);

        painter.setPen(i_drawSettings.getPenColor());
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(lineBtnLineColor1);
        painter.drawLine(lineBtnLineColor2);
        painter.drawLine(lineBtnLineColor3);

        painter.setPen(Qt::NoPen);
        painter.setBrush(i_drawSettings.getPenColor());
        painter.drawRect(rctBtnLineColor);

        iconEditDrawSettingsLine.addPixmap(pxmEditDrawSettingsLine);

        m_pActDrawSettingsLine->setIcon(iconEditDrawSettingsLine);

    } // if( m_pActDrawSettingsLine != nullptr )

    if( m_pActDrawSettingsFill != nullptr )
    {
        QIcon   iconEditDrawSettingsFill;
        QPixmap pxmEditDrawSettingsFill(":/ZS/Draw/DrawToolBrush16x16.bmp");

        pxmEditDrawSettingsFill.setMask(pxmEditDrawSettingsFill.createHeuristicMask());

        QPainter painter(&pxmEditDrawSettingsFill);
        QLine    lineBtnFillColor1( QPoint(13,5), QPoint(13,10) );
        QLine    lineBtnFillColor2( QPoint(14,6), QPoint(14,9) );
        QLine    lineBtnFillColor3( QPoint(15,7), QPoint(15,8) );
        QRect    rctBtnFillColor(0,11,16,5);

        painter.setPen(i_drawSettings.getFillColor());
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(lineBtnFillColor1);
        painter.drawLine(lineBtnFillColor2);
        painter.drawLine(lineBtnFillColor3);

        painter.setPen(Qt::NoPen);
        painter.setBrush(i_drawSettings.getFillColor());
        painter.drawRect(rctBtnFillColor);

        iconEditDrawSettingsFill.addPixmap(pxmEditDrawSettingsFill);

        m_pActDrawSettingsFill->setIcon(iconEditDrawSettingsFill);

    } // if( m_pActDrawSettingsFill != nullptr )

    if( m_pActDrawSettingsText != nullptr )
    {
        QIcon   iconEditDrawSettingsText;
        QPixmap pxmEditDrawSettingsText(":/ZS/Draw/DrawToolText16x16.bmp");

        pxmEditDrawSettingsText.setMask(pxmEditDrawSettingsText.createHeuristicMask());

        QPainter painter(&pxmEditDrawSettingsText);
        QRect    rctBtnLineColor(0,12,16,4);

        painter.setPen(Qt::NoPen);
        painter.setBrush(i_drawSettings.getTextColor());
        painter.drawRect(rctBtnLineColor);

        iconEditDrawSettingsText.addPixmap(pxmEditDrawSettingsText);

        m_pActDrawSettingsText->setIcon(iconEditDrawSettingsText);

    } // if( m_pActDrawSettingsText != nullptr )

} // onDrawingSceneDrawSettingsChanged

/*==============================================================================
protected slots: // Drawing View
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onDrawingViewMousePosChanged( const QPointF& i_ptMousePos )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjMouseEvents != nullptr && m_pTrcAdminObjMouseEvents->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Pos:" + point2Str(i_ptMousePos);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingViewMousePosChanged",
        /* strAddInfo   */ strMthInArgs );

    if( m_pLblStatusBarDrawingViewMouseCursorPos != nullptr )
    {
        //CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
        //CPageSetup* pageSetup = pDrawingView->getPageSetup();
        //CUnit unitWidth = pageSetup->unit(EDirection::Horizontal);

        //QString strMouseCursorPos;

        //strMouseCursorPos += QString("ViewPos: ");
        //strMouseCursorPos += QString::number(i_ptMousePos.x());
        //strMouseCursorPos += QString("/");
        //strMouseCursorPos += QString::number(i_ptMousePos.y());
        //strMouseCursorPos += QString(" [");
        //strMouseCursorPos += QString(unitWidth.symbol());
        //strMouseCursorPos += QString("]");

        //m_pLblStatusBarDrawingViewMouseCursorPos->setText(strMouseCursorPos);

    } // if( m_pLblStatusBarDrawingViewMouseCursorPos != nullptr )

} // onDrawingViewMousePosChanged

/*==============================================================================
protected slots: // Tree View Object Factories (ToolBox)
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onTreeViewObjFactoriesExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onTreeViewObjFactoriesExpanded",
        /* strAddInfo   */ strMthInArgs );

    if( i_modelIdx.isValid() )
    {
        m_pTreeViewObjFactories->resizeColumnToContents(i_modelIdx.column());
    }

} // onTreeViewObjFactoriesExpanded

//------------------------------------------------------------------------------
void CMainWindow::onTreeViewObjFactoriesCurrentChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& /*i_modelIdxPrev*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = qModelIndex2Str(i_modelIdxCurr);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onTreeViewObjFactoriesCurrentChanged",
        /* strAddInfo   */ strMthInArgs );

    CObjFactory* pObjFactory = nullptr;

    if( i_modelIdxCurr.isValid() && i_modelIdxCurr.internalPointer() != nullptr ) {
        CIdxTreeEntry* pTreeEntry = static_cast<CIdxTreeEntry*>(i_modelIdxCurr.internalPointer());
        if( pTreeEntry != nullptr ) {
            pObjFactory = dynamic_cast<CObjFactory*>(pTreeEntry);
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        strAddTrcInfo = "ObjFactory: " + QString(pObjFactory == nullptr ? "nullptr" : pObjFactory->path());
    }

    // If a valid tree node in the object factories model is selected ..
    if( pObjFactory != nullptr ) {
        // .. objects will be created by drag and drop operations but not by a drawing tool.
        CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
        pDrawingScene->setCurrentDrawingTool(nullptr);
    }
} // onTreeViewObjFactoriesCurrentChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::selectTreeViewObjFactoryNode( ZS::Draw::CObjFactory* i_pObjFactory )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString(i_pObjFactory == nullptr ? "nullptr" : i_pObjFactory->path());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "selectTreeViewObjFactoryNode",
        /* strAddInfo   */ strMthInArgs );

    if( m_pTreeViewObjFactories != nullptr ) {
        QObject::disconnect(
            m_pTreeViewObjFactories->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &CMainWindow::onTreeViewObjFactoriesCurrentChanged );

        if( m_pTreeViewObjFactories != nullptr && m_pModelObjFactories != nullptr ) {
            if( i_pObjFactory == nullptr ) {
                m_pTreeViewObjFactories->selectionModel()->clear();
            }
            else {
                CIdxTreeEntry* pTreeEntry =
                    dynamic_cast<CIdxTree*>(m_pModelObjFactories->idxTree())->findLeave(
                        i_pObjFactory->getGroupName(),               // see CObjFactory::registerObjFactory
                        i_pObjFactory->getGraphObjTypeAsString() );  // see CObjFactory::registerObjFactory
                if( pTreeEntry != nullptr ) {
                    ////QModelIndex modelIdxParent;

                    ////CObjPoolTreeEntry* pTreeEntryParent = pTreeEntry->getParentEntry();

                    ////if( pTreeEntryParent != nullptr )
                    ////{
                    ////    modelIdxParent = m_pModelObjFactories->index( pTreeEntryParent->getRowId(), 0 );
                    ////}

                    ////QModelIndex modelIdx = m_pModelObjFactories->index( pTreeEntry->getRowId(), 0 );

                    //m_pTreeViewObjFactories->scrollTo(modelIdx);

                    //m_pTreeViewObjFactories->selectionModel()->select(modelIdx,QItemSelectionModel::ClearAndSelect);

                    //// If a valid tree node in the object factories model is selected ..
                    //// .. objects will be created by drag and drop operations but not by a drawing tool.
                    //m_pDrawingScene->setCurrentDrawingTool(nullptr);
                }
            }
        }

        QObject::connect(
            m_pTreeViewObjFactories->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &CMainWindow::onTreeViewObjFactoriesCurrentChanged );

    } // if( m_pTreeViewObjFactories != nullptr )

} // selectTreeViewObjFactoryNode

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::resizeEvent( QResizeEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs  = "Size: " + size2Str(i_pEv->size());
        strMthInArgs += ", OldSize: " + size2Str(i_pEv->oldSize());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "resizeEvent",
        /* strAddInfo   */ strMthInArgs );

    QMainWindow::resizeEvent(i_pEv);

    if( m_pLblStatusBarDrawingSceneRect != nullptr )
    {
        //CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
        //CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

        //CPageSetup* pageSetup = pDrawingView->getPageSetup();
        //CUnit unitWidth = pageSetup->unit(EDirection::Horizontal);
        //QRectF rect = pDrawingScene->sceneRect();
        //QPointF ptTL = pDrawingView->mapFromScene(rect.topLeft());
        //rect.moveTopLeft(ptTL);
        //QString strRect = point2Str(ptTL) + ", " + size2Str(rect.size());
        //m_pLblStatusBarDrawingSceneRect->setText("SceneRect: " + strRect + " [" + unitWidth.symbol() + "]");
    }
} // resizeEvent

//------------------------------------------------------------------------------
void CMainWindow::showEvent( QShowEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showEvent",
        /* strAddInfo   */ strMthInArgs );

    QMainWindow::showEvent(i_pEv);

    if( m_pLblStatusBarDrawingSceneRect != nullptr ) {
        //CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
        //CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

        //CPageSetup* pageSetup = pDrawingView->getPageSetup();
        //CUnit unitWidth = pageSetup->unit(EDirection::Horizontal);
        //QRectF rect = pDrawingScene->sceneRect();
        //QPointF ptTL = pDrawingView->mapFromScene(rect.topLeft());
        //rect.moveTopLeft(ptTL);
        //QString strRect = point2Str(ptTL) + ", " + size2Str(rect.size());
        //m_pLblStatusBarDrawingSceneRect->setText("SceneRect: " + strRect + " [" + unitWidth.symbol() + "]");
    }
} // showEvent

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setCurrentFile( const QString& i_strFileName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = "FileName:" + i_strFileName;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCurrentFile",
        /* strAddInfo   */ strMthInArgs );

    m_bDrawingChangedSinceLastSave = false;
    m_strCurrentFile = i_strFileName;

    setWindowTitle();

    //if( m_strCurrentFile.isEmpty() )
    //{
    //    setWindowTitle( m_strWindowTitleAppName );
    //}
    //else
    //{
    //    setWindowTitle( m_strWindowTitleAppName + " - " + m_strCurrentFile );
    //}

    //QStringList strlstFilesRecent = m_pSettingsFile->value( m_strSettingsKey + "/RecentFilesList" ).toStringList();

    //strlstFilesRecent.removeAll(m_strCurrentFile);
    //strlstFilesRecent.prepend(m_strCurrentFile);

    //while( strlstFilesRecent.size() > EFilesRecentCountMax )
    //{
    //    strlstFilesRecent.removeLast();
    //}

    //m_pSettingsFile->setValue( m_strSettingsKey + "/RecentFilesList", strlstFilesRecent );

    //updateActionsFilesRecent();

} // setCurrentFile

//------------------------------------------------------------------------------
void CMainWindow::updateStatusBar()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateStatusBar",
        /* strAddInfo   */ "" );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    if( m_pLblStatusBarDrawingSceneEditTool != nullptr ) {
        QString strEditInfo = "Tool: ";
        CEnumEditTool editTool = pDrawingScene->getEditTool();
        if( editTool == EEditTool::CreateObjects ) {
            int     iGraphObjType = pDrawingScene->getCurrentDrawingToolGraphObjType();
            QString strObjFactory = graphObjType2Str(iGraphObjType);
            strEditInfo += "Create " + strObjFactory;
        }
        else if( editTool != EEditTool::None ) {
            strEditInfo += editTool.toString();
        }
        m_pLblStatusBarDrawingSceneEditTool->setText(strEditInfo);
    }

    if( m_pLblStatusBarDrawingSceneEditMode != nullptr || m_pLblStatusBarDrawingSceneGraphObjEditInfo != nullptr ) {
        QString strEditModeInfo;
        QString strGraphObjEditInfo;

        CEnumEditMode       editMode       = pDrawingScene->getEditMode();
        CEnumEditResizeMode editResizeMode = pDrawingScene->getEditResizeMode();

        QList<QGraphicsItem*> arGraphicsItemSelected = pDrawingScene->selectedItems();
        QGraphicsItem*        pGraphicsItemCreating  = pDrawingScene->getGraphicsItemCreating();
        QGraphicsItem*        pGraphicsItem = nullptr;
        CGraphObj*            pGraphObj = nullptr;

        if( editMode != EEditMode::None )
        {
            if( arGraphicsItemSelected.size() > 0 )
            {
                pGraphicsItem = arGraphicsItemSelected[0];
            }
            else if( pGraphicsItemCreating != nullptr )
            {
                pGraphicsItem = pGraphicsItemCreating;
            }

            if( pGraphicsItem != nullptr )
            {
                pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                if( pGraphObj == nullptr )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObj == nullptr" );
                }
                strEditModeInfo += pGraphObj->name();
                strGraphObjEditInfo += pGraphObj->name() + ": " + pGraphObj->getEditInfo();
            }

            if( editMode == EEditMode::Resize )
            {
                if( editResizeMode >= 0 && editResizeMode < CEnumEditResizeMode::count() )
                {
                    if( !strEditModeInfo.isEmpty() )
                    {
                        strEditModeInfo += ".";
                    }
                    strEditModeInfo += editResizeMode.toString();
                }
                else if( editMode != EEditMode::None )
                {
                    if( !strEditModeInfo.isEmpty() )
                    {
                        strEditModeInfo += ".";
                    }
                    strEditModeInfo += editMode.toString();
                }
            }
            else
            {
                if( !strEditModeInfo.isEmpty() )
                {
                    strEditModeInfo += ".";
                }
                strEditModeInfo += editMode.toString();
            }
        } // if( m_editMode != EEditMode::None )

        if( m_pLblStatusBarDrawingSceneEditMode != nullptr )
        {
            m_pLblStatusBarDrawingSceneEditMode->setText("Mode: " + strEditModeInfo);
        }

        if( m_pLblStatusBarDrawingSceneGraphObjEditInfo != nullptr )
        {
            m_pLblStatusBarDrawingSceneGraphObjEditInfo->setText("Info: " + strGraphObjEditInfo);
        }
    } // if( m_pLblStatusBarDrawingSceneEditMode != nullptr )
} // updateStatusBar

//------------------------------------------------------------------------------
void CMainWindow::updateActions()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateActions",
        /* strAddInfo   */ "" );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    CEnumMode mode = pDrawingScene->getMode();

    QList<QGraphicsItem*> arpGraphicItemsSelected = pDrawingScene->selectedItems();
    QGraphicsItem*        pGraphicsItem;

    // Menu - File
    //------------

    //if( m_pActFileNew != nullptr )
    //{
    //    m_pActFileNew;
    //}
    //if( m_pActFileOpen != nullptr )
    //{
    //    m_pActFileOpen;
    //}
    //if( m_pActFileSave != nullptr )
    //{
    //    m_pActFileSave;
    //}
    //if( m_pActFileSaveAs != nullptr )
    //{
    //    m_pActFileSaveAs;
    //}
    //if( m_pActFileQuit != nullptr )
    //{
    //    m_pActFileQuit;
    //}
    //if( m_pActFileQuit != nullptr )
    //{
    //    m_pActFileQuit;
    //}

    // Menu - Mode
    //------------

    if( m_pActModeEdit != nullptr )
    {
        m_pActModeEdit->setChecked( mode == EMode::Edit );
    }
    if( m_pActModeView != nullptr )
    {
        m_pActModeView->setChecked( mode == EMode::View );
    }

    // Menu - Edit - Select
    //---------------------

    if( m_pActEditSelect != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActEditSelect->setEnabled(false);
        }
        else
        {
            if( pDrawingScene->items().size() == 0 )
            {
                m_pActEditSelect->setEnabled(false);
            }
            else // if( m_pDrawingScene->items().size() > 0 )
            {
                m_pActEditSelect->setEnabled(true);
            }
            if( pDrawingScene->getEditTool() != EEditTool::Select )
            {
                m_pActEditSelect->setChecked(false);
            }
            else // if( pDrawingScene->getEditTool() == EEditTool::Select )
            {
                m_pActEditSelect->setChecked(true);
            }
        }
    }

    // Menu - Edit - Rotate and Mirror
    //--------------------------------

    if( mode == EMode::View )
    {
        if( m_pActEditRotateLeft != nullptr )
        {
            m_pActEditRotateLeft->setEnabled(false);
        }
        if( m_pActEditRotateRight != nullptr )
        {
            m_pActEditRotateRight->setEnabled(false);
        }
        if( m_pEdtEditRotateAngle != nullptr )
        {
            m_pEdtEditRotateAngle->setEnabled(false);
        }
        if( m_pActEditMirrorVertical != nullptr )
        {
            m_pActEditMirrorVertical->setEnabled(false);
        }
        if( m_pActEditMirrorHorizontal != nullptr )
        {
            m_pActEditMirrorHorizontal->setEnabled(false);
        }
    }
    else // if( mode == EMode::Edit )
    {
        if( arpGraphicItemsSelected.size() == 0 )
        {
            if( m_pActEditRotateLeft != nullptr )
            {
                m_pActEditRotateLeft->setEnabled(false);
            }
            if( m_pActEditRotateRight != nullptr )
            {
                m_pActEditRotateRight->setEnabled(false);
            }
            if( m_pEdtEditRotateAngle != nullptr )
            {
                m_pEdtEditRotateAngle->setEnabled(false);
            }
            if( m_pActEditMirrorVertical != nullptr )
            {
                m_pActEditMirrorVertical->setEnabled(false);
            }
            if( m_pActEditMirrorHorizontal != nullptr )
            {
                m_pActEditMirrorHorizontal->setEnabled(false);
            }
        }// if( arpGraphicItemsSelected.size() == 0 )

        else // if( arpGraphicItemsSelected.size() > 0 )
        {
            if( m_pActEditRotateLeft != nullptr )
            {
                m_pActEditRotateLeft->setEnabled(true);
            }
            if( m_pActEditRotateRight != nullptr )
            {
                m_pActEditRotateRight->setEnabled(true);
            }
            if( m_pEdtEditRotateAngle != nullptr )
            {
                m_pEdtEditRotateAngle->setEnabled(true);
            }
            if( m_pActEditMirrorVertical != nullptr )
            {
                //m_pActEditMirrorVertical->setEnabled(true); not yet supported
            }
            if( m_pActEditMirrorHorizontal != nullptr )
            {
                //m_pActEditMirrorHorizontal->setEnabled(true); not yet supported
            }
        } // if( arpGraphicItemsSelected.size() > 0 )

    } // if( mode == EMode::Edit )

    // Menu - Edit - Group
    //---------------------

    if( mode == EMode::View )
    {
        if( m_pActEditGroup != nullptr )
        {
            m_pActEditGroup->setEnabled(false);
        }
        if( m_pActEditUngroup != nullptr )
        {
            m_pActEditUngroup->setEnabled(false);
        }
    }
    else
    {
        if( arpGraphicItemsSelected.size() == 0 )
        {
            if( m_pActEditGroup != nullptr )
            {
                m_pActEditGroup->setEnabled(false);
            }
            if( m_pActEditUngroup != nullptr )
            {
                m_pActEditUngroup->setEnabled(false);
            }
        } // if( arpGraphicItemsSelected.size() == 0 )

        else if( arpGraphicItemsSelected.size() == 1 )
        {
            if( m_pActEditGroup != nullptr )
            {
                m_pActEditGroup->setEnabled(false);
            }

            pGraphicsItem = arpGraphicItemsSelected[0];

            if( pGraphicsItem->type() == EGraphObjTypeGroup )
            {
                if( m_pActEditUngroup != nullptr )
                {
                    m_pActEditUngroup->setEnabled(true);
                }
            }
            else
            {
                if( m_pActEditUngroup != nullptr )
                {
                    m_pActEditUngroup->setEnabled(false);
                }
            }

        } // if( arpGraphicItemsSelected.size() == 1 )

        else // if( arpGraphicItemsSelected.size() > 1 )
        {
            if( m_pActEditGroup != nullptr )
            {
                m_pActEditGroup->setEnabled(true);
            }
            if( m_pActEditUngroup != nullptr )
            {
                m_pActEditUngroup->setEnabled(false);
            }

        } // if( arpGraphicItemsSelected.size() > 1 )

    } // if( mode == EMode::Edit )

    // Menu - Draw
    //---------------------

    //if( m_pActDrawChecked != nullptr )
    //{
    //    m_pActDrawChecked;
    //}

    // Menu - Draw - Settings
    //-----------------------

    if( m_pActDrawSettingsLine != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawSettingsLine->setEnabled(false);
        }
        else
        {
            m_pActDrawSettingsLine->setEnabled(true);
        }
    }

    if( m_pActDrawSettingsFill != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawSettingsFill->setEnabled(false);
        }
        else
        {
            m_pActDrawSettingsFill->setEnabled(true);
        }
    }

    if( m_pActDrawSettingsText != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawSettingsText->setEnabled(false);
        }
        else
        {
            m_pActDrawSettingsText->setEnabled(true);
        }
    }

    // Menu - Draw - Standard Shapes
    //------------------------------

    if( m_pActDrawStandardShapePoint != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawStandardShapePoint->setEnabled(false);
            m_pActDrawStandardShapePoint->setChecked(false);
        }
        else
        {
            m_pActDrawStandardShapePoint->setEnabled(true);

            if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPoint )
            {
                m_pActDrawStandardShapePoint->setChecked(true);
            }
            else
            {
                m_pActDrawStandardShapePoint->setChecked(false);
            }
        }
    }

    if( m_pActDrawStandardShapeLine != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawStandardShapeLine->setEnabled(false);
            m_pActDrawStandardShapeLine->setChecked(false);
        }
        else
        {
            m_pActDrawStandardShapeLine->setEnabled(true);

            if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryLine )
            {
                m_pActDrawStandardShapeLine->setChecked(true);
            }
            else
            {
                m_pActDrawStandardShapeLine->setChecked(false);
            }
        }
    }

    if( m_pActDrawStandardShapeRect != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawStandardShapeRect->setEnabled(false);
            m_pActDrawStandardShapeRect->setChecked(false);
        }
        else
        {
            m_pActDrawStandardShapeRect->setEnabled(true);

            if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryRect )
            {
                m_pActDrawStandardShapeRect->setChecked(true);
            }
            else
            {
                m_pActDrawStandardShapeRect->setChecked(false);
            }
        }
    }

    if( m_pActDrawStandardShapeEllipse != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawStandardShapeEllipse->setEnabled(false);
            m_pActDrawStandardShapeEllipse->setChecked(false);
        }
        else
        {
            m_pActDrawStandardShapeEllipse->setEnabled(true);

            if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryEllipse )
            {
                m_pActDrawStandardShapeEllipse->setChecked(true);
            }
            else
            {
                m_pActDrawStandardShapeEllipse->setChecked(false);
            }
        }
    }

    if( m_pActDrawStandardShapePolyline != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawStandardShapePolyline->setEnabled(false);
            m_pActDrawStandardShapePolyline->setChecked(false);
        }
        else
        {
            m_pActDrawStandardShapePolyline->setEnabled(true);

            if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPolyline )
            {
                m_pActDrawStandardShapePolyline->setChecked(true);
            }
            else
            {
                m_pActDrawStandardShapePolyline->setChecked(false);
            }
        }
    }

    if( m_pActDrawStandardShapePolygon != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawStandardShapePolygon->setEnabled(false);
            m_pActDrawStandardShapePolygon->setChecked(false);
        }
        else
        {
            m_pActDrawStandardShapePolygon->setEnabled(true);

            if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPolygon )
            {
                m_pActDrawStandardShapePolygon->setChecked(true);
            }
            else
            {
                m_pActDrawStandardShapePolygon->setChecked(false);
            }
        }
    }

    if( m_pActDrawStandardShapeText != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawStandardShapeText->setEnabled(false);
            m_pActDrawStandardShapeText->setChecked(false);
        }
        else
        {
            m_pActDrawStandardShapeText->setEnabled(true);

            if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryText )
            {
                m_pActDrawStandardShapeText->setChecked(true);
            }
            else
            {
                m_pActDrawStandardShapeText->setChecked(false);
            }
        }
    }

    // Menu - Draw - Graphics
    //-----------------------

    if( m_pActDrawGraphicsImage != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawGraphicsImage->setEnabled(false);
        }
        else
        {
            m_pActDrawGraphicsImage->setEnabled(true);
        }
    }

    // Menu - Draw - Connections
    //--------------------------

    if( m_pActDrawConnectionPoint != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawConnectionPoint->setEnabled(false);
            m_pActDrawConnectionPoint->setChecked(false);
        }
        else
        {
            m_pActDrawConnectionPoint->setEnabled(true);

            if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryConnectionPoint )
            {
                m_pActDrawConnectionPoint->setChecked(true);
            }
            else
            {
                m_pActDrawConnectionPoint->setChecked(false);
            }
        }
    }

    if( m_pActDrawConnectionLine != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pActDrawConnectionLine->setEnabled(false);
            m_pActDrawConnectionLine->setChecked(false);
        }
        else
        {
            m_pActDrawConnectionLine->setEnabled(true);

            if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryConnectionLine )
            {
                m_pActDrawConnectionLine->setChecked(true);
            }
            else
            {
                m_pActDrawConnectionLine->setChecked(false);
            }
        }
    }

    // Menu - Trace
    //---------------------

    //if( m_pActDebugErrLog != nullptr )
    //{
    //    m_pActDebugErrLog;
    //}
    //if( m_pActDebugTraceServer != nullptr )
    //{
    //    m_pActDebugTraceServer;
    //}
    //if( m_pActDebugTraceAdminObjs != nullptr )
    //{
    //    m_pActDebugTraceAdminObjs;
    //}
    //if( m_pActionDebugTest != nullptr )
    //{
    //    m_pActionDebugTest;
    //}

    // Dock Widgets
    //--------------

    // Dock Widget - Object Factories
    //--------------------------------

    if( m_pDockWdgtObjFactories != nullptr )
    {
        if( mode == EMode::View )
        {
            m_pDockWdgtObjFactories->hide();
        }
        else
        {
            m_pDockWdgtObjFactories->show();
        }
    }

    if( mode == EMode::View )
    {
        selectTreeViewObjFactoryNode(nullptr);
    }
    else if( pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        selectTreeViewObjFactoryNode(nullptr);
    }

    // Dock Widget - Graphics Items
    //-----------------------------

} // updateActions

//------------------------------------------------------------------------------
void CMainWindow::updateActionsFilesRecent()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateActionsFilesRecent",
        /* strAddInfo   */ strMthInArgs );

    QSettings settings;

    QStringList strlstFilesRecent = settings.value(objectName()+"/RecentFilesList").toStringList();
    int         idxFile;

    int iFilesRecentCount = Math::minVal( 2, strlstFilesRecent.size(), _ZSArrLen(m_arpActFilesRecent) );

    for( idxFile = 0; idxFile < iFilesRecentCount; idxFile++ )
    {
        QString strFileNameStripped = QFileInfo(strlstFilesRecent[idxFile]).fileName();
        QString strText = tr("&%1 %2").arg(idxFile).arg(strFileNameStripped);

        if( m_arpActFilesRecent[idxFile] != nullptr )
        {
            m_arpActFilesRecent[idxFile]->setText(strText);
            m_arpActFilesRecent[idxFile]->setData(strlstFilesRecent[idxFile]);
            m_arpActFilesRecent[idxFile]->setVisible(true);
        }
    }

    for( idxFile = iFilesRecentCount; idxFile < _ZSArrLen(m_arpActFilesRecent); idxFile++ )
    {
        if( m_arpActFilesRecent[idxFile] != nullptr )
        {
            m_arpActFilesRecent[idxFile]->setVisible(false);
        }
    }

    if( m_pActFilesRecentSeparator != nullptr )
    {
        m_pActFilesRecentSeparator->setVisible( iFilesRecentCount > 0 );
    }

} // updateActionsFilesRecent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryAdded( const ZS::System::SErrResultInfo& /*i_errResultInfo*/ )
//------------------------------------------------------------------------------
{
    updateErrorsStatus();
}

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryChanged( const ZS::System::SErrResultInfo& /*i_errResultInfo*/ )
//------------------------------------------------------------------------------
{
    updateErrorsStatus();
}

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryRemoved( const ZS::System::SErrResultInfo& /*i_errResultInfo*/ )
//------------------------------------------------------------------------------
{
    updateErrorsStatus();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::updateErrorsStatus()
//------------------------------------------------------------------------------
{
    CErrLog* pErrLog = CErrLog::GetInstance();

    EResultSeverity severityMax = EResultSeveritySuccess;
    QString         strToolTip;
    SErrLogEntry*   pErrLogEntry = nullptr;
    EResultSeverity severity;
    int             iRowIdx;

    int ariErrorsCount[EResultSeverityCount];
    int iErrorsCount = 0;

    memset( ariErrorsCount, 0x00, sizeof(ariErrorsCount) );

    if( pErrLog != nullptr )
    {
        pErrLog->lock();

        for( iRowIdx = 0; iRowIdx < pErrLog->getEntryCount(); iRowIdx++ )
        {
            pErrLogEntry = pErrLog->getEntry(iRowIdx);

            if( pErrLogEntry != nullptr )
            {
                if( pErrLogEntry->m_errResultInfo.getSeverity() > severityMax )
                {
                    severityMax = pErrLogEntry->m_errResultInfo.getSeverity();
                }
                if( pErrLogEntry->m_errResultInfo.getSeverity() >= EResultSeverityInfo && pErrLogEntry->m_errResultInfo.getSeverity() < EResultSeverityCount )
                {
                    ariErrorsCount[pErrLogEntry->m_errResultInfo.getSeverity()]++;
                    iErrorsCount++;
                }
            }
        }
        pErrLog->unlock();

        if( iErrorsCount == 0 )
        {
            strToolTip = "There is no Info, no Warning, no Error and no Critical Error message pending";
            m_pLblErrors->hide();
        }
        else if( iErrorsCount > 0 )
        {
            int iErrorsCountTmp = 0;
            int iRowIdxTmp;

            if( iErrorsCount == 1 )
            {
                strToolTip = "There is ";
            }
            else
            {
                strToolTip = "There are ";
            }

            for( iRowIdx = EResultSeverityInfo; iRowIdx < EResultSeverityCount; iRowIdx++ )
            {
                severity = static_cast<EResultSeverity>(iRowIdx);

                if( ariErrorsCount[iRowIdx] > 0 )
                {
                    strToolTip += QString::number(ariErrorsCount[iRowIdx]) + " " + resultSeverity2Str(severity);

                    if( severity == EResultSeverityInfo || severity == EResultSeverityWarning || severity == EResultSeverityError )
                    {
                        if( ariErrorsCount[iRowIdx] > 1 )
                        {
                            strToolTip += "s";
                        }
                    }
                    else if( severity == EResultSeverityCritical )
                    {
                        if( ariErrorsCount[iRowIdx] == 1 )
                        {
                            strToolTip += " Error";
                        }
                        else
                        {
                            strToolTip += " Errors";
                        }
                    }

                    iErrorsCountTmp += ariErrorsCount[iRowIdx];

                    if( iErrorsCountTmp < iErrorsCount )
                    {
                        int iSeveritiesWithErrorsFollow = 0;

                        for( iRowIdxTmp = iRowIdx+1; iRowIdxTmp < EResultSeverityCount; iRowIdxTmp++ )
                        {
                            if( ariErrorsCount[iRowIdxTmp] > 0 )
                            {
                                iSeveritiesWithErrorsFollow++;
                            }
                        }
                        if( iSeveritiesWithErrorsFollow >= 2 )
                        {
                            strToolTip += ", ";
                        }
                        else if( iSeveritiesWithErrorsFollow >= 1 )
                        {
                            strToolTip += " and ";
                        }
                    }
                }
            }
            m_pLblErrors->show();

        } // if( iErrorsCount > 0 )

        strToolTip += ".";

    } // if( pErrLog != nullptr )

    if( m_pLblErrors != nullptr )
    {
        m_pLblErrors->setPixmap( getErrPixmap(severityMax) );
        m_pLblErrors->setToolTip(strToolTip);
    }

} // updateErrorsStatus
