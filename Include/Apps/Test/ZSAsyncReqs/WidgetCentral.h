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

#ifndef ZSApps_TestAsynRequests_WidgetCentral_h
#define ZSApps_TestAsynRequests_WidgetCentral_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRequest.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QProgressBar;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CRequestSequencer;
}

namespace Apps
{
namespace Test
{
namespace AsyncRequests
{
class CDbThread;
class CDbClient;
class CDbClientThread;

//******************************************************************************
class CWidgetCentral : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static CWidgetCentral* GetInstance() { return s_pThis; } // singleton class
public: // ctors and dtor
    CWidgetCentral( QWidget* i_pWdgtParent = nullptr, Qt::WindowFlags i_wflags = Qt::WindowFlags());
    ~CWidgetCentral();
protected slots:
    void onAppStateChanged( int i_iStateCurr, int i_iStatePrev );
    void onAppRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onDbStateChanged( int i_iStateCurr, int i_iStatePrev );
    void onDbRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onDbCltGUIStateChanged( int i_iStateCurr, int i_iStatePrev );
    void onDbCltGUIRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onDbCltSigGenStateChanged( int i_iStateCurr, int i_iStatePrev );
    void onDbCltSigGenRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onDbCltCaptureStateChanged( int i_iStateCurr, int i_iStatePrev );
    void onDbCltCaptureRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onBtnAppStartupClicked( bool i_bChecked = false );
    void onBtnAppShutdownClicked( bool i_bChecked = false );
protected slots:
    void onChkDbUseBlockingMethodCallsStateChanged( int i_iCheckState );
    void onChkDbModeSingleShotStateChanged( int i_iCheckState );
    void onBtnDbExecuteSingleShotClicked( bool i_bChecked = false );
    void onCmbDbTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbStartupClicked( bool i_bChecked = false );
    void onBtnDbShutdownClicked( bool i_bChecked = false );
protected slots:
    void onChkDbCltGUIUseBlockingMethodCallsStateChanged( int i_iCheckState );
    void onCmbDbCltGUIConnectTypeCurrentIndexChanged( int i_iIdx );
    void onCmbDbCltGUIConnectTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltGUIConnectClicked( bool i_bChecked = false );
    void onBtnDbCltGUIDisconnectClicked( bool i_bChecked = false );
    void onCmbDbCltGUIRegisterTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltGUIRegisterClicked( bool i_bChecked = false );
    void onBtnDbCltGUIUnregisterClicked( bool i_bChecked = false );
    void onCmbDbCltGUISchemaTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltGUISelectSchemaClicked( bool i_bChecked = false );
    void onBtnDbCltGUIUpdateSchemaClicked( bool i_bChecked = false );
    void onCmbDbCltGUIDataTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltGUISelectDataClicked( bool i_bChecked = false );
    void onBtnDbCltGUIUpdateDataClicked( bool i_bChecked = false );
protected slots:
    void onChkDbCltSigGenUseBlockingMethodCallsStateChanged( int i_iCheckState );
    void onCmbDbCltSigGenConnectTypeCurrentIndexChanged( int i_iIdx );
    void onCmbDbCltSigGenStartupTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltSigGenStartupClicked( bool i_bChecked = false );
    void onBtnDbCltSigGenShutdownClicked( bool i_bChecked = false );
    void onCmbDbCltSigGenConnectTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltSigGenConnectClicked( bool i_bChecked = false );
    void onBtnDbCltSigGenDisconnectClicked( bool i_bChecked = false );
    void onCmbDbCltSigGenRegisterTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltSigGenRegisterClicked( bool i_bChecked = false );
    void onBtnDbCltSigGenUnregisterClicked( bool i_bChecked = false );
    void onCmbDbCltSigGenSchemaTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltSigGenSelectSchemaClicked( bool i_bChecked = false );
    void onBtnDbCltSigGenUpdateSchemaClicked( bool i_bChecked = false );
    void onCmbDbCltSigGenDataTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltSigGenSelectDataClicked( bool i_bChecked = false );
    void onBtnDbCltSigGenUpdateDataClicked( bool i_bChecked = false );
protected slots:
    void onChkDbCltCaptureUseBlockingMethodCallsStateChanged( int i_iCheckState );
    void onCmbDbCltCaptureConnectTypeCurrentIndexChanged( int i_iIdx );
    void onCmbDbCltCaptureStartupTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltCaptureStartupClicked( bool i_bChecked = false );
    void onBtnDbCltCaptureShutdownClicked( bool i_bChecked = false );
    void onCmbDbCltCaptureConnectTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltCaptureConnectClicked( bool i_bChecked = false );
    void onBtnDbCltCaptureDisconnectClicked( bool i_bChecked = false );
    void onCmbDbCltCaptureRegisterTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltCaptureRegisterClicked( bool i_bChecked = false );
    void onBtnDbCltCaptureUnregisterClicked( bool i_bChecked = false );
    void onCmbDbCltCaptureSchemaTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltCaptureSelectSchemaClicked( bool i_bChecked = false );
    void onBtnDbCltCaptureUpdateSchemaClicked( bool i_bChecked = false );
    void onCmbDbCltCaptureDataTimeoutCurrentIndexChanged( int i_iIdx );
    void onBtnDbCltCaptureSelectDataClicked( bool i_bChecked = false );
    void onBtnDbCltCaptureUpdateDataClicked( bool i_bChecked = false );
protected: // instane methods
    void updateAppReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
protected: // instane methods
    void updateDbReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
protected: // instane methods
    void updateDbCltGUIConnectReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
    void updateDbCltGUIRegisterReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
    void updateDbCltGUISchemaReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
    void updateDbCltGUIDataReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
protected: // instane methods
    void updateDbCltSigGenStartupReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
    void updateDbCltSigGenConnectReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
    void updateDbCltSigGenRegisterReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
    void updateDbCltSigGenSchemaReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
    void updateDbCltSigGenDataReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
protected: // instane methods
    void updateDbCltCaptureStartupReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
    void updateDbCltCaptureConnectReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
    void updateDbCltCaptureRegisterReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
    void updateDbCltCaptureSchemaReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
    void updateDbCltCaptureDataReqInProgressStatus( const QString& i_strRequest, int i_iProgress_perCent );
protected: // overridables of base class QObject
    virtual bool eventFilter( QObject* i_pObjWatched, QEvent* i_pEv );
protected: // class members
    static CWidgetCentral* s_pThis; // singleton class
protected: // instance members
    QVBoxLayout*  m_pLyt;
    // Application
    QHBoxLayout*  m_pLytApp;
    QLabel*       m_pLblApp;
    QPushButton*  m_pBtnAppStartup;
    QPushButton*  m_pBtnAppShutdown;
    QLabel*       m_pLblAppReqInProgress;
    QProgressBar* m_pBarAppReqInProgress;
    QLabel*       m_pLedAppState;
    // Database
    QHBoxLayout*  m_pLytDb;
    QLabel*       m_pLblDb;
    QLabel*       m_pLblDbUseBlockingMethodCalls;
    QCheckBox*    m_pChkDbUseBlockingMethodCalls;
    QHBoxLayout*  m_pLytDbBtnsLine;
    QLabel*       m_pLblDbTimeout;
    QComboBox*    m_pCmbDbTimeout;
    QPushButton*  m_pBtnDbStartup;
    QPushButton*  m_pBtnDbShutdown;
    QLabel*       m_pLblDbReqInProgress;
    QProgressBar* m_pBarDbReqInProgress;
    QLabel*       m_pLedDbState;
    QHBoxLayout*  m_pLytDbSingleShotsLine;
    QLabel*       m_pLblDbModeSingleShot;
    QCheckBox*    m_pChkDbModeSingleShot;
    QPushButton*  m_pBtnDbExecuteSingleShot;
    // DbCltGUI
    QHBoxLayout*  m_pLytDbCltGUI;
    QLabel*       m_pLblDbCltGUI;
    QLabel*       m_pLblDbCltGUIUseBlockingMethodCalls;
    QCheckBox*    m_pChkDbCltGUIUseBlockingMethodCalls;
    QLabel*       m_pLblDbCltGUIConnectType;
    QComboBox*    m_pCmbDbCltGUIConnectType;
    QHBoxLayout*  m_pLytDbCltGUIConnectBtnsLine;
    QLabel*       m_pLblDbCltGUIConnectTimeout;
    QComboBox*    m_pCmbDbCltGUIConnectTimeout;
    QPushButton*  m_pBtnDbCltGUIConnect;
    QPushButton*  m_pBtnDbCltGUIDisconnect;
    QLabel*       m_pLblDbCltGUIConnectReqInProgress;
    QProgressBar* m_pBarDbCltGUIConnectReqInProgress;
    QLabel*       m_pLedDbCltGUIConnectState;
    QHBoxLayout*  m_pLytDbCltGUIRegisterBtnsLine;
    QLabel*       m_pLblDbCltGUIRegisterTimeout;
    QComboBox*    m_pCmbDbCltGUIRegisterTimeout;
    QPushButton*  m_pBtnDbCltGUIRegister;
    QPushButton*  m_pBtnDbCltGUIUnregister;
    QLabel*       m_pLblDbCltGUIRegisterReqInProgress;
    QProgressBar* m_pBarDbCltGUIRegisterReqInProgress;
    QHBoxLayout*  m_pLytDbCltGUISchemaBtnsLine;
    QLabel*       m_pLblDbCltGUISchemaTimeout;
    QComboBox*    m_pCmbDbCltGUISchemaTimeout;
    QPushButton*  m_pBtnDbCltGUISelectSchema;
    QPushButton*  m_pBtnDbCltGUIUpdateSchema;
    QLabel*       m_pLblDbCltGUISchemaReqInProgress;
    QProgressBar* m_pBarDbCltGUISchemaReqInProgress;
    QHBoxLayout*  m_pLytDbCltGUIDataBtnsLine;
    QLabel*       m_pLblDbCltGUIDataTimeout;
    QComboBox*    m_pCmbDbCltGUIDataTimeout;
    QPushButton*  m_pBtnDbCltGUISelectData;
    QPushButton*  m_pBtnDbCltGUIUpdateData;
    QLabel*       m_pLblDbCltGUIDataReqInProgress;
    QProgressBar* m_pBarDbCltGUIDataReqInProgress;
    // DbCltSigGen
    QHBoxLayout*  m_pLytDbCltSigGen;
    QLabel*       m_pLblDbCltSigGen;
    QLabel*       m_pLblDbCltSigGenUseBlockingMethodCalls;
    QCheckBox*    m_pChkDbCltSigGenUseBlockingMethodCalls;
    QLabel*       m_pLblDbCltSigGenConnectType;
    QComboBox*    m_pCmbDbCltSigGenConnectType;
    QHBoxLayout*  m_pLytDbCltSigGenStartupBtnsLine;
    QLabel*       m_pLblDbCltSigGenStartupTimeout;
    QComboBox*    m_pCmbDbCltSigGenStartupTimeout;
    QPushButton*  m_pBtnDbCltSigGenStartup;
    QPushButton*  m_pBtnDbCltSigGenShutdown;
    QLabel*       m_pLblDbCltSigGenStartupReqInProgress;
    QProgressBar* m_pBarDbCltSigGenStartupReqInProgress;
    QHBoxLayout*  m_pLytDbCltSigGenConnectBtnsLine;
    QLabel*       m_pLblDbCltSigGenConnectTimeout;
    QComboBox*    m_pCmbDbCltSigGenConnectTimeout;
    QPushButton*  m_pBtnDbCltSigGenConnect;
    QPushButton*  m_pBtnDbCltSigGenDisconnect;
    QLabel*       m_pLblDbCltSigGenConnectReqInProgress;
    QProgressBar* m_pBarDbCltSigGenConnectReqInProgress;
    QLabel*       m_pLedDbCltSigGenState;
    QHBoxLayout*  m_pLytDbCltSigGenRegisterBtnsLine;
    QLabel*       m_pLblDbCltSigGenRegisterTimeout;
    QComboBox*    m_pCmbDbCltSigGenRegisterTimeout;
    QPushButton*  m_pBtnDbCltSigGenRegister;
    QPushButton*  m_pBtnDbCltSigGenUnregister;
    QLabel*       m_pLblDbCltSigGenRegisterReqInProgress;
    QProgressBar* m_pBarDbCltSigGenRegisterReqInProgress;
    QHBoxLayout*  m_pLytDbCltSigGenSchemaBtnsLine;
    QLabel*       m_pLblDbCltSigGenSchemaTimeout;
    QComboBox*    m_pCmbDbCltSigGenSchemaTimeout;
    QPushButton*  m_pBtnDbCltSigGenSelectSchema;
    QPushButton*  m_pBtnDbCltSigGenUpdateSchema;
    QLabel*       m_pLblDbCltSigGenSchemaReqInProgress;
    QProgressBar* m_pBarDbCltSigGenSchemaReqInProgress;
    QHBoxLayout*  m_pLytDbCltSigGenDataBtnsLine;
    QLabel*       m_pLblDbCltSigGenDataTimeout;
    QComboBox*    m_pCmbDbCltSigGenDataTimeout;
    QPushButton*  m_pBtnDbCltSigGenSelectData;
    QPushButton*  m_pBtnDbCltSigGenUpdateData;
    QLabel*       m_pLblDbCltSigGenDataReqInProgress;
    QProgressBar* m_pBarDbCltSigGenDataReqInProgress;
    // DbCltCapture
    QHBoxLayout*  m_pLytDbCltCapture;
    QLabel*       m_pLblDbCltCapture;
    QLabel*       m_pLblDbCltCaptureUseBlockingMethodCalls;
    QCheckBox*    m_pChkDbCltCaptureUseBlockingMethodCalls;
    QLabel*       m_pLblDbCltCaptureConnectType;
    QComboBox*    m_pCmbDbCltCaptureConnectType;
    QHBoxLayout*  m_pLytDbCltCaptureStartupBtnsLine;
    QLabel*       m_pLblDbCltCaptureStartupTimeout;
    QComboBox*    m_pCmbDbCltCaptureStartupTimeout;
    QPushButton*  m_pBtnDbCltCaptureStartup;
    QPushButton*  m_pBtnDbCltCaptureShutdown;
    QLabel*       m_pLblDbCltCaptureStartupReqInProgress;
    QProgressBar* m_pBarDbCltCaptureStartupReqInProgress;
    QHBoxLayout*  m_pLytDbCltCaptureConnectBtnsLine;
    QLabel*       m_pLblDbCltCaptureConnectTimeout;
    QComboBox*    m_pCmbDbCltCaptureConnectTimeout;
    QPushButton*  m_pBtnDbCltCaptureConnect;
    QPushButton*  m_pBtnDbCltCaptureDisconnect;
    QLabel*       m_pLblDbCltCaptureConnectReqInProgress;
    QProgressBar* m_pBarDbCltCaptureConnectReqInProgress;
    QLabel*       m_pLedDbCltCaptureState;
    QHBoxLayout*  m_pLytDbCltCaptureRegisterBtnsLine;
    QLabel*       m_pLblDbCltCaptureRegisterTimeout;
    QComboBox*    m_pCmbDbCltCaptureRegisterTimeout;
    QPushButton*  m_pBtnDbCltCaptureRegister;
    QPushButton*  m_pBtnDbCltCaptureUnregister;
    QLabel*       m_pLblDbCltCaptureRegisterReqInProgress;
    QProgressBar* m_pBarDbCltCaptureRegisterReqInProgress;
    QHBoxLayout*  m_pLytDbCltCaptureSchemaBtnsLine;
    QLabel*       m_pLblDbCltCaptureSchemaTimeout;
    QComboBox*    m_pCmbDbCltCaptureSchemaTimeout;
    QPushButton*  m_pBtnDbCltCaptureSelectSchema;
    QPushButton*  m_pBtnDbCltCaptureUpdateSchema;
    QLabel*       m_pLblDbCltCaptureSchemaReqInProgress;
    QProgressBar* m_pBarDbCltCaptureSchemaReqInProgress;
    QHBoxLayout*  m_pLytDbCltCaptureDataBtnsLine;
    QLabel*       m_pLblDbCltCaptureDataTimeout;
    QComboBox*    m_pCmbDbCltCaptureDataTimeout;
    QPushButton*  m_pBtnDbCltCaptureSelectData;
    QPushButton*  m_pBtnDbCltCaptureUpdateData;
    QLabel*       m_pLblDbCltCaptureDataReqInProgress;
    QProgressBar* m_pBarDbCltCaptureDataReqInProgress;

}; // class CWidgetCentral

} // namespace AsyncRequests

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestAsynRequests_WidgetCentral_h
