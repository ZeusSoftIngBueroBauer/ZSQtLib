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

#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSIpc/ZSIpcBlkType.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
enum MsgType
==============================================================================*/

const SEnumEntry s_arEnumStrMsgTypes[ZS::Ipc::EMsgTypeCount] =
{
    /* 1013 */ SEnumEntry( EMsgTypeReqStartup-EMsgTypeMin,        "ReqStartup"        ),
    /* 1014 */ SEnumEntry( EMsgTypeConStartup-EMsgTypeMin,        "ConStartup"        ),
    /* 1015 */ SEnumEntry( EMsgTypeReqShutdown-EMsgTypeMin,       "ReqShutdown"       ),
    /* 1016 */ SEnumEntry( EMsgTypeConShutdown-EMsgTypeMin,       "ConShutdown"       ),
    /* 1017 */ SEnumEntry( EMsgTypeReqConnect-EMsgTypeMin,        "ReqConnect"        ),
    /* 1018 */ SEnumEntry( EMsgTypeConConnect-EMsgTypeMin,        "ConConnect"        ),
    /* 1019 */ SEnumEntry( EMsgTypeIndConnected-EMsgTypeMin,      "IndConnected"      ),
    /* 1020 */ SEnumEntry( EMsgTypeReqDisconnect-EMsgTypeMin,     "ReqDisconnect"     ),
    /* 1021 */ SEnumEntry( EMsgTypeConDisconnect-EMsgTypeMin,     "ConDisconnect"     ),
    /* 1022 */ SEnumEntry( EMsgTypeIndDisconnected-EMsgTypeMin,   "IndDisconnected"   ),
    /* 1023 */ SEnumEntry( EMsgTypeReqChangeSettings-EMsgTypeMin, "ReqChangeSettings" ),
    /* 1024 */ SEnumEntry( EMsgTypeConChangeSettings-EMsgTypeMin, "ConChangeSettings" ),
    /* 1025 */ SEnumEntry( EMsgTypeReqSendData-EMsgTypeMin,       "ReqSendData"       ),
    /* 1026 */ SEnumEntry( EMsgTypeReqReceiveData-EMsgTypeMin,    "ReqReceiveData"    ),
    /* 1027 */ SEnumEntry( EMsgTypeIndReceivedData-EMsgTypeMin,   "IndReceivedData"   )
};

//------------------------------------------------------------------------------
QString ZS::Ipc::msgType2Str( int i_iMsgType )
//------------------------------------------------------------------------------
{
    QString strMsgType = QString::number(i_iMsgType);
    int     iMsgType   = i_iMsgType;

    if( iMsgType >= Ipc::EMsgTypeMin && iMsgType <= Ipc::EMsgTypeMax )
    {
        iMsgType -= Ipc::EMsgTypeMin;
        strMsgType = SEnumEntry::enumerator2Str(s_arEnumStrMsgTypes,Ipc::EMsgTypeCount,iMsgType);
    }
    return strMsgType;

} // msgType2Str


