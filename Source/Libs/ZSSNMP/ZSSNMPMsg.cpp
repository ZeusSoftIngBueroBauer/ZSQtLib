/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

--------------------------------------------------------------------------------

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

//lint -esym(766,..\ZSSys/Include/ZSSysMemLeakDump.h)
#include "ZSSNMP/Include/ZSSNMPMsg.h"
#include "ZSSys/Include/ZSSysEnum.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

static const ZS::SEnumEntry s_arEnumStrMsgTypes[EMsgTypeCount] = {
    ZS::SEnumEntry( EMsgTypeReqOpenSession,  "ReqOpenSession"  ),
    ZS::SEnumEntry( EMsgTypeConOpenSession,  "ConOpenSession"  ),
    ZS::SEnumEntry( EMsgTypeReqCloseSession, "ReqCloseSession" ),
    ZS::SEnumEntry( EMsgTypeConCloseSession, "ConCloseSession" ),
    ZS::SEnumEntry( EMsgTypeReqGetValue,     "ReqGetValue"     ),
    ZS::SEnumEntry( EMsgTypeReqGetNextValue, "ReqGetNextValue" ),
    ZS::SEnumEntry( EMsgTypeReqGetBulk,      "ReqGetBulk"      ),
    ZS::SEnumEntry( EMsgTypeReqWalk,         "ReqWalk"         ),
    ZS::SEnumEntry( EMsgTypeConResponse,     "ConResponse"     ),
    ZS::SEnumEntry( EMsgTypeIndTimeout,      "IndTimeout"      ),
};

//------------------------------------------------------------------------------
QString ZS::SNMP::msgType2Str( int i_iMsgType )
//------------------------------------------------------------------------------
{
    return ZS::SEnumEntry::val2Str(s_arEnumStrMsgTypes,_ZSArrLen(s_arEnumStrMsgTypes),i_iMsgType);
}

//------------------------------------------------------------------------------
QString ZS::SNMP::msgType2Str( CMsg* i_pMsg )
//------------------------------------------------------------------------------
{
    QString str;

    int iMsgType = i_pMsg->getMsgType();

    switch( iMsgType )
    {
        case EMsgTypeReqOpenSession:
        {
            SNMP::CMsgReqOpenSession* pMsg = dynamic_cast<SNMP::CMsgReqOpenSession*>(i_pMsg);
            if( pMsg != NULL )
            {
                str = msgType2Str(iMsgType);
            }
            break;
        }
        case EMsgTypeConOpenSession:
        {
            SNMP::CMsgConOpenSession* pMsg = dynamic_cast<SNMP::CMsgConOpenSession*>(i_pMsg);
            if( pMsg != NULL )
            {
                str = msgType2Str(iMsgType);
            }
            break;
        }
        case EMsgTypeReqCloseSession:
        {
            SNMP::CMsgReqCloseSession* pMsg = dynamic_cast<SNMP::CMsgReqCloseSession*>(i_pMsg);
            if( pMsg != NULL )
            {
                str = msgType2Str(iMsgType);
            }
            break;
        }
        case EMsgTypeConCloseSession:
        {
            SNMP::CMsgConCloseSession* pMsg = dynamic_cast<SNMP::CMsgConCloseSession*>(i_pMsg);
            if( pMsg != NULL )
            {
                str = msgType2Str(iMsgType);
            }
            break;
        }
        case EMsgTypeReqGetValue:
        {
            SNMP::CMsgReqGetValue* pMsg = dynamic_cast<SNMP::CMsgReqGetValue*>(i_pMsg);
            if( pMsg != NULL )
            {
                str = msgType2Str(iMsgType);
            }
            break;
        }
        case EMsgTypeReqGetNextValue:
        {
            SNMP::CMsgReqGetNextValue* pMsg = dynamic_cast<SNMP::CMsgReqGetNextValue*>(i_pMsg);
            if( pMsg != NULL )
            {
                str = msgType2Str(iMsgType);
            }
            break;
        }
        case EMsgTypeReqGetBulk:
        {
            SNMP::CMsgReqGetBulk* pMsg = dynamic_cast<SNMP::CMsgReqGetBulk*>(i_pMsg);
            if( pMsg != NULL )
            {
                str = msgType2Str(iMsgType);
            }
            break;
        }
        case EMsgTypeReqWalk:
        {
            SNMP::CMsgReqWalk* pMsg = dynamic_cast<SNMP::CMsgReqWalk*>(i_pMsg);
            if( pMsg != NULL )
            {
                str = msgType2Str(iMsgType);
            }
            break;
        }
        case EMsgTypeConResponse:
        {
            SNMP::CMsgConResponse* pMsg = dynamic_cast<SNMP::CMsgConResponse*>(i_pMsg);
            if( pMsg != NULL )
            {
                str = msgType2Str(iMsgType);
            }
            break;
        }
        case EMsgTypeIndTimeout:
        {
            SNMP::CMsgIndTimeout* pMsg = dynamic_cast<SNMP::CMsgIndTimeout*>(i_pMsg);
            if( pMsg != NULL )
            {
                str = msgType2Str(iMsgType);
            }
            break;
        }
        default:
        {
            break;
        }
    }
    if( str.isEmpty() )
    {
        str = "Undefined SNMP Message Type (= " + QString::number(iMsgType) + ")";
    }

    return str;

} // msgType2Str


