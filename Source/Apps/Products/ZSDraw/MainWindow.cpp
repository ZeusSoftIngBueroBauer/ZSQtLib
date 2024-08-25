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
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryConnectionLine.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryConnectionPoint.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryEllipse.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryGroup.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryImage.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryLine.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryPoint.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryPolygon.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryPolyline.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryRect.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactoryText.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingViewDrawSettingsDlg.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingViewPropertiesDlg.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
#include "ZSDraw/Widgets/ObjFactories/ZSDrawObjFactoriesModel.h"
#include "ZSDraw/Widgets/TreeView/ZSDrawGraphObjsWdgt.h"
#include "ZSPhysValGUI/ZSPhysUnitsDlg.h"
#include "ZSPhysValGUI/ZSPhysValEditDlg.h"
#include "ZSIpcTraceGUI/ZSIpcTrcServerDlg.h"
#include "ZSTestGUI/ZSTestDlg.h"
#include "ZSTest/ZSTest.h"
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

#include <QtCore/qfileinfo.h>
#include <QtCore/qsettings.h>
#include <QtCore/qstandardpaths.h>
#include <QtCore/qtimer.h>

#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qaction.h>
#include <QtGui/qdockwidget.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qlabel.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qstatusbar.h>
#include <QtGui/qtoolbar.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qaction.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qstatusbar.h>
#include <QtWidgets/qtoolbar.h>
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

const QString CMainWindow::c_strObjFactoryQtWidgets = "QtWidgets";
const QString CMainWindow::c_strObjFactoryElectricity = "Electricity";

const QString CMainWindow::c_strMenuNameFile               = "&File";
const QString CMainWindow::c_strMenuNameFileOpenLastUsed   = "Last Used Files ...";
//const QString CMainWindow::c_strMenuNameMode               = "&Mode";
const QString CMainWindow::c_strMenuNameDraw               = "&Draw";
const QString CMainWindow::c_strMenuNameDrawSettings       = "Draw:S&ettings";
const QString CMainWindow::c_strMenuNameDrawStandardShapes = "Draw:&Standard Shapes";
const QString CMainWindow::c_strMenuNameDrawGraphics       = "Draw:&Graphics";
const QString CMainWindow::c_strMenuNameDrawConnections    = "Draw:&Connections";
const QString CMainWindow::c_strMenuNameDrawWidgets        = "Draw:&Widgets";
const QString CMainWindow::c_strMenuNameDrawElectricity    = "Draw:&Electricity";
const QString CMainWindow::c_strMenuNameEdit               = "&Edit";
const QString CMainWindow::c_strMenuNameEditRotate         = "Edit:&Rotate";
const QString CMainWindow::c_strMenuNameEditMirror         = "Edit:&Mirror";
const QString CMainWindow::c_strMenuNameView               = "&View";
const QString CMainWindow::c_strMenuNameDebug              = "Debug";
const QString CMainWindow::c_strMenuNameInfo               = "&Info";

