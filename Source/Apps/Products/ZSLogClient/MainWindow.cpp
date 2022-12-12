/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include "ZSIpcLogGUI/ZSIpcLogClientDlg.h"
#include "ZSIpcLogGUI/ZSIpcLogWdgt.h"
#include "ZSIpcLog/ZSIpcLogClient.h"
#include "ZSSysGUI/ZSSysFindTextDlg.h"
#include "ZSSysGUI/ZSSysErrLogDlg.h"
#include "ZSSysGUI/ZSSysRequestExecTreeDlg.h"
#include "ZSSysGUI/ZSSysLoggerIdxTreeDlg.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysLoggerIdxTree.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Log;
using namespace ZS::Log::GUI;
using namespace ZS::Apps::Products::LogClient;


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
    CIpcLogClient*  i_pLogClient,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent,i_wflags),
    m_pSettingsFile(nullptr),
    m_pLogClient(i_pLogClient),
    m_pMnuFile(nullptr),
    m_pActFileRecallLoggers(nullptr),
    m_pActFileSaveLoggers(nullptr),
    m_pActFileReadLogFile(nullptr),
    m_pActFileWriteLogFile(nullptr),
    m_pActFileQuit(nullptr),
    m_pMnuEdit(nullptr),
    m_pActEditFind(nullptr),
    m_pMnuSettings(nullptr),
    m_pActSettingsLogClient(nullptr),
    m_pActSettingsLoggersIdxTree(nullptr),
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
    m_pWdgtLogClientCnctStatus(nullptr),
    m_pLblLogClientCnctStatusIcon(nullptr),
    m_pWdgtCentral(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("MainWindow");

    setWindowTitle(i_strWindowTitle);

    if( m_pLogClient == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pLogClient,
        /* szSignal     */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onLogClientDestroyed(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pLogClient,
        /* szSignal     */ SIGNAL(connected(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onLogClientConnected(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pLogClient,
        /* szSignal     */ SIGNAL(disconnected(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onLogClientDisconnected(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pLogClient,
        /* szSignal     */ SIGNAL(stateChanged(QObject*,int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onLogClientStateChanged(QObject*,int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Menu> File
    //=============

    m_pMnuFile = menuBar()->addMenu(tr("&File"));

    // <MenuItem> File::Recall Loggers
    //--------------------------------

    m_pActFileRecallLoggers = new QAction("Recall Loggers", this);
    m_pMnuFile->addAction(m_pActFileRecallLoggers);

    if( !connect(
        /* pObjSender   */ m_pActFileRecallLoggers,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActFileRecallLoggersTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Save Loggers
    //------------------------------

    m_pActFileSaveLoggers = new QAction("Save Loggers", this);
    m_pMnuFile->addAction(m_pActFileSaveLoggers);

    if( !connect(
        /* pObjSender   */ m_pActFileSaveLoggers,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActFileSaveLoggersTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Read Log File
    //-------------------------------

    m_pMnuFile->addSeparator();

    m_pActFileReadLogFile = new QAction("Read Log File", this);
    m_pMnuFile->addAction(m_pActFileReadLogFile);

    if( !connect(
        /* pObjSender   */ m_pActFileReadLogFile,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActFileReadLogFileTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Write Log File
    //--------------------------------

    m_pActFileWriteLogFile = new QAction("Write Log File", this);
    m_pMnuFile->addAction(m_pActFileWriteLogFile);

    if( !connect(
        /* pObjSender   */ m_pActFileWriteLogFile,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActFileWriteLogFileTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Quit
    //----------------------

    m_pMnuFile->addSeparator();

    m_pActFileQuit = new QAction("&Quit", this);
    m_pMnuFile->addAction(m_pActFileQuit);

    if( !connect(
        /* pObjSender   */ m_pActFileQuit,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ CApplication::GetInstance(),
        /* szSlot       */ SLOT(quit()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> Edit
    //============

    m_pMnuEdit = menuBar()->addMenu(tr("&Edit"));

    // <MenuItem> Find
    //----------------

    m_pActEditFind = new QAction("&Find",this);
    m_pActEditFind->setShortcuts(QKeySequence::Find);
    m_pMnuEdit->addAction(m_pActEditFind);

    if( !connect(
        /* pObjSender   */ m_pActEditFind,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActEditFindTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> Settings
    //================

    m_pMnuSettings = menuBar()->addMenu(tr("&Settings"));

    // <MenuItem> Settings::LogClient
    //-------------------------------

    m_pActSettingsLogClient = new QAction("&Log Client",this);
    m_pMnuSettings->addAction(m_pActSettingsLogClient);

    if( !connect(
        /* pObjSender   */ m_pActSettingsLogClient,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActSettingsLogClientTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Settings::LoggersIdxTree
    //------------------------------------

    m_pActSettingsLoggersIdxTree = new QAction("&Loggers",this);
    m_pMnuSettings->addAction(m_pActSettingsLoggersIdxTree);

    if( !connect(
        /* pObjSender   */ m_pActSettingsLoggersIdxTree,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActSettingsLoggersIdxTreeTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

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

    QIcon iconReqExecTree;

    QPixmap pxmReqExecTree(":/ZS/App/Zeus32x32.png");

    iconReqExecTree.addPixmap(pxmReqExecTree);

    m_pActDebugRequestExecTree = new QAction( iconReqExecTree, "Request Tree", this );
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

    // <Label> Trace Client Status
    //----------------------------

    m_pWdgtLogClientCnctStatus = new QWidget(this);

    new QHBoxLayout(m_pWdgtLogClientCnctStatus);

    m_pWdgtLogClientCnctStatus->setContentsMargins(0,0,0,0);
    m_pWdgtLogClientCnctStatus->layout()->setContentsMargins(1,1,1,1);
    m_pWdgtLogClientCnctStatus->installEventFilter(this);

    m_pLblLogClientCnctStatusIcon = new QLabel("");
    m_pLblLogClientCnctStatusIcon->setScaledContents(true);
    m_pLblLogClientCnctStatusIcon->setMaximumSize( QSize(16,16) );
    m_pWdgtLogClientCnctStatus->layout()->addWidget(m_pLblLogClientCnctStatusIcon);

    statusBar()->addPermanentWidget(m_pWdgtLogClientCnctStatus);

    onLogClientStateChanged( m_pLogClient, m_pLogClient->getState() );

    // Central Widget
    //===============

    m_pWdgtCentral = new CWidgetCentral(i_pLogClient);

    setCentralWidget(m_pWdgtCentral);

    if( !connect(
        /* pObjSender   */ m_pWdgtCentral,
        /* szSignal     */ SIGNAL(progressBarConnectDblClicked()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onWidgetCentralProgressBarConnectDblClicked()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

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
    m_pLogClient = nullptr;
    m_pMnuFile = nullptr;
    m_pActFileRecallLoggers = nullptr;
    m_pActFileSaveLoggers = nullptr;
    m_pActFileReadLogFile = nullptr;
    m_pActFileWriteLogFile = nullptr;
    m_pActFileQuit = nullptr;
    m_pMnuEdit = nullptr;
    m_pActEditFind = nullptr;
    m_pMnuSettings = nullptr;
    m_pActSettingsLogClient = nullptr;
    m_pActSettingsLoggersIdxTree = nullptr;
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
    m_pWdgtLogClientCnctStatus = nullptr;
    m_pLblLogClientCnctStatusIcon = nullptr;
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
    else if( i_pObjWatched == m_pWdgtLogClientCnctStatus )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            onActSettingsLogClientTriggered();
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
void CMainWindow::onActFileRecallLoggersTriggered()
//------------------------------------------------------------------------------
{
    QSettings settings;
    QString   strAbsFilePath;

    settings.beginGroup("FilePaths");
    strAbsFilePath = settings.value("Loggers", getAppConfigDir("System")).toString();
    settings.endGroup();

    strAbsFilePath = QFileDialog::getOpenFileName(
        /* parent             */ this,
        /* strCaption         */ windowTitle() + ": Recall loggers",
        /* strDir             */ strAbsFilePath,
        /* filter             */ "Loggers (*.xml)",
        /* pstrSelectedFilter */ nullptr,
        /* options            */ QFileDialog::Options() );

    if( !strAbsFilePath.isEmpty() )
    {
        settings.beginGroup("FilePaths");
        settings.setValue("AdminObjs", strAbsFilePath);
        settings.endGroup();

        m_pLogClient->getLoggersIdxTree()->recall(strAbsFilePath);
    }
} // onActFileRecallLoggersTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActFileSaveLoggersTriggered()
//------------------------------------------------------------------------------
{
    QSettings settings;
    QString   strAbsFilePath;

    settings.beginGroup("FilePaths");
    strAbsFilePath = settings.value("Loggers", getAppConfigDir("System")).toString();
    settings.endGroup();

    strAbsFilePath = QFileDialog::getSaveFileName(
        /* parent             */ this,
        /* strCaption         */ windowTitle() + ": Recall loggers",
        /* strDir             */ strAbsFilePath,
        /* filter             */ "Loggers (*.xml)",
        /* pstrSelectedFilter */ nullptr,
        /* options            */ QFileDialog::Options() );

    if( !strAbsFilePath.isEmpty() )
    {
        settings.beginGroup("FilePaths");
        settings.setValue("Loggers", strAbsFilePath);
        settings.endGroup();

        m_pLogClient->getLoggersIdxTree()->save(strAbsFilePath);
    }
} // onActFileSaveLoggersTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActFileReadLogFileTriggered()
//------------------------------------------------------------------------------
{
    QSettings settings;
    QString   strAbsFilePath;

    settings.beginGroup("FilePaths");
    strAbsFilePath = settings.value("LogFile", getAppLogDir("System")).toString();
    settings.endGroup();

    strAbsFilePath = QFileDialog::getOpenFileName(
        /* parent             */ this,
        /* strCaption         */ windowTitle() + ": Read log file",
        /* strDir             */ strAbsFilePath,
        /* filter             */ "Log Files (*.log)",
        /* pstrSelectedFilter */ nullptr,
        /* options            */ QFileDialog::Options() );

    if( !strAbsFilePath.isEmpty() )
    {
        settings.beginGroup("FilePaths");
        settings.setValue("LogFile", strAbsFilePath);
        settings.endGroup();

        m_pWdgtCentral->getLogWidget()->readLogFile(strAbsFilePath);
    }
} // onActFileReadLogFileTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActFileWriteLogFileTriggered()
//------------------------------------------------------------------------------
{
    QSettings settings;
    QString   strAbsFilePath;

    settings.beginGroup("FilePaths");
    strAbsFilePath = settings.value("LogFile", getAppLogDir("System")).toString();
    settings.endGroup();

    strAbsFilePath = QFileDialog::getSaveFileName(
        /* parent             */ this,
        /* strCaption         */ windowTitle() + ": Write log file",
        /* strDir             */ strAbsFilePath,
        /* filter             */ "Log Files (*.log)",
        /* pstrSelectedFilter */ nullptr,
        /* options            */ QFileDialog::Options() );

    if( !strAbsFilePath.isEmpty() )
    {
        settings.beginGroup("FilePaths");
        settings.setValue("LogFile", strAbsFilePath);
        settings.endGroup();

        m_pWdgtCentral->getLogWidget()->writeLogFile(strAbsFilePath);
    }
} // onActFileWriteLogFileTriggered

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
void CMainWindow::onActSettingsLogClientTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Log Client";

    CDlgLogClient* pDlg = CDlgLogClient::GetInstance(CApplication::GetInstance()->getLogClient()->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgLogClient::CreateInstance(
            /* strDlgTitle */ strDlgTitle,
            /* strObjName  */ CApplication::GetInstance()->getLogClient()->objectName(),
            /* pWdgtParent */ nullptr );
        pDlg->setClient(CApplication::GetInstance()->getLogClient());
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
} // onActSettingsLogClientTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActSettingsLoggersIdxTreeTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Loggers";

    CDlgIdxTreeLoggers* pDlg = CDlgIdxTreeLoggers::GetInstance(m_pLogClient->getLoggersIdxTree()->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgIdxTreeLoggers::CreateInstance(
            /* strDlgTitle    */ strDlgTitle,
            /* pTrcAdmIdxTree */ m_pLogClient->getLoggersIdxTree() );
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
} // onActSettingsLoggersIdxTreeTriggered

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
void CMainWindow::onLogClientDestroyed( QObject* i_pTrcClient )
//------------------------------------------------------------------------------
{
    if( i_pTrcClient == m_pLogClient )
    {
        m_pLogClient = nullptr;
    }
}

//------------------------------------------------------------------------------
void CMainWindow::onLogClientConnected( QObject* i_pTrcClient )
//------------------------------------------------------------------------------
{
    m_pActFileRecallLoggers->setEnabled(false);
    m_pActFileReadLogFile->setEnabled(false);
}

//------------------------------------------------------------------------------
void CMainWindow::onLogClientDisconnected( QObject* i_pTrcClient )
//------------------------------------------------------------------------------
{
    m_pActFileRecallLoggers->setEnabled(true);
    m_pActFileReadLogFile->setEnabled(true);
}

//------------------------------------------------------------------------------
void CMainWindow::onLogClientStateChanged( QObject* /*i_pTrcClient*/, int /*i_iState*/ )
//------------------------------------------------------------------------------
{
    QString strStatus;
    QPixmap pxmStatus;
    QString strConnection;
    QString strToolTip;

    Ipc::SSocketDscr socketDscr = m_pLogClient->getSocketDscr();

    strConnection = socketDscr.m_strRemoteHostName + ":" + QString::number(socketDscr.m_uServerListenPort) + ":" + QString::number(socketDscr.m_uRemotePort);
    strToolTip = strConnection;

    if( m_pLogClient->isConnected() )
    {
        strStatus = "Client: Connected";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleGreen.png");
    }
    else if( m_pLogClient->requestInProgress() == CIpcLogClient::ERequestConnect )
    {
        strStatus = "Client: Connecting ...";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleYellow.png");
    }
    else
    {
        strStatus = "Client: Disconnected";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleRed.png");
    }

    if( m_pLblLogClientCnctStatusIcon != nullptr )
    {
        m_pLblLogClientCnctStatusIcon->setPixmap(pxmStatus);
        m_pLblLogClientCnctStatusIcon->setToolTip(strToolTip);
    }

} // onLogClientStateChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onWidgetCentralProgressBarConnectDblClicked()
//------------------------------------------------------------------------------
{
    onActSettingsLogClientTriggered();
}
