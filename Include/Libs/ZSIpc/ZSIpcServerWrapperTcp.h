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

#ifndef ZSIpc_ServerWrapperTcp_h
#define ZSIpc_ServerWrapperTcp_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcServerWrapper.h"

class QTcpServer;

namespace ZS
{
namespace Ipc
{
//******************************************************************************
class ZSIPCDLL_API CTcpServerWrapper : public CIpcServerWrapper
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CTcpServerWrapper"; }
public: // ctors and dtor
    CTcpServerWrapper(
        const QString& i_strObjName,
        ZS::System::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        ZS::System::EMethodTraceDetailLevel i_eTrcMthFileDetailLevel = System::EMethodTraceDetailLevel::None );
    virtual ~CTcpServerWrapper();
public: // overridables of base class CIpcServerWrapper
    virtual QString nameSpace() const override { return NameSpace(); }
    virtual QString className() const override { return ClassName(); }
public: // must overridables of base class CIpcServerWrapper
    virtual void setMaxPendingConnections( int i_iMaxConnections ) override;
    virtual int maxPendingConnections() const override;
    virtual bool listen() override;
    virtual bool isListening() const override;
    virtual void close() override;
    virtual bool hasPendingConnections() const override;
    virtual CIpcSocketWrapper* nextPendingConnection() override;
    virtual ZS::System::EResult error() const override;
    virtual QString errorString() const override;
    virtual ZS::System::SErrResultInfo errResultInfo() const override;
protected slots: // overridables
    virtual void onNewConnection();
protected: // instance methods
    QTcpServer* m_pTcpServer;

}; // class CTcpServerWrapper

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_ServerWrapperTcp_h