/*******************************************************************************
class CMsgReqStartup : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqStartup::CMsgReqStartup(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    const SServerHostSettings& i_hostSettings,
    CBlkType*                  i_pBlkType,
    bool                       i_bMustBeConfirmed,
    qint64                     i_iReqId,
    qint64                     i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqStartup,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_hostSettings(i_hostSettings),
    m_pBlkType(nullptr)
{
    if( i_pBlkType != nullptr )
    {
        m_pBlkType = i_pBlkType->clone();
    }

    setObjectName( Ipc::msgType2Str(EMsgTypeReqStartup) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgReqStartup::~CMsgReqStartup()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pBlkType;
    }
    catch(...)
    {
    }
    m_pBlkType = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgReqStartup::setHostSettings( const SServerHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    m_hostSettings = i_hostSettings;
}

//------------------------------------------------------------------------------
CBlkType* CMsgReqStartup::getBlkType( bool i_bReleaseOwnerShip )
//------------------------------------------------------------------------------
{
    CBlkType* pBlkType = m_pBlkType;

    if( i_bReleaseOwnerShip )
    {
        m_pBlkType = nullptr;
    }
    return pBlkType;

} // getBlkType

//------------------------------------------------------------------------------
void CMsgReqStartup::setBlkType( CBlkType* i_pBlkType )
//------------------------------------------------------------------------------
{
    delete m_pBlkType;
    m_pBlkType = nullptr;

    if( i_pBlkType != nullptr )
    {
        m_pBlkType = i_pBlkType->clone();
    }

} // setBlkType

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqStartup::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqStartup* pMsgCloned = new CMsgReqStartup(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* hostSettings     */ m_hostSettings,
        /* pBlkType         */ m_pBlkType,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setTimeoutInMs(m_iTimeout_ms);
    pMsgCloned->setIsBlockingRequest(m_bIsBlockingRequest);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqStartup::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    CMsgConStartup* pMsgCon = new CMsgConStartup(
        /* pObjSender    */ m_pObjReceiver,
        /* pObjReceiver  */ m_pObjSender,
        /* hostSettings  */ m_hostSettings,
        /* iReqId        */ m_iReqId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ SErrResultInfo(),
        /* iProgress     */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqStartup::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::ReqStartup)";
}

//------------------------------------------------------------------------------
QString CMsgReqStartup::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgReq::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", HostSettings {" + m_hostSettings.getConnectionString() + "}";

    if( m_pBlkType == nullptr )
    {
        strAddTrcInfo += ", BlkType: nullptr";
    }
    else
    {
        strAddTrcInfo += ", BlkType: " + blkType2Str(m_pBlkType->type());
    }

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConStartup : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConStartup::CMsgConStartup(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    const SServerHostSettings& i_hostSettings,
    qint64                     i_iReqId,
    qint64                     i_iMsgIdReq,
    const SErrResultInfo&      i_errResultInfo,
    int                        i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConStartup,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_hostSettings(i_hostSettings),
    m_socketState(ESocketStateListening)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeConStartup) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgConStartup::CMsgConStartup(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConStartup,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_hostSettings(),
    m_socketState(ESocketStateUnconnected)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeConStartup) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgConStartup::~CMsgConStartup()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgConStartup::setSocketState( ESocketState i_socketState )
//------------------------------------------------------------------------------
{
    m_socketState = i_socketState;
}

//------------------------------------------------------------------------------
void CMsgConStartup::setHostSettings( const SServerHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    m_hostSettings = i_hostSettings;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConStartup::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConStartup* pMsgCloned = new CMsgConStartup(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
        /* hostSettings  */ m_hostSettings,
        /* iReqId        */ m_iReqId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ m_errResultInfo,
        /* iProgress     */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setSocketState(m_socketState);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConStartup::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::ConStartup)";
}

