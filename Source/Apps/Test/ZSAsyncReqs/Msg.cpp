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

#include "Msg.h"

#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Apps::Test::AsyncRequests;


/*******************************************************************************
Type definitions and constants
*******************************************************************************/

/*==============================================================================
enum EMsgType
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrMsgTypes[EMsgTypeCount] =
//------------------------------------------------------------------------------
{
    /*  0 */ SEnumEntry( EMsgTypeReqReceiveData,    "ReqReceivedData"   ),
    /*  1 */ SEnumEntry( EMsgTypeReqStartup,        "ReqStartup"        ),
    /*  2 */ SEnumEntry( EMsgTypeConStartup,        "ConStartup"        ),
    /*  3 */ SEnumEntry( EMsgTypeReqShutdown,       "ReqShutdown"       ),
    /*  4 */ SEnumEntry( EMsgTypeConShutdown,       "ConShutdown"       ),
    /*  5 */ SEnumEntry( EMsgTypeReqChangeSettings, "ReqChangeSettings" ),
    /*  6 */ SEnumEntry( EMsgTypeConChangeSettings, "ConChangeSettings" ),
    /*  7 */ SEnumEntry( EMsgTypeReqConnect,        "ReqConnect"        ),
    /*  8 */ SEnumEntry( EMsgTypeConConnect,        "ConConnect"        ),
    /*  9 */ SEnumEntry( EMsgTypeIndConnected,      "IndConnected"      ),
    /* 10 */ SEnumEntry( EMsgTypeReqDisconnect,     "ReqDisconnect"     ),
    /* 11 */ SEnumEntry( EMsgTypeConDisconnect,     "ConDisconnect"     ),
    /* 12 */ SEnumEntry( EMsgTypeIndDisconnected,   "IndDisconnected"   ),
    /* 13 */ SEnumEntry( EMsgTypeReqRegister,       "ReqRegister"       ),
    /* 14 */ SEnumEntry( EMsgTypeConRegister,       "ConRegister"       ),
    /* 15 */ SEnumEntry( EMsgTypeReqUnregister,     "ReqUnregister"     ),
    /* 16 */ SEnumEntry( EMsgTypeConUnregister,     "ConUnregister"     ),
    /* 17 */ SEnumEntry( EMsgTypeReqReadSchema,     "ReqReadSchema"     ),
    /* 18 */ SEnumEntry( EMsgTypeConReadSchema,     "ConReadSchema"     ),
    /* 19 */ SEnumEntry( EMsgTypeReqSaveSchema,     "ReqSaveSchema"     ),
    /* 20 */ SEnumEntry( EMsgTypeConSaveSchema,     "ConSaveSchema"     ),
    /* 21 */ SEnumEntry( EMsgTypeReqSelectSchema,   "ReqSelectSchema"   ),
    /* 22 */ SEnumEntry( EMsgTypeConSelectSchema,   "ConSelectSchema"   ),
    /* 23 */ SEnumEntry( EMsgTypeReqUpdateSchema,   "ReqUpdateSchema"   ),
    /* 24 */ SEnumEntry( EMsgTypeConUpdateSchema,   "ConUpdateSchema"   ),
    /* 25 */ SEnumEntry( EMsgTypeReqReadData,       "ReqReadData"       ),
    /* 26 */ SEnumEntry( EMsgTypeConReadData,       "ConReadData"       ),
    /* 27 */ SEnumEntry( EMsgTypeReqSaveData,       "ReqSaveData"       ),
    /* 28 */ SEnumEntry( EMsgTypeConSaveData,       "ConSaveData"       ),
    /* 29 */ SEnumEntry( EMsgTypeReqSelectData,     "ReqSelectData"     ),
    /* 30 */ SEnumEntry( EMsgTypeConSelectData,     "ConSelectData"     ),
    /* 31 */ SEnumEntry( EMsgTypeReqUpdateData,     "ReqUpdateData"     ),
    /* 32 */ SEnumEntry( EMsgTypeConUpdateData,     "ConUpdateData"     ),
    /* 33 */ SEnumEntry( EMsgTypeReqSet2Default,    "ReqSet2Default"    ),
    /* 34 */ SEnumEntry( EMsgTypeConSet2Default,    "ConSet2Default"    )
};

