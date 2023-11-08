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

#ifndef ZSIpc_InProcMsgServer_h
#define ZSIpc_InProcMsgServer_h

#include <QtCore/qthread.h>

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcCommon.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"

class QRecursiveMutex;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Ipc
{
class CInProcMsgServer;
class CInProcMsgSocket;

//******************************************************************************
class ZSIPCDLL_API CInProcMsgServersAdminObj : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static CInProcMsgServersAdminObj* GetInstance(); // singleton class
public: // ctors and dtor
    CInProcMsgServersAdminObj( bool i_bTracingEnabled = true );
    virtual ~CInProcMsgServersAdminObj();
public: // instance methods
    CInProcMsgServer* find( unsigned int i_uPort ) const;
    QList<unsigned int> getUsedPorts() const;
    unsigned int GetUniquePort() const;
public: // instance methods
    void onServerListen( CInProcMsgServer* i_pServer );
    void onServerClose( CInProcMsgServer* i_pServer );
protected: // instance members
    QMutex*                              m_pMutex;
    QMap<unsigned int,CInProcMsgServer*> m_mapServers; // Map with listening servers (the key corresponds to the port)
    bool                                 m_bDestroying;
    bool                                 m_bTracingEnabled;
    ZS::System::CTrcAdminObj*            m_pTrcAdminObj;

}; // class CInProcMsgServersAdminObj

//******************************************************************************
class ZSIPCDLL_API CInProcMsgServer : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CInProcMsgServer"; }
public: // class methods
    static CInProcMsgServersAdminObj* GetAdminObjInstance();
public: // class methods
    static CInProcMsgServer* Find( unsigned int i_uPort );
    static QList<unsigned int> GetUsedPorts();
    static unsigned int GetUniquePort();
public: // ctors and dtor
    CInProcMsgServer( const QString& i_strObjName, bool i_bTracingEnabled = true );
    virtual ~CInProcMsgServer();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
signals:
    void newConnection();
public: // instance methods
    ZS::System::ECopyDepth getMsgReadBuffCopyDepth() const;
    void setMsgReadBuffCopyDepth( ZS::System::ECopyDepth i_copyDepth );
public: // instance methods
    unsigned int getPort() const;
public: // overridables
    virtual void setMaxPendingConnections( int i_iMaxConnections );
    virtual int maxPendingConnections() const;
    virtual bool listen( unsigned int i_uPort );
    virtual bool isListening() const;
    virtual void close();
    virtual bool hasPendingConnections() const;
    virtual CInProcMsgSocket* nextPendingConnection();
    virtual ZS::System::EResult error() const;
    virtual QString errorString() const;
    virtual ZS::System::SErrResultInfo errResultInfo() const;
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
protected: // instance methods
    QRecursiveMutex*           m_pMutex;
    ESocketState               m_socketState;
    QString                    m_strHostName;
    unsigned int               m_uPort;
    ZS::System::SErrResultInfo m_errResultInfo;
    int                        m_iMaxPendingConnections;
    QVector<CInProcMsgSocket*> m_arpPendingConnections;
    ZS::System::ECopyDepth     m_copyDepthMsgReadBuff;
    bool                       m_bTracingEnabled;
    ZS::System::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CInProcMsgServer

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_InProcMsgServer_h