//------------------------------------------------------------------------------
QString CMsgConStartup::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgCon::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", HostSettings {" + m_hostSettings.getConnectionString() + "}";
    strAddTrcInfo += ", SocketState: " + socketState2Str(m_socketState);

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqShutdown : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqShutdown::CMsgReqShutdown(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqShutdown,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
    setObjectName( Ipc::msgType2Str(EMsgTypeReqShutdown) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgReqShutdown::~CMsgReqShutdown()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqShutdown::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqShutdown* pMsgCloned = new CMsgReqShutdown(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setTimeoutInMs(m_iTimeout_ms);
    pMsgCloned->setIsBlockingRequest(m_bIsBlockingRequest);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqShutdown::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    CMsgConShutdown* pMsgCon = new CMsgConShutdown(
        /* pObjSender    */ m_pObjReceiver,
        /* pObjReceiver  */ m_pObjSender,
        /* iReqId        */ m_iReqId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ SErrResultInfo(),
        /* iProgress     */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqShutdown::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::ReqShutdown)";
}

//------------------------------------------------------------------------------
QString CMsgReqShutdown::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgReq::getAddTrcInfoStr(i_iDetailLevel,i_format);

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConShutdown : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConShutdown::CMsgConShutdown(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConShutdown,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_socketState(ESocketStateListening)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeConShutdown) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgConShutdown::~CMsgConShutdown()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgConShutdown::setSocketState( ESocketState i_socketState )
//------------------------------------------------------------------------------
{
    m_socketState = i_socketState;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConShutdown::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConShutdown* pMsgCloned = new CMsgConShutdown(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
        /* iReqId        */ m_iReqId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ m_errResultInfo,
        /* iProgress     */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);
    pMsgCloned->setSocketState(m_socketState);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConShutdown::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::ConShutdown)";
}

//------------------------------------------------------------------------------
QString CMsgConShutdown::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgCon::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", SocketState: " + socketState2Str(m_socketState);

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqConnect : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqConnect::CMsgReqConnect(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    const SClientHostSettings& i_hostSettings,
    const STimerSettings&      i_watchDogSettings,
    CBlkType*                  i_pBlkType,
    bool                       i_bMustBeConfirmed,
    qint64                     i_iReqId,
    qint64                     i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqConnect,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_hostSettings(i_hostSettings),
    m_watchDogSettings(i_watchDogSettings),
    m_pBlkType(nullptr)
{
    if( i_pBlkType != nullptr )
    {
        m_pBlkType = i_pBlkType->clone();
    }

    setObjectName( Ipc::msgType2Str(EMsgTypeReqConnect) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgReqConnect::~CMsgReqConnect()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pBlkType;
    }
    catch(...)
    {
    }
    m_pBlkType = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgReqConnect::setHostSettings( const SClientHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    m_hostSettings = i_hostSettings;
}

//------------------------------------------------------------------------------
void CMsgReqConnect::setWatchDogSettings( const STimerSettings& i_watchDogSettings )
//------------------------------------------------------------------------------
{
    m_watchDogSettings = i_watchDogSettings;
}

//------------------------------------------------------------------------------
CBlkType* CMsgReqConnect::getBlkType( bool i_bReleaseOwnerShip )
//------------------------------------------------------------------------------
{
    CBlkType* pBlkType = m_pBlkType;

    if( i_bReleaseOwnerShip )
    {
        m_pBlkType = nullptr;
    }
    return pBlkType;

} // getBlkType

//------------------------------------------------------------------------------
void CMsgReqConnect::setBlkType( CBlkType* i_pBlkType )
//------------------------------------------------------------------------------
{
    delete m_pBlkType;
    m_pBlkType = nullptr;

    if( i_pBlkType != nullptr )
    {
        m_pBlkType = i_pBlkType->clone();
    }

} // setBlkType

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqConnect::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqConnect* pMsgCloned = new CMsgReqConnect(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* hostSettings     */ m_hostSettings,
        /* watchDogSettings */ m_watchDogSettings,
        /* pBlkType         */ m_pBlkType,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setTimeoutInMs(m_iTimeout_ms);
    pMsgCloned->setIsBlockingRequest(m_bIsBlockingRequest);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqConnect::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    CMsgConConnect* pMsgCon = new CMsgConConnect(
        /* pObjSender    */ m_pObjReceiver,
        /* pObjReceiver  */ m_pObjSender,
        /* socketDscr    */ m_hostSettings.getSocketDscr(),
        /* iReqId        */ m_iReqId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ SErrResultInfo(),
        /* iProgress     */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqConnect::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::ReqConnect)";
}

//------------------------------------------------------------------------------
QString CMsgReqConnect::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgReq::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", Cnct {" + m_hostSettings.getConnectionString(i_iDetailLevel) + "}";

    strAddTrcInfo += ", WatchDogSettings {";
    strAddTrcInfo += "Enabled: " + bool2Str(m_watchDogSettings.m_bEnabled);
    strAddTrcInfo += ", Interval_ms: " + QString::number(m_watchDogSettings.m_iInterval_ms);
    strAddTrcInfo += ", Timeout_ms: " + QString::number(m_watchDogSettings.m_iTimeout_ms) + "}";

    if( m_pBlkType == nullptr )
    {
        strAddTrcInfo += ", BlkType: nullptr";
    }
    else
    {
        strAddTrcInfo += ", BlkType: " + blkType2Str(m_pBlkType->type());
    }
    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConConnect : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConConnect::CMsgConConnect(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SSocketDscr&    i_socketDscr,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConConnect,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_socketDscr(i_socketDscr)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeConConnect) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgConConnect::~CMsgConConnect()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgConConnect::setSocketState( ESocketState i_socketState )
//------------------------------------------------------------------------------
{
    m_socketDscr.m_socketState = i_socketState;
}

//------------------------------------------------------------------------------
void CMsgConConnect::setSocketDscr( const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    m_socketDscr = i_socketDscr;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConConnect::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConConnect* pMsgCloned = new CMsgConConnect(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
        /* socketDscr    */ m_socketDscr,
        /* iReqId        */ m_iReqId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ m_errResultInfo,
        /* iProgress     */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConConnect::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::ConConnect)";
}

//------------------------------------------------------------------------------
QString CMsgConConnect::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgCon::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", Cnct {" + m_socketDscr.getConnectionString() + "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgIndConnected : public CMsgInd
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgIndConnected::CMsgIndConnected(
    QObject*           i_pObjSender,
    QObject*           i_pObjReceiver,
    const SSocketDscr& i_socketDscr,
    qint64             i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgInd(
        /* iMsgType     */ EMsgTypeIndConnected,
        /* pObjSender   */ i_pObjSender,
        /* pObjReceiver */ i_pObjReceiver,
        /* iMsgIdReq    */ i_iMsgId ),
    m_socketDscr(i_socketDscr)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeIndConnected) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgIndConnected::~CMsgIndConnected()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgIndConnected::setSocketState( ESocketState i_socketState )
//------------------------------------------------------------------------------
{
    m_socketDscr.m_socketState = i_socketState;
}

//------------------------------------------------------------------------------
void CMsgIndConnected::setSocketDscr( const ZS::Ipc::SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    m_socketDscr = i_socketDscr;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgIndConnected::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgIndConnected* pMsgCloned = new CMsgIndConnected(
        /* pObjSender   */ m_pObjSender,
        /* pObjReceiver */ m_pObjReceiver,
        /* socketDscr   */ m_socketDscr,
        /* iMsgId       */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgAck* CMsgIndConnected::createAcknowledgeMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgIndConnected::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::IndConnected)";
}

//------------------------------------------------------------------------------
QString CMsgIndConnected::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgInd::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", Cnct {" + m_socketDscr.getConnectionString() + "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqDisconnect : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqDisconnect::CMsgReqDisconnect(
    QObject*           i_pObjSender,
    QObject*           i_pObjReceiver,
    const SSocketDscr& i_socketDscr,
    bool               i_bMustBeConfirmed,
    qint64             i_iReqId,
    qint64             i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqDisconnect,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_socketDscr(i_socketDscr)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeReqDisconnect) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgReqDisconnect::~CMsgReqDisconnect()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgReqDisconnect::setSocketDscr( const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    m_socketDscr = i_socketDscr;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqDisconnect::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqDisconnect* pMsgCloned = new CMsgReqDisconnect(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* socketDscr       */ m_socketDscr,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setTimeoutInMs(m_iTimeout_ms);
    pMsgCloned->setIsBlockingRequest(m_bIsBlockingRequest);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqDisconnect::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    CMsgConDisconnect* pMsgCon = new CMsgConDisconnect(
        /* pObjSender    */ m_pObjReceiver,
        /* pObjReceiver  */ m_pObjSender,
        /* socketDscr    */ m_socketDscr,
        /* iReqId        */ m_iReqId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ SErrResultInfo(),
        /* iProgress     */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqDisconnect::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::ReqDisconnect)";
}

//------------------------------------------------------------------------------
QString CMsgReqDisconnect::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgReq::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", Cnct {" + m_socketDscr.getConnectionString() + "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConDisconnect : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConDisconnect::CMsgConDisconnect(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SSocketDscr&    i_socketDscr,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConDisconnect,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_socketDscr(i_socketDscr)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeConDisconnect) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgConDisconnect::~CMsgConDisconnect()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgConDisconnect::setSocketState( ESocketState i_socketState )
//------------------------------------------------------------------------------
{
    m_socketDscr.m_socketState = i_socketState;
}

//------------------------------------------------------------------------------
void CMsgConDisconnect::setSocketDscr( const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    m_socketDscr = i_socketDscr;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConDisconnect::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConDisconnect* pMsgCloned = new CMsgConDisconnect(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
        /* socketDscr    */ m_socketDscr,
        /* iReqId        */ m_iReqId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ m_errResultInfo,
        /* iProgress     */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConDisconnect::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::ConDisconnect)";
}

//------------------------------------------------------------------------------
QString CMsgConDisconnect::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgCon::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", Cnct {" + m_socketDscr.getConnectionString() + "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgIndDisconnected : public CMsgInd
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgIndDisconnected::CMsgIndDisconnected(
    QObject*           i_pObjSender,
    QObject*           i_pObjReceiver,
    const SSocketDscr& i_socketDscr,
    qint64             i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgInd(
        /* iMsgType     */ EMsgTypeIndDisconnected,
        /* pObjSender   */ i_pObjSender,
        /* pObjReceiver */ i_pObjReceiver,
        /* iMsgIdReq    */ i_iMsgId ),
    m_socketDscr(i_socketDscr)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeIndDisconnected) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgIndDisconnected::~CMsgIndDisconnected()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgIndDisconnected::setSocketState( ESocketState i_socketState )
//------------------------------------------------------------------------------
{
    m_socketDscr.m_socketState = i_socketState;
}

//------------------------------------------------------------------------------
void CMsgIndDisconnected::setSocketDscr( const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    m_socketDscr = i_socketDscr;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgIndDisconnected::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgIndDisconnected* pMsgCloned = new CMsgIndDisconnected(
        /* pObjSender   */ m_pObjSender,
        /* pObjReceiver */ m_pObjReceiver,
        /* socketDscr   */ m_socketDscr,
        /* iMsgId       */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgAck* CMsgIndDisconnected::createAcknowledgeMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgIndDisconnected::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::IndDisconnected)";
}

//------------------------------------------------------------------------------
QString CMsgIndDisconnected::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgInd::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", Cnct {" + m_socketDscr.getConnectionString() + "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqChangeSettings : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqChangeSettings::CMsgReqChangeSettings(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    const SServerHostSettings& i_hostSettings,
    bool                       i_bMustBeConfirmed,
    qint64                     i_iReqId,
    qint64                     i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqChangeSettings,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_srvCltType(ESrvCltTypeServer),
    m_serverHostSettings(i_hostSettings),
    m_clientHostSettings(),
    m_watchDogSettings(),
    m_pBlkType(nullptr)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeReqChangeSettings) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgReqChangeSettings::CMsgReqChangeSettings(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    const SClientHostSettings& i_hostSettings,
    const STimerSettings&      i_watchDogSettings,
    bool                       i_bMustBeConfirmed,
    qint64                     i_iReqId,
    qint64                     i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqChangeSettings,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_srvCltType(ESrvCltTypeClient),
    m_serverHostSettings(),
    m_clientHostSettings(i_hostSettings),
    m_watchDogSettings(i_watchDogSettings),
    m_pBlkType(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqChangeSettings::~CMsgReqChangeSettings()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pBlkType;
    }
    catch(...)
    {
    }
    m_pBlkType = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ESocketType CMsgReqChangeSettings::getSocketType() const
//------------------------------------------------------------------------------
{
    ESocketType socketType = ESocketTypeUndefined;

    if( m_srvCltType == ESrvCltTypeServer )
    {
        socketType = m_serverHostSettings.m_socketType;
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        socketType = m_clientHostSettings.m_socketType;
    }
    return socketType;

} // getSocketType

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CBlkType* CMsgReqChangeSettings::getBlkType( bool i_bReleaseOwnerShip )
//------------------------------------------------------------------------------
{
    CBlkType* pBlkType = m_pBlkType;

    if( i_bReleaseOwnerShip )
    {
        m_pBlkType = nullptr;
    }
    return pBlkType;

} // getBlkType

//------------------------------------------------------------------------------
void CMsgReqChangeSettings::setBlkType( CBlkType* i_pBlkType )
//------------------------------------------------------------------------------
{
    if( i_pBlkType != nullptr )
    {
        m_pBlkType = i_pBlkType->clone();
    }

} // setBlkType

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqChangeSettings::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqChangeSettings* pMsgCloned = nullptr;

    if( m_srvCltType == ESrvCltTypeServer )
    {
        pMsgCloned = new CMsgReqChangeSettings(
            /* pObjSender       */ m_pObjSender,
            /* pObjReceiver     */ m_pObjReceiver,
            /* hostSettings     */ m_serverHostSettings,
            /* bMustBeConfirmed */ m_bMustBeConfirmed,
            /* iReqId           */ m_iReqId,
            /* iMsgId           */ m_iMsgId );
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        pMsgCloned = new CMsgReqChangeSettings(
            /* pObjSender       */ m_pObjSender,
            /* pObjReceiver     */ m_pObjReceiver,
            /* hostSettings     */ m_clientHostSettings,
            /* watchDogSettings */ m_watchDogSettings,
            /* bMustBeConfirmed */ m_bMustBeConfirmed,
            /* iReqId           */ m_iReqId,
            /* iMsgId           */ m_iMsgId );
    }

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setTimeoutInMs(m_iTimeout_ms);
    pMsgCloned->setIsBlockingRequest(m_bIsBlockingRequest);

    pMsgCloned->setBlkType(m_pBlkType);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqChangeSettings::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    CMsgConChangeSettings* pMsgCon = nullptr;

    if( m_srvCltType == ESrvCltTypeServer )
    {
        pMsgCon = new CMsgConChangeSettings(
            /* pObjSender        */ m_pObjReceiver,
            /* pObjReceiver      */ m_pObjSender,
            /* hostSettings      */ m_serverHostSettings,
            /* iReqId            */ m_iReqId,
            /* iMsgIdReq         */ m_iMsgId,
            /* errResultInfo     */ SErrResultInfo(),
            /* iProgress_perCent */ 100 );
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        pMsgCon = new CMsgConChangeSettings(
            /* pObjSender        */ m_pObjReceiver,
            /* pObjReceiver      */ m_pObjSender,
            /* socketDscr        */ m_clientHostSettings.getSocketDscr(),
            /* iReqId            */ m_iReqId,
            /* iMsgIdReq         */ m_iMsgId,
            /* errResultInfo     */ SErrResultInfo(),
            /* iProgress_perCent */ 100 );
    }

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);
    pMsgCon->setBlkType(m_pBlkType);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqChangeSettings::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::ReqChangeSettings)";
}

