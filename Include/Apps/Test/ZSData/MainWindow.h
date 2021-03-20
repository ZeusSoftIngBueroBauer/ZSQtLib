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

#ifndef ZSApps_TestData_MainWindow_h
#define ZSApps_TestData_MainWindow_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qmainwindow.h>
#else
#include <QtWidgets/qmainwindow.h>
#endif

#include "Test.h"

#include "ZSSys/ZSSysRequest.h"

class QAction;
class QDockWidget;
class QFileDialog;
class QLabel;
class QMenu;
class QProgressBar;
class QSettings;
class QStatusBar;
class QTabWidget;
class QToolBar;

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

namespace Trace
{
namespace GUI
{
class CDlgTrcServer;
class CDlgIdxTreeTrcAdminObjs;
}
}

namespace Data
{
class CDataSet;
class CDb;
class CDbClient;

namespace GUI
{
//class CDlgDbConnection;
//class CDlgDbClientConnection;
class CWdgtDataSet;
}
}

namespace Apps
{
namespace Test
{
namespace Data
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
        Qt::WindowFlags i_wflags = 0 );
    ~CMainWindow();
public: // instance methods
    void setDb( ZS::Data::CDb* i_pDb );
    void setDbDataSet( ZS::Data::CDataSet* i_pDS );
public: // instance methods
    void setDbClt1( ZS::Data::CDbClient* i_pDbClt );
    void setDbClt1DataSet( ZS::Data::CDataSet* i_pDS );
    void setDbClt2( ZS::Data::CDbClient* i_pDbClt );
    void setDbClt2DataSet( ZS::Data::CDataSet* i_pDS );
    void setDbClt3( ZS::Data::CDbClient* i_pDbClt );
    void setDbClt3DataSet( ZS::Data::CDataSet* i_pDS );
protected: // overridables of base class QWidget
    virtual void closeEvent( QCloseEvent* i_pEv );
protected: // overridables of base class QObject
    virtual bool eventFilter( QObject* i_pObjWatched, QEvent* i_pEv );
protected slots:
    void onActFileNewTriggered();
    void onActFileOpenTriggered();
    void onActFileSaveTriggered();
protected slots:
    void onActConnectionsDbSrvTriggered();
    void onActConnectionsDbClt1Triggered();
    void onActConnectionsDbClt2Triggered();
    void onActConnectionsDbClt3Triggered();
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
protected slots:
    void onAppRequestStarted( ZS::System::CRequest* i_pReq );
    void onAppRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected: // instance methods
    void updateReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
private: // class members
    static CMainWindow* s_pThis; // singleton class
    static const QString c_strMenuNameFile;
    static const QString c_strActionNameFileNew;
    static const QString c_strActionNameFileOpen;
    static const QString c_strActionNameFileSave;
    static const QString c_strActionNameFileQuit;
private: // instance members
    QSettings*                   m_pSettingsFile;
    QMenu*                       m_pMnuFile;
    QToolBar*                    m_pToolBarFile;
    QAction*                     m_pActFileNew;
    QAction*                     m_pActFileOpen;
    QAction*                     m_pActFileSave;
    QAction*                     m_pActFileQuit;
    QMenu*                       m_pMnuView;
    QAction*                     m_pActViewDockWdgtDbDataSet;
    QAction*                     m_pActViewDockWdgtDbClt1DataSet;
    QAction*                     m_pActViewDockWdgtDbClt2DataSet;
    QAction*                     m_pActViewDockWdgtDbClt3DataSet;
    QMenu*                       m_pMnuConnections;
    QAction*                     m_pActConnectionsDbSrv;
    QAction*                     m_pActConnectionsDbClt1;
    QAction*                     m_pActConnectionsDbClt2;
    QAction*                     m_pActConnectionsDbClt3;
    QMenu*                       m_pMnuDebug;
    QAction*                     m_pActDebugTrcServer;
    QAction*                     m_pActDebugTrcAdminObjIdxTree;
    QAction*                     m_pActDebugErrLog;
    QAction*                     m_pActDebugRequestExecTree;
    QMenu*                       m_pMnuInfo;
    QAction*                     m_pActInfoVersion;
    QAction*                     m_pActInfoSettingsFile;
    QStatusBar*                  m_pStatusBar;
    QLabel*                      m_pLblReqInProgress;
    QProgressBar*                m_pBarReqInProgress;
    QLabel*                      m_pLblErrors;
    CWidgetCentral*              m_pWdgtCentral;
    QDockWidget*                 m_pDockWdgtDbDataSet;
    ZS::Data::GUI::CWdgtDataSet* m_pWdgtDockDbDataSet;
    QDockWidget*                 m_pDockWdgtDbClt1DataSet;
    ZS::Data::GUI::CWdgtDataSet* m_pWdgtDockDbClt1DataSet;
    QDockWidget*                 m_pDockWdgtDbClt2DataSet;
    ZS::Data::GUI::CWdgtDataSet* m_pWdgtDockDbClt2DataSet;
    QDockWidget*                 m_pDockWdgtDbClt3DataSet;
    ZS::Data::GUI::CWdgtDataSet* m_pWdgtDockDbClt3DataSet;
    QFileDialog*                 m_pDlgFile;
    ZS::Data::CDb*               m_pDb;
    ZS::Data::CDataSet*          m_pDSDb;
    ZS::Data::CDbClient*         m_pDbClt1;
    ZS::Data::CDataSet*          m_pDSDbClt1;
    ZS::Data::CDbClient*         m_pDbClt2;
    ZS::Data::CDataSet*          m_pDSDbClt2;
    ZS::Data::CDbClient*         m_pDbClt3;
    ZS::Data::CDataSet*          m_pDSDbClt3;

}; // class CMainWindow

} // namespace Data

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestData_MainWindow_h