const QString CMainWindow::c_strActionNameFileNew                    = c_strMenuNameFile + ":&New ...";
const QString CMainWindow::c_strActionNameFileOpen                   = c_strMenuNameFile + ":&Open ...";
const QString CMainWindow::c_strActionNameFileSave                   = c_strMenuNameFile + ":&Save ...";
const QString CMainWindow::c_strActionNameFileSaveAs                 = c_strMenuNameFile + ":Save &As ...";
//const QString CMainWindow::c_strActionNameFileOpenLastUsed           = c_strMenuNameFile + ":Last Used Files ...";
const QString CMainWindow::c_strActionNameFilePageSetup              = c_strMenuNameFile + ":Page Set&up ...";
const QString CMainWindow::c_strActionNameFileQuit                   = c_strMenuNameFile + ":&Quit";
//const QString CMainWindow::c_strActionNameModeEdit                   = c_strMenuNameMode + ":&Edit";
//const QString CMainWindow::c_strActionNameModeView                   = c_strMenuNameMode + ":&View";
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
const QString CMainWindow::c_strActionNameEditSelect                 = c_strMenuNameDraw + ":&Select";
const QString CMainWindow::c_strActionNameEditRotateLeft             = c_strMenuNameEdit + ":Rotate &Left by ";
const QString CMainWindow::c_strActionNameEditRotateRight            = c_strMenuNameEdit + ":Rotate &Right by ";
const QString CMainWindow::c_strActionNameEditMirrorVertical         = c_strMenuNameEdit + ":Mirror &Vertical";
const QString CMainWindow::c_strActionNameEditMirrorHorizontal       = c_strMenuNameEdit + ":Mirror &Horizontal";
const QString CMainWindow::c_strActionNameEditGroup                  = c_strMenuNameEdit + ":Group";
const QString CMainWindow::c_strActionNameEditUngroup                = c_strMenuNameEdit + ":Ungroup";
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
    const QString&     i_strWindowTitleAppName,
    ZS::Test::CTest*   i_pTest,
    const QStringList& i_strlstObjFactories,
    QWidget*           i_pWdgtParent,
    Qt::WindowFlags    i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent, i_wflags),
    m_strWindowTitleAppName(i_strWindowTitleAppName),
    m_strlstObjFactories(i_strlstObjFactories),
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
    m_pMenuLastUsedFiles(nullptr),
    m_pToolBarFile(nullptr),
    m_pActFileNew(nullptr),
    m_pActFileOpen(nullptr),
    m_pActFileSave(nullptr),
    m_pActFileSaveAs(nullptr),
    m_pActFilePageSetup(nullptr),
    m_arLastUsedFiles(),
    m_arpActsLastUsedFiles(),
    m_bDrawingChangedSinceLastSave(false),
    m_strCurrentFileAbsFilePath(),
    m_dtCurrentFileLastUsed(),
    m_pActFileQuit(nullptr),
    // Menu - Mode
    //m_pMenuMode(nullptr),
    //m_pToolBarMode(nullptr),
    //m_pActModeEdit(nullptr),
    //m_pActModeView(nullptr),
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
    // Menu - Edit
    m_pMenuEdit(nullptr),
    // Menu - Edit - Select
    m_pToolBarEditSelect(nullptr),
    m_pActEditSelect(nullptr),
    // Menu - Edit - Group
    m_pToolBarEditGroup(nullptr),
    m_pActEditGroup(nullptr),
    m_pActEditUngroup(nullptr),
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
    m_pLblStatusBarMainWindowSize(nullptr),
    m_pLblStatusBarDrawingSceneRect(nullptr),
    m_pLblStatusBarDrawingSize(nullptr),
    m_ptDrawingSceneMouseCursorPos(),
    m_pLblStatusBarDrawingSceneMouseCursorPos_px(nullptr),
    m_pLblStatusBarDrawingSceneMouseCursorPos_metric(nullptr),
    m_pLblStatusBarDrawingViewMouseCursorPos(nullptr),
    m_pLblStatusBarDrawingSceneEditTool(nullptr),
    m_pLblStatusBarDrawingSceneEditMode(nullptr),
    m_pLblStatusBarDrawingSceneGraphObjEditInfo(nullptr),
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

    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "WindowTitle: " + i_strWindowTitleAppName;
        strMthInArgs += ", Test: " + QString(i_pTest == nullptr ? "-" : i_pTest->objectName());
        strMthInArgs += ", ObjFactories [" + i_strlstObjFactories.join(", ") + "]";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_pTrcAdminObjMouseEvents = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::Draw", "CMainWindow", objectName() + "-MouseEvents");

    updateWindowTitle();

    // Graph Object Factories
    //-----------------------

    createObjFactories();

    // Central Widget
    //---------------

    m_pWdgtCentral = new CWidgetCentral();
    setCentralWidget(m_pWdgtCentral);

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    QObject::connect(
        pDrawingScene, &CDrawingScene::drawingSizeChanged,
        this, &CMainWindow::onDrawingSceneSizeChanged );
    QObject::connect(
        pDrawingScene, &CDrawingScene::gridSettingsChanged,
        this, &CMainWindow::onDrawingSceneGridSettingsChanged );
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
        pDrawingScene, &CDrawingScene::drawingToolChanged,
        this, &CMainWindow::onDrawingSceneDrawingToolChanged );
    QObject::connect(
        pDrawingScene, &CDrawingScene::drawSettingsChanged,
        this, &CMainWindow::onDrawingSceneDrawSettingsChanged );

    setUnifiedTitleAndToolBarOnMac(true);

    CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();

    QObject::connect(
        pDrawingView, &CDrawingView::mousePosChanged,
        this, &CMainWindow::onDrawingViewMousePosChanged );
    QObject::connect(
        pDrawingView, &CDrawingView::contentAreaChanged,
        this, &CMainWindow::onDrawingViewContentAreaChanged );

    // Actions/Menu/StatusBar/ToolBars/DockWidgets
    //--------------------------------------------

    QSettings settings;

    m_arLastUsedFiles = ZS::System::readLastUsedFiles(settings);
    while (m_arLastUsedFiles.size() > c_iLastUsedFilesCountMax) {
        m_arLastUsedFiles.removeLast();
    }

    createActions();
    createMenus();
    createStatusBar();
    createToolBars();
    createDockWidgets();

    // Restore Geometry
    //-----------------

    restoreGeometry(settings.value(objectName()+"/Geometry").toByteArray());
    restoreState(settings.value(objectName()+"/WindowState").toByteArray());

    // Initialize Status Settings
    //---------------------------

    QString strFactoryGroupName;
    QString strGraphObjType;
    CObjFactory* pObjFactory = pDrawingScene->getCurrentDrawingTool();
    if (pObjFactory != nullptr) {
        strFactoryGroupName = pObjFactory->getGroupName();
        strGraphObjType = pObjFactory->getGraphObjTypeAsString();
    }
    onDrawingSceneModeChanged(pDrawingScene->getMode());
    onDrawingSceneDrawingToolChanged(strFactoryGroupName, strGraphObjType);
    onDrawingSceneSizeChanged(pDrawingScene->drawingSize());
    onDrawingSceneDrawSettingsChanged(pDrawingScene->drawSettings());

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

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QString strRuntimeInfo = __FILE__ + QString(":") + QString::number(__LINE__);
        mthTracer.trace(strRuntimeInfo);
    }

    CDialog::DestroyAllInstances();

    try {
        delete m_pDlgTest;
    }
    catch(...) {
    }
    m_pDlgTest = nullptr;

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QString strRuntimeInfo = __FILE__ + QString(":") + QString::number(__LINE__);
        mthTracer.trace(strRuntimeInfo);
    }

    destroyDockWidgets();

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QString strRuntimeInfo = __FILE__ + QString(":") + QString::number(__LINE__);
        mthTracer.trace(strRuntimeInfo);
    }

    try {
        delete m_pModelObjFactories;
    }
    catch(...) {
    }
    m_pModelObjFactories = nullptr;

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QString strRuntimeInfo = __FILE__ + QString(":") + QString::number(__LINE__);
        mthTracer.trace(strRuntimeInfo);
    }

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

    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QString strRuntimeInfo = __FILE__ + QString(":") + QString::number(__LINE__);
        mthTracer.trace(strRuntimeInfo);
    }

    // Remove status bar section.
    if( m_pLblStatusBarMainWindowSize != nullptr ) {
        statusBar()->removeWidget(m_pLblStatusBarMainWindowSize);
        delete m_pLblStatusBarMainWindowSize;
        m_pLblStatusBarMainWindowSize = nullptr;
    }
    if( m_pLblStatusBarDrawingSceneRect != nullptr ) {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSceneRect);
        delete m_pLblStatusBarDrawingSceneRect;
        m_pLblStatusBarDrawingSceneRect = nullptr;
    }
    if( m_pLblStatusBarDrawingSize != nullptr ) {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSize);
        delete m_pLblStatusBarDrawingSize;
        m_pLblStatusBarDrawingSize = nullptr;
    }
    if( m_pLblStatusBarDrawingSceneMouseCursorPos_px != nullptr ) {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSceneMouseCursorPos_px);
        delete m_pLblStatusBarDrawingSceneMouseCursorPos_px;
        m_pLblStatusBarDrawingSceneMouseCursorPos_px = nullptr;
    }
    if( m_pLblStatusBarDrawingSceneMouseCursorPos_metric != nullptr ) {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSceneMouseCursorPos_metric);
        delete m_pLblStatusBarDrawingSceneMouseCursorPos_metric;
        m_pLblStatusBarDrawingSceneMouseCursorPos_metric = nullptr;
    }
    if( m_pLblStatusBarDrawingViewMouseCursorPos != nullptr ) {
        statusBar()->removeWidget(m_pLblStatusBarDrawingViewMouseCursorPos);
        delete m_pLblStatusBarDrawingViewMouseCursorPos;
        m_pLblStatusBarDrawingViewMouseCursorPos = nullptr;
    }
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
    m_pMenuLastUsedFiles = nullptr;
    m_pToolBarFile = nullptr;
    m_pActFileNew = nullptr;
    m_pActFileOpen = nullptr;
    m_pActFileSave = nullptr;
    m_pActFileSaveAs = nullptr;
    m_pActFilePageSetup = nullptr;
    //m_arLastUsedFiles.clear();
    //m_arpActsLastUsedFiles.clear();
    m_pActFileQuit = nullptr;
    // Menu - Mode
    //m_pMenuMode = nullptr;
    //m_pToolBarMode = nullptr;
    //m_pActModeEdit = nullptr;
    //m_pActModeView = nullptr;
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
    // Menu - Edit
    m_pMenuEdit = nullptr;
    // Menu - Edit - Select
    m_pToolBarEditSelect = nullptr;
    m_pActEditSelect = nullptr;
    // Menu - Edit - Group
    m_pToolBarEditGroup = nullptr;
    m_pActEditGroup = nullptr;
    m_pActEditUngroup = nullptr;
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
    m_pLblStatusBarMainWindowSize = nullptr;
    m_pLblStatusBarDrawingSceneRect = nullptr;
    m_pLblStatusBarDrawingSize = nullptr;
    //m_ptDrawingSceneMouseCursorPos;
    m_pLblStatusBarDrawingSceneMouseCursorPos_px = nullptr;
    m_pLblStatusBarDrawingSceneMouseCursorPos_metric = nullptr;
    m_pLblStatusBarDrawingViewMouseCursorPos = nullptr;
    m_pLblStatusBarDrawingSceneEditTool = nullptr;
    m_pLblStatusBarDrawingSceneEditMode = nullptr;
    m_pLblStatusBarDrawingSceneGraphObjEditInfo = nullptr;
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
            /* strTitle    */ ZS::System::GUI::getMainWindowTitle(),
            /* strText     */ strMsgText,
            /* buttons     */ QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel );
        if( msgBoxBtn == QMessageBox::Save ) {
            if (m_strCurrentFileAbsFilePath.isEmpty()) {
                onActionFileSaveAsTriggered();
            }
            else {
                onActionFileSaveTriggered();
            }
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
void CMainWindow::updateWindowTitle()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateWindowTitle",
        /* strAddInfo   */ "" );

    QString strWindowTitle = m_strWindowTitleAppName;
    QString strAbsFilePath = tr("Unnamed");
    if (!m_strCurrentFileAbsFilePath.isEmpty())
    {
        strAbsFilePath = m_strCurrentFileAbsFilePath;
    }
    //if (m_pWdgtCentral != nullptr)
    //{
    //    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    //    CEnumMode mode = pDrawingScene->getMode();
    //    strWindowTitle += " - " + mode.toString();
    //}
    strWindowTitle += " - " + strAbsFilePath;
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

    // <Menu> Draw
    //============

    // <Menu> Draw::Standard Shapes
    //---------------------------------------------

    // <MenuItem> Draw::Standard Shapes::Draw Point
    //---------------------------------------------

    QPixmap pxmDrawStandardShapesPoint16x16(":/ZS/Draw/Pen16x16.png");
    m_pObjFactoryPoint = new CObjFactoryPoint(pxmDrawStandardShapesPoint16x16);

    // <MenuItem> Draw::Standard Shapes::Draw Line
    //--------------------------------------------

    QPixmap pxmDrawStandardShapesLine16x16(":/ZS/Draw/GraphObjLine16x16.png");
    m_pObjFactoryLine = new CObjFactoryLine(pxmDrawStandardShapesLine16x16);

    // <MenuItem> Draw::Standard Shapes::Draw Rect
    //--------------------------------------------

    QPixmap pxmDrawStandardShapesRect16x16(":/ZS/Draw/GraphObjRect16x16.png");
    m_pObjFactoryRect = new CObjFactoryRect(pxmDrawStandardShapesRect16x16);

    // <MenuItem> Draw::Standard Shapes::Draw Ellipse
    //-----------------------------------------------

    QPixmap pxmDrawStandardShapesEllipse16x16(":/ZS/Draw/GraphObjEllipse16x16.png");
    m_pObjFactoryEllipse = new CObjFactoryEllipse(pxmDrawStandardShapesEllipse16x16);

    // <MenuItem> Draw::Standard Shapes::Draw Polyline
    //------------------------------------------------

    QPixmap pxmDrawStandardShapesPolyline16x16(":/ZS/Draw/GraphObjPolyline16x16.png");
    m_pObjFactoryPolyline = new CObjFactoryPolyline(pxmDrawStandardShapesPolyline16x16);

    // <MenuItem> Draw::Standard Shapes::Draw Polygon
    //------------------------------------------------

    QPixmap pxmDrawStandardShapesPolygon16x16(":/ZS/Draw/GraphObjPolygon16x16.png");
    m_pObjFactoryPolygon = new CObjFactoryPolygon(pxmDrawStandardShapesPolygon16x16);

    // <MenuItem> Draw::Standard Shapes::Draw Text
    //------------------------------------------------

    QPixmap pxmDrawStandardShapesText16x16(":/ZS/Draw/GraphObjText16x16.png");
    m_pObjFactoryText = new CObjFactoryText(pxmDrawStandardShapesText16x16);

    // <Menu> Draw::Graphics
    //----------------------

    // <MenuItem> Draw::Graphics::Image
    //---------------------------------

    QPixmap pxmDrawGraphicsImage16x16(":/ZS/Draw/GraphObjImage16x16.png");
    m_pObjFactoryImage = new CObjFactoryImage(pxmDrawGraphicsImage16x16);

    // <Menu> Draw::Connections
    //-------------------------

    // <MenuItem> Draw::Connections::Draw Connection Point
    //----------------------------------------------------

    QPixmap pxmDrawConnectionPoint16x16(":/ZS/Draw/GraphObjConnectionPoint16x16.png");
    m_pObjFactoryConnectionPoint = new CObjFactoryConnectionPoint(pxmDrawConnectionPoint16x16);

    // <MenuItem> Draw::Connections::Draw Connection Line
    //----------------------------------------------------

    QPixmap pxmDrawConnectionLine16x16(":/ZS/Draw/GraphObjConnectionLine16x16.png");
    m_pObjFactoryConnectionLine = new CObjFactoryConnectionLine(pxmDrawConnectionLine16x16);

    // <Menu> Draw::Widgets
    //----------------------

    if( m_strlstObjFactories.contains(c_strObjFactoryQtWidgets) )
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
    }

    // <Menu> Draw::Electricity
    //-------------------------

    if( m_strlstObjFactories.contains(c_strObjFactoryElectricity) )
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
    }

    // <Menu> Edit
    //============

    // <MenuItem> Edit::Group
    //-----------------------

    QPixmap pxmEditGroup16x16(":/ZS/Draw/EditGroup16x16.png");
    m_pObjFactoryGroup = new CObjFactoryGroup(pxmEditGroup16x16);

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
    QPixmap pxmFileNew16x16(":/ZS/Menu/MenuFileNew16x16.png");
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
    QPixmap pxmFileOpen16x16(":/ZS/Menu/MenuFileOpen16x16.png");
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
    QPixmap pxmFileSave16x16(":/ZS/Menu/MenuFileSave16x16.png");
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
    QPixmap pxmFileSaveAs16x16(":/ZS/Menu/MenuFileSaveAs16x16.png");
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

    QIcon iconFilePageSetup;
    QPixmap pxmFilePageSetup16x16(":/ZS/Draw/PageSetup16x16.png");
    iconFilePageSetup.addPixmap(pxmFilePageSetup16x16);

    m_pActFilePageSetup = new QAction( iconFilePageSetup, c_strActionNameFilePageSetup.section(":",-1,-1), this );
    m_pActFilePageSetup->setShortcuts(QKeySequence::Preferences);
    m_pActFilePageSetup->setStatusTip(tr("Set properties of current page"));

    QObject::connect(
        m_pActFilePageSetup, &QAction::triggered,
        this, &CMainWindow::onActionFilePageSetupTriggered );

    // <MenuItem> File::Quit
    //----------------------

    m_pActFileQuit = new QAction( c_strActionNameFileQuit.section(":",-1,-1), this );
    m_pActFileQuit->setStatusTip( tr("Quit application") );

    QObject::connect(
        m_pActFileQuit, &QAction::triggered,
        this, &CMainWindow::onActionFileQuitTriggered );

    // <Menu> Mode
    //============

    // <MenuItem> Mode::Edit
    //----------------------

    //QIcon iconModeEdit;
    //iconModeEdit.addPixmap( mode2Pixmap(static_cast<int>(EMode::Edit),24) );

    //m_pActModeEdit = new QAction( iconModeEdit, c_strActionNameModeEdit.section(":",-1,-1), this );
    //m_pActModeEdit->setStatusTip( tr("Activate Edit Mode") );
    //m_pActModeEdit->setCheckable(true);
    //m_pActModeEdit->setChecked(true);

    //QObject::connect(
    //    m_pActModeEdit, &QAction::triggered,
    //    this, &CMainWindow::onActionModeEditToggled );

    //// <MenuItem> Mode::View
    ////-------------------------------

    //QIcon iconModeView;
    //iconModeView.addPixmap( mode2Pixmap(static_cast<int>(EMode::View),24) );

    //m_pActModeView = new QAction( iconModeView, c_strActionNameModeView.section(":",-1,-1), this );
    //m_pActModeView->setStatusTip( tr("Activate View Mode") );
    //m_pActModeView->setCheckable(true);

    //QObject::connect(
    //    m_pActModeView, &QAction::triggered,
    //    this, &CMainWindow::onActionModeViewToggled );

    // <Menu> Draw
    //============

    // <Menu> Draw::Settings
    //----------------------

    // <MenuItem> Draw::Settings::Line
    //--------------------------------

    QIcon iconEditDrawSettingsLine;
    QPixmap pxmEditDrawSettingsLine(":/ZS/Draw/DrawSettingsLineStyle16x16.png");
    iconEditDrawSettingsLine.addPixmap(pxmEditDrawSettingsLine);

    m_pActDrawSettingsLine = new QAction( iconEditDrawSettingsLine, c_strActionNameDrawSettingsLine.section(":",-1,-1), this );
    m_pActDrawSettingsLine->setStatusTip( tr("Draw Settings Line Style") );

    QObject::connect(
        m_pActDrawSettingsLine, &QAction::triggered,
        this, &CMainWindow::onActionDrawSettingsLineTriggered );

    // <MenuItem> Draw::Settings::Fill
    //---------------------------------

    QIcon iconEditDrawSettingsFill;
    QPixmap pxmEditDrawSettingsFill(":/ZS/Draw/DrawSettingsBrushStyle16x16.png");
    iconEditDrawSettingsFill.addPixmap(pxmEditDrawSettingsFill);

    m_pActDrawSettingsFill = new QAction(iconEditDrawSettingsFill, c_strActionNameDrawSettingsFill.section(":",-1,-1), this);
    m_pActDrawSettingsFill->setStatusTip( tr("Draw Settings Fill Style") );

    QObject::connect(
        m_pActDrawSettingsFill, &QAction::triggered,
        this, &CMainWindow::onActionDrawSettingsFillTriggered );

    // <MenuItem> Draw::Settings::Text
    //---------------------------------

    QIcon iconEditDrawSettingsText;
    QPixmap pxmEditDrawSettingsText(":/ZS/Draw/DrawSettingsTextStyle16x16.png");
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
    QPixmap pxmDrawPoint(":/ZS/Draw/Pen16x16.png");
    iconDrawPoint.addPixmap(pxmDrawPoint);

    m_pActDrawStandardShapePoint = new QAction(iconDrawPoint, c_strActionNameDrawStandardShapePoint.section(":",-1,-1), this);
    m_pActDrawStandardShapePoint->setStatusTip(tr("Draw Points"));
    m_pActDrawStandardShapePoint->setCheckable(true);
    m_pActDrawStandardShapePoint->setEnabled(false);

    QObject::connect(
        m_pActDrawStandardShapePoint, &QAction::triggered,
        this, &CMainWindow::onActionDrawStandardShapePointToggled );

    // <MenuItem> Draw::Standard Shapes::Draw Line
    //--------------------------------------------

    QIcon iconDrawLine;
    QPixmap pxmDrawLine(":/ZS/Draw/GraphObjLine16x16.png");
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
    QPixmap pxmDrawRect(":/ZS/Draw/GraphObjRect16x16.png");
    iconDrawRect.addPixmap(pxmDrawRect);

    m_pActDrawStandardShapeRect = new QAction(iconDrawRect, c_strActionNameDrawStandardShapeRect.section(":",-1,-1), this);
    m_pActDrawStandardShapeRect->setStatusTip(tr("Draw Rectangles"));
    m_pActDrawStandardShapeRect->setCheckable(true);
    m_pActDrawStandardShapeRect->setEnabled(false);

    QObject::connect(
        m_pActDrawStandardShapeRect, &QAction::triggered,
        this, &CMainWindow::onActionDrawStandardShapeRectToggled );

    // <MenuItem> Draw::Standard Shapes::Draw Ellipse
    //-----------------------------------------------

    QIcon iconDrawEllipse;
    QPixmap pxmDrawEllipse(":/ZS/Draw/GraphObjEllipse16x16.png");
    iconDrawEllipse.addPixmap(pxmDrawEllipse);

    m_pActDrawStandardShapeEllipse = new QAction(iconDrawEllipse, c_strActionNameDrawStandardShapeEllipse.section(":",-1,-1), this);
    m_pActDrawStandardShapeEllipse->setStatusTip(tr("Draw Ellipses"));
    m_pActDrawStandardShapeEllipse->setCheckable(true);
    m_pActDrawStandardShapeEllipse->setEnabled(false);

    QObject::connect(
        m_pActDrawStandardShapeEllipse, &QAction::triggered,
        this, &CMainWindow::onActionDrawStandardShapeEllipseToggled );

    // <MenuItem> Draw::Standard Shapes::Draw Polyline
    //------------------------------------------------

    QIcon iconDrawPolyline;
    QPixmap pxmDrawPolyline(":/ZS/Draw/GraphObjPolyline16x16.png");
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
    QPixmap pxmDrawPolygon(":/ZS/Draw/GraphObjPolygon16x16.png");
    iconDrawPolygon.addPixmap(pxmDrawPolygon);

    m_pActDrawStandardShapePolygon = new QAction(iconDrawPolygon, c_strActionNameDrawStandardShapePolygon.section(":",-1,-1), this);
    m_pActDrawStandardShapePolygon->setStatusTip(tr("Draw Polygons"));
    m_pActDrawStandardShapePolygon->setCheckable(true);
    m_pActDrawStandardShapePolygon->setEnabled(false);

    QObject::connect(
        m_pActDrawStandardShapePolygon, &QAction::triggered,
        this, &CMainWindow::onActionDrawStandardShapePolygonToggled );

    // <MenuItem> Draw::Standard Shapes::Draw Text
    //------------------------------------------------

    QIcon iconDrawText;
    QPixmap pxmDrawText(":/ZS/Draw/GraphObjText16x16.png");
    iconDrawText.addPixmap(pxmDrawText);

    m_pActDrawStandardShapeText = new QAction(iconDrawText, c_strActionNameDrawStandardShapeText.section(":",-1,-1), this);
    m_pActDrawStandardShapeText->setStatusTip(tr("Draw Texts"));
    m_pActDrawStandardShapeText->setCheckable(true);
    m_pActDrawStandardShapeText->setEnabled(false);

    QObject::connect(
        m_pActDrawStandardShapeText, &QAction::triggered,
        this, &CMainWindow::onActionDrawStandardShapeTextToggled );

    // <Menu> Draw::Graphics
    //----------------------

    // <MenuItem> Draw::Graphics::Image
    //---------------------------------

    QIcon iconDrawImage;
    QPixmap pxmDrawImage(":/ZS/Draw/GraphObjImage16x16.png");
    iconDrawImage.addPixmap(pxmDrawImage);

    m_pActDrawGraphicsImage = new QAction(iconDrawImage, c_strActionNameDrawGraphicsImage.section(":",-1,-1), this);
    m_pActDrawGraphicsImage->setStatusTip(tr("Insert Images"));
    m_pActDrawGraphicsImage->setCheckable(false);
    m_pActDrawGraphicsImage->setEnabled(false);

    QObject::connect(
        m_pActDrawGraphicsImage, &QAction::triggered,
        this, &CMainWindow::onActionDrawGraphicsImageTriggered );

    // <Menu> Draw::Connections
    //-------------------------

    // <MenuItem> Draw::Connections::Draw Connection Point
    //----------------------------------------------------

    QIcon iconDrawConnectionPoint;
    QPixmap pxmDrawConnectionPoint(":/ZS/Draw/GraphObjConnectionPoint16x16.png");
    iconDrawConnectionPoint.addPixmap(pxmDrawConnectionPoint);

    m_pActDrawConnectionPoint = new QAction(iconDrawConnectionPoint, c_strActionNameDrawConnectionPoint.section(":",-1,-1), this);
    m_pActDrawConnectionPoint->setStatusTip(tr("Draw Connection Point"));
    m_pActDrawConnectionPoint->setCheckable(true);
    m_pActDrawConnectionPoint->setEnabled(false);

    QObject::connect(
        m_pActDrawConnectionPoint, &QAction::triggered,
        this, &CMainWindow::onActionDrawConnectionPointToggled );

    // <MenuItem> Draw::Connections::Draw Connection Line
    //----------------------------------------------------

    QIcon iconDrawConnectionLine;
    QPixmap pxmDrawConnectionLine(":/ZS/Draw/GraphObjConnectionLine16x16.png");
    iconDrawConnectionLine.addPixmap(pxmDrawConnectionLine);

    m_pActDrawConnectionLine = new QAction(iconDrawConnectionLine, c_strActionNameDrawConnectionLine.section(":",-1,-1), this);
    m_pActDrawConnectionLine->setStatusTip(tr("Draw Connection Line"));
    m_pActDrawConnectionLine->setCheckable(true);
    m_pActDrawConnectionLine->setEnabled(false);

    QObject::connect(
        m_pActDrawConnectionLine, &QAction::triggered,
        this, &CMainWindow::onActionDrawConnectionLineToggled );

    // <Menu> Edit
    //============

    // <MenuItem> Edit::Select
    //-------------------------------

    QIcon iconEditSelect;
    QPixmap pxmEditSelect16x16(":/ZS/Draw/CursorSelect16x16.png");
    iconEditSelect.addPixmap(pxmEditSelect16x16);

    m_pActEditSelect = new QAction( iconEditSelect, c_strActionNameEditSelect.section(":",-1,-1), this );
    m_pActEditSelect->setStatusTip( tr("Select Object(s)") );
    m_pActEditSelect->setCheckable(true);

    QObject::connect(
        m_pActEditSelect, &QAction::triggered,
        this, &CMainWindow::onActionEditSelectToggled );

    // <MenuItem> Edit::Group
    //-----------------------

    QIcon iconEditGroup;
    QPixmap pxmEditGroup16x16(":/ZS/Draw/EditGroup16x16.png");
    iconEditGroup.addPixmap(pxmEditGroup16x16);

    m_pActEditGroup = new QAction(iconEditGroup, c_strActionNameEditGroup.section(":",-1,-1), this);
    m_pActEditGroup->setStatusTip(tr("Group Selected Objects"));
    m_pActEditGroup->setEnabled(false);

    QObject::connect(
        m_pActEditGroup, &QAction::triggered,
        this, &CMainWindow::onActionEditGroupTriggered );

    // <MenuItem> Edit::Ungroup
    //-------------------------

    QIcon iconEditUngroup;
    QPixmap pxmEditUngroup16x16(":/ZS/Draw/EditUngroup16x16.png");
    iconEditUngroup.addPixmap(pxmEditUngroup16x16);

    m_pActEditUngroup = new QAction( iconEditUngroup, c_strActionNameEditUngroup.section(":",-1,-1), this );
    m_pActEditUngroup->setStatusTip( tr("Ungroup Selected Group(s)") );
    m_pActEditUngroup->setEnabled(false);

    QObject::connect(
        m_pActEditUngroup, &QAction::triggered,
        this, &CMainWindow::onActionEditUngroupTriggered );

    // <MenuItem> Edit::Rotate::Left
    //------------------------------

    QIcon iconEditRotateLeft;
    QPixmap pxmEditRotateLeft(":/ZS/Draw/DrawToolRotateLeftBy90Degrees16x16.png");
    iconEditRotateLeft.addPixmap(pxmEditRotateLeft);

    m_pActEditRotateLeft = new QAction( iconEditRotateLeft, c_strActionNameEditRotateLeft.section(":",-1,-1), this );
    m_pActEditRotateLeft->setStatusTip( tr("Rotate Selected Object(s) Left (counterclockwise)") );
    m_pActEditRotateLeft->setEnabled(false);

    QObject::connect(
        m_pActEditRotateLeft, &QAction::triggered,
        this, &CMainWindow::onActionEditRotateLeftTriggered );

    // <MenuItem> Edit::Rotate::Right
    //-------------------------------

    QIcon iconEditRotateRight;
    QPixmap pxmEditRotateRight(":/ZS/Draw/DrawToolRotateRightBy90Degrees16x16.png");
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

    QIcon iconEditMirrorVertical;
    QPixmap pxmEditMirrorVertical(":/ZS/Draw/DrawToolMirrorVertical16x16.png");
    iconEditMirrorVertical.addPixmap(pxmEditMirrorVertical);

    m_pActEditMirrorVertical = new QAction( iconEditMirrorVertical, c_strActionNameEditMirrorVertical.section(":",-1,-1), this );
    m_pActEditMirrorVertical->setStatusTip( tr("Vertically Mirror Selected Object(s)") );
    m_pActEditMirrorVertical->setEnabled(false);

    QObject::connect(
        m_pActEditMirrorVertical, &QAction::triggered,
        this, &CMainWindow::onActionEditMirrorVerticalTriggered );

    // <MenuItem> Edit::Mirror::Horizontal
    //------------------------------------

    QIcon iconEditMirrorHorizontal;
    QPixmap pxmEditMirrorHorizontal(":/ZS/Draw/DrawToolMirrorHorizontal16x16.png");
    iconEditMirrorHorizontal.addPixmap(pxmEditMirrorHorizontal);

    m_pActEditMirrorHorizontal = new QAction( iconEditMirrorHorizontal, c_strActionNameEditMirrorHorizontal.section(":",-1,-1), this );
    m_pActEditMirrorHorizontal->setStatusTip( tr("Horizontally Mirror Selected Object(s)") );
    m_pActEditMirrorHorizontal->setEnabled(false);

    QObject::connect(
        m_pActEditMirrorHorizontal, &QAction::triggered,
        this, &CMainWindow::onActionEditMirrorHorizontalTriggered );

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

    QIcon iconFileOpen;
    QPixmap pxmFileOpen16x16(":/ZS/Menu/MenuFileOpen16x16.png");
    pxmFileOpen16x16.setMask(pxmFileOpen16x16.createHeuristicMask());
    iconFileOpen.addPixmap(pxmFileOpen16x16);

    m_pMenuFile = m_pMenuBar->addMenu(c_strMenuNameFile);

    m_pMenuFile->addAction(m_pActFileNew);
    m_pMenuFile->addAction(m_pActFileOpen);
    m_pMenuFile->addSeparator();
    m_pMenuFile->addAction(m_pActFileSave);
    m_pMenuFile->addAction(m_pActFileSaveAs);
    m_pMenuFile->addSeparator();
    m_pMenuLastUsedFiles = m_pMenuFile->addMenu(iconFileOpen, c_strMenuNameFileOpenLastUsed);
    m_pMenuFile->addSeparator();
    m_pMenuFile->addAction(m_pActFilePageSetup);

    if( m_arLastUsedFiles.size() == 0 ) {
        m_pMenuLastUsedFiles->setEnabled(false);
    }

    if( m_pActFileQuit != nullptr ) {
        m_pMenuFile->addSeparator();
        m_pMenuFile->addAction(m_pActFileQuit);
    }

    // <Menu> Mode
    //------------

    //m_pMenuMode = m_pMenuBar->addMenu(c_strMenuNameMode);

    //if( m_pActModeEdit != nullptr ) {
    //    m_pMenuMode->addAction(m_pActModeEdit);
    //}
    //if( m_pActModeView != nullptr ) {
    //    m_pMenuMode->addAction(m_pActModeView);
    //}

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

    QString strStyleSheet = "QStatusBar { " \
        "background: qlineargradient( " \
            "x1: 0, y1: 0, x2: 0, y2: 1, " \
            "stop: 0 #E1E1E1, stop: 0.4 #DDDDDD, " \
            "stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3); } ";
    statusBar()->setStyleSheet(strStyleSheet);

    //CPageSetup* pageSetup = m_pDrawingView->getPageSetup();
    //CUnit unitWidth = pageSetup->unit(EOrientation::Horizontal);

    m_pLblStatusBarMainWindowSize = new QLabel("MainWindow {-, -} px");
    statusBar()->addWidget(m_pLblStatusBarMainWindowSize);
    QWidget* pWdgtSpacer = new QWidget();
    pWdgtSpacer->setFixedWidth(10);
    statusBar()->addWidget(pWdgtSpacer);

    m_pLblStatusBarDrawingSceneRect = new QLabel("SceneRect {-, -} px");
    statusBar()->addWidget(m_pLblStatusBarDrawingSceneRect);
    pWdgtSpacer = new QWidget();
    pWdgtSpacer->setFixedWidth(10);
    statusBar()->addWidget(pWdgtSpacer);

    m_pLblStatusBarDrawingSize = new QLabel("DrawingSize {-, -} mm");
    statusBar()->addWidget(m_pLblStatusBarDrawingSize);
    pWdgtSpacer = new QWidget();
    pWdgtSpacer->setFixedWidth(10);
    statusBar()->addWidget(pWdgtSpacer);

    m_pLblStatusBarDrawingSceneMouseCursorPos_px = new QLabel("MousePos {-, -} px");
    statusBar()->addWidget(m_pLblStatusBarDrawingSceneMouseCursorPos_px);
    pWdgtSpacer = new QWidget();
    pWdgtSpacer->setFixedWidth(10);
    statusBar()->addWidget(pWdgtSpacer);

    m_pLblStatusBarDrawingSceneMouseCursorPos_metric = new QLabel("MousePos {-, -} mm");
    statusBar()->addWidget(m_pLblStatusBarDrawingSceneMouseCursorPos_metric);
    pWdgtSpacer = new QWidget();
    pWdgtSpacer->setFixedWidth(10);
    statusBar()->addWidget(pWdgtSpacer);

    //m_pLblStatusBarDrawingViewMouseCursorPos = new QLabel( "ViewPos: -/- [" + unitWidth.symbol() + "]" );
    //m_pLblStatusBarDrawingViewMouseCursorPos->setMinimumWidth(140);
    //statusBar()->addPermanentWidget(m_pLblStatusBarDrawingViewMouseCursorPos);

    //m_pLblStatusBarDrawingSceneEditTool = new QLabel("Tool: -");
    //m_pLblStatusBarDrawingSceneEditTool->setMinimumWidth(80);
    //statusBar()->addPermanentWidget(m_pLblStatusBarDrawingSceneEditTool);

    //m_pLblStatusBarDrawingSceneEditMode = new QLabel("Mode: ");
    //m_pLblStatusBarDrawingSceneEditMode->setMinimumWidth(100);
    //statusBar()->addPermanentWidget(m_pLblStatusBarDrawingSceneEditMode);

    //m_pLblStatusBarDrawingSceneGraphObjEditInfo = new QLabel("Edit: -");
    //m_pLblStatusBarDrawingSceneGraphObjEditInfo->setMinimumWidth(120);
    //statusBar()->addPermanentWidget(m_pLblStatusBarDrawingSceneGraphObjEditInfo);

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

    //m_pToolBarMode = addToolBar("Switching Between View and Edit Mode");
    //m_pToolBarMode->setObjectName("Mode");
    ////m_pToolBarMode->setMaximumHeight(24);
    //m_pToolBarMode->setIconSize( QSize(16,16) );

    //if( m_pActModeEdit != nullptr ) {
    //    m_pToolBarMode->addAction(m_pActModeEdit);
    //}
    //if( m_pActModeView != nullptr ) {
    //    m_pToolBarMode->addAction(m_pActModeView);
    //}

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
    m_pEdtEditRotateAngle->setSuffix(" " +  QString(ZS::System::Math::c_chSymbolDegree));
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

    // <DockWidget> Tree View Object Factories
    //----------------------------------------

    if( !m_strlstObjFactories.isEmpty() )
    {
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
    }

    // Dock Widget - GraphObjs
    //------------------------

    m_pDockWdgtGraphObjs = new QDockWidget("Graphical Objects");
    m_pDockWdgtGraphObjs->setObjectName("Graphical Objects");
    m_pDockWdgtGraphObjs->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

    // Tree View with graphics items as in drawing scene's items list
    m_pWdgtGraphObjs = new CWdgtGraphObjs(m_pWdgtCentral->drawingView());
    m_pDockWdgtGraphObjs->setWidget(m_pWdgtGraphObjs);

    addDockWidget(Qt::RightDockWidgetArea, m_pDockWdgtGraphObjs);

    QSettings settings;
    m_pWdgtGraphObjs->restoreState(settings);

    if( m_pMenuView != nullptr ) {
        if( !m_pMenuView->isEmpty() ) {
            m_pMenuView->addSeparator();
        }
        m_pMenuView->addAction(m_pDockWdgtGraphObjs->toggleViewAction());
    }
} // createDockWidgets

