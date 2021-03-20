/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qsettings.h>
#include <QtCore/qtimer.h>

#include <QtGui/qbitmap.h>
#include <QtGui/qevent.h>
#include <QtGui/qicon.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qbuttongroup.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qdockwidget.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qstatusbar.h>
#include <QtGui/qtoolbar.h>
#include <QtGui/qtoolbutton.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qbuttongroup.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qstatusbar.h>
#include <QtWidgets/qtoolbar.h>
#include <QtWidgets/qtoolbutton.h>
#include <QtWidgets/qtreeview.h>
#endif


#include "MainWindow.h"
#include "ZSDrawObjFactoryWdgtCheckBox.h"
#include "ZSDrawObjFactoryWdgtComboBox.h"
#include "ZSDrawObjFactoryWdgtGroupBox.h"
#include "ZSDrawObjFactoryWdgtLabel.h"
#include "ZSDrawObjFactoryWdgtLineEdit.h"
#include "ZSDrawObjFactoryWdgtPushButton.h"
#include "ZSDrawObjFactoryElectricityCapacitor.h"
#include "ZSDrawObjFactoryElectricityDiode.h"
#include "ZSDrawObjFactoryElectricityInductor.h"
#include "ZSDrawObjFactoryElectricitySwitch.h"
#include "ZSDrawObjFactoryElectricityResistor.h"
#include "ZSDrawObjFactoryElectricityTransistor.h"
#include "ZSDrawObjFactoryElectricityVoltageSource.h"

#include "ZSDraw/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/ZSDrawDlgPageSetup.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawingView.h"
#include "ZSDraw/ZSDrawGraphicsItemsModel.h"
#include "ZSDraw/ZSDrawGraphObjModelEntry.h"
#include "ZSDraw/ZSDrawGraphObj.h"
#include "ZSDraw/ZSDrawObjFactoriesModel.h"
#include "ZSDraw/ZSDrawObjFactoryConnectionLine.h"
#include "ZSDraw/ZSDrawObjFactoryConnectionPoint.h"
#include "ZSDraw/ZSDrawObjFactoryEllipse.h"
#include "ZSDraw/ZSDrawObjFactoryGroup.h"
#include "ZSDraw/ZSDrawObjFactoryImage.h"
#include "ZSDraw/ZSDrawObjFactoryLine.h"
#include "ZSDraw/ZSDrawObjFactoryPoint.h"
#include "ZSDraw/ZSDrawObjFactoryPolygon.h"
#include "ZSDraw/ZSDrawObjFactoryPolyline.h"
#include "ZSDraw/ZSDrawObjFactoryRect.h"
#include "ZSDraw/ZSDrawObjFactoryText.h"
#include "ZSPhysValGUI/ZSPhysValDlgEditPhysVal.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSIpcTraceGUI/ZSIpcTrcServerDlg.h"
#include "ZSTestGUI/ZSTestDlg.h"
#include "ZSSysGUI/ZSSysErrLogDlg.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;
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

const QString CMainWindow::c_strMenuNameFile                         = "&File";
const QString CMainWindow::c_strMenuNameMode                         = "&Mode";
const QString CMainWindow::c_strMenuNameEdit                         = "&Edit";
const QString CMainWindow::c_strMenuNameEditRotate                   = "Edit:&Rotate";
const QString CMainWindow::c_strMenuNameEditMirror                   = "Edit:&Mirror";
const QString CMainWindow::c_strMenuNameDraw                         = "&Draw";
const QString CMainWindow::c_strMenuNameDrawSettings                 = "Draw:S&ettings";
const QString CMainWindow::c_strMenuNameDrawStandardShapes           = "Draw:&Standard Shapes";
const QString CMainWindow::c_strMenuNameDrawGraphics                 = "Draw:&Graphics";
const QString CMainWindow::c_strMenuNameDrawConnections              = "Draw:&Connections";
const QString CMainWindow::c_strMenuNameDrawWidgets                  = "Draw:&Widgets";
const QString CMainWindow::c_strMenuNameDrawElectricity              = "Draw:&Electricity";
const QString CMainWindow::c_strMenuNameView                         = "&View";
const QString CMainWindow::c_strMenuNameDebugTrace                   = "Debug and &Trace";
const QString CMainWindow::c_strMenuNameInfo                         = "&Info";

