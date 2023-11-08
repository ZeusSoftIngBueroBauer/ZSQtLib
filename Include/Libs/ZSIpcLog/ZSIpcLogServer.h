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

#ifndef ZSIpcLog_Server_h
#define ZSIpcLog_Server_h

#include "ZSIpcLog/ZSIpcLogDllMain.h"
#include "ZSIpc/ZSIpcServer.h"
#include "ZSSys/ZSSysLogServer.h"

class QRecursiveMutex;

namespace ZS
{
namespace Ipc
{
struct SServerHostSettings;
}

namespace Log
{
//******************************************************************************
/*! @brief All log outputs should be made via the CIpcLogServer class.

The class is derived from the CLogServer class and extends the base class with
the option to send the log output to a remote client via TCP/IP.

In this way, "online" logging is possible. The states of the Logger objects
are displayed in a tree view in the client and can be changed with a mouse click.

The Log Client provided via the ZSQtLib also makes it possible to separate the
log outputs by color depending on the thread.

Log Server is a singleton class which is usually created when the application
is started by calling the "CreateInstance" class method.
During program execution, a reference to the instance can be obtained via
"GetInstance", parameters can be changed, log outputs can be made in the Log
Output File and sent to connected Log Clients.

Before exiting the application, the Log Server instance must be freed again
with "ReleaseInstance".
*/
class ZSIPCLOGDLL_API CIpcLogServer : public ZS::System::CLogServer
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Log"; }
    static QString ClassName() { return "CIpcLogServer"; }
public: // class methods
    static CIpcLogServer* GetInstance();
    static CIpcLogServer* CreateInstance();
    static void ReleaseInstance();
protected: // ctors and dtor
    CIpcLogServer();
    virtual ~CIpcLogServer();
public: // overridables
    QString nameSpace() const { return CIpcLogServer::NameSpace(); }
    QString className() const { return CIpcLogServer::ClassName(); }
public: // instance methods
    Ipc::CServer* getIpcServer() { return m_pIpcServer; }
public: // overridables of base class CLogServer
    virtual void setEnabled( bool i_bEnabled ) override;
    virtual bool isActive() const override;
public: // overridables of base class CLogServer
    virtual void log( ZS::System::ELogDetailLevel i_eFilterDetailLevel, const QString& i_strLogEntry ) override;
    virtual void log( ZS::System::CLogger* i_pLogger, ZS::System::ELogDetailLevel i_eFilterDetailLevel, const QString& i_strLogEntry ) override;
public: // overridables of base class CLogServer
    virtual void setNewLoggersEnabledAsDefault( bool i_bEnabled ) override;
    virtual void setNewLoggersDefaultDetailLevel( ZS::System::ELogDetailLevel i_eDetailLevel ) override;
public: // overridables of base class CLogServer
    virtual void setLoggerFileAbsoluteFilePath( const QString& i_strAbsFilePath ) override;
public: // overridables of base class CLogServer
    virtual void setUseLocalLogFile( bool i_bUse ) override;
    virtual void setLocalLogFileAbsoluteFilePath( const QString& i_strAbsFilePath ) override;
    virtual void setLocalLogFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms ) override;
    virtual void setLocalLogFileSubFileCountMax( int i_iCountMax ) override;
    virtual void setLocalLogFileSubFileLineCountMax( int i_iCountMax ) override;
    virtual void setLocalLogFileCloseFileAfterEachWrite( bool i_bCloseFile ) override;
public: // overridables of base class CLogServer
    virtual void setUseIpcServer( bool i_bUse ) override;
    virtual void setCacheLogDataIfNotConnected( bool i_bCacheData ) override;
    virtual void setCacheLogDataMaxArrLen( int i_iMaxArrLen ) override;
public: // overridables of base class CLogServer
    virtual void setLogSettings( const ZS::System::SLogServerSettings& i_settings ) override;
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
protected: // instance methods to send index tree entries to the connected clients
    void sendBranch(
        int                                     i_iSocketId,
        ZS::System::MsgProtocol::TSystemMsgType i_systemMsgType,
        ZS::System::MsgProtocol::TCommand       i_cmd,
        ZS::System::CIdxTreeEntry*              i_pBranch );
    void sendLeave(
        int                                     i_iSocketId,
        ZS::System::MsgProtocol::TSystemMsgType i_systemMsgType,
        ZS::System::MsgProtocol::TCommand       i_cmd,
        const QString&                          i_strKeyInTree,
        int                                     i_idxInTree );
protected: // auxiliary methods
    void sendServerSettings(int i_iSocketId);
    void sendCachedLogData(int i_iSocketId);
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
    void onLoggersIdxTreeEntryAdded( const QString& i_strKeyInTree );
    void onLoggersIdxTreeEntryAboutToBeRemoved( const QString& i_strKeyInTree, int i_idxInTree );
    void onLoggersIdxTreeEntryChanged( const QString& i_strKeyInTree );
protected: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pEv ) override;
protected: // instance members
    /*!< The Ipc Server used to send and receice data via TCP/IP. */
    Ipc::CServer* m_pIpcServer;
    /*!< Flag to indicate the the instance is going to be destroyed. */
    bool m_bIsBeingDestroyed;
    /*!< List with socket ids of connected clients. */
    QVector<int> m_ariSocketIdsConnectedClients;
    /*!< This flag is set to true if the client receives data and onReceivedData is in progress updating
         the servers data with the settings read from the remote client. If the settings are updated
         the changed signals are not emitted before all settings have been applied. If this flag is set
         the server knows that the settings are changed by receiving data from the client and data must
         not be send back to the remote client. */
    bool m_bOnReceivedDataUpdateInProcess;
    /*!< Mutex to protect the list of the temporarily stored (cached) data. */
    QRecursiveMutex* m_pMtxListLogDataCached;
    /*!< To avoid reallocation (resizing) the cache for the data the cache is allocated
         with the maximum number of elements. Unused elements are set to nullptr. But for this
         the number of used entries must be counted seperately. */
    int m_iLogDataCachedCount;
    /*!< Cache for storing data as long as no client is connected. */
    QVector<ZS::System::SLogData*> m_arpLogDataCached;

}; // class CIpcLogServer

} // namespace Log

} // namespace ZS

#endif // #ifndef ZSIpcLog_Server_h