//------------------------------------------------------------------------------
QString CMsgReqChangeSettings::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgReq::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", SrvCltType: " + srvCltType2Str(m_srvCltType);

    if( m_srvCltType == ESrvCltTypeServer )
    {
        strAddTrcInfo += ", HostSettings { " + m_serverHostSettings.getConnectionString() + " }";
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        strAddTrcInfo += ", HostSettings { " + m_clientHostSettings.getConnectionString() + " }";
        strAddTrcInfo += ", WatchDogSettings {";
        strAddTrcInfo += "Enabled: " + bool2Str(m_watchDogSettings.m_bEnabled);
        strAddTrcInfo += ", Interval_ms: " + QString::number(m_watchDogSettings.m_iInterval_ms);
        strAddTrcInfo += ", Timeout_ms: " + QString::number(m_watchDogSettings.m_iTimeout_ms) + "}";
    }

    if( m_pBlkType == nullptr )
    {
        strAddTrcInfo += ", BlkType: nullptr";
    }
    else
    {
        strAddTrcInfo += ", BlkType: " + blkType2Str(m_pBlkType->type());
    }
    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConChangeSettings : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConChangeSettings::CMsgConChangeSettings(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    const SServerHostSettings& i_hostSettings,
    qint64                     i_iReqId,
    qint64                     i_iMsgIdReq,
    const SErrResultInfo&      i_errResultInfo,
    int                        i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConChangeSettings,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_srvCltType(ESrvCltTypeServer),
    m_serverHostSettings(i_hostSettings),
    m_clientSocketDscr(),
    m_pBlkType(nullptr)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeConChangeSettings) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgConChangeSettings::CMsgConChangeSettings(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SSocketDscr&    i_socketDscr,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConChangeSettings,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_srvCltType(ESrvCltTypeClient),
    m_serverHostSettings(),
    m_clientSocketDscr(i_socketDscr),
    m_pBlkType(nullptr)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeConChangeSettings) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgConChangeSettings::~CMsgConChangeSettings()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pBlkType;
    }
    catch(...)
    {
    }
    m_pBlkType = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ESocketType CMsgConChangeSettings::getSocketType() const
