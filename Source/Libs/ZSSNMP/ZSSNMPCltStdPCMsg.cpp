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
#include "ZSSNMP/Include/ZSSNMPCltStdPCMsg.h"
#include "ZSSys/Include/ZSSysEnum.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;


/*******************************************************************************
class CMsgReqOpenSession : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqCltStdPCStartSession::CMsgReqCltStdPCStartSession(
    QObject*          i_pObjSender,
    QObject*          i_pObjReceiver,
    const SOidsStdPC& i_oids,
    bool              i_bMustBeConfirmed,
    int               i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeCltStdPCReqStartSession,
        /* pObjSender       */ i_pObjSender,
        /* iSenderId        */ -1,
        /* pObjReceiver     */ i_pObjReceiver,
        /* iReceiverId      */ -1,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iMsgIdReq        */ i_iMsgId ),
    m_oids(i_oids)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqCltStdPCStartSession::~CMsgReqCltStdPCStartSession()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SOidsStdPC CMsgReqCltStdPCStartSession::getOids() const
//------------------------------------------------------------------------------
{
    return m_oids;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
ZS::CMsg* CMsgReqCltStdPCStartSession::clone()
//------------------------------------------------------------------------------
{
    CMsgReqCltStdPCStartSession* pMsgCloned = new CMsgReqCltStdPCStartSession(
        /* pObjSender       */ getSender(),
        /* pObjReceiver     */ getReceiver(),
        /* oids             */ getOids(),
        /* bMustBeConfirmed */ mustBeConfirmed(),
        /* iMsgId           */ getMsgId() );
    return pMsgCloned;
}

//------------------------------------------------------------------------------
QString CMsgReqCltStdPCStartSession::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "REQ::OPENSESSION";
}

//------------------------------------------------------------------------------
QString CMsgReqCltStdPCStartSession::contentToStr( ZS::EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strContent;
    strContent += "System:    " + m_oids.m_oidSystem.toString() + "\n";
    strContent += "SysDescr:  " + m_oids.m_oidSysDescr.toString() + "\n";
    strContent += "SysName:   " + m_oids.m_oidSysName.toString() + "\n";
    strContent += "ProcLoad:  " + m_oids.m_oidHrProcessorLoad.toString() + "\n";
    strContent += "StorType:  " + m_oids.m_oidHrStorageType.toString() + "\n";
    strContent += "StorDescr: " + m_oids.m_oidHrStorageDescr.toString() + "\n";
    strContent += "StorUnits: " + m_oids.m_oidHrStorageAllocationUnits.toString() + "\n";
    strContent += "StorSize:  " + m_oids.m_oidHrStorageSize.toString() + "\n";
    strContent += "StorUsed:  " + m_oids.m_oidHrStorageUsed.toString() + "\n";
    return strContent;
}
