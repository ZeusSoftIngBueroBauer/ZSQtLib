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
#include <QtGui/qfiledialog.h>
#include <QtGui/qlabel.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qstatusbar.h>
#include <QtGui/qtoolbar.h>
#else
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qstatusbar.h>
#include <QtWidgets/qtoolbar.h>
#endif

#include "MainWindow.h"
#include "App.h"
#include "WidgetCentral.h"

#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSSysGUI/ZSSysErrLogDlg.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Test;
using namespace ZS::Apps::Test::Template;


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
    const QString&  i_strWindowTitle,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QMainWindow(i_pWdgtParent,i_wflags),
    m_pSettingsFile(nullptr),
    m_pMnuFile(nullptr),
    m_pToolBarFile(nullptr),
    m_pActFileNew(nullptr),
    m_pActFileOpen(nullptr),
    m_pActFileSave(nullptr),
    m_pActFileQuit(nullptr),
    m_pMnuDebug(nullptr),
    m_pActDebugErrLog(nullptr),
    m_pMnuInfo(nullptr),
    m_pActInfoVersion(nullptr),
    m_pStatusBar(nullptr),
    m_pLblErrors(nullptr),
    m_pWdgtCentral(nullptr),
    m_pDlgFile(nullptr)
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

    m_pToolBarFile = addToolBar("File Operations");
    m_pToolBarFile->setObjectName("File Operations");
    m_pToolBarFile->setIconSize( QSize(16,16) );

    // <MenuItem> File::New
    //----------------------

    QIcon iconFileNew;

    QPixmap pxmFileNew16x16(":/ZS/Menu/MenuFileNew16x16.bmp");

    pxmFileNew16x16.setMask(pxmFileNew16x16.createHeuristicMask());

    iconFileNew.addPixmap(pxmFileNew16x16);

    m_pActFileNew = new QAction(iconFileNew,"&New ...", this);
    m_pActFileNew->setShortcuts(QKeySequence::New);
    m_pActFileNew->setStatusTip(tr("Create a new test step settings file"));

    m_pMnuFile->addAction(m_pActFileNew);

    m_pToolBarFile->addAction(m_pActFileNew);

    if( !QObject::connect(
        /* pObjSender   */ m_pActFileNew,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActFileNewTriggered()) ) )
    {
        throw CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Open
    //----------------------

    QIcon iconFileOpen;

    QPixmap pxmFileOpen16x16(":/ZS/Menu/MenuFileOpen16x16.bmp");

    pxmFileOpen16x16.setMask(pxmFileOpen16x16.createHeuristicMask());

    iconFileOpen.addPixmap(pxmFileOpen16x16);

    m_pActFileOpen = new QAction(iconFileOpen, "&Open ...", this);
    m_pActFileOpen->setShortcuts(QKeySequence::Open);
    m_pActFileOpen->setStatusTip(tr("Open an existing test step settings file"));

    m_pMnuFile->addAction(m_pActFileOpen);

    m_pToolBarFile->addAction(m_pActFileOpen);

    if( !QObject::connect(
        /* pObjSender   */ m_pActFileOpen,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActFileOpenTriggered()) ) )
    {
        throw CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Save
    //----------------------

    QIcon iconFileSave;

    QPixmap pxmFileSave16x16(":/ZS/Menu/MenuFileSave16x16.bmp");

    pxmFileSave16x16.setMask(pxmFileSave16x16.createHeuristicMask());

    iconFileSave.addPixmap(pxmFileSave16x16);

    m_pActFileSave = new QAction(iconFileSave, "&Save ...", this);
    m_pActFileSave->setShortcuts(QKeySequence::Save);
    m_pActFileSave->setStatusTip(tr("Save test step settings"));

    m_pMnuFile->addAction(m_pActFileSave);

    m_pToolBarFile->addAction(m_pActFileSave);

    if( !QObject::connect(
        /* pObjSender   */ m_pActFileSave,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onActFileSaveTriggered()) ) )
    {
        throw CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <MenuItem> File::Separator
    //---------------------------

    m_pMnuFile->addSeparator();

    // <MenuItem> File::Quit
    //----------------------

    m_pActFileQuit = new QAction("Quit", this);
    m_pActFileQuit->setShortcut(Qt::Key_F4 + Qt::ALT);

    m_pMnuFile->addAction(m_pActFileQuit);

    if( !QObject::connect(
        /* pObjSender   */ m_pActFileQuit,
        /* szSignal     */ SIGNAL(triggered()),
        /* pObjReceiver */ qApp,
        /* szSlot       */ SLOT(quit()) ) )
    {
        throw CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

    // <Menu> Debug
    //=============

    m_pMnuDebug = menuBar()->addMenu(tr("&Debug"));

    // <MenuItem> Debug::Error Log
    //----------------------------

    QIcon iconDebugErrorLog;

    QPixmap pxmDebugErrorLog16x16(":/ZS/App/Zeus16x16.bmp");

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

    // <CentralWidget>
    //======================

    m_pWdgtCentral = new CWidgetCentral(this);
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
    // <Geometry>
    //======================

    QSettings settings;

    settings.setValue( "MainWindow/Geometry", saveGeometry() );
    settings.setValue( "MainWindow/WindowState", saveState() );

    // <Clear>
    //======================

    try
    {
        delete m_pDlgFile;
    }
    catch(...)
    {
    }

    CDialog::DestroyAllInstances();

    m_pSettingsFile = nullptr;
    m_pMnuFile = nullptr;
    m_pToolBarFile = nullptr;
    m_pActFileNew = nullptr;
    m_pActFileOpen = nullptr;
    m_pActFileSave = nullptr;
    m_pActFileQuit = nullptr;
    m_pMnuDebug = nullptr;
    m_pActDebugErrLog = nullptr;
    m_pMnuInfo = nullptr;
    m_pActInfoVersion = nullptr;
    m_pStatusBar = nullptr;
    m_pLblErrors = nullptr;
    m_pWdgtCentral = nullptr;
    m_pDlgFile = nullptr;

    s_pThis = nullptr;

} // dtor

/*==============================================================================
protected: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::closeEvent( QCloseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    if( i_pEv->isAccepted() )
    {
        try
        {
            delete m_pDlgFile;
        }
        catch(...)
        {
        }
        m_pDlgFile = nullptr;

        CDialog::HideAllInstances();

    } // if( i_pEv->isAccepted() )

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

} // eventFilter

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActFileNewTriggered()
//------------------------------------------------------------------------------
{
    if( CApplication::GetInstance()->getTest() != nullptr )
    {
        CTestStepAdminObjPool* pObjPool = CApplication::GetInstance()->getTest()->getAdminObjIdxTree();

        if( pObjPool != nullptr )
        {
            pObjPool->clear();
        }
    }

} // onActFileNewTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActFileOpenTriggered()
//------------------------------------------------------------------------------
{
    if( m_pDlgFile == nullptr )
    {
        m_pDlgFile = new QFileDialog(
            /* pWdgtParent  */ nullptr,
            /* strCaption   */ windowTitle() + ": File Dialog",
            /* strDirectory */ "",
            /* strFilter    */ "*.*" );
    }

    CTestStepAdminObjPool* pObjPool = nullptr;

    if( CApplication::GetInstance()->getTest() != nullptr )
    {
        pObjPool = CApplication::GetInstance()->getTest()->getAdminObjIdxTree();
    }

    if( pObjPool != nullptr )
    {
        QString strFile = pObjPool->getFileName();
        QString strDir;

        if( !strFile.isEmpty() )
        {
            QFileInfo fileInfo(strFile);
            QDir dir = fileInfo.absoluteDir();
            strDir = dir.absoluteFilePath(strFile);
        }

        m_pDlgFile->setWindowTitle( windowTitle() + ": Open Test Step Config File" );
        m_pDlgFile->setAcceptMode(QFileDialog::AcceptOpen);
        //m_pDlgFile->setOption(QFileDialog::DontConfirmOverwrite, false);
        //m_pDlgFile->setConfirmOverwrite(true);
        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        m_pDlgFile->setFilter("*.xml");
        #else
        m_pDlgFile->setNameFilter("*.xml");
        #endif
        m_pDlgFile->setDefaultSuffix("*.xml");
        m_pDlgFile->setDirectory(strDir);

        if( !strFile.isEmpty() )
        {
            m_pDlgFile->selectFile(strFile);
        }

        if( m_pDlgFile->exec() )
        {
            QStringList strlstFiles = m_pDlgFile->selectedFiles();

            if( strlstFiles.size() > 0 )
            {
                strFile = strlstFiles[0];

                if( !strFile.isEmpty() )
                {
                    SErrResultInfo errResultInfo = pObjPool->recall(strFile);

                    if( errResultInfo.isErrorResult() )
                    {
                        QString strText = "Reading test step config from file " + strFile + " failed.";
                        strText += "\n\nErrorCode: " + QString::number(errResultInfo.getResult()) + " (" + errResultInfo.getResultStr() + ")";

                        if( !errResultInfo.getAddErrInfoDscr().isEmpty() )
                        {
                            strText += "\n\n" + errResultInfo.getAddErrInfoDscr();
                        }

                        QMessageBox::critical(
                            /* pWdgtParent */ this,
                            /* strTitle    */ windowTitle() + ": Error Message",
                            /* strText     */ strText,
                            /* buttons     */ QMessageBox::Ok );
                    }
                } // if( strFile.isEmpty() )
            } // if( strlstFiles.size() > 0 )
        } // if( m_pDlgFile->exec() )
    } // if( pObjPool != nullptr )

} // onActFileOpenTriggered

