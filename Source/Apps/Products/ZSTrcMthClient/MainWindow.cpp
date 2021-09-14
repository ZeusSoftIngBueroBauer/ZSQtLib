/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include "ZSIpcTraceGUI/ZSIpcTrcClientDlg.h"
#include "ZSIpcTrace/ZSIpcTrcClient.h"
#include "ZSSysGUI/ZSSysFindTextDlg.h"
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
    m_pActFileQuit(nullptr),
    m_pMnuEdit(nullptr),
    m_pActEditFind(nullptr),
    m_pMnuSettings(nullptr),
    m_pActSettingsTrcClient(nullptr),
    m_pActSettingsTrcAdminObjIdxTree(nullptr),
    m_pMnuInfo(nullptr),
    m_pActInfoVersion(nullptr),
    m_pActInfoSettingsFile(nullptr),
    m_pActInfoErrLog(nullptr),
    m_pActInfoRequestExecTree(nullptr),
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

    if( !QObject::connect(
        /* pObjSender   */ m_pTrcClient,
        /* szSignal     */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTrcClientDestroyed(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTrcClient,
        /* szSignal     */ SIGNAL(stateChanged(QObject*,int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTrcClientStateChanged(QObject*,int)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
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
        /* pObjReceiver */ CApplication::GetInstance(),
        /* szSlot       */ SLOT(quit()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }
    //if( !connect(
    //    /* pObjSender   */ m_pActFileQuit,
    //    /* szSignal     */ SIGNAL(triggered()),
    //    /* pObjReceiver */ CApplication::GetInstance(),
    //    /* szSlot       */ SLOT(shutdown()) ) )
    //{
    //    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    //}

    // <Menu> Edit
    //============

    m_pMnuEdit = menuBar()->addMenu(tr("&Edit"));

    // <MenuItem> Trace Client
    //------------------------

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

    // <MenuItem> Settings::TrcClient
    //----------------------------------

    m_pActSettingsTrcClient = new QAction("&Trace Client",this);
    m_pMnuSettings->addAction(m_pActSettingsTrcClient);

    if( !connect(
        /* pObjSender   */ m_pActSettingsTrcClient,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActSettingsTrcClientTriggered()) ) )
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
    m_pTrcClient = nullptr;
    m_pMnuFile = nullptr;
    m_pActFileQuit = nullptr;
    m_pMnuEdit = nullptr;
    m_pActEditFind = nullptr;
    m_pMnuSettings = nullptr;
    m_pMnuSettings = nullptr;
    m_pActSettingsTrcClient = nullptr;
    m_pActSettingsTrcAdminObjIdxTree = nullptr;
    m_pMnuInfo = nullptr;
    m_pActInfoVersion = nullptr;
    m_pActInfoSettingsFile = nullptr;
    m_pActInfoErrLog = nullptr;
    m_pActInfoRequestExecTree = nullptr;
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
void CMainWindow::onActEditFindTriggered()
//------------------------------------------------------------------------------
{
    if( m_pWdgtCentral != nullptr )
    {
        m_pWdgtCentral->findText();
    }

} // onActEditFindTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActSettingsTrcClientTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Trace Client";

    CDlgTrcClient* pDlg = CDlgTrcClient::GetInstance(CApplication::GetInstance()->getTrcClient()->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgTrcClient::CreateInstance(
            /* strObjName  */ CApplication::GetInstance()->getTrcClient()->objectName(),
            /* strDlgTitle */ strDlgTitle,
            /* pWdgtParent */ nullptr );
        pDlg->setClient(CApplication::GetInstance()->getTrcClient());
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

    } // if( pDlg != nullptr )

} // onActSettingsTrcClientTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActSettingsTrcAdminObjIdxTreeTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Trace Admin Objects";

    CDlgIdxTreeTrcAdminObjs* pDlg = CDlgIdxTreeTrcAdminObjs::GetInstance(m_pTrcClient->getTraceAdminObjIdxTree()->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgIdxTreeTrcAdminObjs::CreateInstance(
            /* pTrcAdmObjPool */ m_pTrcClient->getTraceAdminObjIdxTree(),
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
void CMainWindow::onActInfoErrLogTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Error Log";

    CDlgErrLog* pDlg = dynamic_cast<CDlgErrLog*>(CDlgErrLog::GetInstance("ErrLog"));

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
    updateErrorsStatus();

} // onErrLogEntryAdded

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryChanged( const ZS::System::SErrResultInfo& /*i_errResultInfo*/ )
//------------------------------------------------------------------------------
{
    updateErrorsStatus();

} // onErrLogEntryChanged

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryRemoved( const ZS::System::SErrResultInfo& /*i_errResultInfo*/ )
//------------------------------------------------------------------------------
{
    updateErrorsStatus();

} // onErrLogEntryRemoved

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

//------------------------------------------------------------------------------
void CMainWindow::onTrcClientDestroyed( QObject* i_pTrcClient )
//------------------------------------------------------------------------------
{
    if( i_pTrcClient == m_pTrcClient )
    {
        m_pTrcClient = nullptr;
    }

} // onTrcClientDestroyed

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onWidgetCentralProgressBarConnectDblClicked()
//------------------------------------------------------------------------------
{
    onActSettingsTrcClientTriggered();
}
