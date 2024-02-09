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

#include "MainWindow.h"
#include "App.h"
#include "WidgetCentral.h"

#include "ZSIpcTraceGUI/ZSIpcTrcServerDlg.h"
#include "ZSIpcTraceGUI/ZSIpcTrcServerDlg.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSSysGUI/ZSSysErrLogDlg.h"
#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeDlg.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysVersion.h"

#include <QtCore/qsettings.h>

#if QT_VERSION < 0x050000
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qstatusbar.h>
#else
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qstatusbar.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;
using namespace ZS::Apps::Test::Diagram;


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
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent,i_wflags),
    m_pMnuFile(nullptr),
    m_pActFileQuit(nullptr),
    m_pMnuDebug(nullptr),
    m_pActDebugTrcServer(nullptr),
    m_pActDebugTrcServerAdminObjIdxTree(nullptr),
    m_pActDebugErrLog(nullptr),
    m_pMnuInfo(nullptr),
    m_pActInfoVersion(nullptr),
    m_pActInfoSettingsFile(nullptr),
    m_pStatusBar(nullptr),
    m_pLblErrors(nullptr),
    m_pWdgtTrcServerStatus(nullptr),
    m_pLblTrcServerStatusIcon(nullptr),
    m_pLblTrcServerStatusText(nullptr),
    m_pWdgtCentral(nullptr)
{
    if( s_pThis != nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSingletonClassAlreadyInstantiated);
    }

    setObjectName("MainWindow");

    setWindowTitle(i_strWindowTitle);

    CIpcTrcServer* pTrcServer = CApplication::GetInstance()->getTrcServer();

    if( pTrcServer != nullptr )
    {
        QObject::connect(
            pTrcServer->getIpcServer(), &ZS::Ipc::CServer::stateChanged,
            this, &CMainWindow::onTrcServerStateChanged);
    }

    // <Menu> File
    //=============

    m_pMnuFile = menuBar()->addMenu(tr("&File"));

    // <MenuItem> File::Quit
    //----------------------

    m_pActFileQuit = new QAction("&Quit",this);
    m_pMnuFile->addAction(m_pActFileQuit);

    QObject::connect(
        m_pActFileQuit, &QAction::triggered,
        CApplication::GetInstance(), &CApplication::quit);

    // <Menu> Debug
    //=============

    m_pMnuDebug = menuBar()->addMenu(tr("&Debug"));

    // <MenuItem> Debug::TraceServer
    //-------------------------------

    m_pActDebugTrcServer = new QAction("Trace Server",this);
    m_pMnuDebug->addAction(m_pActDebugTrcServer);

    QObject::connect(
        m_pActDebugTrcServer, &QAction::triggered,
        this, &CMainWindow::onActDebugTrcServerTriggered);

    // <MenuItem> Debug::TraceServerAdminObjPool
    //------------------------------------------

    m_pActDebugTrcServerAdminObjIdxTree = new QAction("Trace Server Admin Objects",this);
    m_pMnuDebug->addAction(m_pActDebugTrcServerAdminObjIdxTree);

    QObject::connect(
        m_pActDebugTrcServerAdminObjIdxTree, &QAction::triggered,
        this, &CMainWindow::onActDebugTrcServerAdminObjIdxTreeTriggered);

    // <MenuItem> Debug::Error Log
    //-----------------------------

    QIcon iconErrorLog;

    QPixmap pxmErrorLog16x16(":/ZS/App/Zeus16x16.png");
    iconErrorLog.addPixmap(pxmErrorLog16x16);

    m_pActDebugErrLog = new QAction( iconErrorLog, "Error Log", this );
    m_pActDebugErrLog->setToolTip("Open error log dialog");
    m_pActDebugErrLog->setEnabled(true);

    m_pMnuDebug->addAction(m_pActDebugErrLog);

    QObject::connect(
        m_pActDebugErrLog, &QAction::triggered,
        this, &CMainWindow::onActDebugErrLogTriggered);

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

    // <Label> Trace Server Status
    //----------------------------

    if( pTrcServer != nullptr )
    {
        m_pWdgtTrcServerStatus = new QWidget(this);

        new QHBoxLayout(m_pWdgtTrcServerStatus);

        m_pWdgtTrcServerStatus->setContentsMargins(0,0,0,0);
        m_pWdgtTrcServerStatus->layout()->setContentsMargins(1,1,1,1);
        m_pWdgtTrcServerStatus->installEventFilter(this);
        //m_pWdgtTrcServerStatus->setContextMenuPolicy(Qt::CustomContextMenu);

        //QObject::connect(
        //    m_pWdgtTrcServerStatus, &QWidget::customContextMenuRequested,
        //    this, &CMainWindow::onWdgtTrcServerStatusContextMenuRequested);

        m_pLblTrcServerStatusIcon = new QLabel("");
        m_pLblTrcServerStatusIcon->setScaledContents(true);
        m_pLblTrcServerStatusIcon->setMaximumSize( QSize(16,16) );
        m_pWdgtTrcServerStatus->layout()->addWidget(m_pLblTrcServerStatusIcon);

        m_pLblTrcServerStatusText = new QLabel("");
        m_pWdgtTrcServerStatus->layout()->addWidget(m_pLblTrcServerStatusText);

        statusBar()->addPermanentWidget(m_pWdgtTrcServerStatus);

        onTrcServerStateChanged(pTrcServer->getIpcServer(), pTrcServer->getState());

    } // if( m_pTrcServer != nullptr )

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
    QSettings settings;

    settings.setValue( objectName()+"/Geometry", saveGeometry() );

    CDialog::DestroyAllInstances();

    m_pMnuFile = nullptr;
    m_pActFileQuit = nullptr;
    m_pMnuDebug = nullptr;
    m_pActDebugTrcServer = nullptr;
    m_pActDebugTrcServerAdminObjIdxTree = nullptr;
    m_pActDebugErrLog = nullptr;
    m_pMnuInfo = nullptr;
    m_pActInfoVersion = nullptr;
    m_pActInfoSettingsFile = nullptr;
    m_pStatusBar = nullptr;
    m_pLblErrors = nullptr;
    m_pWdgtTrcServerStatus = nullptr;
    m_pLblTrcServerStatusIcon = nullptr;
    m_pLblTrcServerStatusText = nullptr;
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

    if( i_pObjWatched == m_pWdgtTrcServerStatus )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            onActDebugTrcServerTriggered();
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
    CDlgTrcServer* pDlg = dynamic_cast<CDlgTrcServer*>(CDlgTrcServer::GetInstance());

    if( pDlg == nullptr )
    {
        QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Trace Server";
        pDlg = CDlgTrcServer::CreateInstance(strDlgTitle);
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
} // onActDebugTrcServerTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugTrcServerAdminObjIdxTreeTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Trace Server Admin Objects";

    CDlgIdxTreeTrcAdminObjs* pDlg = CDlgIdxTreeTrcAdminObjs::GetInstance(CTrcServer::GetTraceAdminObjIdxTree()->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgIdxTreeTrcAdminObjs::CreateInstance(
            /* strDlgTitle     */ strDlgTitle,
            /* pTrcAdmObjPool  */ CTrcServer::GetTraceAdminObjIdxTree(),
            /* pWdgtParent     */ nullptr,
            /* wFlags          */ Qt::WindowFlags() );
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
} // onActDebugTrcServerAdminObjIdxTreeTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActDebugErrLogTriggered()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Error Log";

    CDlgErrLog* pDlg = dynamic_cast<CDlgErrLog*>(CDlgErrLog::GetInstance(strDlgTitle));

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
        strStatus = "Server: Listening";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleGreen.png");
    }
    else
    {
        strStatus = "Server: Not Listening";
        pxmStatus = QPixmap(":/ZS/CnctState/LedCircleRed.png");
    }

    if( m_pLblTrcServerStatusIcon != nullptr )
    {
        m_pLblTrcServerStatusIcon->setPixmap(pxmStatus);
        m_pLblTrcServerStatusIcon->setToolTip(strToolTip);
    }

    if( m_pLblTrcServerStatusText != nullptr )
    {
        m_pLblTrcServerStatusText->setText(strStatus);
        m_pLblTrcServerStatusText->setToolTip(strToolTip);
    }

} // onTrcServerStateChanged