//------------------------------------------------------------------------------
void CMainWindow::destroyDockWidgets()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "destroyDockWidgets",
        /* strAddInfo   */ "" );

    delete m_pDockWdgtObjFactories;
    m_pDockWdgtObjFactories = nullptr;

    delete m_pDockWdgtGraphObjs;
    m_pDockWdgtGraphObjs = nullptr;
}

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
bool CMainWindow::eventFilter( QObject* i_pObjWatched, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Obj: " + QString(i_pObjWatched == nullptr ? "nullptr" : i_pObjWatched->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "eventFilter",
        /* strAddInfo   */ strMthInArgs );

    bool bHandled = false;

    if (i_pObjWatched == m_pLblErrors) {
        if (i_pEv->type() == QEvent::MouseButtonDblClick) {
            onActionDebugErrLogTriggered();
            bHandled = true;
        }
    }
    else {
        // pass the event on to the parent class
        bHandled = QMainWindow::eventFilter(i_pObjWatched,i_pEv);
    }
    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(bHandled);
    }
    return bHandled;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::showFilePageSetupDialog( bool i_bModal )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Modal: " + bool2Str(i_bModal);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showFilePageSetupDialog",
        /* strAddInfo   */ strMthInArgs );

    CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Page Setup";
    CDlgDrawingViewProperties* pDlg = CDlgDrawingViewProperties::GetInstance(pDrawingView);
    if( pDlg == nullptr ) {
        pDlg = CDlgDrawingViewProperties::CreateInstance(strDlgTitle, pDrawingView);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->setModal(i_bModal);
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
void CMainWindow::setCurrentUsedFile( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_strAbsFilePath;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCurrentUsedFile",
        /* strAddInfo   */ strMthInArgs );

    if (!i_strAbsFilePath.isEmpty()) {
        SErrResultInfo errResultInfo;
        QFileInfo fileInfo(i_strAbsFilePath);

        if (!fileInfo.isFile() || !fileInfo.exists()) {
            errResultInfo = ErrResultInfoError("setCurrentUsedFile", EResultFileNotFound, i_strAbsFilePath);
        }
        else {
            CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
            // Clear drawing on opening a new file.
            pDrawingScene->clear();
            errResultInfo = pDrawingScene->load(i_strAbsFilePath);
        }

        if (!errResultInfo.isErrorResult()) {
            updateCurrentUsedFile(i_strAbsFilePath);
        }
        else {
            QMessageBox::StandardButtons msgBoxBtns = QMessageBox::Ok;
            QString strMsg;
            strMsg  = "Error on reading file \"" + i_strAbsFilePath + "\"";
            strMsg += "\n\nErrorCode:\t" + errResultInfo.getResultStr();
            if (errResultInfo.getAddErrInfoDscr() != i_strAbsFilePath) {
                strMsg += "\n\n" + errResultInfo.getAddErrInfoDscr();
            }
            if (errResultInfo.getResult() == EResultFileNotFound) {
                strMsg += "\n\nDo you want to remove the file from list of last open files?";
                msgBoxBtns = QMessageBox::Yes | QMessageBox::No;
            }
            QMessageBox::StandardButton msgBoxBtnPressed;
            if (errResultInfo.getSeverity() == EResultSeverityCritical) {
                msgBoxBtnPressed = QMessageBox::critical(this, windowTitle(), strMsg, msgBoxBtns);
            }
            else {
                msgBoxBtnPressed = QMessageBox::warning(this, windowTitle(), strMsg, msgBoxBtns);
            }
            if (msgBoxBtnPressed == QMessageBox::Yes) {
                for (int idxFile = 0; idxFile < m_arLastUsedFiles.size(); ++idxFile) {
                    if (m_arLastUsedFiles[idxFile].m_strAbsFilePath == i_strAbsFilePath) {
                        m_arLastUsedFiles.removeAt(idxFile);
                        break;
                    }
                }
                QSettings settings;
                ZS::System::writeLastUsedFiles(m_arLastUsedFiles, settings);
                updateActionsLastUsedFiles();
            }
        }
    }
} // setCurrentUsedFile

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CMainWindow::setCheckedActionModeEdit(bool i_bChecked)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "setCheckedActionModeEdit",
//        /* strAddInfo   */ strMthInArgs );
//
//    if( m_pActModeEdit != nullptr )
//    {
//        m_pActModeEdit->setChecked(i_bChecked);
//    }
//}
//
////------------------------------------------------------------------------------
//void CMainWindow::setCheckedActionModeView(bool i_bChecked)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "setCheckedActionModeView",
//        /* strAddInfo   */ strMthInArgs );
//
//    if( m_pActModeView != nullptr )
//    {
//        m_pActModeView->setChecked(i_bChecked);
//    }
//}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapePoint(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapePoint",
        /* strAddInfo   */ strMthInArgs );

    if (m_pActDrawStandardShapePoint != nullptr) {
        m_pActDrawStandardShapePoint->setChecked(i_bChecked);
    }
}

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapeLine(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapeLine",
        /* strAddInfo   */ strMthInArgs );

    if (m_pActDrawStandardShapeLine != nullptr) {
        m_pActDrawStandardShapeLine->setChecked(i_bChecked);
    }
}

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapeRect(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapeRect",
        /* strAddInfo   */ strMthInArgs );

    if (m_pActDrawStandardShapeRect != nullptr) {
        m_pActDrawStandardShapeRect->setChecked(i_bChecked);
    }
}

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapeEllipse(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapeEllipse",
        /* strAddInfo   */ strMthInArgs );

    if (m_pActDrawStandardShapeEllipse != nullptr) {
        m_pActDrawStandardShapeEllipse->setChecked(i_bChecked);
    }
}

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapePolyline(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapePolyline",
        /* strAddInfo   */ strMthInArgs );

    if (m_pActDrawStandardShapePolyline != nullptr) {
        m_pActDrawStandardShapePolyline->setChecked(i_bChecked);
    }
}

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapePolygon(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapePolygon",
        /* strAddInfo   */ strMthInArgs );

    if (m_pActDrawStandardShapePolygon != nullptr) {
        m_pActDrawStandardShapePolygon->setChecked(i_bChecked);
    }
}

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapeText(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawStandardShapeText",
        /* strAddInfo   */ strMthInArgs );

    if (m_pActDrawStandardShapeText != nullptr) {
        m_pActDrawStandardShapeText->setChecked(i_bChecked);
    }
}

