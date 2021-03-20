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

#include <QtCore/qcoreapplication.h>

#include "ZSData/ZSDataMsg.h"
#include "ZSData/ZSDataProtocol.h"
#include "ZSData/ZSDataSet.h"
//#include "ZSData/ZSDataChangeSet.h"
#include "ZSIpc/ZSIpcSocketWrapper.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Data;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
enum MsgType
==============================================================================*/

const SEnumEntry s_arEnumStrMsgTypes[ZS::Data::EMsgTypeCount] =
{
    /*  0 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqReceiveData,    "ReqReceiveData"    ),
    /*  1 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqStartup,        "ReqStartup"        ),
    /*  2 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConStartup,        "ConStartup"        ),
    /*  3 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqShutdown,       "ReqShutdown"       ),
    /*  4 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConShutdown,       "ConShutdown"       ),
    /*  5 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqConnect,        "ReqConnect"        ),
    /*  6 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConConnect,        "ConConnect"        ),
    /*  7 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeIndConnected,      "IndConnected"      ),
    /*  8 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqDisconnect,     "ReqDisconnect"     ),
    /*  9 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConDisconnect,     "ConDisconnect"     ),
    /* 10 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeIndDisconnected,   "IndDisconnected"   ),
    /* 11 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqChangeSettings, "ReqChangeSettings" ),
    /* 12 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConChangeSettings, "ConChangeSettings" ),
    // "Simple" Commands
    /* 13 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqReadSchema,     "ReqReadSchema"     ),
    /* 14 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConReadSchema,     "ConReadSchema"     ),
    /* 15 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqSaveSchema,     "ReqSaveSchema"     ),
    /* 16 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConSaveSchema,     "ConSaveSchema"     ),
    /* 17 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqReadData,       "ReqReadData"       ),
    /* 18 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConReadData,       "ConReadData"       ),
    /* 19 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqSaveData,       "ReqSaveData"       ),
    /* 20 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConSaveData,       "ConSaveData"       ),
    /* 21 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqSet2Default,    "ReqSet2Default"    ),
    /* 22 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConSet2Default,    "ConSet2Default"    ),
    // With Node Specs
    /* 23 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqRegister,       "ReqRegister"       ),
    /* 24 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConRegister,       "ConRegister"       ),
    /* 25 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqUnregister,     "ReqUnregister"     ),
    /* 26 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConUnregister,     "ConUnregister"     ),
    /* 27 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqSelectSchema,   "ReqSelectSchema"   ),
    /* 28 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConSelectSchema,   "ConSelectSchema"   ),
    /* 29 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqSelectData,     "ReqSelectData"     ),
    /* 30 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConSelectData,     "ConSelectData"     ),
    // With Data Sets
    /* 31 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqUpdateSchema,   "ReqUpdateSchema"   ),
    /* 32 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConUpdateSchema,   "ConUpdateSchema"   ),
    /* 33 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeReqUpdateData,     "ReqUpdateData"     ),
    /* 34 */ SEnumEntry( -ZS::Data::EMsgTypeMin+ZS::Data::EMsgTypeConUpdateData,     "ConUpdateData"     )
};

//------------------------------------------------------------------------------
QString ZS::Data::msgType2Str( int i_iMsgType )
//------------------------------------------------------------------------------
{
    QString strMsgType = QString::number(i_iMsgType);
    int     iMsgType   = i_iMsgType;

    if( iMsgType >= EMsgTypeMin && iMsgType <= EMsgTypeMax )
    {
        iMsgType -= EMsgTypeMin;
        strMsgType = SEnumEntry::enumerator2Str(s_arEnumStrMsgTypes,EMsgTypeCount,iMsgType);
    }
    return strMsgType;

} // msgType2Str


/*******************************************************************************
class CMsgClassFactory
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgClassFactory::CreateInstance( const QByteArray& i_byteArr, CIpcSocketWrapper* i_pIpcSocketWrapper )
//------------------------------------------------------------------------------
{
    ZS::System::CMsg* pMsg = nullptr;

    SErrResultInfo errResultInfo("ZS::Data", "CMsgClassFactory", "theInst", "CreateInstance");
    QString        strAddErrInfo;

    const ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<const ZS::System::MsgProtocol::SMsgHeader*>(i_byteArr.data());

    /*
    Format and content of data messages (see ZSDataProtocol.h)
    ----------------------------------------------------------

    ================================================
    | System:MsgHeader | Fixed Size Elements       |
    |                  +---------------------------+
    |                  | System:MsgHeader:Buffer   |
    ================================================ -----------
    | Data:SMsgHeader  | Fixed Size Elements       |           #
    +------------------+---------------------------+           |
    |                  | Data:MsgHeader:Buffer     |           |
    ================================================    System:MsgHeader:Data Block
    |                                              |           |
    | Data Block of Data Message                   |           |
    |                                              |           #
    ================================================ -----------
    */

    // System:MsgHeader:Fixed Size Elements
    int iMsgProtSize = sizeof(ZS::System::MsgProtocol::SMsgHeader);

    // System:MsgHeader:Buffer
    iMsgProtSize += pSysMsgHdr->m_blockLenBuffer;

    // System:MsgHeader:Data Block
    iMsgProtSize += pSysMsgHdr->m_blockLenData;

    if( i_byteArr.size() != iMsgProtSize )
    {
        strAddErrInfo += "Received " + QString::number(i_byteArr.size()) + " bytes ";
        strAddErrInfo += " but " + QString::number(iMsgProtSize) + " bytes are specified by message header.";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidMessageContent);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }
    else // if( i_byteArr.size() == iMsgProtSize )
    {
        errResultInfo = checkMsgHeader(pSysMsgHdr);

        if( !errResultInfo.isErrorResult() )
        {
            switch( pSysMsgHdr->m_msgType )
            {
                //--------------------------------------------------------------
                // Internal messages (not serialized)
                //--------------------------------------------------------------

                case EMsgTypeReqReceiveData:
                case EMsgTypeReqStartup:
                case EMsgTypeConStartup:
                case EMsgTypeReqShutdown:
                case EMsgTypeConShutdown:
                case EMsgTypeReqChangeSettings:
                case EMsgTypeConChangeSettings:
                case EMsgTypeReqConnect:
                case EMsgTypeConConnect:
                case EMsgTypeIndConnected:
                case EMsgTypeReqDisconnect:
                case EMsgTypeConDisconnect:
                case EMsgTypeIndDisconnected:
                {
                    strAddErrInfo += "Message type " + msgType2Str(pSysMsgHdr->m_msgType) + " not supported";
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInvalidMessageContent);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                    break;
                }

                //--------------------------------------------------------------
                // "Simple" Commands
                //--------------------------------------------------------------

                case EMsgTypeReqReadSchema:
                {
                    pMsg = new ZS::Data::CMsgReqReadSchema(pSysMsgHdr);
                    break;
                }
                case EMsgTypeConReadSchema:
                {
                    pMsg = new ZS::Data::CMsgConReadSchema(pSysMsgHdr);
                    break;
                }
                case EMsgTypeReqSaveSchema:
                {
                    pMsg = new ZS::Data::CMsgReqSaveSchema(pSysMsgHdr);
                    break;
                }
                case EMsgTypeConSaveSchema:
                {
                    pMsg = new ZS::Data::CMsgConSaveSchema(pSysMsgHdr);
                    break;
                }
                case EMsgTypeReqReadData:
                {
                    pMsg = new ZS::Data::CMsgReqReadData(pSysMsgHdr);
                    break;
                }
                case EMsgTypeConReadData:
                {
                    pMsg = new ZS::Data::CMsgConReadData(pSysMsgHdr);
                    break;
                }
                case EMsgTypeReqSaveData:
                {
                    pMsg = new ZS::Data::CMsgReqSaveData(pSysMsgHdr);
                    break;
                }
                case EMsgTypeConSaveData:
                {
                    pMsg = new ZS::Data::CMsgConSaveData(pSysMsgHdr);
                    break;
                }
                case EMsgTypeReqSet2Default:
                {
                    pMsg = new ZS::Data::CMsgReqSet2Default(pSysMsgHdr);
                    break;
                }
                case EMsgTypeConSet2Default:
                {
                    pMsg = new ZS::Data::CMsgConSet2Default(pSysMsgHdr);
                    break;
                }

                //--------------------------------------------------------------
                // With Node Specs
                //--------------------------------------------------------------

                case EMsgTypeReqRegister:
                {
                    pMsg = new ZS::Data::CMsgReqRegister(pSysMsgHdr);
                    break;
                }
                case EMsgTypeConRegister:
                {
                    pMsg = new ZS::Data::CMsgConRegister(pSysMsgHdr);
                    break;
                }
                case EMsgTypeReqUnregister:
                {
                    pMsg = new ZS::Data::CMsgReqUnregister(pSysMsgHdr);
                    break;
                }
                case EMsgTypeConUnregister:
                {
                    pMsg = new ZS::Data::CMsgConUnregister(pSysMsgHdr);
                    break;
                }
                case EMsgTypeReqSelectSchema:
                {
                    pMsg = new ZS::Data::CMsgReqSelectSchema(pSysMsgHdr);
                    break;
                }
                case EMsgTypeConSelectSchema:
                {
                    pMsg = new ZS::Data::CMsgConSelectSchema(pSysMsgHdr);
                    break;
                }
                case EMsgTypeReqSelectData:
                {
                    pMsg = new ZS::Data::CMsgReqSelectData(pSysMsgHdr);
                    break;
                }
                case EMsgTypeConSelectData:
                {
                    pMsg = new ZS::Data::CMsgConSelectData(pSysMsgHdr);
                    break;
                }

                //--------------------------------------------------------------
                // With Data Sets
                //--------------------------------------------------------------

                case EMsgTypeReqUpdateSchema:
                {
                    pMsg = new ZS::Data::CMsgReqUpdateSchema(pSysMsgHdr);
                    break;
                }
                case EMsgTypeConUpdateSchema:
                {
                    pMsg = new ZS::Data::CMsgConUpdateSchema(pSysMsgHdr);
                    break;
                }
                case EMsgTypeReqUpdateData:
                {
                    pMsg = new ZS::Data::CMsgReqUpdateData(pSysMsgHdr);
                    break;
                }
                case EMsgTypeConUpdateData:
                {
                    pMsg = new ZS::Data::CMsgConUpdateData(pSysMsgHdr);
                    break;
                }

                //--------------------------------------------------------------
                // Out of range
                //--------------------------------------------------------------

                default:
                {
                    strAddErrInfo += "Message type " + msgType2Str(pSysMsgHdr->m_msgType) + " out of range";
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInvalidMessageContent);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                    break;
                }
            } // switch( pSysMsgHdr->m_msgType )

            if( !errResultInfo.isErrorResult() )
            {
                if( pMsg == nullptr )
                {
                    strAddErrInfo += "Message type '0x" + QString::number(pSysMsgHdr->m_msgType,16) + "' out of range";
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInvalidMessageContent);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                }
            }
        } // if( !errResultInfo.isErrorResult() )
    } // if( i_byteArr.size() == iMsgProtSize )

    if( pMsg != nullptr && i_pIpcSocketWrapper != nullptr )
    {
        ZS::Data::CMsgReq* pMsgReq = nullptr;
        ZS::Data::CMsgCon* pMsgCon = nullptr;
        ZS::Data::CMsgInd* pMsgInd = nullptr;
        ZS::Data::CMsgAck* pMsgAck = nullptr;

        SCnctId cnctId( EProtocolTypeZSXML, i_pIpcSocketWrapper->socketType(), i_pIpcSocketWrapper->getSocketId() );

        if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeReq )
        {
            pMsgReq = dynamic_cast<ZS::Data::CMsgReq*>(pMsg);
            pMsgReq->setCnctId(cnctId);
        }
        else if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeCon )
        {
            pMsgCon = dynamic_cast<ZS::Data::CMsgCon*>(pMsg);
            pMsgCon->setCnctId(cnctId);
        }
        else if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeInd )
        {
            pMsgInd = dynamic_cast<ZS::Data::CMsgInd*>(pMsg);
            pMsgInd->setCnctId(cnctId);
        }
        else if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeAck )
        {
            pMsgAck = dynamic_cast<ZS::Data::CMsgAck*>(pMsg);
            pMsgAck->setCnctId(cnctId);
        }
    } // if( pMsg != nullptr && i_pIpcSocketWrapper != nullptr )

    if( errResultInfo.isErrorResult() )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    return pMsg;

} // CreateInstance


