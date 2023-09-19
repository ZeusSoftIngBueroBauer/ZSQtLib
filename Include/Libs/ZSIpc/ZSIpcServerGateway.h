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

#ifndef ZSIpc_ServerGateway_h
#define ZSIpc_ServerGateway_h

#include <QtCore/qthread.h>

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcSrvCltBaseGateway.h"

namespace ZS
{
namespace Ipc
{
class CServer;
class CServerGateway;
class CIpcSocketWrapper;

//******************************************************************************
class ZSIPCDLL_API CServerGatewayThread : public CSrvCltBaseGatewayThread
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CServerGatewayThread"; }
public: // ctors and dtor
    CServerGatewayThread(
        const QString&       i_strObjNameGateway,
        CServer*             i_pServer,
        ZS::System::CErrLog* i_pErrLog,
        ZS::System::EMethodTraceDetailLevel i_eTrcMthFileDetailLevel = ZS::System::EMethodTraceDetailLevel::None );
    virtual ~CServerGatewayThread();
protected: // must overridables of base class QThread
    virtual void run() override;
protected: // instance members
    CServer* m_pServer;

}; // class CServerGatewayThread

//******************************************************************************
class ZSIPCDLL_API CServerGateway : public CSrvCltBaseGateway
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CServerGateway"; }
public: // ctors and dtor
    CServerGateway(
        const QString&        i_strObjName,
        CServer*              i_pServer,
        CServerGatewayThread* i_pThreadGateway,
        ZS::System::CErrLog*  i_pErrLog,
        ZS::System::EMethodTraceDetailLevel i_eTrcMthFileDetailLevel = ZS::System::EMethodTraceDetailLevel::None );
    virtual ~CServerGateway();
protected slots: // overridables
    virtual void onNewConnectionPending( QObject* i_pServerWrapper );
protected slots: // must overridables of base class CSrvCltBaseGateway
    virtual void onDisconnected( QObject* i_pSocketWrapper ) override;
    virtual void onReadyRead( QObject* i_pSocketWrapper ) override;
    virtual void onError( QObject* i_pSocketWrapper, ZS::System::SErrResultInfo& i_errResultInfo ) override;
public: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pMsg ) override;
protected: // instance members
    SServerHostSettings         m_hostSettings;
    CIpcServerWrapper*          m_pIpcServerWrapper;
    QVector<CIpcSocketWrapper*> m_arpIpcSocketWrapper;

}; // class CServerGateway

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_ServerGateway_h