//------------------------------------------------------------------------------
void CMainWindow::triggerActionDrawGraphicsImage()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionDrawGraphicsImage",
        /* strAddInfo   */ "" );

    if (m_pActDrawGraphicsImage != nullptr) {
        m_pActDrawGraphicsImage->trigger();
    }
}

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawConnectionPoint(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawConnectionPoint",
        /* strAddInfo   */ strMthInArgs );

    if (m_pActDrawConnectionPoint != nullptr) {
        m_pActDrawConnectionPoint->setChecked(i_bChecked);
    }
}

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawConnectionLine(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionDrawConnectionLine",
        /* strAddInfo   */ strMthInArgs );

    if (m_pActDrawConnectionLine != nullptr) {
        m_pActDrawConnectionLine->setChecked(i_bChecked);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionEditSelect(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setCheckedActionEditSelect",
        /* strAddInfo   */ strMthInArgs );

    if (m_pActEditSelect != nullptr) {
        m_pActEditSelect->setChecked(i_bChecked);
    }
}

//------------------------------------------------------------------------------
void CMainWindow::triggerActionEditRotateLeft()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionEditRotateLeft",
        /* strAddInfo   */ "" );

    if (m_pActEditRotateLeft != nullptr) {
        m_pActEditRotateLeft->trigger();
    }
}

