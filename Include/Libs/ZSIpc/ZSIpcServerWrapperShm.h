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

#ifndef ZSIpc_ServerWrapperShm_h
#define ZSIpc_ServerWrapperShm_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcServerWrapper.h"

class QSharedMemory;
class QTimer;

namespace ZS
{
namespace Ipc
{
class CShmSocketWrapper;

//******************************************************************************
class ZSIPCDLL_API CShmServerWrapper : public CIpcServerWrapper
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CShmServerWrapper"; }
public: // ctors and dtor
    CShmServerWrapper(
        const QString&      i_strObjName,
        Trace::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        ZS::Trace::ETraceDetailLevelMethodCalls i_eTrcMthFileDetailLevel = ZS::Trace::ETraceDetailLevelMethodCalls::None );
    virtual ~CShmServerWrapper();
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
protected slots:
    void onTimeoutListen();
protected: // instance methods
    QSharedMemory*       m_pSharedMemory;
    int                  m_iSharedMemoryLockCount;
    QTimer*              m_pTimerListen;
    int                  m_iMaxPendingConnections;
    int                  m_iPendingConnections;
    CShmSocketWrapper**  m_arpPendingConnections;

}; // class CShmServerWrapper

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_ServerWrapperShm_h
