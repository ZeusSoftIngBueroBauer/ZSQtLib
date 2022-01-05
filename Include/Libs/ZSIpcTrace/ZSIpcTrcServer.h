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

#ifndef ZSIpcTrace_Server_h
#define ZSIpcTrace_Server_h

#include "ZSIpcTrace/ZSIpcTrcDllMain.h"
#include "ZSIpc/ZSIpcServer.h"
#include "ZSSys/ZSSysTrcServer.h"

class QMutex;

namespace ZS
{
namespace Ipc
{
struct SServerHostSettings;
}

namespace Trace
{
class CTrcAdminObj;
struct SMthTrcData;

//******************************************************************************
/*! @brief Über die Klasse CIpcTrcServer sollten alle Trace Ausgaben erfolgen.

    Die Klasse ist von der Klasse CTrcServer abgeleitet und erweitert die
    Basisklasse um die Möglichkeit, die Trace Ausgaben nicht nur in ein Log-File
    zu schreiben sondern auch über TCP/IP an einen Client zu versenden.

    Auf diese Weise ist ein "Online" tracing möglich und die Zustände der
    Trace Admin Objekte können über einen Treeview im Client über Mausklicks
    verändert und angezeigt werden. Der über die ZSQtLib bereitgestellte
    Trace Client ermöglicht es ferner, die Trace-Ausgaben je nach Thread
    farblich voneinander zu trennen.

    Normalerweise gibt es pro Applikation nur eine Trace Server Instanz die beim
    Start der Applikation durch Aufruf der Klassenmethode "CreateInstance" angelegt
    wird. Während der Programm-Ausführung kann über "GetInstance" eine Referenz auf
    die Instanz erhalten und Parameter ändern oder Log-Ausgaben in das Trace Method
    File vornehmen und an angeschlossen Trace Clients zu verschicken. Vor Beenden
    der Applikation ist die Trace Server Instanz mit "ReleaseInstance" wieder zu löschen.

    Für den Fall, dass mehrere Trace Server verwendet werden sollen (verschiedene
    Log Files, verschiedene Listen-Ports), kann bei "CreateInstance" ein vom Default
    Wert "ZSTrcServer" abweichender Name übergeben werden. Dieser Name ist bei Aufruf
    von "GetInstance" und "ReleaseInstance" wieder zu verwenden.

    Hat man keinen Einfluss auf den Programmstart, programmiert PlugIn Dlls und
    weiss nicht, ob nicht auch an anderer Stelle der Trace Server verwendet wird,
    muss der Zugriff auf die Trace Server über Referenzzähler kontrolliert werden.
    Deshalb besteht die Möglichkeit bei Aufruf von "CreateInstance" das
    Flag "CreateOnlyIfNotYetExisting" zu übergeben.
*/
class ZSIPCTRACEDLL_API CIpcTrcServer : public ZS::Trace::CTrcServer
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Trace"; }      // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    static QString ClassName() { return "CIpcTrcServer"; }  // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // class methods
    static CIpcTrcServer* GetInstance( const QString& i_strName = "ZSTrcServer" );
    static CIpcTrcServer* CreateInstance(
        const QString& i_strName = "ZSTrcServer",
        int i_iTrcDetailLevel = ETraceDetailLevelNone );
    static void ReleaseInstance( const QString& i_strName = "ZSTrcServer" );
    static void ReleaseInstance( CIpcTrcServer* i_pTrcServer );
    static void DestroyAllInstances();
protected: // ctors and dtor
    CIpcTrcServer( const QString& i_strName, int i_iTrcDetailLevel = ETraceDetailLevelNone );
    virtual ~CIpcTrcServer();
public: // overridables
    QString nameSpace() const { return CIpcTrcServer::NameSpace(); }
    QString className() const { return CIpcTrcServer::ClassName(); }
public: // instance methods
    Ipc::CServer* getIpcServer() { return m_pIpcServer; }
public: // overridables of base class CTrcServer
    virtual bool isActive() const override;
public: // overridables of base class CTrcServer
    virtual void setEnabled( bool i_bEnabled ) override;
    virtual void setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled ) override;
    virtual void setNewTrcAdminObjsDefaultDetailLevel( int i_iDetailLevel ) override;
public: // overridables of base class CTrcServer
    virtual void setAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath ) override;
public: // overridables of base class CTrcServer
    virtual void setUseLocalTrcFile( bool i_bUse ) override;
    virtual void setLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath ) override;
    virtual void setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile ) override;
