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

#ifndef ZSDataGUI_DatabaseConnectionsWdgt_h
#define ZSDataGUI_DatabaseConnectionsWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSDataGUI/ZSDataGUIDllMain.h"
//#include "ZSDataGUI/ZSDatabaseTrcMsgLogWdgt.h"
#include "ZSData/ZSDatabase.h"
//#include "ZSData/ZSDataMsg.h"
#include "ZSIpc/ZSIpcDllMain.h"

class QCheckBox;
class QComboBox;
class QGroupBox;
class QHostInfo;
class QLabel;
class QLineEdit;
class QPushButton;
class QSplitter;
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
}
}
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDb;

namespace GUI
{
class CDelegateDbConnectedNodes;
class CModelDbConnections;
class CWdgtDbTrcMsgLog;

//******************************************************************************
class ZSDATAGUIDLL_API CWdgtDbConnections : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::DataGUI::Db"; }
    static QString ClassName() { return "CWdgtDbConnections"; }
public: // ctors and dtor
    CWdgtDbConnections( QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtDbConnections();
signals:
    void accepted();
    void rejected();
    void detailsVisibilityChanged( bool i_bDetailsVisible );
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setDb( CDb* i_pDb );
protected: // instance methods
    void readSettings();
    void saveSettings();
protected slots:
    void onCmbProtocolTypeCurrentIndexChanged( int i_iIdx );
    void onCmbSocketTypeCurrentIndexChanged( int i_iIdx );
    void onChkDefaultToggled( bool i_bChecked );
protected slots:
    void onEdtListenPortTextChanged( const QString& i_strText );
    void onEdtBufferSizeTextChanged( const QString& i_strText );
protected slots:
    void onChkDatabaseNameUsedToggled( bool i_bChecked );
    void onEdtDatabaseNameTextChanged( const QString& i_strText );
    void onChkUserNameUsedToggled( bool i_bChecked );
    void onEdtUserNameTextChanged( const QString& i_strText );
    void onChkUserPasswordUsedToggled( bool i_bChecked );
    void onEdtUserPasswordTextChanged( const QString& i_strText );
protected slots:
    void onBtnOkClicked( bool i_bChecked );
    void onBtnCancelClicked( bool i_bChecked );
    void onBtnResetClicked( bool i_bChecked );
    void onBtnApplyClicked( bool i_bChecked );
    void onBtnStartupClicked( bool i_bChecked );
protected slots:
    void onBtnDetailsClicked( bool i_bChecked );
protected slots:
    void onBtnWdgtSrvCltConnectionsResizeRowsAndColumnsToContentsClicked( bool i_bChecked );
protected slots: // connected to the signals of the database
    void onDbStateChanged( ZS::Data::CDb* i_pDb, ZS::Data::CDb::EState i_state );
    void onDbRequestInProgressChanged( ZS::Data::CDb* i_pDb, ZS::System::SRequestDscr i_reqDscr );
protected slots: // connected to the signals of the database
    void onDbDefaultHostSettingsChanged(
        ZS::Data::CDb*                      i_pDb,
        ZS::Data::EProtocolType             i_protocolType,
        const ZS::Ipc::SServerHostSettings& i_hostSettings );
    void onDbHostSettingsChanged(
        ZS::Data::CDb*                      i_pDb,
        ZS::Data::EProtocolType             i_protocolType,
        const ZS::Ipc::SServerHostSettings& i_hostSettings );
    void onDbHostSettingsRowVersionChanged(
        ZS::Data::CDb*          i_pDb,
        ZS::Data::EProtocolType i_protocolType,
        ZS::Ipc::ESocketType    i_socketType );
//protected slots: // connected to the signals of the database
//    void onDbClientConnected( QObject* i_pDb, const ZS::Ipc::SSocketDscr& i_socketDscr );
//    void onDbClientDisconnected( QObject* i_pDb, const ZS::Ipc::SSocketDscr& i_socketDscr );
//    void onDbClientRegistered( QObject* i_pDb, const ZS::Data::SDbClientRegSpec& i_regSpec );
protected slots: // host info lookup
    void onLocalHostInfoLookedUp( const QHostInfo& i_old_hostInfo );
protected: // instance methods
    QString getConnectionToolTip();
protected: // instance methods
    void onShowDetailsChanged();
protected: // instance methods
    void resetCnctControls();
    void fillCnctControls( const Ipc::SServerHostSettings& i_old_hostSettings );
    void resetDetailControls();
    void fillDetailControls();
protected: // instance methods
    void applySettings();
protected: // instance members
    CDb*                           m_pDb;
    int                            m_cxSpacing;
    int                            m_cxLblWidthClm1;
    int                            m_cxEdtWidthClm1;
    int                            m_cxLblWidthClm2;
    int                            m_cxEdtWidthClm2;
    int                            m_cxLblWidthClm3;
    int                            m_cxEdtWidthClm3;
    QVBoxLayout*                   m_pLyt;
    // Current widget and server default settings
    EProtocolType                  m_protTypeWidget;
    ZS::Ipc::SServerHostSettings   m_hostSettingsWidget;
    ZS::Ipc::SServerHostSettings   m_hostSettingsServerDefault;
    // Header
    QHBoxLayout*                   m_pLytLineType;
    QWidget*                       m_pWdgtProtocolType;
    QHBoxLayout*                   m_pLytProtocolType;
    QLabel*                        m_pLblProtocolType;
    QComboBox*                     m_pCmbProtocolType;
    QWidget*                       m_pWdgtSocketType;
    QHBoxLayout*                   m_pLytSocketType;
    QLabel*                        m_pLblSocketType;
    QComboBox*                     m_pCmbSocketType;
    QLabel*                        m_pLblDefault;
    QCheckBox*                     m_pChkDefault;
    QLabel*                        m_pLblState;
    QLabel*                        m_pLedState;
    // Connection Settings
    QHBoxLayout*                   m_pLytLineLocalHost;
    QWidget*                       m_pWdgtLocalHostName;
    QHBoxLayout*                   m_pLytLocalHostName;
    QLabel*                        m_pLblLocalHostName;
    QLineEdit*                     m_pEdtLocalHostName;
    QWidget*                       m_pWdgtLocalHostAddr;
    QHBoxLayout*                   m_pLytLocalHostAddr;
    QLabel*                        m_pLblLocalHostAddr;
    QLineEdit*                     m_pEdtLocalHostAddr;
    bool                           m_bLocalHostInfoLookUpInProcess;
    QWidget*                       m_pWdgtListenPort;
    QHBoxLayout*                   m_pLytListenPort;
    QLabel*                        m_pLblListenPort;
    QLineEdit*                     m_pEdtListenPort;
    // Buffer Size (for Shm Socket Types only)
    QWidget*                       m_pWdgtLineBufferSize;
    QHBoxLayout*                   m_pLytLineBufferSize;
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
    // Buttons
    QHBoxLayout*                   m_pLytBtns;
    QPushButton*                   m_pBtnOk;
    QPushButton*                   m_pBtnCancel;
    QPushButton*                   m_pBtnReset;
    QPushButton*                   m_pBtnApply;
    QPushButton*                   m_pBtnStartup;
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
    // Splitter for following tables
    QSplitter*                     m_pSplitterDetails;
    // Active Connections
    QHBoxLayout*                   m_pLytLineConnections;
    QLabel*                        m_pLblActiveConnections;
    QLabel*                        m_pEdtActiveConnections;
    QPushButton*                   m_pBtnWdgtSrvCltConnectionsResizeRowsAndColumnsToContents;
    QWidget*                       m_pWdgtSrvCltConnections;
    QVBoxLayout*                   m_pLytSrvCltConnections;
    CModelDbConnections*           m_pModelSrvCltConnections;
    CDelegateDbConnectedNodes*     m_pDelegateSrvCltConnections;
    QTableView*                    m_pTblViewSrvCltConnections;
    // Protocol Trace
    QWidget*                       m_pWdgtMsgLog;
    QVBoxLayout*                   m_pLytMsgLog;
    QLabel*                        m_pLblMsgLog;
    bool                           m_bTrcMsgLogEnabled;
    unsigned int                   m_uTrcMsgLogMaxItems;
    CWdgtDbTrcMsgLog*              m_pWdgtTrcMsgLog;
    // Stretch at bottom of main layout
    int                            m_idxLytItemStretch;
    // Method Trace
    Trace::CTrcAdminObj*           m_pTrcAdminObj;

}; // class CWdgtDbConnections

} // namespace GUI

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSDataGUI_DatabaseConnectionWdgt_old_h
