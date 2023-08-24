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

#ifndef ZSIpc_SrvCltMsg_h
#define ZSIpc_SrvCltMsg_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcCommon.h"
#include "ZSSys/ZSSysMsg.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace Ipc
{
class CBlkType;

enum EMsgType {
    EMsgTypeMin               = (ZS::System::EBaseMsgTypeMax+1),
    EMsgTypeReqStartup        = (EMsgTypeMin +  0),
    EMsgTypeConStartup        = (EMsgTypeMin +  1),
    EMsgTypeReqShutdown       = (EMsgTypeMin +  2),
    EMsgTypeConShutdown       = (EMsgTypeMin +  3),
    EMsgTypeReqConnect        = (EMsgTypeMin +  4),
    EMsgTypeConConnect        = (EMsgTypeMin +  5),
    EMsgTypeIndConnected      = (EMsgTypeMin +  6),
    EMsgTypeReqDisconnect     = (EMsgTypeMin +  7),
    EMsgTypeConDisconnect     = (EMsgTypeMin +  8),
    EMsgTypeIndDisconnected   = (EMsgTypeMin +  9),
    EMsgTypeReqChangeSettings = (EMsgTypeMin + 10),
    EMsgTypeConChangeSettings = (EMsgTypeMin + 11),
    EMsgTypeReqSendData       = (EMsgTypeMin + 12),
    EMsgTypeReqReceiveData    = (EMsgTypeMin + 13),
    EMsgTypeIndReceivedData   = (EMsgTypeMin + 14),
    EMsgTypeMax               = (EMsgTypeMin + 14),
    EMsgTypeCount             = (EMsgTypeMax-EMsgTypeMin+1),
    EMsgTypeUndefined
};

ZSIPCDLL_API QString msgType2Str( int i_iMsgType );

//******************************************************************************
class ZSIPCDLL_API CMsgReqStartup : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqStartup(
        QObject*                   i_pObjSender,
        QObject*                   i_pObjReceiver,
        const SServerHostSettings& i_hostSettings,
        CBlkType*                  i_pBlkType,  // The block will be cloned.
        bool                       i_bMustBeConfirmed = true,
        qint64                     i_iReqId = -1,
        qint64                     i_iMsgId = -1 );
    virtual ~CMsgReqStartup();
public: // instance methods
    ESocketType getSocketType() const { return m_hostSettings.m_socketType; }
    SServerHostSettings getHostSettings() const { return m_hostSettings; }
    void setHostSettings( const SServerHostSettings& i_hostSettings );
    CBlkType* getBlkType( bool i_bReleaseOwnerShip = false );
    void setBlkType( CBlkType* i_pBlkType );    // The block will be cloned.
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    SServerHostSettings m_hostSettings;
    CBlkType*           m_pBlkType;

}; // CMsgReqStartup

//******************************************************************************
class ZSIPCDLL_API CMsgConStartup : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConStartup( // Used by server to indicate that the IpcServer instance is running.
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SServerHostSettings&        i_hostSettings,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConStartup( // Used by both server and client to indicate that gateway thread is running.
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    virtual ~CMsgConStartup();
public: // instance methods
    ESocketState getSocketState() const { return m_socketState; }
    void setSocketState( ESocketState i_socketState );
    ESocketType getSocketType() const { return m_hostSettings.m_socketType; }
    SServerHostSettings getHostSettings() const { return m_hostSettings; }
    void setHostSettings( const SServerHostSettings& i_hostSettings );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    SServerHostSettings  m_hostSettings;
    ESocketState         m_socketState;

}; // CMsgConStartup

//******************************************************************************
class ZSIPCDLL_API CMsgReqShutdown : public ZS::System::CMsgReq
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
class ZSIPCDLL_API CMsgConShutdown : public ZS::System::CMsgCon
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
public: // instance methods
    ESocketState getSocketState() const { return m_socketState; }
    void setSocketState( ESocketState i_socketState );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    ESocketState m_socketState;

}; // CMsgConShutdown

//******************************************************************************
class ZSIPCDLL_API CMsgReqConnect : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqConnect(
        QObject*                   i_pObjSender,
        QObject*                   i_pObjReceiver,
        const SClientHostSettings& i_hostSettings,
        const STimerSettings&      i_watchDogSettings = STimerSettings(),
        CBlkType*                  i_pBlkType = nullptr, // the block will be cloned
        bool                       i_bMustBeConfirmed = true,
        qint64                     i_iReqId = -1,
        qint64                     i_iMsgId = -1 );
    virtual ~CMsgReqConnect();
