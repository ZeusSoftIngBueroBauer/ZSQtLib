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

#ifndef ZSIpc_ServerWrapperInProcMsg_h
#define ZSIpc_ServerWrapperInProcMsg_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcServerWrapper.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"

namespace ZS
{
namespace Ipc
{
class CInProcMsgServer;

//******************************************************************************
class ZSIPCDLL_API CInProcMsgServerWrapper : public CIpcServerWrapper
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CInProcMsgServerWrapper"; }
public: // ctors and dtor
    CInProcMsgServerWrapper(
        const QString& i_strObjName,
        ZS::System::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        ZS::System::EMethodTraceDetailLevel i_eTrcMthFileDetailLevel = ZS::System::EMethodTraceDetailLevel::None );
    virtual ~CInProcMsgServerWrapper();
public: // instance methods
    ZS::System::ECopyDepth getMsgReadBuffCopyDepth() const;
    void setMsgReadBuffCopyDepth( ZS::System::ECopyDepth i_copyDepth );
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
    CInProcMsgServer* m_pInProcMsgServer;

}; // class CInProcMsgServerWrapper

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_ServerWrapperInProcMsg_h
