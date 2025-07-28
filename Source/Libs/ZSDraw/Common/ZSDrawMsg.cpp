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

#include "ZSDraw/Common/ZSDrawMsg.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
enum MsgType
==============================================================================*/

const SEnumEntry s_arEnumStrMsgTypes[ZS::Draw::EMsgTypeCount] =
{
    /* 1013 */ SEnumEntry(EMsgTypeReqDeleteGraphobj-EMsgTypeMin, "ReqDeleteGraphObj")
};

//------------------------------------------------------------------------------
QString ZS::Draw::msgType2Str( int i_iMsgType )
//------------------------------------------------------------------------------
{
    QString strMsgType = QString::number(i_iMsgType);
    int iMsgType = i_iMsgType;

    if (iMsgType >= Draw::EMsgTypeMin && iMsgType <= Draw::EMsgTypeMax)
    {
        iMsgType -= Draw::EMsgTypeMin;
        strMsgType = SEnumEntry::enumerator2Str(s_arEnumStrMsgTypes, Draw::EMsgTypeCount, iMsgType);
    }
    return strMsgType;
}


/*******************************************************************************
class CMsgReqDeleteGraphObj : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqDeleteGraphObj::CMsgReqDeleteGraphObj(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    const QString& i_strKeyInTree) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqDeleteGraphobj,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ false,
        /* iReqId           */ -1,
        /* iMsgIdReq        */ -1 ),
    m_strKeyInTree(i_strKeyInTree)
{
    setObjectName(Draw::msgType2Str(EMsgTypeReqDeleteGraphobj) + QString::number(m_iMsgId) );
}

//------------------------------------------------------------------------------
CMsgReqDeleteGraphObj::~CMsgReqDeleteGraphObj()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqDeleteGraphObj::keyInTree() const
//------------------------------------------------------------------------------
{
    return m_strKeyInTree;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqDeleteGraphObj::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqDeleteGraphObj* pMsgCloned = new CMsgReqDeleteGraphObj(
        /* pObjSender   */ m_pObjSender,
        /* pObjReceiver */ m_pObjReceiver,
        /* strKeyInTree */ m_strKeyInTree);
    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);
    pMsgCloned->setTimeoutInMs(m_iTimeout_ms);
    pMsgCloned->setIsBlockingRequest(m_bIsBlockingRequest);
    return pMsgCloned;
}

//------------------------------------------------------------------------------
CMsgCon* CMsgReqDeleteGraphObj::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    return nullptr;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqDeleteGraphObj::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (Draw::ReqDeleteGraphObj)";
}

//------------------------------------------------------------------------------
QString CMsgReqDeleteGraphObj::getAddTrcInfoStr(int i_iDetailLevel, ZS::System::EContentToStrFormat i_format)
//------------------------------------------------------------------------------
{
    return CMsgReq::getAddTrcInfoStr(i_iDetailLevel, i_format) + ", KeyInTree: " + m_strKeyInTree;
}
