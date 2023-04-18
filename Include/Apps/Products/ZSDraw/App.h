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

#ifndef ZSApps_Draw_App_h
#define ZSApps_Draw_App_h

#include <QtCore/qglobal.h>

#include "ZSSysGUI/ZSSysGUIApp.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysRequestQueue.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;
class CIpcTrcServer;
}

namespace Apps
{
namespace Products
{
namespace Draw
{
class CMainWindow;
class CTest;

//******************************************************************************
class CApplication : public ZS::System::GUI::CGUIApp
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "Apps::Products::Draw"; }
    static QString ClassName() { return "CApplication"; }
public: // class methods
    static CApplication* GetInstance();
public: // ctors and dtor
    CApplication(
        int            i_argc,
        char*          i_argv[],
        const QString& i_strOrganizationName,
        const QString& i_strOrganizationDomain,
        const QString& i_strAppName );
    ~CApplication();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void createAndShowMainWindow(
        const QString&     i_strMainWindowTitle,
        const QString&     i_strFileName,
        const QStringList& i_strlstObjFactories);
protected slots: // instance methods of system shutdown
    void onLastWindowClosed();
protected: // instance members
    ZS::Trace::CIpcTrcServer* m_pTrcServer;
    CTest* m_pTest;
    CMainWindow* m_pMainWindow;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CApplication

} // namespace Draw

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_Draw_App_h
