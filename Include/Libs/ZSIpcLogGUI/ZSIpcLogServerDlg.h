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

#ifndef ZSIpcLogGUI_ServerDlg_h
#define ZSIpcLogGUI_ServerDlg_h

#include "ZSIpcLogGUI/ZSIpcLogGUIDllMain.h"
#include "ZSSysGUI/ZSSysDialog.h"

class QTabWidget;
class QVBoxLayout;

namespace ZS
{
namespace Ipc
{
namespace GUI
{
class CWdgtIpcServer;
}
}

namespace Log
{
class CIpcLogServer;

namespace GUI
{
class CWdgtLogSettings;

//******************************************************************************
class ZSIPCLOGGUIDLL_API CDlgLogServer : public ZS::System::GUI::CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Log::GUI"; }
    static QString ClassName() { return "CDlgIpcServer"; }
public: // class methods
    static CDlgLogServer* CreateInstance(
        const QString&  i_strDlgTitle,
        const QString&  i_strLogServerName = "ZSLogServer",
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgLogServer* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgLogServer(
        const QString&  i_strDlgTitle,
        const QString&  i_strLogServerName = "ZSLogServer",
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgLogServer();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
public: // instance methods
    void setServer( CIpcLogServer* i_pLogServer );
protected slots:
    void onIpcServerSettingsAccepted();
    void onIpcServerSettingsRejected();
    void onLogSettingsAccepted();
    void onLogSettingsRejected();
protected slots:
    void onWdgtIpcServerDetailsVisibilityChanged( bool i_bDetailsVisible );
protected: // instance members
    CIpcLogServer*                m_pIpcLogServer;
    QVBoxLayout*                  m_pLyt;
    QTabWidget*                   m_pTabWidget;
    ZS::Ipc::GUI::CWdgtIpcServer* m_pWdgtIpcServer;
    CWdgtLogSettings*             m_pWdgtLogSettings;

}; // class CDlgLogServer

} // namespace GUI

} // namespace Log

} // namespace ZS

#endif // #ifndef ZSIpcLogGUI_ServerDlg_h
