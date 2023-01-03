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
#include <QtNetwork/qnetworkinterface.h>

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
/*! @brief Structure defining the parameters of a socket connection between a server and a client.

    - If the socket descriptor is used on the client's side
      - the local host name, local port and local object pointer (if used) are
        defining the connection parameters of the client
      - the remote host name, remote port and remote object pointer (if used) are
        defining the connection parameters of the server.

    - If the socket descriptor is used on the server's side
      - the local host name, local port and local object pointer (if used) are
        defining the connection parameters of the server
      - the remote host name, remote port and remote object pointer (if used) are
        defining the connection parameters of the client.
*/
struct ZSIPCDLL_API SSocketDscr
//******************************************************************************
{
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
    ESrvCltType  m_srvCltType;          /*!< Defines whether the socket descriptor is used on server or client site. */
    ESocketType  m_socketType;          /*!< Socket type of the connection. */
    int          m_iSocketId;           /*!< Unique id of the socket connection. */
    ESocketState m_socketState;         /*!< Current state of the connection. */
    unsigned int m_uBufferSize;         /*!< Used for Shm connections (see buffer size of SServerHostSettings and SClientHostSettings). */
    int          m_iConnectTimeout_ms;  /*!< Timeout in milli seconds for a connect request. */
    unsigned int m_uServerListenPort;   /*!< Port the server is listening for incoming connect requests. */
    // Socket connection parameters (valid after connection is established)
    QObject*     m_pObjLocal;           /*!< Reference to local object (only used for InProcMsg sockets). */
    QString      m_strLocalHostName;    /*!< Local host name. */
    QHostAddress m_hostAddrLocal;       /*!< Local host address. */
    unsigned int m_uLocalPort;          /*!< Local port number. This is not the same as the servers local port number.
                                             If a connection is established on client's site a socket is created using
                                             this port number. On server site also a socket is created whose port number
                                             is different from the servers listening port. */
    QObject*     m_pObjRemote;          /*!< Reference to remote object (only used for InProcMsg sockets). */
    QString      m_strRemoteHostName;   /*!< Host name of the remote peer. */
    QHostAddress m_hostAddrRemote;      /*!< Host address of the remote peer. */
    unsigned int m_uRemotePort;         /*!< Remote port number of the peers socket. */
    QString      m_strSocketName;       /*!< Optional. Descriptive name of the socket connection (e.g. logical name of the remote server). */

}; // struct SSocketDscr


//******************************************************************************
/*! @brief Structure summarizing the settings which can be applied to the Ipc server.
*/
struct ZSIPCDLL_API SServerHostSettings
//******************************************************************************
{
public: // ctors and dtor
    SServerHostSettings();
    SServerHostSettings( ESocketType i_socketType );
    SServerHostSettings(
        quint16        i_uLocalPort,
        unsigned int   i_uMaxPendingConnections = 30 );
    SServerHostSettings(
        QObject*     i_pObjLocal,
        unsigned int i_uMaxPendingConnections = 30 );
    SServerHostSettings( const SSocketDscr& i_socketDscr );
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
    ESocketType  m_socketType;          /*!< Socket type used by the Ipc Server. */
    QObject*     m_pObjLocal;           /*!< Used for socketType == Shm. */
    QString      m_strLocalHostName;    /*!< Used for socketType Shm and Tcp. For Tcp servers the local host name is usually "127.0.0.1". */
    QHostAddress m_hostAddrLocal;       /*!< Host address of the local host. If needed got to be retrieved during runtime from Qt's QTcpServer class. */
    quint16      m_uLocalPort;          /*!< Port the server is listening for incoming connection requests. */
    unsigned int m_uMaxPendingConnections; /*!< Maximum number of pending accepted connections. */
    unsigned int m_uBufferSize;         /*!< If communication uses shared memory the block size determines the maximum size of one memory block.
                                             If the data to be sent exceeds the buffer size the data will be divided into several memory blocks. */

}; // struct SServerHostSettings

//******************************************************************************
/*! @brief Structure summarizing the settings which can be applied to the Ipc client.
*/
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
    ESocketType  m_socketType;          /*!< Socket type used by the Ipc Client. Must correspond to the socket type of the server the clients wants to connect with. */
    QObject*     m_pObjRemote;          /*!< Used for socketType == Shm. Reference to IpcServer the client want's to connect to. */
    QString      m_strRemoteHostName;   /*!< Used for socketType Shm and Tcp. Must correspond to the LocalHostName of the Tcp servers the clients wants to connect with. */
    quint16      m_uRemotePort;         /*!< Port the server is listening for incoming connection requests. */
    int          m_iConnectTimeout_ms;  /*!< After the connect timeout the connect request is considered to be failed. */
    unsigned int m_uBufferSize;         /*!< If communication uses shared memory the block size determines the maximum size of one memory block.
                                             If the data to be sent exceeds the buffer size the data will be divided into several memory blocks. */

}; // struct SClientHostSettings


//******************************************************************************
/*! @brief Structure summarizing the settings which can be applied to a watch dog timer.
*/
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
    bool m_bEnabled;        /*!< Timers may be enabled and disabled. */
    int  m_iInterval_ms;    /*!< The timer will be called periodically with this interval in milli seconds. */
    int  m_iTimeout_ms;     /*!< If the counterpart did not reply within the specifed timeout an error may be reported
                                 and the connection may be considered as broken. */

}; // struct STimerSettings


/*******************************************************************************
converting common data types into strings and vice versa
*******************************************************************************/

#if QT_VERSION > QT_VERSION_CHECK(5, 11, 0)
ZSIPCDLL_API QString qNetworkInterfaceType2Str( QNetworkInterface::InterfaceType i_type );
#endif
ZSIPCDLL_API QString qNetworkInterfaceFlags2Str( QNetworkInterface::InterfaceFlags i_flags );

} // namespace Ipc

} // namespace ZS

#endif // #ifndef ZSIpc_Common_h
