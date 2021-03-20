/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include "ZSRemCmd/ZSRemCmdMsg.h"
#include "ZSSys/ZSSysEnumEntry.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::RemCmd;


/*==============================================================================
enum ERequest
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrRequests[ZS::RemCmd::ERequestCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( ZS::RemCmd::ERequestNone,       "None"       ),
    /* 1 */ SEnumEntry( ZS::RemCmd::ERequestCmdExecute, "CmdExecute" )
};

//------------------------------------------------------------------------------
QString ZS::RemCmd::request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrRequests,ZS::RemCmd::ERequestCount,i_iRequest);
}


/*==============================================================================
enum MsgType
==============================================================================*/

const SEnumEntry s_arEnumStrMsgTypes[ZS::RemCmd::EMsgTypeCount] =
{
    /*  0 */ SEnumEntry( EMsgTypeReqCmdExecute-EMsgTypeMin, "ReqCmdExecute" ),
    /*  1 */ SEnumEntry( EMsgTypeConCmdExecute-EMsgTypeMin, "ConCmdExecute" )
};

//------------------------------------------------------------------------------
QString ZS::RemCmd::msgType2Str( int i_iMsgType )
//------------------------------------------------------------------------------
{
    QString strMsgType = QString::number(i_iMsgType);
    int     iMsgType   = i_iMsgType;

    if( iMsgType >= RemCmd::EMsgTypeMin && iMsgType <= RemCmd::EMsgTypeMax )
    {
        iMsgType -= RemCmd::EMsgTypeMin;
        strMsgType = SEnumEntry::enumerator2Str(s_arEnumStrMsgTypes,RemCmd::EMsgTypeCount,iMsgType);
    }
    return strMsgType;

} // msgType2Str


/*******************************************************************************
class CMsgReqCmdExecute : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqCmdExecute::CMsgReqCmdExecute(
    QObject*                    i_pObjSender,
    QObject*                    i_pObjReceiver,
    const ZS::Ipc::SSocketDscr& i_socketDscr,
    const QString&              i_strCmd,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs,
    bool                        i_bMustBeConfirmed,
    qint64                      i_iParentReqId,
    qint64                      i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqCmdExecute,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iParentReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_socketDscr(i_socketDscr),
    m_strCmd(i_strCmd),
    m_bIsQuery(i_bIsQuery),
    m_byteArrArgs(i_byteArrArgs)
{
    setObjectName( RemCmd::msgType2Str(EMsgTypeReqCmdExecute) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgReqCmdExecute::~CMsgReqCmdExecute()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqCmdExecute::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqCmdExecute* pMsgCloned = new CMsgReqCmdExecute(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* socketDscr       */ m_socketDscr,
        /* strCmd           */ m_strCmd,
        /* bIsQuery         */ m_bIsQuery,
        /* byteArrArgs      */ m_byteArrArgs,
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
CMsgCon* CMsgReqCmdExecute::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    CMsgConCmdExecute* pMsgCon = new CMsgConCmdExecute(
        /* pObjSender     */ m_pObjReceiver,
        /* pObjReceiver   */ m_pObjSender,
        /* socketDscr     */ m_socketDscr,
        /* strCmd         */ m_strCmd,
        /* bWasQuery      */ m_bIsQuery,
        /* byteArrResults */ QByteArray(),
        /* iReqId         */ m_iReqId,
        /* iMsgIdReq      */ m_iMsgId,
        /* errResultInfo  */ SErrResultInfo(),
        /* iProgress      */ 100 );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqCmdExecute::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + "(RemCmd::ReqCmdExecute)";
}

//------------------------------------------------------------------------------
QString CMsgReqCmdExecute::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgReq::getAddTrcInfoStr(i_iDetailLevel,i_format);

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgConCmdExecute : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConCmdExecute::CMsgConCmdExecute(
    QObject*                    i_pObjSender,
    QObject*                    i_pObjReceiver,
    const ZS::Ipc::SSocketDscr& i_socketDscr,
    const QString&              i_strCmd,
    bool                        i_bWasQuery,
    const QByteArray&           i_byteArrResults,
    qint64                      i_iParentReqId,
    qint64                      i_iMsgIdReq,
    const SErrResultInfo&       i_errResultInfo,
    int                         i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConCmdExecute,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iParentReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_socketDscr(i_socketDscr),
    m_strCmd(i_strCmd),
    m_bWasQuery(i_bWasQuery),
    m_byteArrResults(i_byteArrResults)
{
    setObjectName( RemCmd::msgType2Str(EMsgTypeConCmdExecute) + QString::number(m_iMsgId) );

} // ctor

//------------------------------------------------------------------------------
CMsgConCmdExecute::~CMsgConCmdExecute()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgConCmdExecute::setResults( const QByteArray& i_byteArrResults )
//------------------------------------------------------------------------------
{
    m_byteArrResults = i_byteArrResults;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConCmdExecute::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConCmdExecute* pMsgCloned = new CMsgConCmdExecute(
        /* pObjSender     */ m_pObjSender,
        /* pObjReceiver   */ m_pObjReceiver,
        /* socketDscr     */ m_socketDscr,
        /* strCmd         */ m_strCmd,
        /* bWasQuery      */ m_bWasQuery,
        /* byteArrResults */ m_byteArrResults,
        /* iReqId         */ m_iReqId,
        /* iMsgIdReq      */ m_iMsgId,
        /* errResultInfo  */ m_errResultInfo,
        /* iProgress      */ m_iProgressInPerCent );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConCmdExecute::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + "(RemCmd::ConCmdExecute)";
}

//------------------------------------------------------------------------------
QString CMsgConCmdExecute::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsgCon::getAddTrcInfoStr(i_iDetailLevel,i_format);

    return strAddTrcInfo;

} // getAddTrcInfoStr
