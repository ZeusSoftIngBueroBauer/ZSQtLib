/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSIpcGUI_ServerWdgt_h
#define ZSIpcGUI_ServerWdgt_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSIpcGUI/ZSIpcGUIDllMain.h"
#include "ZSIpcGUI/ZSIpcTrcMsgLogWdgt.h"
#include "ZSIpc/ZSIpcServer.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRequest.h"

class QCheckBox;
class QComboBox;
class QGroupBox;
class QHostInfo;
class QLabel;
class QLineEdit;
class QProgressBar;
class QPushButton;
class QSplitter;
class QTableView;
class QTextEdit;
class QGridLayout;
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
class CServer;
struct SSocketDscr;

namespace GUI
{
class CModelSrvCltConnection;
class CWdgtTrcMsgLog;

//******************************************************************************
class ZSIPCGUIDLL_API CWdgtIpcServer : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc::GUI"; }
    static QString ClassName() { return "CWdgtIpcServer"; }
public: // ctors and dtor
    CWdgtIpcServer( const QString& i_strServerName = "IpcServer", QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtIpcServer();
signals:
    void accepted();
    void rejected();
    void detailsVisibilityChanged( bool i_bDetailsVisible );
public: // instance methods
    void setServer( CServer* i_pServer );
public: // instance methods
    void setServerObjectNameVisible( bool i_bVisible );
    bool isServerObjectNameVisible() const { return m_bServerObjNameVisible; }
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
    void onEdtLocalPortTextChanged( const QString& i_strText );
    void onEdtMaxPendingConnectionsTextChanged( const QString& i_strText );
    void onEdtBufferSizeTextChanged( const QString& i_strText );
protected slots:
    void onBtnOkClicked( bool i_bChecked );
    void onBtnCancelClicked( bool i_bChecked );
    void onBtnResetClicked( bool i_bChecked );
    void onBtnApplyClicked( bool i_bChecked );
    void onBtnStartupClicked( bool i_bChecked );
protected slots:
    void onBtnDetailsClicked( bool i_bChecked );
protected slots: // connected to the signals of the Ipc Server
    void onIpcServerConnected( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onIpcServerDisconnected( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onIpcServerStateChanged( QObject* i_pServer, int i_iState );
    void onIpcServerSettingsChanged( QObject* i_pServer );
    void onIpcServerRequestInProgressChanged( QObject* i_pServer, ZS::System::SRequestDscr i_reqDscr );
protected slots: // host info lookup
    void onHostInfoLookedUp( const QHostInfo& i_hostInfo );
protected: // instance methods
    QString getConnectionToolTip();
protected: // instance methods
    //void onSocketTypeChanged( ESocketType i_socketType );
protected: // instance methods
    void onShowDetailsChanged();
protected: // instance methods
    void resetCnctControls();
    void fillCnctControls( const SServerHostSettings& i_cnctSettings );
    void fillDetailControls();
    void resetDetailControls();
protected: // instance methods
    void applySettings();
protected:
    CServer*                        m_pServer;
    bool                            m_bServerObjNameVisible;
    bool                            m_bProtocolTypeImageVisible;
    int                             m_iLblWidth;
    QVBoxLayout*                    m_pLyt;
    QWidget*                        m_pWdgtServerObjName;
    QVBoxLayout*                    m_pLytWdgtServerObjName;
    QHBoxLayout*                    m_pLytLineServerObjName;
    QLabel*                         m_pLblServerObjName;
    QLabel*                         m_pLblServerProtocolTypeImg;
    QLabel*                         m_pLblServerState;
    QLabel*                         m_pLedServerState;
    // Connection Settings
    SServerHostSettings             m_hostSettingsServer;
    SServerHostSettings             m_hostSettingsWidget;
    QHBoxLayout*                    m_pLytLineDefault;
    QFormLayout*                    m_pLytCnct;
    QLabel*                         m_pLblSocketType;
    QComboBox*                      m_pCmbSocketType;
    QLabel*                         m_pLblLocalHostName;
    QLineEdit*                      m_pEdtLocalHostName;
    QLabel*                         m_pLblLocalHostAddr;
    QLineEdit*                      m_pEdtLocalHostAddr;
    bool                            m_bHostInfoLookUpInProcess;
    QLabel*                         m_pLblLocalPort;
    QLineEdit*                      m_pEdtLocalPort;
    QLabel*                         m_pLblMaxPendingConnections;
    QLineEdit*                      m_pEdtMaxPendingConnections;
    QLabel*                         m_pLblBufferSize_bytes;
    QLineEdit*                      m_pEdtBufferSize_bytes;
    // Buttons
    QHBoxLayout*                    m_pLytBtns;
    QPushButton*                    m_pBtnOk;
    QPushButton*                    m_pBtnCancel;
    QPushButton*                    m_pBtnReset;
    QPushButton*                    m_pBtnApply;
    QPushButton*                    m_pBtnStartup;
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
    // Splitter for following tables
    QSplitter*                      m_pSplitterDetails;
    // Active Connections
    QHBoxLayout*                    m_pLytLineConnections;
    QLabel*                         m_pLblActiveConnections;
    QLabel*                         m_pEdtActiveConnections;
    QLabel*                         m_pLblArrLenConnections;
    QLabel*                         m_pEdtArrLenConnections;
    QWidget*                        m_pWdgtSrvCltConnections;
    QVBoxLayout*                    m_pLytSrvCltConnections;
    CModelSrvCltConnection*         m_pModelSrvCltConnection;
    QTableView*                     m_pTblViewSrvCltConnection;
    int                             m_idxLytItemStretchAtBottom;
    // Trace
    QWidget*                        m_pWdgtMsgLog;
    QVBoxLayout*                    m_pLytMsgLog;
    QLabel*                         m_pLblMsgLog;
    STrcMsgLogSettings              m_trcMsgLogSettings;
    CWdgtTrcMsgLog*                 m_pWdgtTrcMsgLog;
    Trace::CTrcAdminObj*            m_pTrcAdminObj;

}; // class CWdgtIpcServer

} // namespace GUI

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpcGUI_ServerWdgt_h