//------------------------------------------------------------------------------
{
    ESocketType socketType = ESocketTypeUndefined;

    if( m_srvCltType == ESrvCltTypeServer )
    {
        socketType = m_serverHostSettings.m_socketType;
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        socketType = m_clientSocketDscr.m_socketType;
    }
    return socketType;

} // getSocketType

//------------------------------------------------------------------------------
void CMsgConChangeSettings::setClientSocketState( ESocketState i_socketState )
//------------------------------------------------------------------------------
{
    m_clientSocketDscr.m_socketState = i_socketState;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CBlkType* CMsgConChangeSettings::getBlkType( bool i_bReleaseOwnerShip )
//------------------------------------------------------------------------------
{
    CBlkType* pBlkType = m_pBlkType;

    if( i_bReleaseOwnerShip )
    {
        m_pBlkType = nullptr;
    }
    return pBlkType;

} // getBlkType

//------------------------------------------------------------------------------
void CMsgConChangeSettings::setBlkType( CBlkType* i_pBlkType )
//------------------------------------------------------------------------------
{
    if( i_pBlkType != nullptr )
    {
        m_pBlkType = i_pBlkType->clone();
    }

} // setBlkType

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConChangeSettings::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConChangeSettings* pMsgCloned = nullptr;

    if( m_srvCltType == ESrvCltTypeServer )
    {
        pMsgCloned = new CMsgConChangeSettings(
            /* pObjSender       */ m_pObjSender,
            /* pObjReceiver     */ m_pObjReceiver,
            /* hostSettings     */ m_serverHostSettings,
            /* iReqId           */ m_iReqId,
            /* iMsgId           */ m_iMsgId,
            /* errResultInfo    */ m_errResultInfo,
            /* iProgress        */ m_iProgressInPerCent );
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        pMsgCloned = new CMsgConChangeSettings(
            /* pObjSender       */ m_pObjSender,
            /* pObjReceiver     */ m_pObjReceiver,
            /* socketDscr       */ m_clientSocketDscr,
            /* iReqId           */ m_iReqId,
            /* iMsgId           */ m_iMsgId,
            /* errResultInfo    */ m_errResultInfo,
            /* iProgress        */ m_iProgressInPerCent );
    }

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConChangeSettings::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::ConChangeSettings)";
}

