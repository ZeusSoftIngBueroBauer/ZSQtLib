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

#include "MsgTest.h"

#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Apps::Test::RemCmdServer;


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
    qint64   i_iParentReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqStartup,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iParentReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqStartup::~CMsgReqStartup()
//------------------------------------------------------------------------------
{
} // dtor

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
    return "ZS::Apps::Test::RemCmdServer::ReqStartup";
}

//------------------------------------------------------------------------------
QString CMsgReqStartup::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString szContent;
    QString szTblBeg;
    QString szTblEnd;
    QString szRowBeg;
    QString szRowEnd;
    QString szClmBeg;
    QString szClmEnd;

    if( i_format == EContentToStrFormat::HtmlText )
    {
        szTblBeg = "<table>";
        szTblEnd = "</table>";
        szRowBeg = "<tr>";
        szRowEnd = "</tr>";
        szClmBeg = "<td>";
        szClmEnd = "</td>";
    }
    else
    {
        szRowEnd = "\n";
        szClmEnd = " ";
    }
    szContent += szTblBeg;
    szContent += szRowBeg;
    szContent += szRowEnd;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Must be confirmed: ";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += bool2Str(m_bMustBeConfirmed);
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szTblEnd;
    return szContent;

} // contentToByteArray


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
    qint64                i_iParentReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConStartup,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iParentReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConStartup::~CMsgConStartup()
//------------------------------------------------------------------------------
{
} // dtor

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

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConStartup::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ZS::Apps::Test::RemCmdServer::ConStartup";
}

//------------------------------------------------------------------------------
QString CMsgConStartup::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString szContent;
    QString szTblBeg;
    QString szTblEnd;
    QString szRowBeg;
    QString szRowEnd;
    QString szClmBeg;
    QString szClmEnd;

    if( i_format == EContentToStrFormat::HtmlText )
    {
        szTblBeg = "<table>";
        szTblEnd = "</table>";
        szRowBeg = "<tr>";
        szRowEnd = "</tr>";
        szClmBeg = "<td>";
        szClmEnd = "</td>";
    }
    else
    {
        szRowEnd = "\n";
        szClmEnd = " ";
    }
    szContent += szTblBeg;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Result:";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += getErrResultInfo().getResultStr();
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Progress:";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += QString::number(m_iProgressInPerCent) + "[%]";
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szTblEnd;
    return szContent;

} // contentToByteArray


/*******************************************************************************
class CMsgReqTest : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqTest::CMsgReqTest(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iParentReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqTest,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iParentReqId,
        /* iMsgIdReq        */ i_iMsgId )
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqTest::~CMsgReqTest()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqTest::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqTest* pMsgCloned = new CMsgReqTest(
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
CMsgCon* CMsgReqTest::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqTest::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ZS::Apps::Test::RemCmdServer::ReqTest";
}

//------------------------------------------------------------------------------
QString CMsgReqTest::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString szContent;
    QString szTblBeg;
    QString szTblEnd;
    QString szRowBeg;
    QString szRowEnd;
    QString szClmBeg;
    QString szClmEnd;

    if( i_format == EContentToStrFormat::HtmlText )
    {
        szTblBeg = "<table>";
        szTblEnd = "</table>";
        szRowBeg = "<tr>";
        szRowEnd = "</tr>";
        szClmBeg = "<td>";
        szClmEnd = "</td>";
    }
    else
    {
        szRowEnd = "\n";
        szClmEnd = " ";
    }
    szContent += szTblBeg;
    szContent += szRowBeg;
    szContent += szRowEnd;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Must be confirmed: ";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += bool2Str(m_bMustBeConfirmed);
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szTblEnd;
    return szContent;

} // contentToByteArray


/*******************************************************************************
class CMsgConTest : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConTest::CMsgConTest(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iParentReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConTest,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iParentReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent )
{
} // ctor

//------------------------------------------------------------------------------
CMsgConTest::~CMsgConTest()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConTest::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConTest* pMsgCloned = new CMsgConTest(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* iReqId             */ m_iReqId,
        /* iMsgId             */ m_iMsgId,
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
QString CMsgConTest::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ZS::Apps::Test::RemCmdServer::ConTest";
}

//------------------------------------------------------------------------------
QString CMsgConTest::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString szContent;
    QString szTblBeg;
    QString szTblEnd;
    QString szRowBeg;
    QString szRowEnd;
    QString szClmBeg;
    QString szClmEnd;

    if( i_format == EContentToStrFormat::HtmlText )
    {
        szTblBeg = "<table>";
        szTblEnd = "</table>";
        szRowBeg = "<tr>";
        szRowEnd = "</tr>";
        szClmBeg = "<td>";
        szClmEnd = "</td>";
    }
    else
    {
        szRowEnd = "\n";
        szClmEnd = " ";
    }
    szContent += szTblBeg;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Result:";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += getErrResultInfo().getResultStr();
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Progress:";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += QString::number(m_iProgressInPerCent) + "[%]";
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szTblEnd;
    return szContent;

} // contentToByteArray


