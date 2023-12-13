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

#include "ZSIpcTraceGUI/ZSIpcTrcClientDlg.h"
#include "ZSIpcTraceGUI/ZSIpcTrcServerDlg.h"
#include "ZSIpcTrace/ZSIpcTrcClient.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"
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
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;
using namespace ZS::Apps::Test::IpcTrace;


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
    QMainWindow(i_pWdgtParent, i_wflags),
    m_pTest(i_pTest),
    m_pMnuFile(nullptr),
    m_pActFileSaveTestResults(nullptr),
    m_pActFileQuit(nullptr),
    m_pMnuDebug(nullptr),
    m_pActDebugZSTrcServer(nullptr),
    m_pActDebugZSTrcServerAdminObjIdxTree(nullptr),
    m_pActDebugZSTrcClient(nullptr),
    m_pActDebugZSTrcClientAdminObjIdxTree(nullptr),
    m_pActDebugErrLog(nullptr),
    m_pActDebugRequestExecTree(nullptr),
    m_pMnuInfo(nullptr),
    m_pActInfoVersion(nullptr),
    m_pStatusBar(nullptr),
    m_pWdgtZSTrcServerStatus(nullptr),
    m_pLblZSTrcServerStatusIcon(nullptr),
    m_pLblZSTrcServerStatusText(nullptr),
    m_pWdgtZSTrcClientStatus(nullptr),
    m_pLblZSTrcClientStatusIcon(nullptr),
    m_pLblZSTrcClientStatusText(nullptr),
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

    CIpcTrcServer* pTrcServer = CApplication::GetInstance()->getTrcServer();

    if( pTrcServer != nullptr )
    {
        QObject::connect(
            pTrcServer->getIpcServer(), &ZS::Ipc::CServer::stateChanged,
            this, &CMainWindow::onTrcServerStateChanged);
    }

    CIpcTrcClient* pTrcClient = CApplication::GetInstance()->getTrcClient();

    if( pTrcClient != nullptr )
    {
        QObject::connect(
            pTrcClient, &CIpcTrcClient::stateChanged,
            this, &CMainWindow::onTrcClientStateChanged);
    }

    // <Menu> File
    //=============

    m_pMnuFile = menuBar()->addMenu(tr("&File"));

    // <MenuItem> File::Save Test Results
    //-----------------------------------

    m_pActFileSaveTestResults = new QAction("&Save Test Results",this);
    m_pMnuFile->addAction(m_pActFileSaveTestResults);
    m_pMnuFile->addSeparator();

    QObject::connect(
        m_pActFileSaveTestResults, &QAction::triggered,
        this, &CMainWindow::onActFileSaveTestResultsTriggered);

    // <MenuItem> File::Quit
    //----------------------

    m_pActFileQuit = new QAction("&Quit",this);
    m_pMnuFile->addAction(m_pActFileQuit);

    QObject::connect(
        m_pActFileQuit, &QAction::triggered,
        qApp, &QApplication::quit);

    // <Menu> Debug
    //=============

    m_pMnuDebug = menuBar()->addMenu(tr("&Debug"));

    // <MenuItem> Debug::ZSTraceServer
    //--------------------------------

    m_pActDebugZSTrcServer = new QAction("ZS Trace Server",this);
    m_pMnuDebug->addAction(m_pActDebugZSTrcServer);

    QObject::connect(
        m_pActDebugZSTrcServer, &QAction::triggered,
        this, &CMainWindow::onActDebugZSTrcServerTriggered);

    // <MenuItem> Debug::ZSTraceServerAdminIdxTree
    //--------------------------------------------

    m_pActDebugZSTrcServerAdminObjIdxTree = new QAction("ZS Trace Server Admin Objects",this);
    m_pMnuDebug->addAction(m_pActDebugZSTrcServerAdminObjIdxTree);

    QObject::connect(
        m_pActDebugZSTrcServerAdminObjIdxTree, &QAction::triggered,
        this, &CMainWindow::onActDebugZSTrcServerAdminObjIdxTreeTriggered);

    // <MenuItem> Debug::ZSTraceClient
    //--------------------------------

    m_pActDebugZSTrcClient = new QAction("ZS Trace Client",this);
    m_pMnuDebug->addAction(m_pActDebugZSTrcClient);

    QObject::connect(
        m_pActDebugZSTrcClient, &QAction::triggered,
        this, &CMainWindow::onActDebugZSTrcClientTriggered);

    // <MenuItem> Debug::ZSTraceClientAdminIdxTree
    //--------------------------------------------

    m_pActDebugZSTrcClientAdminObjIdxTree = new QAction("ZS Trace Client Admin Objects",this);
    m_pMnuDebug->addAction(m_pActDebugZSTrcClientAdminObjIdxTree);

    QObject::connect(
        m_pActDebugZSTrcClientAdminObjIdxTree, &QAction::triggered,
        this, &CMainWindow::onActDebugZSTrcClientAdminObjIdxTreeTriggered);

    // <MenuItem> Debug::Error Log
    //-----------------------------

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

    // <Label> ZS Trace Server Status
    //-------------------------------

    m_pWdgtZSTrcServerStatus = new QWidget(this);

    new QHBoxLayout(m_pWdgtZSTrcServerStatus);

    m_pWdgtZSTrcServerStatus->setContentsMargins(0,0,0,0);
    m_pWdgtZSTrcServerStatus->layout()->setContentsMargins(1,1,1,1);
    m_pWdgtZSTrcServerStatus->installEventFilter(this);

    m_pLblZSTrcServerStatusIcon = new QLabel("");
    m_pLblZSTrcServerStatusIcon->setScaledContents(true);
    m_pLblZSTrcServerStatusIcon->setMaximumSize( QSize(16,16) );
    m_pWdgtZSTrcServerStatus->layout()->addWidget(m_pLblZSTrcServerStatusIcon);

    m_pLblZSTrcServerStatusText = new QLabel("ZSTrcServer: ---");
    m_pWdgtZSTrcServerStatus->layout()->addWidget(m_pLblZSTrcServerStatusText);

    statusBar()->addPermanentWidget(m_pWdgtZSTrcServerStatus);

    if( pTrcServer != nullptr )
    {
        onTrcServerStateChanged(pTrcServer->getIpcServer(), pTrcServer->getState());
    }

    // <Label> ZS Trace Client Status
    //-------------------------------

    m_pWdgtZSTrcClientStatus = new QWidget(this);

    new QHBoxLayout(m_pWdgtZSTrcClientStatus);

    m_pWdgtZSTrcClientStatus->setContentsMargins(0,0,0,0);
    m_pWdgtZSTrcClientStatus->layout()->setContentsMargins(1,1,1,1);
    m_pWdgtZSTrcClientStatus->installEventFilter(this);

    m_pLblZSTrcClientStatusIcon = new QLabel("");
    m_pLblZSTrcClientStatusIcon->setScaledContents(true);
    m_pLblZSTrcClientStatusIcon->setMaximumSize( QSize(16,16) );
    m_pWdgtZSTrcClientStatus->layout()->addWidget(m_pLblZSTrcClientStatusIcon);

    m_pLblZSTrcClientStatusText = new QLabel("ZSTrcClient: ---");
    m_pWdgtZSTrcClientStatus->layout()->addWidget(m_pLblZSTrcClientStatusText);

    statusBar()->addPermanentWidget(m_pWdgtZSTrcClientStatus);

    if( pTrcClient != nullptr )
    {
        onTrcClientStateChanged( pTrcClient, pTrcClient->getState() );
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

    // Central Widget
    //===============

    m_pWdgtCentral = new CWidgetCentral(m_pTest);

    setCentralWidget(m_pWdgtCentral);

    QObject::connect(
        m_pWdgtCentral, &CWidgetCentral::progressBarConnectDblClicked,
        this, &CMainWindow::onWidgetCentralProgressBarConnectDblClicked);

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
    m_pActDebugZSTrcServer = nullptr;
    m_pActDebugZSTrcServerAdminObjIdxTree = nullptr;
    m_pActDebugZSTrcClient = nullptr;
    m_pActDebugZSTrcClientAdminObjIdxTree = nullptr;
    m_pActDebugErrLog = nullptr;
    m_pActDebugRequestExecTree = nullptr;
    m_pMnuInfo = nullptr;
    m_pActInfoVersion = nullptr;
    m_pStatusBar = nullptr;
    m_pWdgtZSTrcServerStatus = nullptr;
    m_pLblZSTrcServerStatusIcon = nullptr;
    m_pLblZSTrcServerStatusText = nullptr;
    m_pWdgtZSTrcClientStatus = nullptr;
    m_pLblZSTrcClientStatusIcon = nullptr;
    m_pLblZSTrcClientStatusText = nullptr;
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
    else if( i_pObjWatched == m_pWdgtZSTrcServerStatus )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            onActDebugZSTrcServerTriggered();
            bHandled = true;
        }
    }
    else if( i_pObjWatched == m_pWdgtZSTrcClientStatus )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            onActDebugZSTrcClientTriggered();
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
void CMainWindow::onActDebugZSTrcServerTriggered()
//------------------------------------------------------------------------------
{
    CIpcTrcServer* pServer = CIpcTrcServer::GetInstance();

    if( pServer != nullptr )
    {
        CDlgTrcServer* pDlg = CDlgTrcServer::GetInstance();

        if( pDlg == nullptr )
        {
            QString strDlgTitle = QCoreApplication::applicationName() + ": ZS Trace Server";
            pDlg = CDlgTrcServer::CreateInstance(strDlgTitle);
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
} // onActDebugZSTrcServerTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugZSTrcServerAdminObjIdxTreeTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = QCoreApplication::applicationName() + ": ZS Trace Server Admin Objects";

    CIpcTrcServer* pServer = CIpcTrcServer::GetInstance();

    if( pServer != nullptr )
    {
        CDlgIdxTreeTrcAdminObjs* pDlg = CDlgIdxTreeTrcAdminObjs::GetInstance(pServer->getTraceAdminObjIdxTree()->objectName());

        if( pDlg == nullptr )
        {
            pDlg = CDlgIdxTreeTrcAdminObjs::CreateInstance(
                /* strDlgTitle */ strDlgTitle,
                /* pIdxTree    */ pServer->getTraceAdminObjIdxTree(),
                /* pWdgtParent */ nullptr,
                /* wFlags      */ Qt::WindowFlags() );
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
} // onActDebugZSTrcServerAdminObjIdxTreeTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugZSTrcClientTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": ZS Trace Client";

    CIpcTrcClient* pTrcClient = CApplication::GetInstance()->getTrcClient();

    if( pTrcClient != nullptr )
    {
        CDlgTrcClient* pDlg = CDlgTrcClient::GetInstance(pTrcClient->objectName());

        if( pDlg == nullptr )
        {
            pDlg = CDlgTrcClient::CreateInstance(strDlgTitle, pTrcClient->objectName());
            pDlg->setClient(pTrcClient);
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
} // onActDebugZSTrcClientTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugZSTrcClientAdminObjIdxTreeTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = QCoreApplication::applicationName() + ": ZS Trace Client";

    CIpcTrcClient* pTrcClient = CApplication::GetInstance()->getTrcClient();

    if( pTrcClient != nullptr )
    {
        CDlgIdxTreeTrcAdminObjs* pDlg = CDlgIdxTreeTrcAdminObjs::GetInstance(pTrcClient->getTraceAdminObjIdxTree()->objectName());

        if( pDlg == nullptr )
        {
            pDlg = CDlgIdxTreeTrcAdminObjs::CreateInstance(
                /* strDlgTitle */ strDlgTitle,
                /* pIdxTree    */ pTrcClient->getTraceAdminObjIdxTree(),
                /* pWdgtParent */ nullptr,
                /* wflags      */ Qt::WindowFlags() );
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
} // onActDebugZSTrcClientAdminObjIdxTreeTriggered

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
void CMainWindow::onTrcServerStateChanged( QObject* i_pServer, int /*i_iState*/ )
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

    if( m_pLblZSTrcServerStatusIcon != nullptr )
    {
        m_pLblZSTrcServerStatusIcon->setPixmap(pxmStatus);
        m_pLblZSTrcServerStatusIcon->setToolTip(strToolTip);
    }
    if( m_pLblZSTrcServerStatusText != nullptr )
    {
        m_pLblZSTrcServerStatusText->setText(strStatus);
        m_pLblZSTrcServerStatusText->setToolTip(strToolTip);
    }
} // onTrcServerStateChanged

//------------------------------------------------------------------------------
void CMainWindow::onTrcClientStateChanged( QObject* i_pClient, int /*i_iState*/ )
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

    if( m_pLblZSTrcClientStatusIcon != nullptr )
    {
        m_pLblZSTrcClientStatusIcon->setPixmap(pxmStatus);
        m_pLblZSTrcClientStatusIcon->setToolTip(strToolTip);
    }
    if( m_pLblZSTrcClientStatusText != nullptr )
    {
        m_pLblZSTrcClientStatusText->setText(strStatus);
        m_pLblZSTrcClientStatusText->setToolTip(strToolTip);
    }
} // onTrcClientStateChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onWidgetCentralProgressBarConnectDblClicked()
//------------------------------------------------------------------------------
{
    onActDebugZSTrcClientTriggered();
}
