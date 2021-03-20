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

#ifndef ZSDataGUI_DatabaseClientConnectionWdgt_h
#define ZSDataGUI_DatabaseClientConnectionWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDataGUI/ZSDataGUIDllMain.h"
#include "ZSDataGUI/ZSDatabaseClientTrcMsgLogWdgt.h"
#include "ZSData/ZSDatabaseClient.h"
#include "ZSData/ZSDataMsg.h"
#include "ZSIpc/ZSIpcDllMain.h"

class QCheckBox;
class QComboBox;
class QGroupBox;
class QHostInfo;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableView;
class QTextEdit;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CRequest;

namespace GUI
{
class CProgressBar;
class CSepLine;
}
}

namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDbClient;

namespace GUI
{
class CModelDbConnections;
class CWdgtDbClientTrcMsgLog;

//******************************************************************************
class ZSDATAGUIDLL_API CWdgtDbClientConnection : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::DataGUI::DbClient"; }
    static QString ClassName() { return "CWdgtDbClientConnection"; }
public: // ctors and dtor
    CWdgtDbClientConnection( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtDbClientConnection();
signals:
    void accepted();
    void rejected();
    void detailsVisibilityChanged( bool i_bDetailsVisible );
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setDbClient( CDbClient* i_pDbClient );
protected: // instance methods
    void readSettings();
    void saveSettings();
protected slots:
    void onCmbProtocolTypeCurrentIndexChanged( int i_iIdx );
    void onCmbSocketTypeCurrentIndexChanged( int i_iIdx );
    void onChkDefaultToggled( bool i_bChecked );
protected slots:
    void onEdtRemoteHostNameTextChanged( const QString& i_strText );
    void onEdtRemotePortTextChanged( const QString& i_strText );
    void onEdtConnectTimeoutTextChanged( const QString& i_strText );
    void onEdtBufferSizeTextChanged( const QString& i_strText );
    void onCmbSQLDriversCurrentIndexChanged( int i_iIdx );
protected slots:
    void onChkDatabaseNameUsedToggled( bool i_bChecked );
    void onEdtDatabaseNameTextChanged( const QString& i_strText );
    void onChkUserNameUsedToggled( bool i_bChecked );
    void onEdtUserNameTextChanged( const QString& i_strText );
    void onChkUserPasswordUsedToggled( bool i_bChecked );
    void onEdtUserPasswordTextChanged( const QString& i_strText );
protected slots:
    void onChkWatchDogTimerEnabledToggled( bool i_bChecked );
    void onEdtWatchDogTimerIntervalTextChanged( const QString& i_strText );
    void onEdtWatchDogTimeoutTextChanged( const QString& i_strText );
protected slots:
    void onBtnOkClicked( bool i_bChecked );
    void onBtnCancelClicked( bool i_bChecked );
    void onBtnResetClicked( bool i_bChecked );
    void onBtnApplyClicked( bool i_bChecked );
    void onBtnConnectClicked( bool i_bChecked );
protected slots:
    void onBtnDetailsClicked( bool i_bChecked );
protected slots:
    void onBtnWdgtSrvCltConnectionResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
protected slots:
    void onDbClientStateChanged( ZS::Data::CDbClient* i_pDbClt, ZS::Data::CDbClient::EState i_state );
    void onDbClientRequestInProgressChanged( ZS::Data::CDbClient* i_pDbClt, ZS::System::SRequestDscr i_reqDscr );
protected slots:
    void onDbClientConnected( ZS::Data::CDbClient* i_pClient, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onDbClientDisconnected( ZS::Data::CDbClient* i_pClient, const ZS::Ipc::SSocketDscr& i_socketDscr );
protected slots:
    void onDbClientDefaultProtocolTypeChanged( ZS::Data::CDbClient* i_pClient, ZS::Data::EProtocolType i_protocolType );
    void onDbClientProtocolTypeChanged( ZS::Data::CDbClient* i_pClient, ZS::Data::EProtocolType i_protocolType );
    void onDbClientProtocolTypeRowVersionChanged( ZS::Data::CDbClient* i_pClient );
protected slots:
    void onDbClientDefaultSocketTypeChanged( ZS::Data::CDbClient* i_pClient, ZS::Ipc::ESocketType i_socketType );
    void onDbClientSocketTypeChanged( ZS::Data::CDbClient* i_pClient, ZS::Ipc::ESocketType i_socketType );
    void onDbClientSocketTypeRowVersionChanged( ZS::Data::CDbClient* i_pClient );
protected slots:
    void onDbClientDefaultHostSettingsChanged(
        ZS::Data::CDbClient*                i_pClient,
        ZS::Data::EProtocolType             i_protocolType,
        const ZS::Ipc::SClientHostSettings& i_hostSettings );
    void onDbClientHostSettingsChanged(
        ZS::Data::CDbClient*                i_pClient,
        ZS::Data::EProtocolType             i_protocolType,
        const ZS::Ipc::SClientHostSettings& i_hostSettings );
    void onDbClientHostSettingsRowVersionChanged(
        ZS::Data::CDbClient*    i_pClient,
        ZS::Data::EProtocolType i_protocolType,
        ZS::Ipc::ESocketType    i_socketType );
protected slots:
    void onDbClientDefaultWatchDogSettingsChanged(
        ZS::Data::CDbClient*           i_pClient,
        ZS::Data::EProtocolType        i_protocolType,
        ZS::Ipc::ESocketType           i_socketType,
        const ZS::Ipc::STimerSettings& i_watchDogSettings );
    void onDbClientWatchDogSettingsChanged(
        ZS::Data::CDbClient*           i_pClient,
        ZS::Data::EProtocolType        i_protocolType,
        ZS::Ipc::ESocketType           i_socketType,
        const ZS::Ipc::STimerSettings& i_watchDogSettings );
    void onDbClientWatchDogSettingsRowVersionChanged(
        ZS::Data::CDbClient*    i_pClient,
        ZS::Data::EProtocolType i_protocolType,
        ZS::Ipc::ESocketType    i_socketType );
protected slots: // host info lookup
    void onRemoteHostInfoLookedUp( const QHostInfo& i_old_hostInfo );
protected: // instance methods
    QString getConnectionToolTip();
protected: // instance methods
    void onShowDetailsChanged();
protected: // instance methods
    void resetCnctControls();
    void fillCnctControls( EProtocolType i_protocolType, const Ipc::SClientHostSettings& i_old_hostSettings, const Ipc::STimerSettings& i_watchDogSettings );
    void resetDetailControls();
    void fillDetailControls();
protected: // instance methods
    void applySettings();
protected: // instance members
    CDbClient*                     m_pDbClient;
    QStringList                    m_strlstSQLDrivers;
    int                            m_cxSpacing;
    int                            m_cxLblWidthClm1;
    int                            m_cxEdtWidthClm1;
    int                            m_cxLblWidthClm2;
    int                            m_cxEdtWidthClm2;
    int                            m_cxLblWidthClm3;
    int                            m_cxEdtWidthClm3;
    QVBoxLayout*                   m_pLyt;
    int                            m_idxLytItemStretch;
    // Connection Settings
    EProtocolType                  m_protocolTypeClient;
    EProtocolType                  m_protocolTypeClientDefault;
    EProtocolType                  m_protocolTypeWidget;
    Ipc::SClientHostSettings       m_hostSettingsClient;
    Ipc::SClientHostSettings       m_hostSettingsClientDefault;
    Ipc::SClientHostSettings       m_hostSettingsWidget;
    QHBoxLayout*                   m_pLytLineType;
    QWidget*                       m_pWdgtProtocolType;
    QHBoxLayout*                   m_pLytProtocolType;
    QLabel*                        m_pLblProtocolType;
    QComboBox*                     m_pCmbProtocolType;
    QWidget*                       m_pWdgtSQLDrivers;
    QHBoxLayout*                   m_pLytSQLDrivers;
    QLabel*                        m_pLblSQLDrivers;
    QComboBox*                     m_pCmbSQLDrivers;
    QWidget*                       m_pWdgtSocketType;
    QHBoxLayout*                   m_pLytSocketType;
    QLabel*                        m_pLblSocketType;
    QComboBox*                     m_pCmbSocketType;
    QLabel*                        m_pLblDefault;
    QCheckBox*                     m_pChkDefault;
    QLabel*                        m_pLblState;
    QLabel*                        m_pLedState;
    // Connection Settings
    QHBoxLayout*                   m_pLytLineRemoteHost;
    QWidget*                       m_pWdgtRemoteHostName;;
    QHBoxLayout*                   m_pLytRemoteHostName;;
    QLabel*                        m_pLblRemoteHostName;
    QLineEdit*                     m_pEdtRemoteHostName;
    QWidget*                       m_pWdgtRemoteHostAddr;
    QHBoxLayout*                   m_pLytRemoteHostAddr;
    QLabel*                        m_pLblRemoteHostAddr;
    QLineEdit*                     m_pEdtRemoteHostAddr;
    bool                           m_bRemoteHostInfoLookUpInProcess;
    QWidget*                       m_pWdgtRemotePort;
    QHBoxLayout*                   m_pLytRemotePort;
    QLabel*                        m_pLblRemotePort;
    QLineEdit*                     m_pEdtRemotePort;
    // Connect Timeout and Shm Buffer Size
    QWidget*                       m_pWdgtLineConnectTimeoutBufferSize;
    QHBoxLayout*                   m_pLytLineConnectTimeoutBufferSize;
    QWidget*                       m_pWdgtConnectTimeout;
    QHBoxLayout*                   m_pLytConnectTimeout;
    QLabel*                        m_pLblConnectTimeout_ms;
    QLineEdit*                     m_pEdtConnectTimeout_ms;
    QWidget*                       m_pWdgtBufferSize;
    QHBoxLayout*                   m_pLytBufferSize;
    QLabel*                        m_pLblBufferSize_bytes;
    QLineEdit*                     m_pEdtBufferSize_bytes;
    // User Login
    QWidget*                       m_pWdgtLineUserLogin;
    QHBoxLayout*                   m_pLytLineUserLogin;
    QWidget*                       m_pWdgtDatabaseName;
    QHBoxLayout*                   m_pLytDatabaseName;
    QLabel*                        m_pLblDatabaseName;
    QWidget*                       m_pWdgtEdtDatabaseName;
    QHBoxLayout*                   m_pLytEdtDatabaseName;
    QLineEdit*                     m_pEdtDatabaseName;
    QCheckBox*                     m_pChkDatabaseNameUsed;
    QWidget*                       m_pWdgtUserName;
    QHBoxLayout*                   m_pLytUserName;
    QLabel*                        m_pLblUserName;
    QWidget*                       m_pWdgtEdtUserName;
    QHBoxLayout*                   m_pLytEdtUserName;
    QLineEdit*                     m_pEdtUserName;
    QCheckBox*                     m_pChkUserNameUsed;
    QWidget*                       m_pWdgtUserPassword;
    QHBoxLayout*                   m_pLytUserPassword;
    QLabel*                        m_pLblUserPassword;
    QWidget*                       m_pWdgtEdtUserPassword;
    QHBoxLayout*                   m_pLytEdtUserPassword;
    QLineEdit*                     m_pEdtUserPassword;
    QCheckBox*                     m_pChkUserPasswordUsed;
    // Watch Dog Timer
    Ipc::STimerSettings            m_watchDogSettingsClient;
    Ipc::STimerSettings            m_watchDogSettingsClientDefault;
    Ipc::STimerSettings            m_watchDogSettingsWidget;
    System::GUI::CSepLine*         m_pSepLineWatchDogTimer;
    QWidget*                       m_pWdgtLineWatchDogTimer;
    QHBoxLayout*                   m_pLytLineWatchDogTimer;
    QWidget*                       m_pWdgtWatchDogTimerEnabled;
    QHBoxLayout*                   m_pLytWatchDogTimerEnabled;
    QLabel*                        m_pLblWatchDogTimerEnabled;
    QCheckBox*                     m_pChkWatchDogTimerEnabled;
    QWidget*                       m_pWdgtWatchDogTimerInterval;
    QHBoxLayout*                   m_pLytWatchDogTimerInterval;
    QLabel*                        m_pLblWatchDogTimerInterval_ms;
    QLineEdit*                     m_pEdtWatchDogTimerInterval_ms;
    QWidget*                       m_pWdgtWatchDogTimeout;
    QHBoxLayout*                   m_pLytWatchDogTimeout;
    QLabel*                        m_pLblWatchDogTimeout_ms;
    QLineEdit*                     m_pEdtWatchDogTimeout_ms;
    // Buttons
    QHBoxLayout*                   m_pLytBtns;
    QPushButton*                   m_pBtnOk;
    QPushButton*                   m_pBtnCancel;
    QPushButton*                   m_pBtnReset;
    QPushButton*                   m_pBtnApply;
    QPushButton*                   m_pBtnConnect;
    // Details
    bool                           m_bShowDetails;
    QWidget*                       m_pWdgtDetails;
    QVBoxLayout*                   m_pLytDetails;
    QPushButton*                   m_pBtnDetails;
    QWidget*                       m_pWdgtDetailsStates;
    QVBoxLayout*                   m_pLytDetailsStates;
    QLabel*                        m_pLblDetailsConnectionSettingsApplied;
    QLineEdit*                     m_pEdtDetailsConnectionSettingsApplied;
    QLabel*                        m_pLblDetailsState;
    QLineEdit*                     m_pEdtDetailsState;
    QLabel*                        m_pLblRequestInProgress;
    ZS::System::GUI::CProgressBar* m_pBarRequestInProgress;
    // Active Connection
    QHBoxLayout*                   m_pLytLineConnection;
    QLabel*                        m_pLblActiveConnection;
    QPushButton*                   m_pBtnWdgtSrvCltConnectionResizeRowsAndColumnsToContents;
    QWidget*                       m_pWdgtSrvCltConnection;
    QVBoxLayout*                   m_pLytSrvCltConnection;
    CModelDbConnections*           m_pModelSrvCltConnection;
    QTableView*                    m_pTblViewSrvCltConnection;
    // Trace
    QWidget*                       m_pWdgtMsgLog;
    QVBoxLayout*                   m_pLytMsgLog;
    QLabel*                        m_pLblMsgLog;
    Ipc::GUI::STrcMsgLogSettings   m_trcMsgLogSettings;
    CWdgtDbClientTrcMsgLog*        m_pWdgtTrcMsgLog;
    Trace::CTrcAdminObj*           m_pTrcAdminObj;

}; // class CWdgtDbClientConnection

} // namespace GUI

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSDataGUI_DatabaseClientConnectionWdgt_h
