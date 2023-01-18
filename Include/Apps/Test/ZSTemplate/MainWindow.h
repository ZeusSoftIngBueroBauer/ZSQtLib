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

#ifndef ZSApps_TestTemplate_MainWindow_h
#define ZSApps_TestTemplate_MainWindow_h

#include "ZSSys/ZSSysDllMain.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qmainwindow.h>
#else
#include <QtWidgets/qmainwindow.h>
#endif

class QAction;
class QLabel;
class QMenu;
class QSettings;
class QStatusBar;
class QToolBar;

namespace ZS
{
namespace System
{
struct SErrResultInfo;
}

namespace Apps
{
namespace Test
{
namespace Template
{
class CWidgetCentral;

//******************************************************************************
class CMainWindow : public QMainWindow
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static CMainWindow* GetInstance() { return s_pThis; } // singleton class
public: // ctors and dtor
    CMainWindow(
        const QString&  i_strWindowTitle,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    ~CMainWindow();
protected: // overridables of base class QWidget
    virtual void closeEvent( QCloseEvent* i_pEv );
protected: // overridables of base class QObject
    virtual bool eventFilter( QObject* i_pObjWatched, QEvent* i_pEv );
protected slots:
    void onActFileOpenTriggered();
    void onActFileSaveTriggered();
protected slots:
    void onActDebugErrLogTriggered();
    void onActDebugTrcServerTriggered();
    void onActDebugTrcAdminObjIdxTreeTriggered();
protected slots:
    void onErrLogEntryAdded( const ZS::System::SErrResultInfo& i_errResultInfo );
    void onErrLogEntryChanged( const ZS::System::SErrResultInfo& i_errResultInfo );
    void onErrLogEntryRemoved( const ZS::System::SErrResultInfo& i_errResultInfo );
protected: // instance methods
    void updateErrorsStatus();
protected: // class members
    static CMainWindow* s_pThis; // singleton class
protected: // instance members
    QSettings*      m_pSettingsFile;
    QMenu*          m_pMnuFile;
    QToolBar*       m_pToolBarFile;
    QAction*        m_pActFileOpen;
    QAction*        m_pActFileSave;
    QAction*        m_pActFileQuit;
    QMenu*          m_pMnuDebug;
    QAction*        m_pActDebugErrLog;
    QAction*        m_pActDebugTrcServer;
    QAction*        m_pActDebugTrcAdminObjIdxTree;
    QMenu*          m_pMnuInfo;
    QAction*        m_pActInfoVersion;
    QStatusBar*     m_pStatusBar;
    QLabel*         m_pLblErrors;
    CWidgetCentral* m_pWdgtCentral;

}; // class CMainWindow

} // namespace Template

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestTemplate_MainWindow_h