//------------------------------------------------------------------------------
void CMainWindow::triggerActionEditRotateRight()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionEditRotateRight",
        /* strAddInfo   */ "" );

    if (m_pActEditRotateRight != nullptr) {
        m_pActEditRotateRight->trigger();
    }
}

//------------------------------------------------------------------------------
void CMainWindow::triggerActionEditMirrorVertical()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionEditMirrorVertical",
        /* strAddInfo   */ "" );

    if (m_pActEditMirrorVertical != nullptr) {
        m_pActEditMirrorVertical->trigger();
    }
}

//------------------------------------------------------------------------------
void CMainWindow::triggerActionEditMirrorHorizontal()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionEditMirrorHorizontal",
        /* strAddInfo   */ "" );

    if (m_pActEditMirrorHorizontal != nullptr) {
        m_pActEditMirrorHorizontal->trigger();
    }
}

//------------------------------------------------------------------------------
void CMainWindow::triggerActionEditGroup()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionEditGroup",
        /* strAddInfo   */ "" );

    if (m_pActEditGroup != nullptr) {
        m_pActEditGroup->trigger();
    }
}

//------------------------------------------------------------------------------
void CMainWindow::triggerActionEditUngroup()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "triggerActionEditUngroup",
        /* strAddInfo   */ "" );

    if (m_pActEditUngroup != nullptr) {
        m_pActEditUngroup->trigger();
    }
}

/*==============================================================================
public slots: // Menu - File
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionFileNewTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFileNewTriggered",
        /* strAddInfo   */ strMthInArgs );

    m_pWdgtCentral->drawingScene()->clear();
    updateCurrentUsedFile("");
}

//------------------------------------------------------------------------------
void CMainWindow::onActionFileOpenTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFileOpenTriggered",
        /* strAddInfo   */ strMthInArgs );

    QDir dir;
    if (m_strCurrentFileAbsFilePath.isEmpty()) {
        QString strDocsDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        dir.setPath(strDocsDir);
    }
    else {
        QFileInfo fileInfoCurrentFile(m_strCurrentFileAbsFilePath);
        dir.setPath(fileInfoCurrentFile.absolutePath());
    }
    QString strAbsFilePath = QFileDialog::getOpenFileName(
        /* pWdgtParent */ this,
        /* strCaption  */ "Choose a file to open",
        /* strDir      */ dir.absolutePath(),
        /* strFilter   */ "Drawings (*.xml)" );

    if (!strAbsFilePath.isEmpty()) {
        setCurrentUsedFile(strAbsFilePath);
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onActionFileSaveTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFileSaveTriggered",
        /* strAddInfo   */ strMthInArgs );

    if( m_strCurrentFileAbsFilePath.isEmpty() ) {
        onActionFileSaveAsTriggered();
    }
    else {
        CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
        SErrResultInfo errResultInfo = pDrawingScene->save(m_strCurrentFileAbsFilePath);
        if (errResultInfo.isErrorResult()) {
            QString strMsg;
            strMsg  = "Error on saving file \"" + m_strCurrentFileAbsFilePath + "\"";
            strMsg += "\n\nErrorCode:\t" + errResultInfo.getResultStr();
            strMsg += "\n\n" + errResultInfo.getAddErrInfoDscr();
            if( errResultInfo.getSeverity() == EResultSeverityCritical ) {
                QMessageBox::critical(
                    /* pWdgtParent */ this,
                    /* strTitly    */ windowTitle(),
                    /* strText     */ strMsg,
                    /* buttons     */ QMessageBox::Ok );
            } else {
                QMessageBox::warning(
                    /* pWdgtParent */ this,
                    /* strTitly    */ windowTitle(),
                    /* strText     */ strMsg,
                    /* buttons     */ QMessageBox::Ok );
            }
        }
        else {
            updateCurrentUsedFile(m_strCurrentFileAbsFilePath);
        }
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onActionFileSaveAsTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFileSaveAsTriggered",
        /* strAddInfo   */ strMthInArgs );

    QDir dir;
    if (m_strCurrentFileAbsFilePath.isEmpty()) {
        QString strDocsDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        dir.setPath(strDocsDir);
    }
    else {
        QFileInfo fileInfoCurrentFile(m_strCurrentFileAbsFilePath);
        dir.setPath(fileInfoCurrentFile.absolutePath());
    }
    QString strAbsFilePath = QFileDialog::getSaveFileName(
        /* pWdgtParent */ this,
        /* strCaption  */ "Choose a filename to save under",
        /* strDir      */ dir.absolutePath(),
        /* strFilter   */ "XML File (*.xml);; Image (*.png);; Image (*.jpg);; Image (*.bmp)" );

    if( !strAbsFilePath.isEmpty() ) {
        SErrResultInfo errResultInfo;
        CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
        if( strAbsFilePath.toLower().endsWith(".xml") ) {
            errResultInfo = pDrawingScene->save(strAbsFilePath);
        }
        else {
            const CDrawingSize& drawingSize = pDrawingScene->drawingSize();
            QImage img(
                drawingSize.imageSizeInPixels().width(),
                drawingSize.imageSizeInPixels().height(),
                QImage::Format_ARGB32_Premultiplied);
            QPainter painter(&img);
            pDrawingScene->render(&painter);
            painter.end();
            if( !img.save(strAbsFilePath) ) {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultFileWriteContent);
            }
        }
        if( !errResultInfo.isErrorResult() ) {
            updateCurrentUsedFile(strAbsFilePath);
        }
        else {
            QString strMsg;
            strMsg  = "Error on writing file \"" + strAbsFilePath + "\"";
            strMsg += "\n\nErrorCode:\t" + errResultInfo.getResultStr();
            strMsg += "\n\n" + errResultInfo.getAddErrInfoDscr();
            if( errResultInfo.getSeverity() == EResultSeverityCritical ) {
                QMessageBox::critical(
                    /* pWdgtParent */ this,
                    /* strTitly    */ windowTitle(),
                    /* strText     */ strMsg,
                    /* buttons     */ QMessageBox::Ok );
            } else {
                QMessageBox::warning(
                    /* pWdgtParent */ this,
                    /* strTitly    */ windowTitle(),
                    /* strText     */ strMsg,
                    /* buttons     */ QMessageBox::Ok );
            }
        }
    }
} // onActionFileSaveAsTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionFilePageSetupTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFilePageSetupTriggered",
        /* strAddInfo   */ strMthInArgs );

    showFilePageSetupDialog(false);
}

//------------------------------------------------------------------------------
void CMainWindow::onActionFileLastUsedFilesTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFileLastUsedFilesTriggered",
        /* strAddInfo   */ strMthInArgs );

    QAction* pAct = dynamic_cast<QAction*>(sender());
    QString strAbsFilePath = pAct->data().toString();
    setCurrentUsedFile(strAbsFilePath);
}

//------------------------------------------------------------------------------
void CMainWindow::onActionFileQuitTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionFileQuitTriggered",
        /* strAddInfo   */ strMthInArgs );

    close();
}

/*==============================================================================
public slots: // Menu - Mode
==============================================================================*/

////------------------------------------------------------------------------------
//void CMainWindow::onActionModeEditToggled(bool i_bChecked)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "onActionModeEditToggled",
//        /* strAddInfo   */ strMthInArgs );
//
//    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
//    if( i_bChecked && pDrawingScene->getMode() != EMode::Edit ) {
//        pDrawingScene->setCurrentDrawingTool(nullptr);
//        pDrawingScene->setMode(EMode::Edit);
//    }
//    else if( !i_bChecked && pDrawingScene->getMode() != EMode::View ) {
//        pDrawingScene->setCurrentDrawingTool(nullptr);
//        pDrawingScene->setMode(EMode::View);
//    }
//}
//
////------------------------------------------------------------------------------
//void CMainWindow::onActionModeViewToggled(bool i_bChecked)
////------------------------------------------------------------------------------
//{
//    QString strMthInArgs;
//    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
//        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
//    }
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
//        /* strMethod    */ "onActionModeViewToggled",
//        /* strAddInfo   */ strMthInArgs );
//
//    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
//    if( i_bChecked && pDrawingScene->getMode() != EMode::View ) {
//        pDrawingScene->setCurrentDrawingTool(nullptr);
//        pDrawingScene->setMode(EMode::View);
//    }
//    else if( !i_bChecked && pDrawingScene->getMode() != EMode::Edit ) {
//        pDrawingScene->setCurrentDrawingTool(nullptr);
//        pDrawingScene->setMode(EMode::Edit);
//    }
//} // onActionModeViewToggled

/*==============================================================================
public slots: // Menu - Draw - Settings
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawSettingsLineTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawSettingsLineTriggered",
        /* strAddInfo   */ strMthInArgs );

    CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Draw Settings";
    CDlgDrawingViewDrawSettings* pDlg = CDlgDrawingViewDrawSettings::GetInstance(pDrawingView);
    if( pDlg == nullptr ) {
        pDlg = CDlgDrawingViewDrawSettings::CreateInstance(strDlgTitle, pDrawingView);
        pDlg->setCurrentWidget(CWdgtDrawingViewDrawSettings::EWidget::LineStyle);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else {
        if( pDlg->isHidden() ) {
            pDlg->show();
        }
        pDlg->setCurrentWidget(CWdgtDrawingViewDrawSettings::EWidget::LineStyle);
        pDlg->raise();
        pDlg->activateWindow();
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawSettingsFillTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawSettingsFillTriggered",
        /* strAddInfo   */ strMthInArgs );

    CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Draw Settings";
    CDlgDrawingViewDrawSettings* pDlg = CDlgDrawingViewDrawSettings::GetInstance(pDrawingView);
    if( pDlg == nullptr ) {
        pDlg = CDlgDrawingViewDrawSettings::CreateInstance(strDlgTitle, pDrawingView);
        pDlg->setCurrentWidget(CWdgtDrawingViewDrawSettings::EWidget::FillStyle);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else {
        if( pDlg->isHidden() ) {
            pDlg->show();
        }
        pDlg->setCurrentWidget(CWdgtDrawingViewDrawSettings::EWidget::FillStyle);
        pDlg->raise();
        pDlg->activateWindow();
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawSettingsTextTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDrawSettingsTextTriggered",
        /* strAddInfo   */ strMthInArgs );

    CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Draw Settings";
    CDlgDrawingViewDrawSettings* pDlg = CDlgDrawingViewDrawSettings::GetInstance(pDrawingView);
    if( pDlg == nullptr ) {
        pDlg = CDlgDrawingViewDrawSettings::CreateInstance(strDlgTitle, pDrawingView);
        pDlg->setCurrentWidget(CWdgtDrawingViewDrawSettings::EWidget::TextStyle);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else {
        if( pDlg->isHidden() ) {
            pDlg->show();
        }
        pDlg->setCurrentWidget(CWdgtDrawingViewDrawSettings::EWidget::TextStyle);
        pDlg->raise();
        pDlg->activateWindow();
    }
}

/*==============================================================================
public slots: // Menu - Draw - Standard Shapes
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapePointToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    else {
        if( m_pActDrawChecked == m_pActDrawStandardShapePoint ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPoint ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapeLineToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    else {
        if( m_pActDrawChecked == m_pActDrawStandardShapeLine ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryLine ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapeRectToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    else {
        if( m_pActDrawChecked == m_pActDrawStandardShapeRect ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryRect ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapeEllipseToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    else {
        if( m_pActDrawChecked == m_pActDrawStandardShapeEllipse ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryEllipse ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapePolylineToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    else {
        if( m_pActDrawChecked == m_pActDrawStandardShapePolyline ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPolyline ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapePolygonToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    else {
        if( m_pActDrawChecked == m_pActDrawStandardShapePolygon ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPolygon ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapeTextToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    else {
        if( m_pActDrawChecked == m_pActDrawStandardShapeText ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryText ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
}

/*==============================================================================
public slots: // Menu - Draw - Graphics
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawGraphicsImageTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
}

/*==============================================================================
public slots: // Menu - Draw - Connections
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawConnectionPointToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    else {
        if( m_pActDrawChecked == m_pActDrawConnectionPoint ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryConnectionPoint ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawConnectionLineToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    else {
        if( m_pActDrawChecked == m_pActDrawConnectionLine ) {
            m_pActDrawChecked = nullptr;
        }
        if( pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryConnectionLine ) {
            pDrawingScene->setCurrentDrawingTool(nullptr);
        }
    }
}

/*==============================================================================
public slots: // Menu - Edit - Select/RotateFree
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionEditSelectToggled(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
        //pDrawingScene->setMode(
        //    EMode::Undefined, EEditTool::Select, EEditMode::None, EEditResizeMode::None);
    }
    else {
        if( m_pActDrawChecked == m_pActEditSelect ) {
            m_pActDrawChecked = nullptr;
        }
        //if( pDrawingScene->getEditTool() == EEditTool::Select ) {
        //    pDrawingScene->setMode(
        //        EMode::Undefined, EEditTool::None, EEditMode::None, EEditResizeMode::None);
        //}
    }
}

/*==============================================================================
public slots: // Menu - Edit - Group
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionEditGroupTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditGroupTriggered",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    pDrawingScene->groupGraphObjsSelected();
}

//------------------------------------------------------------------------------
void CMainWindow::onActionEditUngroupTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditUngroupTriggered",
        /* strAddInfo   */ strMthInArgs );

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    pDrawingScene->ungroupGraphObjsSelected();
}