/*******************************************************************************
class CMsgReqOpenSession : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqOpenSession::CMsgReqOpenSession(
    QObject*             i_pObjSender,
    QObject*             i_pObjReceiver,
    const SHostSettings& i_hostSettings,
    bool                 i_bMustBeConfirmed,
    int                  i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqOpenSession,
        /* pObjSender       */ i_pObjSender,
        /* iSenderId        */ -1,
        /* pObjReceiver     */ i_pObjReceiver,
        /* iReceiverId      */ -1,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iMsgIdReq        */ i_iMsgId ),
    m_hostSettings(i_hostSettings)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqOpenSession::~CMsgReqOpenSession()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SHostSettings CMsgReqOpenSession::getHostSettings() const
//------------------------------------------------------------------------------
{
    return m_hostSettings;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
ZS::CMsg* CMsgReqOpenSession::clone()
//------------------------------------------------------------------------------
{
    CMsgReqOpenSession* pMsgCloned = new CMsgReqOpenSession(
        /* pObjSender       */ getSender(),
        /* pObjReceiver     */ getReceiver(),
        /* hostSettings     */ getHostSettings(),
        /* bMustBeConfirmed */ mustBeConfirmed(),
        /* iMsgId           */ getMsgId() );
    return pMsgCloned;
}

//------------------------------------------------------------------------------
QString CMsgReqOpenSession::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "REQ::OPENSESSION";
}

//------------------------------------------------------------------------------
QString CMsgReqOpenSession::contentToStr( ZS::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strContent;
    strContent += "HostName:  " + m_hostSettings.m_strName + "\n";
    strContent += "HostPort:  " + QString::number(m_hostSettings.m_uPort) + "\n";
    strContent += "Community: " + m_hostSettings.m_strCommunity + "\n";
    return strContent;
}


/*******************************************************************************
class CMsgConOpenSession : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConOpenSession::CMsgConOpenSession(
    QObject*                  i_pObjSender,
    QObject*                  i_pObjReceiver,
    int                       i_iMsgIdReq,
    const ZS::SErrResultInfo& i_errResultInfo,
    int                       i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConOpenSession,
        /* pObjSender         */ i_pObjSender,
        /* iSenderId          */ -1,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReceiverId        */ -1,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
}

//------------------------------------------------------------------------------
CMsgConOpenSession::~CMsgConOpenSession()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
ZS::CMsg* CMsgConOpenSession::clone()
//------------------------------------------------------------------------------
{
    CMsgConOpenSession* pMsgCloned = new CMsgConOpenSession(
        /* pObjSender       */ getSender(),
        /* pObjReceiver     */ getReceiver(),
        /* iMsgIdReq        */ getMsgId(),
        /* errResultInfo    */ getErrResultInfo(),
        /* iProgress        */ getProgress() );
    return pMsgCloned;
}

//------------------------------------------------------------------------------
QString CMsgConOpenSession::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "CON::OPENSESSION";
}

