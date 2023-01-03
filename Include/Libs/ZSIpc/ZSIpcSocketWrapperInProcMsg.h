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

#ifndef ZSIpc_SocketWrapperInProcMsg_h
#define ZSIpc_SocketWrapperInProcMsg_h

#include <QtCore/qthread.h>

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcSocketWrapper.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"

class QWaitCondition;

namespace ZS
{
namespace System
{
class CMsg;
}
namespace Ipc
{
class CInProcMsgSocket;

//******************************************************************************
class ZSIPCDLL_API CInProcMsgSocketThread : public QThread
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CInProcMsgSocketThread"; }
public: // ctors and dtor
    CInProcMsgSocketThread(
        const QString& i_strObjName,
        ESrvCltType i_srvCltType,
        ZS::System::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        ZS::System::EMethodTraceDetailLevel i_eTrcMthFileDetailLevel = ZS::System::EMethodTraceDetailLevel::None );
    virtual ~CInProcMsgSocketThread();
signals:
    void socketCreated( QObject* i_pThread, QObject* i_pSocket );
public: // instance methods
    CInProcMsgSocket* getSocket() { return m_pInProcMsgSocket; }
public: // overridables of base class QThread
    virtual void run();
protected: // instance members
    ESrvCltType          m_srvCltType;         // Defines on which side of the connection the socket is existing
    CInProcMsgSocket*    m_pInProcMsgSocket;   // "Peer" to which this socket is connected (socket on the other side of the connection).
    ZS::System::EMethodTraceDetailLevel m_eTrcMthFileDetailLevel;
    ZS::System::CTrcMthFile*  m_pTrcMthFile;   // Either trace method file with detail level is used or
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;  // trace admin object with IpcTrace server.

}; // class CInProcMsgSocketThread

//******************************************************************************
class ZSIPCDLL_API CInProcMsgSocketWrapper : public CIpcSocketWrapper
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Ipc"; }
    static QString ClassName() { return "CInProcMsgSocketWrapper"; }
public: // ctors and dtor
    CInProcMsgSocketWrapper(
        const QString&      i_strObjName,
        ESrvCltType         i_srvCltType,
        int                 i_iSocketId = ZS::Ipc::ESocketIdUndefined,
        CInProcMsgSocket*   i_pInProcMsgSocket = nullptr,
        ZS::System::CTrcMthFile* i_pTrcMthFile = nullptr,         // If != nullptr trace method file with detail level is used instead of trace admin object with Trace server.
        ZS::System::EMethodTraceDetailLevel i_eTrcMthFileDetailLevel = System::EMethodTraceDetailLevel::None );
    virtual ~CInProcMsgSocketWrapper();
public: // overridables of base class CIpcSocketWrapper
    virtual QString nameSpace() const override { return NameSpace(); }
    virtual QString className() const override { return ClassName(); }
public: // instance methods
    ZS::System::ECopyDepth getMsgReadBuffCopyDepth() const;
    void setMsgReadBuffCopyDepth( ZS::System::ECopyDepth i_copyDepth );
public: // overridables
    virtual int messagesAvailable() const;
    virtual void writeMessage( ZS::System::CMsg* i_pMsg );
    virtual ZS::System::CMsg* readMessage();
public: // must overridables of base class CIpcSocketWrapper
    virtual void connectToHost( const QString& i_strRemoteHostName, unsigned int i_uRemotePort, unsigned int i_uBufferSize = 0 ) override;
    virtual bool waitForConnected( int i_iTimeout_ms = 30000 ) override;
    virtual void disconnectFromHost() override;
    virtual bool waitForDisconnected( int i_iTimeout_ms = 30000 ) override;
    virtual void abort() override;
    //virtual qint64 write( const char* i_pcData, qint64 i_iMaxSize );
    virtual qint64 write( const QByteArray& i_byteArray ) override;
    virtual bool waitForReadyRead( int i_iTimeout_ms = 30000 ) override;
    virtual qint64 bytesAvailable() const override;
    //virtual qint64 read( char* i_pcData, qint64 i_iMaxSize );
    virtual QByteArray read( qint64 i_iMaxSize ) override;
    virtual QByteArray readAll() override;
    virtual ZS::System::EResult error() const override;
    virtual QString errorString() const override;
    virtual ZS::System::SErrResultInfo errResultInfo() const override;
protected slots: // overridables
    virtual void onSocketCreated( QObject* i_pThread, QObject* i_pSocket );
    virtual void onSocketDestroyed( QObject* i_pSocket );
protected slots: // overridables
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onError( ZS::System::EResult i_result );
    virtual void onReadyRead();
protected: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pEv );
protected: // instance methods
    CInProcMsgSocketThread* m_pInProcMsgSocketThread;
    CInProcMsgSocket*       m_pInProcMsgSocket;
    ZS::System::ECopyDepth  m_copyDepthMsgReadBuff;
    bool                    m_bSlotOnConnectedConnected;
    bool                    m_bSlotOnDisconnectedConnected;
    bool                    m_bSlotOnErrorConnected;
    bool                    m_bSlotOnReadyReadConnected;
    QWaitCondition*         m_pWaitCondSocketCreated;
    QWaitCondition*         m_pWaitCondConnected;
    QWaitCondition*         m_pWaitCondDisconnected;
    QWaitCondition*         m_pWaitCondReadyRead;

}; // class CInProcMsgSocketWrapper

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_SocketWrapperInProcMsg_h
