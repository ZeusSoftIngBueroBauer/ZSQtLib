/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSIpcLogGUI_ClientDlg_h
#define ZSIpcLogGUI_ClientDlg_h

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
class CWdgtIpcClient;
}
}

namespace Log
{
class CIpcLogClient;

namespace GUI
{
class CWdgtLogSettings;

//******************************************************************************
class ZSIPCLOGGUIDLL_API CDlgLogClient : public ZS::System::GUI::CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Log::GUI"; }
    static QString ClassName() { return "CDlgLogClient"; }
public: // class methods
    static CDlgLogClient* CreateInstance(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgLogClient* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgLogClient(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgLogClient();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
public: // instance methods
    void setClient( CIpcLogClient* i_pLogClient );
protected slots:
    void onIpcClientSettingsAccepted();
    void onIpcClientSettingsRejected();
    void onLogSettingsAccepted();
    void onLogSettingsRejected();
protected slots:
    void onWdgtIpcClientDetailsVisibilityChanged( bool i_bDetailsVisible );
protected: // instance members
    CIpcLogClient*                m_pLogClient;
    QVBoxLayout*                  m_pLyt;
    QTabWidget*                   m_pTabWidget;
    ZS::Ipc::GUI::CWdgtIpcClient* m_pWdgtIpcClient;
    CWdgtLogSettings*             m_pWdgtLogSettings;

}; // class CDlgLogClient

} // namespace GUI

} // namespace Log

} // namespace ZS

#endif // #ifndef ZSIpcLogGUI_ClientDlg_h