//------------------------------------------------------------------------------
QString CMsgConOpenSession::contentToStr( ZS::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strContent;

    if( m_errResultInfo.getResult() == EResultSuccess )
    {
        strContent += "Result: Success\n";
    }
    else
    {
        strContent += "Result:  " + m_errResultInfo.getResultStr() + "\n";
        strContent += "AddInfo: " + m_errResultInfo.getAddErrInfoDscr() + "\n";
    }
    return strContent;
}

/*******************************************************************************
class CMsgReqCloseSession : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqCloseSession::CMsgReqCloseSession(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    int      i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqCloseSession,
        /* pObjSender       */ i_pObjSender,
        /* iSenderId        */ -1,
        /* pObjReceiver     */ i_pObjReceiver,
        /* iReceiverId      */ -1,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iMsgIdReq        */ i_iMsgId )
{
}

//------------------------------------------------------------------------------
CMsgReqCloseSession::~CMsgReqCloseSession()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
ZS::CMsg* CMsgReqCloseSession::clone()
//------------------------------------------------------------------------------
{
    CMsgReqCloseSession* pMsgCloned = new CMsgReqCloseSession(
        /* pObjSender       */ getSender(),
        /* pObjReceiver     */ getReceiver(),
        /* bMustBeConfirmed */ mustBeConfirmed(),
        /* iMsgId           */ getMsgId() );
    return pMsgCloned;
}

//------------------------------------------------------------------------------
QString CMsgReqCloseSession::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "REQ::CLOSESESSION";
}


/*******************************************************************************
class CMsgConCloseSession : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConCloseSession::CMsgConCloseSession(
    QObject*                  i_pObjSender,
    QObject*                  i_pObjReceiver,
    int                       i_iMsgIdReq,
    const ZS::SErrResultInfo& i_errResultInfo,
    int                       i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConCloseSession,
        /* pObjSender         */ i_pObjSender,
        /* iSenderId          */ -1,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReceiverId        */ -1,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
}

//------------------------------------------------------------------------------
CMsgConCloseSession::~CMsgConCloseSession()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
ZS::CMsg* CMsgConCloseSession::clone()
//------------------------------------------------------------------------------
{
    CMsgConCloseSession* pMsgCloned = new CMsgConCloseSession(
        /* pObjSender    */ getSender(),
        /* pObjReceiver  */ getReceiver(),
        /* iMsgIdReq     */ getMsgId(),
        /* errResultInfo */ getErrResultInfo(),
        /* iProgress     */ getProgress() );
    return pMsgCloned;
}

//------------------------------------------------------------------------------
QString CMsgConCloseSession::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "CON::CLOSESESSION";
}

//------------------------------------------------------------------------------
QString CMsgConCloseSession::contentToStr( ZS::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strContent;

    if( m_errResultInfo.getResult() == EResultSuccess )
    {
        strContent += "Result: Success\n";
    }
    else
    {
        strContent += "Result:  " + m_errResultInfo.getResultStr() + "\n";
        strContent += "AddInfo: " + m_errResultInfo.getAddErrInfoDscr() + "\n";
    }
    return strContent;
}


/*******************************************************************************
class CMsgReqGet : public CMsgReq
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqGet::CMsgReqGet(
    int         i_iMsgType,
    QObject*    i_pObjSender,
    QObject*    i_pObjReceiver,
    const COid& i_oid,
    bool        i_bMustBeConfirmed,
    int         i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ i_iMsgType,
        /* pObjSender       */ i_pObjSender,
        /* iSenderId        */ -1,
        /* pObjReceiver     */ i_pObjReceiver,
        /* iReceiverId      */ -1,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iMsgIdReq        */ i_iMsgId ),
    m_oidPtrList()
{
    m_oidPtrList.append( new COid(i_oid) );

} // ctor

