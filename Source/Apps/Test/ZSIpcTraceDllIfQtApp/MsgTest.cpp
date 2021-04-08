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

#include "MsgTest.h"

#include "ZSIpcTrace/ZSIpcTrcDllIf.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace::DllIf;
using namespace ZS::Apps::Test::IpcTraceDllIfQtApp;


/*******************************************************************************
Global Methods
*******************************************************************************/

//------------------------------------------------------------------------------
void ZS::Apps::Test::IpcTraceDllIfQtApp::POST_OR_DELETE_MESSAGE(
    QEvent*                          i_pMsg,
    ZS::Trace::DllIf::CMethodTracer* i_pMethodTracer,
    int                              i_iFilterDetailLevel )
//------------------------------------------------------------------------------
{
    if( i_pMsg != nullptr )
    {
        QString strAddTrcInfo;
        QString strMsgTrcInfo;

        CMsg* pMsg = dynamic_cast<CMsg*>(i_pMsg);

        QObject* pObjReceiver = nullptr;
        QObject* pObjSender   = nullptr;

        int iTrcDetailLevel = ETraceDetailLevelNone;

        if( i_pMethodTracer != nullptr )
        {
            iTrcDetailLevel = i_pMethodTracer->getTraceDetailLevel();

            if( i_pMethodTracer->isActive(i_iFilterDetailLevel) )
            {
                strMsgTrcInfo = "Addr: " + pointer2Str(i_pMsg);
            }
        }

        if( pMsg != nullptr )
        {
            pObjReceiver = pMsg->getReceiver();
            pObjSender   = pMsg->getSender();

            strMsgTrcInfo += ", Content { " + pMsg->getAddTrcInfoStr(iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ? 1 : 0) + " } )";

            if( pObjReceiver != nullptr && pObjSender != nullptr )
            {
                if( i_pMethodTracer != nullptr && i_pMethodTracer->isActive(i_iFilterDetailLevel) )
                {
                    strAddTrcInfo = "postEvent( " + strMsgTrcInfo + " )";
                    i_pMethodTracer->trace(strAddTrcInfo.toLatin1(), i_iFilterDetailLevel);
                }
                QCoreApplication::postEvent(pObjReceiver,i_pMsg);
            }
            else
            {
                if( i_pMethodTracer != nullptr && i_pMethodTracer->isActive(i_iFilterDetailLevel) )
                {
                    strAddTrcInfo = "deleteEvent( " + strMsgTrcInfo + " )";
                    i_pMethodTracer->trace(strAddTrcInfo.toLatin1(), i_iFilterDetailLevel);
                }
                delete i_pMsg;
                i_pMsg = nullptr;
            }
        } // if( pMsg != nullptr )
    } // if( i_pMsg != nullptr )
} // POST_OR_DELETE_MESSAGE


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
    return "ZS::Apps::Test::IpcTrace::ReqTest";
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

    if( i_format == EContentToStrFormatHtmlText )
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
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConTest::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgConTest* pMsgCloned = new CMsgConTest(
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

//------------------------------------------------------------------------------
QString CMsgConTest::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ZS::Apps::Test::IpcTrace::ConTest";
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

    if( i_format == EContentToStrFormatHtmlText )
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
