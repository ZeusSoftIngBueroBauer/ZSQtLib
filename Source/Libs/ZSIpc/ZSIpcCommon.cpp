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

#include <QtCore/qglobal.h>

#include "ZSIpc/ZSIpcCommon.h"
#include "ZSSys/ZSSysAux.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;


/******************************************************************************/
class CInitModuleIpcCommon
/* Please note:
   The class name should be unique for the whole system. Otherwise the compiler
   may be confused and using a CInitModule class from other modules to create
   the static InitModule instance.
*******************************************************************************/
{
public: // ctor
    CInitModuleIpcCommon()
    {
        qRegisterMetaType<SSocketDscr>("SSocketDscr");
        qRegisterMetaType<ZS::Ipc::SSocketDscr>("ZS::Ipc::SSocketDscr");
    }
};

static CInitModuleIpcCommon s_initModule;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
SrvCltType
==============================================================================*/

//------------------------------------------------------------------------------
static const ZS::System::SEnumEntry s_arEnumStrSrvCltType[] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( ESrvCltTypeServer,    "Server",    "Srv" ),
    /* 1 */ SEnumEntry( ESrvCltTypeClient,    "Client",    "Clt" ),
    /* 2 */ SEnumEntry( ESrvCltTypeCount,     "Count",     "All" ),
    /* 3 */ SEnumEntry( ESrvCltTypeUndefined, "Undefined", "?"   )
};

//------------------------------------------------------------------------------
QString ZS::Ipc::srvCltType2Str( int i_srvCltType, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrSrvCltType, _ZSArrLen(s_arEnumStrSrvCltType), i_srvCltType, i_alias);
}

//------------------------------------------------------------------------------
ESrvCltType ZS::Ipc::str2SrvCltType( const QString& i_str, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    ESrvCltType srvCltType = ESrvCltTypeUndefined;
    int iSrvCltType = SEnumEntry::str2Enumerator(s_arEnumStrSrvCltType, _ZSArrLen(s_arEnumStrSrvCltType), i_str, i_alias, Qt::CaseInsensitive);
    if( iSrvCltType >= 0 && iSrvCltType < ESrvCltTypeCount )
    {
        srvCltType = static_cast<ESrvCltType>(iSrvCltType);
    }
    return srvCltType;
}

/*==============================================================================
SocketType
==============================================================================*/

//------------------------------------------------------------------------------
static const SEnumEntry s_arEnumStrSocketType[] =
//------------------------------------------------------------------------------
{                                          // Name,        Symbol
    /* 0 */ SEnumEntry( ESocketTypeTcp,       "Tcp",       "Tcp"       ),
    /* 1 */ SEnumEntry( ESocketTypeShm,       "Shm",       "Shm"       ),
    /* 2 */ SEnumEntry( ESocketTypeInProcMsg, "InProcMsg", "InProcMsg" ),
    /* 3 */ SEnumEntry( ESocketTypeCount,     "Count",     "Count"     ),
    /* 4 */ SEnumEntry( ESocketTypeUndefined, "Undefined", "Undefined" )
};

//------------------------------------------------------------------------------
QString ZS::Ipc::socketType2Str( int i_socketType, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrSocketType, _ZSArrLen(s_arEnumStrSocketType), i_socketType, i_alias);
}

//------------------------------------------------------------------------------
ESocketType ZS::Ipc::str2SocketType( const QString& i_str, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    ESocketType socketType = ESocketTypeUndefined;
    int iSocketType = SEnumEntry::str2Enumerator(s_arEnumStrSocketType, _ZSArrLen(s_arEnumStrSocketType), i_str, i_alias, Qt::CaseInsensitive);
    if( iSocketType >= 0 && iSocketType < ESocketTypeCount )
    {
        socketType = static_cast<ESocketType>(iSocketType);
    }
    return socketType;
}

/*==============================================================================
Socket States
==============================================================================*/

//------------------------------------------------------------------------------
const SEnumEntry s_arEnumStrSocketStates[] =
//------------------------------------------------------------------------------
{                                     // Name,          Symbol
    SEnumEntry( ESocketStateUnconnected, "Unconnected", "Unconnected" ),
    SEnumEntry( ESocketStateHostLookup,  "HostLookup",  "HostLookup"  ),
    SEnumEntry( ESocketStateConnecting,  "Connecting",  "Connecting"  ),
    SEnumEntry( ESocketStateConnected,   "Connected",   "Connected"   ),
    SEnumEntry( ESocketStateBound,       "Bound",       "Bound"       ),
    SEnumEntry( ESocketStateListening,   "Listening",   "Listening"   ),
    SEnumEntry( ESocketStateClosing,     "Closing",     "Closing"     ),
    SEnumEntry( ESocketStateCount,       "Count",       "Count"       ),
    SEnumEntry( ESocketStateUndefined,   "Undefined",   "Undefined"   )
};

