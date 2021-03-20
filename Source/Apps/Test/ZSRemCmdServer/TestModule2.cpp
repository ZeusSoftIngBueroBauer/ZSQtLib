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

#include <QtCore/qglobal.h>

#include "TestModule2.h"
#include "TestModule1.h"
#include "App.h"
#include "MsgTest.h"

#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::RemCmdServer;


/*******************************************************************************
class CTestModule2Thread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestModule2Thread::CTestModule2Thread(
    CTestModule1*  i_pTestModule1,
    const QString& i_strObjNameTestModule2 ) :
//------------------------------------------------------------------------------
    QThread(i_pTestModule1),
    m_pTestModule1(i_pTestModule1),
    m_strTestModule2ObjName(i_strObjNameTestModule2),
    m_pTestModule2(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(m_strTestModule2ObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::RemCmdServer", "CTestModule2Thread", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "ctor",
        /* szAddInfo    */ "" );

} // ctor

//------------------------------------------------------------------------------
CTestModule2Thread::~CTestModule2Thread()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "dtor",
        /* szAddInfo    */ "" );

    quit();

    if( !wait(30000) )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo(
                /* errSource     */ "ZS::Apps::Test::RemCmdServer", "CTestModule2Thread", objectName(), "dtor",
                /* result        */ EResultTimeout,
                /* severity      */ EResultSeverityError,
                /* strAddErrInfo */ "Waiting for thread to quit timed out" );
            CErrLog::GetInstance()->addEntry(errResultInfo);
         }
    }

    try
    {
        delete m_pTestModule2;
    }
    catch(...)
    {
    }
    m_pTestModule2 = nullptr;
    m_pTestModule1 = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestModule2Thread::sleep( unsigned long i_uTime_s )
//------------------------------------------------------------------------------
{
    QThread::sleep(i_uTime_s);
}

/*==============================================================================
public: // overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CTestModule2Thread::run()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "run",
        /* szAddInfo    */ "" );

    m_pTestModule2 = new CTestModule2( this, m_strTestModule2ObjName );

    emit createdTestModule2(this,m_pTestModule2);

    exec();

    try
    {
        delete m_pTestModule2;
    }
    catch(...)
    {
    }
    m_pTestModule2 = nullptr;

} // run


/*******************************************************************************
class CTestModule2 : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestModule2::CTestModule2(
    CTestModule2Thread* i_pTestModule2Thread,
    const QString&      i_strObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTestModule2Thread(i_pTestModule2Thread),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::RemCmdServer", "CTestModule2", i_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "ctor",
        /* szAddInfo    */ "" );

} // ctor

//------------------------------------------------------------------------------
CTestModule2::~CTestModule2()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "dtor",
        /* szAddInfo    */ "" );

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pTestModule2Thread = nullptr;

} // dtor

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CTestModule2::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    switch( static_cast<int>(i_pEv->type()) )
    {
        case EMsgTypeReqTest:
        {
            SErrResultInfo  errResultInfo;
            CMsgReqTest* pMsgReq = dynamic_cast<CMsgReqTest*>(i_pEv);
            if( pMsgReq == nullptr )
            {
                throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
            }
            if( pMsgReq->mustBeConfirmed() )
            {
                m_pTestModule2Thread->sleep(5);

                CMsgConTest* pMsgCon = new CMsgConTest(
                    /* pObjSender         */ this,
                    /* pObjReceiver       */ pMsgReq->getSender(),
                    /* iReqId             */ pMsgReq->getRequestId(),
                    /* iMsgIdReq          */ pMsgReq->getMsgId(),
                    /* errResultInfo      */ errResultInfo,
                    /* iProgressInPerCent */ 100 );
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }
            bEventHandled = true;
            break;
        }
        default:
        {
            break;
        }
    } // switch( i_pMsg->type() )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event