/*==============================================================================
public slots: // Menu - Edit - Rotate
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionEditRotateLeftTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditRotateLeftTriggered",
        /* strAddInfo   */ strMthInArgs );

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
                    double fAngleTmp_deg = 0.0; //pGraphObj->getRotationAngleInDegree();
                    //pGraphObj->setRotationAngleInDegree(fAngleTmp_deg+fAngle_deg);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onActionEditRotateRightTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditRotateRightTriggered",
        /* strAddInfo   */ strMthInArgs );

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
                    double fAngleTmp_deg = 0.0; //pGraphObj->getRotationAngleInDegree();
                    //pGraphObj->setRotationAngleInDegree(fAngleTmp_deg+fAngle_deg);
                }
            }
        }
    }
}

/*==============================================================================
public slots: // Menu - Edit - Mirror
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionEditMirrorVerticalTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditMirrorVerticalTriggered",
        /* strAddInfo   */ strMthInArgs );

    //graphicsItem->scale(1,-1);

    QMessageBox::information(
        /* pWdgtParent */ this,
        /* strTitle    */ QApplication::applicationName() + ": Unsupported Feature",
        /* strText     */ "Sorry but mirroring is not yet supported." );
}

//------------------------------------------------------------------------------
void CMainWindow::onActionEditMirrorHorizontalTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionEditMirrorHorizontalTriggered",
        /* strAddInfo   */ strMthInArgs );

    //graphicsItem->scale(-1,1);

    QMessageBox::information(
        /* pWdgtParent */ this,
        /* strTitle    */ QApplication::applicationName() + ": Unsupported Feature",
        /* strText     */ "Sorry but mirroring is not yet supported." );
}

/*==============================================================================
public slots: // Menu - View - Zoom
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionViewZoomInTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
}

//------------------------------------------------------------------------------
void CMainWindow::onActionViewZoomOutTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
}

//------------------------------------------------------------------------------
void CMainWindow::onEdtViewZoomFactorEditingFinished()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    int iZoomFactor_perCent = m_pEdtViewZoomFactor_perCent->value();
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ZoomFactor: " + QString::number(iZoomFactor_perCent) + " %";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtViewZoomFactorEditingFinished",
        /* strAddInfo   */ strMthInArgs );

    if( m_iViewZoomFactor_perCent != iZoomFactor_perCent ) {
        CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
        pDrawingView->setZoomFactorInPerCent(iZoomFactor_perCent);
        m_iViewZoomFactor_perCent = iZoomFactor_perCent;
    }
}

/*==============================================================================
public slots: // Menu - Debug
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDebugErrLogTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDebugErrLogTriggered",
        /* strAddInfo   */ strMthInArgs );

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
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDebugUnitsTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDebugUnitsTriggered",
        /* strAddInfo   */ strMthInArgs );

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
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDebugTraceServerTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDebugTraceServerTriggered",
        /* strAddInfo   */ strMthInArgs );

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
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDebugTraceAdminObjsTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDebugTraceAdminObjsTriggered",
        /* strAddInfo   */ strMthInArgs );

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
}

//------------------------------------------------------------------------------
void CMainWindow::onActionDebugTestTriggered(bool i_bChecked)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDebugTestTriggered",
        /* strAddInfo   */ strMthInArgs );

    if( m_pDlgTest == nullptr ) {
        m_pDlgTest = new ZS::Test::GUI::CDlgTest("Test", m_pTest);
    }

    m_pDlgTest->show();
    m_pDlgTest->activateWindow();
    m_pDlgTest->raise();
}

/*==============================================================================
public slots: // Drawing Scene
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneSizeChanged(const CDrawingSize& i_drawingSize)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_drawingSize.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneSizeChanged",
        /* strAddInfo   */ strMthInArgs );

    if( !m_bDrawingChangedSinceLastSave ) {
        m_bDrawingChangedSinceLastSave = true;
        updateActions();
    }

    onDrawingViewContentAreaChanged();

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    if (m_pLblStatusBarDrawingSceneMouseCursorPos_px != nullptr) {
        QString strPosInfo = "MousePos {" + qPoint2Str(m_ptDrawingSceneMouseCursorPos) + "} px";
        m_pLblStatusBarDrawingSceneMouseCursorPos_px->setText(strPosInfo);
    }
    if (m_pLblStatusBarDrawingSceneMouseCursorPos_metric != nullptr) {
        const CDrawingSize& drawingSize = pDrawingScene->drawingSize();
        CPhysValPoint physValPoint = pDrawingScene->convert(m_ptDrawingSceneMouseCursorPos, drawingSize.metricUnit());
        QString strPosInfo = "MousePos {" + physValPoint.toString() + "} " + physValPoint.unit().symbol();
        m_pLblStatusBarDrawingSceneMouseCursorPos_metric->setText(strPosInfo);
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneGridSettingsChanged( const CDrawGridSettings& i_settings)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_settings.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneGridSettingsChanged",
        /* strAddInfo   */ strMthInArgs );

    if( !m_bDrawingChangedSinceLastSave ) {
        m_bDrawingChangedSinceLastSave = true;
        updateActions();
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneChanged( const QList<QRectF>& i_region )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
        //if( !m_strCurrentFileAbsFilePath.isEmpty() ) {
        //    m_bDrawingChangedSinceLastSave = true;
        //}
        if( i_region.size() != 1 ) {
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
}

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneFocusItemChanged(
    QGraphicsItem* i_pNewFocusItem,
    QGraphicsItem* i_pOldFocusItem,
    Qt::FocusReason reason )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
}

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
}

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneMousePosChanged( const QPointF& i_ptMousePos )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObjMouseEvents, EMethodTraceDetailLevel::ArgsNormal) )  {
        strMthInArgs = qPoint2Str(i_ptMousePos);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneMousePosChanged",
        /* strAddInfo   */ strMthInArgs );

    m_ptDrawingSceneMouseCursorPos = i_ptMousePos;

    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
    if (m_pLblStatusBarDrawingSceneMouseCursorPos_px != nullptr) {
        QString strPosInfo = "MousePos {" + qPoint2Str(m_ptDrawingSceneMouseCursorPos) + "} px";
        m_pLblStatusBarDrawingSceneMouseCursorPos_px->setText(strPosInfo);
    }
    if (m_pLblStatusBarDrawingSceneMouseCursorPos_metric != nullptr) {
        const CDrawingSize& drawingSize = pDrawingScene->drawingSize();
        CPhysValPoint physValPoint = pDrawingScene->convert(m_ptDrawingSceneMouseCursorPos, drawingSize.metricUnit());
        QString strPosInfo = "MousePos {" + physValPoint.toString() + "} " + physValPoint.unit().symbol();
        m_pLblStatusBarDrawingSceneMouseCursorPos_metric->setText(strPosInfo);
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneModeChanged(const CEnumMode& i_mode)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObjMouseEvents, EMethodTraceDetailLevel::ArgsNormal) )  {
        strMthInArgs = i_mode.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneModeChanged",
        /* strAddInfo   */ "" );

    updateWindowTitle();
    updateActions();
    updateStatusBar();
}

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneDrawingToolChanged(
    const QString& i_strFactoryGrpName, const QString& i_strGraphObjType)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObjMouseEvents, EMethodTraceDetailLevel::ArgsNormal) )  {
        strMthInArgs = i_strFactoryGrpName + ", " + i_strGraphObjType;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneDrawingToolChanged",
        /* strAddInfo   */ "" );

    updateWindowTitle();
    updateActions();
    updateStatusBar();
}

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
        QIcon iconEditDrawSettingsLine;
        QPixmap pxmEditDrawSettingsLine(":/ZS/Draw/DrawSettingsLineStyle16x16.png");
        QSize sizePxm = pxmEditDrawSettingsLine.size();
        QPainter painter(&pxmEditDrawSettingsLine);
        QPen pen(i_drawSettings.getPenColor(), 2);
        painter.setPen(pen);
        painter.setRenderHints(QPainter::Antialiasing, true);
        painter.drawLine(2, 5, sizePxm.width()-6, sizePxm.height()-3);
        iconEditDrawSettingsLine.addPixmap(pxmEditDrawSettingsLine);
        m_pActDrawSettingsLine->setIcon(iconEditDrawSettingsLine);
    }
    if( m_pActDrawSettingsFill != nullptr )
    {
        QIcon   iconEditDrawSettingsFill;
        QPixmap pxmEditDrawSettingsFill(":/ZS/Draw/DrawSettingsBrushStyle16x16.png");

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
    }
    if( m_pActDrawSettingsText != nullptr )
    {
        QIcon iconEditDrawSettingsText;
        QPixmap pxmEditDrawSettingsText(":/ZS/Draw/DrawSettingsTextStyle16x16.png");

        QPainter painter(&pxmEditDrawSettingsText);
        QRect rctBtnLineColor(0, 12, 16, 4);

        painter.setPen(Qt::NoPen);
        painter.setBrush(i_drawSettings.getTextColor());
        painter.drawRect(rctBtnLineColor);

        iconEditDrawSettingsText.addPixmap(pxmEditDrawSettingsText);

        m_pActDrawSettingsText->setIcon(iconEditDrawSettingsText);
    }
}

/*==============================================================================
protected slots: // Drawing View
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onDrawingViewMousePosChanged( const QPointF& i_ptMousePos )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjMouseEvents, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qPoint2Str(i_ptMousePos);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingViewMousePosChanged",
        /* strAddInfo   */ strMthInArgs );

    if (m_pLblStatusBarDrawingViewMouseCursorPos != nullptr) {
        //CDrawingView* pDrawingView = m_pWdgtCentral->drawingView();
        //CPageSetup* pageSetup = pDrawingView->getPageSetup();
        //CUnit unitWidth = pageSetup->unit(EOrientation::Horizontal);
        //QString strMouseCursorPos;
        //strMouseCursorPos += QString("ViewPos: ");
        //strMouseCursorPos += QString::number(i_ptMousePos.x());
        //strMouseCursorPos += QString("/");
        //strMouseCursorPos += QString::number(i_ptMousePos.y());
        //strMouseCursorPos += QString(" [");
        //strMouseCursorPos += QString(unitWidth.symbol());
        //strMouseCursorPos += QString("]");
        //m_pLblStatusBarDrawingViewMouseCursorPos->setText(strMouseCursorPos);
    }
} // onDrawingViewMousePosChanged