/*******************************************************************************
class CMsgReqReceiveData : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqReceiveData::CMsgReqReceiveData(
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
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqReceiveData::~CMsgReqReceiveData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqReceiveData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqReceiveData* pMsgCloned = new ZS::Data::CMsgReqReceiveData(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* iSocketId        */ m_iSocketId,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqReceiveData::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqReceiveData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqReceiveData::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += ", SocketId: " + socketId2Str(m_iSocketId);
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqStartup : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqStartup::CMsgReqStartup(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    EProtocolType              i_protocolType,
    const SServerHostSettings& i_hostSettings,
    bool                       i_bMustBeConfirmed,
    qint64                     i_iReqId,
    qint64                     i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgReq(
        /* iMsgType         */ EMsgTypeReqStartup,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_srvCltType(ESrvCltTypeServer),
    m_protocolType(i_protocolType),
    m_serverHostSettings(i_hostSettings),
    m_clientHostSettings(),
    m_pWatchDogSettings(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqStartup::CMsgReqStartup(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    EProtocolType              i_protocolType,
    const SClientHostSettings& i_hostSettings,
    bool                       i_bMustBeConfirmed,
    qint64                     i_iReqId,
    qint64                     i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgReq(
        /* iMsgType         */ EMsgTypeReqStartup,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_srvCltType(ESrvCltTypeClient),
    m_protocolType(i_protocolType),
    m_serverHostSettings(),
    m_clientHostSettings(i_hostSettings),
    m_pWatchDogSettings(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqStartup::CMsgReqStartup(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    EProtocolType              i_protocolType,
    const SClientHostSettings& i_hostSettings,
    const STimerSettings&      i_watchDogSettings,
    bool                       i_bMustBeConfirmed,
    qint64                     i_iReqId,
    qint64                     i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgReq(
        /* iMsgType         */ EMsgTypeReqStartup,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_srvCltType(ESrvCltTypeClient),
    m_protocolType(i_protocolType),
    m_serverHostSettings(),
    m_clientHostSettings(i_hostSettings),
    m_pWatchDogSettings(nullptr)
{
    m_pWatchDogSettings = new STimerSettings(i_watchDogSettings);

} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqStartup::~CMsgReqStartup()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pWatchDogSettings;
    }
    catch(...)
    {
    }

    m_srvCltType = static_cast<ESrvCltType>(0);
    m_protocolType = static_cast<EProtocolType>(0);
    //m_serverHostSettings;
    //m_clientHostSettings;
    m_pWatchDogSettings = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ESocketType ZS::Data::CMsgReqStartup::getSocketType() const
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
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg*ZS::Data::CMsgReqStartup::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqStartup* pMsgCloned = nullptr;

    if( m_srvCltType == ESrvCltTypeServer )
    {
       pMsgCloned = new ZS::Data::CMsgReqStartup(
            /* pObjSender       */ m_pObjSender,
            /* pObjReceiver     */ m_pObjReceiver,
            /* protocolType     */ m_protocolType,
            /* hostSettings     */ m_serverHostSettings,
            /* bMustBeConfirmed */ m_bMustBeConfirmed,
            /* iReqId           */ m_iReqId,
            /* iMsgId           */ m_iMsgId );
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        if( m_pWatchDogSettings == nullptr )
        {
           pMsgCloned = new ZS::Data::CMsgReqStartup(
                /* pObjSender       */ m_pObjSender,
                /* pObjReceiver     */ m_pObjReceiver,
                /* protocolType     */ m_protocolType,
                /* hostSettings     */ m_clientHostSettings,
                /* bMustBeConfirmed */ m_bMustBeConfirmed,
                /* iReqId           */ m_iReqId,
                /* iMsgId           */ m_iMsgId );
        }
        else // if( m_pWatchDogSettings != nullptr )
        {
            pMsgCloned = new ZS::Data::CMsgReqStartup(
                /* pObjSender       */ m_pObjSender,
                /* pObjReceiver     */ m_pObjReceiver,
                /* protocolType     */ m_protocolType,
                /* hostSettings     */ m_clientHostSettings,
                /* watchDogSettings */ *m_pWatchDogSettings,
                /* bMustBeConfirmed */ m_bMustBeConfirmed,
                /* iReqId           */ m_iReqId,
                /* iMsgId           */ m_iMsgId );
        }
    }

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon*ZS::Data::CMsgReqStartup::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
   ZS::Data::CMsgConStartup* pMsgCon = new ZS::Data::CMsgConStartup(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqStartup::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqStartup::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += ", SrvCltType: " + srvCltType2Str(m_srvCltType);
    strAddTrcInfo += ", ProtType: " + protocolType2Str(m_protocolType);
    strAddTrcInfo += "}";

    if( m_srvCltType == ESrvCltTypeServer )
    {
        strAddTrcInfo += ", HostSettings { " + m_serverHostSettings.getConnectionString() + " }";
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        strAddTrcInfo += ", HostSettings { " + m_clientHostSettings.getConnectionString() + " }";
        //strAddTrcInfo += ", WatchDogSettings { " + m_watchDogSettings.toConnectionString() + " }";
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
ZS::Data::CMsgConStartup::CMsgConStartup(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgCon(
        /* iMsgType           */ EMsgTypeConStartup,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConStartup::~CMsgConStartup()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConStartup::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConStartup* pMsgCloned = new ZS::Data::CMsgConStartup(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
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
QString ZS::Data::CMsgConStartup::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConStartup::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqShutdown : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqShutdown::CMsgReqShutdown(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgReq(
        /* iMsgType         */ EMsgTypeReqShutdown,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqShutdown::~CMsgReqShutdown()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqShutdown::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqShutdown* pMsgCloned = new ZS::Data::CMsgReqShutdown(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqShutdown::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConShutdown* pMsgCon = new ZS::Data::CMsgConShutdown(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqShutdown::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqShutdown::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConShutdown : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConShutdown::CMsgConShutdown(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgCon(
        /* iMsgType           */ EMsgTypeConShutdown,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConShutdown::~CMsgConShutdown()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConShutdown::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConShutdown* pMsgCloned = new ZS::Data::CMsgConShutdown(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
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
QString ZS::Data::CMsgConShutdown::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConShutdown::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqConnect : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqConnect::CMsgReqConnect(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    EProtocolType              i_protocolType,
    const SClientHostSettings& i_hostSettings,
    bool                       i_bMustBeConfirmed,
    qint64                     i_iReqId,
    qint64                     i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgReq(
        /* iMsgType         */ EMsgTypeReqConnect,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_protocolType(i_protocolType),
    m_hostSettings(i_hostSettings),
    m_pWatchDogSettings(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqConnect::CMsgReqConnect(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    EProtocolType              i_protocolType,
    const SClientHostSettings& i_hostSettings,
    const STimerSettings&      i_watchDogSettings,
    bool                       i_bMustBeConfirmed,
    qint64                     i_iReqId,
    qint64                     i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgReq(
        /* iMsgType         */ EMsgTypeReqConnect,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_protocolType(i_protocolType),
    m_hostSettings(i_hostSettings),
    m_pWatchDogSettings(nullptr)
{
    m_pWatchDogSettings = new STimerSettings(i_watchDogSettings);

} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqConnect::~CMsgReqConnect()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pWatchDogSettings;
    }
    catch(...)
    {
    }

    m_pWatchDogSettings = nullptr;

} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqConnect::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqConnect* pMsgCloned = nullptr;

    if( m_pWatchDogSettings == nullptr )
    {
        pMsgCloned = new ZS::Data::CMsgReqConnect(
            /* pObjSender       */ m_pObjSender,
            /* pObjReceiver     */ m_pObjReceiver,
            /* protocolType     */ m_protocolType,
            /* hostSettings     */ m_hostSettings,
            /* bMustBeConfirmed */ m_bMustBeConfirmed,
            /* iReqId           */ m_iReqId,
            /* iMsgId           */ m_iMsgId );
    }
    else // if( m_pWatchDogSettings != nullptr )
    {
        pMsgCloned = new ZS::Data::CMsgReqConnect(
            /* pObjSender       */ m_pObjSender,
            /* pObjReceiver     */ m_pObjReceiver,
            /* protocolType     */ m_protocolType,
            /* hostSettings     */ m_hostSettings,
            /* watchDogSettings */ *m_pWatchDogSettings,
            /* bMustBeConfirmed */ m_bMustBeConfirmed,
            /* iReqId           */ m_iReqId,
            /* iMsgId           */ m_iMsgId );
    }

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqConnect::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConConnect* pMsgCon = new ZS::Data::CMsgConConnect(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* protocolType      */ m_protocolType,
        /* hostSettings      */ m_hostSettings.getSocketDscr(),
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqConnect::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqConnect::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += ", ProtType: " + protocolType2Str(m_protocolType);
    strAddTrcInfo += ", HostSettings { " + m_hostSettings.getConnectionString() + " }";
    //strAddTrcInfo += ", WatchDogSettings { " + m_watchDogSettings + " }";
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConConnect : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConConnect::CMsgConConnect(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    EProtocolType         i_protocolType,
    const SSocketDscr&    i_socketDscr,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgCon(
        /* iMsgType           */ EMsgTypeConConnect,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_protocolType(i_protocolType),
    m_socketDscr(i_socketDscr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConConnect::~CMsgConConnect()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void ZS::Data::CMsgConConnect::setSocketState( Ipc::ESocketState i_socketState )
//------------------------------------------------------------------------------
{
    m_socketDscr.m_socketState = i_socketState;
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgConConnect::setSocketDscr( const Ipc::SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    m_socketDscr = i_socketDscr;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConConnect::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConConnect* pMsgCloned = new ZS::Data::CMsgConConnect(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* protocolType     */ m_protocolType,
        /* socketDscr       */ m_socketDscr,
        /* iReqId           */ m_iReqId,
        /* iMsgIdReq        */ m_iMsgId,
        /* errResultInfo    */ m_errResultInfo,
        /* iProgress        */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConConnect::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConConnect::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += ", ProtType: " + protocolType2Str(m_protocolType);
    strAddTrcInfo += ", SocketDscr { " + m_socketDscr.getConnectionString() + " }";
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgIndConnected : public CMsgInd
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgIndConnected::CMsgIndConnected(
    QObject*           i_pObjSender,
    QObject*           i_pObjReceiver,
    EProtocolType      i_protocolType,
    const SSocketDscr& i_socketDscr,
    qint64             i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgInd(
        /* iMsgType     */ EMsgTypeIndConnected,
        /* pObjSender   */ i_pObjSender,
        /* pObjReceiver */ i_pObjReceiver,
        /* iMsgIdReq    */ i_iMsgId ),
    m_protocolType(i_protocolType),
    m_socketDscr(i_socketDscr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgIndConnected::~CMsgIndConnected()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgIndConnected::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgIndConnected* pMsgCloned = new ZS::Data::CMsgIndConnected(
        /* pObjSender   */ m_pObjSender,
        /* pObjReceiver */ m_pObjReceiver,
        /* protocolType */ m_protocolType,
        /* socketDscr   */ m_socketDscr,
        /* iMsgId       */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgAck* ZS::Data::CMsgIndConnected::createAcknowledgeMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgIndConnected::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgIndConnected::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgInd::getAddTrcInfoStr();
    strAddTrcInfo += ", ProtType: " + protocolType2Str(m_protocolType);
    strAddTrcInfo += ", SocketDscr { " + m_socketDscr.getConnectionString() + " }";
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqDisconnect : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqDisconnect::CMsgReqDisconnect(
    QObject*           i_pObjSender,
    QObject*           i_pObjReceiver,
    EProtocolType      i_protocolType,
    const SSocketDscr& i_socketDscr,
    bool               i_bMustBeConfirmed,
    qint64             i_iReqId,
    qint64             i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgReq(
        /* iMsgType         */ EMsgTypeReqDisconnect,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_protocolType(i_protocolType),
    m_socketDscr(i_socketDscr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqDisconnect::~CMsgReqDisconnect()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqDisconnect::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqDisconnect* pMsgCloned = new ZS::Data::CMsgReqDisconnect(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* protocolType     */ m_protocolType,
        /* socketDscr       */ m_socketDscr,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqDisconnect::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConDisconnect* pMsgCon = new ZS::Data::CMsgConDisconnect(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* protocolType      */ m_protocolType,
        /* socketDscr        */ m_socketDscr,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqDisconnect::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqDisconnect::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += ", ProtType: " + protocolType2Str(m_protocolType);
    strAddTrcInfo += ", SocketDscr { " + m_socketDscr.getConnectionString() + " }";
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConDisconnect : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConDisconnect::CMsgConDisconnect(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    EProtocolType         i_protocolType,
    const SSocketDscr&    i_socketDscr,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgCon(
        /* iMsgType           */ EMsgTypeConDisconnect,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_protocolType(i_protocolType),
    m_socketDscr(i_socketDscr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConDisconnect::~CMsgConDisconnect()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConDisconnect::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConDisconnect* pMsgCloned = new ZS::Data::CMsgConDisconnect(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
        /* protocolType  */ m_protocolType,
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
QString ZS::Data::CMsgConDisconnect::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConDisconnect::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += ", ProtType: " + protocolType2Str(m_protocolType);
    strAddTrcInfo += ", SocketDscr { " + m_socketDscr.getConnectionString() + " }";
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgIndDisconnected : public CMsgInd
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgIndDisconnected::CMsgIndDisconnected(
    QObject*           i_pObjSender,
    QObject*           i_pObjReceiver,
    EProtocolType      i_protocolType,
    const SSocketDscr& i_socketDscr,
    qint64             i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgInd(
        /* iMsgType            */ EMsgTypeIndDisconnected,
        /* pObjSender          */ i_pObjSender,
        /* pObjReceiver        */ i_pObjReceiver,
        /* bMustBeAcknowledged */ false,
        /* iMsgIdReq           */ i_iMsgId ),
    m_protocolType(i_protocolType),
    m_socketDscr(i_socketDscr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgIndDisconnected::~CMsgIndDisconnected()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgIndDisconnected::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgIndDisconnected* pMsgCloned = new ZS::Data::CMsgIndDisconnected(
        /* pObjSender   */ m_pObjSender,
        /* pObjReceiver */ m_pObjReceiver,
        /* protocolType */ m_protocolType,
        /* socketDscr   */ m_socketDscr,
        /* iMsgId       */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgAck* ZS::Data::CMsgIndDisconnected::createAcknowledgeMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgIndDisconnected::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgIndDisconnected::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgInd::getAddTrcInfoStr();
    strAddTrcInfo += ", ProtType: " + protocolType2Str(m_protocolType);
    strAddTrcInfo += ", SocketDscr { " + m_socketDscr.getConnectionString() + " }";
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqChangeSettings : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqChangeSettings::CMsgReqChangeSettings(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    EProtocolType              i_protocolType,
    const SServerHostSettings& i_hostSettings,
    bool                       i_bMustBeConfirmed,
    qint64                     i_iReqId,
    qint64                     i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgReq(
        /* iMsgType         */ EMsgTypeReqChangeSettings,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_srvCltType(ESrvCltTypeServer),
    m_protocolType(i_protocolType),
    m_serverHostSettings(i_hostSettings),
    m_clientHostSettings(),
    m_pWatchDogSettings(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqChangeSettings::CMsgReqChangeSettings(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    EProtocolType              i_protocolType,
    const SClientHostSettings& i_hostSettings,
    bool                       i_bMustBeConfirmed,
    qint64                     i_iReqId,
    qint64                     i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgReq(
        /* iMsgType         */ EMsgTypeReqChangeSettings,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_srvCltType(ESrvCltTypeClient),
    m_protocolType(i_protocolType),
    m_serverHostSettings(),
    m_clientHostSettings(i_hostSettings),
    m_pWatchDogSettings(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqChangeSettings::CMsgReqChangeSettings(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    EProtocolType              i_protocolType,
    const SClientHostSettings& i_hostSettings,
    const STimerSettings&      i_watchDogSettings,
    bool                       i_bMustBeConfirmed,
    qint64                     i_iReqId,
    qint64                     i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgReq(
        /* iMsgType         */ EMsgTypeReqChangeSettings,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_srvCltType(ESrvCltTypeClient),
    m_protocolType(i_protocolType),
    m_serverHostSettings(),
    m_clientHostSettings(i_hostSettings),
    m_pWatchDogSettings(nullptr)
{
    m_pWatchDogSettings = new STimerSettings(i_watchDogSettings);

} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqChangeSettings::~CMsgReqChangeSettings()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pWatchDogSettings;
    }
    catch(...)
    {
    }

    m_srvCltType = static_cast<ESrvCltType>(0);
    m_protocolType = static_cast<EProtocolType>(0);
    //m_serverHostSettings;
    //m_clientHostSettings;
    m_pWatchDogSettings = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ESocketType ZS::Data::CMsgReqChangeSettings::getSocketType() const
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
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqChangeSettings::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqChangeSettings* pMsgCloned = nullptr;

    if( m_srvCltType == ESrvCltTypeServer )
    {
        pMsgCloned = new ZS::Data::CMsgReqChangeSettings(
            /* pObjSender       */ m_pObjSender,
            /* pObjReceiver     */ m_pObjReceiver,
            /* protocolType     */ m_protocolType,
            /* hostSettings     */ m_serverHostSettings,
            /* bMustBeConfirmed */ m_bMustBeConfirmed,
            /* iReqId           */ m_iReqId,
            /* iMsgId           */ m_iMsgId );
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        if( m_pWatchDogSettings == nullptr )
        {
            pMsgCloned = new ZS::Data::CMsgReqChangeSettings(
                /* pObjSender       */ m_pObjSender,
                /* pObjReceiver     */ m_pObjReceiver,
                /* protocolType     */ m_protocolType,
                /* hostSettings     */ m_clientHostSettings,
                /* bMustBeConfirmed */ m_bMustBeConfirmed,
                /* iReqId           */ m_iReqId,
                /* iMsgId           */ m_iMsgId );
        }
        else // if( m_pWatchDogSettings != nullptr )
        {
            pMsgCloned = new ZS::Data::CMsgReqChangeSettings(
                /* pObjSender       */ m_pObjSender,
                /* pObjReceiver     */ m_pObjReceiver,
                /* protocolType     */ m_protocolType,
                /* hostSettings     */ m_clientHostSettings,
                /* watchDogSettings */ *m_pWatchDogSettings,
                /* bMustBeConfirmed */ m_bMustBeConfirmed,
                /* iReqId           */ m_iReqId,
                /* iMsgId           */ m_iMsgId );
        }
    }

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqChangeSettings::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConChangeSettings* pMsgCon = nullptr;

    if( m_srvCltType == ESrvCltTypeServer )
    {
        pMsgCon = new ZS::Data::CMsgConChangeSettings(
            /* pObjSender        */ m_pObjReceiver,
            /* pObjReceiver      */ m_pObjSender,
            /* protocolType      */ m_protocolType,
            /* hostSettings      */ m_serverHostSettings,
            /* iReqId            */ m_iReqId,
            /* iMsgIdReq         */ m_iMsgId,
            /* errResultInfo     */ SErrResultInfo(),
            /* iProgress_perCent */ 100 );
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        pMsgCon = new ZS::Data::CMsgConChangeSettings(
            /* pObjSender        */ m_pObjReceiver,
            /* pObjReceiver      */ m_pObjSender,
            /* protocolType      */ m_protocolType,
            /* socketDscr        */ m_clientHostSettings.getSocketDscr(),
            /* iReqId            */ m_iReqId,
            /* iMsgIdReq         */ m_iMsgId,
            /* errResultInfo     */ SErrResultInfo(),
            /* iProgress_perCent */ 100 );
    }

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqChangeSettings::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqChangeSettings::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += ", SrvCltType: " + srvCltType2Str(m_srvCltType);
    strAddTrcInfo += ", ProtType: " + protocolType2Str(m_protocolType);
    strAddTrcInfo += "}";

    if( m_srvCltType == ESrvCltTypeServer )
    {
        strAddTrcInfo += ", HostSettings { " + m_serverHostSettings.getConnectionString() + " }";
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        strAddTrcInfo += ", HostSettings { " + m_clientHostSettings.getConnectionString() + " }";
        //strAddTrcInfo += ", WatchDogSettings { " + m_watchDogSettings.toConnectionString() + " }";
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
ZS::Data::CMsgConChangeSettings::CMsgConChangeSettings(
    QObject*                   i_pObjSender,
    QObject*                   i_pObjReceiver,
    EProtocolType              i_protocolType,
    const SServerHostSettings& i_hostSettings,
    qint64                     i_iReqId,
    qint64                     i_iMsgIdReq,
    const SErrResultInfo&      i_errResultInfo,
    int                        i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgCon(
        /* iMsgType           */ EMsgTypeConChangeSettings,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_srvCltType(ESrvCltTypeServer),
    m_protocolType(i_protocolType),
    m_serverHostSettings(i_hostSettings),
    m_clientSocketDscr()
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConChangeSettings::CMsgConChangeSettings(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    EProtocolType         i_protocolType,
    const SSocketDscr&    i_socketDscr,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgCon(
        /* iMsgType           */ EMsgTypeConChangeSettings,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_srvCltType(ESrvCltTypeClient),
    m_protocolType(i_protocolType),
    m_serverHostSettings(),
    m_clientSocketDscr(i_socketDscr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConChangeSettings::~CMsgConChangeSettings()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ESocketType ZS::Data::CMsgConChangeSettings::getSocketType() const
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

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConChangeSettings::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConChangeSettings* pMsgCloned = nullptr;

    if( m_srvCltType == ESrvCltTypeServer )
    {
        pMsgCloned = new ZS::Data::CMsgConChangeSettings(
            /* pObjSender       */ m_pObjSender,
            /* pObjReceiver     */ m_pObjReceiver,
            /* protocolType     */ m_protocolType,
            /* hostSettings     */ m_serverHostSettings,
            /* iReqId           */ m_iReqId,
            /* iMsgId           */ m_iMsgId,
            /* errResultInfo    */ m_errResultInfo,
            /* iProgress        */ m_iProgressInPerCent );
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        pMsgCloned = new ZS::Data::CMsgConChangeSettings(
            /* pObjSender       */ m_pObjSender,
            /* pObjReceiver     */ m_pObjReceiver,
            /* protocolType     */ m_protocolType,
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
QString ZS::Data::CMsgConChangeSettings::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConChangeSettings::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::System::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += ", SrvCltType: " + srvCltType2Str(m_srvCltType);
    strAddTrcInfo += ", ProtType: " + protocolType2Str(m_protocolType);

    if( m_srvCltType == ESrvCltTypeServer )
    {
        strAddTrcInfo += ", HostSettings { " + m_serverHostSettings.getConnectionString() + " }";
    }
    else // if( m_srvCltType == ESrvCltTypeClient )
    {
        strAddTrcInfo += ", SocketDscr { " + m_clientSocketDscr.getConnectionString() + " }";
        //strAddTrcInfo += ", WatchDogSettings { " + m_watchDogSettings.toConnectionString() + " }";
    }
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReq : public ZS::System::CMsgReq
*******************************************************************************/

/*==============================================================================
protected: // ctors (pure virtual base class)
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReq::CMsgReq(
    int            i_iMsgType,
    QObject*       i_pObjSender,
    QObject*       i_pObjReceiver,
    const SCnctId& i_cnctId,
    bool           i_bMustBeConfirmed,
    qint64         i_iReqId,
    qint64         i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgReq(
        /* iMsgType         */ i_iMsgType,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_cnctId(i_cnctId)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReq::CMsgReq( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgReq(i_pMsgHdr),
    m_cnctId(EProtocolTypeUndefined, ESocketTypeUndefined)
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    m_cnctId.m_protocolType = static_cast<EProtocolType>(pDataMsgHdr->m_protocolType);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReq::~CMsgReq()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void ZS::Data::CMsgReq::setCnctId( const SCnctId& i_cnctId )
//------------------------------------------------------------------------------
{
    m_cnctId = i_cnctId;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReq::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    strAddTrcInfo += ZS::System::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += ", CnctId { " + m_cnctId.toString() + " }";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgReq::serialize() const
//------------------------------------------------------------------------------
{
    // Create ZS::System Message Header.
    //----------------------------------

    QByteArray byteArrSysMsg = ZS::System::CMsgReq::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrSysMsg.data());

    // Create ZS::Data Message Header with Data Block.
    //------------------------------------------------

    int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader);

    QByteArray byteArrDataMsg(iDataMsgHeaderSize, 0x00);

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(byteArrDataMsg.data());

    pDataMsgHdr->m_protocolVersion = ZS::Data::MsgProtocol::VERSION;
    pDataMsgHdr->m_protocolType    = m_cnctId.m_protocolType;
    pDataMsgHdr->m_iFillByte1      = 0;
    pDataMsgHdr->m_iFillByte2      = 0;
    pDataMsgHdr->m_blockLenBuffer  = 0;
    pDataMsgHdr->m_blockLenData    = 0;

    // Update block length in system message header.
    //----------------------------------------------

    // The data block of the system message is the ZS::Data message header.
    // Set the length of the block in the system message header.
    pSysMsgHdr->m_blockLenData = byteArrDataMsg.size();

    return byteArrSysMsg + byteArrDataMsg;

} // serialize


/*******************************************************************************
class CMsgCon : public ZS::System::CMsgCon
*******************************************************************************/

/*==============================================================================
protected: // ctors (pure virtual base class)
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgCon::CMsgCon(
    int                   i_iMsgType,
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgCon(
        /* iMsgType      */ i_iMsgType,
        /* pObjSender    */ i_pObjSender,
        /* pObjReceiver  */ i_pObjReceiver,
        /* iReqId        */ i_iReqId,
        /* iMsgIdReq     */ i_iMsgIdReq,
        /* errResultInfo */ i_errResultInfo,
        /* iProgress     */ i_iProgressInPerCent ),
    m_cnctId(i_cnctId)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgCon::CMsgCon( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgCon(i_pMsgHdr),
    m_cnctId(EProtocolTypeUndefined, ESocketTypeUndefined)
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    m_cnctId.m_protocolType = static_cast<EProtocolType>(pDataMsgHdr->m_protocolType);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgCon::~CMsgCon()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void ZS::Data::CMsgCon::setCnctId( const SCnctId& i_cnctId )
//------------------------------------------------------------------------------
{
    m_cnctId = i_cnctId;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgCon::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    strAddTrcInfo += ZS::System::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += ", CnctId { " + m_cnctId.toString() + " }";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgCon::serialize() const
//------------------------------------------------------------------------------
{
    // Create ZS::System Message Header.
    //----------------------------------

    QByteArray byteArrSysMsg = ZS::System::CMsgCon::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrSysMsg.data());

    // Create ZS::Data Message Header with Data Block.
    //------------------------------------------------

    int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader);

    QByteArray byteArrDataMsg(iDataMsgHeaderSize, 0x00);

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(byteArrDataMsg.data());

    pDataMsgHdr->m_protocolVersion = ZS::Data::MsgProtocol::VERSION;
    pDataMsgHdr->m_protocolType    = m_cnctId.m_protocolType;
    pDataMsgHdr->m_iFillByte1      = 0;
    pDataMsgHdr->m_iFillByte2      = 0;
    pDataMsgHdr->m_blockLenBuffer  = 0;
    pDataMsgHdr->m_blockLenData    = 0;

    // Update block length in system message header.
    //----------------------------------------------

    // The data block of the system message is the ZS::Data message header.
    // Set the length of the block in the system message header.
    pSysMsgHdr->m_blockLenData = byteArrDataMsg.size();

    return byteArrSysMsg + byteArrDataMsg;

} // serialize


/*******************************************************************************
class CMsgInd : public ZS::System::CMsgInd
*******************************************************************************/

/*==============================================================================
protected: // ctors (pure virtual base class)
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgInd::CMsgInd(
    int            i_iMsgType,
    QObject*       i_pObjSender,
    QObject*       i_pObjReceiver,
    const SCnctId& i_cnctId,
    bool           i_bMustBeAcknowledged,
    qint64         i_iReqId,
    qint64         i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgInd(
        /* iMsgType     */ i_iMsgType,
        /* pObjSender   */ i_pObjSender,
        /* pObjReceiver */ i_pObjReceiver,
        /* bMustBeAck   */ i_bMustBeAcknowledged,
        /* iReqId       */ i_iReqId,
        /* iMsgIdReq    */ i_iMsgId ),
    m_cnctId(i_cnctId)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgInd::CMsgInd( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgInd(i_pMsgHdr),
    m_cnctId(EProtocolTypeUndefined, ESocketTypeUndefined)
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    m_cnctId.m_protocolType = static_cast<EProtocolType>(pDataMsgHdr->m_protocolType);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgInd::~CMsgInd()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void ZS::Data::CMsgInd::setCnctId( const SCnctId& i_cnctId )
//------------------------------------------------------------------------------
{
    m_cnctId = i_cnctId;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgInd::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    strAddTrcInfo += ZS::System::CMsgInd::getAddTrcInfoStr();
    strAddTrcInfo += ", CnctId { " + m_cnctId.toString() + " }";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgInd::serialize() const
//------------------------------------------------------------------------------
{
    // Create ZS::System Message Header.
    //----------------------------------

    QByteArray byteArrSysMsg = ZS::System::CMsgInd::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrSysMsg.data());

    // Create ZS::Data Message Header with Data Block.
    //------------------------------------------------

    int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader);

    QByteArray byteArrDataMsg(iDataMsgHeaderSize, 0x00);

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(byteArrDataMsg.data());

    pDataMsgHdr->m_protocolVersion = ZS::Data::MsgProtocol::VERSION;
    pDataMsgHdr->m_protocolType    = m_cnctId.m_protocolType;
    pDataMsgHdr->m_iFillByte1      = 0;
    pDataMsgHdr->m_iFillByte2      = 0;
    pDataMsgHdr->m_blockLenBuffer  = 0;
    pDataMsgHdr->m_blockLenData    = 0;

    // Update block length in system message header.
    //----------------------------------------------

    // The data block of the system message is the ZS::Data message header.
    // Set the length of the block in the system message header.
    pSysMsgHdr->m_blockLenData = byteArrDataMsg.size();

    return byteArrSysMsg + byteArrDataMsg;

} // serialize


/*******************************************************************************
class CMsgAck : public ZS::System::CMsgAck
*******************************************************************************/

/*==============================================================================
protected: // ctors (pure virtual base class)
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgAck::CMsgAck(
    int                   i_iMsgType,
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdInd,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgAck(
        /* iMsgType      */ i_iMsgType,
        /* pObjSender    */ i_pObjSender,
        /* pObjReceiver  */ i_pObjReceiver,
        /* iReqId        */ i_iReqId,
        /* iMsgIdReq     */ i_iMsgIdInd,
        /* errResultInfo */ i_errResultInfo,
        /* iProgress     */ i_iProgressInPerCent ),
    m_cnctId(i_cnctId)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgAck::CMsgAck( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::System::CMsgAck(i_pMsgHdr),
    m_cnctId(EProtocolTypeUndefined, ESocketTypeUndefined)
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    m_cnctId.m_protocolType = static_cast<EProtocolType>(pDataMsgHdr->m_protocolType);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgAck::~CMsgAck()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void ZS::Data::CMsgAck::setCnctId( const SCnctId& i_cnctId )
//------------------------------------------------------------------------------
{
    m_cnctId = i_cnctId;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgAck::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    strAddTrcInfo += ZS::System::CMsgAck::getAddTrcInfoStr();
    strAddTrcInfo += ", CnctId { " + m_cnctId.toString() + " }";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgAck::serialize() const
//------------------------------------------------------------------------------
{
    // Create ZS::System Message Header.
    //----------------------------------

    QByteArray byteArrSysMsg = ZS::System::CMsgAck::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrSysMsg.data());

    // Create ZS::Data Message Header with Data Block.
    //------------------------------------------------

    int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader);

    QByteArray byteArrDataMsg(iDataMsgHeaderSize, 0x00);

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(byteArrDataMsg.data());

    pDataMsgHdr->m_protocolVersion = ZS::Data::MsgProtocol::VERSION;
    pDataMsgHdr->m_protocolType    = m_cnctId.m_protocolType;
    pDataMsgHdr->m_iFillByte1      = 0;
    pDataMsgHdr->m_iFillByte2      = 0;
    pDataMsgHdr->m_blockLenBuffer  = 0;
    pDataMsgHdr->m_blockLenData    = 0;

    // Update block length in system message header.
    //----------------------------------------------

    // The data block of the system message is the ZS::Data message header.
    // Set the length of the block in the system message header.
    pSysMsgHdr->m_blockLenData = byteArrDataMsg.size();

    return byteArrSysMsg + byteArrDataMsg;

} // serialize


/*******************************************************************************
"Simple" Commands
*******************************************************************************/

/*******************************************************************************
class CMsgReqReadSchema : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqReadSchema::CMsgReqReadSchema(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const SCnctId&  i_cnctId,
    bool            i_bMustBeConfirmed,
    qint64          i_iReqId,
    qint64          i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(
        /* iMsgType         */ EMsgTypeReqReadSchema,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* cnctId           */ i_cnctId,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqReadSchema::CMsgReqReadSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqReadSchema::~CMsgReqReadSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqReadSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqReadSchema* pMsgCloned = new ZS::Data::CMsgReqReadSchema(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* cnctId           */ m_cnctId,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqReadSchema::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConReadSchema* pMsgCon = new ZS::Data::CMsgConReadSchema(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* cnctId            */ m_cnctId,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqReadSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqReadSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConReadSchema : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConReadSchema::CMsgConReadSchema(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(
        /* iMsgType           */ EMsgTypeConReadSchema,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* cnctId             */ i_cnctId,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConReadSchema::CMsgConReadSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConReadSchema::~CMsgConReadSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConReadSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConReadSchema* pMsgCloned = new ZS::Data::CMsgConReadSchema(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* cnctId             */ m_cnctId,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConReadSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConReadSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqSaveSchema : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSaveSchema::CMsgReqSaveSchema(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const SCnctId&  i_cnctId,
    bool            i_bMustBeConfirmed,
    qint64          i_iReqId,
    qint64          i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(
        /* iMsgType         */ EMsgTypeReqSaveSchema,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* cnctId           */ i_cnctId,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSaveSchema::CMsgReqSaveSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSaveSchema::~CMsgReqSaveSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqSaveSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqSaveSchema* pMsgCloned = new ZS::Data::CMsgReqSaveSchema(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* cnctId           */ m_cnctId,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqSaveSchema::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConSaveSchema* pMsgCon = new ZS::Data::CMsgConSaveSchema(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* cnctId            */ m_cnctId,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqSaveSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqSaveSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConSaveSchema : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConSaveSchema::CMsgConSaveSchema(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(
        /* iMsgType           */ EMsgTypeConSaveSchema,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* cnctId             */ i_cnctId,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConSaveSchema::CMsgConSaveSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConSaveSchema::~CMsgConSaveSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConSaveSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConSaveSchema* pMsgCloned = new ZS::Data::CMsgConSaveSchema(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* cnctId             */ m_cnctId,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConSaveSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConSaveSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqReadData : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqReadData::CMsgReqReadData(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const SCnctId&  i_cnctId,
    bool            i_bMustBeConfirmed,
    qint64          i_iReqId,
    qint64          i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(
        /* iMsgType         */ EMsgTypeReqReadData,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* cnctId           */ i_cnctId,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqReadData::CMsgReqReadData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqReadData::~CMsgReqReadData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqReadData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqReadData* pMsgCloned = new ZS::Data::CMsgReqReadData(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* cnctId           */ m_cnctId,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqReadData::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConReadData* pMsgCon = new ZS::Data::CMsgConReadData(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* cnctId            */ m_cnctId,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqReadData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqReadData::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConReadData : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConReadData::CMsgConReadData(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(
        /* iMsgType           */ EMsgTypeConReadData,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* cnctId             */ i_cnctId,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConReadData::CMsgConReadData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConReadData::~CMsgConReadData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConReadData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConReadData* pMsgCloned = new ZS::Data::CMsgConReadData(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* cnctId             */ m_cnctId,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConReadData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConReadData::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqSaveData : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSaveData::CMsgReqSaveData(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const SCnctId&  i_cnctId,
    bool            i_bMustBeConfirmed,
    qint64          i_iReqId,
    qint64          i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(
        /* iMsgType         */ EMsgTypeReqSaveData,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* cnctId           */ i_cnctId,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSaveData::CMsgReqSaveData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSaveData::~CMsgReqSaveData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqSaveData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqSaveData* pMsgCloned = new ZS::Data::CMsgReqSaveData(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* cnctId           */ m_cnctId,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqSaveData::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConSaveData* pMsgCon = new ZS::Data::CMsgConSaveData(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* cnctId            */ m_cnctId,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqSaveData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqSaveData::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConSaveData : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConSaveData::CMsgConSaveData(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(
        /* iMsgType           */ EMsgTypeConSaveData,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* cnctId             */ i_cnctId,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConSaveData::CMsgConSaveData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConSaveData::~CMsgConSaveData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConSaveData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConSaveData* pMsgCloned = new ZS::Data::CMsgConSaveData(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* cnctId             */ m_cnctId,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConSaveData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConSaveData::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqSet2Default : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSet2Default::CMsgReqSet2Default(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const SCnctId&  i_cnctId,
    bool            i_bMustBeConfirmed,
    qint64          i_iReqId,
    qint64          i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(
        /* iMsgType         */ EMsgTypeReqSet2Default,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* cnctId           */ i_cnctId,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSet2Default::CMsgReqSet2Default( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSet2Default::~CMsgReqSet2Default()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqSet2Default::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqSet2Default* pMsgCloned = new ZS::Data::CMsgReqSet2Default(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* cnctId           */ m_cnctId,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqSet2Default::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConSet2Default* pMsgCon = new ZS::Data::CMsgConSet2Default(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* cnctId            */ m_cnctId,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqSet2Default::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqSet2Default::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConSet2Default : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConSet2Default::CMsgConSet2Default(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(
        /* iMsgType           */ EMsgTypeConSet2Default,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* cnctId             */ i_cnctId,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConSet2Default::CMsgConSet2Default( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConSet2Default::~CMsgConSet2Default()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConSet2Default::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConSet2Default* pMsgCloned = new ZS::Data::CMsgConSet2Default(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* cnctId             */ m_cnctId,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConSet2Default::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConSet2Default::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
With Node Specs or Data Sets (ConSelectSchema and ConSelectData)
*******************************************************************************/

/*******************************************************************************
class CMsgReqRegister : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqRegister::CMsgReqRegister(
    QObject*       i_pObjSender,
    QObject*       i_pObjReceiver,
    const SCnctId& i_cnctId,
    bool           i_bMustBeConfirmed,
    qint64         i_iReqId,
    qint64         i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(
        /* iMsgType         */ EMsgTypeReqRegister,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* cnctId           */ i_cnctId,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_dbCltRegSpec(i_cnctId)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqRegister::CMsgReqRegister( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(i_pMsgHdr),
    m_dbCltRegSpec()
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    if( pDataMsgHdr->m_blockLenData > 0 )
    {
        int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer;

        const char* pcDSDataBlock = reinterpret_cast<const char*>(pDataMsgHdr) + iDataMsgHeaderSize;

        QByteArray byteArrRegSpec( pcDSDataBlock, pDataMsgHdr->m_blockLenData );

        m_dbCltRegSpec.deserialize(byteArrRegSpec);

    } // if( pDataMsgHdr->m_blockLenData > 0 )

} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqRegister::~CMsgReqRegister()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class ZS::Data::CMsgReq
==============================================================================*/

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqRegister::setCnctId( const SCnctId& i_cnctId )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReq::setCnctId(i_cnctId);

    m_dbCltRegSpec.m_cnctId = i_cnctId;

} // setCnctId

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqRegister::setClientName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_strClientName = i_strName;
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqRegister::setDSNodeSpec( const SDSNodeSpec& i_dsNodeSpec )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_arDSNodeSpecs.clear();
    m_dbCltRegSpec.m_arDSNodeSpecs.append(i_dsNodeSpec);
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqRegister::setDSNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_arDSNodeSpecs = i_arDSNodeSpecs;
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqRegister::addDSNodeSpec( const SDSNodeSpec& i_dsNodeSpec )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_arDSNodeSpecs.append(i_dsNodeSpec);
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqRegister::addDSNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_arDSNodeSpecs += i_arDSNodeSpecs;
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqRegister::setRegSpec( const SDbClientRegSpec& i_dbCltRegSpec )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec = i_dbCltRegSpec;
}

//------------------------------------------------------------------------------
SDSNodeSpec ZS::Data::CMsgReqRegister::getDSNodeSpec( int i_idx ) const
//------------------------------------------------------------------------------
{
    SDSNodeSpec spec;

    if( i_idx >= 0 && i_idx < m_dbCltRegSpec.m_arDSNodeSpecs.size() )
    {
        spec = m_dbCltRegSpec.m_arDSNodeSpecs[i_idx];
    }
    return spec;

} // getDSNodeSpec

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqRegister::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqRegister* pMsgCloned = new ZS::Data::CMsgReqRegister(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* cnctId           */ m_cnctId,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);
    pMsgCloned->setRegSpec(m_dbCltRegSpec);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqRegister::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConRegister* pMsgCon = new ZS::Data::CMsgConRegister(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* cnctId            */ m_cnctId,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqRegister::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqRegister::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += ", ReqSpec {" + m_dbCltRegSpec.toString(i_iDetailLevel) + "}";
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgReqRegister::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArrDataMsgHdr = ZS::Data::CMsgReq::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrDataMsgHdr.data());

    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + pSysMsgHdr->m_blockLenBuffer;

    char* pcSysDataBlock = reinterpret_cast<char*>(pSysMsgHdr) + iSysMsgHeaderSize;

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    // Create Data Block of ZS::Data Message Header containing NodeSpecs.
    //-------------------------------------------------------------------

    QByteArray byteArrRegSpec;

    m_dbCltRegSpec.serialize(byteArrRegSpec);

    // Update block lengths in system and data message headers.
    //---------------------------------------------------------

    pDataMsgHdr->m_blockLenData = byteArrRegSpec.size();

    pSysMsgHdr->m_blockLenData = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer + pDataMsgHdr->m_blockLenData;

    return byteArrDataMsgHdr + byteArrRegSpec;

} // serialize


/*******************************************************************************
class CMsgConRegister : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConRegister::CMsgConRegister(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(
        /* iMsgType           */ EMsgTypeConRegister,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* cnctId             */ i_cnctId,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConRegister::CMsgConRegister( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConRegister::~CMsgConRegister()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConRegister::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConRegister* pMsgCloned = new ZS::Data::CMsgConRegister(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* cnctId             */ m_cnctId,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConRegister::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConRegister::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqUnregister : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqUnregister::CMsgReqUnregister(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const SCnctId&  i_cnctId,
    bool            i_bMustBeConfirmed,
    qint64          i_iReqId,
    qint64          i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(
        /* iMsgType         */ EMsgTypeReqUnregister,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* cnctId           */ i_cnctId,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqUnregister::CMsgReqUnregister( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqUnregister::~CMsgReqUnregister()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqUnregister::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqUnregister* pMsgCloned = new ZS::Data::CMsgReqUnregister(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* cnctId           */ m_cnctId,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqUnregister::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConUnregister* pMsgCon = new ZS::Data::CMsgConUnregister(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* cnctId            */ m_cnctId,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqUnregister::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqUnregister::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConUnregister : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConUnregister::CMsgConUnregister(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(
        /* iMsgType           */ EMsgTypeConUnregister,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* cnctId             */ i_cnctId,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConUnregister::CMsgConUnregister( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConUnregister::~CMsgConUnregister()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConUnregister::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConUnregister* pMsgCloned = new ZS::Data::CMsgConUnregister(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* cnctId             */ m_cnctId,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConUnregister::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConUnregister::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgCon::getAddTrcInfoStr();
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqSelectSchema : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSelectSchema::CMsgReqSelectSchema(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const SCnctId&  i_cnctId,
    qint64          i_iReqId,
    qint64          i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(
        /* iMsgType         */ EMsgTypeReqSelectSchema,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* cnctId           */ i_cnctId,
        /* bMustBeConfirmed */ true,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_dbCltRegSpec(i_cnctId)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSelectSchema::CMsgReqSelectSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(i_pMsgHdr),
    m_dbCltRegSpec()
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    if( pDataMsgHdr->m_blockLenData > 0 )
    {
        int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer;

        const char* pcDSDataBlock = reinterpret_cast<const char*>(pDataMsgHdr) + iDataMsgHeaderSize;

        QByteArray byteArrRegSpec( pcDSDataBlock, pDataMsgHdr->m_blockLenData );

        m_dbCltRegSpec.deserialize(byteArrRegSpec);

    } // if( pDataMsgHdr->m_blockLenData > 0 )

} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSelectSchema::~CMsgReqSelectSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class ZS::Data::CMsgReq
==============================================================================*/

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectSchema::setCnctId( const SCnctId& i_cnctId )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReq::setCnctId(i_cnctId);

    m_dbCltRegSpec.m_cnctId = i_cnctId;

} // setCnctId

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectSchema::setClientName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_strClientName = i_strName;
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectSchema::setDSNodeSpec( const SDSNodeSpec i_dsNodeSpec )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_arDSNodeSpecs.clear();
    m_dbCltRegSpec.m_arDSNodeSpecs.append(i_dsNodeSpec);
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectSchema::setDSNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_arDSNodeSpecs = i_arDSNodeSpecs;
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectSchema::addDSNodeSpec( const SDSNodeSpec& i_dsNodeSpec )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_arDSNodeSpecs.append(i_dsNodeSpec);
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectSchema::addDSNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_arDSNodeSpecs += i_arDSNodeSpecs;
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectSchema::setRegSpec( const SDbClientRegSpec& i_dbCltRegSpec )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec = i_dbCltRegSpec;
}

//------------------------------------------------------------------------------
SDSNodeSpec ZS::Data::CMsgReqSelectSchema::getDSNodeSpec( int i_idx ) const
//------------------------------------------------------------------------------
{
    SDSNodeSpec dsNodeSpec;

    if( i_idx >= 0 && i_idx < m_dbCltRegSpec.m_arDSNodeSpecs.size() )
    {
        dsNodeSpec = m_dbCltRegSpec.m_arDSNodeSpecs[i_idx];
    }
    return dsNodeSpec;

} // getDSNodeSpec

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqSelectSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqSelectSchema* pMsgCloned = new ZS::Data::CMsgReqSelectSchema(
        /* pObjSender   */ m_pObjSender,
        /* pObjReceiver */ m_pObjReceiver,
        /* cnctId       */ m_cnctId,
        /* iReqId       */ m_iReqId,
        /* iMsgId       */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);
    pMsgCloned->setRegSpec(m_dbCltRegSpec);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqSelectSchema::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConSelectSchema* pMsgCon = new ZS::Data::CMsgConSelectSchema(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* cnctId            */ m_cnctId,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqSelectSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqSelectSchema::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += ", ReqSpec {" + m_dbCltRegSpec.toString(i_iDetailLevel) + "}";
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgReqSelectSchema::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArrDataMsgHdr = ZS::Data::CMsgReq::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrDataMsgHdr.data());

    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + pSysMsgHdr->m_blockLenBuffer;

    char* pcSysDataBlock = reinterpret_cast<char*>(pSysMsgHdr) + iSysMsgHeaderSize;

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    // Create Data Block of ZS::Data Message Header containing NodeSpecs.
    //-------------------------------------------------------------------

    QByteArray byteArrRegSpec;

    m_dbCltRegSpec.serialize(byteArrRegSpec);

    // Update block lengths in system and data message headers.
    //---------------------------------------------------------

    pDataMsgHdr->m_blockLenData = byteArrRegSpec.size();

    pSysMsgHdr->m_blockLenData = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer + pDataMsgHdr->m_blockLenData;

    return byteArrDataMsgHdr + byteArrRegSpec;

} // serialize


/*******************************************************************************
class CMsgConSelectSchema : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConSelectSchema::CMsgConSelectSchema(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(
        /* iMsgType           */ EMsgTypeConSelectSchema,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* cnctId             */ i_cnctId,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
    //m_pChgDS(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConSelectSchema::CMsgConSelectSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(i_pMsgHdr)
    //m_pChgDS(nullptr)
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    if( pDataMsgHdr->m_blockLenData > 0 )
    {
        int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer;

        const char* pcDSDataBlock = reinterpret_cast<const char*>(pDataMsgHdr) + iDataMsgHeaderSize;

        QByteArray byteArrDS( pcDSDataBlock, pDataMsgHdr->m_blockLenData );

        // Message sent from database to client deserialized on client site.
        //m_pChgDS = new CChangeDataSet("MsgConSelectSchema", ERowVersion::Original);

        //m_pChgDS->deserialize(byteArrDS);

    } // if( pDataMsgHdr->m_blockLenData > 0 )

} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConSelectSchema::~CMsgConSelectSchema()
//------------------------------------------------------------------------------
{
    //try
    //{
    //    delete m_pChgDS;
    //}
    //catch(...)
    //{
    //}
    //m_pChgDS = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void ZS::Data::CMsgConSelectSchema::mergeSchema( CChangeDataSet* i_pChgDS )
////------------------------------------------------------------------------------
//{
//    if( m_pChgDS != nullptr )
//    {
//        m_pChgDS->mergeSchema(i_pChgDS);
//    }
//    else
//    {
//        m_pChgDS = i_pChgDS;
//    }
//
//} // mergeSchema

////------------------------------------------------------------------------------
//void ZS::Data::CMsgConSelectSchema::setSchema( CChangeDataSet* i_pChgDS )
////------------------------------------------------------------------------------
//{
//    delete m_pChgDS;
//    m_pChgDS = i_pChgDS;
//}
//
////------------------------------------------------------------------------------
//CDSSchemaChgDscr* ZS::Data::CMsgConSelectSchema::getSchema()
////------------------------------------------------------------------------------
//{
//    CDSSchemaChgDscr* pSchema = nullptr;
//    //m_pChgDS->takeSchemaChanges();
//    return pSchema;
//}
//
////------------------------------------------------------------------------------
//CDSSchemaChgDscr* ZS::Data::CMsgConSelectSchema::takeSchema()
////------------------------------------------------------------------------------
//{
//    CDSSchemaChgDscr* pSchema = nullptr;
//    //m_pChgDS->takeSchemaChanges();
//    return pSchema;
//}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConSelectSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConSelectSchema* pMsgCloned = new ZS::Data::CMsgConSelectSchema(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* cnctId             */ m_cnctId,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    //if( m_pChgDS != nullptr )
    //{
    //    if( i_copyDepth == ECopyDepth::FlatReleaseOwnwership )
    //    {
    //        pMsgCloned->setSchema(m_pChgDS);
    //        m_pChgDS = nullptr;
    //    }
    //    else if( i_copyDepth == ECopyDepth::Deep )
    //    {
    //        CChangeDataSet* pDSCloned = dynamic_cast<CChangeDataSet*>(m_pChgDS->clone(ECloneCopyDepthSchemaAll));
    //        pMsgCloned->setSchema(pDSCloned);
    //        pDSCloned = nullptr;
    //    }
    //}

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConSelectSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConSelectSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgCon::getAddTrcInfoStr();

    //if( m_pChgDS == nullptr )
    //{
    //    strAddTrcInfo += ", ChgEvs { nullptr }";
    //}
    //else
    //{
    //    strAddTrcInfo += ", ChgEvs { " + m_pChgDS->getAddTrcInfo(i_iDetailLevel) + " }";
    //}
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgConSelectSchema::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArrDataMsgHdr = ZS::Data::CMsgCon::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrDataMsgHdr.data());

    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + pSysMsgHdr->m_blockLenBuffer;

    char* pcSysDataBlock = reinterpret_cast<char*>(pSysMsgHdr) + iSysMsgHeaderSize;

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    // Create Data Block of ZS::Data Message Header containing NodeSpecs.
    //-------------------------------------------------------------------

    QByteArray byteArrDS;

    //if( m_pChgDS != nullptr )
    //{
    //    m_pChgDS->serialize(byteArrDS);
    //}

    // Update block lengths in system and data message headers.
    //---------------------------------------------------------

    pDataMsgHdr->m_blockLenData = byteArrDS.size();

    pSysMsgHdr->m_blockLenData = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer + pDataMsgHdr->m_blockLenData;

    return byteArrDataMsgHdr + byteArrDS;

} // serialize


/*******************************************************************************
class CMsgReqSelectData : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSelectData::CMsgReqSelectData(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const SCnctId&  i_cnctId,
    qint64          i_iReqId,
    qint64          i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(
        /* iMsgType         */ EMsgTypeReqSelectData,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* cnctId           */ i_cnctId,
        /* bMustBeConfirmed */ true,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_dbCltRegSpec(i_cnctId)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSelectData::CMsgReqSelectData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(i_pMsgHdr),
    m_dbCltRegSpec()
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    if( pDataMsgHdr->m_blockLenData > 0 )
    {
        int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer;

        const char* pcDSDataBlock = reinterpret_cast<const char*>(pDataMsgHdr) + iDataMsgHeaderSize;

        QByteArray byteArrRegSpec( pcDSDataBlock, pDataMsgHdr->m_blockLenData );

        m_dbCltRegSpec.deserialize(byteArrRegSpec);

    } // if( pDataMsgHdr->m_blockLenData > 0 )

} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqSelectData::~CMsgReqSelectData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class ZS::Data::CMsgReq
==============================================================================*/

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectData::setCnctId( const SCnctId& i_cnctId )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReq::setCnctId(i_cnctId);

    m_dbCltRegSpec.m_cnctId = i_cnctId;

} // setCnctId

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectData::setClientName( const QString& i_strName )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_strClientName = i_strName;
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectData::setDSNodeSpec( const SDSNodeSpec i_dsNodeSpec )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_arDSNodeSpecs.clear();
    m_dbCltRegSpec.m_arDSNodeSpecs.append(i_dsNodeSpec);
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectData::setDSNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_arDSNodeSpecs = i_arDSNodeSpecs;
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectData::addDSNodeSpec( const SDSNodeSpec& i_dsNodeSpec )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_arDSNodeSpecs.append(i_dsNodeSpec);
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectData::addDSNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec.m_arDSNodeSpecs += i_arDSNodeSpecs;
}

//------------------------------------------------------------------------------
void ZS::Data::CMsgReqSelectData::setRegSpec( const SDbClientRegSpec& i_dbCltRegSpec )
//------------------------------------------------------------------------------
{
    m_dbCltRegSpec = i_dbCltRegSpec;
}

//------------------------------------------------------------------------------
SDSNodeSpec ZS::Data::CMsgReqSelectData::getDSNodeSpec( int i_idx ) const
//------------------------------------------------------------------------------
{
    SDSNodeSpec dsNodeSpec;

    if( i_idx >= 0 && i_idx < m_dbCltRegSpec.m_arDSNodeSpecs.size() )
    {
        dsNodeSpec = m_dbCltRegSpec.m_arDSNodeSpecs[i_idx];
    }
    return dsNodeSpec;

} // getDSNodeSpec

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqSelectData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqSelectData* pMsgCloned = new ZS::Data::CMsgReqSelectData(
        /* pObjSender   */ m_pObjSender,
        /* pObjReceiver */ m_pObjReceiver,
        /* cnctId       */ m_cnctId,
        /* iReqId       */ m_iReqId,
        /* iMsgId       */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);
    pMsgCloned->setRegSpec(m_dbCltRegSpec);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqSelectData::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConSelectData* pMsgCon = new ZS::Data::CMsgConSelectData(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* cnctId            */ m_cnctId,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqSelectData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqSelectData::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgReq::getAddTrcInfoStr();
    strAddTrcInfo += ", ReqSpec {" + m_dbCltRegSpec.toString(i_iDetailLevel) + "}";
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgReqSelectData::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArrDataMsgHdr = ZS::Data::CMsgReq::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrDataMsgHdr.data());

    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + pSysMsgHdr->m_blockLenBuffer;

    char* pcSysDataBlock = reinterpret_cast<char*>(pSysMsgHdr) + iSysMsgHeaderSize;

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    // Create Data Block of ZS::Data Message Header containing NodeSpecs.
    //-------------------------------------------------------------------

    QByteArray byteArrRegSpec;

    m_dbCltRegSpec.serialize(byteArrRegSpec);

    // Update block lengths in system and data message headers.
    //---------------------------------------------------------

    pDataMsgHdr->m_blockLenData = byteArrRegSpec.size();

    pSysMsgHdr->m_blockLenData = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer + pDataMsgHdr->m_blockLenData;

    return byteArrDataMsgHdr + byteArrRegSpec;

} // serialize


/*******************************************************************************
class CMsgConSelectData : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConSelectData::CMsgConSelectData(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(
        /* iMsgType           */ EMsgTypeConSelectData,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* cnctId             */ i_cnctId,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
    //m_pChgDS(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConSelectData::CMsgConSelectData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(i_pMsgHdr)
    //m_pChgDS(nullptr)
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    if( pDataMsgHdr->m_blockLenData > 0 )
    {
        int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer;

        const char* pcDSDataBlock = reinterpret_cast<const char*>(pDataMsgHdr) + iDataMsgHeaderSize;

        QByteArray byteArrDS(pDataMsgHdr->m_blockLenData, 0x00);

        memcpy(byteArrDS.data(), pcDSDataBlock, pDataMsgHdr->m_blockLenData);

        // Message sent from database to client deserialized on client site.
        //m_pChgDS = new CChangeDataSet("MsgConSelectSchema", ERowVersion::Original);

        //m_pChgDS->deserialize(byteArrDS);

    } // if( pDataMsgHdr->m_blockLenData > 0 )

} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConSelectData::~CMsgConSelectData()
//------------------------------------------------------------------------------
{
    //try
    //{
    //    delete m_pChgDS;
    //}
    //catch(...)
    //{
    //}
    //m_pChgDS = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void ZS::Data::CMsgConSelectData::mergeData( CChangeDataSet* i_pChgDS )
////------------------------------------------------------------------------------
//{
//    if( m_pChgDS != nullptr )
//    {
//        m_pChgDS->mergeData(i_pChgDS);
//    }
//    else
//    {
//        m_pChgDS = i_pChgDS;
//    }
//
//} // mergeData

////------------------------------------------------------------------------------
//void ZS::Data::CMsgConSelectData::setData( CChangeDataSet* i_pChgDS )
////------------------------------------------------------------------------------
//{
//    delete m_pChgDS;
//    m_pChgDS = i_pChgDS;
//}
//
////------------------------------------------------------------------------------
//CChangeDataSet* ZS::Data::CMsgConSelectData::takeData()
////------------------------------------------------------------------------------
//{
//    CChangeDataSet* pDS = m_pChgDS;
//    m_pChgDS = nullptr;
//    return pDS;
//}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConSelectData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConSelectData* pMsgCloned = new ZS::Data::CMsgConSelectData(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* cnctId             */ m_cnctId,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    //if( m_pChgDS != nullptr )
    //{
    //    if( i_copyDepth == ECopyDepth::FlatReleaseOwnwership )
    //    {
    //        pMsgCloned->setData(m_pChgDS);
    //        m_pChgDS = nullptr;
    //    }
    //    else if( i_copyDepth == ECopyDepth::Deep )
    //    {
    //        CChangeDataSet* pDSCloned = dynamic_cast<CChangeDataSet*>(m_pChgDS->clone(ECloneCopyDepthDataAll));
    //        pMsgCloned->setData(pDSCloned);
    //        pDSCloned = nullptr;
    //    }
    //}

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConSelectData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConSelectData::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgCon::getAddTrcInfoStr();

    //if( m_pChgDS == nullptr )
    //{
    //    strAddTrcInfo += ", ChgEvs { nullptr }";
    //}
    //else
    //{
    //    strAddTrcInfo += ", ChgEvs { " + m_pChgDS->getAddTrcInfo(i_iDetailLevel) + " }";
    //}
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgConSelectData::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArrDataMsgHdr = ZS::Data::CMsgCon::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrDataMsgHdr.data());

    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + pSysMsgHdr->m_blockLenBuffer;

    char* pcSysDataBlock = reinterpret_cast<char*>(pSysMsgHdr) + iSysMsgHeaderSize;

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    // Create Data Block of ZS::Data Message Header containing NodeSpecs.
    //-------------------------------------------------------------------

    QByteArray byteArrDS;

    //if( m_pChgDS != nullptr )
    //{
    //    m_pChgDS->serialize(byteArrDS);
    //}

    // Update block lengths in system and data message headers.
    //---------------------------------------------------------

    pDataMsgHdr->m_blockLenData = byteArrDS.size();

    pSysMsgHdr->m_blockLenData = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer + pDataMsgHdr->m_blockLenData;

    return byteArrDataMsgHdr + byteArrDS;

} // serialize


/*******************************************************************************
With Data Sets
*******************************************************************************/

/*******************************************************************************
class CMsgReqUpdateSchema : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqUpdateSchema::CMsgReqUpdateSchema(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const SCnctId&  i_cnctId,
    bool            i_bMustBeConfirmed,
    qint64          i_iReqId,
    qint64          i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(
        /* iMsgType         */ EMsgTypeReqUpdateSchema,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* cnctId           */ i_cnctId,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
    //m_pChgDS(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqUpdateSchema::CMsgReqUpdateSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(i_pMsgHdr)
    //m_pChgDS(nullptr)
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    if( pDataMsgHdr->m_blockLenData > 0 )
    {
        int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer;

        const char* pcDSDataBlock = reinterpret_cast<const char*>(pDataMsgHdr) + iDataMsgHeaderSize;

        QByteArray byteArrDS(pDataMsgHdr->m_blockLenData, 0x00);

        memcpy(byteArrDS.data(), pcDSDataBlock, pDataMsgHdr->m_blockLenData);

        //m_pChgDS = new CChangeDataSet("MsgReqUpdateSchema", ERowVersion::Undefined);

        //m_pChgDS->deserialize(byteArrDS);

    } // if( pDataMsgHdr->m_blockLenData > 0 )

} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqUpdateSchema::~CMsgReqUpdateSchema()
//------------------------------------------------------------------------------
{
    //try
    //{
    //    delete m_pChgDS;
    //}
    //catch(...)
    //{
    //}
    //m_pChgDS = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void ZS::Data::CMsgReqUpdateSchema::mergeSchema( CChangeDataSet* i_pChgDS )
////------------------------------------------------------------------------------
//{
//    if( m_pChgDS != nullptr )
//    {
//        m_pChgDS->mergeSchema(i_pChgDS);
//    }
//    else
//    {
//        m_pChgDS = i_pChgDS;
//    }
//
//} // mergeSchema

////------------------------------------------------------------------------------
//void ZS::Data::CMsgReqUpdateSchema::setSchema( CChangeDataSet* i_pChgDS )
////------------------------------------------------------------------------------
//{
//    delete m_pChgDS;
//    m_pChgDS = i_pChgDS;
//}
//
////------------------------------------------------------------------------------
//CDSSchemaChgDscr* ZS::Data::CMsgReqUpdateSchema::getSchema()
////------------------------------------------------------------------------------
//{
//    CDSSchemaChgDscr* pSchema = nullptr;
//    //m_pChgDS->takeSchemaChanges();
//    return pSchema;
//}
//
////------------------------------------------------------------------------------
//CDSSchemaChgDscr* ZS::Data::CMsgReqUpdateSchema::takeSchema()
////------------------------------------------------------------------------------
//{
//    CDSSchemaChgDscr* pSchema = nullptr;
//    //m_pChgDS->takeSchemaChanges();
//    return pSchema;
//}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqUpdateSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqUpdateSchema* pMsgCloned = new ZS::Data::CMsgReqUpdateSchema(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* cnctId           */ m_cnctId,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    //if( m_pChgDS != nullptr )
    //{
    //    if( i_copyDepth == ECopyDepth::FlatReleaseOwnwership )
    //    {
    //        pMsgCloned->setSchema(m_pChgDS);
    //        m_pChgDS = nullptr;
    //    }
    //    else if( i_copyDepth == ECopyDepth::Deep )
    //    {
    //        CChangeDataSet* pDSCloned = dynamic_cast<CChangeDataSet*>(m_pChgDS->clone(ECloneCopyDepthSchemaAll));
    //        pMsgCloned->setSchema(pDSCloned);
    //        pDSCloned = nullptr;
    //    }
    //}

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqUpdateSchema::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConUpdateSchema* pMsgCon = new ZS::Data::CMsgConUpdateSchema(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* cnctId            */ m_cnctId,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    //if( m_pChgDS != nullptr )
    //{
    //    if( i_copyDepth == ECopyDepth::FlatKeepOwnership )
    //    {
    //    }
    //    else if( i_copyDepth == ECopyDepth::FlatReleaseOwnwership )
    //    {
    //        pMsgCon->setSchema(m_pChgDS);
    //        m_pChgDS = nullptr;
    //    }
    //    else if( i_copyDepth == ECopyDepth::Deep )
    //    {
    //        CChangeDataSet* pDSCloned = dynamic_cast<CChangeDataSet*>(m_pChgDS->clone(ECloneCopyDepthSchemaAll));
    //        pMsgCon->setSchema(pDSCloned);
    //        pDSCloned = nullptr;
    //    }
    //} // if( m_pChgDS != nullptr )

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqUpdateSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqUpdateSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgReq::getAddTrcInfoStr();

    //if( m_pChgDS == nullptr )
    //{
    //    strAddTrcInfo += ", ChgEvs { nullptr }";
    //}
    //else
    //{
    //    strAddTrcInfo += ", ChgEvs { " + m_pChgDS->getAddTrcInfo(i_iDetailLevel) + " }";
    //}
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgReqUpdateSchema::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArrDataMsgHdr = ZS::Data::CMsgReq::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrDataMsgHdr.data());

    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + pSysMsgHdr->m_blockLenBuffer;

    char* pcSysDataBlock = reinterpret_cast<char*>(pSysMsgHdr) + iSysMsgHeaderSize;

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    // Create Data Block of ZS::Data Message Header containing NodeSpecs.
    //-------------------------------------------------------------------

    QByteArray byteArrDS;

    //if( m_pChgDS != nullptr )
    //{
    //    m_pChgDS->serialize(byteArrDS);
    //}

    // Update block lengths in system and data message headers.
    //---------------------------------------------------------

    pDataMsgHdr->m_blockLenData = byteArrDS.size();

    pSysMsgHdr->m_blockLenData = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer + pDataMsgHdr->m_blockLenData;

    return byteArrDataMsgHdr + byteArrDS;

} // serialize


/*******************************************************************************
class CMsgConUpdateSchema : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConUpdateSchema::CMsgConUpdateSchema(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(
        /* iMsgType           */ EMsgTypeConUpdateSchema,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* cnctId             */ i_cnctId,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
    //m_pChgDS(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConUpdateSchema::CMsgConUpdateSchema( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(i_pMsgHdr)
    //m_pChgDS(nullptr)
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    if( pDataMsgHdr->m_blockLenData > 0 )
    {
        int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer;

        const char* pcDSDataBlock = reinterpret_cast<const char*>(pDataMsgHdr) + iDataMsgHeaderSize;

        QByteArray byteArrDS(pDataMsgHdr->m_blockLenData, 0x00);

        memcpy(byteArrDS.data(), pcDSDataBlock, pDataMsgHdr->m_blockLenData);

        //m_pChgDS = new CChangeDataSet("MsgConUpdateSchema", ERowVersion::Undefined);

        //m_pChgDS->deserialize(byteArrDS);

    } // if( pDataMsgHdr->m_blockLenData > 0 )

} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConUpdateSchema::~CMsgConUpdateSchema()
//------------------------------------------------------------------------------
{
    //try
    //{
    //    delete m_pChgDS;
    //}
    //catch(...)
    //{
    //}
    //m_pChgDS = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void ZS::Data::CMsgConUpdateSchema::mergeSchema( CChangeDataSet* i_pChgDS )
////------------------------------------------------------------------------------
//{
//    if( m_pChgDS != nullptr )
//    {
//        m_pChgDS->mergeSchema(i_pChgDS);
//    }
//    else
//    {
//        m_pChgDS = i_pChgDS;
//    }
//
//} // mergeSchema

////------------------------------------------------------------------------------
//void ZS::Data::CMsgConUpdateSchema::setSchema( CChangeDataSet* i_pChgDS )
////------------------------------------------------------------------------------
//{
//    delete m_pChgDS;
//    m_pChgDS = i_pChgDS;
//}
//
////------------------------------------------------------------------------------
//CDSSchemaChgDscr* ZS::Data::CMsgConUpdateSchema::getSchema()
////------------------------------------------------------------------------------
//{
//    CDSSchemaChgDscr* pSchema = nullptr;
//    //m_pChgDS->takeSchemaChanges();
//    return pSchema;
//}
//
////------------------------------------------------------------------------------
//CDSSchemaChgDscr* ZS::Data::CMsgConUpdateSchema::takeSchema()
////------------------------------------------------------------------------------
//{
//    CDSSchemaChgDscr* pSchema = nullptr;
//    //m_pChgDS->takeSchemaChanges();
//    return pSchema;
//}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConUpdateSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConUpdateSchema* pMsgCloned = new ZS::Data::CMsgConUpdateSchema(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* cnctId             */ m_cnctId,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    //if( m_pChgDS != nullptr )
    //{
    //    if( i_copyDepth == ECopyDepth::FlatReleaseOwnwership )
    //    {
    //        pMsgCloned->setSchema(m_pChgDS);
    //        m_pChgDS = nullptr;
    //    }
    //    else if( i_copyDepth == ECopyDepth::Deep )
    //    {
    //        CChangeDataSet* pDSCloned = dynamic_cast<CChangeDataSet*>(m_pChgDS->clone(ECloneCopyDepthSchemaAll));
    //        pMsgCloned->setSchema(pDSCloned);
    //        pDSCloned = nullptr;
    //    }
    //}

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConUpdateSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConUpdateSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgCon::getAddTrcInfoStr();

    //if( m_pChgDS == nullptr )
    //{
    //    strAddTrcInfo += ", ChgEvs { nullptr }";
    //}
    //else
    //{
    //    strAddTrcInfo += ", ChgEvs { " + m_pChgDS->getAddTrcInfo(i_iDetailLevel) + " }";
    //}
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgConUpdateSchema::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArrDataMsgHdr = ZS::Data::CMsgCon::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrDataMsgHdr.data());

    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + pSysMsgHdr->m_blockLenBuffer;

    char* pcSysDataBlock = reinterpret_cast<char*>(pSysMsgHdr) + iSysMsgHeaderSize;

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    // Create Data Block of ZS::Data Message Header containing NodeSpecs.
    //-------------------------------------------------------------------

    QByteArray byteArrDS;

    //if( m_pChgDS != nullptr )
    //{
    //    m_pChgDS->serialize(byteArrDS);
    //}

    // Update block lengths in system and data message headers.
    //---------------------------------------------------------

    pDataMsgHdr->m_blockLenData = byteArrDS.size();

    pSysMsgHdr->m_blockLenData = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer + pDataMsgHdr->m_blockLenData;

    return byteArrDataMsgHdr + byteArrDS;

} // serialize


/*******************************************************************************
class CMsgReqUpdateData : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgReqUpdateData::CMsgReqUpdateData(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const SCnctId&  i_cnctId,
    bool            i_bMustBeConfirmed,
    qint64          i_iReqId,
    qint64          i_iMsgId ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(
        /* iMsgType         */ EMsgTypeReqUpdateData,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* cnctId           */ i_cnctId,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
    //m_pChgDS(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqUpdateData::CMsgReqUpdateData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgReq(i_pMsgHdr)
    //m_pChgDS(nullptr)
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    if( pDataMsgHdr->m_blockLenData > 0 )
    {
        int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer;

        const char* pcDSDataBlock = reinterpret_cast<const char*>(pDataMsgHdr) + iDataMsgHeaderSize;

        QByteArray byteArrDS(pDataMsgHdr->m_blockLenData, 0x00);

        memcpy(byteArrDS.data(), pcDSDataBlock, pDataMsgHdr->m_blockLenData);

        //m_pChgDS = new CChangeDataSet("MsgReqUpdateData", ERowVersion::Undefined);

        //m_pChgDS->deserialize(byteArrDS);

    } // if( pDataMsgHdr->m_blockLenData > 0 )

} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgReqUpdateData::~CMsgReqUpdateData()
//------------------------------------------------------------------------------
{
    //try
    //{
    //    delete m_pChgDS;
    //}
    //catch(...)
    //{
    //}
    //m_pChgDS = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void ZS::Data::CMsgReqUpdateData::mergeData( CChangeDataSet* i_pChgDS )
////------------------------------------------------------------------------------
//{
//    if( m_pChgDS != nullptr )
//    {
//        m_pChgDS->mergeData(i_pChgDS);
//    }
//    else
//    {
//        m_pChgDS = i_pChgDS;
//    }
//
//} // mergeData

////------------------------------------------------------------------------------
//void ZS::Data::CMsgReqUpdateData::setData( CChangeDataSet* i_pChgDS )
////------------------------------------------------------------------------------
//{
//    delete m_pChgDS;
//    m_pChgDS = i_pChgDS;
//}
//
////------------------------------------------------------------------------------
//CChangeDataSet* ZS::Data::CMsgReqUpdateData::takeData()
////------------------------------------------------------------------------------
//{
//    CChangeDataSet* pDS = m_pChgDS;
//    m_pChgDS = nullptr;
//    return pDS;
//}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgReqUpdateData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgReqUpdateData* pMsgCloned = new ZS::Data::CMsgReqUpdateData(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* cnctId           */ m_cnctId,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    //if( m_pChgDS != nullptr )
    //{
    //    if( i_copyDepth == ECopyDepth::FlatReleaseOwnwership )
    //    {
    //        pMsgCloned->setData(m_pChgDS);
    //        m_pChgDS = nullptr;
    //    }
    //    else if( i_copyDepth == ECopyDepth::Deep )
    //    {
    //        //CChangeDataSet* pDSCloned = dynamic_cast<CChangeDataSet*>(m_pChgDS->clone(ECloneCopyDepthDataAll));
    //        //pMsgCloned->setData(pDSCloned);
    //        //pDSCloned = nullptr;
    //    }
    //}

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
ZS::System::CMsgCon* ZS::Data::CMsgReqUpdateData::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    ZS::Data::CMsgConUpdateData* pMsgCon = new ZS::Data::CMsgConUpdateData(
        /* pObjSender        */ m_pObjReceiver,
        /* pObjReceiver      */ m_pObjSender,
        /* cnctId            */ m_cnctId,
        /* iReqId            */ m_iReqId,
        /* iMsgIdReq         */ m_iMsgId,
        /* errResultInfo     */ SErrResultInfo(),
        /* iProgress_perCent */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    //if( m_pChgDS != nullptr )
    //{
    //    if( i_copyDepth == ECopyDepth::FlatKeepOwnership )
    //    {
    //    }
    //    else if( i_copyDepth == ECopyDepth::FlatReleaseOwnwership )
    //    {
    //        pMsgCon->setData(m_pChgDS);
    //        m_pChgDS = nullptr;
    //    }
    //    else if( i_copyDepth == ECopyDepth::Deep )
    //    {
    //        //CChangeDataSet* pDSCloned = dynamic_cast<CChangeDataSet*>(m_pChgDS->clone(ECloneCopyDepthDataAll));
    //        //pMsgCon->setData(pDSCloned);
    //        //pDSCloned = nullptr;
    //    }
    //} // if( m_pChgDS != nullptr )

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqUpdateData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgReqUpdateData::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgReq::getAddTrcInfoStr();

    //if( m_pChgDS == nullptr )
    //{
    //    strAddTrcInfo += ", ChgEvs { nullptr }";
    //}
    //else
    //{
    //    strAddTrcInfo += ", ChgEvs { " + m_pChgDS->getAddTrcInfo(i_iDetailLevel) + " }";
    //}
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgReqUpdateData::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArrDataMsgHdr = ZS::Data::CMsgReq::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrDataMsgHdr.data());

    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + pSysMsgHdr->m_blockLenBuffer;

    char* pcSysDataBlock = reinterpret_cast<char*>(pSysMsgHdr) + iSysMsgHeaderSize;

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    // Create Data Block of ZS::Data Message Header containing NodeSpecs.
    //-------------------------------------------------------------------

    QByteArray byteArrDS;

    //if( m_pChgDS != nullptr )
    //{
    //    m_pChgDS->serialize(byteArrDS);
    //}

    // Update block lengths in system and data message headers.
    //---------------------------------------------------------

    pDataMsgHdr->m_blockLenData = byteArrDS.size();

    pSysMsgHdr->m_blockLenData = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer + pDataMsgHdr->m_blockLenData;

    return byteArrDataMsgHdr + byteArrDS;

} // serialize


/*******************************************************************************
class CMsgConUpdateData : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Data::CMsgConUpdateData::CMsgConUpdateData(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SCnctId&        i_cnctId,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(
        /* iMsgType           */ EMsgTypeConUpdateData,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* cnctId             */ i_cnctId,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
    //m_pChgDS(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConUpdateData::CMsgConUpdateData( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    ZS::Data::CMsgCon(i_pMsgHdr)
    //m_pChgDS(nullptr)
{
    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

    const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

    const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    if( pDataMsgHdr->m_blockLenData > 0 )
    {
        int iDataMsgHeaderSize = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer;

        const char* pcDSDataBlock = reinterpret_cast<const char*>(pDataMsgHdr) + iDataMsgHeaderSize;

        QByteArray byteArrDS(pDataMsgHdr->m_blockLenData, 0x00);

        memcpy(byteArrDS.data(), pcDSDataBlock, pDataMsgHdr->m_blockLenData);

        //m_pChgDS = new CChangeDataSet("MsgConUpdateData", ERowVersion::Undefined);

        //m_pChgDS->deserialize(byteArrDS);

    } // if( pDataMsgHdr->m_blockLenData > 0 )

} // ctor

//------------------------------------------------------------------------------
ZS::Data::CMsgConUpdateData::~CMsgConUpdateData()
//------------------------------------------------------------------------------
{
    //try
    //{
    //    delete m_pChgDS;
    //}
    //catch(...)
    //{
    //}
    //m_pChgDS = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void ZS::Data::CMsgConUpdateData::mergeData( CChangeDataSet* i_pChgDS )
////------------------------------------------------------------------------------
//{
//    if( m_pChgDS != nullptr )
//    {
//        m_pChgDS->mergeData(i_pChgDS);
//    }
//    else
//    {
//        m_pChgDS = i_pChgDS;
//    }
//
//} // mergeData

////------------------------------------------------------------------------------
//void ZS::Data::CMsgConUpdateData::setData( CChangeDataSet* i_pChgDS )
////------------------------------------------------------------------------------
//{
//    delete m_pChgDS;
//    m_pChgDS = i_pChgDS;
//}
//
////------------------------------------------------------------------------------
//CChangeDataSet* ZS::Data::CMsgConUpdateData::takeData()
////------------------------------------------------------------------------------
//{
//    CChangeDataSet* pDS = m_pChgDS;
//    m_pChgDS = nullptr;
//    return pDS;
//}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* ZS::Data::CMsgConUpdateData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    ZS::Data::CMsgConUpdateData* pMsgCloned = new ZS::Data::CMsgConUpdateData(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* cnctId             */ m_cnctId,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    //if( m_pChgDS != nullptr )
    //{
    //    if( i_copyDepth == ECopyDepth::FlatReleaseOwnwership )
    //    {
    //        pMsgCloned->setData(m_pChgDS);
    //        m_pChgDS = nullptr;
    //    }
    //    else if( i_copyDepth == ECopyDepth::Deep )
    //    {
    //        //CChangeDataSet* pDSCloned = dynamic_cast<CChangeDataSet*>(m_pChgDS->clone(ECloneCopyDepthDataAll));
    //        //pMsgCloned->setData(pDSCloned);
    //        //pDSCloned = nullptr;
    //    }
    //}

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConUpdateData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return ZS::Data::msgType2Str(type());
}

//------------------------------------------------------------------------------
QString ZS::Data::CMsgConUpdateData::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = msgTypeToStr();

    strAddTrcInfo += " {";
    strAddTrcInfo += ZS::Data::CMsgCon::getAddTrcInfoStr();

    //if( m_pChgDS == nullptr )
    //{
    //    strAddTrcInfo += ", ChgEvs { nullptr }";
    //}
    //else
    //{
    //    strAddTrcInfo += ", ChgEvs { " + m_pChgDS->getAddTrcInfo(i_iDetailLevel) + " }";
    //}
    strAddTrcInfo += "}";

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray ZS::Data::CMsgConUpdateData::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArrDataMsgHdr = ZS::Data::CMsgCon::serialize();

    ZS::System::MsgProtocol::SMsgHeader* pSysMsgHdr = reinterpret_cast<ZS::System::MsgProtocol::SMsgHeader*>(byteArrDataMsgHdr.data());

    int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + pSysMsgHdr->m_blockLenBuffer;

    char* pcSysDataBlock = reinterpret_cast<char*>(pSysMsgHdr) + iSysMsgHeaderSize;

    ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

    // Create Data Block of ZS::Data Message Header containing NodeSpecs.
    //-------------------------------------------------------------------

    QByteArray byteArrDS;

    //if( m_pChgDS != nullptr )
    //{
    //    m_pChgDS->serialize(byteArrDS);
    //}

    // Update block lengths in system and data message headers.
    //---------------------------------------------------------

    pDataMsgHdr->m_blockLenData = byteArrDS.size();

    pSysMsgHdr->m_blockLenData = sizeof(ZS::Data::MsgProtocol::SMsgHeader) + pDataMsgHdr->m_blockLenBuffer + pDataMsgHdr->m_blockLenData;

    return byteArrDataMsgHdr + byteArrDS;

} // serialize