public: // overridables of base class CTrcServer
    void setCacheTrcDataIfNotConnected( bool i_bCacheData ) override;
    void setCacheTrcDataMaxArrLen( int i_iMaxArrLen ) override;
public: // overridables of base class CTrcServer
    virtual void traceMethodEnter(
        const CTrcAdminObj* i_pAdminObj,
        const QString&      i_strMethod,
        const QString&      i_strMethodInArgs ) override;
    virtual void traceMethodEnter(
        const CTrcAdminObj* i_pAdminObj,
        const QString&      i_strObjName,
        const QString&      i_strMethod,
        const QString&      i_strMethodInArgs ) override;
    virtual void traceMethod(
        const CTrcAdminObj* i_pAdminObj,
        const QString&      i_strMethod,
        const QString&      i_strAddInfo ) override;
    virtual void traceMethod(
        const CTrcAdminObj* i_pAdminObj,
        const QString&      i_strObjName,
        const QString&      i_strMethod,
        const QString&      i_strAddInfo ) override;
    virtual void traceMethodLeave(
        const CTrcAdminObj* i_pAdminObj,
        const QString&      i_strMethod,
        const QString&      i_strMethodReturn,
        const QString&      i_strMethodOutArgs ) override;
    virtual void traceMethodLeave(
        const CTrcAdminObj* i_pAdminObj,
        const QString&      i_strObjName,
        const QString&      i_strMethod,
        const QString&      i_strMethodReturn,
        const QString&      i_strMethodOutArgs ) override;
protected: // auxiliary methods
    void addEntry(
        const QString&         i_strThreadName,
        const QDateTime&       i_dt,
        double                 i_fSysTimeInSec,
        ZS::System::EMethodDir i_mthDir,
        const CTrcAdminObj*    i_pTrcAdminObj,
        const QString&         i_strObjName,
        const QString&         i_strMethod,
        const QString&         i_strAddInfo = "",
        const QString&         i_strMethodOutArgs = "" );
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
        CTrcAdminObj*                           i_pTrcAdminObj );
protected: // instance methods to explicitely send the changed attributes of a name space to the connected clients
    void sendBranch(
        int                                     i_iSocketId,
        ZS::System::MsgProtocol::TSystemMsgType i_systemMsgType,
        ZS::System::MsgProtocol::TCommand       i_cmd,
        ZS::System::CIdxTreeEntry*              i_pBranch,
        ZS::System::EEnabled                    i_enabled,
        int                                     i_iDetailLevel );
protected: // instance methods of the remote connection
    ZS::System::CRequest* sendData( int i_iSocketId, const QByteArray& i_byteArr, qint64 i_iReqIdParent = -1 );
protected slots: // connected to the signals of the Ipc Server
    void onIpcServerConnected( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onIpcServerDisconnected( QObject* i_pServer, const ZS::Ipc::SSocketDscr& i_socketDscr );
    void onIpcServerReceivedData( QObject* i_pServer, int i_iSocketId, const QByteArray& i_byteArr );
protected: // overridables to parse and execute the incoming data stream
    void onIpcServerReceivedReqSelect( int i_iSocketId, const QString& i_strData );
    void onIpcServerReceivedReqUpdate( int i_iSocketId, const QString& i_strData );
protected slots: // connected to the signals of the trace admin object pool
    void onTrcAdminObjIdxTreeEntryAdded( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CIdxTreeEntry* i_pTreeEntry );
    void onTrcAdminObjIdxTreeEntryChanged( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CIdxTreeEntry* i_pTreeEntry );
protected: // overridables of inherited class QObject
    virtual bool event( QEvent* i_pEv ) override;
protected: // instance members
    Ipc::CServer*         m_pIpcServer;
    bool                  m_bIsBeingDestroyed;
    QVector<int>          m_ariSocketIdsConnectedTrcClients;
    QVector<int>          m_ariSocketIdsRegisteredTrcClients;
    bool                  m_bOnReceivedDataUpdateInProcess;
    // Also the list of the temporarily stored trace data must be protected as the
    // traced data may be added and removed from within different thread contexts
    // (if the data should be temporarily stored).
    QMutex*               m_pMtxListTrcDataCached;
    int                   m_iTrcDataCachedCount;
    QVector<SMthTrcData*> m_arpTrcDataCached;

}; // class CIpcTrcServer

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSIpcTrace_Server_h
