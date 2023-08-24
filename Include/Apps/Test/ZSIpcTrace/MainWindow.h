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

#ifndef ZSApps_TestIpcTrace_MainWindow_h
#define ZSApps_TestIpcTrace_MainWindow_h

#include "ZSSys/ZSSysDllMain.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
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
class CRequest;
struct SErrResultInfo;
}

namespace Apps
{
namespace Test
{
namespace IpcTrace
{
class CTest;
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
        const QString&  i_strWindowTitle,
        CTest*          i_pTest,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    ~CMainWindow();
protected: // overridables of base class QWidget
    virtual void closeEvent( QCloseEvent* i_pEv );
protected: // overridables of base class QObject
    virtual bool eventFilter( QObject* i_pObjWatched, QEvent* i_pEv );
protected slots:
    void onActFileSaveTestResultsTriggered();
    void onActDebugZSTrcServerTriggered();
    void onActDebugZSTrcServerAdminObjIdxTreeTriggered();
    void onActDebugZSTrcClientTriggered();
    void onActDebugZSTrcClientAdminObjIdxTreeTriggered();
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
protected slots:
    void onTrcServerStateChanged( QObject* i_pServer, int i_iState );
    void onTrcClientStateChanged( QObject* i_pClient, int i_iState );
protected slots:
    void onWidgetCentralProgressBarConnectDblClicked();
private: // class members
    static CMainWindow* s_pThis; // singleton class
private: // instance members
    CTest*          m_pTest;
    QMenu*          m_pMnuFile;
    QAction*        m_pActFileSaveTestResults;
    QAction*        m_pActFileQuit;
    QMenu*          m_pMnuDebug;
    QAction*        m_pActDebugZSTrcServer;
    QAction*        m_pActDebugZSTrcServerAdminObjIdxTree;
    QAction*        m_pActDebugZSTrcClient;
    QAction*        m_pActDebugZSTrcClientAdminObjIdxTree;
    QAction*        m_pActDebugErrLog;
    QAction*        m_pActDebugRequestExecTree;
    QMenu*          m_pMnuInfo;
    QAction*        m_pActInfoVersion;
    QStatusBar*     m_pStatusBar;
    QWidget*        m_pWdgtZSTrcServerStatus;
    QLabel*         m_pLblZSTrcServerStatusIcon;
    QLabel*         m_pLblZSTrcServerStatusText;
    QWidget*        m_pWdgtZSTrcClientStatus;
    QLabel*         m_pLblZSTrcClientStatusIcon;
    QLabel*         m_pLblZSTrcClientStatusText;
    QLabel*         m_pLblReqInProgress;
    QProgressBar*   m_pBarReqInProgress;
    QLabel*         m_pLblErrors;
    CWidgetCentral* m_pWdgtCentral;

}; // class CMainWindow

} // namespace IpcTrace

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestIpcTrace_MainWindow_h