//------------------------------------------------------------------------------
QString CMsgConChangeSettings::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgCon::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", SrvCltType: " + srvCltType2Str(m_srvCltType);

    if( m_srvCltType == ESrvCltTypeServer )
    {
        strAddTrcInfo += ", HostSettings { " + m_serverHostSettings.getConnectionString() + " }";
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        strAddTrcInfo += ", HostSettings { " + m_clientSocketDscr.getConnectionString() + " }";
    }

    if( m_pBlkType == nullptr )
    {
        strAddTrcInfo += ", BlkType: nullptr";
    }
    else
    {
        strAddTrcInfo += ", BlkType: " + blkType2Str(m_pBlkType->type());
    }
    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqSendData : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqSendData::CMsgReqSendData(
    QObject*          i_pObjSender,
    QObject*          i_pObjReceiver,
    int               i_iSocketId,
    const QByteArray& i_byteArr,
    bool              i_bMustBeConfirmed,
    qint64            i_iReqId,
    qint64            i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqSendData,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_iSocketId(i_iSocketId),
    m_byteArr(i_byteArr)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeReqSendData) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgReqSendData::~CMsgReqSendData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgReqSendData::setByteArray( const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    m_byteArr = i_byteArr;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqSendData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqSendData* pMsgCloned = new CMsgReqSendData(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* iSocketId        */ m_iSocketId,
        /* byteArr          */ m_byteArr,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setTimeoutInMs(m_iTimeout_ms);
    pMsgCloned->setIsBlockingRequest(m_bIsBlockingRequest);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqSendData::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqSendData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::ReqSendData)";
}