public: // instance methods
    ESocketType getSocketType() const { return m_hostSettings.m_socketType; }
    SClientHostSettings getHostSettings() const { return m_hostSettings; }
    void setHostSettings( const SClientHostSettings& i_hostSettings );
    STimerSettings getWatchDogSettings() const { return m_watchDogSettings; }
    void setWatchDogSettings( const STimerSettings& i_watchDogSettings );
    CBlkType* getBlkType( bool i_bReleaseOwnerShip = false );
    void setBlkType( CBlkType* i_pBlkType );    // The block will be cloned.
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    SClientHostSettings m_hostSettings;
    STimerSettings      m_watchDogSettings;
    CBlkType*           m_pBlkType;

}; // CMsgReqConnect

//******************************************************************************
class ZSIPCDLL_API CMsgConConnect : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConConnect(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SSocketDscr&                i_socketDscr,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    virtual ~CMsgConConnect();
public: // instance methods
    ESocketState getSocketState() const { return m_socketDscr.m_socketState; }
    void setSocketState( ESocketState i_socketState );
public: // instance methods
    SSocketDscr getSocketDscr() const { return m_socketDscr; }
    void setSocketDscr( const SSocketDscr& i_socketDscr );
public: // instance methods
    ESocketType getSocketType() const { return m_socketDscr.m_socketType; }
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    SSocketDscr m_socketDscr;

}; // CMsgConConnect

//******************************************************************************
class ZSIPCDLL_API CMsgIndConnected : public ZS::System::CMsgInd
//******************************************************************************
{
public: // ctors and dtor
    CMsgIndConnected(
        QObject*           i_pObjSender,
        QObject*           i_pObjReceiver,
        const SSocketDscr& i_socketDscr,
        qint64             i_iMsgId = -1 );
    virtual ~CMsgIndConnected();
public: // instance methods
    ESocketState getSocketState() const { return m_socketDscr.m_socketState; }
    void setSocketState( ESocketState i_socketState );
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
    SSocketDscr getSocketDscr() const { return m_socketDscr; }
    void setSocketDscr( const ZS::Ipc::SSocketDscr& i_socketDscr );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgAck* createAcknowledgeMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    SSocketDscr m_socketDscr;

}; // CMsgIndConnected

//******************************************************************************
class ZSIPCDLL_API CMsgReqDisconnect : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqDisconnect(
        QObject*           i_pObjSender,
        QObject*           i_pObjReceiver,
        const SSocketDscr& i_socketDscr,
        bool               i_bMustBeConfirmed = true,
        qint64             i_iReqId = -1,
        qint64             i_iMsgId = -1 );
    virtual ~CMsgReqDisconnect();
public: // instance methods
    ESocketState getSocketState() const { return m_socketDscr.m_socketState; }
public: // instance methods
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
    SSocketDscr getSocketDscr() const { return m_socketDscr; }
    void setSocketDscr( const ZS::Ipc::SSocketDscr& i_socketDscr );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    SSocketDscr m_socketDscr;

}; // CMsgReqDisconnect

//******************************************************************************
class ZSIPCDLL_API CMsgConDisconnect : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConDisconnect(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SSocketDscr&                i_socketDscr,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    virtual ~CMsgConDisconnect();
public: // instance methods
    ESocketState getSocketState() const { return m_socketDscr.m_socketState; }
    void setSocketState( ESocketState i_socketState );
public: // instance methods
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
    SSocketDscr getSocketDscr() const { return m_socketDscr; }
    void setSocketDscr( const ZS::Ipc::SSocketDscr& i_socketDscr );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    SSocketDscr m_socketDscr;

}; // CMsgConDisconnect

//******************************************************************************
class ZSIPCDLL_API CMsgIndDisconnected : public ZS::System::CMsgInd
//******************************************************************************
{
public: // ctors and dtor
    CMsgIndDisconnected(
        QObject*           i_pObjSender,
        QObject*           i_pObjReceiver,
        const SSocketDscr& i_socketDscr,
        qint64             i_iMsgId = -1 );
    virtual ~CMsgIndDisconnected();
public: // instance methods
    ESocketState getSocketState() const { return m_socketDscr.m_socketState; }
    void setSocketState( ESocketState i_socketState );
public: // instance methods
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
    SSocketDscr getSocketDscr() const { return m_socketDscr; }
    void setSocketDscr( const ZS::Ipc::SSocketDscr& i_socketDscr );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgAck* createAcknowledgeMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    SSocketDscr m_socketDscr;

}; // CMsgIndDisconnected

