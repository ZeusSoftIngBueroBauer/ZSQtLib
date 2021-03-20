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

#ifndef ZSRemCmdGUI_DlgRemCmds_h
#define ZSRemCmdGUI_DlgRemCmds_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qdialog.h>
#else
#include <QtWidgets/qdialog.h>
#endif

#include "ZSRemCmdGUI/ZSRemCmdGUIDllMain.h"
#include "ZSRemCmd/ZSRemCmdServer.h"

class QGroupBox;
class QLabel;
class QLineEdit;
class QModelIndex;
class QPushButton;
class QSplitter;
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

namespace RemCmd
{
class CRemCmdClient;
class CRemCmdServer;
class CRemCmdObjPool;

namespace GUI
{
class CWdgtRemCmdObjPool;

//******************************************************************************
class ZSREMCMDGUIDLL_API CDlgRemCmds : public QDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::RemCmd::GUI"; }
    static QString ClassName() { return "CDlgRemCmds"; }
public: // class methods
    static CDlgRemCmds* GetInstance( ZS::Ipc::ESrvCltType i_srvCltType, const QString& i_strDlgTitle );
    static CDlgRemCmds* CreateInstance( ZS::Ipc::ESrvCltType i_srvCltType, const QString& i_strDlgTitle, QWidget* i_pWdgtParent = nullptr );
    static void DestroyInstance( ZS::Ipc::ESrvCltType i_srvCltType, const QString& i_strDlgTitle );
    static void DestroyInstance( CDlgRemCmds* i_pDlg );
    static void DestroyAllInstances( ZS::Ipc::ESrvCltType i_srvCltType = ZS::Ipc::ESrvCltTypeCount );
    static void HideAllInstances( ZS::Ipc::ESrvCltType i_srvCltType = ZS::Ipc::ESrvCltTypeCount );
protected: // ctors
    CDlgRemCmds(
        ZS::Ipc::ESrvCltType i_srvCltType,
        const QString&       i_strDlgTitle,
        QWidget*             i_pWdgtParent = nullptr );
public: // dtor
    virtual ~CDlgRemCmds();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setRemCmdObjPool( CRemCmdObjPool* i_pRemCmdObjPool );
public: // instance methods
    void setRemCmdClient( CRemCmdClient* i_pRemCmdClient );
    void setRemCmdServer( CRemCmdServer* i_pRemCmdServer );
public: // instance methods
    void show();
    void hide();
protected: // overridables of base class QWidget
    virtual void closeEvent( QCloseEvent* i_pEv );
public: // instance methods
    void addLogItem( ZS::System::ETransmitDir i_transmitDir, bool i_bBold, const QString& i_strMsg );
protected: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pEv );
protected slots:
    void onBtnConnectClicked( bool i_bChecked );
    void onTreeViewRemCmdObjPoolExpanded( const QModelIndex& i_modelIdx );
    void onRemCmdObjSelected( const QString& i_strCmd, const QString& i_strSyntax, const QString& i_strDscr );
    void onBtnRemCmdExecuteClicked();
    void onBtnLogClearClicked( bool i_bChecked );
protected slots: // connected to the signals of the remote command server or client
    void onRemCmdSrvCltDestroyed( QObject* i_pRemCmdSrvClt );
    void onRemCmdSrvCltSettingsChanged( QObject* i_pRemCmdSrvClt );
    void onRemCmdSrvCltRequestInProgressChanged( QObject* i_pRemCmdSrvClt, ZS::System::SRequestDscr i_reqDscr );
protected slots: // connected to the signals of the remote command server
    void onRemCmdServerConnected( QObject* i_pRemCmdServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onRemCmdServerDisconnected( QObject* i_pRemCmdServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onRemCmdServerReceivedData( QObject* i_pRemCmdServer, int i_iSocketId, const QByteArray& i_byteArrData );
    void onRemCmdServerDataSent( QObject* i_pRemCmdServer, int i_iSocketId, const QByteArray& i_byteArrData );
protected slots: // connected to the signals of the remote command client
    void onRemCmdClientConnected( QObject* i_pRemCmdClient );
    void onRemCmdClientDisconnected( QObject* i_pRemCmdClient );
    void onRemCmdClientReceivedData( QObject* i_pRemCmdClient, const QByteArray& i_byteArrData );
    void onRemCmdClientCommandSent( QObject* i_pRemCmdClient, const QString& i_strCmd, const QByteArray& i_byteArrArgs );
protected: // class members
    static QHash<QString,CDlgRemCmds*> s_arhshInstances[ZS::Ipc::ESrvCltTypeCount]; // Key is title of dialog
protected: // instance members
    ZS::Ipc::ESrvCltType                 m_srvCltType;
    ZS::RemCmd::CRemCmdClient*           m_pRemCmdClient;
    ZS::RemCmd::CRemCmdServer*           m_pRemCmdServer;
    ZS::RemCmd::CRemCmdObjPool*          m_pRemCmdObjPool;
    QVBoxLayout*                         m_pLyt;
    QHBoxLayout*                         m_pLytLineCnct;
    QPushButton*                         m_pBtnConnect;
    ZS::System::GUI::CProgressBar*       m_pProgressBarCnct;
    QSplitter*                           m_pSplitter;
    QGroupBox*                           m_pGrpRemCmdObjPool;
    QVBoxLayout*                         m_pLytRemCmdObjPool;
    QSplitter*                           m_pSplitterRemCmd;
    ZS::RemCmd::GUI::CWdgtRemCmdObjPool* m_pWdgtRemCmdObjPool;
    QWidget*                             m_pWdgtRemCmd;
    QVBoxLayout*                         m_pLytRemCmd;
    QHBoxLayout*                         m_pLytLineRemCmd;
    QLabel*                              m_pLblRemCmd;
    QLineEdit*                           m_pEdtRemCmd;
    QPushButton*                         m_pBtnRemCmdExecute;
    QHBoxLayout*                         m_pLytLineRemCmdSyntax;
    QLabel*                              m_pLblRemCmdSyntax;
    QLineEdit*                           m_pEdtRemCmdSyntax;
    QTextEdit*                           m_pEdtRemCmdDscr;
    QGroupBox*                           m_pGrpLog;
    QVBoxLayout*                         m_pLytLog;
    QTextEdit*                           m_pEdtLog;
    QHBoxLayout*                         m_pLytLineLogBtns;
    QPushButton*                         m_pBtnLogClear;
    ZS::Trace::CTrcAdminObj*             m_pTrcAdminObj;

}; // class CDlgRemCmds

} // namespace GUI

} // namespace RemCmd

} // namespace ZS

#endif // #ifndef ZSRemCmdGUI_DlgRemCmds_h
