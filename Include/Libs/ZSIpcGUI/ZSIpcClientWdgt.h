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

#ifndef ZSIpcGUI_ClientWdgt_h
#define ZSIpcGUI_ClientWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSIpcGUI/ZSIpcGUIDllMain.h"
#include "ZSIpcGUI/ZSIpcTrcMsgLogWdgt.h"
#include "ZSIpc/ZSIpcClient.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRequest.h"

class QCheckBox;
class QComboBox;
class QGroupBox;
class QHostInfo;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableView;
class QTextEdit;
class QFormLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
namespace GUI
{
class CProgressBar;
}
}
namespace Trace
{
class CTrcAdminObj;
}

namespace Ipc
{
class CClient;
struct SSocketDscr;

namespace GUI
{
class CWdgtTrcMsgLog;

//******************************************************************************
class ZSIPCGUIDLL_API CWdgtIpcClient : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc::GUI"; }
    static QString ClassName() { return "CWdgtIpcClient"; }
public: // ctors and dtor
    CWdgtIpcClient( const QString& i_strObjName, QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtIpcClient();
signals:
    void accepted();
    void rejected();
    void detailsVisibilityChanged( bool i_bDetailsVisible );
public: // instance methods
    void setClient( CClient* i_pClient );
public: // instance methods
    void setClientObjectNameVisible( bool i_bVisible );
    bool isClientObjectNameVisible() const { return m_bClientObjNameVisible; }
public: // instance methods
    void setProtocolTypeImage( const QPixmap& i_pxm );
    void setProtocolTypeImageVisible( bool i_bVisible );
    bool isProtocolTypeImageVisible() const{ return m_bProtocolTypeImageVisible; }
protected: // instance methods
    void readSettings();
    void saveSettings();
protected slots:
    void onCmbSocketTypeCurrentIndexChanged( int i_iIdx );
protected slots:
    void onEdtRemoteHostNameTextChanged( const QString& i_strText );
    void onEdtRemotePortTextChanged( const QString& i_strText );
    void onEdtConnectTimeoutTextChanged( const QString& i_strText );
    void onEdtBufferSizeTextChanged( const QString& i_strText );
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
protected slots: // connected to the signals of the Ipc Client
    void onClientStateChanged( QObject* i_pClient, int i_iState );
    void onClientSettingsChanged( QObject* i_pClient );
    void onClientRequestInProgressChanged( QObject* i_pClient, ZS::System::SRequestDscr i_reqDscr );
protected slots: // host info lookup
    void onRemoteHostInfoLookedUp( const QHostInfo& i_hostInfo );
protected: // instance methods
    QString getConnectionToolTip();
protected: // instance methods
    void onShowDetailsChanged();
protected: // instance methods
    void resetCnctControls();
    void fillCnctControls( const SClientHostSettings& i_hostSettings, const STimerSettings& i_watchDogSettings );
    void resetDetailControls();
    void fillDetailControls();
protected: // instance methods
    void applySettings();
protected: // instance members
    CClient*                        m_pClient;
    bool                            m_bClientObjNameVisible;
    bool                            m_bProtocolTypeImageVisible;
    int                             m_iLblWidth;
    QVBoxLayout*                    m_pLyt;
    QWidget*                        m_pWdgtClientObjName;
    QVBoxLayout*                    m_pLytWdgtClientObjName;
    QHBoxLayout*                    m_pLytLineClientObjName;
    QLabel*                         m_pLblClientObjName;
    QLabel*                         m_pLblClientProtocolTypeImg;
    QLabel*                         m_pLblClientState;
    QLabel*                         m_pLedClientState;
    // Connection Settings
    SClientHostSettings             m_hostSettingsClient;
    SClientHostSettings             m_hostSettingsWidget;
    QHBoxLayout*                    m_pLytLineDefault;
    QFormLayout*                    m_pLytCnct;
    QLabel*                         m_pLblSocketType;
    QComboBox*                      m_pCmbSocketType;
    QLabel*                         m_pLblRemoteHostName;
    QLineEdit*                      m_pEdtRemoteHostName;
    QLabel*                         m_pLblRemoteHostAddr;
    QLineEdit*                      m_pEdtRemoteHostAddr;
    bool                            m_bRemoteHostInfoLookUpInProcess;
    QLabel*                         m_pLblRemotePort;
    QLineEdit*                      m_pEdtRemotePort;
    QLabel*                         m_pLblConnectTimeout_ms;
    QLineEdit*                      m_pEdtConnectTimeout_ms;
    QLabel*                         m_pLblBufferSize_bytes;
    QLineEdit*                      m_pEdtBufferSize_bytes;
    // Watch Dog Timer
    bool                            m_bWatchDogSettingsUsed;
    STimerSettings                  m_watchDogSettingsClient;
    STimerSettings                  m_watchDogSettingsWidget;
    QLabel*                         m_pLblWatchDogTimer;
    QFormLayout*                    m_pLytWatchDogTimer;
    QLabel*                         m_pLblWatchDogTimerEnabled;
    QCheckBox*                      m_pChkWatchDogTimerEnabled;
    QLabel*                         m_pLblWatchDogTimerInterval_ms;
    QLineEdit*                      m_pEdtWatchDogTimerInterval_ms;
    QLabel*                         m_pLblWatchDogTimeout_ms;
    QLineEdit*                      m_pEdtWatchDogTimeout_ms;
    // Buttons
    QHBoxLayout*                    m_pLytBtns;
    QPushButton*                    m_pBtnOk;
    QPushButton*                    m_pBtnCancel;
    QPushButton*                    m_pBtnReset;
    QPushButton*                    m_pBtnApply;
    QPushButton*                    m_pBtnConnect;
    // Details
    bool                            m_bShowDetails;
    QWidget*                        m_pWdgtDetails;
    QVBoxLayout*                    m_pLytDetails;
    QPushButton*                    m_pBtnDetails;
    QWidget*                        m_pWdgtDetailsStates;
    QVBoxLayout*                    m_pLytDetailsStates;
    QLabel*                         m_pLblDetailsConnectionSettingsApplied;
    QLineEdit*                      m_pEdtDetailsConnectionSettingsApplied;
    QLabel*                         m_pLblDetailsState;
    QLineEdit*                      m_pEdtDetailsState;
    QLabel*                         m_pLblRequestInProgress;
    ZS::System::GUI::CProgressBar*  m_pBarRequestInProgress;
    // Active Connection
    QLabel*                         m_pLblLocalHostName;
    QLineEdit*                      m_pEdtLocalHostName;
    QLabel*                         m_pLblLocalPort;
    QLineEdit*                      m_pEdtLocalPort;
    QLabel*                         m_pLblSocketId;
    QLineEdit*                      m_pEdtSocketId;
    int                             m_idxLytItemStretchAtBottom;
    // Trace
    QWidget*                        m_pWdgtMsgLog;
    QVBoxLayout*                    m_pLytMsgLog;
    QLabel*                         m_pLblMsgLog;
    STrcMsgLogSettings              m_trcMsgLogSettings;
    CWdgtTrcMsgLog*                 m_pWdgtTrcMsgLog;
    Trace::CTrcAdminObj*            m_pTrcAdminObj;

}; // class CWdgtIpcClient

} // namespace GUI

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpcGUI_ClientWdgt_h
