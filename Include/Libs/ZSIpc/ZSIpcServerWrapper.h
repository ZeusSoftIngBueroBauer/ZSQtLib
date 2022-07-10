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

#ifndef ZSIpc_ServerWrapper_h
#define ZSIpc_ServerWrapper_h

#include <QtCore/qobject.h>

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcCommon.h"
#include "ZSSys/ZSSysErrResult.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;
class CTrcMthFile;
}

namespace Ipc
{
class CIpcSocketWrapper;

//******************************************************************************
class ZSIPCDLL_API CIpcServerWrapper : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CIpcServerWrapper"; }
protected: // ctor
    CIpcServerWrapper(
        const QString& i_strObjName,
        ESocketType i_socketType,
        ZS::System::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        ZS::System::EMethodTraceDetailLevel i_eTrcMthFileDetailLevel = ZS::System::EMethodTraceDetailLevel::None );
public: // dtor
    virtual ~CIpcServerWrapper();
signals:
    void newConnection( QObject* i_pSocketWrapper );
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    ESocketType socketType() const;
public: // overridables
    virtual void setLocalHostName( const QString& i_strName );
    virtual QString getLocalHostName() const { return m_strLocalHostName; }
    virtual QHostAddress getLocalHostAddress() const { return m_hostAddrLocal; }
    virtual void setLocalPort( unsigned int i_uPort );
    virtual unsigned int getLocalPort() const { return m_uLocalPort; }
public: // must overridables
    virtual void setMaxPendingConnections( int i_iMaxConnections ) = 0;
    virtual int maxPendingConnections() const = 0;
    virtual bool listen() = 0;
    virtual bool isListening() const = 0;
    virtual void close() = 0;
    virtual bool hasPendingConnections() const = 0;
    virtual CIpcSocketWrapper* nextPendingConnection() = 0;
    virtual ZS::System::EResult error() const = 0;
    virtual QString errorString() const = 0;
    virtual ZS::System::SErrResultInfo errResultInfo() const = 0;
protected: // instance methods
    bool areTraceMethodCallsActive( ZS::System::EMethodTraceDetailLevel i_eFilterDetailLevel ) const;
    ZS::System::EMethodTraceDetailLevel getMethodCallsTraceDetailLevel() const;
    bool isTraceRuntimeInfoActive( ZS::System::ELogDetailLevel i_eFilterDetailLevel ) const;
    ZS::System::ELogDetailLevel getRuntimeInfoTraceDetailLevel() const;
protected: // instance members
    ESocketType          m_socketType;
    QString              m_strLocalHostName;
    QHostAddress         m_hostAddrLocal;
    unsigned int         m_uLocalPort;
    QObject*             m_pServer;
    ZS::System::EMethodTraceDetailLevel m_eTrcMthFileDetailLevel;
    ZS::System::CTrcMthFile*  m_pTrcMthFile;   // Either trace method file with detail level is used or
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;  // trace admin object with IpcTrace server.

}; // class CIpcServerWrapper

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_ServerWrapper_h
