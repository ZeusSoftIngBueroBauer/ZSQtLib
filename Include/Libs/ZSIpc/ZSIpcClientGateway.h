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

#ifndef ZSIpc_ClientGateway_h
#define ZSIpc_ClientGateway_h

#include <QtCore/qthread.h>

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcSrvCltBaseGateway.h"

namespace ZS
{
namespace Ipc
{
class CClient;
class CClientGateway;
class CIpcSocketWrapper;

//******************************************************************************
class ZSIPCDLL_API CClientGatewayThread : public CSrvCltBaseGatewayThread
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CClientGatewayThread"; }
public: // ctors and dtor
    CClientGatewayThread(
        const QString& i_strObjNameGateway,
        CClient* i_pClient,
        ZS::System::CErrLog* i_pErrLog,
        ZS::System::EMethodTraceDetailLevel i_eTrcMthFileDetailLevel = ZS::System::EMethodTraceDetailLevel::None );
    virtual ~CClientGatewayThread();
protected: // must overridables of base class QThread
    virtual void run() override;
protected: // instance members
    CClient* m_pClient;

}; // class CClientGatewayThread


//******************************************************************************
class ZSIPCDLL_API CClientGateway : public CSrvCltBaseGateway
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CClientGateway"; }
public: // ctors and dtor
    CClientGateway(
        const QString& i_strObjName,
        CClient* i_pClient,
        CClientGatewayThread* i_pThreadGateway,
        ZS::System::CErrLog* i_pErrLog,
        ZS::System::EMethodTraceDetailLevel i_eTrcMthFileDetailLevel = ZS::System::EMethodTraceDetailLevel::None );
    virtual ~CClientGateway();
protected slots: // overridables
    virtual void onConnected( QObject* i_pSocketWrapper );
    virtual void onTimeoutConnect();
    virtual void onTimeoutWatchDog();
protected slots: // must overridables of base class CSrvCltBaseGateway
    virtual void onDisconnected( QObject* i_pSocketWrapper ) override;
    virtual void onReadyRead( QObject* i_pSocketWrapper ) override;
    virtual void onError( QObject* i_pSocketWrapper, const ZS::System::SErrResultInfo& i_errResultInfo ) override;
public: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pMsg ) override;
protected: // instance members
    SSocketDscr        m_socketDscr;
    CIpcSocketWrapper* m_pIpcSocketWrapper;
    QTimer*            m_pTimerConnect;
    STimerSettings     m_watchDogSettings;
    QTimer*            m_pTimerWatchDog;

}; // class CClientGateway

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_ClientGateway_h
