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

*******************************************************************************/

#include <QtCore/qglobal.h>
#if QT_VERSION < 0x040100
#include <QtCore/qdeepcopy.h>
#endif
#include "ZSDiagram/ZSDiagramMsg.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Diagram;


/*******************************************************************************
class ZSDIAGRAMDLL_API CMsgReqShow : public ZS::System::CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqShow::CMsgReqShow(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iParentReqId,
    qint64   /*i_iMsgId*/ ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqShow,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iParentReqId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqShow::~CMsgReqShow()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqShow::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqShow* pMsgCloned = new CMsgReqShow(
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
CMsgCon* CMsgReqShow::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    CMsgConShow* pMsgCon = new CMsgConShow(
        /* pObjSender    */ m_pObjReceiver,
        /* pObjReceiver  */ m_pObjSender,
        /* iTimerId      */ m_iReqId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ SErrResultInfo() );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage


/*******************************************************************************
class ZSDIAGRAMDLL_API CMsgConShow : public ZS::System::CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConShow::CMsgConShow(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iParentReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConShow,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iParentReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ 100 )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConShow::~CMsgConShow()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConShow::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConShow* pMsgCloned = new CMsgConShow(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
        /* iReqId        */ m_iReqId,
        /* iMsgId        */ m_iMsgId,
        /* errResultInfo */ m_errResultInfo );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone


/*******************************************************************************
class ZSDIAGRAMDLL_API CMsgReqHide : public ZS::System::CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqHide::CMsgReqHide(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iParentReqId,
    qint64   /*i_iMsgId*/ ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqHide,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iParentReqId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqHide::~CMsgReqHide()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqHide::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqHide* pMsgCloned = new CMsgReqHide(
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
CMsgCon* CMsgReqHide::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    CMsgConHide* pMsgCon = new CMsgConHide(
        /* pObjSender    */ m_pObjReceiver,
        /* pObjReceiver  */ m_pObjSender,
        /* iTimerId      */ m_iReqId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ SErrResultInfo() );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage


/*******************************************************************************
class ZSDIAGRAMDLL_API CMsgConHide : public ZS::System::CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConHide::CMsgConHide(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iParentReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConHide,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iParentReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ 100 )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConHide::~CMsgConHide()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConHide::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConHide* pMsgCloned = new CMsgConHide(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
        /* iReqId        */ m_iReqId,
        /* iMsgId        */ m_iMsgId,
        /* errResultInfo */ m_errResultInfo );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone


/*******************************************************************************
class ZSDIAGRAMDLL_API CMsgReqSetValue : public ZS::System::CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqSetValue::CMsgReqSetValue(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iParentReqId,
    qint64   /*i_iMsgId*/ ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqSetValue,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iParentReqId ),
    m_msgValType(EMsgValTypeNone),
    m_iVal(0),
    m_fVal(0.0),
    m_strVal(),
    m_physVal()
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSetValue::CMsgReqSetValue(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    int      i_iVal,
    bool     i_bMustBeConfirmed,
    qint64   i_iParentReqId,
    qint64   /*i_iMsgId*/ ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqSetValue,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iParentReqId ),
    m_msgValType(EMsgValTypeInt),
    m_iVal(i_iVal),
    m_fVal(0.0),
    m_strVal(),
    m_physVal()
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSetValue::CMsgReqSetValue(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    double   i_fVal,
    bool     i_bMustBeConfirmed,
    qint64   i_iParentReqId,
    qint64   /*i_iMsgId*/ ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqSetValue,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iParentReqId ),
    m_msgValType(EMsgValTypeDouble),
    m_iVal(0),
    m_fVal(i_fVal),
    m_strVal(),
    m_physVal()
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSetValue::CMsgReqSetValue(
    QObject*       i_pObjSender,
    QObject*       i_pObjReceiver,
    const QString& i_strVal,
    bool           i_bMustBeConfirmed,
    qint64         i_iParentReqId,
    qint64         /*i_iMsgId*/ ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqSetValue,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iParentReqId ),
    m_msgValType(EMsgValTypeString),
    m_iVal(0),
    m_fVal(0.0),
    #if QT_VERSION >= 0x040100
    m_strVal(i_strVal),
    #else
    m_strVal( QDeepCopy<QString>(i_strVal) ),
    #endif
    m_physVal()
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSetValue::CMsgReqSetValue(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const CPhysVal& i_physVal,
    bool            i_bMustBeConfirmed,
    qint64          i_iParentReqId,
    qint64          /*i_iMsgId*/ ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqSetValue,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iParentReqId ),
    m_msgValType(EMsgValTypePhysVal),
    m_iVal(0),
    m_fVal(0.0),
    m_strVal(),
    m_physVal(i_physVal)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqSetValue::~CMsgReqSetValue()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqSetValue::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqSetValue* pMsgCloned = new CMsgReqSetValue(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    if( m_msgValType & EMsgValTypeInt )
    {
        pMsgCloned->setVal(m_iVal);
    }
    if( m_msgValType & EMsgValTypeDouble )
    {
        pMsgCloned->setVal(m_fVal);
    }
    if( m_msgValType & EMsgValTypeString )
    {
        pMsgCloned->setVal(m_strVal);
    }
    if( m_msgValType & EMsgValTypePhysVal )
    {
        pMsgCloned->setVal(m_physVal);
    }
    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqSetValue::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    CMsgConSetValue* pMsgCon = new CMsgConSetValue(
        /* pObjSender    */ m_pObjReceiver,
        /* pObjReceiver  */ m_pObjSender,
        /* iTimerId      */ m_iReqId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ SErrResultInfo() );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CMsgReqSetValue::isValid( EMsgValType i_valType ) const
//------------------------------------------------------------------------------
{
    return (m_msgValType & i_valType);
}

//------------------------------------------------------------------------------
void CMsgReqSetValue::setVal( int i_iVal )
//------------------------------------------------------------------------------
{
    m_iVal = i_iVal;
    m_msgValType |= EMsgValTypeInt;
}

//------------------------------------------------------------------------------
int CMsgReqSetValue::getValInt() const
//------------------------------------------------------------------------------
{
    return m_iVal;
}

//------------------------------------------------------------------------------
void CMsgReqSetValue::setVal( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fVal = i_fVal;
    m_msgValType |= EMsgValTypeDouble;
}

//------------------------------------------------------------------------------
double CMsgReqSetValue::getValDouble() const
//------------------------------------------------------------------------------
{
    return m_fVal;
}

//------------------------------------------------------------------------------
void CMsgReqSetValue::setVal( const QString& i_strVal )
//------------------------------------------------------------------------------
{
    #if QT_VERSION >= 0x040100
    m_strVal = i_strVal;
    #else
    m_strVal = QDeepCopy<QString>(i_strVal);
    #endif
    m_msgValType |= EMsgValTypeString;
}

//------------------------------------------------------------------------------
QString CMsgReqSetValue::getValStr() const
//------------------------------------------------------------------------------
{
    return m_strVal;
}

//------------------------------------------------------------------------------
void CMsgReqSetValue::setVal( const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    m_physVal = i_physVal;
    m_msgValType |= EMsgValTypePhysVal;
}

//------------------------------------------------------------------------------
CPhysVal CMsgReqSetValue::getValPhys() const
//------------------------------------------------------------------------------
{
    return m_physVal;
}


/*******************************************************************************
class ZSDIAGRAMDLL_API CMsgConSetValue : public ZS::System::CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConSetValue::CMsgConSetValue(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iParentReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConSetValue,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iParentReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ 100 )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConSetValue::~CMsgConSetValue()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConSetValue::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConSetValue* pMsgCloned = new CMsgConSetValue(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
        /* iReqId        */ m_iReqId,
        /* iMsgId        */ m_iMsgId,
        /* errResultInfo */ m_errResultInfo );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone
