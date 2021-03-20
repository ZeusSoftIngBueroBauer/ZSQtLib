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

#ifndef ZSSNPMMsg_h
#define ZSSNPMMsg_h

#include <QtCore/qvariant.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSNMP/Include/ZSSNMPOid.h"
#include "ZSSNMP/Include/ZSSNMPExplorer.h"
#include "ZSSys/Include/ZSSysMsg.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace SNMP
{
typedef enum
{
    EMsgTypeMin             = (QEvent::User +  1),
    // Message for communication between threads:
    EMsgTypeReqOpenSession  = (EMsgTypeMin  +  1),
    EMsgTypeConOpenSession  = (EMsgTypeMin  +  2),
    EMsgTypeReqCloseSession = (EMsgTypeMin  +  3),
    EMsgTypeConCloseSession = (EMsgTypeMin  +  4),
    EMsgTypeReqGetValue     = (EMsgTypeMin  +  5),
    EMsgTypeReqGetNextValue = (EMsgTypeMin  +  6),
    EMsgTypeReqGetBulk      = (EMsgTypeMin  +  7),
    EMsgTypeReqWalk         = (EMsgTypeMin  +  8),
    EMsgTypeConResponse     = (EMsgTypeMin  +  9),
    // Messages for internal state machines:
    EMsgTypeIndTimeout      = (EMsgTypeMin  + 10),
    EMsgTypeMax             = (EMsgTypeMin  + 10),
    EMsgTypeCount           = (EMsgTypeMax-EMsgTypeMin+1),
    EMsgTypeUndefined
}   EMsgType;

ZSSNMPDLL_API QString msgType2Str( int i_iMsgType );
ZSSNMPDLL_API QString msgType2Str( ZS::System::CMsg* i_pMsg );

//******************************************************************************
class ZSSNMPDLL_API CMsgReqOpenSession : public CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqOpenSession(
        QObject*             i_pObjSender,
        QObject*             i_pObjReceiver,
        const SHostSettings& i_hostSettings,
        bool                 i_bMustBeConfirmed = true,
        int                  i_iMsgId = -1 );
    virtual ~CMsgReqOpenSession();
public: // instance methods
    SHostSettings getHostSettings() const;
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone();
    virtual QString msgTypeToStr() const;
    virtual QString contentToStr( EContentToStrFormat i_format = EContentToStrFormatPlainText );
protected: // instance members
    SHostSettings m_hostSettings;

}; // CMsgReqOpenSession

//******************************************************************************
class ZSSNMPDLL_API CMsgConOpenSession : public CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConOpenSession(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        int                   i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                   i_iProgressInPerCent = 100.0 );
    virtual ~CMsgConOpenSession();
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone();
    virtual QString msgTypeToStr() const;
    virtual QString contentToStr( EContentToStrFormat i_format = EContentToStrFormatPlainText );

}; // CMsgConOpenSession

//******************************************************************************
class ZSSNMPDLL_API CMsgReqCloseSession : public CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqCloseSession(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = true,
        int      i_iMsgId = -1 );
    virtual ~CMsgReqCloseSession();
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone();
    virtual QString msgTypeToStr() const;

}; // CMsgReqCloseSession

//******************************************************************************
class ZSSNMPDLL_API CMsgConCloseSession : public CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConCloseSession(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        int                   i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                   i_iProgressInPerCent = 100.0 );
    virtual ~CMsgConCloseSession();
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone();
    virtual QString msgTypeToStr() const;
    virtual QString contentToStr( EContentToStrFormat i_format = EContentToStrFormatPlainText );

}; // CMsgConCloseSession

//******************************************************************************
class ZSSNMPDLL_API CMsgReqGet : public CMsgReq
//******************************************************************************
{
protected: // ctors and dtor
    CMsgReqGet(
        int         i_iMsgType,
        QObject*    i_pObjSender,
        QObject*    i_pObjReceiver,
        const COid& i_oid,
        bool        i_bMustBeConfirmed = true,
        int         i_iMsgId = -1 );
    CMsgReqGet(
        int                i_iMsgType,
        QObject*           i_pObjSender,
        QObject*           i_pObjReceiver,
        const TOidPtrList& i_oidPtrList, // the message takes ownership of the COid instances
        bool               i_bMustBeConfirmed = true,
        int                i_iMsgId = -1 );
    virtual ~CMsgReqGet();
public: // instance methods
    TOidPtrList getOidPtrList( bool i_bReleaseOwnership = true );
    int getOidCount() const;
    COid* getOid( int i_idx, bool i_bReleaseOwnerShip = true );
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone();
    virtual QString contentToStr( EContentToStrFormat i_format = EContentToStrFormatPlainText );
protected: // instance members
    TOidPtrList m_oidPtrList;

}; // CMsgReqGet