//------------------------------------------------------------------------------
QString CMsgReqSendData::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgReq::getAddTrcInfoStr(i_iDetailLevel,i_format);

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqReceiveData : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqReceiveData::CMsgReqReceiveData(
    QObject*  i_pObjSender,
    QObject*  i_pObjReceiver,
    int       i_iSocketId,
    bool      i_bMustBeConfirmed,
    qint64    i_iReqId,
    qint64    i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqReceiveData,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_iSocketId(i_iSocketId)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeReqReceiveData) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgReqReceiveData::~CMsgReqReceiveData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqReceiveData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqReceiveData* pMsgCloned = new CMsgReqReceiveData(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* iSocketId        */ m_iSocketId,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setTimeoutInMs(m_iTimeout_ms);
    pMsgCloned->setIsBlockingRequest(m_bIsBlockingRequest);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqReceiveData::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqReceiveData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::ReqReceiveData)";
}

//------------------------------------------------------------------------------
QString CMsgReqReceiveData::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgReq::getAddTrcInfoStr(i_iDetailLevel,i_format);

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgIndReceivedData : public CMsgInd
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgIndReceivedData::CMsgIndReceivedData(
    QObject*          i_pObjSender,
    QObject*          i_pObjReceiver,
    int               i_iSocketId,
    const QByteArray& i_byteArr,
    qint64            i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgInd(
        /* iMsgType     */ EMsgTypeIndReceivedData,
        /* pObjSender   */ i_pObjSender,
        /* pObjReceiver */ i_pObjReceiver,
        /* iMsgIdReq    */ i_iMsgId ),
    m_iSocketId(i_iSocketId),
    m_byteArr(i_byteArr)
{
    setObjectName( Ipc::msgType2Str(EMsgTypeIndReceivedData) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgIndReceivedData::~CMsgIndReceivedData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgIndReceivedData::setByteArray( const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    m_byteArr = i_byteArr;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgIndReceivedData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgIndReceivedData* pMsgCloned = new CMsgIndReceivedData(
        /* pObjSender   */ m_pObjSender,
        /* pObjReceiver */ m_pObjReceiver,
        /* iSocketId    */ m_iSocketId,
        /* byteArr      */ m_byteArr,
        /* iMsgId       */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgAck* CMsgIndReceivedData::createAcknowledgeMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgIndReceivedData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Ipc::IndReceivedData)";
}

//------------------------------------------------------------------------------
QString CMsgIndReceivedData::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgInd::getAddTrcInfoStr(i_iDetailLevel,i_format);

    return strAddTrcInfo;

} // getAddTrcInfoStr