//------------------------------------------------------------------------------
void CMainWindow::onDrawingViewContentAreaChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjMouseEvents,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingViewContentAreaChanged",
        /* strAddInfo   */ "" );

    if (m_pWdgtCentral != nullptr) {
        CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();
        QRect rectScene = pDrawingScene->sceneRect().toRect();

        if (m_pLblStatusBarDrawingSceneRect != nullptr) {
            QString strSizeInfo = "SceneRect {" + qSize2Str(rectScene.size()) + "} px";
            m_pLblStatusBarDrawingSceneRect->setText(strSizeInfo);
        }
        if (m_pLblStatusBarDrawingSize != nullptr) {
            const CDrawingSize& drawingSize = pDrawingScene->drawingSize();
            QString strSizeInfo = " Drawing {" +
                drawingSize.metricImageWidth().toString(EUnitFind::None, PhysValSubStr::Val) + ", " +
                drawingSize.metricImageHeight().toString(EUnitFind::None, PhysValSubStr::Val) + "} " +
                drawingSize.metricUnit().symbol();
            m_pLblStatusBarDrawingSize->setText(strSizeInfo);
        }
    }
}

/*==============================================================================
protected slots: // Tree View Object Factories (ToolBox)
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onTreeViewObjFactoriesExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onTreeViewObjFactoriesExpanded",
        /* strAddInfo   */ strMthInArgs );

    if (i_modelIdx.isValid()) {
        m_pTreeViewObjFactories->resizeColumnToContents(i_modelIdx.column());
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onTreeViewObjFactoriesCurrentChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& /*i_modelIdxPrev*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::selectTreeViewObjFactoryNode( ZS::Draw::CObjFactory* i_pObjFactory )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
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
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs  = "Size: " + qSize2Str(i_pEv->size());
        strMthInArgs += ", OldSize: " + qSize2Str(i_pEv->oldSize());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "resizeEvent",
        /* strAddInfo   */ strMthInArgs );

    QMainWindow::resizeEvent(i_pEv);

    if (m_pLblStatusBarMainWindowSize != nullptr) {
        m_pLblStatusBarMainWindowSize->setText("MainWindow {" + qSize2Str(size()) + "} px");
    }
}

//------------------------------------------------------------------------------
void CMainWindow::showEvent( QShowEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "showEvent",
        /* strAddInfo   */ strMthInArgs );

    QMainWindow::showEvent(i_pEv);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::updateStatusBar()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateStatusBar",
        /* strAddInfo   */ "" );

    //if (m_pWdgtCentral != nullptr)
    //{
    //    CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

    //    if (m_pLblStatusBarDrawingSceneEditTool != nullptr) {
    //        QString strEditInfo = "Tool: ";
    //        CEnumEditTool editTool = pDrawingScene->getEditTool();
    //        if( editTool == EEditTool::CreateObjects ) {
    //            int iGraphObjType = pDrawingScene->getCurrentDrawingToolGraphObjType();
    //            QString strObjFactory = graphObjType2Str(iGraphObjType);
    //            strEditInfo += "Create " + strObjFactory;
    //        }
    //        else if( editTool != EEditTool::None ) {
    //            strEditInfo += editTool.toString();
    //        }
    //        m_pLblStatusBarDrawingSceneEditTool->setText(strEditInfo);
    //    }

    //    if (m_pLblStatusBarDrawingSceneEditMode != nullptr || m_pLblStatusBarDrawingSceneGraphObjEditInfo != nullptr) {
    //        QString strEditModeInfo;
    //        QString strGraphObjEditInfo;
    //        CEnumEditMode editMode = pDrawingScene->getEditMode();
    //        if (editMode != EEditMode::None) {
    //            QGraphicsItem* pGraphicsItemSelected = nullptr;
    //            QList<QGraphicsItem*> arGraphicsItemSelected = pDrawingScene->selectedItems();
    //            if (arGraphicsItemSelected.size() > 0) {
    //                pGraphicsItemSelected = arGraphicsItemSelected[0];
    //            }
    //            //else if (pDrawingScene->getGraphicsItemCreating() != nullptr) {
    //            //    pGraphicsItemSelected = pDrawingScene->getGraphicsItemCreating();
    //            //}

    //            if (pGraphicsItemSelected != nullptr) {
    //                CGraphObj* pGraphObjSelected = dynamic_cast<CGraphObj*>(pGraphicsItemSelected);
    //                if (pGraphObjSelected == nullptr) {
    //                    throw CException(__FILE__, __LINE__, EResultInvalidDynamicTypeCast, "pGraphObj == nullptr");
    //                }
    //                strEditModeInfo += pGraphObjSelected->name();
    //                strGraphObjEditInfo += pGraphObjSelected->name() + ": " + pGraphObjSelected->getEditInfo();
    //            }

    //            if (editMode == EEditMode::Resize) {
    //                CEnumEditResizeMode editResizeMode = pDrawingScene->getEditResizeMode();
    //                if (editResizeMode >= 0 && editResizeMode < CEnumEditResizeMode::count()) {
    //                    if (!strEditModeInfo.isEmpty()) {
    //                        strEditModeInfo += ".";
    //                    }
    //                    strEditModeInfo += editResizeMode.toString();
    //                }
    //                else if (editMode != EEditMode::None) {
    //                    if (!strEditModeInfo.isEmpty()) {
    //                        strEditModeInfo += ".";
    //                    }
    //                    strEditModeInfo += editMode.toString();
    //                }
    //            }
    //            else {
    //                if (!strEditModeInfo.isEmpty()) {
    //                    strEditModeInfo += ".";
    //                }
    //                strEditModeInfo += editMode.toString();
    //            }
    //        }
    //        if (m_pLblStatusBarDrawingSceneEditMode != nullptr) {
    //            m_pLblStatusBarDrawingSceneEditMode->setText("Mode: " + strEditModeInfo);
    //        }
    //        if (m_pLblStatusBarDrawingSceneGraphObjEditInfo != nullptr) {
    //            m_pLblStatusBarDrawingSceneGraphObjEditInfo->setText("Info: " + strGraphObjEditInfo);
    //        }
    //    }
    //}
}

