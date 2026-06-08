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

#pragma once

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
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
class CDlgIdxTreeTrcAdminObjs;
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
}
}
} // ZS

namespace ZS::Apps::Products::MethodTraceClient
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
        const QString& i_strWindowTitle,
        ZS::Trace::CIpcTrcClient* i_pTrcClient,
        QWidget* i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    ~CMainWindow();
protected: // overridables of base class QWidget
    virtual void closeEvent(QCloseEvent* i_pEv) override;
    virtual bool eventFilter(QObject* i_pObjWatched, QEvent* i_pEv) override;
protected slots:
    void onActFileRecallAdminObjsTriggered();
    void onActFileSaveAdminObjsTriggered();
    void onActFileReadTrcMthFileTriggered();
    void onActFileWriteTrcMthFileTriggered();
protected slots:
    void onActSettingsWdgtTrcMthListTriggered();
    void onActSettingsTrcClientTriggered();
    void onActSettingsTrcAdminObjIdxTreeTriggered();
protected slots:
    void onActDebugErrLogTriggered();
    void onActDebugRequestExecTreeTriggered();
protected slots:
    void onErrLogEntryAdded(const ZS::System::SErrResultInfo& i_errResultInfo);
    void onErrLogEntryChanged(const ZS::System::SErrResultInfo& i_errResultInfo);
    void onErrLogEntryRemoved(const ZS::System::SErrResultInfo& i_errResultInfo);
protected: // instance methods
    void updateErrorsStatus();
protected: // instance methods
    void updateReqInProgressStatus(const QString& i_strRequest, int i_iProgress_perCent);
protected slots:
    void onTrcClientDestroyed(QObject* i_pTrcClient);
    void onTrcClientConnected(QObject* i_pTrcClient);
    void onTrcClientDisconnected(QObject* i_pTrcClient);
    void onTrcClientStateChanged(QObject* i_pTrcClient, int i_iState);
protected slots:
    void onWidgetCentralProgressBarConnectDblClicked();
private: // class members
    static CMainWindow*       s_pThis; // singleton class
protected: // instance members
    QSettings* m_pSettingsFile = nullptr;
    ZS::Trace::CIpcTrcClient* m_pTrcClient = nullptr;
    QMenu* m_pMnuFile = nullptr;
    QAction* m_pActFileRecallAdminObjs = nullptr;
    QAction* m_pActFileSaveAdminObjs = nullptr;
    QAction* m_pActFileReadTrcMthFile = nullptr;
    QAction* m_pActFileWriteTrcMthFile = nullptr;
    QAction* m_pActFileQuit = nullptr;
    QMenu* m_pMnuSettings = nullptr;
    QAction* m_pActSettingsWdgtTrcMthList = nullptr;
    QAction* m_pActSettingsTrcClient = nullptr;
    QAction* m_pActSettingsTrcAdminObjIdxTree = nullptr;
    QMenu* m_pMnuDebug = nullptr;
    QAction* m_pActDebugErrLog = nullptr;
    QAction* m_pActDebugRequestExecTree = nullptr;
    QMenu* m_pMnuInfo = nullptr;
    QAction* m_pActInfoVersion = nullptr;
    QAction* m_pActInfoSettingsFile = nullptr;
    QStatusBar* m_pStatusBar = nullptr;
    QLabel* m_pLblReqInProgress = nullptr;
    QProgressBar* m_pBarReqInProgress = nullptr;
    QLabel* m_pLblErrors = nullptr;
    QWidget* m_pWdgtTrcClientCnctStatus = nullptr;
    QLabel* m_pLblTrcClientCnctStatusIcon = nullptr;
    CWidgetCentral* m_pWdgtCentral = nullptr;
};

} // ZS::Apps::Products::MethodTraceClient