/*******************************************************************************
class CMsgReqDbParSetVal : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqDbParSetVal::CMsgReqDbParSetVal(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const QString&  i_strDbPar,
    const QVariant& i_val,
    bool            i_bMustBeConfirmed,
    qint64          i_iParentReqId,
    qint64          i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqDbParSetVal,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iParentReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_strDbPar(i_strDbPar),
    m_val(i_val)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqDbParSetVal::~CMsgReqDbParSetVal()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqDbParSetVal::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqDbParSetVal* pMsgCloned = new CMsgReqDbParSetVal(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* strDbPar         */ m_strDbPar,
        /* val              */ m_val,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqDbParSetVal::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqDbParSetVal::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ZS::Apps::Test::RemCmdServer::ReqDbParSetVal";
}

//------------------------------------------------------------------------------
QString CMsgReqDbParSetVal::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString szContent;
    QString szTblBeg;
    QString szTblEnd;
    QString szRowBeg;
    QString szRowEnd;
    QString szClmBeg;
    QString szClmEnd;

    if( i_format == EContentToStrFormat::HtmlText )
    {
        szTblBeg = "<table>";
        szTblEnd = "</table>";
        szRowBeg = "<tr>";
        szRowEnd = "</tr>";
        szClmBeg = "<td>";
        szClmEnd = "</td>";
    }
    else
    {
        szRowEnd = "\n";
        szClmEnd = " ";
    }
    szContent += szTblBeg;
    szContent += szRowBeg;
    szContent += szRowEnd;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Must be confirmed: ";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += bool2Str(m_bMustBeConfirmed);
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szTblEnd;
    return szContent;

} // contentToByteArray


/*******************************************************************************
class CMsgConDbParSetVal : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConDbParSetVal::CMsgConDbParSetVal(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const QString&        i_strDbPar,
    const QVariant&       i_val,
    qint64                i_iParentReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConDbParSetVal,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iParentReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_strDbPar(i_strDbPar),
    m_val(i_val)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConDbParSetVal::~CMsgConDbParSetVal()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConDbParSetVal::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConDbParSetVal* pMsgCloned = new CMsgConDbParSetVal(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* strDbPar           */ m_strDbPar,
        /* val                */ m_val,
        /* iReqId             */ m_iReqId,
        /* iMsgId             */ m_iMsgId,
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
QString CMsgConDbParSetVal::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ZS::Apps::Test::RemCmdServer::ConDbParSetVal";
}

//------------------------------------------------------------------------------
QString CMsgConDbParSetVal::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString szContent;
    QString szTblBeg;
    QString szTblEnd;
    QString szRowBeg;
    QString szRowEnd;
    QString szClmBeg;
    QString szClmEnd;

    if( i_format == EContentToStrFormat::HtmlText )
    {
        szTblBeg = "<table>";
        szTblEnd = "</table>";
        szRowBeg = "<tr>";
        szRowEnd = "</tr>";
        szClmBeg = "<td>";
        szClmEnd = "</td>";
    }
    else
    {
        szRowEnd = "\n";
        szClmEnd = " ";
    }
    szContent += szTblBeg;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Result:";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += getErrResultInfo().getResultStr();
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Progress:";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += QString::number(m_iProgressInPerCent) + "[%]";
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szTblEnd;
    return szContent;

} // contentToByteArray


/*******************************************************************************
class CMsgReqDbParGetVal : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqDbParGetVal::CMsgReqDbParGetVal(
    QObject*       i_pObjSender,
    QObject*       i_pObjReceiver,
    const QString& i_strDbPar,
    qint64         i_iParentReqId,
    qint64         i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EMsgTypeReqDbParGetVal,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ true,
        /* iReqId           */ i_iParentReqId,
        /* iMsgIdReq        */ i_iMsgId ),
    m_strDbPar(i_strDbPar)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqDbParGetVal::~CMsgReqDbParGetVal()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqDbParGetVal::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgReqDbParGetVal* pMsgCloned = new CMsgReqDbParGetVal(
        /* pObjSender   */ m_pObjSender,
        /* pObjReceiver */ m_pObjReceiver,
        /* strDbPar     */ m_strDbPar,
        /* iReqId       */ m_iReqId,
        /* iMsgId       */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqDbParGetVal::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqDbParGetVal::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ZS::Apps::Test::RemCmdServer::ReqDbParGetVal";
}

//------------------------------------------------------------------------------
QString CMsgReqDbParGetVal::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString szContent;
    QString szTblBeg;
    QString szTblEnd;
    QString szRowBeg;
    QString szRowEnd;
    QString szClmBeg;
    QString szClmEnd;

    if( i_format == EContentToStrFormat::HtmlText )
    {
        szTblBeg = "<table>";
        szTblEnd = "</table>";
        szRowBeg = "<tr>";
        szRowEnd = "</tr>";
        szClmBeg = "<td>";
        szClmEnd = "</td>";
    }
    else
    {
        szRowEnd = "\n";
        szClmEnd = " ";
    }
    szContent += szTblBeg;
    szContent += szRowBeg;
    szContent += szRowEnd;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Must be confirmed: ";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += bool2Str(m_bMustBeConfirmed);
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szTblEnd;
    return szContent;

} // contentToByteArray


