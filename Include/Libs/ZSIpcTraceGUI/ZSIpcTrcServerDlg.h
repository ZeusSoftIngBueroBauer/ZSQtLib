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

#ifndef ZSIpcTraceGUI_ServerDlg_h
#define ZSIpcTraceGUI_ServerDlg_h

#include "ZSIpcTraceGUI/ZSIpcTrcGUIDllMain.h"
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

namespace Trace
{
class CIpcTrcServer;

namespace GUI
{
class CWdgtTrcSettings;

//******************************************************************************
class ZSIPCTRACEGUIDLL_API CDlgTrcServer : public ZS::System::GUI::CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Trace::GUI"; }
    static QString ClassName() { return "CDlgIpcServer"; }
public: // class methods
    static CDlgTrcServer* CreateInstance(
        const QString&  i_strDlgTitle,
        const QString&  i_strTrcServerName = "ZSTrcServer",
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgTrcServer* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgTrcServer(
        const QString&  i_strDlgTitle,
        const QString&  i_strTrcServerName = "ZSTrcServer",
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgTrcServer();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
public: // instance methods
    void setServer( CIpcTrcServer* i_pTrcServer );
protected slots:
    void onSettingsAccepted();
    void onSettingsRejected();
protected slots:
    void onWdgtIpcServerDetailsVisibilityChanged( bool i_bDetailsVisible );
protected: // instance members
    CIpcTrcServer*                m_pIpcTrcServer;
    QVBoxLayout*                  m_pLyt;
    QTabWidget*                   m_pTabWidget;
    ZS::Ipc::GUI::CWdgtIpcServer* m_pWdgtIpcServer;
    CWdgtTrcSettings*             m_pWdgtTrcSettings;

}; // class CDlgTrcServer

} // namespace GUI

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSIpcTraceGUI_ServerDlg_h
