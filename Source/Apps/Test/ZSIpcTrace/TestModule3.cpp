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

#include "TestModule3.h"
#include "TestModule2.h"
#include "TestModule1.h"
#include "App.h"
#include "MsgTest.h"

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::IpcTrace;


/*******************************************************************************
class CMyClass3Thread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass3Thread::CMyClass3Thread(
    CMyClass2*     i_pMyClass2,
    const QString& i_strMyClass3ObjName ) :
//------------------------------------------------------------------------------
    QThread(i_pMyClass2),
    m_pMyClass2(i_pMyClass2),
    m_strMyClass3ObjName(i_strMyClass3ObjName),
    m_pMyClass3(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(m_strMyClass3ObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    m_pTrcAdminObj->setTraceDetailLevel(ETraceDetailLevelMethodArgs);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "MyClass2: " + QString(i_pMyClass2 == nullptr ? "null" : i_pMyClass2->objectName());
        strMthInArgs += ", MyClass3ObjName: " + i_strMyClass3ObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CMyClass3Thread::~CMyClass3Thread()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    quit();

    if( !wait(30000) )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo(
                /* errSource     */ "ZS::Apps::Test::IpcTrace", "CMyClass3Thread", objectName(), "dtor",
                /* result        */ EResultTimeout,
                /* severity      */ EResultSeverityError,
                /* strAddErrInfo */ "Waiting for thread to quit timed out" );
            CErrLog::GetInstance()->addEntry(errResultInfo);
         }
    }

    if( m_pMyClass2 != nullptr )
    {
        try
        {
            delete m_pMyClass2;
        }
        catch(...)
        {
        }
        m_pMyClass2 = nullptr;
    }

    m_pMyClass2 = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass3Thread::sleep( unsigned long i_uTime_s )
//------------------------------------------------------------------------------
{
    QThread::sleep(i_uTime_s);
}

/*==============================================================================
public: // overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass3Thread::run()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "run",
        /* strAddInfo   */ "" );

    m_pMyClass3 = new CMyClass3(this, m_strMyClass3ObjName);

    m_pMyClass3->recursiveTraceMethod();

    m_pMyClass3->startMessageTimer();

    exec();

    try
    {
        delete m_pMyClass3;
    }
    catch(...)
    {
    }
    m_pMyClass3 = nullptr;

} // run


/*******************************************************************************
class CMyClass3 : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass3::CMyClass3(
    CMyClass3Thread* i_pMyClass3Thread,
    const QString&   i_strObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMyClass3Thread(i_pMyClass3Thread),
    m_pTmrMessages(nullptr),
    m_iMsgCount(0),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    m_pTrcAdminObj->setTraceDetailLevel(ETraceDetailLevelRuntimeInfo);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "MyClass3Thread: " + QString(i_pMyClass3Thread == nullptr ? "null" : i_pMyClass3Thread->objectName());
        strMthInArgs += ", ObjName: " + i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

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
CMyClass3::~CMyClass3()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pMyClass3Thread = nullptr;
    m_pTmrMessages = nullptr;
    m_iMsgCount = 0;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CMyClass3::recursiveTraceMethod()
//------------------------------------------------------------------------------
{
    static int s_iCount = 0;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "recursiveTraceMethod",
        /* strAddInfo   */ "" );

    ++s_iCount;

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        QString strTrcMsg = "Count=" + QString::number(s_iCount);
        mthTracer.trace(strTrcMsg, ETraceDetailLevelRuntimeInfo);
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
void CMyClass3::startMessageTimer()
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
void CMyClass3::onTmrMessagesTimeout()
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
bool CMyClass3::event( QEvent* i_pEv )
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
            /* strMethodInArgs    */ strMthInArgs );

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