/*******************************************************************************
class CMsgConDbParGetVal : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConDbParGetVal::CMsgConDbParGetVal(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const QString&        i_strDbPar,
    const QVariant&       i_val,
    qint64                i_iParentReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType           */ EMsgTypeConDbParGetVal,
        /* pObjSender         */ i_pObjSender,
        /* pObjReceiver       */ i_pObjReceiver,
        /* iReqId             */ i_iParentReqId,
        /* iMsgIdReq          */ i_iMsgIdReq,
        /* errResultInfo      */ i_errResultInfo,
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_strDbPar(i_strDbPar),
    m_val(i_val)
{
} // ctor

//------------------------------------------------------------------------------
CMsgConDbParGetVal::~CMsgConDbParGetVal()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConDbParGetVal::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConDbParGetVal* pMsgCloned = new CMsgConDbParGetVal(
        /* pObjSender         */ m_pObjSender,
        /* pObjReceiver       */ m_pObjReceiver,
        /* strDbPar           */ m_strDbPar,
        /* val                */ m_val,
        /* iReqId             */ m_iReqId,
        /* iMsgId             */ m_iMsgId,
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
QString CMsgConDbParGetVal::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ZS::Apps::Test::RemCmdServer::ConDbParGetVal";
}

//------------------------------------------------------------------------------
QString CMsgConDbParGetVal::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString szContent;
    QString szTblBeg;
    QString szTblEnd;
    QString szRowBeg;
    QString szRowEnd;
    QString szClmBeg;
    QString szClmEnd;

    if( i_format == EContentToStrFormat::HtmlText )
    {
        szTblBeg = "<table>";
        szTblEnd = "</table>";
        szRowBeg = "<tr>";
        szRowEnd = "</tr>";
        szClmBeg = "<td>";
        szClmEnd = "</td>";
    }
    else
    {
        szRowEnd = "\n";
        szClmEnd = " ";
    }
    szContent += szTblBeg;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Result:";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += getErrResultInfo().getResultStr();
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Progress:";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += QString::number(m_iProgressInPerCent) + "[%]";
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szTblEnd;
    return szContent;

} // contentToByteArray


/*******************************************************************************
class CMsgIndDbParValChanged : public CMsgInd
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgIndDbParValChanged::CMsgIndDbParValChanged(
    QObject*        i_pObjSender,
    QObject*        i_pObjReceiver,
    const QString&  i_strDbPar,
    const QVariant& i_val,
    qint64          i_iMsgIdReq ) :
//------------------------------------------------------------------------------
    CMsgInd(
        /* iMsgType     */ EMsgTypeIndDbParValChanged,
        /* pObjSender   */ i_pObjSender,
        /* pObjReceiver */ i_pObjReceiver,
        /* iMsgIdReq    */ i_iMsgIdReq ),
    m_strDbPar(i_strDbPar),
    m_val(i_val)
{
} // ctor

//------------------------------------------------------------------------------
CMsgIndDbParValChanged::~CMsgIndDbParValChanged()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgIndDbParValChanged::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgIndDbParValChanged* pMsgCloned = new CMsgIndDbParValChanged(
        /* pObjSender   */ m_pObjSender,
        /* pObjReceiver */ m_pObjReceiver,
        /* strDbPar     */ m_strDbPar,
        /* val          */ m_val,
        /* iMsgIdReq    */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgAck* CMsgIndDbParValChanged::createAcknowledgeMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgIndDbParValChanged::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ZS::Apps::Test::RemCmdServer::IndDbParValChanged";
}

//------------------------------------------------------------------------------
QString CMsgIndDbParValChanged::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString szContent;
    QString szTblBeg;
    QString szTblEnd;
    QString szRowBeg;
    QString szRowEnd;
    QString szClmBeg;
    QString szClmEnd;

    if( i_format == EContentToStrFormat::HtmlText )
    {
        szTblBeg = "<table>";
        szTblEnd = "</table>";
        szRowBeg = "<tr>";
        szRowEnd = "</tr>";
        szClmBeg = "<td>";
        szClmEnd = "</td>";
    }
    else
    {
        szRowEnd = "\n";
        szClmEnd = " ";
    }
    szContent += szTblBeg;
    szContent += szRowBeg;
    szContent += szClmBeg;
    szContent += "Must be acknowledged: ";
    szContent += szClmEnd;
    szContent += szClmBeg;
    szContent += bool2Str(m_bMustBeAcknowledged);
    szContent += szClmEnd;
    szContent += szRowEnd;
    szContent += szTblEnd;
    return szContent;

} // contentToByteArray