//------------------------------------------------------------------------------
QString ZS::Ipc::socketState2Str( int i_socketState, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrSocketStates, _ZSArrLen(s_arEnumStrSocketStates), i_socketState, i_alias);
}

/*==============================================================================
SocketId
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Ipc::socketId2Str( int i_iSocketId )
//------------------------------------------------------------------------------
{
    QString strSocketId;

    if( i_iSocketId >= ESocketIdMin && i_iSocketId <= ESocketIdMax )
    {
        strSocketId = QString::number(i_iSocketId);
    }
    else if( i_iSocketId == ESocketIdUndefined )
    {
        strSocketId = "Undefined";
    }
    else if( i_iSocketId == ESocketIdAllSockets )
    {
        strSocketId = "AllSockets";
    }
    else
    {
        strSocketId = "Invalid (" + QString::number(i_iSocketId) + ")";
    }
    return strSocketId;

} // socketId2Str

/*******************************************************************************
struct SSocketDscr
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
SSocketDscr::SSocketDscr() :
//------------------------------------------------------------------------------
    m_srvCltType(ESrvCltTypeUndefined),
    m_socketType(ESocketTypeUndefined),
    m_iSocketId(ESocketIdUndefined),
    m_socketState(ESocketStateUnconnected),
    m_uBufferSize(0),
    m_iConnectTimeout_ms(0),
    m_uServerListenPort(0),
    // Socket connection parameters (valid after connection is established)
    m_pObjLocal(nullptr),
    m_strLocalHostName(),
    m_hostAddrLocal(),
    m_uLocalPort(0),
    m_pObjRemote(nullptr),
    m_strRemoteHostName(),
    m_hostAddrRemote(),
    m_uRemotePort(0),
    m_strSocketName()
{
} // ctor

//------------------------------------------------------------------------------
SSocketDscr::SSocketDscr(
    ESrvCltType i_srvCltType,
    ESocketType i_socketType ) :
//------------------------------------------------------------------------------
    m_srvCltType(i_srvCltType),
    m_socketType(i_socketType),
    m_iSocketId(ESocketIdUndefined),
    m_socketState(ESocketStateUnconnected),
    m_uBufferSize(0),
    m_iConnectTimeout_ms(0),
    m_uServerListenPort(0),
    // Socket connection parameters (valid after connection is established)
    m_pObjLocal(nullptr),
    m_strLocalHostName(),
    m_hostAddrLocal(),
    m_uLocalPort(0),
    m_pObjRemote(nullptr),
    m_strRemoteHostName(),
    m_hostAddrRemote(),
    m_uRemotePort(0),
    m_strSocketName()
{
} // ctor

//------------------------------------------------------------------------------
SSocketDscr::SSocketDscr(
    QObject*     i_pObjSrvClt,
    ESrvCltType  i_srvCltType ) :
//------------------------------------------------------------------------------
    m_srvCltType(i_srvCltType),
    m_socketType(ESocketTypeInProcMsg),
    m_iSocketId(ESocketIdUndefined),
    m_socketState(ESocketStateUnconnected),
    m_uBufferSize(0),
    m_iConnectTimeout_ms(0),
    m_uServerListenPort(0),
    // Socket connection parameters (valid after connection is established)
    m_pObjLocal(i_pObjSrvClt),
    m_strLocalHostName(),
    m_hostAddrLocal(),
    m_uLocalPort(0),
    m_pObjRemote(nullptr),
    m_strRemoteHostName(),
    m_hostAddrRemote(),
    m_uRemotePort(0),
    m_strSocketName()
{
} // ctor

//------------------------------------------------------------------------------
SSocketDscr::SSocketDscr(
    unsigned int i_uServerListenPort,
    ESocketType  i_socketType,
    ESrvCltType  i_srvCltType ) :
//------------------------------------------------------------------------------
    m_srvCltType(i_srvCltType),
    m_socketType(i_socketType),
    m_iSocketId(ESocketIdUndefined),
    m_socketState(ESocketStateUnconnected),
    m_uBufferSize(0),
    m_iConnectTimeout_ms(0),
    m_uServerListenPort(i_uServerListenPort),
    // Socket connection parameters (valid after connection is established)
    m_pObjLocal(nullptr),
    m_strLocalHostName(),
    m_hostAddrLocal(),
    m_uLocalPort(0),
    m_pObjRemote(nullptr),
    m_strRemoteHostName(),
    m_hostAddrRemote(),
    m_uRemotePort(0),
    m_strSocketName()
{
} // ctor

//------------------------------------------------------------------------------
SSocketDscr::SSocketDscr(
    const QString& i_strRemoteHostName,
    unsigned int   i_uServerListenPort,
    ESocketType    i_socketType,
    ESrvCltType    i_srvCltType ) :
//------------------------------------------------------------------------------
    m_srvCltType(i_srvCltType),
    m_socketType(i_socketType),
    m_iSocketId(ESocketIdUndefined),
    m_socketState(ESocketStateUnconnected),
    m_uBufferSize(4096),
    m_iConnectTimeout_ms(30000),
    m_uServerListenPort(i_uServerListenPort),
    // Socket connection parameters (valid after connection is established)
    m_pObjLocal(nullptr),
    m_strLocalHostName(),
    m_hostAddrLocal(),
    m_uLocalPort(0),
    m_pObjRemote(nullptr),
    m_strRemoteHostName(i_strRemoteHostName),
    m_hostAddrRemote(),
    m_uRemotePort(0),
    m_strSocketName()
{
} // ctor

//------------------------------------------------------------------------------
SSocketDscr::SSocketDscr( const SSocketDscr& i_socketDscrOther ) :
//------------------------------------------------------------------------------
    m_srvCltType(i_socketDscrOther.m_srvCltType),
    m_socketType(i_socketDscrOther.m_socketType),
    m_iSocketId(i_socketDscrOther.m_iSocketId),
    m_socketState(i_socketDscrOther.m_socketState),
    m_uBufferSize(i_socketDscrOther.m_uBufferSize),
    m_iConnectTimeout_ms(i_socketDscrOther.m_iConnectTimeout_ms),
    m_uServerListenPort(i_socketDscrOther.m_uServerListenPort),
    // Socket connection parameters (valid after connection is established)
    m_pObjLocal(i_socketDscrOther.m_pObjLocal),
    m_strLocalHostName(i_socketDscrOther.m_strLocalHostName),
    m_hostAddrLocal(i_socketDscrOther.m_hostAddrLocal),
    m_uLocalPort(i_socketDscrOther.m_uLocalPort),
    m_pObjRemote(i_socketDscrOther.m_pObjRemote),
    m_strRemoteHostName(i_socketDscrOther.m_strRemoteHostName),
    m_hostAddrRemote(i_socketDscrOther.m_hostAddrRemote),
    m_uRemotePort(i_socketDscrOther.m_uRemotePort),
    m_strSocketName(i_socketDscrOther.m_strSocketName)
{
} // copy ctor

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
bool SSocketDscr::isValid() const
//------------------------------------------------------------------------------
{
    return ((m_srvCltType != ESrvCltTypeUndefined) && (m_socketState != ESocketStateUndefined));
}

//------------------------------------------------------------------------------
ESrvCltType SSocketDscr::getRemoteSrvCltType() const
//------------------------------------------------------------------------------
{
    if( m_srvCltType == ESrvCltTypeServer )
    {
        return ESrvCltTypeClient;
    }
    return ESrvCltTypeServer;
}

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
QString SSocketDscr::getConnectionString( int i_iDetailLevel ) const
//------------------------------------------------------------------------------
{
    QString str;

    if( m_socketType == ESocketTypeInProcMsg )
    {
        str = QString( m_pObjRemote == nullptr ? "nullptr" : m_pObjRemote->objectName() );
    }
    else
    {
        str = m_strRemoteHostName + ":" + QString::number(m_uServerListenPort);
    }

    if( i_iDetailLevel > 0 )
    {
        str += " {" + socketType2Str(m_socketType);
        str += ", " + srvCltType2Str(m_srvCltType);
        str += ", " + socketState2Str(m_socketState);

        if( i_iDetailLevel > 1 && m_socketState == ESocketStateConnected )
        {
            if( m_socketType == ESocketTypeInProcMsg )
            {
                str += " (LocObject: " + QString( m_pObjLocal == nullptr ? "nullptr" : m_pObjLocal->objectName() ) + ")";
            }
            else
            {
                str += " (LocPort: " + QString::number(m_uLocalPort);
                str += ", RemPort: " + QString::number(m_uRemotePort) + ")";
            }
        }
        str += "}";

    } // if( i_iDetailLevel > 0 )

    return str;

} // getConnectionString

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool SSocketDscr::operator == ( const SSocketDscr& i_socketDscrOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_srvCltType != i_socketDscrOther.m_srvCltType )
    {
        bEqual = false;
    }
    else if( m_socketType != i_socketDscrOther.m_socketType )
    {
        bEqual = false;
    }
    else if( m_iSocketId != i_socketDscrOther.m_iSocketId )
    {
        bEqual = false;
    }
    else if( m_socketState != i_socketDscrOther.m_socketState )
    {
        bEqual = false;
    }
    else if( m_uBufferSize != i_socketDscrOther.m_uBufferSize )
    {
        bEqual = false;
    }
    else if( m_iConnectTimeout_ms != i_socketDscrOther.m_iConnectTimeout_ms )
    {
        bEqual = false;
    }
    else if( m_uServerListenPort != i_socketDscrOther.m_uServerListenPort )
    {
        bEqual = false;
    }
    else if( m_pObjLocal != i_socketDscrOther.m_pObjLocal )
    {
        bEqual = false;
    }
    else if( m_strLocalHostName != i_socketDscrOther.m_strLocalHostName )
    {
        bEqual = false;
    }
    else if( m_uLocalPort != i_socketDscrOther.m_uLocalPort )
    {
        bEqual = false;
    }
    else if( m_pObjRemote != i_socketDscrOther.m_pObjRemote )
    {
        bEqual = false;
    }
    else if( m_strRemoteHostName != i_socketDscrOther.m_strRemoteHostName )
    {
        bEqual = false;
    }
    else if( m_uRemotePort != i_socketDscrOther.m_uRemotePort )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool SSocketDscr::operator != ( const SSocketDscr& i_socketDscrOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_socketDscrOther);
}


/*******************************************************************************
struct SServerHostSettings
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SServerHostSettings::SServerHostSettings() :
//------------------------------------------------------------------------------
    m_socketType(ESocketTypeUndefined),
    m_pObjLocal(nullptr),
    m_strLocalHostName(),
    m_hostAddrLocal(),
    m_uLocalPort(0),
    m_uMaxPendingConnections(0),
    m_uBufferSize(0)
{
} // ctor

//------------------------------------------------------------------------------
SServerHostSettings::SServerHostSettings( ESocketType i_socketType ) :
//------------------------------------------------------------------------------
    m_socketType(i_socketType),
    m_pObjLocal(nullptr),
    m_strLocalHostName(),
    m_hostAddrLocal(),
    m_uLocalPort(0),
    m_uMaxPendingConnections(0),
    m_uBufferSize(0)
{
    if( i_socketType == ESocketTypeTcp )
    {
        m_strLocalHostName = "127.0.0.1";
        m_uLocalPort = 24763;
        m_uMaxPendingConnections = 30;
    }
    else if( i_socketType == ESocketTypeShm )
    {
        m_uLocalPort = 24763;
        m_uMaxPendingConnections = 30;
        m_uBufferSize = 4096;
    }
    else if( i_socketType == ESocketTypeInProcMsg )
    {
    }

} // ctor

//------------------------------------------------------------------------------
SServerHostSettings::SServerHostSettings(
    const QString& i_strLocalHostName,
    quint16        i_uLocalPort,
    unsigned int   i_uMaxPendingConnections ) :
//------------------------------------------------------------------------------
    m_socketType(ESocketTypeTcp),
    m_pObjLocal(nullptr),
    m_strLocalHostName(i_strLocalHostName),
    m_hostAddrLocal(),
    m_uLocalPort(i_uLocalPort),
    m_uMaxPendingConnections(i_uMaxPendingConnections),
    m_uBufferSize(0)
{
} // ctor

//------------------------------------------------------------------------------
SServerHostSettings::SServerHostSettings(
    QObject*     i_pObjLocal,
    unsigned int i_uMaxPendingConnections ) :
//------------------------------------------------------------------------------
    m_socketType(ESocketTypeInProcMsg),
    m_pObjLocal(i_pObjLocal),
    m_strLocalHostName(),
    m_hostAddrLocal(),
    m_uLocalPort(0),
    m_uMaxPendingConnections(i_uMaxPendingConnections),
    m_uBufferSize(4096)
{
} // ctor

//------------------------------------------------------------------------------
SServerHostSettings::SServerHostSettings( const SSocketDscr& i_socketDscr ) :
//------------------------------------------------------------------------------
    m_socketType(i_socketDscr.m_socketType),
    m_pObjLocal(i_socketDscr.m_pObjLocal),
    m_strLocalHostName(i_socketDscr.m_strLocalHostName),
    m_hostAddrLocal(i_socketDscr.m_hostAddrLocal),
    m_uLocalPort(i_socketDscr.m_uLocalPort),
    m_uMaxPendingConnections(30),
    m_uBufferSize(i_socketDscr.m_uBufferSize)
{
} // copy ctor

////------------------------------------------------------------------------------
//SServerHostSettings::SServerHostSettings( const QString& i_strCnct ) :
////------------------------------------------------------------------------------
//    m_socketType(ESocketTypeTcp),
//    m_pObjLocal(nullptr),
//    m_strLocalHostName("127.0.0.1"),
//    m_hostAddrLocal(),
//    m_uLocalPort(24763),
//    m_uMaxPendingConnections(30),
//    m_uBufferSize(4096)
//{
//} // ctor

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
void SServerHostSettings::toSocketDscr( SSocketDscr& i_socketDscr ) const
//------------------------------------------------------------------------------
{
    i_socketDscr.m_socketType = m_socketType;
    //i_socketDscr.m_iSocketId;
    //i_socketDscr.m_socketState;
    i_socketDscr.m_uBufferSize = m_uBufferSize;
    //i_socketDscr.m_iConnectTimeout_ms;
    //i_socketDscr.m_uServerListenPort;
    i_socketDscr.m_pObjLocal = m_pObjLocal;
    i_socketDscr.m_strLocalHostName = m_strLocalHostName;
    //i_socketDscr.m_hostAddrLocal;
    i_socketDscr.m_uLocalPort = m_uLocalPort;
    //i_socketDscr.m_pObjRemote;
    //i_socketDscr.m_strRemoteHostName;
    //i_socketDscr.m_hostAddrRemote;
    //i_socketDscr.m_uRemotePort;

} // toSocketDscr

//------------------------------------------------------------------------------
SSocketDscr SServerHostSettings::getSocketDscr() const
//------------------------------------------------------------------------------
{
    SSocketDscr socketDscr(ESrvCltTypeServer,m_socketType);

    //socketDscr.m_iSocketId;
    //socketDscr.m_socketState;
    socketDscr.m_uBufferSize = m_uBufferSize;
    //socketDscr.m_iConnectTimeout_ms;
    //socketDscr.m_uServerListenPort;
    socketDscr.m_pObjLocal = m_pObjLocal;
    socketDscr.m_strLocalHostName = m_strLocalHostName;
    //socketDscr.m_hostAddrLocal;
    socketDscr.m_uLocalPort = m_uLocalPort;
    //socketDscr.m_pObjRemote;
    //socketDscr.m_strRemoteHostName;
    //socketDscr.m_hostAddrRemote;
    //socketDscr.m_uRemotePort;

    return socketDscr;

} // getSocketDscr

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
QString SServerHostSettings::getConnectionString( int i_iDetailLevel ) const
//------------------------------------------------------------------------------
{
    QString str;

    if( m_socketType == ESocketTypeInProcMsg )
    {
        str = QString( m_pObjLocal == nullptr ? "nullptr" : m_pObjLocal->objectName() );
    }
    else
    {
        str = m_strLocalHostName + ":" + QString::number(m_uLocalPort);
    }

    if( i_iDetailLevel > 0 )
    {
        str += " {" + socketType2Str(m_socketType) + "}";
    }

    return str;

} // getConnectionString

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
SServerHostSettings& SServerHostSettings::operator = ( const SServerHostSettings& i_settingsOther )
//------------------------------------------------------------------------------
{
    m_socketType = i_settingsOther.m_socketType;
    m_pObjLocal = i_settingsOther.m_pObjLocal;
    m_strLocalHostName = i_settingsOther.m_strLocalHostName;
    m_hostAddrLocal = i_settingsOther.m_hostAddrLocal;
    m_uLocalPort = i_settingsOther.m_uLocalPort;
    m_uMaxPendingConnections = i_settingsOther.m_uMaxPendingConnections;
    m_uBufferSize = i_settingsOther.m_uBufferSize;

    return *this;

} // operator =

//------------------------------------------------------------------------------
SServerHostSettings& SServerHostSettings::operator = ( const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    m_socketType = i_socketDscr.m_socketType;
    m_pObjLocal = i_socketDscr.m_pObjLocal;
    m_strLocalHostName = i_socketDscr.m_strLocalHostName;
    m_hostAddrLocal = i_socketDscr.m_hostAddrLocal;
    m_uLocalPort = i_socketDscr.m_uLocalPort;
    m_uMaxPendingConnections = 30;
    m_uBufferSize = i_socketDscr.m_uBufferSize;

    return *this;

} // operator =

//------------------------------------------------------------------------------
bool SServerHostSettings::operator == ( const SServerHostSettings& i_settingsOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_socketType != i_settingsOther.m_socketType )
    {
        bEqual = false;
    }
    else if( m_pObjLocal != i_settingsOther.m_pObjLocal )
    {
        bEqual = false;
    }
    //else if( m_strLocalHostName != i_settingsOther.m_strLocalHostName )
    //{
    //    bEqual = false;
    //}
    else if( m_uLocalPort != i_settingsOther.m_uLocalPort )
    {
        bEqual = false;
    }
    else if( m_uMaxPendingConnections != i_settingsOther.m_uMaxPendingConnections )
    {
        bEqual = false;
    }
    else if( m_socketType == ESocketTypeShm && m_uBufferSize != i_settingsOther.m_uBufferSize )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool SServerHostSettings::operator != ( const SServerHostSettings& i_settingsOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_settingsOther);
}

//------------------------------------------------------------------------------
bool SServerHostSettings::operator == ( const SSocketDscr& i_socketDscr ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_socketType != i_socketDscr.m_socketType )
    {
        bEqual = false;
    }
    else if( m_pObjLocal != i_socketDscr.m_pObjLocal )
    {
        bEqual = false;
    }
    //else if( m_strLocalHostName != i_socketDscr.m_strLocalHostName )
    //{
    //    bEqual = false;
    //}
    else if( m_uLocalPort != i_socketDscr.m_uLocalPort )
    {
        bEqual = false;
    }
    else if( m_socketType == ESocketTypeShm && m_uBufferSize != i_socketDscr.m_uBufferSize )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool SServerHostSettings::operator != ( const SSocketDscr& i_socketDscr ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_socketDscr);
}


/*******************************************************************************
struct SClientHostSettings
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SClientHostSettings::SClientHostSettings() :
//------------------------------------------------------------------------------
    m_socketType(ESocketTypeUndefined),
    m_pObjRemote(nullptr),
    m_strRemoteHostName(),
    m_uRemotePort(0),
    m_iConnectTimeout_ms(0),
    m_uBufferSize(0)
{
} // ctor

//------------------------------------------------------------------------------
SClientHostSettings::SClientHostSettings( ESocketType i_socketType ) :
//------------------------------------------------------------------------------
    m_socketType(i_socketType),
    m_pObjRemote(nullptr),
    m_strRemoteHostName(),
    m_uRemotePort(0),
    m_iConnectTimeout_ms(5000),
    m_uBufferSize()
{
    if( i_socketType == ESocketTypeTcp )
    {
        m_strRemoteHostName = "127.0.0.1";
        m_uRemotePort = 24763;
    }
    else if( i_socketType == ESocketTypeShm )
    {
        m_uRemotePort = 24763;
    }
    else if( i_socketType == ESocketTypeInProcMsg )
    {
    }

} // ctor

//------------------------------------------------------------------------------
SClientHostSettings::SClientHostSettings(
    const QString& i_strRemoteHostName,
    quint16        i_uRemotePort,
    int            i_iConnectTimeout_ms ) :
//------------------------------------------------------------------------------
    m_socketType(ESocketTypeTcp),
    m_pObjRemote(nullptr),
    m_strRemoteHostName(i_strRemoteHostName),
    m_uRemotePort(i_uRemotePort),
    m_iConnectTimeout_ms(i_iConnectTimeout_ms),
    m_uBufferSize(0)
{
} // ctor

//------------------------------------------------------------------------------
SClientHostSettings::SClientHostSettings(
    QObject* i_pObjRemote,
    int      i_iConnectTimeout_ms ) :
//------------------------------------------------------------------------------
    m_socketType(ESocketTypeInProcMsg),
    m_pObjRemote(i_pObjRemote),
    m_strRemoteHostName(),
    m_uRemotePort(0),
    m_iConnectTimeout_ms(i_iConnectTimeout_ms),
    m_uBufferSize(4096)
{
} // ctor

//------------------------------------------------------------------------------
SClientHostSettings::SClientHostSettings( const SSocketDscr& i_socketDscr ) :
//------------------------------------------------------------------------------
    m_socketType(i_socketDscr.m_socketType),
    m_pObjRemote(i_socketDscr.m_pObjRemote),
    m_strRemoteHostName(i_socketDscr.m_strRemoteHostName),
    m_uRemotePort(i_socketDscr.m_uServerListenPort),
    m_iConnectTimeout_ms(i_socketDscr.m_iConnectTimeout_ms),
    m_uBufferSize(i_socketDscr.m_uBufferSize)
{
} // copy ctor

////------------------------------------------------------------------------------
//SClientHostSettings::SClientHostSettings( const QString& i_strCnct ) :
////------------------------------------------------------------------------------
//    m_socketType(ESocketTypeTcp),
//    m_pObjRemote(nullptr),
//    m_strRemoteHostName("127.0.0.1"),
//    m_uRemotePort(24763),
//    m_iConnectTimeout_ms(5000),
//    m_uBufferSize(4096)
//{
//} // ctor

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
void SClientHostSettings::toSocketDscr( SSocketDscr& i_socketDscr ) const
//------------------------------------------------------------------------------
{
    i_socketDscr.m_socketType = m_socketType;
    //i_socketDscr.m_iSocketId;
    //i_socketDscr.m_socketState;
    i_socketDscr.m_uBufferSize = m_uBufferSize;
    i_socketDscr.m_iConnectTimeout_ms = m_iConnectTimeout_ms;
    i_socketDscr.m_uServerListenPort = m_uRemotePort;
    //i_socketDscr.m_pObjLocal;
    //i_socketDscr.m_strLocalHostName;
    //i_socketDscr.m_hostAddrLocal;
    //i_socketDscr.m_uLocalPort;
    i_socketDscr.m_pObjRemote = m_pObjRemote;
    i_socketDscr.m_strRemoteHostName = m_strRemoteHostName;
    //i_socketDscr.m_hostAddrRemote;
    //i_socketDscr.m_uRemotePort; // Set if connection is established

} // toSocketDscr

//------------------------------------------------------------------------------
SSocketDscr SClientHostSettings::getSocketDscr() const
//------------------------------------------------------------------------------
{
    SSocketDscr socketDscr(ESrvCltTypeClient,m_socketType);

    //socketDscr.m_iSocketId;
    //socketDscr.m_socketState;
    socketDscr.m_uBufferSize = m_uBufferSize;
    socketDscr.m_iConnectTimeout_ms = m_iConnectTimeout_ms;
    socketDscr.m_uServerListenPort = m_uRemotePort;
    //socketDscr.m_pObjLocal;
    //socketDscr.m_strLocalHostName;
    //socketDscr.m_hostAddrLocal;
    //socketDscr.m_uLocalPort;
    socketDscr.m_pObjRemote = m_pObjRemote;
    socketDscr.m_strRemoteHostName = m_strRemoteHostName;
    //socketDscr.m_hostAddrRemote;
    //socketDscr.m_uRemotePort; // Set if connection is established

    return socketDscr;

} // getSocketDscr

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
QString SClientHostSettings::getConnectionString( int i_iDetailLevel ) const
//------------------------------------------------------------------------------
{
    QString str;

    if( m_socketType == ESocketTypeInProcMsg )
    {
        if( m_pObjRemote == nullptr && m_strRemoteHostName.isEmpty() )
        {
            str = "nullptr";
        }
        else if( m_pObjRemote != nullptr )
        {
            str =  m_pObjRemote->objectName();
        }
        else // if( !m_strRemoteHostName.isEmpty() )
        {
            str = m_strRemoteHostName;
        }
    }
    else
    {
        str = m_strRemoteHostName + ":" + QString::number(m_uRemotePort);
    }

    if( i_iDetailLevel > 0 )
    {
        str += " {" + socketType2Str(m_socketType) + "}";
    }

    return str;

} // getConnectionString

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
SClientHostSettings& SClientHostSettings::operator = ( const SClientHostSettings& i_settingsOther )
//------------------------------------------------------------------------------
{
    m_socketType = i_settingsOther.m_socketType;
    m_pObjRemote = i_settingsOther.m_pObjRemote;
    m_strRemoteHostName = i_settingsOther.m_strRemoteHostName;
    m_uRemotePort = i_settingsOther.m_uRemotePort;
    m_iConnectTimeout_ms = i_settingsOther.m_iConnectTimeout_ms;
    m_uBufferSize = i_settingsOther.m_uBufferSize;

    return *this;

} // operator =

//------------------------------------------------------------------------------
SClientHostSettings& SClientHostSettings::operator = ( const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    m_socketType = i_socketDscr.m_socketType;
    m_pObjRemote = i_socketDscr.m_pObjRemote;
    m_strRemoteHostName = i_socketDscr.m_strRemoteHostName;
    m_uRemotePort = i_socketDscr.m_uServerListenPort;
    m_iConnectTimeout_ms = i_socketDscr.m_iConnectTimeout_ms;
    m_uBufferSize = i_socketDscr.m_uBufferSize;

    return *this;

} // operator =

//------------------------------------------------------------------------------
bool SClientHostSettings::operator == ( const SClientHostSettings& i_settingsOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_socketType != i_settingsOther.m_socketType )
    {
        bEqual = false;
    }
    else if( m_pObjRemote != i_settingsOther.m_pObjRemote )
    {
        bEqual = false;
    }
    else if( m_strRemoteHostName != i_settingsOther.m_strRemoteHostName )
    {
        bEqual = false;
    }
    else if( m_uRemotePort != i_settingsOther.m_uRemotePort )
    {
        bEqual = false;
    }
    else if( m_iConnectTimeout_ms != i_settingsOther.m_iConnectTimeout_ms )
    {
        bEqual = false;
    }
    else if( m_socketType == ESocketTypeShm )
    {
        if( m_uBufferSize != i_settingsOther.m_uBufferSize )
        {
            bEqual = false;
        }
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool SClientHostSettings::operator != ( const SClientHostSettings& i_settingsOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_settingsOther);
}

//------------------------------------------------------------------------------
bool SClientHostSettings::operator == ( const SSocketDscr& i_socketDscr ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_socketType != i_socketDscr.m_socketType )
    {
        bEqual = false;
    }
    else if( m_pObjRemote != i_socketDscr.m_pObjRemote )
    {
        bEqual = false;
    }
    else if( m_strRemoteHostName != i_socketDscr.m_strRemoteHostName )
    {
        bEqual = false;
    }
    else if( m_uRemotePort != i_socketDscr.m_uServerListenPort )
    {
        bEqual = false;
    }
    else if( m_iConnectTimeout_ms != i_socketDscr.m_iConnectTimeout_ms )
    {
        bEqual = false;
    }
    else if( m_uBufferSize != i_socketDscr.m_uBufferSize )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool SClientHostSettings::operator != ( const SSocketDscr& i_socketDscr ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_socketDscr);
}


/*******************************************************************************
struct STimerSettings
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
STimerSettings::STimerSettings(
    bool i_bEnabled,
    int  i_iInterval_ms,
    int  i_iTimeout_ms ) :
//------------------------------------------------------------------------------
    m_bEnabled(i_bEnabled),
    m_iInterval_ms(i_iInterval_ms),
    m_iTimeout_ms(i_iTimeout_ms)
{
} // ctor

//------------------------------------------------------------------------------
STimerSettings::STimerSettings( const STimerSettings& i_other ) :
//------------------------------------------------------------------------------
    m_bEnabled(i_other.m_bEnabled),
    m_iInterval_ms(i_other.m_iInterval_ms),
    m_iTimeout_ms(i_other.m_iTimeout_ms)
{
} // ctor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
STimerSettings& STimerSettings::operator = ( const STimerSettings& i_other )
//------------------------------------------------------------------------------
{
    m_bEnabled = i_other.m_bEnabled;
    m_iInterval_ms = i_other.m_iInterval_ms;
    m_iTimeout_ms = i_other.m_iTimeout_ms;

    return *this;

} // operator = STimerSettings

//------------------------------------------------------------------------------
bool STimerSettings::operator == ( const STimerSettings& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_bEnabled != i_other.m_bEnabled )
    {
        bEqual = false;
    }
    else if( m_iInterval_ms != i_other.m_iInterval_ms )
    {
        bEqual = false;
    }
    else if( m_iTimeout_ms != i_other.m_iTimeout_ms )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool STimerSettings::operator != ( const STimerSettings& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
QString STimerSettings::toString() const
//------------------------------------------------------------------------------
{
    QString str;

    str = "Enabled: " + bool2Str(m_bEnabled);
    str += ", Interval: " + QString::number(m_iInterval_ms);
    str += ", Timeout: " + QString::number(m_iTimeout_ms);

    return str;

} // toString;