//------------------------------------------------------------------------------
CMsgReqGet::CMsgReqGet(
    int                i_iMsgType,
    QObject*           i_pObjSender,
    QObject*           i_pObjReceiver,
    const TOidPtrList& i_oidPtrList,
    bool               i_bMustBeConfirmed,
    int                i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ i_iMsgType,
        /* pObjSender       */ i_pObjSender,
        /* iSenderId        */ -1,
        /* pObjReceiver     */ i_pObjReceiver,
        /* iReceiverId      */ -1,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iMsgIdReq        */ i_iMsgId ),
    m_oidPtrList(i_oidPtrList)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqGet::~CMsgReqGet()
//------------------------------------------------------------------------------
{
    COid* pOid;
    int   idx;

    for( idx = 0; idx < m_oidPtrList.count(); idx++ )
    {
        pOid = m_oidPtrList[idx];

        try
        {
            delete pOid;
        }
        catch(...)
        {
        }
        m_oidPtrList[idx] = NULL;
    }

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
TOidPtrList CMsgReqGet::getOidPtrList( bool i_bReleaseOwnership )
//------------------------------------------------------------------------------
{
    TOidPtrList oidPtrList = m_oidPtrList;

    if( i_bReleaseOwnership )
    {
        for( int idx = 0; idx < m_oidPtrList.count(); idx++ )
        {
            m_oidPtrList[idx] = NULL;
        }
        m_oidPtrList.clear();
    }
    return oidPtrList;

} // getOidPtrList

//------------------------------------------------------------------------------
int CMsgReqGet::getOidCount() const
//------------------------------------------------------------------------------
{
    return m_oidPtrList.count();
}

//------------------------------------------------------------------------------
COid* CMsgReqGet::getOid( int i_idx, bool i_bReleaseOwnerShip )
//------------------------------------------------------------------------------
{
    COid* pOid = NULL;

    if( i_idx < m_oidPtrList.count() )
    {
        pOid = m_oidPtrList[i_idx];

        if( i_bReleaseOwnerShip )
        {
            m_oidPtrList[i_idx] = NULL;
        }
    }
    return pOid;

} // getOid

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
ZS::CMsg* CMsgReqGet::clone()
//------------------------------------------------------------------------------
{
    TOidPtrList oidPtrList;

    COid* pOid;
    int   idx;

    for( idx = 0; idx < m_oidPtrList.count(); idx++ )
    {
        pOid = m_oidPtrList[idx];

        if( pOid != NULL )
        {
            oidPtrList.append( new COid(*pOid) );
        }
    }

    CMsgReqGet* pMsgCloned = new CMsgReqGet(
        /* iMsgType         */ getMsgType(),
        /* pObjSender       */ getSender(),
        /* pObjReceiver     */ getReceiver(),
        /* oidPtrList       */ oidPtrList,
        /* bMustBeConfirmed */ mustBeConfirmed(),
        /* iMsgId           */ getMsgId() );

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
QString CMsgReqGet::contentToStr( ZS::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    COid*   pOid;
    QString strContent;
    QString strTmp1;

    int idx;

    for( idx = 0; idx < m_oidPtrList.count(); idx++ )
    {
        pOid = m_oidPtrList[idx];

        strTmp1 = QString::number(idx);
        if( strTmp1.count() < 2 )
        {
            strTmp1.insert(0," ");
        }
        strContent += "[" + strTmp1 + "] ";

        if( pOid != NULL )
        {
            strTmp1 = "<" + pOid->toString(EOidFormatNumeric).toAscii() + ">";
        }
        else
        {
            strTmp1 = "<NO_OID>";
        }
        strContent += strTmp1;
        strContent += "\n";
    }
    return strContent;

} // contentToStr


/*******************************************************************************
class CMsgReqGetValue : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqGetValue::CMsgReqGetValue(
    QObject*    i_pObjSender,
    QObject*    i_pObjReceiver,
    const COid& i_oid,
    bool        i_bMustBeConfirmed,
    int         i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReqGet(
        /* iMsgType         */ EMsgTypeReqGetValue,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* oid              */ i_oid,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqGetValue::CMsgReqGetValue(
    QObject*           i_pObjSender,
    QObject*           i_pObjReceiver,
    const TOidPtrList& i_oidPtrList,
    bool               i_bMustBeConfirmed,
    int                i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReqGet(
        /* iMsgType         */ EMsgTypeReqGetValue,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* oidPtrList       */ i_oidPtrList,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqGetValue::~CMsgReqGetValue()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
ZS::CMsg* CMsgReqGetValue::clone()
//------------------------------------------------------------------------------
{
    TOidPtrList oidPtrList;

    COid* pOid;
    int   idx;

    for( idx = 0; idx < m_oidPtrList.count(); idx++ )
    {
        pOid = m_oidPtrList[idx];

        if( pOid != NULL )
        {
            oidPtrList.append( new COid(*pOid) );
        }
    }

    CMsgReqGetValue* pMsgCloned = new CMsgReqGetValue(
        /* pObjSender       */ getSender(),
        /* pObjReceiver     */ getReceiver(),
        /* oidPtrList       */ oidPtrList,
        /* bMustBeConfirmed */ mustBeConfirmed(),
        /* iMsgId           */ getMsgId() );

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
QString CMsgReqGetValue::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "GET";
}


/*******************************************************************************
class CMsgReqGetNextValue : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqGetNextValue::CMsgReqGetNextValue(
    QObject*    i_pObjSender,
    QObject*    i_pObjReceiver,
    const COid& i_oid,
    bool        i_bMustBeConfirmed,
    int         i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReqGet(
        /* iMsgType         */ EMsgTypeReqGetNextValue,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* oid              */ i_oid,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqGetNextValue::CMsgReqGetNextValue(
    QObject*           i_pObjSender,
    QObject*           i_pObjReceiver,
    const TOidPtrList& i_oidPtrList,
    bool               i_bMustBeConfirmed,
    int                i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReqGet(
        /* iMsgType         */ EMsgTypeReqGetNextValue,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* oidPtrList       */ i_oidPtrList,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqGetNextValue::~CMsgReqGetNextValue()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
ZS::CMsg* CMsgReqGetNextValue::clone()
//------------------------------------------------------------------------------
{
    TOidPtrList oidPtrList;

    COid* pOid;
    int   idx;

    for( idx = 0; idx < m_oidPtrList.count(); idx++ )
    {
        pOid = m_oidPtrList[idx];

        if( pOid != NULL )
        {
            oidPtrList.append( new COid(*pOid) );
        }
    }

    CMsgReqGetNextValue* pMsgCloned = new CMsgReqGetNextValue(
        /* pObjSender       */ getSender(),
        /* pObjReceiver     */ getReceiver(),
        /* oidPtrList       */ oidPtrList,
        /* bMustBeConfirmed */ mustBeConfirmed(),
        /* iMsgId           */ getMsgId() );

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
QString CMsgReqGetNextValue::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "GETNEXT";
}


/*******************************************************************************
class CMsgReqGetBulk : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqGetBulk::CMsgReqGetBulk(
    QObject*    i_pObjSender,
    QObject*    i_pObjReceiver,
    const COid& i_oid,
    bool        i_bMustBeConfirmed,
    int         i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReqGet(
        /* iMsgType         */ EMsgTypeReqGetBulk,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* oid              */ i_oid,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqGetBulk::CMsgReqGetBulk(
    QObject*           i_pObjSender,
    QObject*           i_pObjReceiver,
    const TOidPtrList& i_oidPtrList,
    bool               i_bMustBeConfirmed,
    int                i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReqGet(
        /* iMsgType         */ EMsgTypeReqGetBulk,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* oidPtrList       */ i_oidPtrList,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqGetBulk::~CMsgReqGetBulk()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
ZS::CMsg* CMsgReqGetBulk::clone()
//------------------------------------------------------------------------------
{
    TOidPtrList oidPtrList;

    COid* pOid;
    int   idx;

    for( idx = 0; idx < m_oidPtrList.count(); idx++ )
    {
        pOid = m_oidPtrList[idx];

        if( pOid != NULL )
        {
            oidPtrList.append( new COid(*pOid) );
        }
    }

    CMsgReqGetBulk* pMsgCloned = new CMsgReqGetBulk(
        /* pObjSender       */ getSender(),
        /* pObjReceiver     */ getReceiver(),
        /* oidPtrList       */ oidPtrList,
        /* bMustBeConfirmed */ mustBeConfirmed(),
        /* iMsgId           */ getMsgId() );

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
QString CMsgReqGetBulk::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "GETBULK";
}


/*******************************************************************************
class CMsgReqWalk : public CMsgReq
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqWalk::CMsgReqWalk(
    QObject*    i_pObjSender,
    QObject*    i_pObjReceiver,
    const COid& i_oid,
    bool        i_bMustBeConfirmed,
    int         i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqWalk,
        /* pObjSender       */ i_pObjSender,
        /* iSenderId        */ -1,
        /* pObjReceiver     */ i_pObjReceiver,
        /* iReceiverId      */ -1,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iMsgIdReq        */ i_iMsgId ),
    m_pOid( new COid(i_oid) )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqWalk::~CMsgReqWalk()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pOid;
    }
    catch(...)
    {
    }
    m_pOid = NULL;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
COid* CMsgReqWalk::getOid( bool i_bReleaseOwnerShip )
//------------------------------------------------------------------------------
{
    COid* pOid = m_pOid;

    if( i_bReleaseOwnerShip )
    {
        m_pOid = NULL;
    }
    return pOid;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
ZS::CMsg* CMsgReqWalk::clone()
//------------------------------------------------------------------------------
{
    CMsgReqWalk* pMsgCloned;

    if( m_pOid == NULL )
    {
        pMsgCloned = new CMsgReqWalk(
            /* pObjSender       */ getSender(),
            /* pObjReceiver     */ getReceiver(),
            /* oid              */ COid(*m_pOid),
            /* bMustBeConfirmed */ mustBeConfirmed(),
            /* iMsgId           */ getMsgId() );
    }
    else
    {
        pMsgCloned = new CMsgReqWalk(
            /* pObjSender       */ getSender(),
            /* pObjReceiver     */ getReceiver(),
            /* oid              */ COid(),
            /* bMustBeConfirmed */ mustBeConfirmed(),
            /* iMsgId           */ getMsgId() );
    }
    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
QString CMsgReqWalk::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "WALK";
}

//------------------------------------------------------------------------------
QString CMsgReqWalk::contentToStr( ZS::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strContent;
    QString strTmp1;

    if( m_pOid != NULL )
    {
        strContent = "<" + m_pOid->toString(EOidFormatNumeric).toAscii() + ">\n";
    }
    else
    {
        strContent = "<NO_OID>\n";
    }
    return strContent;

} // contentToStr


/*******************************************************************************
class CMsgConResponse : public CMsgCon
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConResponse::CMsgConResponse(
    QObject*                  i_pObjSender,
    QObject*                  i_pObjReceiver,
    const TValuePtrList&      i_valuePtrList,
    int                       i_iMsgIdReq,
    const ZS::SErrResultInfo& i_errResultInfo,
    int                       i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConResponse,
        /* pObjSender         */ i_pObjSender,
        /* iSenderId          */ -1,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReceiverId        */ -1,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_valuePtrList(i_valuePtrList)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConResponse::~CMsgConResponse()
//------------------------------------------------------------------------------
{
    CValue* pValue;
    int     idx;

    for( idx = 0; idx < m_valuePtrList.count(); idx++ )
    {
        pValue = m_valuePtrList[idx];

        try
        {
            delete pValue;
        }
        catch (...)
        {
        }
        m_valuePtrList[idx] = NULL;
    }

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
TValuePtrList CMsgConResponse::getValuePtrList( bool i_bReleaseOwnership )
//------------------------------------------------------------------------------
{
    TValuePtrList valuePtrList = m_valuePtrList;

    if( i_bReleaseOwnership )
    {
        for( int idx = 0; idx < m_valuePtrList.count(); idx++ )
        {
            m_valuePtrList[idx] = NULL;
        }
        m_valuePtrList.clear();
    }
    return valuePtrList;

} // getValuePtrList

//------------------------------------------------------------------------------
int CMsgConResponse::getValueCount() const
//------------------------------------------------------------------------------
{
    return m_valuePtrList.count();
}

//------------------------------------------------------------------------------
CValue* CMsgConResponse::getValue( int i_idx, bool i_bReleaseOwnerShip )
//------------------------------------------------------------------------------
{
    CValue* pValue = NULL;

    if( i_idx < m_valuePtrList.count() )
    {
        pValue = m_valuePtrList[i_idx];

        if( i_bReleaseOwnerShip )
        {
            m_valuePtrList[i_idx] = NULL;
        }
    }
    return pValue;

} // getValue

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
ZS::CMsg* CMsgConResponse::clone()
//------------------------------------------------------------------------------
{
    TValuePtrList valuePtrList;

    CValue* pValue;
    int     idx;

    for( idx = 0; idx < m_valuePtrList.count(); idx++ )
    {
        pValue = m_valuePtrList[idx];

        if( pValue != NULL )
        {
            valuePtrList.append( new CValue(*pValue) );
        }
    }

    CMsgConResponse* pMsgCloned = new CMsgConResponse(
        /* pObjSender    */ getSender(),
        /* pObjReceiver  */ getReceiver(),
        /* valuePtrList  */ valuePtrList,
        /* iMsgIdReq     */ getMsgId(),
        /* errResultInfo */ getErrResultInfo(),
        /* iProgress     */ getProgress() );

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
QString CMsgConResponse::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "RESPONSE";
}

//------------------------------------------------------------------------------
QString CMsgConResponse::contentToStr( ZS::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    CValue* pValue;
    COid*   pOid;
    QString strContent;
    QString strTmp1;
    QString strTmp2;

    if( m_errResultInfo.getResult() == EResultSuccess )
    {
        strContent += "Result: Success\n";
    }
    else
    {
        strContent += "Result:  " + m_errResultInfo.getResultStr() + "\n";
        strContent += "AddInfo: " + m_errResultInfo.getAddErrInfoDscr() + "\n";
    }

    int idx;

    for( idx = 0; idx < m_valuePtrList.count(); idx++ )
    {
        pValue = m_valuePtrList[idx];

        if( pValue != NULL )
        {
            pOid = pValue->getOid();

            strTmp1 = QString::number(idx);
            if( strTmp1.count() < 2 )
            {
                strTmp1.insert(0," ");
            }
            strContent += "[" + strTmp1 + "] ";

            if( pOid != NULL )
            {
                strTmp1 = "<" + pOid->toString(EOidFormatNumeric).toAscii() + ">";
            }
            else
            {
                strTmp1 = "<NO_OID>";
            }
            if( m_valuePtrList.count() > 1 && strTmp1.count() < 32 )
            {
                strTmp2.fill(' ',32-strTmp1.count());
                strTmp1.append(strTmp2);
            }
            strContent += strTmp1 + " ";

            strTmp1 = "<" + valueType2Str(pValue->getValueType()) + ">";
            if( m_valuePtrList.count() > 1 && strTmp1.count() < 13 )
            {
                strTmp2.fill(' ',13-strTmp1.count());
                strTmp1.append(strTmp2);
            }
            strContent += strTmp1 + " ";

            strContent += pValue->getValueString();
        }
        strContent += "\n";
    }
    return strContent;

} // contentToStr


/*******************************************************************************
class CMsgIndTimeout : public CMsgInd
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgIndTimeout::CMsgIndTimeout(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver ) :
//------------------------------------------------------------------------------
    CMsgInd(
        /* iMsgType         */ EMsgTypeIndTimeout,
        /* pObjSender       */ i_pObjSender,
        /* iSenderId        */ -1,
        /* pObjReceiver     */ i_pObjReceiver,
        /* iReceiverId      */ -1,
        /* bMustBeConfirmed */ false,
        /* iMsgIdReq        */ -1 )
{
} // ctor

//------------------------------------------------------------------------------
CMsgIndTimeout::~CMsgIndTimeout()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
ZS::CMsg* CMsgIndTimeout::clone()
//------------------------------------------------------------------------------
{
    CMsgIndTimeout* pMsgCloned = new CMsgIndTimeout(
        /* pObjSender   */ getSender(),
        /* pObjReceiver */ getReceiver() );
    return pMsgCloned;
}

//------------------------------------------------------------------------------
QString CMsgIndTimeout::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "SNMP::IndTimeout";
}
