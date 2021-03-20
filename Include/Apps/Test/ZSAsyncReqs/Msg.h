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

#ifndef ZSApps_TestAsyncRequests_Msg_h
#define ZSApps_TestAsyncRequests_Msg_h

#include "ZSIpc/ZSIpcDllMain.h"
#include "ZSIpc/ZSIpcCommon.h"
#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysMsg.h"

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace AsyncRequests
{
class CDb;

/*******************************************************************************
Type definitions and constants
*******************************************************************************/

//==============================================================================
enum EMsgType {
//==============================================================================
    EMsgTypeMin               = (ZS::System::EBaseMsgTypeMax+1),
    EMsgTypeReqReceiveData    = (EMsgTypeMin +  0),
    EMsgTypeReqStartup        = (EMsgTypeMin +  1),
    EMsgTypeConStartup        = (EMsgTypeMin +  2),
    EMsgTypeReqShutdown       = (EMsgTypeMin +  3),
    EMsgTypeConShutdown       = (EMsgTypeMin +  4),
    EMsgTypeReqChangeSettings = (EMsgTypeMin +  5),
    EMsgTypeConChangeSettings = (EMsgTypeMin +  6),
    EMsgTypeReqConnect        = (EMsgTypeMin +  7),
    EMsgTypeConConnect        = (EMsgTypeMin +  8),
    EMsgTypeIndConnected      = (EMsgTypeMin +  9),
    EMsgTypeReqDisconnect     = (EMsgTypeMin + 10),
    EMsgTypeConDisconnect     = (EMsgTypeMin + 11),
    EMsgTypeIndDisconnected   = (EMsgTypeMin + 12),
    EMsgTypeReqRegister       = (EMsgTypeMin + 13),
    EMsgTypeConRegister       = (EMsgTypeMin + 14),
    EMsgTypeReqUnregister     = (EMsgTypeMin + 15),
    EMsgTypeConUnregister     = (EMsgTypeMin + 16),
    EMsgTypeReqReadSchema     = (EMsgTypeMin + 17),
    EMsgTypeConReadSchema     = (EMsgTypeMin + 18),
    EMsgTypeReqSaveSchema     = (EMsgTypeMin + 19),
    EMsgTypeConSaveSchema     = (EMsgTypeMin + 20),
    EMsgTypeReqSelectSchema   = (EMsgTypeMin + 21),
    EMsgTypeConSelectSchema   = (EMsgTypeMin + 22),
    EMsgTypeReqUpdateSchema   = (EMsgTypeMin + 23),
    EMsgTypeConUpdateSchema   = (EMsgTypeMin + 24),
    EMsgTypeReqReadData       = (EMsgTypeMin + 25),
    EMsgTypeConReadData       = (EMsgTypeMin + 26),
    EMsgTypeReqSaveData       = (EMsgTypeMin + 27),
    EMsgTypeConSaveData       = (EMsgTypeMin + 28),
    EMsgTypeReqSelectData     = (EMsgTypeMin + 29),
    EMsgTypeConSelectData     = (EMsgTypeMin + 30),
    EMsgTypeReqUpdateData     = (EMsgTypeMin + 31),
    EMsgTypeConUpdateData     = (EMsgTypeMin + 32),
    EMsgTypeReqSet2Default    = (EMsgTypeMin + 33),
    EMsgTypeConSet2Default    = (EMsgTypeMin + 34),
    EMsgTypeMax               = (EMsgTypeMin + 34),
    EMsgTypeCount             = (EMsgTypeMax-EMsgTypeMin+1),
    EMsgTypeUndefined
};
QString msgType2Str( int i_iMsgType );


//******************************************************************************
class CMsgClassFactory
//******************************************************************************
{
public: // class methods
    static ZS::System::SErrResultInfo CheckMsgHeader( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    static ZS::System::CMsg* CreateInstance( const QByteArray& i_byteArr );
};

//******************************************************************************
class CMsgReqReceiveData : public ZS::System::CMsgReq
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
class CMsgReqStartup : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqStartup(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqStartup();
public: // instance methods
    ZS::Ipc::ESocketType getSocketType() const { return m_hostSettings.m_socketType; }
public: // instance methods
    ZS::Ipc::SServerHostSettings getHostSettings() const { return m_hostSettings; }
    void setHostSettings( const ZS::Ipc::SServerHostSettings& i_hostSettings );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    ZS::Ipc::SServerHostSettings m_hostSettings;

}; // CMsgReqStartup

//******************************************************************************
class CMsgConStartup : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConStartup(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                               i_iProgressInPerCent );
    virtual ~CMsgConStartup();
public: // instance methods
    ZS::Ipc::ESocketType getSocketType() const { return m_hostSettings.m_socketType; }
public: // instance methods
    ZS::Ipc::SServerHostSettings getHostSettings() const { return m_hostSettings; }
    void setHostSettings( const ZS::Ipc::SServerHostSettings& i_hostSettings );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    ZS::Ipc::SServerHostSettings m_hostSettings;

}; // CMsgConStartup

//******************************************************************************
class CMsgReqShutdown : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqShutdown(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqShutdown();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgReqShutdown

//******************************************************************************
class CMsgConShutdown : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConShutdown(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        qint64                            i_iReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                               i_iProgressInPerCent );
    virtual ~CMsgConShutdown();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgConShutdown

//******************************************************************************
class CMsgReqChangeSettings : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqChangeSettings(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqChangeSettings();
public: // instance methods
    ZS::Ipc::ESocketType getSocketType() const;
public: // instance methods
    ZS::Ipc::SServerHostSettings getServerHostSettings() const { return m_serverHostSettings; }
    void setServerHostSettings( const ZS::Ipc::SServerHostSettings& i_hostSettings );
public: // instance methods
    ZS::Ipc::SClientHostSettings getClientHostSettings() const { return m_clientHostSettings; }
    void setClientHostSettings( const ZS::Ipc::SClientHostSettings& i_hostSettings );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    ZS::Ipc::ESrvCltType         m_srvCltType;
    ZS::Ipc::SServerHostSettings m_serverHostSettings;
    ZS::Ipc::SClientHostSettings m_clientHostSettings;

}; // CMsgReqChangeSettings

//******************************************************************************
class CMsgConChangeSettings : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConChangeSettings(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    virtual ~CMsgConChangeSettings();
public: // instance methods
    ZS::Ipc::ESocketType getSocketType() const;
public: // instance methods
    ZS::Ipc::SServerHostSettings getServerHostSettings() const { return m_serverHostSettings; }
    void setServerHostSettings( const ZS::Ipc::SServerHostSettings& i_hostSettings );
public: // instance methods
    ZS::Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    void setSocketDscr( const ZS::Ipc::SSocketDscr& i_hostSettings );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    ZS::Ipc::ESrvCltType         m_srvCltType;
    ZS::Ipc::SServerHostSettings m_serverHostSettings;
    ZS::Ipc::SSocketDscr         m_socketDscr;

}; // CMsgConChangeSettings

//******************************************************************************
class CMsgReqConnect : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqConnect(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqConnect();
public: // instance methods
    CDb* getDb() { return m_pDb; }
    void setDb( CDb* i_pDb );
public: // instance methods
    ZS::Ipc::ESocketType getSocketType() const { return m_hostSettings.m_socketType; }
public: // instance methods
    ZS::Ipc::SClientHostSettings getHostSettings() const { return m_hostSettings; }
    void setHostSettings( const ZS::Ipc::SClientHostSettings& i_hostSettings );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    CDb*                     m_pDb;
    ZS::Ipc::SClientHostSettings m_hostSettings;

}; // CMsgReqConnect

//******************************************************************************
class CMsgConConnect : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConConnect(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    virtual ~CMsgConConnect();
public: // instance methods
    ZS::Ipc::ESocketType getSocketType() const { return m_socketDscr.m_socketType; }
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
public: // instance methods
    ZS::Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    void setSocketDscr( const ZS::Ipc::SSocketDscr& i_socketDscr );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    ZS::Ipc::SSocketDscr m_socketDscr;

}; // CMsgConConnect

//******************************************************************************
class CMsgIndConnected : public ZS::System::CMsgInd
//******************************************************************************
{
public: // ctors and dtor
    CMsgIndConnected(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeAcknowledged = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgIndConnected();
public: // instance methods
    ZS::Ipc::ESocketType getSocketType() const { return m_socketDscr.m_socketType; }
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
public: // instance methods
    ZS::Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    void setSocketDscr( const ZS::Ipc::SSocketDscr& i_socketDscr );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgAck* createAcknowledgeMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    ZS::Ipc::SSocketDscr m_socketDscr;

}; // CMsgIndConnected

//******************************************************************************
class CMsgReqDisconnect : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqDisconnect(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqDisconnect();
public: // instance methods
    ZS::Ipc::ESocketType getSocketType() const { return m_socketDscr.m_socketType; }
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
public: // instance methods
    ZS::Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    void setSocketDscr( const ZS::Ipc::SSocketDscr& i_socketDscr );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    ZS::Ipc::SSocketDscr m_socketDscr;

}; // CMsgReqDisconnect

//******************************************************************************
class CMsgConDisconnect : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConDisconnect(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    virtual ~CMsgConDisconnect();
public: // instance methods
    ZS::Ipc::ESocketType getSocketType() const { return m_socketDscr.m_socketType; }
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
public: // instance methods
    ZS::Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    void setSocketDscr( const ZS::Ipc::SSocketDscr& i_socketDscr );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    ZS::Ipc::SSocketDscr m_socketDscr;

}; // CMsgConDisconnect

//******************************************************************************
class CMsgIndDisconnected : public ZS::System::CMsgInd
//******************************************************************************
{
public: // ctors and dtor
    CMsgIndDisconnected(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeAcknowledged = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgIndDisconnected();
public: // instance methods
    ZS::Ipc::ESocketType getSocketType() const { return m_socketDscr.m_socketType; }
    int getSocketId() const { return m_socketDscr.m_iSocketId; }
public: // instance methods
    ZS::Ipc::SSocketDscr getSocketDscr() const { return m_socketDscr; }
    void setSocketDscr( const ZS::Ipc::SSocketDscr& i_socketDscr );
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgAck* createAcknowledgeMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;
protected: // instance members
    ZS::Ipc::SSocketDscr m_socketDscr;

}; // CMsgIndDisconnected

//******************************************************************************
class CMsgReqRegister : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqRegister(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqRegister( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqRegister();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgReqRegister

//******************************************************************************
class CMsgConRegister : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConRegister(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgConRegister( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConRegister();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgConRegister

//******************************************************************************
class CMsgReqUnregister : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqUnregister(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqUnregister( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqUnregister();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgReqUnregister

//******************************************************************************
class CMsgConUnregister : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConUnregister(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgConUnregister( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConUnregister();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgConUnregister

//******************************************************************************
class CMsgReqReadSchema : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqReadSchema(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqReadSchema( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqReadSchema();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgReqReadSchema

//******************************************************************************
class CMsgConReadSchema : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConReadSchema(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgConReadSchema( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConReadSchema();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgConReadSchema

//******************************************************************************
class CMsgReqSaveSchema : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqSaveSchema(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqSaveSchema( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqSaveSchema();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgReqSaveSchema

//******************************************************************************
class CMsgConSaveSchema : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConSaveSchema(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgConSaveSchema( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConSaveSchema();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgConSaveSchema

//******************************************************************************
class CMsgReqSelectSchema : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqSelectSchema( // This message is used to query data and must always be confirmed.
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqSelectSchema( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqSelectSchema();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgReqSelectSchema

//******************************************************************************
class CMsgConSelectSchema : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConSelectSchema(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgConSelectSchema( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConSelectSchema();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgConSelectSchema

//******************************************************************************
class CMsgReqUpdateSchema : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqUpdateSchema(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqUpdateSchema( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqUpdateSchema();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgReqUpdateSchema

//******************************************************************************
class CMsgConUpdateSchema : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConUpdateSchema(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgConUpdateSchema( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConUpdateSchema();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgConUpdateSchema

//******************************************************************************
class CMsgReqReadData : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqReadData(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqReadData( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqReadData();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgReqReadData

//******************************************************************************
class CMsgConReadData : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConReadData(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgConReadData( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConReadData();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgConReadData

//******************************************************************************
class CMsgReqSaveData : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqSaveData(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqSaveData( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqSaveData();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgReqSaveData

//******************************************************************************
class CMsgConSaveData : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConSaveData(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgConSaveData( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConSaveData();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgConSaveData

//******************************************************************************
class CMsgReqSelectData : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqSelectData( // This message is used to query data and must always be confirmed.
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqSelectData( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqSelectData();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgReqSelectData

//******************************************************************************
class CMsgConSelectData : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConSelectData(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgConSelectData( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConSelectData();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgConSelectData

//******************************************************************************
class CMsgReqUpdateData : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqUpdateData(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqUpdateData( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqUpdateData();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgReqUpdateData

//******************************************************************************
class CMsgConUpdateData : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConUpdateData(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgConUpdateData( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConUpdateData();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgConUpdateData

//******************************************************************************
class CMsgReqSet2Default : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqSet2Default(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqSet2Default( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgReqSet2Default();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership ); // not used, throws exception
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgReqSet2Default

//******************************************************************************
class CMsgConSet2Default : public ZS::System::CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConSet2Default(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        qint64                i_iReqId,
        qint64                i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo,
        int                   i_iProgressInPerCent );
    CMsgConSet2Default( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr );
    virtual ~CMsgConSet2Default();
public: // must overridables of base class CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // overridables of base class CMsg
    virtual QString msgTypeToStr() const;
    virtual QString getAddTrcInfoStr( int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText );
public: // overridables of base class CMsg
    virtual QByteArray serialize() const;

}; // CMsgConSet2Default

} // namespace AsyncRequests

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // ZSApps_TestAsyncRequests_Msg_h
