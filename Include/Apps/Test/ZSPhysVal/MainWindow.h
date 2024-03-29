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

#ifndef ZSApps_TestPhysVal_MainWindow_h
#define ZSApps_TestPhysVal_MainWindow_h

#include "ZSSys/ZSSysDllMain.h"

#if QT_VERSION < 0x050000
#include <QtGui/qmainwindow.h>
#else
#include <QtWidgets/qmainwindow.h>
#endif

class QAction;
class QLabel;
class QMenu;

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
namespace PhysVal
{
class CTest;
class CWidgetCentral;

//******************************************************************************
class CMainWindow : public QMainWindow
//******************************************************************************
{
public: // class methods
    static CMainWindow* GetInstance() { return s_pThis; } // singleton class
public: // ctors and dtor
    CMainWindow(
        const QString&  i_strWindowTitle,
        CTest*          i_pTest,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    ~CMainWindow();
protected: // overridables of base class QWidget
    virtual void closeEvent( QCloseEvent* i_pEv ) override;
protected: // overridables of base class QObject
    virtual bool eventFilter( QObject* i_pObjWatched, QEvent* i_pEv ) override;
protected slots:
    void onActFileSaveTestResultsTriggered();
    void onActDebugErrLogTriggered();
protected slots:
    void onErrLogEntryAdded( const ZS::System::SErrResultInfo& i_errResultInfo );
    void onErrLogEntryChanged( const ZS::System::SErrResultInfo& i_errResultInfo );
    void onErrLogEntryRemoved( const ZS::System::SErrResultInfo& i_errResultInfo );
protected: // instance methods
    void updateErrorsStatus();
private: // class members
    static CMainWindow* s_pThis; // singleton class
private: // instance members
    CTest*          m_pTest;
    QMenu*          m_pMnuFile;
    QAction*        m_pActFileSaveTestResults;
    QAction*        m_pActFileQuit;
    QMenu*          m_pMnuDebug;
    QAction*        m_pActDebugErrLog;
    QMenu*          m_pMnuInfo;
    QAction*        m_pActInfoVersion;
    QStatusBar*     m_pStatusBar;
    QLabel*         m_pLblErrors;
    CWidgetCentral* m_pWdgtCentral;

}; // class CMainWindow

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_MainWindow_h