const QString CMainWindow::c_strActionNameFileNew                    = c_strMenuNameFile + ":&New ...";
const QString CMainWindow::c_strActionNameFileOpen                   = c_strMenuNameFile + ":&Open ...";
const QString CMainWindow::c_strActionNameFileSave                   = c_strMenuNameFile + ":&Save ...";
const QString CMainWindow::c_strActionNameFileSaveAs                 = c_strMenuNameFile + ":Save &As ...";
const QString CMainWindow::c_strActionNameFilePageSetup              = c_strMenuNameFile + ":Page Set&up ...";
const QString CMainWindow::c_strActionNameFileQuit                   = c_strMenuNameFile + ":&Quit";
const QString CMainWindow::c_strActionNameModeEdit                   = c_strMenuNameMode + ":&Edit";
const QString CMainWindow::c_strActionNameModeSimulation             = c_strMenuNameMode + ":&Simulation";
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
const QString CMainWindow::c_strActionNameViewZoom                   = c_strMenuNameView + ":&Zoom";
const QString CMainWindow::c_strActionNameTraceErrLog                = c_strMenuNameDebugTrace + ":&Error Log";
const QString CMainWindow::c_strActionNameTraceServer                = c_strMenuNameDebugTrace + ":&Trace Server";
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
    m_pActModeSimulation(nullptr),
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
    m_pActViewZoom(nullptr),
    m_pCmbViewZoom(nullptr),
    // Menu - Trace
    m_pMenuDebugTrace(nullptr),
    m_pActTraceErrLog(nullptr),
    m_pActTraceServer(nullptr),
    m_pActionDebugTest(nullptr),
    // Menu - Info
    m_pMenuInfo(nullptr),
    m_pActInfoVersion(nullptr),
    // Dock Widgets
    // Dock Widget - Object Factories
    m_pDockWdgtObjFactories(nullptr),
    m_pTreeViewObjFactories(nullptr),
    m_pModelObjFactories(nullptr),
    // Dock Widget - GraphObjs (Tabs with tree views with differently sorted and ordered graphical objects)
    m_pDockWdgtGraphObjs(nullptr),
    m_pTabWdgtGraphObjs(nullptr),
    // Dock Widget - GraphObjs - Tab GraphicsItems (tree View with graphics items as in drawing scene's items list)
    m_pWdgtGraphicsItems(nullptr),
    m_pLytGraphicsItems(nullptr),
    m_pLytGraphicsItemsLineRefresh(nullptr),
    m_pLblGraphicsItemsRefresh(nullptr),
    m_pBtnGraphicsItemsRefresh(nullptr),
    m_pTreeViewGraphicsItems(nullptr),
    m_pModelGraphicsItems(nullptr),
    // Dialogs
    m_pDlgTest(nullptr),
    // Status Bar
    m_pLblStatusBarDrawingSceneEditTool(nullptr),
    m_pLblStatusBarDrawingSceneEditMode(nullptr),
    m_pLblStatusBarDrawingSceneGraphObjEditInfo(nullptr),
    m_pLblStatusBarDrawingSceneMouseCursorPos(nullptr),
    m_pLblStatusBarDrawingViewMouseCursorPos(nullptr),
    // Central Widget with Drawing
    m_pLyt(nullptr),
    m_pDrawingScene(nullptr),
    m_pDrawingView(nullptr),
    m_pWdgtCentral(nullptr),
    // Trace
    m_pTrcAdminObj(nullptr)
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
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // Graph Object Factories
    //-----------------------

    createObjFactories();

    // Actions
    //--------

    createActions();

    // Toolbox
    //--------

    //createToolBox();

    // Menu
    //-----

    createMenus();

    // Status bar
    //-----------

    m_pLblStatusBarDrawingSceneEditTool = new QLabel("Tool: -");
    m_pLblStatusBarDrawingSceneEditTool->setMinimumWidth(80);
    statusBar()->addPermanentWidget(m_pLblStatusBarDrawingSceneEditTool);

    m_pLblStatusBarDrawingSceneEditMode = new QLabel("");
    m_pLblStatusBarDrawingSceneEditMode->setMinimumWidth(100);
    statusBar()->addPermanentWidget(m_pLblStatusBarDrawingSceneEditMode);

    m_pLblStatusBarDrawingSceneGraphObjEditInfo = new QLabel("Edit: -");
    m_pLblStatusBarDrawingSceneGraphObjEditInfo->setMinimumWidth(120);
    statusBar()->addPermanentWidget(m_pLblStatusBarDrawingSceneGraphObjEditInfo);

    m_pLblStatusBarDrawingSceneMouseCursorPos = new QLabel( "ScenePos: (x/y)[" + Geometry::GraphDevice()->Pixel()->getSymbol() + "]" );
    m_pLblStatusBarDrawingSceneMouseCursorPos->setMinimumWidth(160);
    statusBar()->addPermanentWidget(m_pLblStatusBarDrawingSceneMouseCursorPos);

    m_pLblStatusBarDrawingViewMouseCursorPos = new QLabel( "ViewPos: (x/y)[" + Geometry::GraphDevice()->Pixel()->getSymbol() + "]" );
    m_pLblStatusBarDrawingViewMouseCursorPos->setMinimumWidth(160);
    statusBar()->addPermanentWidget(m_pLblStatusBarDrawingViewMouseCursorPos);

    // Drawing Scene
    //---------------

    // Belongs to central widget but must be created before the tool bars.

    m_pDrawingScene = new CDrawingScene(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(changed(const QList<QRectF>&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneChanged(const QList<QRectF>&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(selectionChanged()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneSelectionChanged()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(mousePosChanged(const QPointF&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneMousePosChanged(const QPointF&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(modeChanged()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneModeChanged()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(drawSettingsChanged(const ZS::Draw::CDrawSettings&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneDrawSettingsChanged(const ZS::Draw::CDrawSettings&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(graphObjCreated(ZS::Draw::CGraphObj*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneGraphObjCreated(ZS::Draw::CGraphObj*)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(graphObjDestroyed(const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneGraphObjDestroyed(const QString&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(graphObjIdChanged(const QString&,const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneGraphObjIdChanged(const QString&,const QString&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // ToolBars
    //---------

    createToolBars();

    // DockWidgets
    //------------

    createDockWidgets();

    // Central Widget
    //---------------

    // Tool bars must have been created before.

    m_pLyt = new QHBoxLayout();

    m_pDrawingView = new CDrawingView(
        /* pDrawingScene */ m_pDrawingScene,
        /* pWdgtParent   */ nullptr );
    m_pDrawingView->setMouseTracking(true);
    m_pLyt->addWidget(m_pDrawingView);

    m_pWdgtCentral = new QWidget();
    m_pWdgtCentral->setLayout(m_pLyt);

    setCentralWidget(m_pWdgtCentral);
    setWindowTitle();
    setUnifiedTitleAndToolBarOnMac(true);

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingView,
        /* szSignal     */ SIGNAL(mousePosChanged(const QPointF&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingViewMousePosChanged(const QPointF&)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // Restore Geometry
    //-----------------

    QSettings settings;

    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

    // Initialize Status Settings
    //---------------------------

    if( m_pDrawingScene != nullptr )
    {
        onDrawingSceneModeChanged();

        onDrawingSceneDrawSettingsChanged( m_pDrawingScene->getDrawSettings() );
    }

} // ctor

//------------------------------------------------------------------------------
CMainWindow::~CMainWindow()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    QSettings settings;

    settings.setValue( objectName()+"/Geometry", saveGeometry() );

    CDialog::DestroyAllInstances();

    try
    {
        delete m_pDlgTest;
    }
    catch(...)
    {
    }
    m_pDlgTest = nullptr;

    try
    {
        delete m_pModelGraphicsItems;
    }
    catch(...)
    {
    }
    m_pModelGraphicsItems = nullptr;

    try
    {
        delete m_pModelObjFactories;
    }
    catch(...)
    {
    }
    m_pModelObjFactories = nullptr;

    try
    {
        delete m_pObjFactoryPoint;
    }
    catch(...)
    {
    }
    m_pObjFactoryPoint = nullptr;

    try
    {
        delete m_pObjFactoryLine;
    }
    catch(...)
    {
    }
    m_pObjFactoryLine = nullptr;

    try
    {
        delete m_pObjFactoryRect;
    }
    catch(...)
    {
    }
    m_pObjFactoryRect = nullptr;

    try
    {
        delete m_pObjFactoryEllipse;
    }
    catch(...)
    {
    }
    m_pObjFactoryEllipse = nullptr;

    try
    {
        delete m_pObjFactoryPolyline;
    }
    catch(...)
    {
    }
    m_pObjFactoryPolyline = nullptr;

    try
    {
        delete m_pObjFactoryPolygon;
    }
    catch(...)
    {
    }
    m_pObjFactoryPolygon = nullptr;

    try
    {
        delete m_pObjFactoryText;
    }
    catch(...)
    {
    }
    m_pObjFactoryText = nullptr;

    try
    {
        delete m_pObjFactoryImage;
    }
    catch(...)
    {
    }
    m_pObjFactoryImage = nullptr;

    try
    {
        delete m_pObjFactoryConnectionPoint;
    }
    catch(...)
    {
    }
    m_pObjFactoryConnectionPoint = nullptr;

    try
    {
        delete m_pObjFactoryConnectionLine;
    }
    catch(...)
    {
    }
    m_pObjFactoryConnectionLine = nullptr;

    try
    {
        delete m_pObjFactoryGroup;
    }
    catch(...)
    {
    }
    m_pObjFactoryGroup = nullptr;

    try
    {
        delete m_pObjFactoryWdgtCheckBox;
    }
    catch(...)
    {
    }
    m_pObjFactoryWdgtCheckBox = nullptr;

    try
    {
        delete m_pObjFactoryWdgtComboBox;
    }
    catch(...)
    {
    }
    m_pObjFactoryWdgtComboBox = nullptr;

    try
    {
        delete m_pObjFactoryWdgtGroupBox;
    }
    catch(...)
    {
    }
    m_pObjFactoryWdgtGroupBox = nullptr;

    try
    {
        delete m_pObjFactoryWdgtLabel;
    }
    catch(...)
    {
    }
    m_pObjFactoryWdgtLabel = nullptr;

    try
    {
        delete m_pObjFactoryWdgtLineEdit;
    }
    catch(...)
    {
    }
    m_pObjFactoryWdgtLineEdit = nullptr;

    try
    {
        delete m_pObjFactoryWdgtPushButton;
    }
    catch(...)
    {
    }
    m_pObjFactoryWdgtPushButton = nullptr;

    try
    {
        delete m_pObjFactoryElectricityCapacitor;
    }
    catch(...)
    {
    }
    m_pObjFactoryElectricityCapacitor = nullptr;

    try
    {
        delete m_pObjFactoryElectricityDiode;
    }
    catch(...)
    {
    }
    m_pObjFactoryElectricityDiode = nullptr;

    try
    {
        delete m_pObjFactoryElectricityInductor;
    }
    catch(...)
    {
    }
    m_pObjFactoryElectricityInductor = nullptr;

    try
    {
        delete m_pObjFactoryElectricityResistor;
    }
    catch(...)
    {
    }
    m_pObjFactoryElectricityResistor = nullptr;

    try
    {
        delete m_pObjFactoryElectricitySwitch;
    }
    catch(...)
    {
    }
    m_pObjFactoryElectricitySwitch = nullptr;

    try
    {
        delete m_pObjFactoryElectricityTransistor;
    }
    catch(...)
    {
    }
    m_pObjFactoryElectricityTransistor = nullptr;

    try
    {
        delete m_pObjFactoryElectricityVoltageSource;
    }
    catch(...)
    {
    }
    m_pObjFactoryElectricityVoltageSource = nullptr;


    // Remove status bar section.
    if( m_pLblStatusBarDrawingSceneEditTool != nullptr )
    {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSceneEditTool);
        delete m_pLblStatusBarDrawingSceneEditTool;
        m_pLblStatusBarDrawingSceneEditTool = nullptr;
    }
    if( m_pLblStatusBarDrawingSceneEditMode != nullptr )
    {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSceneEditMode);
        delete m_pLblStatusBarDrawingSceneEditMode;
        m_pLblStatusBarDrawingSceneEditMode = nullptr;
    }
    if( m_pLblStatusBarDrawingSceneGraphObjEditInfo != nullptr )
    {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSceneGraphObjEditInfo);
        delete m_pLblStatusBarDrawingSceneGraphObjEditInfo;
        m_pLblStatusBarDrawingSceneGraphObjEditInfo = nullptr;
    }
    if( m_pLblStatusBarDrawingSceneMouseCursorPos != nullptr )
    {
        statusBar()->removeWidget(m_pLblStatusBarDrawingSceneMouseCursorPos);
        delete m_pLblStatusBarDrawingSceneMouseCursorPos;
        m_pLblStatusBarDrawingSceneMouseCursorPos = nullptr;
    }
    if( m_pLblStatusBarDrawingViewMouseCursorPos != nullptr )
    {
        statusBar()->removeWidget(m_pLblStatusBarDrawingViewMouseCursorPos);
        delete m_pLblStatusBarDrawingViewMouseCursorPos;
        m_pLblStatusBarDrawingViewMouseCursorPos = nullptr;
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

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
    m_pActModeSimulation = nullptr;
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
    m_pActViewZoom = nullptr;
    m_pCmbViewZoom = nullptr;
    // Menu - Trace
    m_pMenuDebugTrace = nullptr;
    m_pActTraceErrLog = nullptr;
    m_pActTraceServer = nullptr;
    m_pActionDebugTest = nullptr;
    // Menu - Info
    m_pMenuInfo = nullptr;
    m_pActInfoVersion = nullptr;
    // Dock Widgets
    // Dock Widget - Object Factories
    m_pDockWdgtObjFactories = nullptr;
    m_pTreeViewObjFactories = nullptr;
    m_pModelObjFactories = nullptr;
    // Dock Widget - GraphObjs (Tabs with tree views with differently sorted and ordered graphical objects)
    m_pDockWdgtGraphObjs = nullptr;
    m_pTabWdgtGraphObjs = nullptr;
    // Dock Widget - GraphObjs - Tab GraphicsItems (tree View with graphics items as in drawing scene's items list)
    m_pWdgtGraphicsItems = nullptr;
    m_pLytGraphicsItems = nullptr;
    m_pLytGraphicsItemsLineRefresh = nullptr;
    m_pLblGraphicsItemsRefresh = nullptr;
    m_pBtnGraphicsItemsRefresh = nullptr;
    m_pTreeViewGraphicsItems = nullptr;
    m_pModelGraphicsItems = nullptr;
    // Dialogs
    m_pDlgTest = nullptr;
    // Status Bar
    m_pLblStatusBarDrawingSceneEditTool = nullptr;
    m_pLblStatusBarDrawingSceneEditMode = nullptr;
    m_pLblStatusBarDrawingSceneGraphObjEditInfo = nullptr;
    m_pLblStatusBarDrawingSceneMouseCursorPos = nullptr;
    m_pLblStatusBarDrawingViewMouseCursorPos = nullptr;
    // Central Widget with Drawing
    m_pLyt = nullptr;
    m_pDrawingScene = nullptr;
    m_pDrawingView = nullptr;
    m_pWdgtCentral = nullptr;
    // Trace
    m_pTrcAdminObj = nullptr;

    s_pThis = nullptr;

} // dtor

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setWindowTitle()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "FileName:" + m_strCurrentFile;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setWindowTitle",
        /* strAddInfo   */ strAddTrcInfo );

    QString strWindowTitle = m_strWindowTitleAppName;

    QString strFileName = tr("Unnamed");

    if( !m_strCurrentFile.isEmpty() )
    {
        QFileInfo fileInfo(m_strCurrentFile);
        strFileName = fileInfo.fileName();
    }

    EMode mode = EMode::Edit;

    if( m_pDrawingScene != nullptr )
    {
        mode = m_pDrawingScene->getMode();
    }

    strWindowTitle += " - " + CEnumMode::toString(mode);

    strWindowTitle += " - " + strFileName;

    QMainWindow::setWindowTitle(strWindowTitle);

} // setWindowTitle

/*==============================================================================
protected: // instance methods (for ctor)
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::createObjFactories()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createObjFactories",
        /* strAddInfo   */ "" );

    // <Menu> Edit
    //============

    // <MenuItem> Edit::Group
    //-----------------------

    QIcon   iconEditGroup;
    QPixmap pxmEditGroup16x16(":/ZS/Draw/EditGroup16x16.bmp");

    pxmEditGroup16x16.setMask(pxmEditGroup16x16.createMaskFromColor(Qt::white));

    iconEditGroup.addPixmap(pxmEditGroup16x16);

    m_pObjFactoryGroup = new CObjFactoryGroup(false,iconEditGroup);

    // <Menu> Draw
    //============

    // <Menu> Draw::Standard Shapes
    //---------------------------------------------

    // <MenuItem> Draw::Standard Shapes::Draw Point
    //---------------------------------------------

    QIcon   iconDrawStandardShapesPoint;
    QPixmap pxmDrawStandardShapesPoint16x16(":/ZS/Draw/DrawPoint16x16.bmp");

    pxmDrawStandardShapesPoint16x16.setMask(pxmDrawStandardShapesPoint16x16.createHeuristicMask());

    iconDrawStandardShapesPoint.addPixmap(pxmDrawStandardShapesPoint16x16);

    m_pObjFactoryPoint = new CObjFactoryPoint(false,iconDrawStandardShapesPoint);

    // <MenuItem> Draw::Standard Shapes::Draw Line
    //--------------------------------------------

    QIcon   iconDrawStandardShapesLine;
    QPixmap pxmDrawStandardShapesLine16x16(":/ZS/Draw/DrawLine16x16.bmp");

    pxmDrawStandardShapesLine16x16.setMask(pxmDrawStandardShapesLine16x16.createHeuristicMask());

    iconDrawStandardShapesLine.addPixmap(pxmDrawStandardShapesLine16x16);

    m_pObjFactoryLine = new CObjFactoryLine(false,iconDrawStandardShapesLine);

    // <MenuItem> Draw::Standard Shapes::Draw Rect
    //--------------------------------------------

    QIcon   iconDrawStandardShapesRect;
    QPixmap pxmDrawStandardShapesRect16x16(":/ZS/Draw/DrawRect16x16.bmp");

    pxmDrawStandardShapesRect16x16.setMask(pxmDrawStandardShapesRect16x16.createHeuristicMask());

    iconDrawStandardShapesRect.addPixmap(pxmDrawStandardShapesRect16x16);

    m_pObjFactoryRect = new CObjFactoryRect(false,iconDrawStandardShapesRect);

    // <MenuItem> Draw::Standard Shapes::Draw Ellipse
    //-----------------------------------------------

    QIcon   iconDrawStandardShapesEllipse;
    QPixmap pxmDrawStandardShapesEllipse16x16(":/ZS/Draw/DrawEllipse16x16.bmp");

    pxmDrawStandardShapesEllipse16x16.setMask(pxmDrawStandardShapesEllipse16x16.createHeuristicMask());

    iconDrawStandardShapesEllipse.addPixmap(pxmDrawStandardShapesEllipse16x16);

    m_pObjFactoryEllipse = new CObjFactoryEllipse(false,iconDrawStandardShapesEllipse);

    // <MenuItem> Draw::Standard Shapes::Draw Polyline
    //------------------------------------------------

    QIcon   iconDrawStandardShapesPolyline;
    QPixmap pxmDrawStandardShapesPolyline16x16(":/ZS/Draw/DrawPolyline16x16.bmp");

    pxmDrawStandardShapesPolyline16x16.setMask(pxmDrawStandardShapesPolyline16x16.createHeuristicMask());

    iconDrawStandardShapesPolyline.addPixmap(pxmDrawStandardShapesPolyline16x16);

    m_pObjFactoryPolyline = new CObjFactoryPolyline(false,iconDrawStandardShapesPolyline);

    // <MenuItem> Draw::Standard Shapes::Draw Polygon
    //------------------------------------------------

    QIcon   iconDrawStandardShapesPolygon;
    QPixmap pxmDrawStandardShapesPolygon16x16(":/ZS/Draw/DrawPolygon16x16.bmp");

    pxmDrawStandardShapesPolygon16x16.setMask(pxmDrawStandardShapesPolygon16x16.createHeuristicMask());

    iconDrawStandardShapesPolygon.addPixmap(pxmDrawStandardShapesPolygon16x16);

    m_pObjFactoryPolygon = new CObjFactoryPolygon(false,iconDrawStandardShapesPolygon);

    // <MenuItem> Draw::Standard Shapes::Draw Text
    //------------------------------------------------

    QIcon   iconDrawStandardShapesText;
    QPixmap pxmDrawStandardShapesText16x16(":/ZS/Draw/DrawText16x16.bmp");

    pxmDrawStandardShapesText16x16.setMask(pxmDrawStandardShapesText16x16.createHeuristicMask());

    iconDrawStandardShapesText.addPixmap(pxmDrawStandardShapesText16x16);

    m_pObjFactoryText = new CObjFactoryText(false,iconDrawStandardShapesText);

    // <Menu> Draw::Graphics
    //----------------------

    // <MenuItem> Draw::Graphics::Image
    //---------------------------------

    QIcon   iconDrawGraphicsImage;
    QPixmap pxmDrawGraphicsImage16x16(":/ZS/Draw/DrawImage16x16.bmp");

    pxmDrawGraphicsImage16x16.setMask(pxmDrawGraphicsImage16x16.createHeuristicMask());

    iconDrawGraphicsImage.addPixmap(pxmDrawGraphicsImage16x16);

    m_pObjFactoryImage = new CObjFactoryImage(false,iconDrawGraphicsImage);

    // <Menu> Draw::Connections
    //-------------------------

    // <MenuItem> Draw::Connections::Draw Connection Point
    //----------------------------------------------------

    QIcon   iconDrawConnectionPoint;
    QPixmap pxmDrawConnectionPoint16x16(":/ZS/Draw/DrawConnectionPoint16x16.bmp");

    pxmDrawConnectionPoint16x16.setMask(pxmDrawConnectionPoint16x16.createHeuristicMask());

    iconDrawConnectionPoint.addPixmap(pxmDrawConnectionPoint16x16);

    m_pObjFactoryConnectionPoint = new CObjFactoryConnectionPoint(false,iconDrawConnectionPoint);

    // <MenuItem> Draw::Connections::Draw Connection Line
    //----------------------------------------------------

    QIcon   iconDrawConnectionLine;
    QPixmap pxmDrawConnectionLine16x16(":/ZS/Draw/DrawConnectionLine16x16.bmp");

    pxmDrawConnectionLine16x16.setMask(pxmDrawConnectionLine16x16.createHeuristicMask());

    iconDrawConnectionLine.addPixmap(pxmDrawConnectionLine16x16);

    m_pObjFactoryConnectionLine = new CObjFactoryConnectionLine(false,iconDrawConnectionLine);

    // <Menu> Draw::Widgets
    //----------------------

    if( m_uAddObjFactories & EAddObjFactoriesQtWidgets )
    {
        // <MenuItem> Draw::Widgets::CheckBox
        //-----------------------------------

        QIcon   iconDrawWdgtCheckBox;
        QPixmap pxmDrawWdgtCheckBox16x16(":/ZS/Draw/QtWidgets/CheckBox16x16.bmp");

        pxmDrawWdgtCheckBox16x16.setMask(pxmDrawWdgtCheckBox16x16.createHeuristicMask());

        iconDrawWdgtCheckBox.addPixmap(pxmDrawWdgtCheckBox16x16);

        m_pObjFactoryWdgtCheckBox = new CObjFactoryWdgtCheckBox(true,iconDrawWdgtCheckBox);

        // <MenuItem> Draw::Widgets::ComboBox
        //-----------------------------------

        QIcon   iconDrawWdgtComboBox;
        QPixmap pxmDrawWdgtComboBox16x16(":/ZS/Draw/QtWidgets/ComboBox16x16.bmp");

        pxmDrawWdgtComboBox16x16.setMask(pxmDrawWdgtComboBox16x16.createHeuristicMask());

        iconDrawWdgtComboBox.addPixmap(pxmDrawWdgtComboBox16x16);

        m_pObjFactoryWdgtComboBox = new CObjFactoryWdgtComboBox(true,iconDrawWdgtComboBox);

        // <MenuItem> Draw::Widgets::GroupBox
        //-----------------------------------

        QIcon   iconDrawWdgtGroupBox;
        QPixmap pxmDrawWdgtGroupBox16x16(":/ZS/Draw/QtWidgets/GroupBox16x16.bmp");

        pxmDrawWdgtGroupBox16x16.setMask(pxmDrawWdgtGroupBox16x16.createHeuristicMask());

        iconDrawWdgtGroupBox.addPixmap(pxmDrawWdgtGroupBox16x16);

        m_pObjFactoryWdgtGroupBox = new CObjFactoryWdgtGroupBox(true,iconDrawWdgtGroupBox);

        // <MenuItem> Draw::Widgets::Label
        //-----------------------------------

        QIcon   iconDrawWdgtLabel;
        QPixmap pxmDrawWdgtLabel16x16(":/ZS/Draw/QtWidgets/Label16x16.bmp");

        pxmDrawWdgtLabel16x16.setMask(pxmDrawWdgtLabel16x16.createHeuristicMask());

        iconDrawWdgtLabel.addPixmap(pxmDrawWdgtLabel16x16);

        m_pObjFactoryWdgtLabel = new CObjFactoryWdgtLabel(true,iconDrawWdgtLabel);

        // <MenuItem> Draw::Widgets::LineEdit
        //-----------------------------------

        QIcon   iconDrawWdgtLineEdit;
        QPixmap pxmDrawWdgtLineEdit16x16(":/ZS/Draw/QtWidgets/LineEdit16x16.bmp");

        pxmDrawWdgtLineEdit16x16.setMask(pxmDrawWdgtLineEdit16x16.createHeuristicMask());

        iconDrawWdgtLineEdit.addPixmap(pxmDrawWdgtLineEdit16x16);

        m_pObjFactoryWdgtLineEdit = new CObjFactoryWdgtLineEdit(true,iconDrawWdgtLineEdit);

        // <MenuItem> Draw::Widgets::PushButton
        //-------------------------------------

        QIcon   iconDrawWdgtPushButton;
        QPixmap pxmDrawWdgtPushButton16x16(":/ZS/Draw/QtWidgets/PushButton16x16.bmp");

        pxmDrawWdgtPushButton16x16.setMask(pxmDrawWdgtPushButton16x16.createHeuristicMask());

        iconDrawWdgtPushButton.addPixmap(pxmDrawWdgtPushButton16x16);

        m_pObjFactoryWdgtPushButton = new CObjFactoryWdgtPushButton(true,iconDrawWdgtPushButton);

    } // if( m_uAddObjFactories & EAddObjFactoriesQtWidgets )

    // <Menu> Draw::Electricity
    //-------------------------

    if( m_uAddObjFactories & EAddObjFactoriesElectricity )
    {
        // <MenuItem> Draw::Electricity::VoltageSource
        //--------------------------------------------

        QIcon   iconDrawVoltageSource;
        QPixmap pxmDrawVoltageSource(":/ZS/Draw/Electricity/VoltageSource16x16.bmp");

        pxmDrawVoltageSource.setMask(pxmDrawVoltageSource.createHeuristicMask());

        iconDrawVoltageSource.addPixmap(pxmDrawVoltageSource);

        m_pObjFactoryElectricityVoltageSource = new CObjFactoryVoltageSource(true,iconDrawVoltageSource);

        // <MenuItem> Draw::Electricity::Resistor
        //---------------------------------------

        QIcon   iconDrawResistor;
        QPixmap pxmDrawResistor(":/ZS/Draw/Electricity/Resistor16x16.bmp");

        pxmDrawResistor.setMask(pxmDrawResistor.createHeuristicMask());

        iconDrawResistor.addPixmap(pxmDrawResistor);

        m_pObjFactoryElectricityResistor = new CObjFactoryResistor(true,iconDrawResistor);

        // <MenuItem> Draw::Electricity::Inductor
        //---------------------------------------

        QIcon   iconDrawInductor;
        QPixmap pxmDrawInductor(":/ZS/Draw/Electricity/Inductor16x16.bmp");

        pxmDrawInductor.setMask(pxmDrawInductor.createHeuristicMask());

        iconDrawInductor.addPixmap(pxmDrawInductor);

        m_pObjFactoryElectricityInductor = new CObjFactoryInductor(true,iconDrawInductor);

        // <MenuItem> Draw::Electricity::Capacitor
        //---------------------------------------

        QIcon   iconDrawCapacitor;
        QPixmap pxmDrawCapacitor(":/ZS/Draw/Electricity/Capacitor16x16.bmp");

        pxmDrawCapacitor.setMask(pxmDrawCapacitor.createHeuristicMask());

        iconDrawCapacitor.addPixmap(pxmDrawCapacitor);

        m_pObjFactoryElectricityCapacitor = new CObjFactoryCapacitor(true,iconDrawCapacitor);

        // <MenuItem> Draw::Electricity::Switch
        //---------------------------------------

        QIcon   iconDrawSwitch;
        QPixmap pxmDrawSwitch(":/ZS/Draw/Electricity/Switch16x16.bmp");

        pxmDrawSwitch.setMask(pxmDrawSwitch.createHeuristicMask());

        iconDrawSwitch.addPixmap(pxmDrawSwitch);

        m_pObjFactoryElectricitySwitch = new CObjFactorySwitch(true,iconDrawSwitch);

        // <MenuItem> Draw::Electricity::Diode
        //-----------------------------------------

        QIcon   iconDrawDiode;
        QPixmap pxmDrawDiode(":/ZS/Draw/Electricity/Diode16x16.bmp");

        pxmDrawDiode.setMask(pxmDrawDiode.createHeuristicMask());

        iconDrawDiode.addPixmap(pxmDrawDiode);

        m_pObjFactoryElectricityDiode = new CObjFactoryDiode(true,iconDrawDiode);

        // <MenuItem> Draw::Electricity::Transistor
        //-----------------------------------------

        QIcon   iconDrawTransistor;
        QPixmap pxmDrawTransistor(":/ZS/Draw/Electricity/Transistor16x16.bmp");

        pxmDrawTransistor.setMask(pxmDrawTransistor.createHeuristicMask());

        iconDrawTransistor.addPixmap(pxmDrawTransistor);

        m_pObjFactoryElectricityTransistor = new CObjFactoryTransistor(true,iconDrawTransistor);

    } // if( m_uAddObjFactories & EAddObjFactoriesElectricity )

} // createObjFactories

//------------------------------------------------------------------------------
void CMainWindow::createActions()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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

    if( !connect(
        /* pObjSender   */ m_pActFileNew,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionFileNewTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Open
    //----------------------

    QIcon iconFileOpen;

    QPixmap pxmFileOpen16x16(":/ZS/Menu/MenuFileOpen16x16.bmp");

    pxmFileOpen16x16.setMask(pxmFileOpen16x16.createHeuristicMask());

    iconFileOpen.addPixmap(pxmFileOpen16x16);

    m_pActFileOpen = new QAction( iconFileOpen, c_strActionNameFileOpen.section(":",-1,-1), this );
    m_pActFileOpen->setShortcuts(QKeySequence::Open);
    m_pActFileOpen->setStatusTip(tr("Open an existing file"));

    if( !connect(
        /* pObjSender   */ m_pActFileOpen,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionFileOpenTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Save
    //----------------------

    QIcon iconFileSave;

    QPixmap pxmFileSave16x16(":/ZS/Menu/MenuFileSave16x16.bmp");

    pxmFileSave16x16.setMask(pxmFileSave16x16.createHeuristicMask());

    iconFileSave.addPixmap(pxmFileSave16x16);

    m_pActFileSave = new QAction( iconFileSave, c_strActionNameFileSave.section(":",-1,-1), this );
    m_pActFileSave->setShortcuts(QKeySequence::Save);
    m_pActFileSave->setStatusTip(tr("Save current file"));

    if( !connect(
        /* pObjSender   */ m_pActFileSave,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionFileSaveTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Save As
    //-------------------------

    QIcon iconFileSaveAs;

    QPixmap pxmFileSaveAs16x16(":/ZS/Menu/MenuFileSaveAs16x16.bmp");

    pxmFileSaveAs16x16.setMask(pxmFileSaveAs16x16.createHeuristicMask());

    iconFileSaveAs.addPixmap(pxmFileSaveAs16x16);

    m_pActFileSaveAs = new QAction( iconFileSaveAs, c_strActionNameFileSaveAs.section(":",-1,-1), this );
    m_pActFileSaveAs->setShortcuts(QKeySequence::SaveAs);
    m_pActFileSaveAs->setStatusTip(tr("Save current file choosing a different file name"));

    if( !connect(
        /* pObjSender   */ m_pActFileSaveAs,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionFileSaveAsTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Page Setup
    //----------------------------

    QIcon   iconFilePageSetup;
    QPixmap pxmFilePageSetup16x16(":/ZS/Draw/FilePageSetup16x16.bmp");

    pxmFilePageSetup16x16.setMask(pxmFilePageSetup16x16.createHeuristicMask());

    iconFilePageSetup.addPixmap(pxmFilePageSetup16x16);

    m_pActFilePageSetup = new QAction( iconFilePageSetup, c_strActionNameFilePageSetup.section(":",-1,-1), this );
    m_pActFilePageSetup->setShortcuts(QKeySequence::Preferences);
    m_pActFilePageSetup->setStatusTip(tr("Set properties of current page"));

    if( !connect(
        /* pObjSender   */ m_pActFilePageSetup,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionFilePageSetupTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

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

                if( !connect(
                    /* pObjSender   */ m_arpActFilesRecent[idxFile],
                    /* szSignal     */ SIGNAL(triggered(bool)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onActionFileRecentTriggered(bool)) ) )
                {
                    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
                }
            }
        }

        updateActionsFilesRecent();

    } // if( bHasActFilesRecent )

    // <MenuItem> File::Quit
    //----------------------

    m_pActFileQuit = new QAction( c_strActionNameFileQuit.section(":",-1,-1), this );
    m_pActFileQuit->setStatusTip( tr("Quit application") );

    if( !QObject::connect(
        /* pObjSender   */ m_pActFileQuit,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ QApplication::instance(),
        /* szSlot       */ SLOT(onLastWindowClosed()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

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

    if( !connect(
        /* pObjSender   */ m_pActModeEdit,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionModeEditToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Mode::Simulation
    //-------------------------------

    QIcon iconModeSimulation;

    iconModeSimulation.addPixmap( mode2Pixmap(static_cast<int>(EMode::Simulation),24) );

    m_pActModeSimulation = new QAction( iconModeSimulation, c_strActionNameModeSimulation.section(":",-1,-1), this );
    m_pActModeSimulation->setStatusTip( tr("Activate Simulation Mode") );
    m_pActModeSimulation->setCheckable(true);

    if( !connect(
        /* pObjSender   */ m_pActModeSimulation,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionModeSimulationToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

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

    if( !connect(
        /* pObjSender   */ m_pActEditSelect,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionEditSelectToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Edit::Rotate::Left
    //------------------------------

    QIcon   iconEditRotateLeft;
    QPixmap pxmEditRotateLeft(":/ZS/Draw/DrawToolRotateLeftBy90Degrees16x16.bmp");

    pxmEditRotateLeft.setMask(pxmEditRotateLeft.createHeuristicMask());

    iconEditRotateLeft.addPixmap(pxmEditRotateLeft);

    m_pActEditRotateLeft = new QAction( iconEditRotateLeft, c_strActionNameEditRotateLeft.section(":",-1,-1), this );
    m_pActEditRotateLeft->setStatusTip( tr("Rotate Selected Object(s) Left (counterclockwise)") );
    m_pActEditRotateLeft->setEnabled(false);

    if( !connect(
        /* pObjSender   */ m_pActEditRotateLeft,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionEditRotateLeftTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Edit::Rotate::Right
    //-------------------------------

    QIcon   iconEditRotateRight;
    QPixmap pxmEditRotateRight(":/ZS/Draw/DrawToolRotateRightBy90Degrees16x16.bmp");

    pxmEditRotateRight.setMask(pxmEditRotateRight.createHeuristicMask());

    iconEditRotateRight.addPixmap(pxmEditRotateRight);

    m_pActEditRotateRight = new QAction( iconEditRotateRight, c_strActionNameEditRotateRight.section(":",-1,-1), this );
    m_pActEditRotateRight->setStatusTip( tr("Rotate Selected Object(s) Right (clockwise)") );
    m_pActEditRotateRight->setEnabled(false);

    if( !connect(
        /* pObjSender   */ m_pActEditRotateRight,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionEditRotateRightTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

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

    if( !connect(
        /* pObjSender   */ m_pActEditMirrorVertical,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionEditMirrorVerticalTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Edit::Mirror::Horizontal
    //------------------------------------

    QIcon   iconEditMirrorHorizontal;
    QPixmap pxmEditMirrorHorizontal(":/ZS/Draw/DrawToolMirrorHorizontal16x16.bmp");

    pxmEditMirrorHorizontal.setMask(pxmEditMirrorHorizontal.createHeuristicMask());

    iconEditMirrorHorizontal.addPixmap(pxmEditMirrorHorizontal);

    m_pActEditMirrorHorizontal = new QAction( iconEditMirrorHorizontal, c_strActionNameEditMirrorHorizontal.section(":",-1,-1), this );
    m_pActEditMirrorHorizontal->setStatusTip( tr("Horizontally Mirror Selected Object(s)") );
    m_pActEditMirrorHorizontal->setEnabled(false);

    if( !connect(
        /* pObjSender   */ m_pActEditMirrorHorizontal,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionEditMirrorHorizontalTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Edit::Group
    //-----------------------

    m_pActEditGroup = new QAction( m_pObjFactoryGroup->getToolIcon(), c_strActionNameEditGroup.section(":",-1,-1), this );
    m_pActEditGroup->setStatusTip( tr("Group Selected Objects") );
    m_pActEditGroup->setEnabled(false);

    if( !connect(
        /* pObjSender   */ m_pActEditGroup,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionEditGroupTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Edit::Ungroup
    //-------------------------

    QIcon   iconEditUngroup;
    QPixmap pxmEditUngroup16x16(":/ZS/Draw/EditUngroup16x16.bmp");

    pxmEditUngroup16x16.setMask(pxmEditUngroup16x16.createMaskFromColor(Qt::white));

    iconEditUngroup.addPixmap(pxmEditUngroup16x16);

    m_pActEditUngroup = new QAction( iconEditUngroup, c_strActionNameEditUngroup.section(":",-1,-1), this );
    m_pActEditUngroup->setStatusTip( tr("Ungroup Selected Group(s)") );
    m_pActEditUngroup->setEnabled(false);

    if( !connect(
        /* pObjSender   */ m_pActEditUngroup,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionEditUngroupTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

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

    if( !connect(
        /* pObjSender   */ m_pActDrawSettingsLine,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawSettingsLineTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Draw::Settings::Fill
    //---------------------------------

    QIcon   iconEditDrawSettingsFill;
    QPixmap pxmEditDrawSettingsFill(":/ZS/Draw/DrawToolBrush16x16.bmp");

    pxmEditDrawSettingsFill.setMask(pxmEditDrawSettingsFill.createHeuristicMask());

    iconEditDrawSettingsFill.addPixmap(pxmEditDrawSettingsFill);

    m_pActDrawSettingsFill = new QAction( iconEditDrawSettingsFill, c_strActionNameDrawSettingsFill.section(":",-1,-1), this );
    m_pActDrawSettingsFill->setStatusTip( tr("Draw Settings Fill Style") );

    if( !connect(
        /* pObjSender   */ m_pActDrawSettingsFill,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawSettingsFillTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Draw::Settings::Text
    //---------------------------------

    QIcon   iconEditDrawSettingsText;
    QPixmap pxmEditDrawSettingsText(":/ZS/Draw/DrawToolText16x16.bmp");

    pxmEditDrawSettingsText.setMask(pxmEditDrawSettingsText.createHeuristicMask());

    iconEditDrawSettingsText.addPixmap(pxmEditDrawSettingsText);

    m_pActDrawSettingsText = new QAction( iconEditDrawSettingsText, c_strActionNameDrawSettingsText.section(":",-1,-1), this );
    m_pActDrawSettingsText->setStatusTip( tr("Draw Settings Text Style") );

    if( !connect(
        /* pObjSender   */ m_pActDrawSettingsText,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawSettingsTextTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> Draw::Standard Shapes
    //---------------------------------------------

    // <MenuItem> Draw::Standard Shapes::Draw Point
    //---------------------------------------------

    m_pActDrawStandardShapePoint = new QAction( m_pObjFactoryPoint->getToolIcon(), c_strActionNameDrawStandardShapePoint.section(":",-1,-1), this );
    m_pActDrawStandardShapePoint->setStatusTip( tr("Draw Points") );
    m_pActDrawStandardShapePoint->setCheckable(true);

    if( !connect(
        /* pObjSender   */ m_pActDrawStandardShapePoint,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawStandardShapePointToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Line
    //--------------------------------------------

    m_pActDrawStandardShapeLine = new QAction( m_pObjFactoryLine->getToolIcon(), c_strActionNameDrawStandardShapeLine.section(":",-1,-1), this );
    m_pActDrawStandardShapeLine->setStatusTip( tr("Draw Lines") );
    m_pActDrawStandardShapeLine->setCheckable(true);

    if( !connect(
        /* pObjSender   */ m_pActDrawStandardShapeLine,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawStandardShapeLineToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Rect
    //--------------------------------------------

    m_pActDrawStandardShapeRect = new QAction( m_pObjFactoryRect->getToolIcon(), c_strActionNameDrawStandardShapeRect.section(":",-1,-1), this );
    m_pActDrawStandardShapeRect->setStatusTip( tr("Draw Rectangles") );
    m_pActDrawStandardShapeRect->setCheckable(true);

    if( !connect(
        /* pObjSender   */ m_pActDrawStandardShapeRect,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawStandardShapeRectToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Ellipse
    //-----------------------------------------------

    m_pActDrawStandardShapeEllipse = new QAction( m_pObjFactoryEllipse->getToolIcon(), c_strActionNameDrawStandardShapeEllipse.section(":",-1,-1), this );
    m_pActDrawStandardShapeEllipse->setStatusTip( tr("Draw Ellipses") );
    m_pActDrawStandardShapeEllipse->setCheckable(true);

    if( !connect(
        /* pObjSender   */ m_pActDrawStandardShapeEllipse,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawStandardShapeEllipseToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Polyline
    //------------------------------------------------

    m_pActDrawStandardShapePolyline = new QAction( m_pObjFactoryPolyline->getToolIcon(), c_strActionNameDrawStandardShapePolyline.section(":",-1,-1), this );
    m_pActDrawStandardShapePolyline->setStatusTip( tr("Draw Polylines") );
    m_pActDrawStandardShapePolyline->setCheckable(true);

    if( !connect(
        /* pObjSender   */ m_pActDrawStandardShapePolyline,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawStandardShapePolylineToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Polygon
    //------------------------------------------------

    m_pActDrawStandardShapePolygon = new QAction( m_pObjFactoryPolygon->getToolIcon(), c_strActionNameDrawStandardShapePolygon.section(":",-1,-1), this );
    m_pActDrawStandardShapePolygon->setStatusTip( tr("Draw Polygons") );
    m_pActDrawStandardShapePolygon->setCheckable(true);

    if( !connect(
        /* pObjSender   */ m_pActDrawStandardShapePolygon,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawStandardShapePolygonToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Text
    //------------------------------------------------

    m_pActDrawStandardShapeText = new QAction( m_pObjFactoryText->getToolIcon(), c_strActionNameDrawStandardShapeText.section(":",-1,-1), this );
    m_pActDrawStandardShapeText->setStatusTip( tr("Draw Texts") );
    m_pActDrawStandardShapeText->setCheckable(true);

    if( !connect(
        /* pObjSender   */ m_pActDrawStandardShapeText,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawStandardShapeTextToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> Draw::Graphics
    //----------------------

    // <MenuItem> Draw::Graphics::Image
    //---------------------------------

    m_pActDrawGraphicsImage = new QAction( m_pObjFactoryImage->getToolIcon(), c_strActionNameDrawGraphicsImage.section(":",-1,-1), this );
    m_pActDrawGraphicsImage->setStatusTip( tr("Insert Images") );
    m_pActDrawGraphicsImage->setCheckable(false);

    if( !connect(
        /* pObjSender   */ m_pActDrawGraphicsImage,
        /* szSignal     */ SIGNAL(triggered(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawGraphicsImageTriggered(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> Draw::Connections
    //-------------------------

    // <MenuItem> Draw::Connections::Draw Connection Point
    //----------------------------------------------------

    m_pActDrawConnectionPoint = new QAction( m_pObjFactoryConnectionPoint->getToolIcon(), c_strActionNameDrawConnectionPoint.section(":",-1,-1), this );
    m_pActDrawConnectionPoint->setStatusTip( tr("Draw Connection Point") );
    m_pActDrawConnectionPoint->setCheckable(true);

    if( !connect(
        /* pObjSender   */ m_pActDrawConnectionPoint,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawConnectionPointToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Draw::Connections::Draw Connection Line
    //----------------------------------------------------

    m_pActDrawConnectionLine = new QAction( m_pObjFactoryConnectionLine->getToolIcon(), c_strActionNameDrawConnectionLine.section(":",-1,-1), this );
    m_pActDrawConnectionLine->setStatusTip( tr("Draw Connection Line") );
    m_pActDrawConnectionLine->setCheckable(true);

    if( !connect(
        /* pObjSender   */ m_pActDrawConnectionLine,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActionDrawConnectionLineToggled(bool)) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> View
    //============

    // <MenuItem> View::Zoom
    //----------------------


    // <Menu> Debug & Trace
    //=====================

    // <MenuItem> DebugTrace::Error Log
    //---------------------------------

    if( CErrLog::GetInstance() != nullptr )
    {
        m_pActTraceErrLog = new QAction(c_strActionNameTraceErrLog.section(":",-1,-1),this);

        if( !QObject::connect(
            /* pObjSender   */ m_pActTraceErrLog,
            /* szSignal     */ SIGNAL(triggered(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onActionTraceErrLogTriggered(bool)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    }

    // <MenuItem> DebugTrace::Trace Server
    //------------------------------------

    if( CTrcServer::GetInstance() != nullptr )
    {
        m_pActTraceServer = new QAction(c_strActionNameTraceServer.section(":",-1,-1),this);

        if( !QObject::connect(
            /* pObjSender   */ m_pActTraceServer,
            /* szSignal     */ SIGNAL(triggered(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onActionTraceServerTriggered(bool)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    }

    // <MenuItem> DebugTrace::Test
    //-----------------------------------------------------------------------

    if( m_pTest != nullptr )
    {
        m_pActionDebugTest = new QAction( "Subsystem Test", this );

        if( !connect(
            /* pObjSender   */ m_pActionDebugTest,
            /* szSignal     */ SIGNAL(triggered(bool)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onActionDebugTestTriggered(bool)) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

    } // if( m_pTest != nullptr )

    // <Menu> Info
    //=====================

    // <MenuItem> Info::Version
    //-------------------------

    QString strActionInfoVersion = "Version: " + ZS::System::c_strSysVersionNr;

    #ifdef _DEBUG
    strActionInfoVersion += " (Debug)";
    #endif

    m_pActInfoVersion = new QAction(strActionInfoVersion,this);

} // createActions

//------------------------------------------------------------------------------
void CMainWindow::createMenus()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createMenus",
        /* strAddInfo   */ "" );

    m_pMenuBar = menuBar();

    // <Menu> File
    //============

    m_pMenuFile = m_pMenuBar->addMenu(c_strMenuNameFile);

    // <MenuItem> File::New
    //----------------------

    m_pMenuFile->addAction(m_pActFileNew);

    // <MenuItem> File::Open
    //----------------------

    m_pMenuFile->addAction(m_pActFileOpen);

    // <MenuItem> File::Save
    //----------------------

    m_pMenuFile->addSeparator();

    m_pMenuFile->addAction(m_pActFileSave);

    // <MenuItem> File::Save As
    //-------------------------

    m_pMenuFile->addAction(m_pActFileSaveAs);

    // <MenuItem> File::Page Setup
    //----------------------------

    m_pMenuFile->addSeparator();

    m_pMenuFile->addAction(m_pActFilePageSetup);

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
        m_pActFilesRecentSeparator = m_pMenuFile->addSeparator();

        for( idxFile = 0; idxFile < _ZSArrLen(m_arpActFilesRecent); idxFile++ )
        {
            if( m_arpActFilesRecent[idxFile] != nullptr )
            {
                m_pMenuFile->addAction(m_arpActFilesRecent[idxFile]);
            }
        }

    } // if( bHasActFilesRecent )

    // <MenuItem> File::Quit
    //----------------------

    if( m_pActFileQuit != nullptr )
    {
        m_pMenuFile->addSeparator();
        m_pMenuFile->addAction(m_pActFileQuit);
    }

    // <Menu> Mode
    //============

    m_pMenuMode = m_pMenuBar->addMenu(c_strMenuNameMode);

    // <MenuItem> Mode::Edit
    //----------------------------

    if( m_pActModeEdit != nullptr )
    {
        m_pMenuMode->addAction(m_pActModeEdit);
    }

    // <MenuItem> Mode::Simulation
    //----------------------------

    if( m_pActModeSimulation != nullptr )
    {
        m_pMenuMode->addAction(m_pActModeSimulation);
    }

    // <Menu> Edit
    //============

    m_pMenuEdit = m_pMenuBar->addMenu(c_strMenuNameEdit);

    // <MenuItem> Edit::Select
    //------------------------

    if( m_pActEditSelect != nullptr )
    {
        m_pMenuEdit->addAction(m_pActEditSelect);
    }

    // <MenuItem> Edit::Rotate
    //------------------------

    m_pMenuEditRotate = m_pMenuEdit->addMenu(c_strMenuNameEditRotate.section(":",-1,-1));

    // <MenuItem> Edit::Rotate::Left
    //------------------------------

    if( m_pActEditRotateLeft != nullptr )
    {
        m_pMenuEditRotate->addAction(m_pActEditRotateLeft);
    }

    // <MenuItem> Edit::Rotate::Right
    //-------------------------------

    if( m_pActEditRotateRight != nullptr )
    {
        m_pMenuEditRotate->addAction(m_pActEditRotateRight);
    }

    // <MenuItem> Edit::Mirror
    //------------------------

    m_pMenuEditMirror = m_pMenuEdit->addMenu(c_strMenuNameEditMirror.section(":",-1,-1));

    // <MenuItem> Edit::Mirror::Mirror Vertical
    //------------------------------------------

    if( m_pActEditMirrorVertical != nullptr )
    {
        m_pMenuEditMirror->addAction(m_pActEditMirrorVertical);
    }

    // <MenuItem> Edit::Mirror::Mirror Horizontal
    //-------------------------------------------

    if( m_pActEditMirrorHorizontal != nullptr )
    {
        m_pMenuEditMirror->addAction(m_pActEditMirrorHorizontal);
    }

    // <MenuItem> Edit::Group
    //-----------------------

    if( m_pActEditGroup != nullptr )
    {
        m_pMenuEdit->addAction(m_pActEditGroup);
    }

    // <MenuItem> Edit::Ungroup
    //-------------------------

    if( m_pActEditUngroup != nullptr )
    {
        m_pMenuEdit->addAction(m_pActEditUngroup);
    }

    // <Menu> Draw
    //============

    m_pMenuDraw = m_pMenuBar->addMenu(c_strMenuNameDraw);

    // <MenuItem> Draw::Settings
    //--------------------------

    m_pMenuDrawSettings = m_pMenuDraw->addMenu(c_strMenuNameDrawSettings.section(":",-1,-1));

    // <MenuItem> Draw::Settings::Line
    //--------------------------------

    if( m_pActDrawSettingsLine != nullptr )
    {
        m_pMenuDrawSettings->addAction(m_pActDrawSettingsLine);
    }

    // <MenuItem> Draw::Settings::Fill
    //--------------------------------

    if( m_pActDrawSettingsFill != nullptr )
    {
        m_pMenuDrawSettings->addAction(m_pActDrawSettingsFill);
    }

    // <MenuItem> Draw::Settings::Text
    //--------------------------------

    if( m_pActDrawSettingsText != nullptr )
    {
        m_pMenuDrawSettings->addAction(m_pActDrawSettingsText);
    }

    // <MenuItem> Draw::Standard Shapes
    //---------------------------------

    m_pMenuDrawStandardShapes = m_pMenuDraw->addMenu(c_strMenuNameDrawStandardShapes.section(":",-1,-1));

    // <MenuItem> Draw::Standard Shapes::Draw Point
    //---------------------------------------------

    if( m_pActDrawStandardShapePoint != nullptr )
    {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapePoint);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Line
    //---------------------------------------------

    if( m_pActDrawStandardShapeLine != nullptr )
    {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapeLine);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Rect
    //---------------------------------------------

    if( m_pActDrawStandardShapeRect != nullptr )
    {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapeRect);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Ellipse
    //-----------------------------------------------

    if( m_pActDrawStandardShapeEllipse != nullptr )
    {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapeEllipse);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Polyline
    //------------------------------------------------

    if( m_pActDrawStandardShapePolyline != nullptr )
    {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapePolyline);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Polygon
    //-----------------------------------------------

    if( m_pActDrawStandardShapePolygon != nullptr )
    {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapePolygon);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Text
    //-----------------------------------------------

    if( m_pActDrawStandardShapeText != nullptr )
    {
        m_pMenuDrawStandardShapes->addAction(m_pActDrawStandardShapeText);
    }

    // <MenuItem> Draw::Image
    //---------------------------------

    m_pMenuDrawGraphics = m_pMenuDraw->addMenu(c_strMenuNameDrawGraphics.section(":",-1,-1));

    // <MenuItem> Draw::Graphics:Image
    //---------------------------------------------

    if( m_pActDrawGraphicsImage != nullptr )
    {
        m_pMenuDrawGraphics->addAction(m_pActDrawGraphicsImage);
    }

    // <MenuItem> Draw::Connections
    //-----------------------------

    m_pMenuDrawConnections = m_pMenuDraw->addMenu(c_strMenuNameDrawConnections.section(":",-1,-1));

    // <MenuItem> Draw::Connections::Connection Point
    //-----------------------------------------------

    if( m_pActDrawConnectionPoint != nullptr )
    {
        m_pMenuDrawConnections->addAction(m_pActDrawConnectionPoint);
    }

    // <MenuItem> Draw::Connections::Connection Line
    //-----------------------------------------------

    if( m_pActDrawConnectionLine != nullptr )
    {
        m_pMenuDrawConnections->addAction(m_pActDrawConnectionLine);
    }

    // <Menu> View
    //============

    m_pMenuView = m_pMenuBar->addMenu(c_strMenuNameView);

    // <Menu> Debug & Trace
    //=====================

    if( m_pActTraceErrLog != nullptr || m_pActTraceServer != nullptr || m_pActionDebugTest != nullptr )
    {
        m_pMenuDebugTrace = m_pMenuBar->addMenu(c_strMenuNameDebugTrace);

        // <MenuItem> DebugTrace::Error Log
        //---------------------------------

        if( m_pActTraceErrLog != nullptr )
        {
            m_pMenuDebugTrace->addAction(m_pActTraceErrLog);
        }

        // <MenuItem> DebugTrace::Trace Server
        //------------------------------------

        if( m_pActTraceServer != nullptr )
        {
            m_pMenuDebugTrace->addAction(m_pActTraceServer);
        }

        // <MenuItem> DebugTrace::Test
        //-----------------------------------------------------------------------

        if( m_pActionDebugTest != nullptr )
        {
            m_pMenuDebugTrace->addAction(m_pActionDebugTest);
        }

    } // if( m_pActTraceErrLog != nullptr || m_pActTraceServer != nullptr || m_pActionDebugTest != nullptr )

    // <Menu> Info
    //============

    m_pMenuInfo = m_pMenuBar->addMenu(c_strMenuNameInfo);

    // <MenuItem> Info::Version
    //-------------------------

    if( m_pActInfoVersion != nullptr )
    {
        m_pMenuInfo->addAction(m_pActInfoVersion);
    }

} // createMenus

//------------------------------------------------------------------------------
void CMainWindow::createToolBars()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createToolBars",
        /* strAddInfo   */ "" );

    // <Menu> File
    //============

    m_pToolBarFile = addToolBar("File Operations");
    m_pToolBarFile->setObjectName("File Operations");
    //m_pToolBarFile->setMaximumHeight(24);
    m_pToolBarFile->setIconSize( QSize(16,16) );

    // <MenuItem> File::New
    //----------------------

    if( m_pActFileNew != nullptr )
    {
        m_pToolBarFile->addAction(m_pActFileNew);
    }

    // <MenuItem> File::Open
    //----------------------

    if( m_pActFileOpen != nullptr )
    {
        m_pToolBarFile->addAction(m_pActFileOpen);
    }

    // <MenuItem> File::Save
    //----------------------

    if( m_pActFileSave != nullptr )
    {
        m_pToolBarFile->addAction(m_pActFileSave);
    }

    // <MenuItem> File::Save As
    //-------------------------

    if( m_pActFileSaveAs != nullptr )
    {
        m_pToolBarFile->addAction(m_pActFileSaveAs);
    }

    // <MenuItem> File::Page Setup
    //----------------------------

    if( m_pActFilePageSetup != nullptr )
    {
        m_pToolBarFile->addAction(m_pActFilePageSetup);
    }

    // <Menu> Mode
    //============

    m_pToolBarMode = addToolBar("Switching Between Simulation and Edit Mode");
    m_pToolBarMode->setObjectName("Mode");
    //m_pToolBarMode->setMaximumHeight(24);
    m_pToolBarMode->setIconSize( QSize(16,16) );

    // <MenuItem> Mode::Edit
    //----------------------

    if( m_pActModeEdit != nullptr )
    {
        m_pToolBarMode->addAction(m_pActModeEdit);
    }

    // <MenuItem> Mode::Simulation
    //----------------------------

    if( m_pActModeSimulation != nullptr )
    {
        m_pToolBarMode->addAction(m_pActModeSimulation);
    }

    // <Menu> Edit
    //============

    m_pToolBarEditSelect = addToolBar("Edit Select and Rotate Free");
    m_pToolBarEditSelect->setObjectName("Edit Select and Rotate Free");
    //m_pToolBarEditSelect->setMaximumHeight(24);
    m_pToolBarEditSelect->setIconSize( QSize(16,16) );

    // <MenuItem> Edit::Select
    //------------------------

    if( m_pActEditSelect != nullptr )
    {
        m_pToolBarEditSelect->addAction(m_pActEditSelect);
    }

    // <MenuItems> Rotate and Mirror
    //--------------------------------

    m_pToolBarEditRotateAndMirror = addToolBar("Edit Rotate and Mirror");
    m_pToolBarEditRotateAndMirror->setObjectName("Edit Rotate and Mirror");
    m_pToolBarEditRotateAndMirror->setIconSize( QSize(16,16) );

    // <MenuItem> Edit::Rotate::Left
    //--------------------------------

    if( m_pActEditRotateLeft != nullptr )
    {
        m_pToolBarEditRotateAndMirror->addAction(m_pActEditRotateLeft);
    }

    // <MenuItem> Edit::Rotate::Right
    //---------------------------------

    if( m_pActEditRotateRight != nullptr )
    {
        m_pToolBarEditRotateAndMirror->addAction(m_pActEditRotateRight);
    }

    // <MenuItem> Edit::Rotate::Angle
    //---------------------------------

    m_pEdtEditRotateAngle = new QDoubleSpinBox();
    m_pEdtEditRotateAngle->setRange(0.0,360.0);
    m_pEdtEditRotateAngle->setDecimals(1);
    m_pEdtEditRotateAngle->setSingleStep(1.0);
    m_pEdtEditRotateAngle->setSuffix(" °");
    m_pEdtEditRotateAngle->setValue(90.0);
    m_pToolBarEditRotateAndMirror->addWidget(m_pEdtEditRotateAngle);

    // <MenuItem> Edit::Mirror Vertical
    //---------------------------------

    if( m_pActEditMirrorVertical != nullptr )
    {
        m_pToolBarEditRotateAndMirror->addAction(m_pActEditMirrorVertical);
    }

    // <MenuItem> Edit::Mirror Horizontal
    //-----------------------------------

    if( m_pActEditMirrorHorizontal != nullptr )
    {
        m_pToolBarEditRotateAndMirror->addAction(m_pActEditMirrorHorizontal);
    }

    // <MenuItems> Edit::Group
    //------------------------

    m_pToolBarEditGroup = addToolBar("Edit Group and Ungroup");
    m_pToolBarEditGroup->setObjectName("Edit Group and Ungroup");
    //m_pToolBarEditGroup->setMaximumHeight(24);
    m_pToolBarEditGroup->setIconSize( QSize(16,16) );

    // <MenuItem> Edit::Group
    //-----------------------

    if( m_pActEditGroup != nullptr )
    {
        m_pToolBarEditGroup->addAction(m_pActEditGroup);
    }

    // <MenuItem> Edit::Ungroup
    //-------------------------

    if( m_pActEditGroup != nullptr )
    {
        m_pToolBarEditGroup->addAction(m_pActEditUngroup);
    }

    // <Menu> Draw
    //============

    // <MenuItem> Draw::Settings
    //--------------------------

    m_pToolBarDrawSettings = addToolBar("Draw Settings");
    m_pToolBarDrawSettings->setObjectName("Draw Settings");
    //m_pToolBarDrawSettings->setMaximumHeight(24);
    m_pToolBarDrawSettings->setIconSize( QSize(16,16) );

    // <MenuItem> Draw::Settings::Line
    //--------------------------------

    if( m_pActDrawSettingsLine != nullptr )
    {
        m_pToolBarDrawSettings->addAction(m_pActDrawSettingsLine);
    }

    // <MenuItem> Draw::Settings::Fill
    //---------------------------------

    if( m_pActDrawSettingsFill != nullptr )
    {
        m_pToolBarDrawSettings->addAction(m_pActDrawSettingsFill);
    }

    // <MenuItem> Draw::Settings::Text
    //---------------------------------

    if( m_pActDrawSettingsText != nullptr )
    {
        m_pToolBarDrawSettings->addAction(m_pActDrawSettingsText);
    }

    // <MenuItem> Draw::Standard Shapes
    //---------------------------------

    m_pToolBarDrawStandardShapes = addToolBar("Draw Standard Shapes");
    m_pToolBarDrawStandardShapes->setObjectName("Draw Standard Shapes");
    //m_pToolBarDrawStandardShapes->setMaximumHeight(24);
    m_pToolBarDrawStandardShapes->setIconSize( QSize(16,16) );

    // <MenuItem> Draw::Standard Shapes::Draw Point
    //---------------------------------------------

    if( m_pActDrawStandardShapePoint != nullptr )
    {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapePoint);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Line
    //---------------------------------------------

    if( m_pActDrawStandardShapeLine != nullptr )
    {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapeLine);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Rect
    //---------------------------------------------

    if( m_pActDrawStandardShapeRect != nullptr )
    {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapeRect);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Circle
    //----------------------------------------------

    if( m_pActDrawStandardShapeEllipse != nullptr )
    {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapeEllipse);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Polyline
    //------------------------------------------------

    if( m_pActDrawStandardShapePolyline != nullptr )
    {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapePolyline);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Polygon
    //-----------------------------------------------

    if( m_pActDrawStandardShapePolygon != nullptr )
    {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapePolygon);
    }

    // <MenuItem> Draw::Standard Shapes::Draw Text
    //-----------------------------------------------

    if( m_pActDrawStandardShapeText != nullptr )
    {
        m_pToolBarDrawStandardShapes->addAction(m_pActDrawStandardShapeText);
    }

    // <MenuItem> Draw::Standard Shapes
    //---------------------------------

    m_pToolBarDrawGraphics = addToolBar("Draw Graphics");
    m_pToolBarDrawGraphics->setObjectName("Draw Graphics");
    //m_pToolBarDrawGraphics->setMaximumHeight(24);
    m_pToolBarDrawGraphics->setIconSize( QSize(16,16) );

    // <MenuItem> Draw::Graphics::Image
    //---------------------------------

    if( m_pActDrawGraphicsImage != nullptr )
    {
        m_pToolBarDrawGraphics->addAction(m_pActDrawGraphicsImage);
    }

    // <MenuItem> Draw::Connections
    //-----------------------------

    m_pToolBarDrawConnections = addToolBar("Draw Connections");
    m_pToolBarDrawConnections->setObjectName("Draw Connections");
    //m_pToolBarDrawConnections->setMaximumHeight(24);
    m_pToolBarDrawConnections->setIconSize( QSize(16,16) );

    // <MenuItem> Draw::Connections::Draw Connection Point
    //--------------------------------------------------------

    if( m_pActDrawConnectionPoint != nullptr )
    {
        m_pToolBarDrawConnections->addAction(m_pActDrawConnectionPoint);
    }

    // <MenuItem> Draw::Connections::Draw Connection Line
    //-------------------------------------------------------

    if( m_pActDrawConnectionLine != nullptr )
    {
        m_pToolBarDrawConnections->addAction(m_pActDrawConnectionLine);
    }

} // createToolBars

//------------------------------------------------------------------------------
void CMainWindow::createDockWidgets()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createDockWidgets",
        /* strAddInfo   */ "" );

    // <DockWidget> Tree View Object Factories
    //----------------------------------------

    if( m_uAddObjFactories != EAddObjFactoriesNone )
    {
        m_pDockWdgtObjFactories = new QDockWidget("Tool Box",this);
        m_pDockWdgtObjFactories->setObjectName("Object Factories");
        m_pDockWdgtObjFactories->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

        m_pModelObjFactories = new CObjFactoriesModel( CObjFactory::ObjPoolToolBoxFactories() );

        m_pTreeViewObjFactories = new QTreeView(this);

        m_pTreeViewObjFactories->setModel(m_pModelObjFactories);
        m_pTreeViewObjFactories->setSelectionBehavior(QAbstractItemView::SelectItems);
        m_pTreeViewObjFactories->setSelectionMode(QAbstractItemView::SingleSelection);
        //m_pTreeViewObjFactories->setAllColumnsShowFocus(true);
        m_pTreeViewObjFactories->setDragEnabled(true);
        m_pTreeViewObjFactories->setDropIndicatorShown(true);
        // Please note that the first column should not be hidden to allow
        // collapsing and expanding the tree view.
        m_pTreeViewObjFactories->showColumn(CModelObjPool::EColumnNodeName);
        m_pTreeViewObjFactories->hideColumn(CModelObjPool::EColumnType);
        m_pTreeViewObjFactories->hideColumn(CModelObjPool::EColumnObjId);
        m_pTreeViewObjFactories->hideColumn(CModelObjPool::EColumnObjState);
        m_pTreeViewObjFactories->hideColumn(CModelObjPool::EColumnObjAddress);

        m_pDockWdgtObjFactories->setWidget(m_pTreeViewObjFactories);

        addDockWidget( Qt::LeftDockWidgetArea, m_pDockWdgtObjFactories );

        if( !QObject::connect(
            /* pObjSender   */ m_pTreeViewObjFactories,
            /* szSignal     */ SIGNAL( expanded(const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewObjFactoriesExpanded(const QModelIndex&) ) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pTreeViewObjFactories->selectionModel(),
            /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewObjFactoriesCurrentChanged(const QModelIndex&, const QModelIndex&) ) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

        if( m_pMenuView != nullptr )
        {
            m_pMenuView->addAction(m_pDockWdgtObjFactories->toggleViewAction());
        }

    } // if( m_uAddObjFactories != EAddObjFactoriesNone )

    // Dock Widget - GraphObjs
    //------------------------

    // Tabs with tree views with differently sorted and ordered graphical objects

    m_pDockWdgtGraphObjs = new QDockWidget("Graphical Objects",this);
    m_pDockWdgtGraphObjs->setObjectName("Graphical Objects");
    m_pDockWdgtGraphObjs->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

    m_pTabWdgtGraphObjs = new QTabWidget();

    m_pDockWdgtGraphObjs->setWidget(m_pTabWdgtGraphObjs);

    addDockWidget( Qt::RightDockWidgetArea, m_pDockWdgtGraphObjs );

    if( m_pMenuView != nullptr )
    {
        m_pMenuView->addAction(m_pDockWdgtGraphObjs->toggleViewAction());
    }

    // Dock Widget - GraphObjs - Tab GraphicsItems
    //--------------------------------------------

    // Tree View with graphics items as in drawing scene's items list

    m_pWdgtGraphicsItems = new QWidget();
    m_pTabWdgtGraphObjs->addTab( m_pWdgtGraphicsItems, "Graphics Items" );

    m_pLytGraphicsItems = new QVBoxLayout();
    m_pWdgtGraphicsItems->setLayout(m_pLytGraphicsItems);

    // <Line> Refresh
    //---------------

    m_pLytGraphicsItemsLineRefresh = new QHBoxLayout();
    m_pLytGraphicsItems->addLayout(m_pLytGraphicsItemsLineRefresh);

    m_pLblGraphicsItemsRefresh = new QLabel("Refresh");
    m_pLytGraphicsItemsLineRefresh->addWidget(m_pLblGraphicsItemsRefresh);

    QIcon   iconRefresh;
    QPixmap pxmRefresh(":/ZS/Draw/Refresh16x16.bmp");

    pxmRefresh.setMask(pxmRefresh.createMaskFromColor(Qt::white));

    iconRefresh.addPixmap(pxmRefresh);

    m_pBtnGraphicsItemsRefresh = new QPushButton();
    m_pLytGraphicsItemsLineRefresh->addWidget(m_pBtnGraphicsItemsRefresh);
    m_pBtnGraphicsItemsRefresh->setIcon(iconRefresh);
    m_pBtnGraphicsItemsRefresh->setMaximumWidth(pxmRefresh.width()+12);

    m_pLytGraphicsItemsLineRefresh->addStretch();

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnGraphicsItemsRefresh,
        /* szSignal     */ SIGNAL( clicked(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTreeViewGraphicsItemsBtnRefreshClicked(bool) ) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <TreeView> Graphics Items
    //--------------------------

    m_pModelGraphicsItems = new CGraphicsItemsModel(m_pDrawingScene);

    m_pTreeViewGraphicsItems = new QTreeView(this);
    m_pLytGraphicsItems->addWidget(m_pTreeViewGraphicsItems);

    m_pTreeViewGraphicsItems->setModel(m_pModelGraphicsItems);
    m_pTreeViewGraphicsItems->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_pTreeViewGraphicsItems->setSelectionMode(QAbstractItemView::SingleSelection);
    //m_pTreeViewGraphicsItems->setAllColumnsShowFocus(true);
    m_pTreeViewGraphicsItems->setDragEnabled(true);
    m_pTreeViewGraphicsItems->setDropIndicatorShown(true);
    // Please note that the first column should not be hidden to allow
    // collapsing and expanding the tree view.

    if( !QObject::connect(
        /* pObjSender   */ m_pTreeViewGraphicsItems,
        /* szSignal     */ SIGNAL( expanded(const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTreeViewGraphicsItemsExpanded(const QModelIndex&) ) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTreeViewGraphicsItems->selectionModel(),
        /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTreeViewGraphicsItemsCurrentChanged(const QModelIndex&, const QModelIndex&) ) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTreeViewGraphicsItems,
        /* szSignal     */ SIGNAL( doubleClicked(const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTreeViewGraphicsItemsDoubleClicked(const QModelIndex&) ) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

} // createDockWidgets

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "closeEvent",
        /* strAddInfo   */ "" );

    if( m_bDrawingChangedSinceLastSave )
    {
        QString                     strMsgText;
        QMessageBox::StandardButton msgBoxBtn;

        strMsgText  = "The drawing has been modified.\n";
        strMsgText += "Do you want to save your changes?";

        msgBoxBtn = QMessageBox::warning(
            /* pWdgtParent */ this,
            /* strTitle    */ QApplication::applicationName(),
            /* strText     */ strMsgText,
            /* buttons     */ QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel );

        if( msgBoxBtn == QMessageBox::Save )
        {
            onActionFileSaveTriggered(true);
            i_pEv->accept();
        }
        else if( msgBoxBtn == QMessageBox::Cancel )
        {
            i_pEv->ignore();
        }
        else
        {
            i_pEv->accept();
        }
    }

    if( i_pEv->isAccepted() )
    {
        CDialog::HideAllInstances();

        if( m_pDlgTest != nullptr && m_pDlgTest->isVisible() )
        {
            m_pDlgTest->hide();
        }
    }

} // closeEvent

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionModeEdit( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCheckedActionModeEdit",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pActModeEdit != nullptr )
    {
        m_pActModeEdit->setChecked(i_bChecked);
    }

} // setCheckedActionModeEdit

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionModeSimulation( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCheckedActionModeSimulation",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pActModeSimulation != nullptr )
    {
        m_pActModeSimulation->setChecked(i_bChecked);
    }

} // setCheckedActionModeSimulation

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionEditSelect( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCheckedActionEditSelect",
        /* strAddInfo   */ strAddTrcInfo );

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
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCheckedActionDrawStandardShapePoint",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pActDrawStandardShapePoint != nullptr )
    {
        m_pActDrawStandardShapePoint->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapePoint

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapeLine( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCheckedActionDrawStandardShapeLine",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pActDrawStandardShapeLine != nullptr )
    {
        m_pActDrawStandardShapeLine->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapeLine

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapeRect( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCheckedActionDrawStandardShapeRect",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pActDrawStandardShapeRect != nullptr )
    {
        m_pActDrawStandardShapeRect->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapeRect

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapeEllipse( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCheckedActionDrawStandardShapeEllipse",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pActDrawStandardShapeEllipse != nullptr )
    {
        m_pActDrawStandardShapeEllipse->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapeEllipse

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapePolyline( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCheckedActionDrawStandardShapePolyline",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pActDrawStandardShapePolyline != nullptr )
    {
        m_pActDrawStandardShapePolyline->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapePolyline

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapePolygon( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCheckedActionDrawStandardShapePolygon",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pActDrawStandardShapePolygon != nullptr )
    {
        m_pActDrawStandardShapePolygon->setChecked(i_bChecked);
    }

} // setCheckedActionDrawStandardShapePolygon

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawStandardShapeText( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCheckedActionDrawStandardShapeText",
        /* strAddInfo   */ strAddTrcInfo );

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
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCheckedActionDrawConnectionPoint",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pActDrawConnectionPoint != nullptr )
    {
        m_pActDrawConnectionPoint->setChecked(i_bChecked);
    }

} // setCheckedActionDrawConnectionPoint

//------------------------------------------------------------------------------
void CMainWindow::setCheckedActionDrawConnectionLine( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked:" + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCheckedActionDrawConnectionLine",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionFileNewTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        m_pDrawingScene->clear();

        setCurrentFile("");
    }

} // onActionFileNewTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionFileOpenTriggered( bool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionFileOpenTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    QString strFileName = QFileDialog::getOpenFileName(
        /* pWdgtParent */ this,
        /* strCaption  */ "Choose a file to open",
        /* strDir      */ "",
        /* strFilter   */ "Drawings (*.xml)" );

    if( strFileName.isEmpty() )
    {
        return;
    }

    if( m_pDrawingScene != nullptr )
    {
        // Clear drawing on opening a new file.
        m_pDrawingScene->clear();

        SErrResultInfo errResultInfo = m_pDrawingScene->load(strFileName);

        if( !errResultInfo.isErrorResult() )
        {
            setCurrentFile(strFileName);
        }
        else
        {
            QString strMsg;

            strMsg  = "Error on reading file \"" + strFileName + "\"";
            strMsg += "\n\nErrorCode:\t" + errResultInfo.getResultStr();
            strMsg += "\n\n" + errResultInfo.getAddErrInfoDscr();

            if( errResultInfo.getSeverity() == EResultSeverityCritical )
            {
                QMessageBox::critical(
                    /* pWdgtParent */ this,
                    /* strTitly    */ windowTitle(),
                    /* strText     */ strMsg,
                    /* buttons     */ QMessageBox::Ok );
            }
            else
            {
                QMessageBox::warning(
                    /* pWdgtParent */ this,
                    /* strTitly    */ windowTitle(),
                    /* strText     */ strMsg,
                    /* buttons     */ QMessageBox::Ok );
            }

        } // if( errResultInfo.isErrorResult() )

    } // if( m_pDrawingScene != nullptr )

} // onActionFileOpenTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionFileSaveTriggered( bool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "FileName:" + m_strCurrentFile;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionFileSaveTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_strCurrentFile.isEmpty() )
    {
        QString strFileName = QFileDialog::getSaveFileName(
            /* pWdgtParent */ this,
            /* strCaption  */ "Choose a filename to save under",
            /* strDir      */ "",
            /* strFilter   */ "Drawings (*.xml)" );

        if( strFileName.isEmpty() )
        {
            return;
        }
        if( !strFileName.toLower().endsWith(".xml") )
        {
            strFileName.append(".xml");
        }

        setCurrentFile(strFileName);
    }

    if( m_pDrawingScene != nullptr )
    {
        m_pDrawingScene->save(m_strCurrentFile);
    }

} // onActionFileSaveTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionFileSaveAsTriggered( bool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionFileSaveAsTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    QString strFileName = QFileDialog::getSaveFileName(
        /* pWdgtParent */ this,
        /* strCaption  */ "Choose a filename to save under",
        /* strDir      */ "",
        /* strFilter   */ "Drawings (*.xml)" );

    if( strFileName.isEmpty() )
    {
        return;
    }
    if( !strFileName.toLower().endsWith(".xml") )
    {
        strFileName.append(".xml");
    }

    setCurrentFile(strFileName);

    if( m_pDrawingScene != nullptr )
    {
        m_pDrawingScene->save(strFileName);
    }

} // onActionFileSaveAsTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionFilePageSetupTriggered( bool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionFilePageSetupTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    CDlgPageSetup* pDlgPageSetup = new CDlgPageSetup(m_pDrawingView);

    pDlgPageSetup->setCurrentWidget(CDlgPageSetup::EWidgetDrawingScene);

    pDlgPageSetup->exec();

} // onActionFilePageSetupTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionFileRecentTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionFileRecentTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    //QAction* pAct = dynamic_cast<QAction*>(sender());

    //if( m_pDrawPaper != nullptr && pAct != nullptr )
    //{
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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionModeEditToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        if( i_bChecked && m_pDrawingScene->getMode() != EMode::Edit )
        {
            m_pDrawingScene->setCurrentDrawingTool(nullptr);
            m_pDrawingScene->setMode(EMode::Edit);
        }
        else if( !i_bChecked && m_pDrawingScene->getMode() != EMode::Simulation )
        {
            m_pDrawingScene->setCurrentDrawingTool(nullptr);
            m_pDrawingScene->setMode(EMode::Simulation);
        }

    } // if( m_pDrawingScene != nullptr )

} // onActionModeEditToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionModeSimulationToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionModeSimulationToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        if( i_bChecked && m_pDrawingScene->getMode() != EMode::Simulation )
        {
            m_pDrawingScene->setCurrentDrawingTool(nullptr);
            m_pDrawingScene->setMode(EMode::Simulation);
        }
        else if( !i_bChecked && m_pDrawingScene->getMode() != EMode::Edit )
        {
            m_pDrawingScene->setCurrentDrawingTool(nullptr);
            m_pDrawingScene->setMode(EMode::Edit);
        }

    } // if( m_pDrawingScene != nullptr )

} // onActionModeSimulationToggled

/*==============================================================================
public slots: // Menu - Edit - Select/RotateFree
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionEditSelectToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionEditSelectToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        if( i_bChecked )
        {
            if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActEditSelect )
            {
                m_pActDrawChecked->setChecked(false);
            }
            m_pActDrawChecked = m_pActEditSelect;

            m_pDrawingScene->setCurrentDrawingTool(nullptr);
            m_pDrawingScene->setMode( EMode::Ignore, EEditToolSelect, EEditModeUndefined, EEditResizeModeUndefined );

        } // if( i_bChecked )

        else // if( !i_bChecked )
        {
            if( m_pActDrawChecked == m_pActEditSelect )
            {
                m_pActDrawChecked = nullptr;
            }
            if( m_pDrawingScene->getEditTool() == EEditToolSelect )
            {
                m_pDrawingScene->setMode( EMode::Ignore, EEditToolUndefined, EEditModeUndefined, EEditResizeModeUndefined );
            }

        } // if( !i_bChecked )

    } // if( m_pDrawingScene != nullptr )

} // onActionEditSelectToggled

/*==============================================================================
public slots: // Menu - Edit - Rotate
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionEditRotateLeftTriggered( bool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionEditRotateLeftTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        double fAngle_deg = 90.0;

        if( m_pEdtEditRotateAngle != nullptr )
        {
            fAngle_deg = m_pEdtEditRotateAngle->value();
        }

        QList<QGraphicsItem*> arpGraphicsItemsSelected = m_pDrawingScene->selectedItems();

        // If graphical objects are selected ...
        if( arpGraphicsItemsSelected.size() > 0 )
        {
            // .. all selected graphical objects will be changed according to the new settings.
            QGraphicsItem* pGraphicsItem;
            CGraphObj*     pGraphObj;
            int            idxGraphObj;

            for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
            {
                pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];
                pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                if( pGraphObj != nullptr )
                {
                    if( pGraphObj->getType() != EGraphObjTypeLabel && pGraphObj->getType() != EGraphObjTypeSelectionPoint )
                    {
                        double fAngleTmp_deg = pGraphObj->getRotationAngleInDegree();
                        pGraphObj->setRotationAngleInDegree(fAngleTmp_deg+fAngle_deg);
                    }
                }
            }

        } // if( arpGraphicsItemsSelected.size() > 0 )

    } // if( m_pDrawingScene != nullptr )

} // onActionEditRotateLeftTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionEditRotateRightTriggered( bool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionEditRotateRightTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        double fAngle_deg = -90.0;

        if( m_pEdtEditRotateAngle != nullptr )
        {
            fAngle_deg = -m_pEdtEditRotateAngle->value();
        }

        QList<QGraphicsItem*> arpGraphicsItemsSelected = m_pDrawingScene->selectedItems();

        // If graphical objects are selected ...
        if( arpGraphicsItemsSelected.size() > 0 )
        {
            // .. all selected graphical objects will be changed according to the new settings.
            QGraphicsItem* pGraphicsItem;
            CGraphObj*     pGraphObj;
            int            idxGraphObj;

            for( idxGraphObj = 0; idxGraphObj < arpGraphicsItemsSelected.size(); idxGraphObj++ )
            {
                pGraphicsItem = arpGraphicsItemsSelected[idxGraphObj];
                pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

                if( pGraphObj != nullptr )
                {
                    if( pGraphObj->getType() != EGraphObjTypeLabel && pGraphObj->getType() != EGraphObjTypeSelectionPoint )
                    {
                        double fAngleTmp_deg = pGraphObj->getRotationAngleInDegree();
                        pGraphObj->setRotationAngleInDegree(fAngleTmp_deg+fAngle_deg);
                    }
                }
            }

        } // if( arpGraphicsItemsSelected.size() > 0 )

    } // if( m_pDrawingScene != nullptr )

} // onActionEditRotateRightTriggered

/*==============================================================================
public slots: // Menu - Edit - Mirror
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionEditMirrorVerticalTriggered( bool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionEditMirrorVerticalTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        //graphicsItem->scale(1,-1);
    }

    QMessageBox::information(
        /* pWdgtParent */ this,
        /* strTitle    */ QApplication::applicationName() + ": Unsupported Feature",
        /* strText     */ "Sorry but mirroring is not yet supported." );

} // onActionEditMirrorVerticalTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionEditMirrorHorizontalTriggered( bool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionEditMirrorHorizontalTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        //graphicsItem->scale(-1,1);
    }

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionEditGroupTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        m_pDrawingScene->groupGraphObjsSelected();
    }

} // onActionEditGroupTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionEditUngroupTriggered( bool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionEditUngroupTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        m_pDrawingScene->ungroupGraphObjsSelected();
    }

} // onActionEditUngroupTriggered

/*==============================================================================
public slots: // Menu - Draw - Settings
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawSettingsLineTriggered( bool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawSettingsLineTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    CDlgFormatGraphObjs* pDlgFormatGraphObjs = nullptr;

    // If no graphical object is selected ...
    if( m_pDrawingScene->selectedItems().size() == 0 )
    {
        // .. set default drawing attribute for following draw commands.
        pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene);
    }

    // If one specific graphical object is selected ...
    else if( m_pDrawingScene->selectedItems().size() == 1 )
    {
        // .. set drawing attributes for this object.
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(m_pDrawingScene->selectedItems()[0]);
        if( pGraphObj != nullptr )
        {
            pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene,pGraphObj);
        }
    }

    // If several graphical objects are selected ...
    else // if( m_pDrawingScene->selectedItems().size() > 1 )
    {
        // .. set drawing attributes for those object.
        pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene);
    }

    if( pDlgFormatGraphObjs != nullptr )
    {
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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawSettingsFillTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    CDlgFormatGraphObjs* pDlgFormatGraphObjs = nullptr;

    // If no graphical object is selected ...
    if( m_pDrawingScene->selectedItems().size() == 0 )
    {
        // .. set default drawing attribute for following draw commands.
        pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene);
    }

    // If one specific graphical object is selected ...
    else if( m_pDrawingScene->selectedItems().size() == 1 )
    {
        // .. set drawing attributes for this object.
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(m_pDrawingScene->selectedItems()[0]);
        if( pGraphObj != nullptr )
        {
            pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene,pGraphObj);
        }
    }

    // If several graphical objects are selected ...
    else // if( m_pDrawingScene->selectedItems().size() > 1 )
    {
        // .. set drawing attributes for those object.
        pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene);
    }

    if( pDlgFormatGraphObjs != nullptr )
    {
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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawSettingsTextTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    CDlgFormatGraphObjs* pDlgFormatGraphObjs = nullptr;

    // If no graphical object is selected ...
    if( m_pDrawingScene->selectedItems().size() == 0 )
    {
        // .. set default drawing attribute for following draw commands.
        pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene);
    }

    // If one specific graphical object is selected ...
    else if( m_pDrawingScene->selectedItems().size() == 1 )
    {
        // .. set drawing attributes for this object.
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(m_pDrawingScene->selectedItems()[0]);
        if( pGraphObj != nullptr )
        {
            pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene,pGraphObj);
        }
    }

    // If several graphical objects are selected ...
    else // if( m_pDrawingScene->selectedItems().size() > 1 )
    {
        // .. set drawing attributes for those object.
        pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene);
    }

    if( pDlgFormatGraphObjs != nullptr )
    {
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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawStandardShapePointToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        if( i_bChecked )
        {
            if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapePoint )
            {
                m_pActDrawChecked->setChecked(false);
            }
            m_pActDrawChecked = m_pActDrawStandardShapePoint;

            m_pDrawingScene->setCurrentDrawingTool(m_pObjFactoryPoint);

        } // if( i_bChecked )

        else // if( !i_bChecked )
        {
            if( m_pActDrawChecked == m_pActDrawStandardShapePoint )
            {
                m_pActDrawChecked = nullptr;
            }
            if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPoint )
            {
                m_pDrawingScene->setCurrentDrawingTool(nullptr);
            }
        } // if( !i_bChecked )

    } // if( m_pDrawingScene != nullptr )

} // onActionDrawStandardShapePointToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapeLineToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawStandardShapeLineToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        if( i_bChecked )
        {
            if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapeLine )
            {
                m_pActDrawChecked->setChecked(false);
            }
            m_pActDrawChecked = m_pActDrawStandardShapeLine;

            m_pDrawingScene->setCurrentDrawingTool(m_pObjFactoryLine);

        } // if( i_bChecked )

        else // if( !i_bChecked )
        {
            if( m_pActDrawChecked == m_pActDrawStandardShapeLine )
            {
                m_pActDrawChecked = nullptr;
            }
            if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryLine )
            {
                m_pDrawingScene->setCurrentDrawingTool(nullptr);
            }
        } // if( !i_bChecked )

    } // if( m_pDrawingScene != nullptr )

} // onActionDrawStandardShapeLineToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapeRectToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawStandardShapeRectToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        if( i_bChecked )
        {
            if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapeRect )
            {
                m_pActDrawChecked->setChecked(false);
            }
            m_pActDrawChecked = m_pActDrawStandardShapeRect;

            m_pDrawingScene->setCurrentDrawingTool(m_pObjFactoryRect);

        } // if( i_bChecked )

        else // if( !i_bChecked )
        {
            if( m_pActDrawChecked == m_pActDrawStandardShapeRect )
            {
                m_pActDrawChecked = nullptr;
            }
            if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryRect )
            {
                m_pDrawingScene->setCurrentDrawingTool(nullptr);
            }
        } // if( !i_bChecked )

    } // if( m_pDrawingScene != nullptr )

} // onActionDrawStandardShapeRectToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapeEllipseToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawStandardShapeEllipseToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        if( i_bChecked )
        {
            if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapeEllipse )
            {
                m_pActDrawChecked->setChecked(false);
            }
            m_pActDrawChecked = m_pActDrawStandardShapeEllipse;

            m_pDrawingScene->setCurrentDrawingTool(m_pObjFactoryEllipse);

        } // if( i_bChecked )

        else // if( !i_bChecked )
        {
            if( m_pActDrawChecked == m_pActDrawStandardShapeEllipse )
            {
                m_pActDrawChecked = nullptr;
            }
            if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryEllipse )
            {
                m_pDrawingScene->setCurrentDrawingTool(nullptr);
            }
        } // if( !i_bChecked )

    } // if( m_pDrawingScene != nullptr )

} // onActionDrawStandardShapeEllipseToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapePolylineToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawStandardShapePolylineToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        if( i_bChecked )
        {
            if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapePolyline )
            {
                m_pActDrawChecked->setChecked(false);
            }
            m_pActDrawChecked = m_pActDrawStandardShapePolyline;

            m_pDrawingScene->setCurrentDrawingTool(m_pObjFactoryPolyline);

        } // if( i_bChecked )

        else // if( !i_bChecked )
        {
            if( m_pActDrawChecked == m_pActDrawStandardShapePolyline )
            {
                m_pActDrawChecked = nullptr;
            }
            if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPolyline )
            {
                m_pDrawingScene->setCurrentDrawingTool(nullptr);
            }
        } // if( !i_bChecked )

    } // if( m_pDrawingScene != nullptr )

} // onActionDrawStandardShapePolylineToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapePolygonToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawStandardShapePolygonToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        if( i_bChecked )
        {
            if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapePolygon )
            {
                m_pActDrawChecked->setChecked(false);
            }
            m_pActDrawChecked = m_pActDrawStandardShapePolygon;

            m_pDrawingScene->setCurrentDrawingTool(m_pObjFactoryPolygon);

        } // if( i_bChecked )

        else // if( !i_bChecked )
        {
            if( m_pActDrawChecked == m_pActDrawStandardShapePolygon )
            {
                m_pActDrawChecked = nullptr;
            }
            if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPolygon )
            {
                m_pDrawingScene->setCurrentDrawingTool(nullptr);
            }
        } // if( !i_bChecked )

    } // if( m_pDrawingScene != nullptr )

} // onActionDrawStandardShapePolygonToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawStandardShapeTextToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawStandardShapeTextToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        if( i_bChecked )
        {
            if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawStandardShapeText )
            {
                m_pActDrawChecked->setChecked(false);
            }
            m_pActDrawChecked = m_pActDrawStandardShapeText;

            m_pDrawingScene->setCurrentDrawingTool(m_pObjFactoryText);

        } // if( i_bChecked )

        else // if( !i_bChecked )
        {
            if( m_pActDrawChecked == m_pActDrawStandardShapeText )
            {
                m_pActDrawChecked = nullptr;
            }
            if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryText )
            {
                m_pDrawingScene->setCurrentDrawingTool(nullptr);
            }
        } // if( !i_bChecked )

    } // if( m_pDrawingScene != nullptr )

} // onActionDrawStandardShapeTextToggled

/*==============================================================================
public slots: // Menu - Draw - Graphics
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawGraphicsImageTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawGraphicsImageTriggered",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        // Please note that the graphics image button is not a checkable button
        // and i_bChecked is always false.
        if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawGraphicsImage )
        {
            m_pActDrawChecked->setChecked(false);
        }
        m_pActDrawChecked = m_pActDrawGraphicsImage;

        QString strFileName = QFileDialog::getOpenFileName(
            /* pWdgtParent */ this,
            /* strCaption  */ "Choose image file",
            /* strDir      */ "",
            /* strFilter   */ "Image Files (*.bmp *.jpg)" );

        if( !strFileName.isEmpty() )
        {
            m_pObjFactoryImage->setFileName(strFileName);
            m_pDrawingScene->setCurrentDrawingTool(m_pObjFactoryImage);
        }

    } // if( m_pDrawingScene != nullptr )

} // onActionDrawGraphicsImageTriggered

/*==============================================================================
public slots: // Menu - Draw - Connections
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawConnectionPointToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawConnectionPointToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        if( i_bChecked )
        {
            if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawConnectionPoint )
            {
                m_pActDrawChecked->setChecked(false);
            }
            m_pActDrawChecked = m_pActDrawConnectionPoint;

            m_pDrawingScene->setCurrentDrawingTool(m_pObjFactoryConnectionPoint);

        } // if( i_bChecked )

        else // if( !i_bChecked )
        {
            if( m_pActDrawChecked == m_pActDrawConnectionPoint )
            {
                m_pActDrawChecked = nullptr;
            }
            if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryConnectionPoint )
            {
                m_pDrawingScene->setCurrentDrawingTool(nullptr);
            }
        } // if( !i_bChecked )

    } // if( m_pDrawingScene != nullptr )

} // onActionDrawConnectionPointToggled

//------------------------------------------------------------------------------
void CMainWindow::onActionDrawConnectionLineToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDrawConnectionLineToggled",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pDrawingScene != nullptr )
    {
        if( i_bChecked )
        {
            if( m_pActDrawChecked != nullptr && m_pActDrawChecked != m_pActDrawConnectionLine )
            {
                m_pActDrawChecked->setChecked(false);
            }
            m_pActDrawChecked = m_pActDrawConnectionLine;

            m_pDrawingScene->setCurrentDrawingTool(m_pObjFactoryConnectionLine);

        } // if( i_bChecked )

        else // if( !i_bChecked )
        {
            if( m_pActDrawChecked == m_pActDrawConnectionLine )
            {
                m_pActDrawChecked = nullptr;
            }
            if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryConnectionLine )
            {
                m_pDrawingScene->setCurrentDrawingTool(nullptr);
            }
        } // if( !i_bChecked )

    } // if( m_pDrawingScene != nullptr )

} // onActionDrawConnectionLineToggled

/*==============================================================================
public slots: // Menu - Trace
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActionTraceErrLogTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionTraceErrLogTriggered",
        /* strAddInfo   */ "" );

    QString strDlgTitle = getMainWindowTitle() + ": Error Log";

    CDlgErrLog* pDlg = dynamic_cast<CDlgErrLog*>(CDlgErrLog::GetInstance(strDlgTitle));

    if( pDlg == nullptr )
    {
        pDlg = CDlgErrLog::CreateInstance("ErrLog", strDlgTitle);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else // if( pReqSeq != nullptr )
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();

    } // if( pDlg != nullptr )

} // onActionTraceErrLogTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionTraceServerTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionTraceServerTriggered",
        /* strAddInfo   */ "" );

    QString strDlgTitle = getMainWindowTitle() + ": Method Trace Server";

    CDlgTrcServer* pDlg = dynamic_cast<CDlgTrcServer*>(CDlgTrcServer::GetInstance(strDlgTitle));

    if( pDlg == nullptr )
    {
        pDlg = CDlgTrcServer::CreateInstance("MthTrcServer", strDlgTitle);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else // if( pReqSeq != nullptr )
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();

    } // if( pDlg != nullptr )

} // onActionTraceServerTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActionDebugTestTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionDebugTestTriggered",
        /* strAddInfo   */ "" );

    if( m_pDlgTest == nullptr )
    {
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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        QRectF rct;
        int    idx;

        strAddTrcInfo = "RectsCount:" + QString::number(i_region.size());

        for( idx = 0; idx < i_region.size(); idx++ )
        {
            rct = i_region[idx];

            if( idx == 0 )
            {
                strAddTrcInfo += " " + QString::number(idx);
            }
            else
            {
                strAddTrcInfo += ", " + QString::number(idx);
            }

            strAddTrcInfo += ":(" + QString::number(rct.left(),'f',1);
            strAddTrcInfo += "," + QString::number(rct.top(),'f',1);
            strAddTrcInfo += "," + QString::number(rct.width(),'f',1);
            strAddTrcInfo += "," + QString::number(rct.height(),'f',1) + ")";
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDrawingSceneChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( !m_bDrawingChangedSinceLastSave )
    {
        if( !m_strCurrentFile.isEmpty() )
        {
            m_bDrawingChangedSinceLastSave = true;
        }
        else if( i_region.size() != 1 )
        {
            m_bDrawingChangedSinceLastSave = true;
        }
        else // if( i_region.size() == 1 )
        {
            QRectF rct = i_region[0];

            if( rct.size() == m_pDrawingScene->sceneRect().size() )
            {
                if( m_pDrawingScene->items().size() > 0 )
                {
                    m_bDrawingChangedSinceLastSave = true;
                }
            }
        }
    }

    updateActions();

} // onDrawingSceneChanged

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneSelectionChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDrawingSceneSelectionChanged",
        /* strAddInfo   */ "" );

    updateActions();

    //QObject::disconnect(
    //    /* pObjSender   */ m_pTreeViewGraphicsItems->selectionModel(),
    //    /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT( onTreeViewGraphicsItemsCurrentChanged(const QModelIndex&, const QModelIndex&) ) );

    //m_pTreeViewGraphicsItems->clearSelection();

    //QList<QGraphicsItem*> arpGraphicItems = m_pDrawingScene->selectedItems();
    //QGraphicsItem*        pGraphicsItem;
    //QModelIndex           modelIdx;
    //int                   idxGraphObj;

    //for( idxGraphObj = 0; idxGraphObj < arpGraphicItems.size(); idxGraphObj++ )
    //{
    //    pGraphicsItem = arpGraphicItems[idxGraphObj];

    //    modelIdx = m_pModelGraphicsItems->getModelIndex(pGraphicsItem);

    //    if( modelIdx.isValid() )
    //    {
    //        m_pTreeViewGraphicsItems->setCurrentIndex(modelIdx);
    //        m_pTreeViewGraphicsItems->scrollTo(modelIdx);
    //    }
    //}

    //if( !QObject::connect(
    //    /* pObjSender   */ m_pTreeViewGraphicsItems->selectionModel(),
    //    /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT( onTreeViewGraphicsItemsCurrentChanged(const QModelIndex&, const QModelIndex&) ) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

} // onDrawingSceneSelectionChanged

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneMousePosChanged( const QPointF& i_ptMousePos )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Pos:" + point2Str(i_ptMousePos);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDrawingSceneMousePosChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pLblStatusBarDrawingSceneMouseCursorPos != nullptr )
    {
        QString strMouseCursorPos;

        strMouseCursorPos += QString("ScenePos:");
        strMouseCursorPos += QString::number(i_ptMousePos.x());
        strMouseCursorPos += QString("/");
        strMouseCursorPos += QString::number(i_ptMousePos.y());
        strMouseCursorPos += QString(" [");
        strMouseCursorPos += QString(Geometry::GraphDevice()->Pixel()->getSymbol());
        strMouseCursorPos += QString("]");

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

        m_pLblStatusBarDrawingSceneMouseCursorPos->setText(strMouseCursorPos);

    } // if( m_pLblStatusBarDrawingSceneMouseCursorPos != nullptr )

} // onDrawingSceneMousePosChanged

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneModeChanged()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        EMode           mode              = m_pDrawingScene->getMode();
        EEditTool       editTool          = m_pDrawingScene->getEditTool();
        EEditMode       editMode          = m_pDrawingScene->getEditMode();
        EEditResizeMode editResizeMode    = m_pDrawingScene->getEditResizeMode();
        int             iObjFactoryType   = m_pDrawingScene->getCurrentDrawingToolGraphObjType();
        CGraphObj*      pGraphObjCreating = m_pDrawingScene->getGraphObjCreating();

        strAddTrcInfo  = "Mode:" + CEnumMode::toString(mode);
        strAddTrcInfo += ", EditTool:" + editTool2Str(editTool);
        strAddTrcInfo += ", EditMode:" + editMode2Str(editMode);
        strAddTrcInfo += ", ResizeMode:" + editResizeMode2Str(editResizeMode);
        strAddTrcInfo += ", ObjFactory:" + graphObjType2Str(iObjFactoryType);

        if( pGraphObjCreating == nullptr )
        {
            strAddTrcInfo += ", GraphObjCreating:nullptr";
        }
        else
        {
            strAddTrcInfo += ", GraphObjCreating:" + pGraphObjCreating->getObjName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDrawingSceneModeChanged",
        /* strAddInfo   */ strAddTrcInfo );

    setWindowTitle();

    updateActions();

    updateStatusBar();

} // onDrawingSceneModeChanged

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneDrawSettingsChanged( const CDrawSettings& i_drawSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDrawingSceneDrawSettingsChanged",
        /* strAddInfo   */ strAddTrcInfo );

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

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneGraphObjCreated( ZS::Draw::CGraphObj* i_pGraphObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObj:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObj:" + i_pGraphObj->getObjName(true);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDrawingSceneGraphObjCreated",
        /* strAddInfo   */ strAddTrcInfo );

} // onDrawingSceneGraphObjCreated

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneGraphObjDestroyed( const QString& i_strObjId )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "GraphObjId:" + i_strObjId;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDrawingSceneGraphObjDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

} // onDrawingSceneGraphObjDestroyed

//------------------------------------------------------------------------------
void CMainWindow::onDrawingSceneGraphObjIdChanged(
    const QString& i_strObjIdOld,
    const QString& i_strObjIdNew )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "GraphObjIdOld:" + i_strObjIdOld;
        strAddTrcInfo += ", GraphObjIdNew:" + i_strObjIdNew;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDrawingSceneGraphObjIdChanged",
        /* strAddInfo   */ strAddTrcInfo );

} // onDrawingSceneGraphObjIdChanged

/*==============================================================================
protected slots: // Drawing View
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onDrawingViewMousePosChanged( const QPointF& i_ptMousePos )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "Pos:" + point2Str(i_ptMousePos);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDrawingViewMousePosChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pLblStatusBarDrawingViewMouseCursorPos != nullptr )
    {
        QString strMouseCursorPos;

        strMouseCursorPos += QString("ViewPos: ");
        strMouseCursorPos += QString::number(i_ptMousePos.x());
        strMouseCursorPos += QString("/");
        strMouseCursorPos += QString::number(i_ptMousePos.y());
        strMouseCursorPos += QString(" [");
        strMouseCursorPos += QString(Geometry::GraphDevice()->Pixel()->getSymbol());
        strMouseCursorPos += QString("]");

        m_pLblStatusBarDrawingViewMouseCursorPos->setText(strMouseCursorPos);

    } // if( m_pLblStatusBarDrawingViewMouseCursorPos != nullptr )

} // onDrawingViewMousePosChanged

/*==============================================================================
protected slots: // Tree View Object Factories (ToolBox)
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onTreeViewObjFactoriesExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTreeViewObjFactoriesExpanded",
        /* strAddInfo   */ strAddTrcInfo );

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
    CObjFactory* pObjFactorySelected = nullptr;

    if( i_modelIdxCurr.isValid() && i_modelIdxCurr.internalPointer() != nullptr )
    {
        CObjPoolTreeEntry* pTreeEntry = static_cast<CObjPoolTreeEntry*>(i_modelIdxCurr.internalPointer());

        if( pTreeEntry != nullptr )
        {
            pObjFactorySelected = static_cast<CObjFactory*>(pTreeEntry->getObj());
        }
    }

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( pObjFactorySelected == nullptr )
        {
            strAddTrcInfo = "ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo = "ObjFactory:" + pObjFactorySelected->getGraphObjTypeAsString();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTreeViewObjFactoriesCurrentChanged",
        /* strAddInfo   */ strAddTrcInfo );

    // If a valid tree node in the object factories model is selected ..
    if( pObjFactorySelected != nullptr )
    {
        // .. objects will be created by drag and drop operations but not by a drawing tool.
        m_pDrawingScene->setCurrentDrawingTool(nullptr);
    }

} // onTreeViewObjFactoriesCurrentChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::selectTreeViewObjFactoryNode( ZS::Draw::CObjFactory* i_pObjFactory )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pObjFactory == nullptr )
        {
            strAddTrcInfo = "ObjFactory:nullptr";
        }
        else
        {
            strAddTrcInfo = "ObjFactory:" + i_pObjFactory->getGraphObjTypeAsString();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "selectTreeViewObjFactoryNode",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pTreeViewObjFactories != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pTreeViewObjFactories->selectionModel(),
            /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewObjFactoriesCurrentChanged(const QModelIndex&, const QModelIndex&) ) );

        if( m_pTreeViewObjFactories != nullptr && m_pModelObjFactories != nullptr )
        {
            if( i_pObjFactory == nullptr )
            {
                m_pTreeViewObjFactories->selectionModel()->clear();
            }
            else
            {
                CObjPoolTreeEntry* pTreeEntry = m_pModelObjFactories->getObjPool()->getTreeEntry(
                    /* strNameSpace */ i_pObjFactory->getGroupName(),             // see CObjFactory::registerObjFactory
                    /* strObjName   */ i_pObjFactory->getGraphObjTypeAsString(),  // see CObjFactory::registerObjFactory
                    /* entryType    */ EObjPoolEntryTypeObject );

                if( pTreeEntry != nullptr )
                {
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

        if( !QObject::connect(
            /* pObjSender   */ m_pTreeViewObjFactories->selectionModel(),
            /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewObjFactoriesCurrentChanged(const QModelIndex&, const QModelIndex&) ) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }

    } // if( m_pTreeViewObjFactories != nullptr )

} // selectTreeViewObjFactoryNode

/*==============================================================================
protected slots: // Tree View Graphics Items
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onTreeViewGraphicsItemsBtnRefreshClicked( bool )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTreeViewGraphicsItemsExpanded",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pModelGraphicsItems != nullptr )
    {
        m_pModelGraphicsItems->update();
    }

} // onTreeViewGraphicsItemsBtnRefreshClicked

//------------------------------------------------------------------------------
void CMainWindow::onTreeViewGraphicsItemsExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTreeViewGraphicsItemsExpanded",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_modelIdx.isValid() )
    {
        m_pTreeViewGraphicsItems->resizeColumnToContents(i_modelIdx.column());
    }

} // onTreeViewGraphicsItemsExpanded

//------------------------------------------------------------------------------
void CMainWindow::onTreeViewGraphicsItemsCurrentChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& /*i_modelIdxPrev*/ )
//------------------------------------------------------------------------------
{
    CGraphObj* pGraphObj = nullptr;

    if( i_modelIdxCurr.isValid() )
    {
        CGraphObjModelEntry* pModelEntry = static_cast<CGraphObjModelEntry*>(i_modelIdxCurr.internalPointer());

        if( pModelEntry != nullptr )
        {
            pGraphObj = pModelEntry->getGraphObj();
        }
    }

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObj:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObj:" + pGraphObj->getObjName(true);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTreeViewGraphicsItemsCurrentChanged",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);

    if( pGraphicsItem != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pDrawingScene,
            /* szSignal     */ SIGNAL(selectionChanged()),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDrawingSceneSelectionChanged()) );

        m_pDrawingScene->clearSelection();

        pGraphicsItem->setSelected(true);

        if( !QObject::connect(
            /* pObjSender   */ m_pDrawingScene,
            /* szSignal     */ SIGNAL(selectionChanged()),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDrawingSceneSelectionChanged()) ) )
        {
            throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
        }
    }

} // onTreeViewGraphicsItemsCurrentChanged

//------------------------------------------------------------------------------
void CMainWindow::onTreeViewGraphicsItemsDoubleClicked( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    CGraphObj* pGraphObj = nullptr;

    if( i_modelIdx.isValid() )
    {
        CGraphObjModelEntry* pModelEntry = static_cast<CGraphObjModelEntry*>(i_modelIdx.internalPointer());

        if( pModelEntry != nullptr )
        {
            pGraphObj = pModelEntry->getGraphObj();
        }
    }

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( pGraphObj == nullptr )
        {
            strAddTrcInfo = "GraphObj:nullptr";
        }
        else
        {
            strAddTrcInfo = "GraphObj:" + pGraphObj->getObjName(true);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTreeViewGraphicsItemsDoubleClicked",
        /* strAddInfo   */ strAddTrcInfo );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);

    if( pGraphicsItem != nullptr )
    {
        m_pDrawingScene->clearSelection();

        pGraphicsItem->setSelected(true);

        CDlgFormatGraphObjs* pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene,pGraphObj);

        pDlgFormatGraphObjs->setCurrentWidget(CDlgFormatGraphObjs::c_strWdgtObjName);

        pDlgFormatGraphObjs->exec();

        delete pDlgFormatGraphObjs;
        pDlgFormatGraphObjs = nullptr;
    }

} // onTreeViewGraphicsItemsDoubleClicked

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setCurrentFile( const QString& i_strFileName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "FileName:" + i_strFileName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCurrentFile",
        /* strAddInfo   */ strAddTrcInfo );

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
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateStatusBar",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pLblStatusBarDrawingSceneEditTool != nullptr )
    {
        QString strEditInfo;

        EEditTool editTool = m_pDrawingScene->getEditTool();

        if( editTool == EEditToolCreateObjects )
        {
            int     iGraphObjType = m_pDrawingScene->getCurrentDrawingToolGraphObjType();
            QString strObjFactory = graphObjType2Str(iGraphObjType);

            strEditInfo = "Create " + strObjFactory;
        }
        else if( editTool != EEditToolUndefined )
        {
            strEditInfo = editTool2Str(editTool);
        }

        m_pLblStatusBarDrawingSceneEditTool->setText(strEditInfo);

    } // if( m_pLblStatusBarDrawingSceneEditTool != nullptr )

    if( m_pLblStatusBarDrawingSceneEditMode != nullptr || m_pLblStatusBarDrawingSceneGraphObjEditInfo != nullptr )
    {
        QString strEditModeInfo;
        QString strGraphObjEditInfo;

        EEditMode       editMode       = m_pDrawingScene->getEditMode();
        EEditResizeMode editResizeMode = m_pDrawingScene->getEditResizeMode();

        QList<QGraphicsItem*> arGraphicsItemSelected = m_pDrawingScene->selectedItems();
        QGraphicsItem*        pGraphicsItemCreating  = m_pDrawingScene->getGraphicsItemCreating();
        QGraphicsItem*        pGraphicsItem = nullptr;
        CGraphObj*            pGraphObj = nullptr;

        if( editMode != EEditModeUndefined )
        {
            if( arGraphicsItemSelected.size() == 1 )
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
                strEditModeInfo = pGraphObj->getObjName();
                strGraphObjEditInfo = pGraphObj->getObjName() + ": " + pGraphObj->getEditInfo();
            }

            if( editMode == EEditModeResize )
            {
                if( editResizeMode >= 0 && editResizeMode < EEditResizeModeCount )
                {
                    if( !strEditModeInfo.isEmpty() )
                    {
                        strEditModeInfo += ".";
                    }
                    strEditModeInfo += editResizeMode2Str(editResizeMode);
                }
                else if( editMode != EEditModeUndefined )
                {
                    if( !strEditModeInfo.isEmpty() )
                    {
                        strEditModeInfo += ".";
                    }
                    strEditModeInfo += editMode2Str(editMode);
                }
            }
            else
            {
                if( !strEditModeInfo.isEmpty() )
                {
                    strEditModeInfo += ".";
                }
                strEditModeInfo += editMode2Str(editMode);
            }

        } // if( m_editMode != EEditModeUndefined )

        if( m_pLblStatusBarDrawingSceneEditMode != nullptr )
        {
            m_pLblStatusBarDrawingSceneEditMode->setText(strEditModeInfo);
        }

        if( m_pLblStatusBarDrawingSceneGraphObjEditInfo != nullptr )
        {
            m_pLblStatusBarDrawingSceneGraphObjEditInfo->setText(strGraphObjEditInfo);
        }


    } // if( m_pLblStatusBarDrawingSceneEditMode != nullptr )

} // updateStatusBar

//------------------------------------------------------------------------------
void CMainWindow::updateActions()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateActions",
        /* strAddInfo   */ strAddTrcInfo );

    EMode mode = EMode::Edit;

    if( m_pDrawingScene != nullptr )
    {
        mode = m_pDrawingScene->getMode();

        QList<QGraphicsItem*> arpGraphicItemsSelected = m_pDrawingScene->selectedItems();
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
        if( m_pActModeSimulation != nullptr )
        {
            m_pActModeSimulation->setChecked( mode == EMode::Simulation );
        }

        // Menu - Edit - Select
        //---------------------

        if( m_pActEditSelect != nullptr )
        {
            if( mode == EMode::Simulation )
            {
                m_pActEditSelect->setEnabled(false);
            }
            else
            {
                if( m_pDrawingScene->items().size() == 0 )
                {
                    m_pActEditSelect->setEnabled(false);
                }
                else // if( m_pDrawingScene->items().size() > 0 )
                {
                    m_pActEditSelect->setEnabled(true);
                }
                if( m_pDrawingScene->getEditTool() != EEditToolSelect )
                {
                    m_pActEditSelect->setChecked(false);
                }
                else // if( m_pDrawingScene->getEditTool() == EEditToolSelect )
                {
                    m_pActEditSelect->setChecked(true);
                }
            }
        }

        // Menu - Edit - Rotate and Mirror
        //--------------------------------

        if( mode == EMode::Simulation )
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
        } // if( mode == EMode::Simulation )

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

        if( mode == EMode::Simulation )
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
            if( mode == EMode::Simulation )
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
            if( mode == EMode::Simulation )
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
            if( mode == EMode::Simulation )
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
            if( mode == EMode::Simulation )
            {
                m_pActDrawStandardShapePoint->setEnabled(false);
                m_pActDrawStandardShapePoint->setChecked(false);
            }
            else
            {
                m_pActDrawStandardShapePoint->setEnabled(true);

                if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPoint )
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
            if( mode == EMode::Simulation )
            {
                m_pActDrawStandardShapeLine->setEnabled(false);
                m_pActDrawStandardShapeLine->setChecked(false);
            }
            else
            {
                m_pActDrawStandardShapeLine->setEnabled(true);

                if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryLine )
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
            if( mode == EMode::Simulation )
            {
                m_pActDrawStandardShapeRect->setEnabled(false);
                m_pActDrawStandardShapeRect->setChecked(false);
            }
            else
            {
                m_pActDrawStandardShapeRect->setEnabled(true);

                if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryRect )
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
            if( mode == EMode::Simulation )
            {
                m_pActDrawStandardShapeEllipse->setEnabled(false);
                m_pActDrawStandardShapeEllipse->setChecked(false);
            }
            else
            {
                m_pActDrawStandardShapeEllipse->setEnabled(true);

                if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryEllipse )
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
            if( mode == EMode::Simulation )
            {
                m_pActDrawStandardShapePolyline->setEnabled(false);
                m_pActDrawStandardShapePolyline->setChecked(false);
            }
            else
            {
                m_pActDrawStandardShapePolyline->setEnabled(true);

                if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPolyline )
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
            if( mode == EMode::Simulation )
            {
                m_pActDrawStandardShapePolygon->setEnabled(false);
                m_pActDrawStandardShapePolygon->setChecked(false);
            }
            else
            {
                m_pActDrawStandardShapePolygon->setEnabled(true);

                if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryPolygon )
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
            if( mode == EMode::Simulation )
            {
                m_pActDrawStandardShapeText->setEnabled(false);
                m_pActDrawStandardShapeText->setChecked(false);
            }
            else
            {
                m_pActDrawStandardShapeText->setEnabled(true);

                if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryText )
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
            if( mode == EMode::Simulation )
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
            if( mode == EMode::Simulation )
            {
                m_pActDrawConnectionPoint->setEnabled(false);
                m_pActDrawConnectionPoint->setChecked(false);
            }
            else
            {
                m_pActDrawConnectionPoint->setEnabled(true);

                if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryConnectionPoint )
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
            if( mode == EMode::Simulation )
            {
                m_pActDrawConnectionLine->setEnabled(false);
                m_pActDrawConnectionLine->setChecked(false);
            }
            else
            {
                m_pActDrawConnectionLine->setEnabled(true);

                if( m_pDrawingScene->getCurrentDrawingTool() == m_pObjFactoryConnectionLine )
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

        //if( m_pActTraceErrLog != nullptr )
        //{
        //    m_pActTraceErrLog;
        //}
        //if( m_pActTraceServer != nullptr )
        //{
        //    m_pActTraceServer;
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
            if( mode == EMode::Simulation )
            {
                m_pDockWdgtObjFactories->hide();
            }
            else
            {
                m_pDockWdgtObjFactories->show();
            }
        }

        if( mode == EMode::Simulation )
        {
            selectTreeViewObjFactoryNode(nullptr);
        }
        else if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
        {
            selectTreeViewObjFactoryNode(nullptr);
        }

        // Dock Widget - Graphics Items
        //-----------------------------

    } // if( m_pDrawingScene != nullptr )

} // updateActions

//------------------------------------------------------------------------------
void CMainWindow::updateActionsFilesRecent()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateActionsFilesRecent",
        /* strAddInfo   */ strAddTrcInfo );

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
