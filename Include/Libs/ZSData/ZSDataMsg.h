/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSData_ConnectionMsg_h
#define ZSData_ConnectionMsg_h

#include "ZSData/ZSDataCommon.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysMsg.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace Ipc
{
class CIpcSocketWrapper;
}

namespace Data
{
//class CChangeDataSet;
//class CDSSchemaChgDscr;
//class CDSDataChgDscr;

typedef enum
{
    EMsgTypeMin               = (Ipc::EMsgTypeMax+1),
    EMsgTypeReqReceiveData    = (EMsgTypeMin +  0),
    EMsgTypeReqStartup        = (EMsgTypeMin +  1),
    EMsgTypeConStartup        = (EMsgTypeMin +  2),
    EMsgTypeReqShutdown       = (EMsgTypeMin +  3),
    EMsgTypeConShutdown       = (EMsgTypeMin +  4),
    EMsgTypeReqConnect        = (EMsgTypeMin +  5),
    EMsgTypeConConnect        = (EMsgTypeMin +  6),
    EMsgTypeIndConnected      = (EMsgTypeMin +  7),
    EMsgTypeReqDisconnect     = (EMsgTypeMin +  8),
    EMsgTypeConDisconnect     = (EMsgTypeMin +  9),
    EMsgTypeIndDisconnected   = (EMsgTypeMin + 10),
    EMsgTypeReqChangeSettings = (EMsgTypeMin + 11),
    EMsgTypeConChangeSettings = (EMsgTypeMin + 12),
    // "Simple" Commands
    EMsgTypeReqReadSchema     = (EMsgTypeMin + 13),
    EMsgTypeConReadSchema     = (EMsgTypeMin + 14),
    EMsgTypeReqSaveSchema     = (EMsgTypeMin + 15),
    EMsgTypeConSaveSchema     = (EMsgTypeMin + 16),
    EMsgTypeReqReadData       = (EMsgTypeMin + 17),
    EMsgTypeConReadData       = (EMsgTypeMin + 18),
    EMsgTypeReqSaveData       = (EMsgTypeMin + 19),
    EMsgTypeConSaveData       = (EMsgTypeMin + 20),
    EMsgTypeReqSet2Default    = (EMsgTypeMin + 21),
    EMsgTypeConSet2Default    = (EMsgTypeMin + 22),
    // With Node Specs or Data Sets (ConSelectSchema and ConSelectData)
    EMsgTypeReqRegister       = (EMsgTypeMin + 23),
    EMsgTypeConRegister       = (EMsgTypeMin + 24),
    EMsgTypeReqUnregister     = (EMsgTypeMin + 25),
    EMsgTypeConUnregister     = (EMsgTypeMin + 26),
    EMsgTypeReqSelectSchema   = (EMsgTypeMin + 27),
    EMsgTypeConSelectSchema   = (EMsgTypeMin + 28),
    EMsgTypeReqSelectData     = (EMsgTypeMin + 29),
    EMsgTypeConSelectData     = (EMsgTypeMin + 30),
    // With Data Sets
    EMsgTypeReqUpdateSchema   = (EMsgTypeMin + 31),
    EMsgTypeConUpdateSchema   = (EMsgTypeMin + 32),
    EMsgTypeReqUpdateData     = (EMsgTypeMin + 33),
    EMsgTypeConUpdateData     = (EMsgTypeMin + 34),
    EMsgTypeMax               = (EMsgTypeMin + 34),
    EMsgTypeCount             = (EMsgTypeMax-EMsgTypeMin+1),
    EMsgTypeUndefined
}   EMsgType;

ZSDATADLL_API QString msgType2Str( int i_iMsgType );


//******************************************************************************
class CMsgClassFactory
//******************************************************************************
{
public: // class methods
    static ZS::System::CMsg* CreateInstance( const QByteArray& i_byteArr, ZS::Ipc::CIpcSocketWrapper* i_pIpcSocketWrapper );
};


//******************************************************************************
class ZSDATADLL_API CMsgReqReceiveData : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqReceiveData(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        int      i_iSocketId,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqReceiveData();
public: // instance methods
    int getSocketId() const { return m_iSocketId; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    int m_iSocketId;

}; // CMsgReqReceiveData


//******************************************************************************
class ZSDATADLL_API CMsgReqStartup : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqStartup(
        QObject*                        i_pObjSender,
        QObject*                        i_pObjReceiver,
        EProtocolType                   i_protocolType,
        const Ipc::SServerHostSettings& i_hostSettings,
        bool                            i_bMustBeConfirmed = true,
        qint64                          i_iReqId = -1,
        qint64                          i_iMsgId = -1 );
    CMsgReqStartup(
        QObject*                        i_pObjSender,
        QObject*                        i_pObjReceiver,
        EProtocolType                   i_protocolType,
        const Ipc::SClientHostSettings& i_hostSettings,
        bool                            i_bMustBeConfirmed = true,
        qint64                          i_iReqId = -1,
        qint64                          i_iMsgId = -1 );
    CMsgReqStartup(
        QObject*                        i_pObjSender,
        QObject*                        i_pObjReceiver,
        EProtocolType                   i_protocolType,
        const Ipc::SClientHostSettings& i_hostSettings,
        const Ipc::STimerSettings&      i_watchDogSettings,
        bool                            i_bMustBeConfirmed = true,
        qint64                          i_iReqId = -1,
        qint64                          i_iMsgId = -1 );
    virtual ~CMsgReqStartup();
public: // instance methods
    Ipc::ESrvCltType getSrvCltType() const { return m_srvCltType; }
    EProtocolType getProtocolType() const { return m_protocolType; }
    Ipc::ESocketType getSocketType() const;
    Ipc::SServerHostSettings getServerHostSettings() const { return m_serverHostSettings; }
    Ipc::SClientHostSettings getClientHostSettings() const { return m_clientHostSettings; }
    Ipc::STimerSettings* getWatchDogSettings() const { return m_pWatchDogSettings; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    Ipc::ESrvCltType         m_srvCltType;
    EProtocolType            m_protocolType;
    Ipc::SServerHostSettings m_serverHostSettings;
    Ipc::SClientHostSettings m_clientHostSettings;
    Ipc::STimerSettings*     m_pWatchDogSettings;

}; // CMsgReqStartup


//******************************************************************************
class ZSDATADLL_API CMsgConStartup : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConStartup(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    virtual ~CMsgConStartup();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgConStartup


//******************************************************************************
class ZSDATADLL_API CMsgReqShutdown : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqShutdown(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = true,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqShutdown();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgReqShutdown


//******************************************************************************
class ZSDATADLL_API CMsgConShutdown : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConShutdown(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    virtual ~CMsgConShutdown();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgConShutdown


//******************************************************************************
class ZSDATADLL_API CMsgReqConnect : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqConnect( // Sent from clients to their socket gateways or for InProcMsg to database object.
        QObject*                        i_pObjSender,
        QObject*                        i_pObjReceiver,
        EProtocolType                   i_protocolType,
        const Ipc::SClientHostSettings& i_hostSettings,
        bool                            i_bMustBeConfirmed = true,
        qint64                          i_iReqId = -1,
        qint64                          i_iMsgId = -1 );
    CMsgReqConnect( // Sent from clients to their socket gateways or for InProcMsg to database object.
        QObject*                        i_pObjSender,
        QObject*                        i_pObjReceiver,
        EProtocolType                   i_protocolType,
        const Ipc::SClientHostSettings& i_hostSettings,
        const Ipc::STimerSettings&      i_watchDogSettings,
        bool                            i_bMustBeConfirmed = true,
        qint64                          i_iReqId = -1,
        qint64                          i_iMsgId = -1 );
    virtual ~CMsgReqConnect();
public: // instance methods
    EProtocolType getProtocolType() const { return m_protocolType; }
    Ipc::ESocketType getSocketType() const { return m_hostSettings.m_socketType; }
    Ipc::SClientHostSettings getHostSettings() const { return m_hostSettings; }
    Ipc::STimerSettings* getWatchDogSettings() const { return m_pWatchDogSettings; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    EProtocolType            m_protocolType;
    Ipc::SClientHostSettings m_hostSettings;
    Ipc::STimerSettings*     m_pWatchDogSettings;

}; // CMsgReqConnect


//******************************************************************************
class ZSDATADLL_API CMsgConConnect : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConConnect(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        EProtocolType                     i_protocolType,
        const Ipc::SSocketDscr&           i_socketDscr,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    virtual ~CMsgConConnect();
public: // instance methods
    EProtocolType getProtocolType() const { return m_protocolType; }
public: // instance methods
    Ipc::ESocketState getSocketState() const { return m_socketDscr.m_socketState; }
    void setSocketState( Ipc::ESocketState i_socketState );
    Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    void setSocketDscr( const Ipc::SSocketDscr& i_socketDscr );
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    EProtocolType    m_protocolType;
    Ipc::SSocketDscr m_socketDscr;

}; // CMsgConConnect


//******************************************************************************
class ZSDATADLL_API CMsgIndConnected : public ZS::System::CMsgInd
//******************************************************************************
{
public: // ctors and dtor
    CMsgIndConnected(
        QObject*                i_pObjSender,
        QObject*                i_pObjReceiver,
        EProtocolType           i_protocolType,
        const Ipc::SSocketDscr& i_socketDscr,
        qint64                  i_iMsgId = -1 );
    virtual ~CMsgIndConnected();
public: // instance methods
    EProtocolType getProtocolType() const { return m_protocolType; }
    Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgAck* createAcknowledgeMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    EProtocolType    m_protocolType;
    Ipc::SSocketDscr m_socketDscr;

}; // CMsgIndConnected


//******************************************************************************
class ZSDATADLL_API CMsgReqDisconnect : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqDisconnect(
        QObject*                i_pObjSender,
        QObject*                i_pObjReceiver,
        EProtocolType           i_protocolType,
        const Ipc::SSocketDscr& i_socketDscr,
        bool                    i_bMustBeConfirmed = true,
        qint64                  i_iReqId = -1,
        qint64                  i_iMsgId = -1 );
    virtual ~CMsgReqDisconnect();
public: // instance methods
    EProtocolType getProtocolType() const { return m_protocolType; }
    Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    EProtocolType    m_protocolType;
    Ipc::SSocketDscr m_socketDscr;

}; // CMsgReqDisconnect


//******************************************************************************
class ZSDATADLL_API CMsgConDisconnect : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConDisconnect(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        EProtocolType                     i_protocolType,
        const Ipc::SSocketDscr&           i_socketDscr,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    virtual ~CMsgConDisconnect();
public: // instance methods
    EProtocolType getProtocolType() const { return m_protocolType; }
    Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    EProtocolType    m_protocolType;
    Ipc::SSocketDscr m_socketDscr;

}; // CMsgConDisconnect


//******************************************************************************
class ZSDATADLL_API CMsgIndDisconnected : public ZS::System::CMsgInd
//******************************************************************************
{
public: // ctors and dtor
    CMsgIndDisconnected(
        QObject*                i_pObjSender,
        QObject*                i_pObjReceiver,
        EProtocolType           i_protocolType,
        const Ipc::SSocketDscr& i_socketDscr,
        qint64                  i_iMsgId = -1 );
    virtual ~CMsgIndDisconnected();
public: // instance methods
    EProtocolType getProtocolType() const { return m_protocolType; }
    Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgAck* createAcknowledgeMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    EProtocolType    m_protocolType;
    Ipc::SSocketDscr m_socketDscr;

}; // CMsgIndDisconnected


//******************************************************************************
class ZSDATADLL_API CMsgReqChangeSettings : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqChangeSettings(  // ctor for servers
        QObject*                        i_pObjSender,
        QObject*                        i_pObjReceiver,
        EProtocolType                   i_protocolType,
        const Ipc::SServerHostSettings& i_hostSettings,
        bool                            i_bMustBeConfirmed = true,
        qint64                          i_iReqId = -1,
        qint64                          i_iMsgId = -1 );
    CMsgReqChangeSettings(  // ctor for clients
        QObject*                        i_pObjSender,
        QObject*                        i_pObjReceiver,
        EProtocolType                   i_protocolType,
        const Ipc::SClientHostSettings& i_hostSettings,
        bool                            i_bMustBeConfirmed = true,
        qint64                          i_iReqId = -1,
        qint64                          i_iMsgId = -1 );
    CMsgReqChangeSettings(  // ctor for clients
        QObject*                        i_pObjSender,
        QObject*                        i_pObjReceiver,
        EProtocolType                   i_protocolType,
        const Ipc::SClientHostSettings& i_hostSettings,
        const Ipc::STimerSettings&      i_watchDogSettings,
        bool                            i_bMustBeConfirmed = true,
        qint64                          i_iReqId = -1,
        qint64                          i_iMsgId = -1 );
    virtual ~CMsgReqChangeSettings();
public: // instance methods
    Ipc::ESrvCltType getSrvCltType() const { return m_srvCltType; }
    EProtocolType getProtocolType() const { return m_protocolType; }
    Ipc::ESocketType getSocketType() const;
    Ipc::SServerHostSettings getServerHostSettings() const { return m_serverHostSettings; }
    Ipc::SClientHostSettings getClientHostSettings() const { return m_clientHostSettings; }
    Ipc::STimerSettings* getWatchDogSettings() const { return m_pWatchDogSettings; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    Ipc::ESrvCltType         m_srvCltType;
    EProtocolType            m_protocolType;
    Ipc::SServerHostSettings m_serverHostSettings;
    Ipc::SClientHostSettings m_clientHostSettings;
    Ipc::STimerSettings*     m_pWatchDogSettings;

}; // CMsgReqChangeSettings


//******************************************************************************
class ZSDATADLL_API CMsgConChangeSettings : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConChangeSettings( // ctor for servers
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        EProtocolType                     i_protocolType,
        const Ipc::SServerHostSettings&   i_hostSettings,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConChangeSettings( // ctor for clients
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        EProtocolType                     i_protocolType,
        const Ipc::SSocketDscr&           i_socketDscr,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    virtual ~CMsgConChangeSettings();
public: // instance methods
    Ipc::ESrvCltType getSrvCltType() const { return m_srvCltType; }
    EProtocolType getProtocolType() const { return m_protocolType; }
    Ipc::ESocketType getSocketType() const;
    Ipc::SServerHostSettings getServerHostSettings() const { return m_serverHostSettings; }
    Ipc::SSocketDscr getClientSocketDscr() const { return m_clientSocketDscr; }
    int getClientSocketId() const { return m_clientSocketDscr.m_iSocketId; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    Ipc::ESrvCltType         m_srvCltType;
    EProtocolType            m_protocolType;
    Ipc::SServerHostSettings m_serverHostSettings;
    Ipc::SSocketDscr         m_clientSocketDscr;

}; // CMsgConChangeSettings


//******************************************************************************
class ZSDATADLL_API CMsgReq : public ZS::System::CMsgReq
//******************************************************************************
{
protected: // ctors (pure virtual base class)
    CMsgReq(
        int            i_iMsgType,
        QObject*       i_pObjSender,
        QObject*       i_pObjReceiver,
        const SCnctId& i_cnctId,
        bool           i_bMustBeConfirmed = false,
        qint64         i_iReqId = -1,
        qint64         i_iMsgId = -1 );
    CMsgReq( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
public: // dtor
    virtual ~CMsgReq();
public: // instance methods
    virtual void setCnctId( const SCnctId& i_cnctId );
    SCnctId getCnctId() const { return m_cnctId; }
public: // instance methods
    EProtocolType getProtocolType() const { return m_cnctId.m_protocolType; }
    Ipc::ESocketType getSocketType() const { return m_cnctId.m_socketType; }
    int getSocketId() const { return m_cnctId.m_iSocketId; }
    QObject* getDbClient() const { return m_cnctId.m_pObjDbClt; }
public: // must overridables of base class CMsg (not overridden as this is a pure virtual base class)
    //virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // must overridables of base class System::CMsgReq (not overridden as this is a pure virtual base class)
    //virtual CMsgCon* createConfirmationMessage( ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ) = 0;
public: // overridables of base class CMsg
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    SCnctId m_cnctId;

}; // CMsgReq


//******************************************************************************
class ZSDATADLL_API CMsgCon : public ZS::System::CMsgCon
//******************************************************************************
{
protected: // ctors (pure virtual base class)
    CMsgCon(
        int                               i_iMsgType,
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgCon( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
public: // dtor
    virtual ~CMsgCon();
public: // instance methods
    virtual void setCnctId( const SCnctId& i_cnctId );
    SCnctId getCnctId() const { return m_cnctId; }
public: // instance methods
    EProtocolType getProtocolType() const { return m_cnctId.m_protocolType; }
    Ipc::ESocketType getSocketType() const { return m_cnctId.m_socketType; }
    int getSocketId() const { return m_cnctId.m_iSocketId; }
    QObject* getDbClient() const { return m_cnctId.m_pObjDbClt; }
public: // must overridables of base class CMsg (not overridden as this is a pure virtual base class)
    //virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    SCnctId m_cnctId;

}; // CMsgCon


//******************************************************************************
class ZSDATADLL_API CMsgInd : public ZS::System::CMsgInd
//******************************************************************************
{
protected: // ctors (pure virtual base class)
    CMsgInd(
        int             i_iMsgType,
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const SCnctId&  i_cnctId,
        bool            i_bMustBeAcknowledged = false,
        qint64          i_iReqId = -1,
        qint64          i_iMsgId = -1 );
    CMsgInd( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
public: // dtor
    virtual ~CMsgInd();
public: // instance methods
    virtual void setCnctId( const SCnctId& i_cnctId );
    SCnctId getCnctId() const { return m_cnctId; }
public: // instance methods
    EProtocolType getProtocolType() const { return m_cnctId.m_protocolType; }
    Ipc::ESocketType getSocketType() const { return m_cnctId.m_socketType; }
    int getSocketId() const { return m_cnctId.m_iSocketId; }
    QObject* getDbClient() const { return m_cnctId.m_pObjDbClt; }
public: // must overridables of base class CMsg (not overridden as this is a pure virtual base class)
    //virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // must overridables of base class System::CMsgInd (not overridden as this is a pure virtual base class)
    //virtual ZS::System::CMsgAck* createAcknowledgeMessage( ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ) = 0;
public: // overridables of base class CMsg
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    SCnctId m_cnctId;

}; // CMsgInd


//******************************************************************************
class ZSDATADLL_API CMsgAck : public ZS::System::CMsgAck
//******************************************************************************
{
protected: // ctors (pure virtual base class)
    CMsgAck(
        int                               i_iMsgType,
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdInd,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgAck( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
public: // dtor
    virtual ~CMsgAck();
public: // instance methods
    virtual void setCnctId( const SCnctId& i_cnctId );
    SCnctId getCnctId() const { return m_cnctId; }
public: // instance methods
    EProtocolType getProtocolType() const { return m_cnctId.m_protocolType; }
    Ipc::ESocketType getSocketType() const { return m_cnctId.m_socketType; }
    int getSocketId() const { return m_cnctId.m_iSocketId; }
    QObject* getDbClient() const { return m_cnctId.m_pObjDbClt; }
public: // must overridables of base class CMsg (not overridden as this is a pure virtual base class)
    //virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    SCnctId m_cnctId;

}; // CMsgAck


/*******************************************************************************
"Simple" Commands
*******************************************************************************/

//******************************************************************************
class ZSDATADLL_API CMsgReqReadSchema : public ZS::Data::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqReadSchema(
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const SCnctId&  i_cnctId,
        bool            i_bMustBeConfirmed = false,
        qint64          i_iReqId = -1,
        qint64          i_iMsgId = -1 );
    CMsgReqReadSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqReadSchema();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgReqReadSchema


//******************************************************************************
class ZSDATADLL_API CMsgConReadSchema : public ZS::Data::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConReadSchema(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConReadSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConReadSchema();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgConReadSchema


//******************************************************************************
class ZSDATADLL_API CMsgReqSaveSchema : public ZS::Data::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqSaveSchema(
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const SCnctId&  i_cnctId,
        bool            i_bMustBeConfirmed = false,
        qint64          i_iReqId = -1,
        qint64          i_iMsgId = -1 );
    CMsgReqSaveSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqSaveSchema();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgReqSaveSchema


//******************************************************************************
class ZSDATADLL_API CMsgConSaveSchema : public ZS::Data::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConSaveSchema(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConSaveSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConSaveSchema();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgConSaveSchema


//******************************************************************************
class ZSDATADLL_API CMsgReqReadData : public ZS::Data::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqReadData(
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const SCnctId&  i_cnctId,
        bool            i_bMustBeConfirmed = false,
        qint64          i_iReqId = -1,
        qint64          i_iMsgId = -1 );
    CMsgReqReadData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqReadData();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgReqReadData


//******************************************************************************
class ZSDATADLL_API CMsgConReadData : public ZS::Data::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConReadData(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConReadData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConReadData();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgConReadData


//******************************************************************************
class ZSDATADLL_API CMsgReqSaveData : public ZS::Data::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqSaveData(
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const SCnctId&  i_cnctId,
        bool            i_bMustBeConfirmed = false,
        qint64          i_iReqId = -1,
        qint64          i_iMsgId = -1 );
    CMsgReqSaveData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqSaveData();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgReqSaveData


//******************************************************************************
class ZSDATADLL_API CMsgConSaveData : public ZS::Data::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConSaveData(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConSaveData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConSaveData();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgConSaveData


//******************************************************************************
class ZSDATADLL_API CMsgReqSet2Default : public ZS::Data::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqSet2Default(
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const SCnctId&  i_cnctId,
        bool            i_bMustBeConfirmed = false,
        qint64          i_iReqId = -1,
        qint64          i_iMsgId = -1 );
    CMsgReqSet2Default( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqSet2Default();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgReqSet2Default


//******************************************************************************
class ZSDATADLL_API CMsgConSet2Default : public ZS::Data::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConSet2Default(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConSet2Default( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConSet2Default();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgConSet2Default


/*******************************************************************************
With Node Specs or Data Sets (ConSelectSchema and ConSelectData)
*******************************************************************************/

//******************************************************************************
class ZSDATADLL_API CMsgReqRegister : public ZS::Data::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqRegister(
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const SCnctId&  i_cnctId,
        bool            i_bMustBeConfirmed = false,
        qint64          i_iReqId = -1,
        qint64          i_iMsgId = -1 );
    CMsgReqRegister( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqRegister();
public: // overridables of base class ZS::Data::CMsgReq
    virtual void setCnctId( const SCnctId& i_cnctId );
public: // instance methods
    void setClientName( const QString& i_strName );
    void setDSNodeSpec( const SDSNodeSpec& i_dsNodeSpec );
    void setDSNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs );
    void addDSNodeSpec( const SDSNodeSpec& i_dsNodeSpec );
    void addDSNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs );
    void setRegSpec( const SDbClientRegSpec& i_dbCltRegSpec );
    QString getClientName() const { return m_dbCltRegSpec.m_strClientName; }
    QVector<SDSNodeSpec> getDSNodeSpecs() const { return m_dbCltRegSpec.m_arDSNodeSpecs; }
    int getDSNodeSpecsCount() const { return m_dbCltRegSpec.m_arDSNodeSpecs.size(); }
    SDSNodeSpec getDSNodeSpec( int i_idxSpec = 0 ) const;
    SDbClientRegSpec getRegSpec() const { return m_dbCltRegSpec; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    SDbClientRegSpec m_dbCltRegSpec;

}; // CMsgReqRegister


//******************************************************************************
class ZSDATADLL_API CMsgConRegister : public ZS::Data::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConRegister(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConRegister( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConRegister();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgConRegister


//******************************************************************************
class ZSDATADLL_API CMsgReqUnregister : public ZS::Data::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqUnregister(
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const SCnctId&  i_cnctId,
        bool            i_bMustBeConfirmed = false,
        qint64          i_iReqId = -1,
        qint64          i_iMsgId = -1 );
    CMsgReqUnregister( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqUnregister();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgReqUnregister


//******************************************************************************
class ZSDATADLL_API CMsgConUnregister : public ZS::Data::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConUnregister(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConUnregister( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConUnregister();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );

}; // CMsgConUnregister


//******************************************************************************
class ZSDATADLL_API CMsgReqSelectSchema : public ZS::Data::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqSelectSchema( // This message is used to query data and must always be confirmed.
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const SCnctId&  i_cnctId,
        qint64          i_iReqId = -1,
        qint64          i_iMsgId = -1 );
    CMsgReqSelectSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqSelectSchema();
public: // overridables of base class ZS::Data::CMsgReq
    virtual void setCnctId( const SCnctId& i_cnctId );
public: // instance methods
    void setClientName( const QString& i_strName );
    void setDSNodeSpec( const SDSNodeSpec i_dsNodeSpec );
    void setDSNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs );
    void addDSNodeSpec( const SDSNodeSpec& i_dsNodeSpec );
    void addDSNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs );
    void setRegSpec( const SDbClientRegSpec& i_dbCltRegSpec );
    QString getClientName() const { return m_dbCltRegSpec.m_strClientName; }
    QVector<SDSNodeSpec> getDSNodeSpecs() const { return m_dbCltRegSpec.m_arDSNodeSpecs; }
    int getDSNodeSpecsCount() const { return m_dbCltRegSpec.m_arDSNodeSpecs.size(); }
    SDSNodeSpec getDSNodeSpec( int i_idx = 0 ) const;
    SDbClientRegSpec getRegSpec() const { return m_dbCltRegSpec; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    SDbClientRegSpec m_dbCltRegSpec;

}; // CMsgReqSelectSchema


//******************************************************************************
class ZSDATADLL_API CMsgConSelectSchema : public ZS::Data::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConSelectSchema(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConSelectSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConSelectSchema();
public: // instance methods
    //void mergeSchema( CChangeDataSet* i_pChgDS );
    //void setSchema( CChangeDataSet* i_pChgDS ); // the message takes ownership of the data set
    //CDSSchemaChgDscr* getSchema();      // the message keeps ownership of the data set
    //CDSSchemaChgDscr* takeSchema();     // the message releases ownership of the data set
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    //CChangeDataSet* m_pChgDS;

}; // CMsgConSelectSchema


//******************************************************************************
class ZSDATADLL_API CMsgReqSelectData : public ZS::Data::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqSelectData( // This message is used to query data and must always be confirmed.
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const SCnctId&  i_cnctId,
        qint64          i_iReqId = -1,
        qint64          i_iMsgId = -1 );
    CMsgReqSelectData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqSelectData();
public: // overridables of base class ZS::Data::CMsgReq
    virtual void setCnctId( const SCnctId& i_cnctId );
public: // instance methods
    void setClientName( const QString& i_strName );
    void setDSNodeSpec( const SDSNodeSpec i_dsNodeSpec );
    void setDSNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs );
    void addDSNodeSpec( const SDSNodeSpec& i_dsNodeSpec );
    void addDSNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs );
    void setRegSpec( const SDbClientRegSpec& i_dbCltRegSpec );
    QString getClientName() const { return m_dbCltRegSpec.m_strClientName; }
    QVector<SDSNodeSpec> getDSNodeSpecs() const { return m_dbCltRegSpec.m_arDSNodeSpecs; }
    int getDSNodeSpecsCount() const { return m_dbCltRegSpec.m_arDSNodeSpecs.size(); }
    SDSNodeSpec getDSNodeSpec( int i_idx = 0 ) const;
    SDbClientRegSpec getRegSpec() const { return m_dbCltRegSpec; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    SDbClientRegSpec m_dbCltRegSpec;

}; // CMsgReqSelectData


//******************************************************************************
class ZSDATADLL_API CMsgConSelectData : public ZS::Data::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConSelectData(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConSelectData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConSelectData();
public: // instance methods
    //void mergeData( CChangeDataSet* i_pChgDS );
    //void setData( CChangeDataSet* i_pChgDS );         // the message takes ownership of the data set
    //CChangeDataSet* getData() { return m_pChgDS; }    // the message keeps ownership of the data set
    //CChangeDataSet* takeData();                    // the message releases ownership of the data set
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    //CChangeDataSet* m_pChgDS;

}; // CMsgConSelectData


/*******************************************************************************
With Data Sets
*******************************************************************************/

//******************************************************************************
class ZSDATADLL_API CMsgReqUpdateSchema : public ZS::Data::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqUpdateSchema(
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const SCnctId&  i_cnctId,
        bool            i_bMustBeConfirmed = false,
        qint64          i_iReqId = -1,
        qint64          i_iMsgId = -1 );
    CMsgReqUpdateSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqUpdateSchema();
public: // instance methods
    //void mergeSchema( CChangeDataSet* i_pChgDS );
    //void setSchema( CChangeDataSet* i_pChgDS ); // the message takes ownership of the data set
    //CDSSchemaChgDscr* getSchema();      // the message keeps ownership of the data set
    //CDSSchemaChgDscr* takeSchema();     // the message releases ownership of the data set
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    //CChangeDataSet* m_pChgDS;

}; // CMsgReqUpdateSchema


//******************************************************************************
class ZSDATADLL_API CMsgConUpdateSchema : public ZS::Data::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConUpdateSchema(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConUpdateSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConUpdateSchema();
public: // instance methods
    //void mergeSchema( CChangeDataSet* i_pChgDS );
    //void setSchema( CChangeDataSet* i_pChgDS ); // the message takes ownership of the data set
    //CDSSchemaChgDscr* getSchema();      // the message keeps ownership of the data set
    //CDSSchemaChgDscr* takeSchema();     // the message releases ownership of the data set
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    //CChangeDataSet* m_pChgDS;

}; // CMsgConUpdateSchema


//******************************************************************************
class ZSDATADLL_API CMsgReqUpdateData : public ZS::Data::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqUpdateData(
        QObject*        i_pObjSender,
        QObject*        i_pObjReceiver,
        const SCnctId&  i_cnctId,
        bool            i_bMustBeConfirmed = false,
        qint64          i_iReqId = -1,
        qint64          i_iMsgId = -1 );
    CMsgReqUpdateData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqUpdateData();
public: // instance methods
    //void mergeData( CChangeDataSet* i_pChgDS );
    //void setData( CChangeDataSet* i_pChgDS );         // the message takes ownership of the data set
    //CChangeDataSet* getData() { return m_pChgDS; }    // the message keeps ownership of the data set
    //CChangeDataSet* takeData();                    // the message releases ownership of the data set
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    //CChangeDataSet* m_pChgDS;

}; // CMsgReqUpdateData


//******************************************************************************
class ZSDATADLL_API CMsgConUpdateData : public ZS::Data::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConUpdateData(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SCnctId&                    i_cnctId,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConUpdateData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConUpdateData();
public: // instance methods
    //void mergeData( CChangeDataSet* i_pChgDS );
    //void setData( CChangeDataSet* i_pChgDS );         // the message takes ownership of the data set
    //CChangeDataSet* getData() { return m_pChgDS; }    // the message keeps ownership of the data set
    //CChangeDataSet* takeData();                    // the message releases ownership of the data set
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    //CChangeDataSet* m_pChgDS;

}; // CMsgConUpdateData


} // namespace Data

} // namespace ZS

#endif // ZSDataConnectionMsg_h
