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

#ifndef ZSIpc_Common_h
#define ZSIpc_Common_h

#include <QtCore/qstring.h>
#include <QtNetwork/qhostaddress.h>

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSSys/ZSSysCommon.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace Ipc
{
enum ESrvCltType {
    ESrvCltTypeServer = 0,
    ESrvCltTypeClient = 1,
    ESrvCltTypeCount,
    ESrvCltTypeUndefined
};
ZSIPCDLL_API QString srvCltType2Str( int i_srvCltType, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrSymbol );
ZSIPCDLL_API ESrvCltType str2SrvCltType( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrSymbol );

enum ESocketType {
    ESocketTypeTcp       = 0,
    ESocketTypeShm       = 1,
    ESocketTypeInProcMsg = 2,
    ESocketTypeCount,
    ESocketTypeUndefined
};
ZSIPCDLL_API QString socketType2Str( int i_socketType, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrSymbol );
ZSIPCDLL_API ESocketType str2SocketType( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrSymbol );

enum ESocketState {
    ESocketStateUnconnected = 0,
    ESocketStateHostLookup  = 1,
    ESocketStateConnecting  = 2,
    ESocketStateConnected   = 3,
    ESocketStateBound       = 4,
    ESocketStateListening   = 5,
    ESocketStateClosing     = 6,
    ESocketStateCount,
    ESocketStateUndefined
};
ZSIPCDLL_API QString socketState2Str( int i_socketState, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrSymbol );

const int ESocketIdAllSockets = -2;
const int ESocketIdUndefined = -1;
const int ESocketIdMin = 0;
const int ESocketIdMax = 65535;

ZSIPCDLL_API QString socketId2Str( int i_iSocketId );

//******************************************************************************
struct ZSIPCDLL_API SSocketDscr
//******************************************************************************
{
// If the socket descriptor is used on the client's side
// - the local host name, local port and local object pointer (if used) are
//   defining the connection parameters of the client
// - the remote host name, remote port and remote object pointer (if used) are
//   defining the connection parameters of the server.
// If the socket descriptor is used on the server's side
// - the local host name, local port and local object pointer (if used) are
//   defining the connection parameters of the server
// - the remote host name, remote port and remote object pointer (if used) are
//   defining the connection parameters of the client.
public: // ctors
    SSocketDscr();
    SSocketDscr(
        ESrvCltType  i_srvCltType,
        ESocketType  i_socketType = ZS::Ipc::ESocketTypeUndefined );
    SSocketDscr( // ctor for sockets with socketType = InProcMsg for both server's and client's side
        QObject*     i_pObjSrvClt,
        ESrvCltType  i_srvCltType );
    SSocketDscr( // ctor for sockets on server's side
        unsigned int i_uServerListenPort,     // Port the server is listening for incoming connect requests.
        ESocketType  i_socketType = ZS::Ipc::ESocketTypeUndefined,
        ESrvCltType  i_srvCltType = ZS::Ipc::ESrvCltTypeServer );
    SSocketDscr( // ctor for sockets on client's side
        const QString& i_strRemoteHostName,
        unsigned int   i_uServerListenPort,   // Port the server is listening for incoming connect requests.
        ESocketType    i_socketType = ZS::Ipc::ESocketTypeUndefined,
        ESrvCltType    i_srvCltType = ZS::Ipc::ESrvCltTypeClient );
    SSocketDscr( const SSocketDscr& i_socketDscrOther );
public: // struct methods
    bool isValid() const;
    ESrvCltType getRemoteSrvCltType() const;
public: // struct methods
    QString getConnectionString( int i_iDetailLevel = 0 ) const;
public: // operators
    bool operator == ( const SSocketDscr& i_socketDscrOther ) const;
    bool operator != ( const SSocketDscr& i_socketDscrOther ) const;
public: // struct members
    ESrvCltType  m_srvCltType;
    ESocketType  m_socketType;
    int          m_iSocketId;
    ESocketState m_socketState;
    unsigned int m_uBufferSize;
    int          m_iConnectTimeout_ms;
    unsigned int m_uServerListenPort;   // Port the server is listening for incoming connect requests.
    // Socket connection parameters (valid after connection is established)
    QObject*     m_pObjLocal;
    QString      m_strLocalHostName;
    QHostAddress m_hostAddrLocal;
    unsigned int m_uLocalPort;
    QObject*     m_pObjRemote;
    QString      m_strRemoteHostName;   // Must be valid on client's side before creating the socket.
    QHostAddress m_hostAddrRemote;
    unsigned int m_uRemotePort;
    QString      m_strSocketName;       // Optional. Descriptive name of the socket connection (e.g. logical name of the remote server).

}; // struct SSocketDscr


//******************************************************************************
struct ZSIPCDLL_API SServerHostSettings
//******************************************************************************
{
public: // ctors and dtor
    SServerHostSettings();
    SServerHostSettings( ESocketType i_socketType );
    SServerHostSettings( // ctor for sockets with socketType = Tcp
        const QString& i_strLocalHostName,
        quint16        i_uLocalPort,
        unsigned int   i_uMaxPendingConnections = 30 );
    SServerHostSettings( // ctor for sockets with socketType = InProcMsg
        QObject*     i_pObjLocal,
        unsigned int i_uMaxPendingConnections = 30 );
    SServerHostSettings( const SSocketDscr& i_socketDscr );
    //SServerHostSettings( const QString& i_strCnct );
public: // struct methods
    void toSocketDscr( SSocketDscr& i_socketDscr ) const;
    SSocketDscr getSocketDscr() const;
public: // struct methods
    QString getConnectionString( int i_iDetailLevel = 0 ) const;
public: // operators
    SServerHostSettings& operator = ( const SServerHostSettings& i_settingsOther );
    SServerHostSettings& operator = ( const SSocketDscr& i_socketDscr );
    bool operator == ( const SServerHostSettings& i_settingsOther ) const;
    bool operator != ( const SServerHostSettings& i_settingsOther ) const;
    bool operator == ( const SSocketDscr& i_socketDscr ) const;
    bool operator != ( const SSocketDscr& i_socketDscr ) const;
public: // struct members
    ESocketType  m_socketType;
    QObject*     m_pObjLocal;
    QString      m_strLocalHostName; // So far only used for shm servers and shm clients. Tcp servers and tcp clients determine the local host name by themselves.
    QHostAddress m_hostAddrLocal;
    quint16      m_uLocalPort;      // Port the server is listening for incoming connection requests.
    unsigned int m_uMaxPendingConnections;
    unsigned int m_uBufferSize;     // Only used by shared memory socket clients

}; // struct SServerHostSettings

//******************************************************************************
struct ZSIPCDLL_API SClientHostSettings
//******************************************************************************
{
public: // ctors and dtor
    SClientHostSettings();
    SClientHostSettings( ESocketType i_socketType );
    SClientHostSettings( // ctor for sockets with socketType = Tcp
        const QString& i_strRemoteHostName,
        quint16        i_uRemotePort,
        int            i_iConnectTimeout_ms = 5000 );
    SClientHostSettings( // ctor for sockets with socketType = InProcMsg
        QObject*     i_pObjRemote,
        int          i_iConnectTimeout_ms = 5000 );
    //SClientHostSettings( const QString& i_strCnct );
    SClientHostSettings( const SSocketDscr& i_socketDscr );
public: // struct methods
    void toSocketDscr( SSocketDscr& i_socketDscr ) const;
    SSocketDscr getSocketDscr() const;
public: // struct methods
    QString getConnectionString( int i_iDetailLevel = 0 ) const;
public: // operators
    SClientHostSettings& operator = ( const SClientHostSettings& i_settingsOther );
    SClientHostSettings& operator = ( const SSocketDscr& i_socketDscr );
    bool operator == ( const SClientHostSettings& i_settingsOther ) const;
    bool operator != ( const SClientHostSettings& i_settingsOther ) const;
    bool operator == ( const SSocketDscr& i_socketDscr ) const;
    bool operator != ( const SSocketDscr& i_socketDscr ) const;
public: // struct members
    ESocketType  m_socketType;
    QObject*     m_pObjRemote;
    QString      m_strRemoteHostName;
    quint16      m_uRemotePort; // Port the server is listening for incoming connection requests.
    int          m_iConnectTimeout_ms;
    unsigned int m_uBufferSize; // only used by shared memory socket clients

}; // struct SClientHostSettings


//******************************************************************************
struct ZSIPCDLL_API STimerSettings
//******************************************************************************
{
public: // ctors and dtor
    STimerSettings(
        bool i_bEnabled = true,
        int  i_iInterval_ms = 5000,
        int  i_iTimeout_ms = 11000 );
    STimerSettings( const STimerSettings& i_other );
    virtual ~STimerSettings() {};
public: // operators
    STimerSettings& operator = ( const STimerSettings& i_other );
    bool operator == ( const STimerSettings& i_other ) const;
    bool operator != ( const STimerSettings& i_other ) const;
public: // struct methods
    QString toString() const;
public: // struct members
    bool m_bEnabled;
    int  m_iInterval_ms;
    int  m_iTimeout_ms; // May be set to -1 to indicate that a global timeout may be used (e.g. from server host settings etc.).

}; // struct STimerSettings


} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_Common_h
