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
#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qprogressbar.h>
#include <QtGui/qstatusbar.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qprogressbar.h>
#include <QtWidgets/qstatusbar.h>
#endif

#include "MainWindow.h"
#include "App.h"
#include "WidgetCentral.h"

#include "ZSRemCmd/ZSRemCmdServer.h"
#include "ZSRemCmdGUI/ZSRemCmdsDlg.h"
#include "ZSIpcTraceGUI/ZSIpcTrcServerDlg.h"
#include "ZSIpcGUI/ZSIpcServerDlg.h"
#include "ZSSysGUI/ZSSysErrLogDlg.h"
#include "ZSSysGUI/ZSSysRequestExecTreeDlg.h"
#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeDlg.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Ipc::GUI;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;
using namespace ZS::RemCmd;
using namespace ZS::RemCmd::GUI;
using namespace ZS::Apps::Test::RemCmdServer;


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
    CRemCmdServer*  i_pRemCmdServer,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent,i_wflags),
    m_pSettingsFile(nullptr),
    m_pRemCmdServer(i_pRemCmdServer),
    m_pMnuFile(nullptr),
    m_pActFileQuit(nullptr),
    m_pMnuSettings(nullptr),
    m_pActSettingsRemCmdServer(nullptr),
    m_pActSettingsTraceServer(nullptr),
    m_pActSettingsTrcAdminObjIdxTree(nullptr),
    m_pActSettingsFileInfo(nullptr),
    m_pMnuInfo(nullptr),
    m_pActInfoRemCmdObjPool(nullptr),
    m_pActInfoErrLog(nullptr),
    m_pActInfoRequestExecTree(nullptr),
    m_pActInfoVersion(nullptr),
    m_pStatusBar(nullptr),
    m_pLblReqInProgress(nullptr),
    m_pBarReqInProgress(nullptr),
    m_pLblErrors(nullptr),
    m_pWdgtRemCmdServerStatus(nullptr),
    m_pLblRemCmdServerStatusIcon(nullptr),
    m_pWdgtCentral(nullptr),
    m_pDlgRemCmds(nullptr),
    m_pTrcAdminObj(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("MainWindow");

    setWindowTitle(i_strWindowTitle);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::RemCmdServer", "CMainWindow", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( m_pRemCmdServer == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdServer,
        /* szSignal     */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRemCmdServerDestroyed(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdServer,
        /* szSignal     */ SIGNAL(stateChanged(QObject*,int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRemCmdServerStateChanged(QObject*,int)) ) )
    {
        throw CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Menu> File
    //=============

    m_pMnuFile = menuBar()->addMenu(tr("&File"));

    // <MenuItem> File::Quit
    //----------------------

    m_pActFileQuit = new QAction("&Quit",this);
    m_pMnuFile->addAction(m_pActFileQuit);

    if( !connect(
        /* pObjSender   */ m_pActFileQuit,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ qApp,
        /* szSlot       */ SLOT(quit()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> Settings
    //=======================

    m_pMnuSettings = menuBar()->addMenu(tr("&Settings"));

    // <MenuItem> Settings::RemCmdServer
    //------------------------------------

    m_pActSettingsRemCmdServer = new QAction("&Remote Command Server",this);
    m_pMnuSettings->addAction(m_pActSettingsRemCmdServer);

    if( !connect(
        /* pObjSender   */ m_pActSettingsRemCmdServer,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActSettingsRemCmdServerTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Settings::Trace Server
    //-----------------------------------

    m_pActSettingsTraceServer = new QAction("&Trace Server",this);
    m_pMnuSettings->addAction(m_pActSettingsTraceServer);

    if( !connect(
        /* pObjSender   */ m_pActSettingsTraceServer,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActSettingsTraceServerTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Settings::TraceAdminObjPool
    //---------------------------------------

    m_pActSettingsTrcAdminObjIdxTree = new QAction("&Trace Admin Objects",this);
    m_pMnuSettings->addAction(m_pActSettingsTrcAdminObjIdxTree);

    if( !connect(
        /* pObjSender   */ m_pActSettingsTrcAdminObjIdxTree,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActSettingsTrcAdminObjIdxTreeTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Settings::Settings File Info
    //----------------------------------------

    m_pSettingsFile = CApplication::GetInstance()->getSettingsFile();

    if( m_pSettingsFile != nullptr )
    {
        QString strActionSettingsFileInfo = "Settings File: " + m_pSettingsFile->fileName();

        m_pActSettingsFileInfo = new QAction(strActionSettingsFileInfo,this);
        m_pMnuSettings->addAction(m_pActSettingsFileInfo);
    }

    // <Menu> Info
    //=============

    m_pMnuInfo = menuBar()->addMenu(tr("&Info"));

    // <MenuItem> Info::RemCmdObjPool
    //---------------------------------------

    m_pActInfoRemCmdObjPool = new QAction("&Remote Commands",this);
    m_pMnuInfo->addAction(m_pActInfoRemCmdObjPool);

    if( !connect(
        /* pObjSender   */ m_pActInfoRemCmdObjPool,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActInfoRemCmdObjPoolTriggered()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> Info::Error Log
    //----------------------------

    QIcon iconErrorLog;

    QPixmap pxmErrorLog16x16(":/ZS/App/Zeus16x16.bmp");

    pxmErrorLog16x16.setMask(pxmErrorLog16x16.createHeuristicMask());

    iconErrorLog.addPixmap(pxmErrorLog16x16);

    m_pActInfoErrLog = new QAction( iconErrorLog, "Error Log", this );
    m_pActInfoErrLog->setToolTip("Open error log dialog");
    m_pActInfoErrLog->setEnabled(true);

    m_pMnuInfo->addAction(m_pActInfoErrLog);

    if( !QObject::connect(
        /* pObjSender   */ m_pActInfoErrLog,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActInfoErrLogTriggered()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <MenuItem> Info::Request Execution Tree
    //-----------------------------------------

    QIcon iconReqExecTree;

    QPixmap pxmReqExecTree16x16(":/ZS/App/Zeus16x16.bmp");

    pxmReqExecTree16x16.setMask(pxmReqExecTree16x16.createHeuristicMask());

    iconReqExecTree.addPixmap(pxmReqExecTree16x16);

    m_pActInfoRequestExecTree = new QAction( iconReqExecTree, "Request Tree", this );
    m_pActInfoRequestExecTree->setToolTip("Open request execution tree dialog");
    m_pActInfoRequestExecTree->setEnabled(true);

    m_pMnuInfo->addAction(m_pActInfoRequestExecTree);

    if( !QObject::connect(
        /* pObjSender   */ m_pActInfoRequestExecTree,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActInfoRequestExecTreeTriggered()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

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

    QString strStyleSheet = "QStatusBar { background: qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E1E1E1, stop: 0.4 #DDDDDD, stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3); } ";

    m_pStatusBar = new QStatusBar();
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

    // <Label> Remote Command Server Status
    //-------------------------------------

    m_pWdgtRemCmdServerStatus = new QWidget(this);

    new QHBoxLayout(m_pWdgtRemCmdServerStatus);

    m_pWdgtRemCmdServerStatus->setContentsMargins(0,0,0,0);
    m_pWdgtRemCmdServerStatus->layout()->setContentsMargins(1,1,1,1);
    m_pWdgtRemCmdServerStatus->installEventFilter(this);
    //m_pWdgtRemCmdServerStatus->setContextMenuPolicy(Qt::CustomContextMenu);

    //if( !QObject::connect(
    //    /* pObjSender   */ m_pWdgtRemCmdServerStatus,
    //    /* szSignal     */ SIGNAL(customContextMenuRequested(const QPoint&)),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT(onWdgtRemCmdServerStatusContextMenuRequested(const QPoint&)) ) )
    //{
    //    throw CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    //}

    m_pLblRemCmdServerStatusIcon = new QLabel("");
    m_pLblRemCmdServerStatusIcon->setScaledContents(true);
    m_pLblRemCmdServerStatusIcon->setMaximumSize( QSize(16,16) );
    m_pWdgtRemCmdServerStatus->layout()->addWidget(m_pLblRemCmdServerStatusIcon);

    statusBar()->addPermanentWidget(m_pWdgtRemCmdServerStatus);

    onRemCmdServerStateChanged( m_pRemCmdServer, m_pRemCmdServer->getState() );

    // Central Widget
    //===============

    m_pWdgtCentral = new CWidgetCentral();

    setCentralWidget(m_pWdgtCentral);

    // Restore geometry of widget
    //===========================

    QSettings settings;

    restoreGeometry( settings.value(objectName()+"/Geometry").toByteArray() );

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

    try
    {
        delete m_pDlgRemCmds;
    }
    catch(...)
    {
    }
    m_pDlgRemCmds = nullptr;

    CDlgIpcServer::DestroyAllInstances();

    CDialog::DestroyAllInstances();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pSettingsFile = nullptr;
    m_pRemCmdServer = nullptr;
    m_pMnuFile = nullptr;
    m_pActFileQuit = nullptr;
    m_pMnuSettings = nullptr;
    m_pActSettingsRemCmdServer = nullptr;
    m_pActSettingsTraceServer = nullptr;
    m_pActSettingsTrcAdminObjIdxTree = nullptr;
    m_pActSettingsFileInfo = nullptr;
    m_pMnuInfo = nullptr;
    m_pActInfoRemCmdObjPool = nullptr;
    m_pActInfoErrLog = nullptr;
    m_pActInfoRequestExecTree = nullptr;
    m_pActInfoVersion = nullptr;
    m_pStatusBar = nullptr;
    m_pLblReqInProgress = nullptr;
    m_pBarReqInProgress = nullptr;
    m_pLblErrors = nullptr;
    m_pWdgtRemCmdServerStatus = nullptr;
    m_pLblRemCmdServerStatusIcon = nullptr;
    m_pWdgtCentral = nullptr;
    m_pDlgRemCmds = nullptr;
    m_pTrcAdminObj = nullptr;

    s_pThis = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::setDb( CDatabase* i_pDb )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDb",
        /* strAddInfo   */ "" );

    m_pWdgtCentral->setDb(i_pDb);

} // setDb

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

    if( m_pDlgRemCmds != nullptr )
    {
        m_pDlgRemCmds->hide();
    }

    //CDlgIpcServer::HideAllInstances();

    CDialog::HideAllInstances();

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
            onActInfoRequestExecTreeTriggered();
            bHandled = true;
        }
    }
    else if( i_pObjWatched == m_pLblErrors )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            onActInfoErrLogTriggered();
            bHandled = true;
        }
    }
    else if( i_pObjWatched == m_pWdgtRemCmdServerStatus )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            onActSettingsRemCmdServerTriggered();
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
void CMainWindow::onActSettingsRemCmdServerTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActSettingsRemCmdServerTriggered",
        /* strAddInfo   */ "" );

    QString strDlgTitle = windowTitle() + ": Remote Command Server";

    CDlgIpcServer* pDlg = CDlgIpcServer::GetInstance(m_pRemCmdServer->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgIpcServer::CreateInstance(m_pRemCmdServer->objectName(), strDlgTitle);
        pDlg->setServer(m_pRemCmdServer);
        pDlg->adjustSize();
        pDlg->show();
    }
    else
    {
        pDlg->setServer(m_pRemCmdServer);

        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }

} // onActSettingsRemCmdServerTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActSettingsTraceServerTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActSettingsTraceServerTriggered",
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

} // onActSettingsTraceServerTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActSettingsTrcAdminObjIdxTreeTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActSettingsTrcAdminObjIdxTreeTriggered",
        /* strAddInfo   */ "" );

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

} // onActSettingsTrcAdminObjIdxTreeTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActInfoRemCmdObjPoolTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActInfoRemCmdObjPoolTriggered",
        /* strAddInfo   */ "" );

    QString strDlgTitle = getMainWindowTitle() + ": Remote Commands";

    m_pDlgRemCmds = CDlgRemCmds::GetInstance(ESrvCltTypeServer, strDlgTitle);

    if( m_pDlgRemCmds == nullptr )
    {
        m_pDlgRemCmds = CDlgRemCmds::CreateInstance(ESrvCltTypeServer, strDlgTitle);
        m_pDlgRemCmds->setRemCmdServer(m_pRemCmdServer);
        m_pDlgRemCmds->adjustSize();
        m_pDlgRemCmds->show();
    }
    else
    {
        if( m_pDlgRemCmds->isHidden() )
        {
            m_pDlgRemCmds->show();
        }
        m_pDlgRemCmds->raise();
        m_pDlgRemCmds->activateWindow();
    }

} // onActInfoRemCmdObjPoolTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActInfoErrLogTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActInfoErrLogTriggered",
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

} // onActInfoErrLogTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActInfoRequestExecTreeTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActInfoRequestExecTreeTriggered",
        /* strAddInfo   */ "" );

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

} // onActInfoRequestExecTreeTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryAdded( const ZS::System::SErrResultInfo& /*i_errResultInfo*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onErrLogEntryAdded",
        /* strAddInfo   */ "" );

    updateErrorsStatus();

} // onErrLogEntryAdded

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryChanged( const ZS::System::SErrResultInfo& /*i_errResultInfo*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onErrLogEntryChanged",
        /* strAddInfo   */ "" );

    updateErrorsStatus();

} // onErrLogEntryChanged

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryRemoved( const ZS::System::SErrResultInfo& /*i_errResultInfo*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onErrLogEntryRemoved",
        /* strAddInfo   */ "" );

    updateErrorsStatus();

} // onErrLogEntryRemoved

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::updateErrorsStatus()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateErrorsStatus",
        /* strAddInfo   */ "" );

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
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateReqInProgressStatus",
        /* strAddInfo   */ "" );

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
void CMainWindow::onRemCmdServerStateChanged( QObject* i_pRemCmdServer, int i_iState )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pRemCmdServer == nullptr )
        {
            strAddTrcInfo = "RemCmdServer: nullptr";
        }
        else
        {
            strAddTrcInfo = "RemCmdServer: " + i_pRemCmdServer->objectName();
        }
        strAddTrcInfo += ", State: " + CRemCmdServer::State2Str(i_iState);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdServerStateChanged",
        /* strAddInfo   */ strAddTrcInfo );

    QString strStatus;
    QPixmap pxmStatus;
    QString strConnection;
    QString strToolTip;

    Ipc::SServerHostSettings cnctSettings = m_pRemCmdServer->getHostSettings();

    strConnection = cnctSettings.m_strLocalHostName + ":" + QString::number(cnctSettings.m_uLocalPort);
    strToolTip = strConnection;

    if( m_pRemCmdServer->isListening() )
    {
        strStatus = "Server: Running";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleGreen.png");
    }
    else if( m_pRemCmdServer->isStartingUp() )
    {
        strStatus = "Server: Starting Up ...";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleYellow.png");
    }
    else
    {
        strStatus = "Server: Not Running";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleRed.png");
    }

    if( m_pLblRemCmdServerStatusIcon != nullptr )
    {
        m_pLblRemCmdServerStatusIcon->setPixmap(pxmStatus);
        m_pLblRemCmdServerStatusIcon->setToolTip(strToolTip);
    }

} // onRemCmdServerStateChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onRemCmdServerDestroyed( QObject* i_pRemCmdServer )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pRemCmdServer == nullptr )
        {
            strAddTrcInfo = "Thread: nullptr";
        }
        else
        {
            strAddTrcInfo = "Thread: " + i_pRemCmdServer->objectName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRemCmdServerDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pRemCmdServer == m_pRemCmdServer )
    {
        m_pRemCmdServer = nullptr;
    }

} // onRemCmdServerDestroyed
