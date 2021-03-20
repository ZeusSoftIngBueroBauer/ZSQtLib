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

#include "ZSIpcTraceGUI/ZSIpcTrcServerDlg.h"
#include "ZSSysGUI/ZSSysErrLogDlg.h"
#include "ZSSysGUI/ZSSysRequestExecTreeDlg.h"
#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeDlg.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;
using namespace ZS::Apps::Products::PCResourceMonitor;


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
    const QString&      i_strWindowTitle,
    CPCResourceMonitor* i_pPCResourceMonitor,
    QWidget*            i_pWdgtParent,
    Qt::WindowFlags     i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent,i_wflags),
    m_pPCResourceMonitor(i_pPCResourceMonitor),
    m_pMnuFile(nullptr),
    m_pActFileQuit(nullptr),
    m_pMnuSettings(nullptr),
    m_pActSettingsFileInfo(nullptr),
    m_pMnuDebug(nullptr),
    m_pActDebugTrcServer(nullptr),
    m_pActDebugTrcAdminObjIdxTree(nullptr),
    m_pActDebugErrLog(nullptr),
    m_pActDebugRequestExecTree(nullptr),
    m_pMnuInfo(nullptr),
    m_pActInfoVersion(nullptr),
    m_pStatusBar(nullptr),
    m_pLblReqInProgress(nullptr),
    m_pBarReqInProgress(nullptr),
    m_pLblErrors(nullptr),
    m_pWdgtCentral(nullptr),
    m_pTrcAdminObj(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }
    s_pThis = this;

    setObjectName("MainWindow");

    setWindowTitle(i_strWindowTitle);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Products::PCResourceMonitor", "CMainWindow", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

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
        /* szSlot       */ SLOT(shutdown()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> Settings
    //================

    m_pMnuSettings = menuBar()->addMenu(tr("&Settings"));

    // <MenuItem> Settings::Settings File Info
    //----------------------------------------

    QSettings* pSettingsFile = CApplication::GetInstance()->getSettingsFile();

    if( pSettingsFile != nullptr )
    {
        QString strActionSettingsFileInfo = "Settings File: " + pSettingsFile->fileName();

        m_pActSettingsFileInfo = new QAction(strActionSettingsFileInfo,this);
        m_pMnuSettings->addAction(m_pActSettingsFileInfo);
    }

    // <Menu> Debug
    //================

    m_pMnuDebug = menuBar()->addMenu(tr("&Debug"));

    // <MenuItem> Debug::TraceServer
    //----------------------------------

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
    //---------------------------------------

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

    QIcon iconErrorLog;

    QPixmap pxmErrorLog16x16(":/ZS/App/Zeus16x16.bmp");

    pxmErrorLog16x16.setMask(pxmErrorLog16x16.createHeuristicMask());

    iconErrorLog.addPixmap(pxmErrorLog16x16);

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

    QPixmap pxmReqExecTree16x16(":/ZS/App/Zeus16x16.bmp");

    pxmReqExecTree16x16.setMask(pxmReqExecTree16x16.createHeuristicMask());

    iconReqExecTree.addPixmap(pxmReqExecTree16x16);

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

    // Central Widget
    //===============

    m_pWdgtCentral = new CWidgetCentral(m_pPCResourceMonitor);

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
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    QSettings settings;

    settings.setValue( "MainWindow/Geometry",saveGeometry());

    CDialog::DestroyAllInstances();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pPCResourceMonitor = nullptr;
    m_pMnuFile = nullptr;
    m_pActFileQuit = nullptr;
    m_pMnuSettings = nullptr;
    m_pActSettingsFileInfo = nullptr;
    m_pMnuDebug = nullptr;
    m_pActDebugTrcServer = nullptr;
    m_pActDebugTrcAdminObjIdxTree = nullptr;
    m_pActDebugErrLog = nullptr;
    m_pActDebugRequestExecTree = nullptr;
    m_pMnuInfo = nullptr;
    m_pActInfoVersion = nullptr;
    m_pStatusBar = nullptr;
    m_pLblReqInProgress = nullptr;
    m_pBarReqInProgress = nullptr;
    m_pLblErrors = nullptr;
    m_pWdgtCentral = nullptr;
    m_pTrcAdminObj = nullptr;

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
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "closeEvent",
        /* strAddInfo   */ "" );

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
void CMainWindow::onActDebugTrcServerTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActDebugTrcServerTriggered",
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

} // onActDebugTrcServerTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugTrcAdminObjIdxTreeTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActDebugTrcAdminObjIdxTreeTriggered",
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

} // onActDebugTrcAdminObjIdxTreeTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActDebugErrLogTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActDebugErrLogTriggered",
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

} // onActDebugErrLogTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugRequestExecTreeTriggered()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActDebugRequestExecTreeTriggered",
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

} // onActDebugRequestExecTreeTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryAdded( const ZS::System::SErrResultInfo& i_errResultInfo )
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
void CMainWindow::onErrLogEntryChanged( const ZS::System::SErrResultInfo& i_errResultInfo )
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
void CMainWindow::onErrLogEntryRemoved( const ZS::System::SErrResultInfo& i_errResultInfo )
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

