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
#include <QtGui/qbitmap.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qdockwidget.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qprogressbar.h>
#include <QtGui/qstatusbar.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qtoolbar.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qprogressbar.h>
#include <QtWidgets/qstatusbar.h>
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qtoolbar.h>
#endif

#include "MainWindow.h"
#include "App.h"
#include "WidgetCentral.h"

#include "ZSDataGUI/ZSDatabaseConnectionsDlg.h"
#include "ZSDataGUI/ZSDatabaseClientConnectionDlg.h"
#include "ZSDataGUI/ZSDataSetWdgt.h"
#include "ZSData/ZSDatabase.h"
#include "ZSData/ZSDatabaseClient.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSIpcTraceGUI/ZSIpcTrcServerDlg.h"
#include "ZSSysGUI/ZSSysErrLogDlg.h"
#include "ZSSysGUI/ZSSysRequestExecTreeDlg.h"
#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeDlg.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Test;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;
using namespace ZS::Data;
using namespace ZS::Data::GUI;
using namespace ZS::Apps::Test::Data;


/*******************************************************************************
class CMainWindow : public QMainWindow
*******************************************************************************/

/*==============================================================================
private: // class members
==============================================================================*/

CMainWindow* CMainWindow::s_pThis = nullptr; // singleton class

const QString CMainWindow::c_strMenuNameFile       = "&File";
const QString CMainWindow::c_strActionNameFileNew  = c_strMenuNameFile + ":&New ...";
const QString CMainWindow::c_strActionNameFileOpen = c_strMenuNameFile + ":&Open ...";
const QString CMainWindow::c_strActionNameFileSave = c_strMenuNameFile + ":&Save ...";
const QString CMainWindow::c_strActionNameFileQuit = c_strMenuNameFile + ":&Quit";

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMainWindow::CMainWindow(
    const QString&  i_strWindowTitle,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent,i_wflags),
    m_pSettingsFile(nullptr),
    m_pMnuFile(nullptr),
    m_pToolBarFile(nullptr),
    m_pActFileNew(nullptr),
    m_pActFileOpen(nullptr),
    m_pActFileSave(nullptr),
    m_pActFileQuit(nullptr),
    m_pMnuView(nullptr),
    m_pActViewDockWdgtDbDataSet(nullptr),
    m_pActViewDockWdgtDbClt1DataSet(nullptr),
    m_pActViewDockWdgtDbClt2DataSet(nullptr),
    m_pActViewDockWdgtDbClt3DataSet(nullptr),
    m_pMnuConnections(nullptr),
    m_pActConnectionsDbSrv(nullptr),
    m_pActConnectionsDbClt1(nullptr),
    m_pActConnectionsDbClt2(nullptr),
    m_pActConnectionsDbClt3(nullptr),
    m_pMnuDebug(nullptr),
    m_pActDebugTrcServer(nullptr),
    m_pActDebugTrcAdminObjIdxTree(nullptr),
    m_pActDebugErrLog(nullptr),
    m_pActDebugRequestExecTree(nullptr),
    m_pMnuInfo(nullptr),
    m_pActInfoVersion(nullptr),
    m_pActInfoSettingsFile(nullptr),
    m_pStatusBar(nullptr),
    m_pLblReqInProgress(nullptr),
    m_pBarReqInProgress(nullptr),
    m_pLblErrors(nullptr),
    m_pWdgtCentral(nullptr),
    m_pDockWdgtDbDataSet(nullptr),
    m_pWdgtDockDbDataSet(nullptr),
    m_pDockWdgtDbClt1DataSet(nullptr),
    m_pWdgtDockDbClt1DataSet(nullptr),
    m_pDockWdgtDbClt2DataSet(nullptr),
    m_pWdgtDockDbClt2DataSet(nullptr),
    m_pDockWdgtDbClt3DataSet(nullptr),
    m_pWdgtDockDbClt3DataSet(nullptr),
    m_pDlgFile(nullptr),
    //m_pDlgTrcServer(nullptr),
    //m_pDlgTrcAdminObjIdxTree(nullptr),
    //m_pDlgErrLog(nullptr),
    //m_pDlgRequestExecTree(nullptr),
    m_pDb(nullptr),
    m_pDSDb(nullptr),
    m_pDbClt1(nullptr),
    m_pDSDbClt1(nullptr),
    m_pDbClt2(nullptr),
    m_pDSDbClt2(nullptr),
    m_pDbClt3(nullptr),
    m_pDSDbClt3(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("MainWindow");

    setWindowTitle(i_strWindowTitle);

    QPainter painter;
    QFont    fntToolBarIcons("Helvetica",8);

    // <Menu> File
    //======================

    m_pMnuFile = menuBar()->addMenu(tr("File"));

    m_pToolBarFile = addToolBar("File Operations");
    m_pToolBarFile->setObjectName("File Operations");
    m_pToolBarFile->setIconSize( QSize(16,16) );

    // <MenuItem> File::New
    //----------------------

    QIcon iconFileNew;

    QPixmap pxmFileNew16x16(":/ZS/Menu/MenuFileNew16x16.bmp");

    pxmFileNew16x16.setMask(pxmFileNew16x16.createHeuristicMask());

    iconFileNew.addPixmap(pxmFileNew16x16);

    m_pActFileNew = new QAction( iconFileNew, c_strActionNameFileNew.section(":",-1,-1), this );
    m_pActFileNew->setShortcuts(QKeySequence::New);
    m_pActFileNew->setStatusTip(tr("Create a new test step settings file"));

    m_pMnuFile->addAction(m_pActFileNew);

    m_pToolBarFile->addAction(m_pActFileNew);

    if( !QObject::connect(
        /* pObjSender   */ m_pActFileNew,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActFileNewTriggered()) ) )
    {
        throw CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Open
    //----------------------

    QIcon iconFileOpen;

    QPixmap pxmFileOpen16x16(":/ZS/Menu/MenuFileOpen16x16.bmp");

    pxmFileOpen16x16.setMask(pxmFileOpen16x16.createHeuristicMask());

    iconFileOpen.addPixmap(pxmFileOpen16x16);

    m_pActFileOpen = new QAction( iconFileOpen, c_strActionNameFileOpen.section(":",-1,-1), this );
    m_pActFileOpen->setShortcuts(QKeySequence::Open);
    m_pActFileOpen->setStatusTip(tr("Open an existing test step settings file"));

    m_pMnuFile->addAction(m_pActFileOpen);

    m_pToolBarFile->addAction(m_pActFileOpen);

    if( !QObject::connect(
        /* pObjSender   */ m_pActFileOpen,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActFileOpenTriggered()) ) )
    {
        throw CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Save
    //----------------------

    QIcon iconFileSave;

    QPixmap pxmFileSave16x16(":/ZS/Menu/MenuFileSave16x16.bmp");

    pxmFileSave16x16.setMask(pxmFileSave16x16.createHeuristicMask());

    iconFileSave.addPixmap(pxmFileSave16x16);

    m_pActFileSave = new QAction( iconFileSave, c_strActionNameFileSave.section(":",-1,-1), this );
    m_pActFileSave->setShortcuts(QKeySequence::Save);
    m_pActFileSave->setStatusTip(tr("Save test step settings"));

    m_pMnuFile->addAction(m_pActFileSave);

    m_pToolBarFile->addAction(m_pActFileSave);

    if( !QObject::connect(
        /* pObjSender   */ m_pActFileSave,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActFileSaveTriggered()) ) )
    {
        throw CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Separator
    //---------------------------

    m_pMnuFile->addSeparator();

    // <MenuItem> File::Quit
    //----------------------

    m_pActFileQuit = new QAction( c_strActionNameFileQuit.section(":",-1,-1), this );
    m_pActFileQuit->setShortcut(Qt::Key_F4 + Qt::ALT);

    m_pMnuFile->addAction(m_pActFileQuit);

    if( !QObject::connect(
        /* pObjSender   */ m_pActFileQuit,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ qApp,
        /* szSlot       */ SLOT(quit()) ) )
    {
        throw CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> View
    //=============

    m_pMnuView = menuBar()->addMenu(tr("&View"));

    // <DockWidget> Database::DataSet
    //-------------------------------

    m_pDockWdgtDbDataSet = new QDockWidget("Database Data Set",this);
    m_pDockWdgtDbDataSet->setObjectName("DbDataSet");
    m_pDockWdgtDbDataSet->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

    m_pWdgtDockDbDataSet = new CWdgtDataSet("Db");

    m_pDockWdgtDbDataSet->setWidget(m_pWdgtDockDbDataSet);

    addDockWidget( Qt::RightDockWidgetArea, m_pDockWdgtDbDataSet );

    if( m_pMnuView != nullptr )
    {
        m_pMnuView->addAction(m_pDockWdgtDbDataSet->toggleViewAction());
    }

    // <DockWidget> DbClt1::DataSet
    //-----------------------------

    m_pDockWdgtDbClt1DataSet = new QDockWidget("Client 1 Data Set",this);
    m_pDockWdgtDbClt1DataSet->setObjectName("DbClt1DataSet");
    m_pDockWdgtDbClt1DataSet->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

    m_pWdgtDockDbClt1DataSet = new CWdgtDataSet("DbClt1");

    m_pDockWdgtDbClt1DataSet->setWidget(m_pWdgtDockDbClt1DataSet);

    addDockWidget(Qt::RightDockWidgetArea, m_pDockWdgtDbClt1DataSet);

    if( m_pMnuView != nullptr )
    {
        m_pMnuView->addAction(m_pDockWdgtDbClt1DataSet->toggleViewAction());
    }

    // <DockWidget> DbClt2::DataSet
    //--------------------------------

    m_pDockWdgtDbClt2DataSet = new QDockWidget("Client 2 Data Set",this);
    m_pDockWdgtDbClt2DataSet->setObjectName("DbClt2DataSet");
    m_pDockWdgtDbClt2DataSet->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

    m_pWdgtDockDbClt2DataSet = new CWdgtDataSet("DbClt2");

    m_pDockWdgtDbClt2DataSet->setWidget(m_pWdgtDockDbClt2DataSet);

    addDockWidget( Qt::RightDockWidgetArea, m_pDockWdgtDbClt2DataSet );

    if( m_pMnuView != nullptr )
    {
        m_pMnuView->addAction(m_pDockWdgtDbClt2DataSet->toggleViewAction());
    }

    // <DockWidget> DbClt3::DataSet
    //---------------------------------

    m_pDockWdgtDbClt3DataSet = new QDockWidget("Client 3 Data Set",this);
    m_pDockWdgtDbClt3DataSet->setObjectName("DbClt3DataSet");
    m_pDockWdgtDbClt3DataSet->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

    m_pWdgtDockDbClt3DataSet = new CWdgtDataSet("DbClt3");

    m_pDockWdgtDbClt3DataSet->setWidget(m_pWdgtDockDbClt3DataSet);

    addDockWidget( Qt::RightDockWidgetArea, m_pDockWdgtDbClt3DataSet );

    if( m_pMnuView != nullptr )
    {
        m_pMnuView->addAction(m_pDockWdgtDbClt3DataSet->toggleViewAction());
    }

    // <Menu> Connections
    //===================

    m_pMnuConnections = menuBar()->addMenu(tr("&Connections"));

    // <MenuItem> Connections::Database Server
    //----------------------------------------

    m_pActConnectionsDbSrv = new QAction("Database Server",this);
    m_pMnuConnections->addAction(m_pActConnectionsDbSrv);

    if( !connect(
        /* pObjSender   */ m_pActConnectionsDbSrv,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActConnectionsDbSrvTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Connections::Client 1
    //---------------------------------

    m_pActConnectionsDbClt1 = new QAction("Client 1",this);
    m_pMnuConnections->addAction(m_pActConnectionsDbClt1);

    if( !connect(
        /* pObjSender   */ m_pActConnectionsDbClt1,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActConnectionsDbClt1Triggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Connections::Client 2
    //---------------------------------

    m_pActConnectionsDbClt2 = new QAction("Client 2",this);
    m_pMnuConnections->addAction(m_pActConnectionsDbClt2);

    if( !connect(
        /* pObjSender   */ m_pActConnectionsDbClt2,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActConnectionsDbClt2Triggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Connections::Client 3
    //---------------------------------

    m_pActConnectionsDbClt3 = new QAction("Client 3",this);
    m_pMnuConnections->addAction(m_pActConnectionsDbClt3);

    if( !connect(
        /* pObjSender   */ m_pActConnectionsDbClt3,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActConnectionsDbClt3Triggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> Debug
    //=============

    m_pMnuDebug = menuBar()->addMenu(tr("&Debug"));

    // <MenuItem> Debug::TraceServer
    //------------------------------

    m_pActDebugTrcServer = new QAction("&Trace Server",this);
    m_pMnuDebug->addAction(m_pActDebugTrcServer);

    if( !connect(
        /* pObjSender   */ m_pActDebugTrcServer,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActDebugTrcServerTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Debug::TraceAdminObjPool
    //------------------------------------

    m_pActDebugTrcAdminObjIdxTree = new QAction("&Trace Admin Objects",this);
    m_pMnuDebug->addAction(m_pActDebugTrcAdminObjIdxTree);

    if( !connect(
        /* pObjSender   */ m_pActDebugTrcAdminObjIdxTree,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActDebugTrcAdminObjIdxTreeTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Debug::Error Log
    //----------------------------

    QIcon iconDebugErrorLog;

    QPixmap pxmDebugErrorLog16x16(":/ZS/App/Zeus16x16.bmp");

    pxmDebugErrorLog16x16.setMask(pxmDebugErrorLog16x16.createHeuristicMask());

    iconDebugErrorLog.addPixmap(pxmDebugErrorLog16x16);

    m_pActDebugErrLog = new QAction( iconDebugErrorLog, "Error Log", this );
    m_pActDebugErrLog->setToolTip("Open error log dialog");
    m_pActDebugErrLog->setEnabled(true);

    m_pMnuDebug->addAction(m_pActDebugErrLog);

    if( !QObject::connect(
        /* pObjSender   */ m_pActDebugErrLog,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActDebugErrLogTriggered()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <MenuItem> Debug::Request Execution Tree
    //-----------------------------------------

    QIcon iconDebugReqExecTree;

    QPixmap pxmDebugReqExecTree16x16(":/ZS/App/Zeus16x16.bmp");

    pxmDebugReqExecTree16x16.setMask(pxmDebugReqExecTree16x16.createHeuristicMask());

    iconDebugReqExecTree.addPixmap(pxmDebugReqExecTree16x16);

    m_pActDebugRequestExecTree = new QAction( iconDebugReqExecTree, "Request Tree", this );
    m_pActDebugRequestExecTree->setToolTip("Open request execution tree dialog");
    m_pActDebugRequestExecTree->setEnabled(true);

    m_pMnuDebug->addAction(m_pActDebugRequestExecTree);

    if( !QObject::connect(
        /* pObjSender   */ m_pActDebugRequestExecTree,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActDebugRequestExecTreeTriggered()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Menu> Info
    //============

    m_pMnuInfo = menuBar()->addMenu(tr("&Info"));

    // <MenuItem> Info::Version
    //--------------------------

    QString strActionInfoVersion = "Version: " + ZS::System::c_strSysVersionNr;

    #ifdef _DEBUG
    strActionInfoVersion += " (Debug)";
    #endif

    m_pActInfoVersion = new QAction(strActionInfoVersion,this);
    m_pMnuInfo->addAction(m_pActInfoVersion);

    // <MenuItem> Settings::Settings File Info
    //----------------------------------------

    m_pSettingsFile = CApplication::GetInstance()->getSettingsFile();

    if( m_pSettingsFile != nullptr )
    {
        QString strActionSettingsFileInfo = "Settings File: " + m_pSettingsFile->fileName();

        QIcon iconModeEdit;

        iconModeEdit.addPixmap( mode2Pixmap(static_cast<int>(EMode::Edit),24) );

        m_pActInfoSettingsFile = new QAction( iconModeEdit, strActionSettingsFileInfo, this );

        m_pMnuInfo->addAction(m_pActInfoSettingsFile);
    }

    // <StatusBar>
    //======================

    QString strStyleSheet = "QStatusBar { " \
        "background: qlineargradient( " \
        "x1: 0, y1: 0, x2: 0, y2: 1, " \
        "stop: 0 #E1E1E1, stop: 0.4 #DDDDDD, " \
        "stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3); } ";

    m_pStatusBar = new QStatusBar;
    m_pStatusBar->setStyleSheet(strStyleSheet);
    setStatusBar(m_pStatusBar);

    // <Label> RequestInProgress with ProgressBar
    //-------------------------------------------

    m_pBarReqInProgress = new QProgressBar();
    m_pBarReqInProgress->setMaximumHeight(12);
    m_pBarReqInProgress->setMaximumWidth(150);
    m_pBarReqInProgress->setRange(0,100);
    m_pBarReqInProgress->setTextVisible(false);
    m_pBarReqInProgress->installEventFilter(this);
    m_pStatusBar->addPermanentWidget(m_pBarReqInProgress);
    m_pBarReqInProgress->hide();

    m_pLblReqInProgress = new QLabel("Ready");
    m_pLblReqInProgress->installEventFilter(this);
    m_pStatusBar->addPermanentWidget(m_pLblReqInProgress);
    m_pLblReqInProgress->hide();

    // <Label> Errors
    //---------------

    if( CErrLog::GetInstance() != nullptr )
    {
        m_pLblErrors = new QLabel("Errors");
        m_pLblErrors->installEventFilter(this);
        m_pStatusBar->addPermanentWidget(m_pLblErrors);

        updateErrorsStatus();

        if( !QObject::connect(
            /* pObjSender   */ CErrLog::GetInstance(),
            /* szSignal     */ SIGNAL(entryAdded(const ZS::System::SErrResultInfo&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onErrLogEntryAdded(const ZS::System::SErrResultInfo&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ CErrLog::GetInstance(),
            /* szSignal     */ SIGNAL(entryChanged(const ZS::System::SErrResultInfo&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onErrLogEntryChanged(const ZS::System::SErrResultInfo&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ CErrLog::GetInstance(),
            /* szSignal     */ SIGNAL(entryRemoved(const ZS::System::SErrResultInfo&)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onErrLogEntryRemoved(const ZS::System::SErrResultInfo&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( CErrLog::GetInstance() != nullptr )

    // <CentralWidget>
    //======================

    m_pWdgtCentral = new CWidgetCentral();

    setCentralWidget(m_pWdgtCentral);

    // <Geometry>
    //======================

    tabifyDockWidget( m_pDockWdgtDbClt1DataSet, m_pDockWdgtDbClt2DataSet );
    tabifyDockWidget( m_pDockWdgtDbClt1DataSet, m_pDockWdgtDbClt3DataSet );

    QSettings settings;

    restoreGeometry( settings.value("MainWindow/Geometry").toByteArray() );
    restoreState( settings.value("MainWindow/WindowState").toByteArray() );

    restoreDockWidget(m_pDockWdgtDbDataSet);
    restoreDockWidget(m_pDockWdgtDbClt1DataSet);
    restoreDockWidget(m_pDockWdgtDbClt2DataSet);
    restoreDockWidget(m_pDockWdgtDbClt3DataSet);

} // ctor

//------------------------------------------------------------------------------
CMainWindow::~CMainWindow()
//------------------------------------------------------------------------------
{
    // <Geometry>
    //======================

    QSettings settings;

    settings.setValue( "MainWindow/Geometry", saveGeometry() );
    settings.setValue( "MainWindow/WindowState", saveState() );

    // <Clear>
    //======================

    try
    {
        delete m_pDlgFile;
    }
    catch(...)
    {
    }

    CDialog::DestroyAllInstances();

    m_pSettingsFile = nullptr;
    m_pMnuFile = nullptr;
    m_pToolBarFile = nullptr;
    m_pActFileNew = nullptr;
    m_pActFileOpen = nullptr;
    m_pActFileSave = nullptr;
    m_pActFileQuit = nullptr;
    m_pMnuView = nullptr;
    m_pActViewDockWdgtDbDataSet = nullptr;
    m_pActViewDockWdgtDbClt1DataSet = nullptr;
    m_pActViewDockWdgtDbClt2DataSet = nullptr;
    m_pActViewDockWdgtDbClt3DataSet = nullptr;
    m_pMnuConnections = nullptr;
    m_pActConnectionsDbSrv = nullptr;
    m_pActConnectionsDbClt1 = nullptr;
    m_pActConnectionsDbClt3 = nullptr;
    m_pActConnectionsDbClt2 = nullptr;
    m_pMnuDebug = nullptr;
    m_pActDebugTrcServer = nullptr;
    m_pActDebugTrcAdminObjIdxTree = nullptr;
    m_pActDebugErrLog = nullptr;
    m_pActDebugRequestExecTree = nullptr;
    m_pMnuInfo = nullptr;
    m_pActInfoVersion = nullptr;
    m_pActInfoSettingsFile = nullptr;
    m_pStatusBar = nullptr;
    m_pLblReqInProgress = nullptr;
    m_pBarReqInProgress = nullptr;
    m_pLblErrors = nullptr;
    m_pWdgtCentral = nullptr;
    m_pDockWdgtDbDataSet = nullptr;
    m_pWdgtDockDbDataSet = nullptr;
    m_pDockWdgtDbClt1DataSet = nullptr;
    m_pWdgtDockDbClt1DataSet = nullptr;
    m_pDockWdgtDbClt2DataSet = nullptr;
    m_pWdgtDockDbClt2DataSet = nullptr;
    m_pDockWdgtDbClt3DataSet = nullptr;
    m_pWdgtDockDbClt3DataSet = nullptr;
    m_pDlgFile = nullptr;
    m_pDb = nullptr;
    m_pDSDb = nullptr;
    m_pDbClt1 = nullptr;
    m_pDSDbClt1 = nullptr;
    m_pDbClt2 = nullptr;
    m_pDSDbClt2 = nullptr;
    m_pDbClt3 = nullptr;
    m_pDSDbClt3 = nullptr;

    s_pThis = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setDb( CDb* i_pDb )
//------------------------------------------------------------------------------
{
    if( m_pDb != i_pDb )
    {
        m_pDb = i_pDb;

        QString strDlgTitle = getMainWindowTitle() + ": Database Server Connections";

        CDlgDbConnections* pDlg = CDlgDbConnections::GetInstance(m_pDb->objectName());

        if( pDlg != nullptr )
        {
            pDlg->setDb(m_pDb);
        }
    } // if( m_pDb != i_pDb )

} // setDb

//------------------------------------------------------------------------------
void CMainWindow::setDbDataSet( CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    if( m_pDSDb != i_pDS )
    {
        m_pDSDb = i_pDS;

        if( m_pWdgtDockDbDataSet != nullptr )
        {
            m_pWdgtDockDbDataSet->setDataSet(m_pDSDb);
        }
    } // if( m_pDSDb != i_pDS )

} // setDbDataSet

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setDbClt1( CDbClient* i_pDbClt )
//------------------------------------------------------------------------------
{
    if( m_pDbClt1 != i_pDbClt )
    {
        m_pDbClt1 = i_pDbClt;

        QString strDlgTitle = getMainWindowTitle() + ": Database Client 1 Connections";

        CDlgDbClientConnection* pDlg = CDlgDbClientConnection::GetInstance(m_pDbClt1->objectName());

        if( pDlg != nullptr )
        {
            pDlg->setDbClient(m_pDbClt1);
        }
    } // if( m_pDbClt1 != i_pDbClt )

} // setDbClt1

//------------------------------------------------------------------------------
void CMainWindow::setDbClt1DataSet( CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    if( m_pDSDbClt1 != i_pDS )
    {
        m_pDSDbClt1 = i_pDS;

        if( m_pWdgtDockDbClt1DataSet != nullptr )
        {
            m_pWdgtDockDbClt1DataSet->setDataSet(m_pDSDbClt1);
        }
    } // if( m_pDSDbClt1 != i_pDS )

} // setDbClt1DataSet

//------------------------------------------------------------------------------
void CMainWindow::setDbClt2( CDbClient* i_pDbClt )
//------------------------------------------------------------------------------
{
    if( m_pDbClt2 != i_pDbClt )
    {
        m_pDbClt2 = i_pDbClt;

        QString strDlgTitle = getMainWindowTitle() + ": Database Client 2 Connections";

        CDlgDbClientConnection* pDlg = CDlgDbClientConnection::GetInstance(m_pDbClt2->objectName());

        if( pDlg != nullptr )
        {
            pDlg->setDbClient(m_pDbClt2);
        }
    } // if( m_pDbClt2 != i_pDbClt )

} // setDbClt2

//------------------------------------------------------------------------------
void CMainWindow::setDbClt2DataSet( CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    if( m_pDSDbClt2 != i_pDS )
    {
        m_pDSDbClt2 = i_pDS;

        if( m_pWdgtDockDbClt2DataSet != nullptr )
        {
            m_pWdgtDockDbClt2DataSet->setDataSet(m_pDSDbClt2);
        }
    } // if( m_pDSDbClt2 != i_pDS )

} // setDbClt2DataSet

//------------------------------------------------------------------------------
void CMainWindow::setDbClt3( CDbClient* i_pDbClt )
//------------------------------------------------------------------------------
{
    if( m_pDbClt3 != i_pDbClt )
    {
        m_pDbClt3 = i_pDbClt;

        QString strDlgTitle = getMainWindowTitle() + ": Database Client 3 Connections";

        CDlgDbClientConnection* pDlg = CDlgDbClientConnection::GetInstance(m_pDbClt3->objectName());

        if( pDlg != nullptr )
        {
            pDlg->setDbClient(m_pDbClt3);
        }
    } // if( m_pDbClt3 != i_pDbClt )

} // setDbClt3

//------------------------------------------------------------------------------
void CMainWindow::setDbClt3DataSet( CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    if( m_pDSDbClt3 != i_pDS )
    {
        m_pDSDbClt3 = i_pDS;

        if( m_pWdgtDockDbClt3DataSet != nullptr )
        {
            m_pWdgtDockDbClt3DataSet->setDataSet(m_pDSDbClt3);
        }
    } // if( m_pDSDbClt3 != i_pDS )

} // setDbClt3DataSet

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    if( i_pEv->isAccepted() )
    {
        try
        {
            delete m_pDlgFile;
        }
        catch(...)
        {
        }
        m_pDlgFile = nullptr;

        CDialog::HideAllInstances();

    } // if( i_pEv->isAccepted() )

    QMainWindow::closeEvent(i_pEv);

} // closeEvent

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CMainWindow::eventFilter( QObject* i_pObjWatched, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bHandled = false;

    if( i_pObjWatched == m_pLblReqInProgress || i_pObjWatched == m_pBarReqInProgress )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            onActDebugRequestExecTreeTriggered();
            bHandled = true;
        }
    }
    else if( i_pObjWatched == m_pLblErrors )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            onActDebugErrLogTriggered();
            bHandled = true;
        }
    }
    else
    {
        // pass the event on to the parent class
        bHandled = QMainWindow::eventFilter(i_pObjWatched,i_pEv);
    }
    return bHandled;

} // eventFilter

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActFileNewTriggered()
//------------------------------------------------------------------------------
{
    if( CApplication::GetInstance()->getTest() != nullptr )
    {
        CTestStepAdminObjPool* pObjPool = CApplication::GetInstance()->getTest()->getAdminObjIdxTree();

        if( pObjPool != nullptr )
        {
            pObjPool->clear();
        }
    }

} // onActFileNewTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActFileOpenTriggered()
//------------------------------------------------------------------------------
{
    if( m_pDlgFile == nullptr )
    {
        m_pDlgFile = new QFileDialog(
            /* pWdgtParent  */ nullptr,
            /* strCaption   */ windowTitle() + ": File Dialog",
            /* strDirectory */ "",
            /* strFilter    */ "*.*" );
    }

    CTestStepAdminObjPool* pObjPool = nullptr;

    if( CApplication::GetInstance()->getTest() != nullptr )
    {
        pObjPool = CApplication::GetInstance()->getTest()->getAdminObjIdxTree();
    }

    if( pObjPool != nullptr )
    {
        QString strFile = pObjPool->getFileName();
        QString strDir;

        if( !strFile.isEmpty() )
        {
            QFileInfo fileInfo(strFile);
            QDir dir = fileInfo.absoluteDir();
            strDir = dir.absoluteFilePath(strFile);
        }

        m_pDlgFile->setWindowTitle( windowTitle() + ": Open Test Step Config File" );
        m_pDlgFile->setAcceptMode(QFileDialog::AcceptOpen);
        m_pDlgFile->setConfirmOverwrite(true);
        #if QT_VERSION < 0x050000
        m_pDlgFile->setFilter("*.xml");
        #else
        m_pDlgFile->setNameFilter("*.xml");
        #endif
        m_pDlgFile->setDefaultSuffix("*.xml");
        m_pDlgFile->setDirectory(strDir);

        if( !strFile.isEmpty() )
        {
            m_pDlgFile->selectFile(strFile);
        }

        if( m_pDlgFile->exec() )
        {
            QStringList strlstFiles = m_pDlgFile->selectedFiles();

            if( strlstFiles.size() > 0 )
            {
                strFile = strlstFiles[0];

                if( !strFile.isEmpty() )
                {
                    SErrResultInfo errResultInfo = pObjPool->recall(strFile);

                    if( errResultInfo.isErrorResult() )
                    {
                        QString strText = "Reading test step config from file " + strFile + " failed.";
                        strText += "\n\nErrorCode: " + QString::number(errResultInfo.getResult()) + " (" + errResultInfo.getResultStr() + ")";

                        if( !errResultInfo.getAddErrInfoDscr().isEmpty() )
                        {
                            strText += "\n\n" + errResultInfo.getAddErrInfoDscr();
                        }

                        QMessageBox::critical(
                            /* pWdgtParent */ this,
                            /* strTitle    */ windowTitle() + ": Error Message",
                            /* strText     */ strText,
                            /* buttons     */ QMessageBox::Ok );
                    }
                } // if( strFile.isEmpty() )
            } // if( strlstFiles.size() > 0 )
        } // if( m_pDlgFile->exec() )
    } // if( pObjPool != nullptr )

} // onActFileOpenTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActFileSaveTriggered()
//------------------------------------------------------------------------------
{
    if( m_pDlgFile == nullptr )
    {
        m_pDlgFile = new QFileDialog(
            /* pWdgtParent  */ nullptr,
            /* strCaption   */ windowTitle() + ": File Dialog",
            /* strDirectory */ "",
            /* strFilter    */ "*.*" );
    }

    CTestStepAdminObjPool* pObjPool = nullptr;

    if( CApplication::GetInstance()->getTest() != nullptr )
    {
        pObjPool = CApplication::GetInstance()->getTest()->getAdminObjIdxTree();
    }

    if( pObjPool != nullptr )
    {
        QString strFile = pObjPool->getFileName();
        QString strDir;

        if( !strFile.isEmpty() )
        {
            QFileInfo fileInfo(strFile);
            QDir dir = fileInfo.absoluteDir();
            strDir = dir.absoluteFilePath(strFile);
        }

        m_pDlgFile->setWindowTitle( windowTitle() + ": Save Test Step Config File" );
        m_pDlgFile->setAcceptMode(QFileDialog::AcceptSave);
        m_pDlgFile->setConfirmOverwrite(true);
        #if QT_VERSION < 0x050000
        m_pDlgFile->setFilter("*.xml");
        #else
        m_pDlgFile->setNameFilter("*.xml");
        #endif
        m_pDlgFile->setDefaultSuffix("*.xml");
        m_pDlgFile->setDirectory(strDir);

        if( !strFile.isEmpty() )
        {
            m_pDlgFile->selectFile(strFile);
        }

        if( m_pDlgFile->exec() )
        {
            QStringList strlstFiles = m_pDlgFile->selectedFiles();
            QString     strFile;

            if( strlstFiles.size() > 0 )
            {
                strFile = strlstFiles[0];

                if( !strFile.isEmpty() )
                {
                    SErrResultInfo errResultInfo = pObjPool->save(strFile);

                    if( errResultInfo.isErrorResult() )
                    {
                        QString strText = "Saving test step config in file " + strFile + " failed.";
                        strText += "\n\nErrorCode: " + QString::number(errResultInfo.getResult()) + " (" + errResultInfo.getResultStr() + ")";

                        if( !errResultInfo.getAddErrInfoDscr().isEmpty() )
                        {
                            strText += "\n\n" + errResultInfo.getAddErrInfoDscr();
                        }

                        QMessageBox::critical(
                            /* pWdgtParent */ this,
                            /* strTitle    */ windowTitle() + ": Error Message",
                            /* strText     */ strText,
                            /* buttons     */ QMessageBox::Ok );
                    }
                } // if( strFile.isEmpty() )
            } // if( strlstFiles.size() > 0 )
        } // if( m_pDlgFile->exec() )
    } // if( pObjPool != nullptr )

} // onActFileSaveTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActConnectionsDbSrvTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Database Server Connections";

    CDlgDbConnections* pDlg = CDlgDbConnections::GetInstance(m_pDb->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgDbConnections::CreateInstance(
            /* strObjName  */ "DbSrvConnections",
            /* strDlgTitle */ strDlgTitle,
            /* pWdgtParent */ nullptr );
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->setDb(m_pDb);
        pDlg->adjustSize();
        pDlg->show();
    }
    else // if( pDlg != nullptr )
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }

} // onActConnectionsDbSrvTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActConnectionsDbClt1Triggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Database Client 1 Connections";

    CDlgDbClientConnection* pDlg = CDlgDbClientConnection::GetInstance(m_pDbClt1->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgDbClientConnection::CreateInstance(
            /* strObjName  */ "DbClt1Connections",
            /* strDlgTitle */ strDlgTitle,
            /* pWdgtParent */ nullptr );
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->setDbClient(m_pDbClt1);
        pDlg->adjustSize();
        pDlg->show();
    }
    else // if( pDlg != nullptr )
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }

} // onActConnectionsDbClt1Triggered

//------------------------------------------------------------------------------
void CMainWindow::onActConnectionsDbClt2Triggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Database Client 2 Connections";

    CDlgDbClientConnection* pDlg = CDlgDbClientConnection::GetInstance(m_pDbClt2->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgDbClientConnection::CreateInstance(
            /* strObjName  */ "DbClt2Connections",
            /* strDlgTitle */ strDlgTitle,
            /* pWdgtParent */ nullptr );
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->setDbClient(m_pDbClt2);
        pDlg->adjustSize();
        pDlg->show();
    }
    else // if( pDlg != nullptr )
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }

} // onActConnectionsDbClt2Triggered

//------------------------------------------------------------------------------
void CMainWindow::onActConnectionsDbClt3Triggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Database Client 3 Connections";

    CDlgDbClientConnection* pDlg = CDlgDbClientConnection::GetInstance(m_pDbClt3->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgDbClientConnection::CreateInstance(
            /* strObjName  */ "DbClt3Connections",
            /* strDlgTitle */ strDlgTitle,
            /* pWdgtParent */ nullptr );
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->setDbClient(m_pDbClt3);
        pDlg->adjustSize();
        pDlg->show();
    }
    else // if( pDlg != nullptr )
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }

} // onActConnectionsDbClt3Triggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActDebugTrcServerTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Method Trace Server";

    CDlgTrcServer* pDlg = dynamic_cast<CDlgTrcServer*>(CDlgTrcServer::GetInstance(strDlgTitle));

    if( pDlg == nullptr )
    {
        pDlg = CDlgTrcServer::CreateInstance(
            /* strObjName  */ "MethodTraceServer",
            /* strDlgTitle */ strDlgTitle,
            /* pWdgtParent */ nullptr );
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else // if( pDlg != nullptr )
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }

} // onActDebugTrcServerTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugTrcAdminObjIdxTreeTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Method Trace Admin Objects";

    CDlgIdxTreeTrcAdminObjs* pDlg = CDlgIdxTreeTrcAdminObjs::GetInstance(CTrcServer::GetTraceAdminObjIdxTree()->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgIdxTreeTrcAdminObjs::CreateInstance(
            /* pTrcAdmObjPool */ CTrcServer::GetTraceAdminObjIdxTree(),
            /* strDlgTitle    */ strDlgTitle );
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

} // onActDebugTrcAdminObjIdxTreeTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugErrLogTriggered()
//------------------------------------------------------------------------------
{
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

} // onActDebugErrLogTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugRequestExecTreeTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Requests Execution Tree";

    CDlgRequestExecTree* pDlg = dynamic_cast<CDlgRequestExecTree*>(CDlgRequestExecTree::GetInstance(strDlgTitle));

    if( pDlg == nullptr )
    {
        pDlg = CDlgRequestExecTree::CreateInstance("ReqExecTree", strDlgTitle);
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

} // onActDebugRequestExecTreeTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryAdded( const ZS::System::SErrResultInfo& )
//------------------------------------------------------------------------------
{
    updateErrorsStatus();
}

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryChanged( const ZS::System::SErrResultInfo& )
//------------------------------------------------------------------------------
{
    updateErrorsStatus();
}

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryRemoved( const ZS::System::SErrResultInfo& )
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

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onAppRequestStarted( ZS::System::CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAppRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    onAppRequestChanged( i_pReq->getDescriptor() );

} // onAppRequestStarted

//------------------------------------------------------------------------------
void CMainWindow::onAppRequestChanged( const ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strReqInProgress;
    int     iProgress_perCent = i_reqDscr.m_iProgress_perCent;

    if( i_reqDscr.m_strCreatorObjName.isEmpty() )
    {
        strReqInProgress = i_reqDscr.m_strRequest;
    }
    else
    {
        strReqInProgress = i_reqDscr.m_strCreatorObjName + "." + i_reqDscr.m_strRequest;
    }

    updateReqInProgressStatus( strReqInProgress, iProgress_perCent );

} // onAppRequestChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::updateReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent )
//------------------------------------------------------------------------------
{
    if( m_pLblReqInProgress != nullptr )
    {
        m_pLblReqInProgress->setText(i_strRequest);
    }

    if( m_pBarReqInProgress != nullptr )
    {
        if( i_iProgress_perCent < 100 )
        {
            if( !m_pBarReqInProgress->isVisible() )
            {
                m_pBarReqInProgress->show();
            }
        }
        else
        {
            if( m_pBarReqInProgress->isVisible() )
            {
                m_pBarReqInProgress->hide();
            }
        }
        m_pBarReqInProgress->setValue(i_iProgress_perCent);
    }

} // updateReqInProgressStatus
