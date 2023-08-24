/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer, Germany
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
using namespace ZS::Apps::Test::IpcLogDllIfQtApp;


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
        /* iMsgIdReq        */ i_iMsgId ),
    m_strCommand(),
    m_strlstCommandArgs()
{
} // ctor

//------------------------------------------------------------------------------
CMsgReqTest::~CMsgReqTest()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgReqTest::setCommand( const QString& i_strCommand )
//------------------------------------------------------------------------------
{
    m_strCommand = i_strCommand;
}

//------------------------------------------------------------------------------
QString CMsgReqTest::getCommand() const
//------------------------------------------------------------------------------
{
    return m_strCommand;
}

//------------------------------------------------------------------------------
void CMsgReqTest::setCommandArg( const QString& i_strArg )
//------------------------------------------------------------------------------
{
    m_strlstCommandArgs.clear();
    m_strlstCommandArgs.append(i_strArg);
}

//------------------------------------------------------------------------------
void CMsgReqTest::setCommandArgs( const QStringList& i_strlstArgs )
//------------------------------------------------------------------------------
{
    m_strlstCommandArgs = i_strlstArgs;
}

//------------------------------------------------------------------------------
QString CMsgReqTest::getCommandArg() const
//------------------------------------------------------------------------------
{
    return m_strlstCommandArgs.size() == 0 ? "" : m_strlstCommandArgs[0];
}

//------------------------------------------------------------------------------
QStringList CMsgReqTest::getCommandArgs() const
//------------------------------------------------------------------------------
{
    return m_strlstCommandArgs;
}

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
    pMsgCloned->setCommand(m_strCommand);
    pMsgCloned->setCommandArgs(m_strlstCommandArgs);
    return pMsgCloned;
}

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
    return "ZS::Apps::Test::IpcTraceDllIfQtApp::ReqTest";
}

//------------------------------------------------------------------------------
QString CMsgReqTest::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    return msgTypeToStr() + ", " + m_strCommand;
}


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
        /* iProgressInPerCent */ i_iProgressInPerCent ),
    m_strCommand()
{
} // ctor

//------------------------------------------------------------------------------
CMsgConTest::~CMsgConTest()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgConTest::setCommand( const QString& i_strCommand )
//------------------------------------------------------------------------------
{
    m_strCommand = i_strCommand;
}

//------------------------------------------------------------------------------
QString CMsgConTest::getCommand() const
//------------------------------------------------------------------------------
{
    return m_strCommand;
}

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
    pMsgCloned->setCommand(m_strCommand);
    return pMsgCloned;
}

//------------------------------------------------------------------------------
QString CMsgConTest::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return "ZS::Apps::Test::IpcTraceDllIfQtApp::ConTest";
}

//------------------------------------------------------------------------------
QString CMsgConTest::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    return msgTypeToStr() + ", " + m_strCommand;
}