//------------------------------------------------------------------------------
void CMainWindow::updateActions()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateActions",
        /* strAddInfo   */ "" );

    if (m_pWdgtCentral != nullptr )
    {
        CDrawingScene* pDrawingScene = m_pWdgtCentral->drawingScene();

        CEnumMode mode = pDrawingScene->getMode();

        QList<QGraphicsItem*> arpGraphicItemsSelected = pDrawingScene->selectedItems();
        //QGraphicsItem* pGraphicsItem;

        // Menu - File
        //------------

        //if (m_pActFileNew != nullptr) {
        //    m_pActFileNew;
        //}
        //if (m_pActFileOpen != nullptr) {
        //    m_pActFileOpen;
        //}
        //if (m_pActFileSave != nullptr) {
        //    m_pActFileSave;
        //}
        //if (m_pActFileSaveAs != nullptr) {
        //    m_pActFileSaveAs;
        //}
        //if (m_pActFileQuit != nullptr) {
        //    m_pActFileQuit;
        //}
        //if (m_pActFileQuit != nullptr) {
        //    m_pActFileQuit;
        //}

        // Menu - Mode
        //------------

        //if (m_pActModeEdit != nullptr) {
        //    m_pActModeEdit->setChecked( mode == EMode::Edit );
        //}
        //if (m_pActModeView != nullptr) {
        //    m_pActModeView->setChecked( mode == EMode::View );
        //}

        // Menu - Draw
        //---------------------

        //if (m_pActDrawChecked != nullptr) {
        //    m_pActDrawChecked;
        //}

        // Menu - Draw - Settings
        //-----------------------

        if (m_pActDrawSettingsLine != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawSettingsLine->setEnabled(false);
            }
            else {
                m_pActDrawSettingsLine->setEnabled(true);
            }
        }
        if (m_pActDrawSettingsFill != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawSettingsFill->setEnabled(false);
            }
            else {
                m_pActDrawSettingsFill->setEnabled(true);
            }
        }
        if (m_pActDrawSettingsText != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawSettingsText->setEnabled(false);
            }
            else {
                m_pActDrawSettingsText->setEnabled(true);
            }
        }

        // Menu - Draw - Standard Shapes
        //------------------------------

        if (m_pActDrawStandardShapePoint != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawStandardShapePoint->setEnabled(false);
                m_pActDrawStandardShapePoint->setChecked(false);
            }
            else {
                m_pActDrawStandardShapePoint->setEnabled(false);
                if (pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPoint) {
                    m_pActDrawStandardShapePoint->setChecked(true);
                }
                else {
                    m_pActDrawStandardShapePoint->setChecked(false);
                }
            }
        }
        if (m_pActDrawStandardShapeLine != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawStandardShapeLine->setEnabled(false);
                m_pActDrawStandardShapeLine->setChecked(false);
            }
            else {
                m_pActDrawStandardShapeLine->setEnabled(true);
                if (pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryLine) {
                    m_pActDrawStandardShapeLine->setChecked(true);
                }
                else {
                    m_pActDrawStandardShapeLine->setChecked(false);
                }
            }
        }
        if (m_pActDrawStandardShapeRect != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawStandardShapeRect->setEnabled(false);
                m_pActDrawStandardShapeRect->setChecked(false);
            }
            else {
                m_pActDrawStandardShapeRect->setEnabled(false);
                if (pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryRect) {
                    m_pActDrawStandardShapeRect->setChecked(true);
                }
                else {
                    m_pActDrawStandardShapeRect->setChecked(false);
                }
            }
        }
        if (m_pActDrawStandardShapeEllipse != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawStandardShapeEllipse->setEnabled(false);
                m_pActDrawStandardShapeEllipse->setChecked(false);
            }
            else {
                m_pActDrawStandardShapeEllipse->setEnabled(false);
                if (pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryEllipse) {
                    m_pActDrawStandardShapeEllipse->setChecked(true);
                }
                else {
                    m_pActDrawStandardShapeEllipse->setChecked(false);
                }
            }
        }
        if (m_pActDrawStandardShapePolyline != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawStandardShapePolyline->setEnabled(false);
                m_pActDrawStandardShapePolyline->setChecked(false);
            }
            else {
                m_pActDrawStandardShapePolyline->setEnabled(false);
                if (pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPolyline) {
                    m_pActDrawStandardShapePolyline->setChecked(true);
                }
                else {
                    m_pActDrawStandardShapePolyline->setChecked(false);
                }
            }
        }
        if (m_pActDrawStandardShapePolygon != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawStandardShapePolygon->setEnabled(false);
                m_pActDrawStandardShapePolygon->setChecked(false);
            }
            else {
                m_pActDrawStandardShapePolygon->setEnabled(false);
                if (pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPolygon) {
                    m_pActDrawStandardShapePolygon->setChecked(true);
                }
                else {
                    m_pActDrawStandardShapePolygon->setChecked(false);
                }
            }
        }
        if (m_pActDrawStandardShapeText != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawStandardShapeText->setEnabled(false);
                m_pActDrawStandardShapeText->setChecked(false);
            }
            else {
                m_pActDrawStandardShapeText->setEnabled(false);
                if (pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryText) {
                    m_pActDrawStandardShapeText->setChecked(true);
                }
                else {
                    m_pActDrawStandardShapeText->setChecked(false);
                }
            }
        }

        // Menu - Draw - Graphics
        //-----------------------

        if (m_pActDrawGraphicsImage != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawGraphicsImage->setEnabled(false);
            }
            else {
                m_pActDrawGraphicsImage->setEnabled(false);
            }
        }

        // Menu - Draw - Connections
        //--------------------------

        if (m_pActDrawConnectionPoint != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawConnectionPoint->setEnabled(false);
                m_pActDrawConnectionPoint->setChecked(false);
            }
            else {
                m_pActDrawConnectionPoint->setEnabled(false);
                if (pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryConnectionPoint) {
                    m_pActDrawConnectionPoint->setChecked(true);
                }
                else {
                    m_pActDrawConnectionPoint->setChecked(false);
                }
            }
        }
        if (m_pActDrawConnectionLine != nullptr) {
            if (mode == EMode::View) {
                m_pActDrawConnectionLine->setEnabled(false);
                m_pActDrawConnectionLine->setChecked(false);
            }
            else {
                m_pActDrawConnectionLine->setEnabled(false);
                if (pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryConnectionLine) {
                    m_pActDrawConnectionLine->setChecked(true);
                }
                else {
                    m_pActDrawConnectionLine->setChecked(false);
                }
            }
        }

        // Menu - Edit - Select
        //---------------------

        if (m_pActEditSelect != nullptr) {
            if (mode == EMode::View) {
                m_pActEditSelect->setEnabled(false);
            }
            else {
                //if (pDrawingScene->items().size() == 0) {
                //    m_pActEditSelect->setEnabled(false);
                //}
                //else {
                //    m_pActEditSelect->setEnabled(true);
                //}
                if (pDrawingScene->getCurrentDrawingTool() == nullptr) {
                    m_pActEditSelect->setChecked(true);
                }
                else {
                    m_pActEditSelect->setChecked(false);
                }
            }
        }

        // Menu - Edit - Rotate and Mirror
        //--------------------------------

        if (mode == EMode::View) {
            if (m_pActEditRotateLeft != nullptr) {
                m_pActEditRotateLeft->setEnabled(false);
            }
            if (m_pActEditRotateRight != nullptr) {
                m_pActEditRotateRight->setEnabled(false);
            }
            if (m_pEdtEditRotateAngle != nullptr) {
                m_pEdtEditRotateAngle->setEnabled(false);
            }
            if (m_pActEditMirrorVertical != nullptr) {
                m_pActEditMirrorVertical->setEnabled(false);
            }
            if (m_pActEditMirrorHorizontal != nullptr) {
                m_pActEditMirrorHorizontal->setEnabled(false);
            }
        }
        else {
            if (arpGraphicItemsSelected.size() == 0) {
                if (m_pActEditRotateLeft != nullptr) {
                    m_pActEditRotateLeft->setEnabled(false);
                }
                if (m_pActEditRotateRight != nullptr) {
                    m_pActEditRotateRight->setEnabled(false);
                }
                if (m_pEdtEditRotateAngle != nullptr) {
                    m_pEdtEditRotateAngle->setEnabled(false);
                }
                if (m_pActEditMirrorVertical != nullptr) {
                    m_pActEditMirrorVertical->setEnabled(false);
                }
                if (m_pActEditMirrorHorizontal != nullptr) {
                    m_pActEditMirrorHorizontal->setEnabled(false);
                }
            }
            else {
                if (m_pActEditRotateLeft != nullptr) {
                    m_pActEditRotateLeft->setEnabled(false);
                }
                if (m_pActEditRotateRight != nullptr) {
                    m_pActEditRotateRight->setEnabled(false);
                }
                if (m_pEdtEditRotateAngle != nullptr) {
                    m_pEdtEditRotateAngle->setEnabled(false);
                }
                if (m_pActEditMirrorVertical != nullptr) {
                    //m_pActEditMirrorVertical->setEnabled(true); not yet supported
                }
                if (m_pActEditMirrorHorizontal != nullptr) {
                    //m_pActEditMirrorHorizontal->setEnabled(true); not yet supported
                }
            }
        }

        // Menu - Edit - Group
        //---------------------

        if (mode == EMode::View) {
            if (m_pActEditGroup != nullptr) {
                m_pActEditGroup->setEnabled(false);
            }
            if (m_pActEditUngroup != nullptr) {
                m_pActEditUngroup->setEnabled(false);
            }
        }
        else
        {
            if (arpGraphicItemsSelected.size() == 0) {
                if (m_pActEditGroup != nullptr) {
                    m_pActEditGroup->setEnabled(false);
                }
                if(m_pActEditUngroup != nullptr) {
                    m_pActEditUngroup->setEnabled(false);
                }
            }
            else if (arpGraphicItemsSelected.size() == 1) {
                if (m_pActEditGroup != nullptr) {
                    m_pActEditGroup->setEnabled(false);
                }
                QGraphicsItem* pGraphicsItem = arpGraphicItemsSelected[0];
                if (pGraphicsItem->type() == EGraphObjTypeGroup) {
                    if (m_pActEditUngroup != nullptr) {
                        m_pActEditUngroup->setEnabled(true);
                    }
                }
                else {
                    if (m_pActEditUngroup != nullptr) {
                        m_pActEditUngroup->setEnabled(false);
                    }
                }
            }
            else {
                if (m_pActEditGroup != nullptr) {
                    m_pActEditGroup->setEnabled(true);
                }
                if (m_pActEditUngroup != nullptr) {
                    m_pActEditUngroup->setEnabled(false);
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

        if (m_pDockWdgtObjFactories != nullptr) {
            if (mode == EMode::View) {
                m_pDockWdgtObjFactories->hide();
            }
            else {
                m_pDockWdgtObjFactories->show();
            }
        }
        if (mode == EMode::View) {
            selectTreeViewObjFactoryNode(nullptr);
        }
        else if (pDrawingScene->getCurrentDrawingTool() != nullptr) {
            selectTreeViewObjFactoryNode(nullptr);
        }

        // Dock Widget - Graphics Items
        //-----------------------------

    }
} // updateActions

//------------------------------------------------------------------------------
void CMainWindow::updateActionsLastUsedFiles()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateActionsLastUsedFiles",
        /* strAddInfo   */ "" );

    for (int idxFile = 0; idxFile < m_arpActsLastUsedFiles.size(); ++idxFile) {
        delete m_arpActsLastUsedFiles[idxFile];
        m_arpActsLastUsedFiles[idxFile] = nullptr;
    }
    m_arpActsLastUsedFiles.clear();

    QIcon iconApp;

    QPixmap pxmApp32x32(":/ZS/App/ZeusSoft_32x32.png");
    QPixmap pxmApp48x48(":/ZS/App/ZeusSoft_48x48.png");
    QPixmap pxmApp64x64(":/ZS/App/ZeusSoft_64x64.png");

    iconApp.addPixmap(pxmApp32x32);
    iconApp.addPixmap(pxmApp48x48);
    iconApp.addPixmap(pxmApp64x64);

    for (int idxFile = 0; idxFile < m_arLastUsedFiles.size(); idxFile++) {
        QAction* pAct = new QAction(iconApp, m_arLastUsedFiles[idxFile].m_strAbsFilePath, this);
        pAct->setData(m_arLastUsedFiles[idxFile].m_strAbsFilePath);
        pAct->setVisible(true);
        QObject::connect(
            pAct, &QAction::triggered,
            this, &CMainWindow::onActionFileLastUsedFilesTriggered );
        m_arpActsLastUsedFiles.append(pAct);
        m_pMenuLastUsedFiles->addAction(pAct);
        if (m_arLastUsedFiles[idxFile].m_strAbsFilePath == m_strCurrentFileAbsFilePath) {
            pAct->setEnabled(false);
        }
        else {
            QObject::connect(
                pAct, &QAction::triggered,
                this, &CMainWindow::onActionFileLastUsedFilesTriggered);
        }
    }
}

//------------------------------------------------------------------------------
void CMainWindow::updateCurrentUsedFile( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "File:" + i_strAbsFilePath;
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateCurrentUsedFile",
        /* strAddInfo   */ strMthInArgs );

    if (i_strAbsFilePath.isEmpty()) {
        m_dtCurrentFileLastUsed = QDateTime();
    }
    else {
        m_dtCurrentFileLastUsed = QDateTime::currentDateTime();
    }

    if (m_strCurrentFileAbsFilePath != i_strAbsFilePath) {
        m_bDrawingChangedSinceLastSave = false;
        m_strCurrentFileAbsFilePath = i_strAbsFilePath;
        updateWindowTitle();
    }

    if (!m_strCurrentFileAbsFilePath.isEmpty()) {
        bool bCurrentFileUpdated = false;
        for (int idxFile = 0; idxFile < m_arLastUsedFiles.size(); ++idxFile) {
            if (m_arLastUsedFiles[idxFile].m_strAbsFilePath == i_strAbsFilePath) {
                m_arLastUsedFiles[idxFile].m_dtLastUsed = m_dtCurrentFileLastUsed;
                bCurrentFileUpdated = true;
                break;
            }
        }
        if (!bCurrentFileUpdated) {
            m_arLastUsedFiles.append({m_strCurrentFileAbsFilePath, m_dtCurrentFileLastUsed});
        }
        ZS::System::sortLastUsedFiles(m_arLastUsedFiles);
        while (m_arLastUsedFiles.size() > c_iLastUsedFilesCountMax) {
            m_arLastUsedFiles.removeLast();
        }
        QSettings settings;
        ZS::System::writeLastUsedFiles(m_arLastUsedFiles, settings);
        updateActionsLastUsedFiles();
    }
}

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
    if (m_pLblErrors != nullptr) {
        EResultSeverity severityMax = EResultSeveritySuccess;
        QString strToolTip;

        int ariErrorsCount[EResultSeverityCount];
        memset(ariErrorsCount, 0x00, sizeof(ariErrorsCount));

        CErrLog* pErrLog = CErrLog::GetInstance();
        if (pErrLog != nullptr) {
            pErrLog->lock();
            int iErrorsCount = 0;
            for (int iRowIdx = 0; iRowIdx < pErrLog->getEntryCount(); iRowIdx++) {
                SErrLogEntry* pErrLogEntry = pErrLog->getEntry(iRowIdx);
                if (pErrLogEntry != nullptr) {
                    if (pErrLogEntry->m_errResultInfo.getSeverity() > severityMax) {
                        severityMax = pErrLogEntry->m_errResultInfo.getSeverity();
                    }
                    if ((pErrLogEntry->m_errResultInfo.getSeverity() >= EResultSeverityInfo)
                     && (pErrLogEntry->m_errResultInfo.getSeverity() < EResultSeverityCount)) {
                        ariErrorsCount[pErrLogEntry->m_errResultInfo.getSeverity()]++;
                        iErrorsCount++;
                    }
                }
            }
            pErrLog->unlock();

            if (iErrorsCount == 0) {
                strToolTip = "There is no Info, no Warning, no Error and no Critical Error message pending";
                m_pLblErrors->hide();
            }
            else if (iErrorsCount > 0) {
                int iErrorsCountTmp = 0;
                if (iErrorsCount == 1) {
                    strToolTip = "There is ";
                }
                else {
                    strToolTip = "There are ";
                }
                for (int iRowIdx = EResultSeverityInfo; iRowIdx < EResultSeverityCount; iRowIdx++) {
                    EResultSeverity severity = static_cast<EResultSeverity>(iRowIdx);
                    if (ariErrorsCount[iRowIdx] > 0) {
                        strToolTip += QString::number(ariErrorsCount[iRowIdx]) + " " + resultSeverity2Str(severity);
                        if (severity == EResultSeverityInfo || severity == EResultSeverityWarning || severity == EResultSeverityError) {
                            if (ariErrorsCount[iRowIdx] > 1) {
                                strToolTip += "s";
                            }
                        }
                        else if (severity == EResultSeverityCritical) {
                            if (ariErrorsCount[iRowIdx] == 1) {
                                strToolTip += " Error";
                            }
                            else {
                                strToolTip += " Errors";
                            }
                        }
                        iErrorsCountTmp += ariErrorsCount[iRowIdx];
                        if (iErrorsCountTmp < iErrorsCount) {
                            int iSeveritiesWithErrorsFollow = 0;
                            for (int iRowIdxTmp = iRowIdx+1; iRowIdxTmp < EResultSeverityCount; iRowIdxTmp++) {
                                if (ariErrorsCount[iRowIdxTmp] > 0) {
                                    iSeveritiesWithErrorsFollow++;
                                }
                            }
                            if (iSeveritiesWithErrorsFollow >= 2) {
                                strToolTip += ", ";
                            }
                            else if (iSeveritiesWithErrorsFollow >= 1) {
                                strToolTip += " and ";
                            }
                        }
                    }
                }
                m_pLblErrors->show();
            }
            strToolTip += ".";
        }

        m_pLblErrors->setPixmap( getErrPixmap(severityMax) );
        m_pLblErrors->setToolTip(strToolTip);
    }
} // updateErrorsStatus
