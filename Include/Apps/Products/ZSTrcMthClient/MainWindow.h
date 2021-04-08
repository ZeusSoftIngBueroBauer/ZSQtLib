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

#ifndef ZSApps_TrcMthClient_MainWindow_h
#define ZSApps_TrcMthClient_MainWindow_h

#include "ZSSys/ZSSysDllMain.h"

#if QT_VERSION < 0x050000
#include <QtGui/qmainwindow.h>
#else
#include <QtWidgets/qmainwindow.h>
#endif

class QAction;
class QLabel;
class QMenu;
class QProgressBar;
class QStatusBar;
class QSettings;

namespace ZS
{
namespace System
{
class CRequest;
struct SErrResultInfo;

namespace GUI
{
class CDlgErrLog;
class CDlgRequestExecTree;
}
}
namespace Ipc
{
namespace GUI
{
class CDlgIpcClient;
}
}

namespace Trace
{
class CIpcTrcClient;

namespace GUI
{
class CDlgTrcClient;
class CDlgIdxTreeTrcAdminObjs;
}
}

namespace Apps
{
namespace Products
{
namespace MethodTraceClient
{
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
        const QString&            i_strWindowTitle,
        ZS::Trace::CIpcTrcClient* i_pTrcClient,
        QWidget*                  i_pWdgtParent = nullptr,
        Qt::WindowFlags           i_wflags = Qt::WindowFlags());
    ~CMainWindow();
protected: // overridables of base class QWidget
    virtual void closeEvent( QCloseEvent* i_pEv );
protected: // overridables of base class QObject
    virtual bool eventFilter( QObject* i_pObjWatched, QEvent* i_pEv );
protected slots:
    void onActEditFindTriggered();
protected slots:
    void onActSettingsTrcClientTriggered();
    void onActSettingsTrcAdminObjIdxTreeTriggered();
protected slots:
    void onActInfoErrLogTriggered();
    void onActInfoRequestExecTreeTriggered();
protected slots:
    void onErrLogEntryAdded( const ZS::System::SErrResultInfo& i_errResultInfo );
    void onErrLogEntryChanged( const ZS::System::SErrResultInfo& i_errResultInfo );
    void onErrLogEntryRemoved( const ZS::System::SErrResultInfo& i_errResultInfo );
protected: // instance methods
    void updateErrorsStatus();
protected: // instance methods
    void updateReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
protected slots:
    void onTrcClientStateChanged( QObject* i_pTrcClient, int i_iState );
    void onTrcClientDestroyed( QObject* i_pTrcClient );
protected slots:
    void onWidgetCentralProgressBarConnectDblClicked();
private: // class members
    static CMainWindow*       s_pThis; // singleton class
protected: // instance members
    QSettings*                m_pSettingsFile;
    ZS::Trace::CIpcTrcClient* m_pTrcClient;
    QMenu*                    m_pMnuFile;
    QAction*                  m_pActFileQuit;
    QMenu*                    m_pMnuEdit;
    QAction*                  m_pActEditFind;
    QMenu*                    m_pMnuSettings;
    QAction*                  m_pActSettingsTrcClient;
    QAction*                  m_pActSettingsTrcAdminObjIdxTree;
    QMenu*                    m_pMnuInfo;
    QAction*                  m_pActInfoVersion;
    QAction*                  m_pActInfoSettingsFile;
    QAction*                  m_pActInfoErrLog;
    QAction*                  m_pActInfoRequestExecTree;
    QStatusBar*               m_pStatusBar;
    QLabel*                   m_pLblReqInProgress;
    QProgressBar*             m_pBarReqInProgress;
    QLabel*                   m_pLblErrors;
    QWidget*                  m_pWdgtTrcClientCnctStatus;
    QLabel*                   m_pLblTrcClientCnctStatusIcon;
    CWidgetCentral*           m_pWdgtCentral;

}; // class CMainWindow

} // namespace MethodTraceClient

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TrcMthClient_MainWindow_h
