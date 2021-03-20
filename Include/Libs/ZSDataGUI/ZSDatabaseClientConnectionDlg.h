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

#ifndef ZSDataGUI_DatabaseClientConnectionDlg_h
#define ZSDataGUI_DatabaseClientConnectionDlg_h

#include "ZSDataGUI/ZSDataGUIDllMain.h"
#include "ZSSysGUI/ZSSysDialog.h"

class QVBoxLayout;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDbClient;

namespace GUI
{
class CWdgtDbClientConnection;

//******************************************************************************
class ZSDATAGUIDLL_API CDlgDbClientConnection : public ZS::System::GUI::CDialog
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Data::GUI"; }
    static QString ClassName() { return "CDlgDbClientConnection"; }
public: // class methods
    static CDlgDbClientConnection* CreateInstance(
        const QString&  i_strObjName,
        const QString&  i_strDlgTitle,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags() );
    static CDlgDbClientConnection* GetInstance( const QString& i_strObjName );
protected: // ctor
    CDlgDbClientConnection(
        const QString&  i_strObjName,
        const QString&  i_strDlgTitle,
        QWidget*        i_pWdgtParent = nullptr,
        Qt::WindowFlags i_wFlags = Qt::WindowFlags());
public: // dtor
    virtual ~CDlgDbClientConnection();
public: // overridables of base class ZS::System::GUI::CDialog
    virtual QString nameSpace() override { return NameSpace(); }
    virtual QString className() override { return ClassName(); }
public: // instance methods
    void setDbClient( CDbClient* i_pDbClient );
public: // instance methods
    void show();
    void hide();
protected: // overridables of base class QWidget
    virtual void closeEvent( QCloseEvent* i_pEv );
protected slots:
    void onWdgtCnctAccepted();
    void onWdgtCnctRejected();
protected slots:
    void onWdgtCnctDetailsVisibilityChanged( bool i_bDetailsVisible );
protected: // instance members
    CDbClient*               m_pDbClt;
    QVBoxLayout*             m_pLyt;
    CWdgtDbClientConnection* m_pWdgtCnct;

}; // class CDlgDbClientConnection

} // namespace GUI

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSDataGUI_DatabaseClientConnectionDlg_h