//******************************************************************************
class ZSSNMPDLL_API CMsgReqGetValue : public CMsgReqGet
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqGetValue(
        QObject*    i_pObjSender,
        QObject*    i_pObjReceiver,
        const COid& i_oid,
        bool        i_bMustBeConfirmed = true,
        int         i_iMsgId = -1 );
    CMsgReqGetValue(
        QObject*           i_pObjSender,
        QObject*           i_pObjReceiver,
        const TOidPtrList& i_oidPtrList, // the message takes ownership of the COid instances
        bool               i_bMustBeConfirmed = true,
        int                i_iMsgId = -1 );
    virtual ~CMsgReqGetValue();
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone();
    virtual QString msgTypeToStr() const;

}; // CMsgReqGetValue

//******************************************************************************
class ZSSNMPDLL_API CMsgReqGetNextValue : public CMsgReqGet
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqGetNextValue(
        QObject*    i_pObjSender,
        QObject*    i_pObjReceiver,
        const COid& i_oid,
        bool        i_bMustBeConfirmed = true,
        int         i_iMsgId = -1 );
    CMsgReqGetNextValue(
        QObject*           i_pObjSender,
        QObject*           i_pObjReceiver,
        const TOidPtrList& i_oidPtrList, // the message takes ownership of the COid instances
        bool               i_bMustBeConfirmed = true,
        int                i_iMsgId = -1 );
    virtual ~CMsgReqGetNextValue();
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone();
    virtual QString msgTypeToStr() const;

}; // CMsgReqGetNextValue

//******************************************************************************
class ZSSNMPDLL_API CMsgReqGetBulk : public CMsgReqGet
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqGetBulk(
        QObject*    i_pObjSender,
        QObject*    i_pObjReceiver,
        const COid& i_oid,
        bool        i_bMustBeConfirmed = true,
        int         i_iMsgId = -1 );
    CMsgReqGetBulk(
        QObject*           i_pObjSender,
        QObject*           i_pObjReceiver,
        const TOidPtrList& i_oidPtrList, // the message takes ownership of the COid instances
        bool               i_bMustBeConfirmed = true,
        int                i_iMsgId = -1 );
    virtual ~CMsgReqGetBulk();
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone();
    virtual QString msgTypeToStr() const;

}; // CMsgReqGetBulk

//******************************************************************************
class ZSSNMPDLL_API CMsgReqWalk : public CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqWalk(
        QObject*    i_pObjSender,
        QObject*    i_pObjReceiver,
        const COid& i_oid,
        bool        i_bMustBeConfirmed = true,
        int         i_iMsgId = -1 );
    virtual ~CMsgReqWalk();
public: // instance methods
    COid* getOid( bool i_bReleaseOwnerShip = true );
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone();
    virtual QString msgTypeToStr() const;
    virtual QString contentToStr( EContentToStrFormat i_format = EContentToStrFormatPlainText );
protected: // instance members
    COid* m_pOid;

}; // CMsgReqWalk

//******************************************************************************
class ZSSNMPDLL_API CMsgConResponse : public CMsgCon
//******************************************************************************
{
public: // ctors and dtor
    CMsgConResponse(
        QObject*              i_pObjSender,
        QObject*              i_pObjReceiver,
        const TValuePtrList&  i_valuePtrList, // the message takes ownership of the CValue instances
        int                   i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ZS::System::SErrResultInfo(),
        int                   i_iProgressInPerCent = 100.0 );
    virtual ~CMsgConResponse();
public: // instance methods
    TValuePtrList getValuePtrList( bool i_bReleaseOwnership = true );
    int getValueCount() const;
    CValue* getValue( int i_idx, bool i_bReleaseOwnerShip = true );
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone();
    virtual QString msgTypeToStr() const;
    virtual QString contentToStr( EContentToStrFormat i_format = EContentToStrFormatPlainText );
protected: // instance members
    TValuePtrList m_valuePtrList;

}; // CMsgConResponse

//******************************************************************************
class ZSSNMPDLL_API CMsgIndTimeout : public CMsgInd
//******************************************************************************
{
public: // ctors and dtor
    CMsgIndTimeout(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver );
    virtual ~CMsgIndTimeout();
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone();
    virtual QString msgTypeToStr() const;

}; // CMsgIndTimeout

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNPMMsg_h
