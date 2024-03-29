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

#ifndef ZSIpcTrace_Server_h
#define ZSIpcTrace_Server_h

#include "ZSIpcTrace/ZSIpcTrcDllMain.h"
#include "ZSIpc/ZSIpcServer.h"
#include "ZSSys/ZSSysTrcServer.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
class QRecursiveMutex;
#else
class QMutex;
#endif

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}
namespace Ipc
{
struct SServerHostSettings;
}

namespace Trace
{
struct SMthTrcData;

//******************************************************************************
/*! @brief All trace outputs should be made via the CIpcTrcServer class.

The class is derived from the CTrcServer class and extends the base class with
the option to send the trace output to a remote client via TCP/IP.

In this way, "online" tracing is possible. The states of the Trace Admin objects
can be changed and displayed via a tree view in the client with a mouse click.

The Trace Client provided via the ZSQtLib also makes it possible to separate the
trace outputs by color depending on the thread.

Trace Server is a singleton which is usually created when the application is
started by calling the "CreateInstance" class method.
During program execution, a reference to the instance can be obtained via
"GetInstance", parameters can be changed, log outputs can be made in the Trace
Method File and sent to connected Trace Clients.

Before exiting the application, the Trace Server instance must be freed again
with "ReleaseInstance".
*/
class ZSIPCTRACEDLL_API CIpcTrcServer : public ZS::System::CTrcServer
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Trace"; }
    static QString ClassName() { return "CIpcTrcServer"; }
public: // class methods
    static CIpcTrcServer* GetInstance();
    static CIpcTrcServer* CreateInstance(
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevel = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelMutex = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelIpcServer = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelIpcServerMutex = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelIpcServerGateway = ZS::System::EMethodTraceDetailLevel::None );
    static void ReleaseInstance();
protected: // ctors and dtor
    CIpcTrcServer(
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevel = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelMutex = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelIpcServer = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelIpcServerMutex = ZS::System::EMethodTraceDetailLevel::None,
        ZS::System::EMethodTraceDetailLevel i_eTrcDetailLevelIpcServerGateway = ZS::System::EMethodTraceDetailLevel::None );
    virtual ~CIpcTrcServer();
public: // instance methods
    Ipc::CServer* getIpcServer() { return m_pIpcServer; }
public: // overridables of base class CTrcServer
    virtual bool isActive() const override;
public: // overridables of base class CTrcServer
    virtual void setEnabled( bool i_bEnabled ) override;
    virtual void setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled ) override;
    virtual void setNewTrcAdminObjsMethodCallsDefaultDetailLevel( ZS::System::EMethodTraceDetailLevel i_eDetailLevel );
    virtual void setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel( ZS::System::ELogDetailLevel i_eDetailLevel );
public: // overridables of base class CTrcServer
    virtual void setAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath ) override;
public: // overridables of base class CTrcServer
    virtual void setUseLocalTrcFile( bool i_bUse ) override;
    virtual void setLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath ) override;
    virtual void setLocalTrcFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms ) override;
    virtual void setLocalTrcFileSubFileCountMax( int i_iCountMax ) override;
    virtual void setLocalTrcFileSubFileLineCountMax( int i_iCountMax ) override;
    virtual void setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile ) override;
public: // overridables of base class CTrcServer
    virtual void setUseIpcServer( bool i_bUse ) override;
    virtual void setCacheTrcDataIfNotConnected( bool i_bCacheData ) override;
    virtual void setCacheTrcDataMaxArrLen( int i_iMaxArrLen ) override;
public: // overridables of base class CTrcServer
    virtual void setTraceSettings( const ZS::System::STrcServerSettings& i_settings ) override;
public: // overridables of base class CTrcServer
    virtual void traceMethodEnter(
        const ZS::System::CTrcAdminObj* i_pAdminObj,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs ) override;
    virtual void traceMethodEnter(
        const ZS::System::CTrcAdminObj* i_pAdminObj,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs ) override;
    virtual void traceMethod(
        const ZS::System::CTrcAdminObj* i_pAdminObj,
        const QString& i_strMethod,
        const QString& i_strAddInfo ) override;
    virtual void traceMethod(
        const ZS::System::CTrcAdminObj* i_pAdminObj,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strAddInfo ) override;
    virtual void traceMethodLeave(
        const ZS::System::CTrcAdminObj* i_pAdminObj,
        const QString& i_strMethod,
        const QString& i_strMethodReturn,
        const QString& i_strMethodOutArgs ) override;
    virtual void traceMethodLeave(
        const ZS::System::CTrcAdminObj* i_pAdminObj,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodReturn,
        const QString& i_strMethodOutArgs ) override;
protected: // auxiliary methods
    void addEntry(
        const QString&                  i_strThreadName,
        const QDateTime&                i_dt,
        double                          i_fSysTimeInSec,
        ZS::System::EMethodDir          i_mthDir,
        const ZS::System::CTrcAdminObj* i_pTrcAdminObj,
        const QString&                  i_strObjName,
        const QString&                  i_strMethod,
        const QString&                  i_strAddInfo = "",
        const QString&                  i_strMethodOutArgs = "" );
public: // instance methods of the remote connection
    ZS::System::CRequest* startup( int i_iTimeout_ms = 5000, bool i_bWait = true, qint64 i_iReqIdParent = -1 );
    ZS::System::CRequest* shutdown( int i_iTimeout_ms = 5000, bool i_bWait = true, qint64 i_iReqIdParent = -1 );
public: // instance methods of the remote connection
    // Need to be called after changing settings to apply the settings:
    ZS::System::CRequest* changeSettings( int i_iTimeout_ms = 5000, bool i_bWait = true, qint64 i_iReqIdParent = -1 );
public: // instance methods changing and reading the host settings
    Ipc::SServerHostSettings getHostSettings() const;
    void setHostSettings( const Ipc::SServerHostSettings& i_hostSettings );
public: // instance methods of the remote connection
    void setBlkType( Ipc::CBlkType* i_pBlkType ); // The class takes ownership of the data block.
    Ipc::CBlkType* getBlkType();
public: // instance methods of the remote connection
    int getSocketId( int i_idxSocket ) const;
    Ipc::SSocketDscr getSocketDscr( int i_iSocketId ) const;
    int getArrLenConnections() const;
    int getActiveConnections() const;
public: // instance methods (state machine)
    Ipc::CServer::EState getState() const;
    bool isListening() const;
    bool isStartingUp() const;
    bool isConnected( int i_iSocketId = Ipc::ESocketIdUndefined ) const;
public: // instance methods (state machine)
    bool isBusy() const;
    Ipc::CServer::ERequest requestInProgress() const;
    QString requestInProgress2Str( bool i_bShort = true ) const;
    ZS::System::CRequest* getRequestInProgress() const;
public: // instance methods (aborting requests)
    void abortRequest( qint64 i_iRequestId ); // immediately aborts the request if in progress or removes the request from the list of pending requests
    void abortRequestInProgress();            // immediately aborts the request in progress
    void abortAllRequests();                  // immediately aborts all requests in progress
protected: // instance methods to recursively send index tree entries to the connected clients
    void sendBranch(
        int                                     i_iSocketId,
        ZS::System::MsgProtocol::TSystemMsgType i_systemMsgType,
        ZS::System::MsgProtocol::TCommand       i_cmd,
        ZS::System::CIdxTreeEntry*              i_pBranch );
protected: // instance methods to explicitely send the attributes of a trace admin object to the connected clients
    void sendAdminObj(
        int                                     i_iSocketId,
        ZS::System::MsgProtocol::TSystemMsgType i_systemMsgType,
        ZS::System::MsgProtocol::TCommand       i_cmd,
        const QString&                          i_strKeyInTree,
        int                                     i_idxInTree );
protected: // auxiliary methods
    void sendServerSettings(int i_iSocketId);
    void sendCachedTrcData(int i_iSocketId);
protected: // instance methods of the remote connection
    ZS::System::CRequest* sendData( int i_iSocketId, const QByteArray& i_byteArr, qint64 i_iReqIdParent = -1 );
protected slots: // connected to the signals of the Ipc Server
    void onIpcServerConnected( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onIpcServerDisconnected( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onIpcServerReceivedData( QObject* i_pServer, int i_iSocketId, const QByteArray& i_byteArr );
protected: // overridables to parse and execute the incoming data stream
    void onIpcServerReceivedReqSelect( int i_iSocketId, const QString& i_strData );
    void onIpcServerReceivedReqUpdate( int i_iSocketId, const QString& i_strData );
protected slots: // connected to the signals of the index tree
    void onTrcAdminObjIdxTreeEntryAdded( const QString& i_strKeyInTree );
    void onTrcAdminObjIdxTreeEntryAboutToBeRemoved( const QString& i_strKeyInTree, int i_idxInTree );
    void onTrcAdminObjIdxTreeEntryChanged( const QString& i_strKeyInTree );
protected: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pEv ) override;
protected: // instance members
    /*!< The Ipc Server used to send and receice data via TCP/IP. */
    ZS::Ipc::CServer* m_pIpcServer;
    /*!< Flag to indicate the the instance is going to be destroyed. */
    bool m_bIsBeingDestroyed;
    /*!< List with socket ids of connected clients. */
    QVector<int> m_ariSocketIdsConnectedTrcClients;
    /*!< This flag is set to true if the client receives data and onReceivedData is in progress updating
         the servers data with the settings read from the remote client. If the settings are updated
         the changed signals are not emitted before all settings have been applied. If this flag is set
         the server knows that the settings are changed by receiving data from the client and data must
         not be send back to the remote client. */
    bool m_bOnReceivedDataUpdateInProcess;
    /*!< Mutex to protect the list of the temporarily stored (cached) trace data. */
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QRecursiveMutex* m_pMtxListTrcDataCached;
    #else
    QMutex* m_pMtxListTrcDataCached;
    #endif
    /*!< To avoid reallocation (resizing) the cache for the trace data the cache is allocated
         with the maximum number of elements. Unused elements are set to nullptr. But for this
         the number of used entries must be counted seperately. */
    int m_iTrcDataCachedCount;
    /*!< Cache for storing trace data as long as no client is connected. */
    QVector<SMthTrcData*> m_arpTrcDataCached;

}; // class CIpcTrcServer

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSIpcTrace_Server_h
