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

#if QT_VERSION < 0x050000
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qprogressbar.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qstatusbar.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qprogressbar.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qstatusbar.h>
#endif

#include "MainWindow.h"
#include "App.h"
#include "WidgetCentral.h"

#include "ZSIpcTraceGUI/ZSIpcTrcServerDlg.h"
#include "ZSSysGUI/ZSSysErrLogDlg.h"
#include "ZSSysGUI/ZSSysRequestSequencerDlg.h"
#include "ZSSysGUI/ZSSysRequestExecTreeDlg.h"
#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;
using namespace ZS::Apps::Test::AsyncRequests;


/*******************************************************************************
class CMainWindow : public QMainWindow
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

CMainWindow* CMainWindow::s_pThis = nullptr; // singleton class

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMainWindow::CMainWindow(
    const QString&     i_strWindowTitle,
    QSettings*         i_pSettingsFile,
    CRequestSequencer* i_pReqSeqrAppStartup,
    CRequestSequencer* i_pReqSeqrAppShutdown,
    QWidget*           i_pWdgtParent,
    Qt::WindowFlags    i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent,i_wflags),
    m_pSettingsFile(i_pSettingsFile),
    m_pReqSeqrAppStartup(i_pReqSeqrAppStartup),
    m_pReqSeqrAppShutdown(i_pReqSeqrAppShutdown),
    m_pMnuFile(nullptr),
    m_pActFileQuit(nullptr),
    m_pMnuDebug(nullptr),
    m_pActDebugRequestSeqAppStartup(nullptr),
    m_pActDebugRequestSeqAppShutdown(nullptr),
    m_pActDebugRequestExecTree(nullptr),
    m_pActDebugErrLog(nullptr),
    m_pActDebugTrcServer(nullptr),
    m_pMnuInfo(nullptr),
    m_pActInfoVersion(nullptr),
    m_pStatusBar(nullptr),
    m_pLblSettingsFile(nullptr),
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
        /* szSlot       */ SLOT(onLastWindowClosed()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> Debug
    //=============

    m_pMnuDebug = menuBar()->addMenu(tr("&Debug"));

    // <MenuItem> Debug::Request Sequencer App Startup
    //------------------------------------------------

    m_pActDebugRequestSeqAppStartup = new QAction( "Request Sequence System Startup", this );
    m_pActDebugRequestSeqAppStartup->setToolTip("Open request sequence dialog");
    m_pActDebugRequestSeqAppStartup->setEnabled(true);

    m_pMnuDebug->addAction(m_pActDebugRequestSeqAppStartup);

    if( !QObject::connect(
        /* pObjSender   */ m_pActDebugRequestSeqAppStartup,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActDebugRequestSeqAppStartupTriggered()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <MenuItem> Debug::Request Sequencer App Shutdown
    //------------------------------------------------

    m_pActDebugRequestSeqAppShutdown = new QAction( "Request Sequence System Shutdown", this );
    m_pActDebugRequestSeqAppShutdown->setToolTip("Open request sequence dialog");
    m_pActDebugRequestSeqAppShutdown->setEnabled(true);

    m_pMnuDebug->addAction(m_pActDebugRequestSeqAppShutdown);

    if( !QObject::connect(
        /* pObjSender   */ m_pActDebugRequestSeqAppShutdown,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActDebugRequestSeqAppShutdownTriggered()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <MenuItem> Debug::Request Execution Tree
    //-----------------------------------------

    m_pActDebugRequestExecTree = new QAction( "Request Tree", this );
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

    // <MenuItem> Debug::Error Log
    //----------------------------

    QIcon iconDebugErrorLog;

    QPixmap pxmDebugErrorLog16x16(":/ZS/Result/ResultSeveritySuccess.bmp");

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

    // <MenuItem> Debug::Trace Server
    //-------------------------------

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

    QString strStyleSheet =
        "QStatusBar { "
            "background: qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E1E1E1, stop: 0.4 #DDDDDD, stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3); } ";
    //strStyleSheet +=
    //    "QStatusBar::item { "
    //        "border: 1px solid white; "
    //        "border-radius: 3px; } ";

    m_pStatusBar = new QStatusBar;
    m_pStatusBar->setStyleSheet(strStyleSheet);
    setStatusBar(m_pStatusBar);

    // <Label> Settings File
    //------------------------

    m_pLblSettingsFile = new QLabel( "SettingsFile: " + m_pSettingsFile->fileName() );
    m_pStatusBar->addWidget(m_pLblSettingsFile);

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

    if( !QObject::connect(
        /* pObjSender   */ CApplication::GetInstance(),
        /* szSignal     */ SIGNAL(requestStarted(ZS::System::CRequest*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAppRequestStarted(ZS::System::CRequest*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

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

    m_pWdgtCentral = new CWidgetCentral();
    setCentralWidget(m_pWdgtCentral);

    // Restore geometry of widget
    //===========================

    QSettings settings;

    restoreGeometry( settings.value("MainWindow/Geometry").toByteArray() );

} // ctor

//------------------------------------------------------------------------------
CMainWindow::~CMainWindow()
//------------------------------------------------------------------------------
{
    // Save position and size of main window
    //--------------------------------------

    QSettings settings;

    settings.setValue( "MainWindow/Geometry", saveGeometry() );

    // Reset instance members
    //-----------------------

    CDialog::DestroyAllInstances();

    m_pSettingsFile = nullptr;
    m_pReqSeqrAppStartup = nullptr;
    m_pReqSeqrAppShutdown = nullptr;
    m_pMnuFile = nullptr;
    m_pActFileQuit = nullptr;
    m_pMnuDebug = nullptr;
    m_pActDebugRequestSeqAppStartup = nullptr;
    m_pActDebugRequestSeqAppShutdown = nullptr;
    m_pActDebugRequestExecTree = nullptr;
    m_pActDebugErrLog = nullptr;
    m_pActDebugTrcServer = nullptr;
    m_pMnuInfo = nullptr;
    m_pActInfoVersion = nullptr;
    m_pStatusBar = nullptr;
    m_pLblSettingsFile = nullptr;
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
void CMainWindow::onActDebugRequestSeqAppStartupTriggered()
//------------------------------------------------------------------------------
{
    if( m_pReqSeqrAppStartup != nullptr )
    {
        QString strDlgTitle = getMainWindowTitle() + ": Startup Request Sequencer";

        CDlgRequestSequencer* pDlg = dynamic_cast<CDlgRequestSequencer*>(CDlgRequestSequencer::GetInstance(strDlgTitle));

        if( pDlg == nullptr )
        {
            pDlg = CDlgRequestSequencer::CreateInstance(objectName() + ".Startup", strDlgTitle);
            pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
            pDlg->setSequencer(m_pReqSeqrAppStartup);
            pDlg->adjustSize();
            pDlg->show();
        }
        else // if( pDlg != nullptr )
        {
            pDlg->setSequencer(m_pReqSeqrAppStartup);

            if( pDlg->isHidden() )
            {
                pDlg->show();
            }
            pDlg->raise();
            pDlg->activateWindow();

        } // if( pDlg != nullptr )
    } // if( m_pReqSeqrAppStartup != nullptr )

} // onActDebugRequestSeqAppStartupTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugRequestSeqAppShutdownTriggered()
//------------------------------------------------------------------------------
{
    if( m_pReqSeqrAppShutdown != nullptr )
    {
        QString strDlgTitle = getMainWindowTitle() + ": Shutdown Request Sequencer";

        CDlgRequestSequencer* pDlg = dynamic_cast<CDlgRequestSequencer*>(CDlgRequestSequencer::GetInstance(strDlgTitle));

        if( pDlg == nullptr )
        {
            pDlg = CDlgRequestSequencer::CreateInstance(objectName() + ".Shutdown", strDlgTitle);
            pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
            pDlg->setSequencer(m_pReqSeqrAppShutdown);
            pDlg->adjustSize();
            pDlg->show();
        }
        else // if( pReqSeq != nullptr )
        {
            pDlg->setSequencer(m_pReqSeqrAppShutdown);

            if( pDlg->isHidden() )
            {
                pDlg->show();
            }
            pDlg->raise();
            pDlg->activateWindow();

        } // if( pDlg != nullptr )
    } // if( m_pReqSeqrAppShutdown != nullptr )

} // onActDebugRequestSeqAppShutdownTriggered

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
void CMainWindow::onActDebugTrcServerTriggered()
//------------------------------------------------------------------------------
{
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

} // onActDebugTrcServerTriggered

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

    if( m_pActDebugErrLog != nullptr )
    {
        m_pActDebugErrLog->setIcon( getErrPixmap(severityMax) );
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
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
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
