/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer, Germany
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
#include <QtGui/qevent.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qfiledialog.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qprogressbar.h>
#include <QtGui/qstatusbar.h>
#else
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qprogressbar.h>
#include <QtWidgets/qstatusbar.h>
#endif

#include "MainWindow.h"
#include "App.h"
#include "WidgetCentral.h"

#include "ZSIpcTraceGUI/ZSIpcTrcClientDlg.h"
#include "ZSIpcTraceGUI/ZSIpcTrcMthWdgt.h"
#include "ZSIpcTraceGUI/ZSIpcTrcMthListWdgtSettingsDlg.h"
#include "ZSIpcTrace/ZSIpcTrcClient.h"
#include "ZSSysGUI/ZSSysFindTextDlg.h"
#include "ZSSysGUI/ZSSysErrLogDlg.h"
#include "ZSSysGUI/ZSSysRequestExecTreeDlg.h"
#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeDlg.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;
using namespace ZS::Apps::Products::MethodTraceClient;


/*******************************************************************************
class CMainWindow : public QMainWindow
*******************************************************************************/

/*==============================================================================
private: // class members
==============================================================================*/

CMainWindow* CMainWindow::s_pThis = nullptr; // singleton class

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
    const QString&  i_strWindowTitle,
    CIpcTrcClient*  i_pTrcClient,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent,i_wflags),
    m_pSettingsFile(nullptr),
    m_pTrcClient(i_pTrcClient),
    m_pMnuFile(nullptr),
    m_pActFileRecallAdminObjs(nullptr),
    m_pActFileSaveAdminObjs(nullptr),
    m_pActFileReadTrcMthFile(nullptr),
    m_pActFileWriteTrcMthFile(nullptr),
    m_pActFileQuit(nullptr),
    m_pMnuEdit(nullptr),
    m_pActEditFind(nullptr),
    m_pMnuSettings(nullptr),
    m_pActSettingsWdgtTrcMthList(nullptr),
    m_pActSettingsTrcClient(nullptr),
    m_pActSettingsTrcAdminObjIdxTree(nullptr),
    m_pMnuDebug(nullptr),
    m_pActDebugErrLog(nullptr),
    m_pActDebugRequestExecTree(nullptr),
    m_pMnuInfo(nullptr),
    m_pActInfoVersion(nullptr),
    m_pActInfoSettingsFile(nullptr),
    m_pStatusBar(nullptr),
    m_pLblReqInProgress(nullptr),
    m_pBarReqInProgress(nullptr),
    m_pLblErrors(nullptr),
    m_pWdgtTrcClientCnctStatus(nullptr),
    m_pLblTrcClientCnctStatusIcon(nullptr),
    m_pWdgtCentral(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("MainWindow");

    setWindowTitle(i_strWindowTitle);

    if( m_pTrcClient == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    QObject::connect(
        m_pTrcClient, &QObject::destroyed,
        this, &CMainWindow::onTrcClientDestroyed);
    QObject::connect(
        m_pTrcClient, &CIpcTrcClient::connected,
        this, &CMainWindow::onTrcClientConnected);
    QObject::connect(
        m_pTrcClient, &CIpcTrcClient::disconnected,
        this, &CMainWindow::onTrcClientDisconnected);
    QObject::connect(
        m_pTrcClient, &CIpcTrcClient::stateChanged,
        this, &CMainWindow::onTrcClientStateChanged);

    // <Menu> File
    //=============

    m_pMnuFile = menuBar()->addMenu(tr("&File"));

    // <MenuItem> File::Recall Trace Admin Objects
    //---------------------------------------------

    m_pActFileRecallAdminObjs = new QAction("Recall Trace Admin Objects", this);
    m_pMnuFile->addAction(m_pActFileRecallAdminObjs);

    QObject::connect(
        m_pActFileRecallAdminObjs, &QAction::triggered,
        this, &CMainWindow::onActFileRecallAdminObjsTriggered);

    // <MenuItem> File::Save Trace Admin Objects
    //------------------------------------------

    m_pActFileSaveAdminObjs = new QAction("Save Trace Admin Objects", this);
    m_pMnuFile->addAction(m_pActFileSaveAdminObjs);

    QObject::connect(
        m_pActFileSaveAdminObjs, &QAction::triggered,
        this, &CMainWindow::onActFileSaveAdminObjsTriggered);

    // <MenuItem> File::Read Trace Method File
    //----------------------------------------

    m_pMnuFile->addSeparator();

    m_pActFileReadTrcMthFile = new QAction("Read Trace Method File", this);
    m_pMnuFile->addAction(m_pActFileReadTrcMthFile);

    QObject::connect(
        m_pActFileReadTrcMthFile, &QAction::triggered,
        this, &CMainWindow::onActFileReadTrcMthFileTriggered);

    // <MenuItem> File::Write Trace Method File
    //-----------------------------------------

    m_pActFileWriteTrcMthFile = new QAction("Write Trace Method File", this);
    m_pMnuFile->addAction(m_pActFileWriteTrcMthFile);

    QObject::connect(
        m_pActFileWriteTrcMthFile, &QAction::triggered,
        this, &CMainWindow::onActFileWriteTrcMthFileTriggered);

    // <MenuItem> File::Quit
    //----------------------

    m_pMnuFile->addSeparator();

    m_pActFileQuit = new QAction("&Quit", this);
    m_pMnuFile->addAction(m_pActFileQuit);

    QObject::connect(
        m_pActFileQuit, &QAction::triggered,
        qApp, &CApplication::quit);

    // <Menu> Edit
    //============

    m_pMnuEdit = menuBar()->addMenu(tr("&Edit"));

    // <MenuItem> Find
    //----------------

    m_pActEditFind = new QAction("&Find",this);
    m_pActEditFind->setShortcuts(QKeySequence::Find);
    m_pMnuEdit->addAction(m_pActEditFind);

    QObject::connect(
        m_pActEditFind, &QAction::triggered,
        this, &CMainWindow::onActEditFindTriggered);

    // <Menu> Settings
    //================

    m_pMnuSettings = menuBar()->addMenu(tr("&Settings"));

    // <MenuItem> Settings::WdgtTrcMthList
    //------------------------------------

    m_pActSettingsWdgtTrcMthList = new QAction("&Method Trace Widget",this);
    m_pMnuSettings->addAction(m_pActSettingsWdgtTrcMthList);

    QObject::connect(
        m_pActSettingsWdgtTrcMthList, &QAction::triggered,
        this, &CMainWindow::onActSettingsWdgtTrcMthListTriggered);

    // <MenuItem> Settings::TrcClient
    //----------------------------------

    m_pActSettingsTrcClient = new QAction("&Trace Client",this);
    m_pMnuSettings->addAction(m_pActSettingsTrcClient);

    QObject::connect(
        m_pActSettingsTrcClient, &QAction::triggered,
        this, &CMainWindow::onActSettingsTrcClientTriggered);

    // <MenuItem> Settings::TraceAdminIdxTree
    //---------------------------------------

    m_pActSettingsTrcAdminObjIdxTree = new QAction("&Trace Admin Objects",this);
    m_pMnuSettings->addAction(m_pActSettingsTrcAdminObjIdxTree);

    QObject::connect(
        m_pActSettingsTrcAdminObjIdxTree, &QAction::triggered,
        this, &CMainWindow::onActSettingsTrcAdminObjIdxTreeTriggered);

    // <Menu> Debug
    //=============

    m_pMnuDebug = menuBar()->addMenu(tr("&Debug"));

    // <MenuItem> Debug::Error Log
    //----------------------------

    QIcon iconErrorLog;
    QPixmap pxmErrorLog(":/ZS/App/Zeus32x32.png");
    iconErrorLog.addPixmap(pxmErrorLog);

    m_pActDebugErrLog = new QAction( iconErrorLog, "Error Log", this );
    m_pActDebugErrLog->setToolTip("Open error log dialog");
    m_pActDebugErrLog->setEnabled(true);

    m_pMnuDebug->addAction(m_pActDebugErrLog);

    QObject::connect(
        m_pActDebugErrLog, &QAction::triggered,
        this, &CMainWindow::onActDebugErrLogTriggered);

    // <MenuItem> Debug::Request Execution Tree
    //-----------------------------------------

    QIcon iconReqExecTree;
    QPixmap pxmReqExecTree(":/ZS/App/Zeus32x32.png");
    iconReqExecTree.addPixmap(pxmReqExecTree);

    m_pActDebugRequestExecTree = new QAction( iconReqExecTree, "Request Tree", this );
    m_pActDebugRequestExecTree->setToolTip("Open request execution tree dialog");
    m_pActDebugRequestExecTree->setEnabled(true);

    m_pMnuDebug->addAction(m_pActDebugRequestExecTree);

    QObject::connect(
        m_pActDebugRequestExecTree, &QAction::triggered,
        this, &CMainWindow::onActDebugRequestExecTreeTriggered);

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

    // <MenuItem> Info::Settings File
    //-------------------------------

    m_pSettingsFile = CApplication::GetInstance()->getSettingsFile();

    if( m_pSettingsFile != nullptr )
    {
        QString strActionInfoSettingsFile = "Settings File: " + m_pSettingsFile->fileName();

        m_pActInfoSettingsFile = new QAction(strActionInfoSettingsFile,this);
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
        m_pLblErrors->hide();

        updateErrorsStatus();

        QObject::connect(
            CErrLog::GetInstance(), &CErrLog::entryAdded,
            this, &CMainWindow::onErrLogEntryAdded);
        QObject::connect(
            CErrLog::GetInstance(), &CErrLog::entryChanged,
            this, &CMainWindow::onErrLogEntryChanged);
        QObject::connect(
            CErrLog::GetInstance(), &CErrLog::entryRemoved,
            this, &CMainWindow::onErrLogEntryRemoved);
    }

    // <Label> Trace Client Status
    //----------------------------

    m_pWdgtTrcClientCnctStatus = new QWidget(this);

    new QHBoxLayout(m_pWdgtTrcClientCnctStatus);

    m_pWdgtTrcClientCnctStatus->setContentsMargins(0,0,0,0);
    m_pWdgtTrcClientCnctStatus->layout()->setContentsMargins(1,1,1,1);
    m_pWdgtTrcClientCnctStatus->installEventFilter(this);

    m_pLblTrcClientCnctStatusIcon = new QLabel("");
    m_pLblTrcClientCnctStatusIcon->setScaledContents(true);
    m_pLblTrcClientCnctStatusIcon->setMaximumSize( QSize(16,16) );
    m_pWdgtTrcClientCnctStatus->layout()->addWidget(m_pLblTrcClientCnctStatusIcon);

    statusBar()->addPermanentWidget(m_pWdgtTrcClientCnctStatus);

    onTrcClientStateChanged( m_pTrcClient, m_pTrcClient->getState() );

    // Central Widget
    //===============

    m_pWdgtCentral = new CWidgetCentral(i_pTrcClient);

    setCentralWidget(m_pWdgtCentral);

    QObject::connect(
        m_pWdgtCentral, &CWidgetCentral::progressBarConnectDblClicked,
        this, &CMainWindow::onWidgetCentralProgressBarConnectDblClicked);

    // Restore geometry of widget
    //===========================

    QSettings settings;

    restoreGeometry( settings.value("MainWindow/Geometry").toByteArray() );

} // ctor

//------------------------------------------------------------------------------
CMainWindow::~CMainWindow()
//------------------------------------------------------------------------------
{
    QSettings settings;

    settings.setValue( "MainWindow/Geometry",saveGeometry());

    CDialog::DestroyAllInstances();

    m_pSettingsFile = nullptr;
    m_pTrcClient = nullptr;
    m_pMnuFile = nullptr;
    m_pActFileRecallAdminObjs = nullptr;
    m_pActFileSaveAdminObjs = nullptr;
    m_pActFileReadTrcMthFile = nullptr;
    m_pActFileWriteTrcMthFile = nullptr;
    m_pActFileQuit = nullptr;
    m_pMnuEdit = nullptr;
    m_pActEditFind = nullptr;
    m_pMnuSettings = nullptr;
    m_pActSettingsWdgtTrcMthList = nullptr;
    m_pActSettingsTrcClient = nullptr;
    m_pActSettingsTrcAdminObjIdxTree = nullptr;
    m_pMnuDebug = nullptr;
    m_pActDebugErrLog = nullptr;
    m_pActDebugRequestExecTree = nullptr;
    m_pMnuInfo = nullptr;
    m_pActInfoVersion = nullptr;
    m_pActInfoSettingsFile = nullptr;
    m_pStatusBar = nullptr;
    m_pLblReqInProgress = nullptr;
    m_pBarReqInProgress = nullptr;
    m_pLblErrors = nullptr;
    m_pWdgtTrcClientCnctStatus = nullptr;
    m_pLblTrcClientCnctStatusIcon = nullptr;
    m_pWdgtCentral = nullptr;

    s_pThis = nullptr;

} // dtor

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    CDialog::HideAllInstances();

    QMainWindow::closeEvent(i_pEv);

} // closeEvent

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
    else if( i_pObjWatched == m_pWdgtTrcClientCnctStatus )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            onActSettingsTrcClientTriggered();
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
void CMainWindow::onActFileRecallAdminObjsTriggered()
//------------------------------------------------------------------------------
{
    QSettings settings;
    QString   strAbsFilePath;

    settings.beginGroup("FilePaths");
    strAbsFilePath = settings.value("AdminObjs", getAppConfigDir()).toString();
    settings.endGroup();

    strAbsFilePath = QFileDialog::getOpenFileName(
        /* parent             */ this,
        /* strCaption         */ windowTitle() + ": Recall trace admin objects",
        /* strDir             */ strAbsFilePath,
        /* filter             */ "Trace Admin Objects (*.xml)",
        /* pstrSelectedFilter */ nullptr,
        /* options            */ QFileDialog::Options() );

    if( !strAbsFilePath.isEmpty() )
    {
        settings.beginGroup("FilePaths");
        settings.setValue("AdminObjs", strAbsFilePath);
        settings.endGroup();

        m_pTrcClient->getTraceAdminObjIdxTree()->recall(strAbsFilePath);
    }
} // onActFileRecallAdminObjsTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActFileSaveAdminObjsTriggered()
//------------------------------------------------------------------------------
{
    QSettings settings;
    QString   strAbsFilePath;

    settings.beginGroup("FilePaths");
    strAbsFilePath = settings.value("AdminObjs", getAppConfigDir()).toString();
    settings.endGroup();

    strAbsFilePath = QFileDialog::getSaveFileName(
        /* parent             */ this,
        /* strCaption         */ windowTitle() + ": Recall trace admin objects",
        /* strDir             */ strAbsFilePath,
        /* filter             */ "Trace Admin Objects (*.xml)",
        /* pstrSelectedFilter */ nullptr,
        /* options            */ QFileDialog::Options() );

    if( !strAbsFilePath.isEmpty() )
    {
        settings.beginGroup("FilePaths");
        settings.setValue("AdminObjs", strAbsFilePath);
        settings.endGroup();

        m_pTrcClient->getTraceAdminObjIdxTree()->save(strAbsFilePath);
    }
} // onActFileSaveAdminObjsTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActFileReadTrcMthFileTriggered()
//------------------------------------------------------------------------------
{
    QSettings settings;
    QString   strAbsFilePath;

    settings.beginGroup("FilePaths");
    strAbsFilePath = settings.value("TrcMthFile", getAppLogDir()).toString();
    settings.endGroup();

    strAbsFilePath = QFileDialog::getOpenFileName(
        /* parent             */ this,
        /* strCaption         */ windowTitle() + ": Read trace method log file",
        /* strDir             */ strAbsFilePath,
        /* filter             */ "Trace Method Log Files (*.log)",
        /* pstrSelectedFilter */ nullptr,
        /* options            */ QFileDialog::Options() );

    if( !strAbsFilePath.isEmpty() )
    {
        settings.beginGroup("FilePaths");
        settings.setValue("TrcMthFile", strAbsFilePath);
        settings.endGroup();

        m_pWdgtCentral->getTraceMethodListWidget()->readTraceMethodFile(strAbsFilePath);
    }
} // onActFileReadTrcMthFileTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActFileWriteTrcMthFileTriggered()
//------------------------------------------------------------------------------
{
    QSettings settings;
    QString   strAbsFilePath;

    settings.beginGroup("FilePaths");
    strAbsFilePath = settings.value("TrcMthFile", getAppLogDir()).toString();
    settings.endGroup();

    strAbsFilePath = QFileDialog::getSaveFileName(
        /* parent             */ this,
        /* strCaption         */ windowTitle() + ": Write trace method log file",
        /* strDir             */ strAbsFilePath,
        /* filter             */ "Trace Method Log Files (*.log)",
        /* pstrSelectedFilter */ nullptr,
        /* options            */ QFileDialog::Options() );

    if( !strAbsFilePath.isEmpty() )
    {
        settings.beginGroup("FilePaths");
        settings.setValue("TrcMthFile", strAbsFilePath);
        settings.endGroup();

        m_pWdgtCentral->getTraceMethodListWidget()->writeTraceMethodFile(strAbsFilePath);
    }
} // onActFileWriteTrcMthFileTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActEditFindTriggered()
//------------------------------------------------------------------------------
{
    m_pWdgtCentral->findText();
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActSettingsWdgtTrcMthListTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Method Trace Widget Settings";

    CDlgWdgtTrcMthListSettings* pDlg = CDlgWdgtTrcMthListSettings::GetInstance(
        m_pWdgtCentral->getTraceMethodListWidget()->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgWdgtTrcMthListSettings::CreateInstance(
            /* strDlgTitle */ strDlgTitle,
            /* strObjName  */ m_pWdgtCentral->getTraceMethodListWidget()->objectName(),
            /* pWdgtParent */ nullptr );
        pDlg->setTraceMethodListWidget(m_pWdgtCentral->getTraceMethodListWidget());
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }
} // onActSettingsWdgtTrcMthListTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActSettingsTrcClientTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Trace Client";

    CDlgTrcClient* pDlg = CDlgTrcClient::GetInstance(CApplication::GetInstance()->getTrcClient()->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgTrcClient::CreateInstance(
            /* strDlgTitle */ strDlgTitle,
            /* strObjName  */ CApplication::GetInstance()->getTrcClient()->objectName(),
            /* pWdgtParent */ nullptr );
        pDlg->setClient(CApplication::GetInstance()->getTrcClient());
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }
} // onActSettingsTrcClientTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActSettingsTrcAdminObjIdxTreeTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Trace Admin Objects";

    CDlgIdxTreeTrcAdminObjs* pDlg = CDlgIdxTreeTrcAdminObjs::GetInstance(m_pTrcClient->getTraceAdminObjIdxTree()->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgIdxTreeTrcAdminObjs::CreateInstance(
            /* strDlgTitle    */ strDlgTitle,
            /* pTrcAdmIdxTree */ m_pTrcClient->getTraceAdminObjIdxTree() );
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }
} // onActSettingsTrcAdminObjIdxTreeTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActDebugErrLogTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = QCoreApplication::applicationName() + ": Error Log";

    CDlgErrLog* pDlg = dynamic_cast<CDlgErrLog*>(CDlgErrLog::GetInstance());

    if( pDlg == nullptr )
    {
        pDlg = CDlgErrLog::CreateInstance(strDlgTitle);
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }
} // onActDebugErrLogTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugRequestExecTreeTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = QCoreApplication::applicationName() + ": Requests Execution Tree";

    CDlgRequestExecTree* pDlg = dynamic_cast<CDlgRequestExecTree*>(CDlgRequestExecTree::GetInstance("ReqExecTree"));

    if( pDlg == nullptr )
    {
        pDlg = CDlgRequestExecTree::CreateInstance(strDlgTitle, "ReqExecTree");
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }
} // onActDebugRequestExecTreeTriggered

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
    QString strToolTip;
    int iErrorsCount = 0;

    if( pErrLog != nullptr )
    {
        CErrLogLocker errLogLocker(pErrLog);
        QVector<int> ariErrorsCount(EResultSeverityCount, 0);
        for (int iSeverity = 0; iSeverity < ariErrorsCount.count(); ++iSeverity) {
            EResultSeverity severity = static_cast<EResultSeverity>(iSeverity);
            int iEntryCount = pErrLog->getEntryCount(severity);
            if (iEntryCount > 0) {
                ariErrorsCount[iSeverity] = iEntryCount;
                iErrorsCount += iEntryCount;
                if (severity > severityMax) {
                    severityMax = severity;
                }
            }
        }
        if (iErrorsCount == 0) {
            strToolTip = "There is no Info, no Warning, no Error and no Critical Error message pending";
        }
        else if (iErrorsCount > 0) {
            if (iErrorsCount == 1) {
                strToolTip = "There is ";
            } else {
                strToolTip = "There are ";
            }
            int iErrorsCountTmp = 0;
            for (int iSeverity = 0; iSeverity < ariErrorsCount.count(); ++iSeverity) {
                EResultSeverity severity = static_cast<EResultSeverity>(iSeverity);
                if (ariErrorsCount[iSeverity] > 0) {
                    strToolTip += QString::number(ariErrorsCount[iSeverity]) + " " + resultSeverity2Str(severity);
                    if (severity == EResultSeverityInfo || severity == EResultSeverityWarning || severity == EResultSeverityError) {
                        if (ariErrorsCount[iSeverity] > 1) {
                            strToolTip += "s";
                        }
                    }
                    else if (severity == EResultSeverityCritical) {
                        if (ariErrorsCount[iSeverity] == 1) {
                            strToolTip += " Critical Error";
                        } else {
                            strToolTip += " Critical Errors";
                        }
                    }
                    iErrorsCountTmp += ariErrorsCount[iSeverity];
                    if (iErrorsCountTmp < iErrorsCount) {
                        int iSeveritiesWithErrorsFollow = 0;
                        for (int iSeverityTmp = iSeverity+1; iSeverityTmp < EResultSeverityCount; iSeverityTmp++) {
                            if (ariErrorsCount[iSeverityTmp] > 0) {
                                iSeveritiesWithErrorsFollow++;
                            }
                        }
                        if (iSeveritiesWithErrorsFollow >= 2) {
                            strToolTip += ", ";
                        } else if (iSeveritiesWithErrorsFollow >= 1) {
                            strToolTip += " and ";
                        }
                    }
                }
            }
        }
        strToolTip += ".";
    }

    if (m_pLblErrors != nullptr) {
        m_pLblErrors->setPixmap( getErrPixmap(severityMax) );
        m_pLblErrors->setToolTip(strToolTip);
        if( iErrorsCount > 0 ) {
            m_pLblErrors->show();
        } else {
            m_pLblErrors->hide();
        }
    }

} // updateErrorsStatus

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

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onTrcClientDestroyed( QObject* i_pTrcClient )
//------------------------------------------------------------------------------
{
    if( i_pTrcClient == m_pTrcClient )
    {
        m_pTrcClient = nullptr;
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onTrcClientConnected( QObject* i_pTrcClient )
//------------------------------------------------------------------------------
{
    m_pActFileRecallAdminObjs->setEnabled(false);
    m_pActFileReadTrcMthFile->setEnabled(false);
}

//------------------------------------------------------------------------------
void CMainWindow::onTrcClientDisconnected( QObject* i_pTrcClient )
//------------------------------------------------------------------------------
{
    m_pActFileRecallAdminObjs->setEnabled(true);
    m_pActFileReadTrcMthFile->setEnabled(true);
}

//------------------------------------------------------------------------------
void CMainWindow::onTrcClientStateChanged( QObject* /*i_pTrcClient*/, int /*i_iState*/ )
//------------------------------------------------------------------------------
{
    QString strStatus;
    QPixmap pxmStatus;
    QString strConnection;
    QString strToolTip;

    Ipc::SSocketDscr socketDscr = m_pTrcClient->getSocketDscr();

    strConnection = socketDscr.m_strRemoteHostName + ":" + QString::number(socketDscr.m_uServerListenPort) + ":" + QString::number(socketDscr.m_uRemotePort);
    strToolTip = strConnection;

    if( m_pTrcClient->isConnected() )
    {
        strStatus = "Client: Connected";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleGreen.png");
    }
    else if( m_pTrcClient->requestInProgress() == CIpcTrcClient::ERequestConnect )
    {
        strStatus = "Client: Connecting ...";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleYellow.png");
    }
    else
    {
        strStatus = "Client: Disconnected";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleRed.png");
    }

    if( m_pLblTrcClientCnctStatusIcon != nullptr )
    {
        m_pLblTrcClientCnctStatusIcon->setPixmap(pxmStatus);
        m_pLblTrcClientCnctStatusIcon->setToolTip(strToolTip);
    }

} // onTrcClientStateChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onWidgetCentralProgressBarConnectDblClicked()
//------------------------------------------------------------------------------
{
    onActSettingsTrcClientTriggered();
}
