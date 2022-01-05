/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include <QtCore/qtimer.h>

#include "TestModule2.h"
#include "TestModule1.h"
#include "App.h"
#include "MsgTest.h"

#include "ZSIpcTrace/ZSIpcTrcDllIf.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace::DllIf;
using namespace ZS::Apps::Test::IpcTraceDllIfQtApp;


/*******************************************************************************
class CTestModule2Thread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestModule2Thread::CTestModule2Thread(
    CTestModule1*  i_pTestModule1,
    const QString& i_strTestModule2ObjName ) :
//------------------------------------------------------------------------------
    QThread(i_pTestModule1),
    m_pTestModule1(i_pTestModule1),
    m_pTestModule2(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strTestModule2ObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace().toLatin1(), ClassName().toLatin1(), objectName().toLatin1());

    m_pTrcAdminObj->setTraceDetailLevel(ETraceDetailLevelMethodArgs);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TestModule1: " + QString(i_pTestModule1 == nullptr ? "null" : i_pTestModule1->objectName());
        strMthInArgs += ", TestModule2ObjName: " + i_strTestModule2ObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs.toLatin1() );

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
                /* errSource     */ "ZS::Apps::Test::IpcTrace", "CTestModule2Thread", objectName(), "dtor",
                /* result        */ EResultTimeout,
                /* severity      */ EResultSeverityError,
                /* strAddErrInfo */ "Waiting for thread to quit timed out" );
            CErrLog::GetInstance()->addEntry(errResultInfo);
         }
    }

    if( m_pTestModule2 != nullptr )
    {
        try
        {
            delete m_pTestModule2;
        }
        catch(...)
        {
        }
        m_pTestModule2 = nullptr;
    }

    m_pTestModule1 = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

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

    m_pTestModule2 = new CTestModule2(this, objectName());

    m_pTestModule2->recursiveTraceMethod();

    m_pTestModule2->startMessageTimer();

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
    m_pTmrMessages(nullptr),
    m_iMsgCount(0),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace().toLatin1(), ClassName().toLatin1(), objectName().toLatin1());

    m_pTrcAdminObj->setTraceDetailLevel(ETraceDetailLevelRuntimeInfo);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TestModule2Thread: " + QString(i_pTestModule2Thread == nullptr ? "null" : i_pTestModule2Thread->objectName());
        strMthInArgs += ", ObjName: " + i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs.toLatin1() );

    m_pTmrMessages = new QTimer(this);

    if( !QObject::connect(
        /* szSender   */ m_pTmrMessages,
        /* szSignal   */ SIGNAL(timeout()),
        /* szReceiver */ this,
        /* szSlot     */ SLOT(onTmrMessagesTimeout())) )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultSignalSlotConnectionFailed);
    }

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

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pTestModule2Thread = nullptr;
    m_pTmrMessages = nullptr;
    m_iMsgCount = 0;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CTestModule2::recursiveTraceMethod()
//------------------------------------------------------------------------------
{
    static int s_iCount = 0;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "recursiveTraceMethod",
        /* szAddInfo    */ "" );

    ++s_iCount;

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        QString strTrcMsg = "Count=" + QString::number(s_iCount);
        mthTracer.trace(strTrcMsg.toLatin1(), ETraceDetailLevelRuntimeInfo);
    }

    if( s_iCount <= 10 )
    {
        recursiveTraceMethod();
    }

    --s_iCount;

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(s_iCount);
    }

    return s_iCount;

} // recursiveTraceMethod

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestModule2::startMessageTimer()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startMessageTimer",
        /* strAddInfo   */ "" );

    if( !m_pTmrMessages->isActive() )
    {
        m_iMsgCount = 0;
        m_pTmrMessages->start(100);
    }
} // startMessageTimer

/*==============================================================================
protected: // slots
==============================================================================*/

//------------------------------------------------------------------------------
void CTestModule2::onTmrMessagesTimeout()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelRuntimeInfo,
        /* strMethod    */ "onTmrMessagesTimeout",
        /* strAddInfo   */ "" );

    CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
    POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);
    pMsgReq = nullptr;

} // onTmrMessagesTimeout

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CTestModule2::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bHandled = false;

    ZS::System::CMsg* pMsg = dynamic_cast<ZS::System::CMsg*>(i_pEv);

    if( pMsg != nullptr )
    {
        ++m_iMsgCount;

        if( m_iMsgCount >= 100 && m_pTmrMessages->isActive() )
        {
            m_pTmrMessages->stop();
        }

        QString strMthInArgs;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = "Msg: " + QString(pMsg == nullptr ? "null" : pMsg->msgTypeToStr());
        }

        CMethodTracer mthTracer(
            /* pTrcServer         */ m_pTrcAdminObj,
            /* iFilterDetailLevel */ ETraceDetailLevelRuntimeInfo,
            /* strMethod          */ "event",
            /* strMethodInArgs    */ strMthInArgs.toLatin1() );

        bHandled = true;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
        {
            mthTracer.setMethodReturn(m_iMsgCount);
        }
    }

    if( !bHandled )
    {
        bHandled = QObject::event(i_pEv);
    }

    return bHandled;

} // event