//******************************************************************************
class ZSIPCDLL_API CMsgReqChangeSettings : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqChangeSettings(  // ctor for servers
        QObject*                   i_pObjSender,
        QObject*                   i_pObjReceiver,
        const SServerHostSettings& i_hostSettings,
        bool                       i_bMustBeConfirmed = true,
        qint64                     i_iReqId = -1,
        qint64                     i_iMsgId = -1 );
    CMsgReqChangeSettings(  // ctor for clients
        QObject*                   i_pObjSender,
        QObject*                   i_pObjReceiver,
        const SClientHostSettings& i_hostSettings,
        const STimerSettings&      i_watchDogSettings,
        bool                       i_bMustBeConfirmed = true,
        qint64                     i_iReqId = -1,
        qint64                     i_iMsgId = -1 );
    virtual ~CMsgReqChangeSettings();
public: // instance methods
    Ipc::ESrvCltType getSrvCltType() const { return m_srvCltType; }
    Ipc::SServerHostSettings getServerHostSettings() const { return m_serverHostSettings; }
    Ipc::SClientHostSettings getClientHostSettings() const { return m_clientHostSettings; }
    Ipc::ESocketType getSocketType() const;
    Ipc::STimerSettings getWatchDogSettings() const { return m_watchDogSettings; }
public: // instance methods
    void setBlkType( CBlkType* i_pBlkType );  // the block will be cloned
    CBlkType* getBlkType( bool i_bReleaseOwnerShip = false );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    Ipc::ESrvCltType    m_srvCltType;
    SServerHostSettings m_serverHostSettings;
    SClientHostSettings m_clientHostSettings;
    STimerSettings      m_watchDogSettings;
    CBlkType*           m_pBlkType;

}; // CMsgReqChangeSettings

//******************************************************************************
class ZSIPCDLL_API CMsgConChangeSettings : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConChangeSettings(  // ctor for servers
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SServerHostSettings&        i_hostSettings,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    CMsgConChangeSettings(  // ctor for clients
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        const SSocketDscr&                i_socketDscr,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                               i_iProgressInPerCent = 100 );
    virtual ~CMsgConChangeSettings();
public: // instance methods
    Ipc::ESrvCltType getSrvCltType() const { return m_srvCltType; }
    Ipc::ESocketType getSocketType() const;
    Ipc::SServerHostSettings getServerHostSettings() const { return m_serverHostSettings; }
    Ipc::SSocketDscr getClientSocketDscr() const { return m_clientSocketDscr; }
    int getClientSocketId() const { return m_clientSocketDscr.m_iSocketId; }
    ESocketState getClientSocketState() const { return m_clientSocketDscr.m_socketState; }
    void setClientSocketState( ESocketState i_socketState );
public: // instance methods
    void setBlkType( CBlkType* i_pBlkType );  // the block will be cloned
    CBlkType* getBlkType( bool i_bReleaseOwnerShip = false );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    Ipc::ESrvCltType         m_srvCltType;
    Ipc::SServerHostSettings m_serverHostSettings;
    Ipc::SSocketDscr         m_clientSocketDscr;
    CBlkType*                m_pBlkType;

}; // CMsgConChangeSettings

//******************************************************************************
class ZSIPCDLL_API CMsgReqSendData : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqSendData(
        QObject*          i_pObjSender,
        QObject*          i_pObjReceiver,
        int               i_iSocketId,  // ESocketIdAllSockets (=-2) addresses all sockets
        const QByteArray& i_byteArr,
        bool              i_bMustBeConfirmed = true,
        qint64            i_iReqId = -1,
        qint64            i_iMsgId = -1 );
    virtual ~CMsgReqSendData();
public: // instance methods
    int getSocketId() const { return m_iSocketId; }
    QByteArray getByteArray() { return m_byteArr; }
    void setByteArray( const QByteArray& i_byteArr );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    int        m_iSocketId;
    QByteArray m_byteArr;

}; // CMsgReqSendData

//******************************************************************************
class ZSIPCDLL_API CMsgReqReceiveData : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqReceiveData(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        int      i_iSocketId,   // ESocketIdAllSockets (=-2) addresses all sockets
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
class ZSIPCDLL_API CMsgIndReceivedData : public ZS::System::CMsgInd
//******************************************************************************
{
public: // ctors and dtor
    CMsgIndReceivedData(
        QObject*          i_pObjSender,
        QObject*          i_pObjReceiver,
        int               i_iSocketId,
        const QByteArray& i_byteArr,
        qint64            i_iMsgId = -1 );
    virtual ~CMsgIndReceivedData();
public: // instance methods
    int getSocketId() const { return m_iSocketId; }
    QByteArray getByteArray() { return m_byteArr; }
    void setByteArray( const QByteArray& i_byteArr );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgAck* createAcknowledgeMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
protected: // instance members
    int        m_iSocketId;
    QByteArray m_byteArr;

}; // CMsgIndReceivedData

} // namespace Ipc

} // namespace ZS

#endif // ZSIpc_SrvCltMsg_h
