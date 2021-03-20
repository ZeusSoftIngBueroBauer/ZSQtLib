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

#ifndef ZSApps_RemCmdClient_WidgetCentral_h
#define ZSApps_RemCmdClient_WidgetCentral_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include "ZSRemCmd/ZSRemCmdClient.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysRequestQueue.h"

class QGroupBox;
class QLabel;
class QLineEdit;
class QModelIndex;
class QPushButton;
class QScrollArea;
class QSettings;
class QSplitter;
class QTabWidget;
class QTextEdit;
class QTreeView;
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

namespace RemCmd
{
class CRemCmdClient;

namespace GUI
{
class CWdgtRemCmdObjPool;
}
}

namespace Apps
{
namespace Products
{
namespace RemCmdClient
{
//******************************************************************************
class CWidgetCentral : public QWidget
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWidgetCentral(
        ZS::RemCmd::CRemCmdClient* i_pRemCmdClient,
        QWidget*                   i_pWdgtParent = nullptr,
        Qt::WindowFlags            i_wflags = Qt::WindowFlags());
    ~CWidgetCentral();
signals:
    void editRemoteSettingsTriggered();
public: // instance methods
    void addLogItem( ZS::System::ETransmitDir i_transmitDir, bool i_bBold, const QString& i_strMsg );
protected slots:
    void onBtnConnectClicked( bool i_bChecked );
protected slots: // connected to the signals of the remote command object pool's tree view
    void onTreeViewRemCmdObjPoolExpanded( const QModelIndex& );
protected slots:
    void onRemCmdObjSelected( const QString& i_strCmd, const QString& i_strSyntax, const QString& i_strDscr );
protected slots:
    void onBtnRemCmdExecuteClicked( bool i_bChecked );
protected slots:
    void onBtnLogClearClicked( bool i_bChecked );
protected: // overridables of inherited class QWidget
    virtual void mouseDoubleClickEvent( QMouseEvent* i_pEv );
protected: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pEv );
protected slots: // connected to the signals of the remote command client
    virtual void onRemCmdClientConnected( QObject* i_pRemCmdClient );
    virtual void onRemCmdClientDisconnected( QObject* i_pRemCmdClient );
    virtual void onRemCmdClientSettingsChanged( QObject* i_pRemCmdClient );
    virtual void onRemCmdClientReceivedData( QObject* i_pRemCmdClient, const QByteArray& i_byteArr );
    virtual void onRemCmdClientCommandSent( QObject* i_pRemCmdClient, const QString& i_strCmd, const QByteArray& i_byteArrArgs );
    virtual void onRemCmdClientRequestInProgressChanged( QObject* i_pRemCmdClient, ZS::System::SRequestDscr i_reqDscr );
private: // class members
    static CWidgetCentral*               s_pThis; // singleton class
protected: // instance members
    ZS::RemCmd::CRemCmdClient*           m_pRemCmdClient;
    QVBoxLayout*                         m_pLyt;
    QHBoxLayout*                         m_pLytLineCnct;
    QPushButton*                         m_pBtnConnect;
    ZS::System::GUI::CProgressBar*       m_pProgressBarCnct;
    QSplitter*                           m_pSplitter;
    QSplitter*                           m_pSplitterRemCmd;
    QGroupBox*                           m_pGrpRemCmdObjPool;
    QVBoxLayout*                         m_pLytRemCmdObjPool;
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

}; // class CWidgetCentral

} // namespace RemCmdClient

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_RemCmdClient_WidgetCentral_h
