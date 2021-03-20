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

#ifndef ZSApps_PCResourceMonitor_MainWindow_h
#define ZSApps_PCResourceMonitor_MainWindow_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qmainwindow.h>
#else
#include <QtWidgets/qmainwindow.h>
#endif

class QAction;
class QLabel;
class QMenu;
class QProgressBar;
class QSettings;
class QStatusBar;

namespace ZS
{
namespace System
{
struct SErrResultInfo;

namespace GUI
{
class CDlgErrLog;
class CDlgRequestExecTree;
}
}

namespace Trace
{
class CTrcAdminObj;

namespace GUI
{
class CDlgTrcServer;
class CDlgIdxTreeTrcAdminObjs;
}
}

namespace Apps
{
namespace Products
{
namespace PCResourceMonitor
{
class CPCResourceMonitor;
class CWidgetCentral;

//******************************************************************************
class CMainWindow : public QMainWindow
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static CMainWindow* GetInstance(); // singleton class
public: // ctors and dtor
    CMainWindow(
        const QString&      i_strWindowTitle,
        CPCResourceMonitor* i_pPCResourceMonitor,
        QWidget*            i_pWdgtParent = nullptr,
        Qt::WindowFlags     i_wflags = 0 );
    ~CMainWindow();
protected: // overridables of base class QWidget
    virtual void closeEvent( QCloseEvent* i_pEv );
protected: // overridables of base class QObject
    virtual bool eventFilter( QObject* i_pObjWatched, QEvent* i_pEv );
protected slots:
    void onActDebugTrcServerTriggered();
    void onActDebugTrcAdminObjIdxTreeTriggered();
    void onActDebugErrLogTriggered();
    void onActDebugRequestExecTreeTriggered();
protected slots:
    void onErrLogEntryAdded( const ZS::System::SErrResultInfo& i_errResultInfo );
    void onErrLogEntryChanged( const ZS::System::SErrResultInfo& i_errResultInfo );
    void onErrLogEntryRemoved( const ZS::System::SErrResultInfo& i_errResultInfo );
protected: // instance methods
    void updateErrorsStatus();
protected: // instance methods
    void updateReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
private: // class members
    static CMainWindow*       s_pThis; // singleton class
protected: // instance members
    CPCResourceMonitor*       m_pPCResourceMonitor;
    QMenu*                    m_pMnuFile;
    QAction*                  m_pActFileQuit;
    QMenu*                    m_pMnuSettings;
    QAction*                  m_pActSettingsFileInfo;
    QMenu*                    m_pMnuDebug;
    QAction*                  m_pActDebugTrcServer;
    QAction*                  m_pActDebugTrcAdminObjIdxTree;
    QAction*                  m_pActDebugErrLog;
    QAction*                  m_pActDebugRequestExecTree;
    QMenu*                    m_pMnuInfo;
    QAction*                  m_pActInfoVersion;
    QStatusBar*               m_pStatusBar;
    QLabel*                   m_pLblReqInProgress;
    QProgressBar*             m_pBarReqInProgress;
    QLabel*                   m_pLblErrors;
    CWidgetCentral*           m_pWdgtCentral;
    ZS::Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CMainWindow

} // namespace PCResourceMonitor

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_PCResourceMonitor_MainWindow_h