//------------------------------------------------------------------------------
QString ZS::Apps::Test::AsyncRequests::msgType2Str( int i_iMsgType )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str( s_arEnumStrMsgTypes, EMsgTypeCount, i_iMsgType );
}


/*******************************************************************************
class CMsgClassFactory
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CMsgClassFactory::CheckMsgHeader( ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr )
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo("ZS::Apps::Test::AsyncRequest", "CMsgClassFactory", "theInst", "checkMsgHeader");

    QString strAddErrInfo;

    if( i_pMsgHdr->m_protocolVersion != ZS::System::MsgProtocol::VERSION )
    {
        strAddErrInfo += "Protocol version received '0x" + QString::number(i_pMsgHdr->m_protocolVersion,16) + "' but current version is '0x" + QString::number(MsgProtocol::VERSION,16) + "'";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidMessageContent);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }

    return errResultInfo;

} // CheckMsgHeader

//------------------------------------------------------------------------------
CMsg* CMsgClassFactory::CreateInstance( const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    CMsg* pMsg = nullptr;

    SErrResultInfo errResultInfo("ZS::Apps::Test::AsyncRequest", "CMsgClassFactory", "theInst", "CreateInstance");
    QString        strAddErrInfo;

    const MsgProtocol::SMsgHeader* pMsgHdrCTmp = reinterpret_cast<const MsgProtocol::SMsgHeader*>(i_byteArr.data());
    MsgProtocol::SMsgHeader* pMsgHdr = const_cast<MsgProtocol::SMsgHeader*>(pMsgHdrCTmp);
    //char* pcMsg = reinterpret_cast<char*>(pMsgHdr);

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
    iMsgProtSize += pMsgHdr->m_blockLenBuffer;

    // System:MsgHeader:Data Block
    iMsgProtSize += pMsgHdr->m_blockLenData;

    if( i_byteArr.size() != iMsgProtSize )
    {
        strAddErrInfo += "Received " + QString::number(i_byteArr.size()) + " bytes but " + QString::number(iMsgProtSize) + " bytes are specified in message header.";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidMessageContent);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }
    else // if( i_byteArr.size() == iMsgProtSize )
    {
        errResultInfo = checkMsgHeader(pMsgHdr);

        if( !errResultInfo.isErrorResult() )
        {
            switch( pMsgHdr->m_msgType )
            {
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
                    strAddErrInfo += "Message type " + msgType2Str(pMsgHdr->m_msgType) + " not supported";
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInvalidMessageContent);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                    break;
                }
                case EMsgTypeReqRegister:
                {
                    pMsg = new CMsgReqRegister(pMsgHdr);
                    break;
                }
                case EMsgTypeConRegister:
                {
                    pMsg = new CMsgConRegister(pMsgHdr);
                    break;
                }
                case EMsgTypeReqUnregister:
                {
                    pMsg = new CMsgReqUnregister(pMsgHdr);
                    break;
                }
                case EMsgTypeConUnregister:
                {
                    pMsg = new CMsgConUnregister(pMsgHdr);
                    break;
                }
                case EMsgTypeReqReadSchema:
                {
                    pMsg = new CMsgReqReadSchema(pMsgHdr);
                    break;
                }
                case EMsgTypeConReadSchema:
                {
                    pMsg = new CMsgConReadSchema(pMsgHdr);
                    break;
                }
                case EMsgTypeReqSaveSchema:
                {
                    pMsg = new CMsgReqSaveSchema(pMsgHdr);
                    break;
                }
                case EMsgTypeConSaveSchema:
                {
                    pMsg = new CMsgConSaveSchema(pMsgHdr);
                    break;
                }
                case EMsgTypeReqSelectSchema:
                {
                    pMsg = new CMsgReqSelectSchema(pMsgHdr);
                    break;
                }
                case EMsgTypeConSelectSchema:
                {
                    pMsg = new CMsgConSelectSchema(pMsgHdr);
                    break;
                }
                case EMsgTypeReqUpdateSchema:
                {
                    pMsg = new CMsgReqUpdateSchema(pMsgHdr);
                    break;
                }
                case EMsgTypeConUpdateSchema:
                {
                    pMsg = new CMsgConUpdateSchema(pMsgHdr);
                    break;
                }
                case EMsgTypeReqReadData:
                {
                    pMsg = new CMsgReqReadData(pMsgHdr);
                    break;
                }
                case EMsgTypeConReadData:
                {
                    pMsg = new CMsgConReadData(pMsgHdr);
                    break;
                }
                case EMsgTypeReqSaveData:
                {
                    pMsg = new CMsgReqSaveData(pMsgHdr);
                    break;
                }
                case EMsgTypeConSaveData:
                {
                    pMsg = new CMsgConSaveData(pMsgHdr);
                    break;
                }
                case EMsgTypeReqSelectData:
                {
                    pMsg = new CMsgReqSelectData(pMsgHdr);
                    break;
                }
                case EMsgTypeConSelectData:
                {
                    pMsg = new CMsgConSelectData(pMsgHdr);
                    break;
                }
                case EMsgTypeReqUpdateData:
                {
                    pMsg = new CMsgReqUpdateData(pMsgHdr);
                    break;
                }
                case EMsgTypeConUpdateData:
                {
                    pMsg = new CMsgConUpdateData(pMsgHdr);
                    break;
                }
                case EMsgTypeReqSet2Default:
                {
                    pMsg = new CMsgReqSet2Default(pMsgHdr);
                    break;
                }
                case EMsgTypeConSet2Default:
                {
                    pMsg = new CMsgConSet2Default(pMsgHdr);
                    break;
                }
                default:
                {
                    strAddErrInfo += "Message type " + msgType2Str(pMsgHdr->m_msgType) + " out of range";
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInvalidMessageContent);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                    break;
                }
            } // switch( pHdr->m_msgType )

            if( !errResultInfo.isErrorResult() )
            {
                if( pMsg == nullptr )
                {
                    strAddErrInfo += "Message type '0x" + QString::number(pMsgHdr->m_msgType,16) + "' out of range";
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInvalidMessageContent);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                }
            }
        } // if( !errResultInfo.isErrorResult() )
    } // if( i_byteArr.size() == iMsgProtSize )

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
    return "ReqReceiveData";
}

//------------------------------------------------------------------------------
QString CMsgReqReceiveData::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}


/*******************************************************************************
class CMsgReqStartup : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqStartup::CMsgReqStartup(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqStartup,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_hostSettings()
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqStartup::~CMsgReqStartup()
//------------------------------------------------------------------------------
{
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
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setHostSettings(m_hostSettings);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqStartup::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqStartup::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqStartup";
}

//------------------------------------------------------------------------------
QString CMsgReqStartup::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqStartup::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConStartup : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

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
    m_hostSettings()
{
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
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setHostSettings(m_hostSettings);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConStartup::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConStartup";
}

//------------------------------------------------------------------------------
QString CMsgConStartup::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConStartup::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


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

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqShutdown::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqShutdown::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqShutdown";
}

//------------------------------------------------------------------------------
QString CMsgReqShutdown::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqShutdown::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


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
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConShutdown::~CMsgConShutdown()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConShutdown::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConShutdown* pMsgCloned = new CMsgConShutdown(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
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
QString CMsgConShutdown::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConShutdown";
}

//------------------------------------------------------------------------------
QString CMsgConShutdown::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConShutdown::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqChangeSettings : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqChangeSettings::CMsgReqChangeSettings(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqChangeSettings,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_srvCltType(ESrvCltTypeUndefined),
    m_serverHostSettings(),
    m_clientHostSettings()
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqChangeSettings::~CMsgReqChangeSettings()
//------------------------------------------------------------------------------
{
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
    else
    {
        socketType = m_clientHostSettings.m_socketType;
    }
    return socketType;

} // getSocketType

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgReqChangeSettings::setServerHostSettings( const SServerHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    m_srvCltType = ESrvCltTypeServer;
    m_serverHostSettings = i_hostSettings;
}

//------------------------------------------------------------------------------
void CMsgReqChangeSettings::setClientHostSettings( const SClientHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    m_srvCltType = ESrvCltTypeClient;
    m_clientHostSettings = i_hostSettings;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqChangeSettings::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqChangeSettings* pMsgCloned = new CMsgReqChangeSettings(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    if( m_srvCltType == ESrvCltTypeServer )
    {
        pMsgCloned->setServerHostSettings(m_serverHostSettings);
    }
    else
    {
        pMsgCloned->setClientHostSettings(m_clientHostSettings);
    }

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqChangeSettings::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqChangeSettings::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqChangeSettings";
}

//------------------------------------------------------------------------------
QString CMsgReqChangeSettings::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqChangeSettings::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConChangeSettings : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConChangeSettings::CMsgConChangeSettings(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
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
    m_srvCltType(ESrvCltTypeUndefined),
    m_serverHostSettings(),
    m_socketDscr()
{
} // ctor

//------------------------------------------------------------------------------
CMsgConChangeSettings::~CMsgConChangeSettings()
//------------------------------------------------------------------------------
{
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
    else
    {
        socketType = m_socketDscr.m_socketType;
    }
    return socketType;

} // getSocketType

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgConChangeSettings::setServerHostSettings( const SServerHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    m_srvCltType = ESrvCltTypeServer;
    m_serverHostSettings = i_hostSettings;
}

//------------------------------------------------------------------------------
void CMsgConChangeSettings::setSocketDscr( const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    m_srvCltType = ESrvCltTypeClient;
    m_socketDscr = i_socketDscr;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConChangeSettings::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConChangeSettings* pMsgCloned = new CMsgConChangeSettings(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    if( m_srvCltType == ESrvCltTypeServer )
    {
        pMsgCloned->setServerHostSettings(m_serverHostSettings);
    }
    else
    {
        pMsgCloned->setSocketDscr(m_socketDscr);
    }

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConChangeSettings::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConChangeSettings";
}

//------------------------------------------------------------------------------
QString CMsgConChangeSettings::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConChangeSettings::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqConnect : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqConnect::CMsgReqConnect(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqConnect,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_pDb(nullptr),
    m_hostSettings()
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqConnect::~CMsgReqConnect()
//------------------------------------------------------------------------------
{
    m_pDb = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgReqConnect::setDb( CDb* i_pDb )
//------------------------------------------------------------------------------
{
    m_pDb = i_pDb;
}

//------------------------------------------------------------------------------
void CMsgReqConnect::setHostSettings( const SClientHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    m_hostSettings = i_hostSettings;
}

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
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setDb(m_pDb);
    pMsgCloned->setHostSettings(m_hostSettings);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqConnect::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqConnect::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqConnect";
}

//------------------------------------------------------------------------------
QString CMsgReqConnect::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqConnect::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


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
    m_socketDscr()
{
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
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setSocketDscr(m_socketDscr);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConConnect::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConConnect";
}

//------------------------------------------------------------------------------
QString CMsgConConnect::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConConnect::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgIndConnected : public CMsgInd
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgIndConnected::CMsgIndConnected(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeAcknowledged,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgInd(
        /* iMsgType            */ EMsgTypeIndConnected,
        /* pObjSender          */ i_pObjSender,
        /* pObjReceiver        */ i_pObjReceiver,
        /* bMustBeAcknowledged */ i_bMustBeAcknowledged,
        /* iReqId              */ i_iReqId,
        /* iMsgIdReq           */ i_iMsgId ),
    m_socketDscr()
{
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
void CMsgIndConnected::setSocketDscr( const SSocketDscr& i_socketDscr )
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
        /* pObjSender          */ m_pObjSender,
        /* pObjReceiver        */ m_pObjReceiver,
        /* bMustBeAcknowledged */ m_bMustBeAcknowledged,
        /* iReqId              */ m_iReqId,
        /* iMsgId              */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setSocketDscr(m_socketDscr);

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
    return "IndConnected";
}

