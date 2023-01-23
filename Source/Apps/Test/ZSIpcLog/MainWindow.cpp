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
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qprogressbar.h>
#include <QtGui/qstatusbar.h>
#else
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
#include "Test.h"

#include "ZSIpcLogGUI/ZSIpcLogClientDlg.h"
#include "ZSIpcLogGUI/ZSIpcLogServerDlg.h"
#include "ZSIpcLog/ZSIpcLogClient.h"
#include "ZSIpcLog/ZSIpcLogServer.h"
#include "ZSSysGUI/ZSSysErrLogDlg.h"
#include "ZSSysGUI/ZSSysRequestExecTreeDlg.h"
#include "ZSSysGUI/ZSSysLoggerIdxTreeDlg.h"
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
using namespace ZS::Apps::Test::IpcLog;


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
    CTest*          i_pTest,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent,i_wflags),
    m_pTest(i_pTest),
    m_pMnuFile(nullptr),
    m_pActFileSaveTestResults(nullptr),
    m_pActFileQuit(nullptr),
    m_pMnuDebug(nullptr),
    m_pActDebugLogServer(nullptr),
    m_pActDebugLogServerLoggersIdxTree(nullptr),
    m_pActDebugLogClient(nullptr),
    m_pActDebugLogClientLoggersIdxTree(nullptr),
    m_pActDebugErrLog(nullptr),
    m_pActDebugRequestExecTree(nullptr),
    m_pMnuInfo(nullptr),
    m_pActInfoVersion(nullptr),
    m_pStatusBar(nullptr),
    m_pWdgtLogServerStatus(nullptr),
    m_pLblLogServerStatusIcon(nullptr),
    m_pLblLogServerStatusText(nullptr),
    m_pWdgtLogClientStatus(nullptr),
    m_pLblLogClientStatusIcon(nullptr),
    m_pLblLogClientStatusText(nullptr),
    m_pLblReqInProgress(nullptr),
    m_pBarReqInProgress(nullptr),
    m_pLblErrors(nullptr),
    m_pWdgtCentral(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("MainWindow");

    setWindowTitle(i_strWindowTitle);

    CIpcLogServer* pLogServer = CApplication::GetInstance()->getLogServer();

    if( pLogServer != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ pLogServer->getIpcServer(),
            /* szSignal     */ SIGNAL(stateChanged(QObject*,int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onLogServerStateChanged(QObject*,int)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }

    CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();

    if( pLogClient != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ pLogClient,
            /* szSignal     */ SIGNAL(stateChanged(QObject*,int)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onLogClientStateChanged(QObject*,int)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }

    // <Menu> File
    //=============

    m_pMnuFile = menuBar()->addMenu(tr("&File"));

    // <MenuItem> File::Quit
    //----------------------

    m_pActFileSaveTestResults = new QAction("&Save Test Results",this);
    m_pMnuFile->addAction(m_pActFileSaveTestResults);
    m_pMnuFile->addSeparator();

    if( !connect(
        /* pObjSender   */ m_pActFileSaveTestResults,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActFileSaveTestResultsTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Quit
    //----------------------

    m_pActFileQuit = new QAction("&Quit",this);
    m_pMnuFile->addAction(m_pActFileQuit);

    if( !connect(
        /* pObjSender   */ m_pActFileQuit,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ CApplication::GetInstance(),
        /* szSlot       */ SLOT(quit()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> Debug
    //=============

    m_pMnuDebug = menuBar()->addMenu(tr("&Debug"));

    // <MenuItem> Debug::LogServer
    //----------------------------

    m_pActDebugLogServer = new QAction("Log Server",this);
    m_pMnuDebug->addAction(m_pActDebugLogServer);

    if( !connect(
        /* pObjSender   */ m_pActDebugLogServer,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActDebugLogServerTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Debug::LogServerLoggersIdxTree
    //------------------------------------------

    m_pActDebugLogServerLoggersIdxTree = new QAction("Log Server Loggers",this);
    m_pMnuDebug->addAction(m_pActDebugLogServerLoggersIdxTree);

    if( !connect(
        /* pObjSender   */ m_pActDebugLogServerLoggersIdxTree,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActDebugLogServerLoggersIdxTreeTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Debug::LogClient
    //----------------------------

    m_pActDebugLogClient = new QAction("Log Client",this);
    m_pMnuDebug->addAction(m_pActDebugLogClient);

    if( !connect(
        /* pObjSender   */ m_pActDebugLogClient,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActDebugLogClientTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Debug::LogClientLoggersIdxTree
    //------------------------------------------

    m_pActDebugLogClientLoggersIdxTree = new QAction("Log Client Loggers",this);
    m_pMnuDebug->addAction(m_pActDebugLogClientLoggersIdxTree);

    if( !connect(
        /* pObjSender   */ m_pActDebugLogClientLoggersIdxTree,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActDebugLogClientLoggersIdxTreeTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Debug::Error Log
    //-----------------------------

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
    //=============

    m_pMnuInfo = menuBar()->addMenu(tr("&Info"));

    // <MenuItem> Info::Version
    //-------------------------

    QString strActionInfoVersion = "Version: " + ZS::System::c_strSysVersionNr;

    #ifdef _DEBUG
    strActionInfoVersion += " (Debug)";
    #endif

    m_pActInfoVersion = new QAction(strActionInfoVersion,this);
    m_pMnuInfo->addAction(m_pActInfoVersion);

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

    // <Label> Log Server Status
    //--------------------------

    m_pWdgtLogServerStatus = new QWidget(this);

    new QHBoxLayout(m_pWdgtLogServerStatus);

    m_pWdgtLogServerStatus->setContentsMargins(0,0,0,0);
    m_pWdgtLogServerStatus->layout()->setContentsMargins(1,1,1,1);
    m_pWdgtLogServerStatus->installEventFilter(this);

    m_pLblLogServerStatusIcon = new QLabel("");
    m_pLblLogServerStatusIcon->setScaledContents(true);
    m_pLblLogServerStatusIcon->setMaximumSize( QSize(16,16) );
    m_pWdgtLogServerStatus->layout()->addWidget(m_pLblLogServerStatusIcon);

    m_pLblLogServerStatusText = new QLabel("LogServer: ---");
    m_pWdgtLogServerStatus->layout()->addWidget(m_pLblLogServerStatusText);

    statusBar()->addPermanentWidget(m_pWdgtLogServerStatus);

    if( pLogServer != nullptr )
    {
        onLogServerStateChanged(pLogServer->getIpcServer(), pLogServer->getState());
    }

    // <Label> Log Client Status
    //--------------------------

    m_pWdgtLogClientStatus = new QWidget(this);

    new QHBoxLayout(m_pWdgtLogClientStatus);

    m_pWdgtLogClientStatus->setContentsMargins(0,0,0,0);
    m_pWdgtLogClientStatus->layout()->setContentsMargins(1,1,1,1);
    m_pWdgtLogClientStatus->installEventFilter(this);

    m_pLblLogClientStatusIcon = new QLabel("");
    m_pLblLogClientStatusIcon->setScaledContents(true);
    m_pLblLogClientStatusIcon->setMaximumSize( QSize(16,16) );
    m_pWdgtLogClientStatus->layout()->addWidget(m_pLblLogClientStatusIcon);

    m_pLblLogClientStatusText = new QLabel("LogClient: ---");
    m_pWdgtLogClientStatus->layout()->addWidget(m_pLblLogClientStatusText);

    statusBar()->addPermanentWidget(m_pWdgtLogClientStatus);

    if( pLogClient != nullptr )
    {
        onLogClientStateChanged( pLogClient, pLogClient->getState() );
    }

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

    updateReqInProgressStatus( "Ready", 100 );

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

    // Central Widget
    //===============

    m_pWdgtCentral = new CWidgetCentral(m_pTest);

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

    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

} // ctor

//------------------------------------------------------------------------------
CMainWindow::~CMainWindow()
//------------------------------------------------------------------------------
{
    QSettings settings;

    settings.setValue( objectName()+"/Geometry", saveGeometry() );

    CDialog::DestroyAllInstances();

    m_pTest = nullptr;
    m_pMnuFile = nullptr;
    m_pActFileQuit = nullptr;
    m_pActFileSaveTestResults = nullptr;
    m_pMnuDebug = nullptr;
    m_pActDebugLogServer = nullptr;
    m_pActDebugLogServerLoggersIdxTree = nullptr;
    m_pActDebugLogClient = nullptr;
    m_pActDebugLogClientLoggersIdxTree = nullptr;
    m_pActDebugErrLog = nullptr;
    m_pActDebugRequestExecTree = nullptr;
    m_pMnuInfo = nullptr;
    m_pActInfoVersion = nullptr;
    m_pStatusBar = nullptr;
    m_pWdgtLogServerStatus = nullptr;
    m_pLblLogServerStatusIcon = nullptr;
    m_pLblLogServerStatusText = nullptr;
    m_pWdgtLogClientStatus = nullptr;
    m_pLblLogClientStatusIcon = nullptr;
    m_pLblLogClientStatusText = nullptr;
    m_pLblReqInProgress = nullptr;
    m_pBarReqInProgress = nullptr;
    m_pLblErrors = nullptr;
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
}

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
    else if( i_pObjWatched == m_pWdgtLogServerStatus )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            onActDebugLogServerTriggered();
            bHandled = true;
        }
    }
    else if( i_pObjWatched == m_pWdgtLogClientStatus )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            onActDebugLogClientTriggered();
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
void CMainWindow::onActFileSaveTestResultsTriggered()
//------------------------------------------------------------------------------
{
    m_pTest->saveTestResults();
}

//------------------------------------------------------------------------------
void CMainWindow::onActDebugLogServerTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = QCoreApplication::applicationName() + ": Log Server";

    CIpcLogServer* pServer = CIpcLogServer::GetInstance();

    if( pServer != nullptr )
    {
        CDlgLogServer* pDlg = CDlgLogServer::GetInstance(pServer->objectName());

        if( pDlg == nullptr )
        {
            pDlg = CDlgLogServer::CreateInstance(strDlgTitle, pServer->objectName());
            pDlg->setServer(pServer);
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
    }
} // onActDebugLogServerTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugLogServerLoggersIdxTreeTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = QCoreApplication::applicationName() + ": Log Server Loggers";

    CIpcLogServer* pServer = CIpcLogServer::GetInstance();

    if( pServer != nullptr )
    {
        CDlgIdxTreeLoggers* pDlg = CDlgIdxTreeLoggers::GetInstance(pServer->getLoggersIdxTree()->objectName());

        if( pDlg == nullptr )
        {
            pDlg = CDlgIdxTreeLoggers::CreateInstance(
                /* strDlgTitle    */ strDlgTitle,
                /* pTrcAdmIdxTree */ pServer->getLoggersIdxTree(),
                /* pWdgtParent    */ nullptr,
                /* wFlags         */ Qt::WindowFlags() );
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
    }
} // onActDebugLogServerLoggersIdxTreeTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugLogClientTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Log Client";

    CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();

    if( pLogClient != nullptr )
    {
        CDlgLogClient* pDlg = CDlgLogClient::GetInstance(pLogClient->objectName());

        if( pDlg == nullptr )
        {
            pDlg = CDlgLogClient::CreateInstance(strDlgTitle, pLogClient->objectName());
            pDlg->setClient(pLogClient);
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
    }
} // onActDebugLogClientTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugLogClientLoggersIdxTreeTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = QCoreApplication::applicationName() + ": Log Client Loggers";

    CIpcLogClient* pLogClient = CApplication::GetInstance()->getLogClient();

    if( pLogClient != nullptr )
    {
        CDlgIdxTreeLoggers* pDlg = CDlgIdxTreeLoggers::GetInstance(pLogClient->getLoggersIdxTree()->objectName());

        if( pDlg == nullptr )
        {
            pDlg = CDlgIdxTreeLoggers::CreateInstance(
                /* strDlgTitle     */ strDlgTitle,
                /* pTrcAdmObjPool  */ pLogClient->getLoggersIdxTree(),
                /* pWdgtParent     */ nullptr,
                /* wflags          */ Qt::WindowFlags() );
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
    }
} // onActDebugLogClientLoggersIdxTreeTriggered

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
void CMainWindow::onLogServerStateChanged( QObject* i_pServer, int /*i_iState*/ )
//------------------------------------------------------------------------------
{
    QString strStatus;
    QPixmap pxmStatus;
    QString strConnection;
    QString strToolTip;

    Ipc::CServer* pIpcServer = dynamic_cast<Ipc::CServer*>(i_pServer);

    Ipc::SServerHostSettings cnctSettings = pIpcServer->getHostSettings();

    strConnection = cnctSettings.m_strLocalHostName + ":" + QString::number(cnctSettings.m_uLocalPort);
    strToolTip = strConnection;

    if( pIpcServer->isListening() )
    {
        strStatus = pIpcServer->objectName() + ": Listening";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleGreen.png");
    }
    else
    {
        strStatus = pIpcServer->objectName() + ": Not Listening";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleRed.png");
    }

    if( m_pLblLogServerStatusIcon != nullptr )
    {
        m_pLblLogServerStatusIcon->setPixmap(pxmStatus);
        m_pLblLogServerStatusIcon->setToolTip(strToolTip);
    }
    if( m_pLblLogServerStatusText != nullptr )
    {
        m_pLblLogServerStatusText->setText(strStatus);
        m_pLblLogServerStatusText->setToolTip(strToolTip);
    }
} // onLogServerStateChanged

//------------------------------------------------------------------------------
void CMainWindow::onLogClientStateChanged( QObject* i_pClient, int /*i_iState*/ )
//------------------------------------------------------------------------------
{
    QString strStatus;
    QPixmap pxmStatus;
    QString strConnection;
    QString strToolTip;

    Ipc::CClient* pIpcClient = dynamic_cast<Ipc::CClient*>(i_pClient);

    Ipc::SClientHostSettings cnctSettings = pIpcClient->getHostSettings();

    strConnection = cnctSettings.m_strRemoteHostName + ":" + QString::number(cnctSettings.m_uRemotePort);
    strToolTip = strConnection;

    if( pIpcClient->isConnected() )
    {
        strStatus = pIpcClient->objectName() + ": Connected";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleGreen.png");
    }
    else
    {
        strStatus = pIpcClient->objectName() + ": Not Connected";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleRed.png");
    }

    if( m_pLblLogClientStatusIcon != nullptr )
    {
        m_pLblLogClientStatusIcon->setPixmap(pxmStatus);
        m_pLblLogClientStatusIcon->setToolTip(strToolTip);
    }
    if( m_pLblLogClientStatusText != nullptr )
    {
        m_pLblLogClientStatusText->setText(strStatus);
        m_pLblLogClientStatusText->setToolTip(strToolTip);
    }
} // onLogClientStateChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onWidgetCentralProgressBarConnectDblClicked()
//------------------------------------------------------------------------------
{
    onActDebugLogClientTriggered();
}