//------------------------------------------------------------------------------
void CMainWindow::onActFileSaveTriggered()
//------------------------------------------------------------------------------
{
    if( m_pDlgFile == nullptr )
    {
        m_pDlgFile = new QFileDialog(
            /* pWdgtParent  */ nullptr,
            /* strCaption   */ windowTitle() + ": File Dialog",
            /* strDirectory */ "",
            /* strFilter    */ "*.*" );
    }

    CTestStepAdminObjPool* pObjPool = nullptr;

    if( CApplication::GetInstance()->getTest() != nullptr )
    {
        pObjPool = CApplication::GetInstance()->getTest()->getAdminObjIdxTree();
    }

    if( pObjPool != nullptr )
    {
        QString strFile = pObjPool->getFileName();
        QString strDir;

        if( !strFile.isEmpty() )
        {
            QFileInfo fileInfo(strFile);
            QDir dir = fileInfo.absoluteDir();
            strDir = dir.absoluteFilePath(strFile);
        }

        m_pDlgFile->setWindowTitle( windowTitle() + ": Save Test Step Config File" );
        m_pDlgFile->setAcceptMode(QFileDialog::AcceptSave);
        //m_pDlgFile->setOption(QFileDialog::DontConfirmOverwrite, false);
        //m_pDlgFile->setConfirmOverwrite(true);
        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        m_pDlgFile->setFilter("*.xml");
        #else
        m_pDlgFile->setNameFilter("*.xml");
        #endif
        m_pDlgFile->setDefaultSuffix("*.xml");
        m_pDlgFile->setDirectory(strDir);

        if( !strFile.isEmpty() )
        {
            m_pDlgFile->selectFile(strFile);
        }

        if( m_pDlgFile->exec() )
        {
            QStringList strlstFiles = m_pDlgFile->selectedFiles();
            QString     strFile;

            if( strlstFiles.size() > 0 )
            {
                strFile = strlstFiles[0];

                if( !strFile.isEmpty() )
                {
                    SErrResultInfo errResultInfo = pObjPool->save(strFile);

                    if( errResultInfo.isErrorResult() )
                    {
                        QString strText = "Saving test step config in file " + strFile + " failed.";
                        strText += "\n\nErrorCode: " + QString::number(errResultInfo.getResult()) + " (" + errResultInfo.getResultStr() + ")";

                        if( !errResultInfo.getAddErrInfoDscr().isEmpty() )
                        {
                            strText += "\n\n" + errResultInfo.getAddErrInfoDscr();
                        }

                        QMessageBox::critical(
                            /* pWdgtParent */ this,
                            /* strTitle    */ windowTitle() + ": Error Message",
                            /* strText     */ strText,
                            /* buttons     */ QMessageBox::Ok );
                    }
                } // if( strFile.isEmpty() )
            } // if( strlstFiles.size() > 0 )
        } // if( m_pDlgFile->exec() )
    } // if( pObjPool != nullptr )

} // onActFileSaveTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onActDebugErrLogTriggered()
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

} // onActDebugErrLogTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryAdded( const ZS::System::SErrResultInfo& )
//------------------------------------------------------------------------------
{
    updateErrorsStatus();
}

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryChanged( const ZS::System::SErrResultInfo& )
//------------------------------------------------------------------------------
{
    updateErrorsStatus();
}

//------------------------------------------------------------------------------
void CMainWindow::onErrLogEntryRemoved( const ZS::System::SErrResultInfo& )
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

