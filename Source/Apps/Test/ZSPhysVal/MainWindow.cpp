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

#include "MainWindow.h"
#include "WidgetCentral.h"
#include "Test.h"

#include "ZSSysGUI/ZSSysErrLogDlg.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysVersion.h"

#include <QtCore/qsettings.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qlabel.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qstatusbar.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qstatusbar.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CMainWindow : public QMainWindow
*******************************************************************************/

/*==============================================================================
private: // class members
==============================================================================*/

CMainWindow*  CMainWindow::s_pThis = nullptr; // singleton class

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
    m_pActDebugErrLog(nullptr),
    m_pMnuInfo(nullptr),
    m_pActInfoVersion(nullptr),
    m_pStatusBar(nullptr),
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
    //======================

    m_pMnuFile = menuBar()->addMenu(tr("File"));

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

    m_pActFileQuit = m_pMnuFile->addAction(tr("Exit"));
    m_pActFileQuit->setShortcut(Qt::Key_F4 | Qt::ALT);

    QObject::connect(
        m_pActFileQuit, &QAction::triggered,
        qApp, &QApplication::quit);

    // <Menu> Debug
    //=============

    m_pMnuDebug = menuBar()->addMenu(tr("&Debug"));

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

    // <CentralWidget>
    //======================

    m_pWdgtCentral = new CWidgetCentral();
    setCentralWidget(m_pWdgtCentral);

    // <Geometry>
    //======================

    QSettings settings;

    restoreGeometry( settings.value("MainWindow/Geometry").toByteArray() );

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
    m_pActDebugErrLog = nullptr;
    m_pMnuInfo = nullptr;
    m_pActInfoVersion = nullptr;
    m_pStatusBar = nullptr;
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

    if( i_pObjWatched == m_pLblErrors )
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
}

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
