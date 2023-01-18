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

#ifndef ZSIpcGUI_ClientDlg_h
#define ZSIpcGUI_ClientDlg_h

#include "ZSIpcGUI/ZSIpcGUIDllMain.h"
#include "ZSSysGUI/ZSSysDialog.h"

class QVBoxLayout;

namespace ZS
{
namespace Ipc
{
class CClient;

namespace GUI
{
class CWdgtIpcClient;

//******************************************************************************
class ZSIPCGUIDLL_API CDlgIpcClient : public ZS::System::GUI::CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc::GUI"; }
    static QString ClassName() { return "CDlgIpcClient"; }
public: // class methods
    static CDlgIpcClient* CreateInstance(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wflags = Qt::WindowFlags());
    static CDlgIpcClient* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgIpcClient(
        const QString&  i_strDlgTitle,
        const QString&  i_strObjName,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgIpcClient();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
public: // instance methods
    void setClient( CClient* i_pClient );
public: // instance methods
    void setProtocolTypeImage( const QPixmap& i_pxm );
    void setProtocolTypeImageVisible( bool i_bVisible );
    bool isProtocolTypeImageVisible() const;
protected slots:
    void onSettingsAccepted();
    void onSettingsRejected();
protected slots:
    void onWdgtIpcClientDetailsVisibilityChanged( bool i_bDetailsVisible );
protected: // instance members
    QVBoxLayout*    m_pLyt;
    CWdgtIpcClient* m_pWdgtIpcClient;

}; // class CDlgIpcClient

} // namespace GUI

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpcGUI_ClientDlg_h