//------------------------------------------------------------------------------
QString CMsgIndConnected::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgInd::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgIndConnected::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgInd::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqDisconnect : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqDisconnect::CMsgReqDisconnect(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqDisconnect,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_socketDscr()
{
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
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setSocketDscr(m_socketDscr);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqDisconnect::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqDisconnect::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqDisconnect";
}

//------------------------------------------------------------------------------
QString CMsgReqDisconnect::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqDisconnect::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


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
    m_socketDscr()
{
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
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* iReqId             */ m_iReqId,
        /* iMsgIdReq          */ m_iMsgId,
        /* errResultInfo      */ m_errResultInfo,
        /* iProgressInPerCent */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setSocketDscr(m_socketDscr);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConDisconnect::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConDisconnect";
}

//------------------------------------------------------------------------------
QString CMsgConDisconnect::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConDisconnect::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgIndDisconnected : public CMsgInd
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgIndDisconnected::CMsgIndDisconnected(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeAcknowledged,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgInd(
        /* iMsgType            */ EMsgTypeIndDisconnected,
        /* pObjSender          */ i_pObjSender,
        /* pObjReceiver        */ i_pObjReceiver,
        /* bMustBeAcknowledged */ i_bMustBeAcknowledged,
        /* iReqId              */ i_iReqId,
        /* iMsgIdReq           */ i_iMsgId ),
    m_socketDscr()
{
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
        /* pObjSender          */ m_pObjSender,
        /* pObjReceiver        */ m_pObjReceiver,
        /* bMustBeAcknowledged */ m_bMustBeAcknowledged,
        /* iReqId              */ m_iReqId,
        /* iMsgId              */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    pMsgCloned->setSocketDscr(m_socketDscr);

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
    return "IndDisconnected";
}

//------------------------------------------------------------------------------
QString CMsgIndDisconnected::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgInd::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgIndDisconnected::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgInd::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqRegister : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqRegister::CMsgReqRegister(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqRegister,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqRegister::CMsgReqRegister( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqRegister::~CMsgReqRegister()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqRegister::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqRegister* pMsgCloned = new CMsgReqRegister(
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
CMsgCon* CMsgReqRegister::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqRegister::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqRegister";
}

//------------------------------------------------------------------------------
QString CMsgReqRegister::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqRegister::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConRegister : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConRegister::CMsgConRegister(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConRegister,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConRegister::CMsgConRegister( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConRegister::~CMsgConRegister()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConRegister::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConRegister* pMsgCloned = new CMsgConRegister(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
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
QString CMsgConRegister::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConRegister";
}

//------------------------------------------------------------------------------
QString CMsgConRegister::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConRegister::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqUnregister : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqUnregister::CMsgReqUnregister(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqUnregister,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqUnregister::CMsgReqUnregister( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqUnregister::~CMsgReqUnregister()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqUnregister::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqUnregister* pMsgCloned = new CMsgReqUnregister(
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
CMsgCon* CMsgReqUnregister::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqUnregister::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqUnregister";
}

//------------------------------------------------------------------------------
QString CMsgReqUnregister::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqUnregister::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConUnregister : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConUnregister::CMsgConUnregister(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConUnregister,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConUnregister::CMsgConUnregister( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConUnregister::~CMsgConUnregister()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConUnregister::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConUnregister* pMsgCloned = new CMsgConUnregister(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
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
QString CMsgConUnregister::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConUnregister";
}

//------------------------------------------------------------------------------
QString CMsgConUnregister::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConUnregister::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqReadSchema : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqReadSchema::CMsgReqReadSchema(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqReadSchema,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqReadSchema::CMsgReqReadSchema( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqReadSchema::~CMsgReqReadSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqReadSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqReadSchema* pMsgCloned = new CMsgReqReadSchema(
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
CMsgCon* CMsgReqReadSchema::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqReadSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqReadSchema";
}

//------------------------------------------------------------------------------
QString CMsgReqReadSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqReadSchema::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConReadSchema : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConReadSchema::CMsgConReadSchema(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConReadSchema,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConReadSchema::CMsgConReadSchema( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConReadSchema::~CMsgConReadSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConReadSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConReadSchema* pMsgCloned = new CMsgConReadSchema(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
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
QString CMsgConReadSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConReadSchema";
}

//------------------------------------------------------------------------------
QString CMsgConReadSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConReadSchema::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqSaveSchema : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqSaveSchema::CMsgReqSaveSchema(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqSaveSchema,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSaveSchema::CMsgReqSaveSchema( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSaveSchema::~CMsgReqSaveSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqSaveSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqSaveSchema* pMsgCloned = new CMsgReqSaveSchema(
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
CMsgCon* CMsgReqSaveSchema::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqSaveSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqSaveSchema";
}

//------------------------------------------------------------------------------
QString CMsgReqSaveSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqSaveSchema::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConSaveSchema : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConSaveSchema::CMsgConSaveSchema(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConSaveSchema,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConSaveSchema::CMsgConSaveSchema( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConSaveSchema::~CMsgConSaveSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConSaveSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConSaveSchema* pMsgCloned = new CMsgConSaveSchema(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
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
QString CMsgConSaveSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConSaveSchema";
}

//------------------------------------------------------------------------------
QString CMsgConSaveSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConSaveSchema::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqSelectSchema : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqSelectSchema::CMsgReqSelectSchema(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqSelectSchema,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ true,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSelectSchema::CMsgReqSelectSchema( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSelectSchema::~CMsgReqSelectSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqSelectSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqSelectSchema* pMsgCloned = new CMsgReqSelectSchema(
        /* pObjSender   */ m_pObjSender,
        /* pObjReceiver */ m_pObjReceiver,
        /* iReqId       */ m_iReqId,
        /* iMsgId       */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqSelectSchema::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqSelectSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqSelectSchema";
}

//------------------------------------------------------------------------------
QString CMsgReqSelectSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqSelectSchema::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConSelectSchema : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConSelectSchema::CMsgConSelectSchema(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConSelectSchema,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConSelectSchema::CMsgConSelectSchema( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConSelectSchema::~CMsgConSelectSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConSelectSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConSelectSchema* pMsgCloned = new CMsgConSelectSchema(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
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
QString CMsgConSelectSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConSelectSchema";
}

//------------------------------------------------------------------------------
QString CMsgConSelectSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConSelectSchema::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqUpdateSchema : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqUpdateSchema::CMsgReqUpdateSchema(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqUpdateSchema,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqUpdateSchema::CMsgReqUpdateSchema( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqUpdateSchema::~CMsgReqUpdateSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqUpdateSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqUpdateSchema* pMsgCloned = new CMsgReqUpdateSchema(
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
CMsgCon* CMsgReqUpdateSchema::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqUpdateSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqUpdateSchema";
}

//------------------------------------------------------------------------------
QString CMsgReqUpdateSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqUpdateSchema::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConUpdateSchema : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConUpdateSchema::CMsgConUpdateSchema(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConUpdateSchema,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConUpdateSchema::CMsgConUpdateSchema( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConUpdateSchema::~CMsgConUpdateSchema()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConUpdateSchema::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConUpdateSchema* pMsgCloned = new CMsgConUpdateSchema(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
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
QString CMsgConUpdateSchema::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConUpdateSchema";
}

//------------------------------------------------------------------------------
QString CMsgConUpdateSchema::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConUpdateSchema::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqReadData : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqReadData::CMsgReqReadData(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqReadData,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqReadData::CMsgReqReadData( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgReq(i_pMsgHdr)
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
CMsg* CMsgReqReadData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqReadData* pMsgCloned = new CMsgReqReadData(
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
CMsgCon* CMsgReqReadData::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqReadData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqReadData";
}

//------------------------------------------------------------------------------
QString CMsgReqReadData::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqReadData::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConReadData : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConReadData::CMsgConReadData(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConReadData,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConReadData::CMsgConReadData( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConReadData::~CMsgConReadData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConReadData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConReadData* pMsgCloned = new CMsgConReadData(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
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
QString CMsgConReadData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConReadData";
}

//------------------------------------------------------------------------------
QString CMsgConReadData::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConReadData::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqSaveData : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqSaveData::CMsgReqSaveData(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqSaveData,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSaveData::CMsgReqSaveData( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSaveData::~CMsgReqSaveData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqSaveData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqSaveData* pMsgCloned = new CMsgReqSaveData(
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
CMsgCon* CMsgReqSaveData::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqSaveData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqSaveData";
}

//------------------------------------------------------------------------------
QString CMsgReqSaveData::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqSaveData::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConSaveData : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConSaveData::CMsgConSaveData(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConSaveData,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConSaveData::CMsgConSaveData( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConSaveData::~CMsgConSaveData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConSaveData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConSaveData* pMsgCloned = new CMsgConSaveData(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
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
QString CMsgConSaveData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConSaveData";
}

//------------------------------------------------------------------------------
QString CMsgConSaveData::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConSaveData::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqSelectData : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqSelectData::CMsgReqSelectData(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqSelectData,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ true,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSelectData::CMsgReqSelectData( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSelectData::~CMsgReqSelectData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqSelectData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqSelectData* pMsgCloned = new CMsgReqSelectData(
        /* pObjSender   */ m_pObjSender,
        /* pObjReceiver */ m_pObjReceiver,
        /* iReqId       */ m_iReqId,
        /* iMsgId       */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqSelectData::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqSelectData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqSelectData";
}

//------------------------------------------------------------------------------
QString CMsgReqSelectData::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqSelectData::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConSelectData : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConSelectData::CMsgConSelectData(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConSelectData,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConSelectData::CMsgConSelectData( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConSelectData::~CMsgConSelectData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConSelectData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConSelectData* pMsgCloned = new CMsgConSelectData(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
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
QString CMsgConSelectData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConSelectData";
}

//------------------------------------------------------------------------------
QString CMsgConSelectData::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConSelectData::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqUpdateData : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqUpdateData::CMsgReqUpdateData(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqUpdateData,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqUpdateData::CMsgReqUpdateData( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqUpdateData::~CMsgReqUpdateData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqUpdateData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqUpdateData* pMsgCloned = new CMsgReqUpdateData(
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
CMsgCon* CMsgReqUpdateData::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqUpdateData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqUpdateData";
}

//------------------------------------------------------------------------------
QString CMsgReqUpdateData::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqUpdateData::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConUpdateData : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConUpdateData::CMsgConUpdateData(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConUpdateData,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConUpdateData::CMsgConUpdateData( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConUpdateData::~CMsgConUpdateData()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConUpdateData::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConUpdateData* pMsgCloned = new CMsgConUpdateData(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
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
QString CMsgConUpdateData::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConUpdateData";
}

//------------------------------------------------------------------------------
QString CMsgConUpdateData::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConUpdateData::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReqSet2Default : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqSet2Default::CMsgReqSet2Default(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqSet2Default,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSet2Default::CMsgReqSet2Default( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgReq(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSet2Default::~CMsgReqSet2Default()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqSet2Default::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqSet2Default* pMsgCloned = new CMsgReqSet2Default(
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
CMsgCon* CMsgReqSet2Default::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqSet2Default::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ReqSet2Default";
}

//------------------------------------------------------------------------------
QString CMsgReqSet2Default::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReqSet2Default::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgReq::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgConSet2Default : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConSet2Default::CMsgConSet2Default(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConSet2Default,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConSet2Default::CMsgConSet2Default( MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsgCon(i_pMsgHdr)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConSet2Default::~CMsgConSet2Default()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConSet2Default::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConSet2Default* pMsgCloned = new CMsgConSet2Default(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
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
QString CMsgConSet2Default::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ConSet2Default";
}

//------------------------------------------------------------------------------
QString CMsgConSet2Default::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    return CMsgCon::getAddTrcInfoStr();
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgConSet2Default::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsgCon::serialize();

    //MsgProtocol::SMsgHeader* pHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    return byteArr;

} // serialize


