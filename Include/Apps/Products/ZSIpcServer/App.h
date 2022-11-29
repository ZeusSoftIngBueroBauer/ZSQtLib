/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSApps_IpcServer_App_h
#define ZSApps_IpcServer_App_h

#include <QtGui/QGuiApplication>

class QQmlApplicationEngine;
class QQuickWindow;

namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace GUI
{
class CModelErrLog;
}
}
namespace Trace
{
class CIpcTrcServer;
}
namespace Apps
{
namespace Products
{
namespace IpcServer
{
//******************************************************************************
class CApplication : public QGuiApplication
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Products::IpcServer"; }
    static QString ClassName() { return "CApplication"; }
    static CApplication* GetInstance();
public: // ctors and dtor
    CApplication(
        int            i_argc,
        char*          i_argv[],
        const QString& i_strOrganizationName,
        const QString& i_strOrganizationDomain,
        const QString& i_strAppName,
        const QString& i_strAppVersion,
        const QString& i_strWindowTitle );
    ~CApplication();
protected: // instance members
    QQmlApplicationEngine*         m_pQmlAppEngine;
    QQuickWindow*                  m_pMainWindow;
    ZS::System::GUI::CModelErrLog* m_pErrLogModel;
    ZS::Trace::CIpcTrcServer*      m_pTrcServer;
    ZS::System::CTrcAdminObj*      m_pTrcAdminObj;

}; // class CApplication

} // namespace IpcServer

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_IpcServer_App_h
