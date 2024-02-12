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

#include <QtCore/qtimer.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonobject.h>

#include "MyClass2.h"
#include "MyClass1.h"
#include "MyClass3.h"
#include "App.h"
#include "MsgTest.h"

#include "ZSIpcTrace/ZSIpcTrcDllIf.h"

#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysWaitCondition.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace::DllIf;
using namespace ZS::Apps::Test::IpcTraceDllIfQtApp;


/*******************************************************************************
class CMyClass2Thread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass2Thread::CMyClass2Thread( const QString& i_strMyClass2ObjName, CMyClass1* i_pMyClass1 ) :
//------------------------------------------------------------------------------
    QThread(i_pMyClass1),
    m_pMtxWaitForClass2Created(nullptr),
    m_pWaitConditionClass2Created(nullptr),
    m_pMyClass1(i_pMyClass1),
    m_strMyClass2ObjName(i_strMyClass2ObjName),
    m_pMyClass2(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName("MyClass2Thread" + m_strMyClass2ObjName);

    m_pTrcAdminObj = Trace::DllIf::CIpcTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        ClassName().toLatin1().data(),
        objectName().toLatin1().data());

    m_pTrcAdminObj->setMethodCallsTraceDetailLevel(EMethodTraceDetailLevelArgsNormal);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevelArgsNormal )
    {
        strMthInArgs = i_strMyClass2ObjName;
        strMthInArgs += ", " + QString(i_pMyClass1 == nullptr ? "nullptr" : i_pMyClass1->objectName());
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "ctor",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    m_pMtxWaitForClass2Created = new CMutex(ClassName() + "-" + objectName() + "-WaitClass2Created");
    m_pWaitConditionClass2Created = new CWaitCondition(ClassName() + "-" + objectName() + "-WaitClass2Created");

} // ctor

//------------------------------------------------------------------------------
CMyClass2Thread::~CMyClass2Thread()
//------------------------------------------------------------------------------
{
    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod s   */ "dtor",
        /* szMthInArgs  */ "" );

    emit aboutToBeDestroyed(this, objectName());

    if( isRunning() )
    {
        quit();

        if( !wait(30000) )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                SErrResultInfo errResultInfo = ErrResultInfoError(
                    "dtor", EResultTimeout, "Waiting for thread to quit timed out");
                CErrLog::GetInstance()->addEntry(errResultInfo);
             }
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

    try
    {
        delete m_pMtxWaitForClass2Created;
    }
    catch(...)
    {
    }
    m_pMtxWaitForClass2Created = nullptr;

    try
    {
        delete m_pWaitConditionClass2Created;
    }
    catch(...)
    {
    }
    m_pWaitConditionClass2Created = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    Trace::DllIf::CIpcTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods (reimplementing methods of base class QObject)
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass2Thread::setObjectName(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    // Please note that the method will not be traced if called in the ctor.
    // The method is called before the trace admin object is created.
    // But if the method is called to rename an already existing object the
    // method will be traced as the trace admin object is then existing.

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = i_strObjName;
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "setObjectName",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    QObject::setObjectName(i_strObjName);

    if( m_pMtxWaitForClass2Created != nullptr )
    {
        m_pMtxWaitForClass2Created->setObjectName(ClassName() + "-" + objectName() + "-WaitClass2Created");
    }
    if( m_pWaitConditionClass2Created != nullptr )
    {
        m_pWaitConditionClass2Created->setObjectName(ClassName() + "-" + objectName() + "-WaitClass2Created");
    }

    // Should be the last so that the method tracer traces leave method
    // not before the child objects have been renamed.
    if( m_pTrcAdminObj != nullptr )
    {
        m_pTrcAdminObj = Trace::DllIf::CIpcTrcServer::RenameTraceAdminObj(
            m_pTrcAdminObj, objectName().toLatin1().data());
    }

} // setObjectName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass2* CMyClass2Thread::waitForMyClass2Created()
//------------------------------------------------------------------------------
{
    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* strMethod    */ "waitForMyClass2Created",
        /* strMthInArgs */ "" );

    // It is not sufficient just to wait for the wait condition to be signalled.
    // The thread may already have been started, created the Class3 instance and invoked
    // the "onClass3ThreadRunning" slot which signalled the wait condition. A wait here
    // without a timeout may therefore result in a deadlock. And in addition before and
    // after calling "wait" it will be checked whether the Class3 instance has been created.
    if( m_pMtxWaitForClass2Created->tryLock() )
    {
        while( m_pMyClass2 == nullptr )
        {
            m_pWaitConditionClass2Created->wait(m_pMtxWaitForClass2Created, 100);
        }
        m_pMtxWaitForClass2Created->unlock();
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevelArgsNormal))
    {
        mthTracer.setMethodReturn(m_pMyClass2 == nullptr ? "nullptr" : m_pMyClass2->objectName().toStdString().c_str());
    }

    return m_pMyClass2;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass2Thread::sleep( unsigned long i_uTime_s )
//------------------------------------------------------------------------------
{
    QThread::sleep(i_uTime_s);
}

/*==============================================================================
public: // overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass2Thread::run()
//------------------------------------------------------------------------------
{
    // To always get the same trace output. Sleep a bit to let the thread starting
    // instance wait on the wait condition.
    CSleeperThread::msleep(5);

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "run",
        /* szMthInArgs  */ "" );

    m_pMyClass2 = new CMyClass2(m_strMyClass2ObjName, this);

    QObject::connect(
        m_pMyClass2, &CMyClass2::aboutToBeDestroyed,
        this, &CMyClass2Thread::onClass2AboutToBeDestroyed,
        Qt::DirectConnection);

    // To always get the same trace output. Sleep a bit to let the thread starting
    // instance wait on the wait condition.
    CSleeperThread::msleep(5);

    exec();

    QObject::disconnect(
        m_pMyClass2, &CMyClass2::aboutToBeDestroyed,
        this, &CMyClass2Thread::onClass2AboutToBeDestroyed);

    try
    {
        delete m_pMyClass2;
    }
    catch(...)
    {
    }
    m_pMyClass2 = nullptr;

} // run

/*==============================================================================
public: // replacing method of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass2Thread::start( QThread::Priority i_priority )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = qThreadPriority2Str(i_priority);
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "start",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    return QThread::start(i_priority);
}

//------------------------------------------------------------------------------
void CMyClass2Thread::quit()
//------------------------------------------------------------------------------
{
    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "quit",
        /* szMthInArgs  */ "" );

    return QThread::quit();
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
//------------------------------------------------------------------------------
bool CMyClass2Thread::wait( QDeadlineTimer i_deadline )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = QString(i_deadline.isForever() ? "Forever" : QString::number(i_deadline.deadline()));
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "wait",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    bool bResult = QThread::wait(i_deadline);

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        mthTracer.setMethodReturn(bResult);
    }
    return bResult;
}
#endif

//------------------------------------------------------------------------------
bool CMyClass2Thread::wait( unsigned long i_time_ms )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = QString::number(i_time_ms) + " ms";
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "wait",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    bool bResult = QThread::wait(i_time_ms);

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        mthTracer.setMethodReturn(bResult);
    }
    return bResult;
}

/*==============================================================================
protected: // replacing method of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
int CMyClass2Thread::exec()
//------------------------------------------------------------------------------
{
    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "exec",
        /* szMthInArgs  */ "" );

    int iResult = QThread::exec();

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        mthTracer.setMethodReturn(iResult);
    }
    return iResult;
}

//------------------------------------------------------------------------------
void CMyClass2Thread::onClass2AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    m_pMyClass2 = nullptr;
}


/*******************************************************************************
class CMyClass2 : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass2::CMyClass2( const QString& i_strObjName, CMyClass2Thread* i_pMyClass2Thread ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMyClass2Thread(i_pMyClass2Thread),
    m_pTmrMessages(nullptr),
    m_pMtxCounters(nullptr),
    m_iRecursionCount(0),
    m_iMsgCount(0),
    m_strMyClass3ObjName(),
    m_pMyClass3Thread(nullptr),
    m_pMyClass3(nullptr),
    m_pMtxWaitClass3ThreadRunning(nullptr),
    m_pWaitClass3ThreadRunning(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = Trace::DllIf::CIpcTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        ClassName().toLatin1().data(),
        objectName().toLatin1().data());

    m_pTrcAdminObj->setMethodCallsTraceDetailLevel(EMethodTraceDetailLevelArgsNormal);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevelArgsNormal )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", " + QString(i_pMyClass2Thread == nullptr ? "nullptr" : i_pMyClass2Thread->objectName());
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "ctor",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    m_pTmrMessages = new QTimer(this);

    QObject::connect(
        m_pTmrMessages, &QTimer::timeout,
        this, &CMyClass2::onTmrMessagesTimeout);

    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    m_pMtxCounters = new CRecursiveMutex(ClassName() + "-" + objectName() + "-Counters");
    #else
    m_pMtxCounters = new CMutex(ClassName() + "-" + objectName() + "-Counters", QMutex::Recursive);
    #endif
    m_pMtxWaitClass3ThreadRunning = new CMutex(ClassName() + "-" + objectName() + "-WaitClass3ThreadRunning");
    m_pWaitClass3ThreadRunning = new CWaitCondition(ClassName() + "-" + objectName() + "-Class3ThreadRunning");

} // ctor

//------------------------------------------------------------------------------
CMyClass2::~CMyClass2()
//------------------------------------------------------------------------------
{
    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "dtor",
        /* szMthInArgs  */ "" );

    emit aboutToBeDestroyed(this, objectName());

    // If the dtor is called from within the context of another thread than the
    // thread creating the class 2 instance (e.g. from the main thread if destroying
    // class 1 instance) the class 3 thread cannot be deleted here. Otherwise the
    // app may crash with "cannot send events to objects owned by a different thread".
    // To avoid this crash a message must be sent to this instance so that the thread
    // is destroyed in the context which created the thread.
    stopClass3Thread();

    try
    {
        delete m_pMtxCounters;
    }
    catch(...)
    {
    }
    m_pMtxCounters = nullptr;

    try
    {
        delete m_pMtxWaitClass3ThreadRunning;
    }
    catch(...)
    {
    }
    m_pMtxWaitClass3ThreadRunning = nullptr;

    try
    {
        delete m_pWaitClass3ThreadRunning;
    }
    catch(...)
    {
    }
    m_pWaitClass3ThreadRunning = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    Trace::DllIf::CIpcTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pMyClass2Thread = nullptr;
    m_pTmrMessages = nullptr;
    m_iRecursionCount = 0;
    m_iMsgCount = 0;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods (reimplementing methods of base class QObject)
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass2::setObjectName(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    // Please note that the method will not be traced if called in the ctor.
    // The method is called before the trace admin object is created.
    // But if the method is called to rename an already existing object the
    // method will be traced as the trace admin object is then existing.

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = i_strObjName;
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "setObjectName",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    QObject::setObjectName(i_strObjName);

    if( m_pMtxCounters != nullptr )
    {
        m_pMtxCounters->setObjectName(ClassName() + "-" + objectName() + "-Counters");
    }
    if( m_pMtxWaitClass3ThreadRunning != nullptr )
    {
        m_pMtxWaitClass3ThreadRunning->setObjectName(ClassName() + "-" + objectName() + "-WaitClass3ThreadRunning");
    }
    if( m_pWaitClass3ThreadRunning != nullptr )
    {
        m_pWaitClass3ThreadRunning->setObjectName(ClassName() + "-" + objectName() + "-Class3ThreadRunning");
    }

    // Should be the last so that the method tracer traces leave method
    // not before the child objects have been renamed.
    if( m_pTrcAdminObj != nullptr )
    {
        m_pTrcAdminObj = Trace::DllIf::CIpcTrcServer::RenameTraceAdminObj(
            m_pTrcAdminObj, objectName().toLatin1().data());
    }

} // setObjectName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CMyClass2::instMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "instMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    if( QThread::currentThread() != thread() )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
        pMsgReq->setCommand("instMethod");
        pMsgReq->setCommandArg(i_strMthInArgs);
        POST_OR_DELETE_MESSAGE(pMsgReq);
        pMsgReq = nullptr;
        strResult = "You here from me later ...";
    }
    else // if( QThread::currentThread() == thread() )
    {
        strResult = "Hello World";
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet.toLatin1().data());
    }

    return strResult;

} // instMethod

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CMyClass2::recursiveTraceMethod()
//------------------------------------------------------------------------------
{
    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "recursiveTraceMethod",
        /* szMthInArgs  */ "" );

    CMutexLocker mtxLocker(m_pMtxCounters);

    if( QThread::currentThread() != thread() )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
        pMsgReq->setCommand("recursiveTraceMethod");
        POST_OR_DELETE_MESSAGE(pMsgReq);
        pMsgReq = nullptr;
    }
    else // if( QThread::currentThread() == thread() )
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevelDebug) )
        {
            QString strTrcMsg = "RecursionCount=" + QString::number(m_iRecursionCount);
            mthTracer.trace(strTrcMsg.toLatin1().data());
        }

        ++m_iRecursionCount;

        if( m_iRecursionCount <= 10 )
        {
            recursiveTraceMethod();
        }
        --m_iRecursionCount;
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        mthTracer.setMethodReturn(m_iRecursionCount);
    }

    return m_iRecursionCount;

} // recursiveTraceMethod

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass2::startMessageTimer()
//------------------------------------------------------------------------------
{
    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "startMessageTimer",
        /* szMthInArgs  */ "" );

    CMutexLocker mtxLocker(m_pMtxCounters);

    if( QThread::currentThread() != thread() )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
        pMsgReq->setCommand("startMessageTimer");
        POST_OR_DELETE_MESSAGE(pMsgReq);
        pMsgReq = nullptr;
    }
    else // if( QThread::currentThread() == thread() )
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevelDebug) )
        {
            mthTracer.trace("m_pTmrMessages->start(100)");
        }

        if( !m_pTmrMessages->isActive() )
        {
            m_iMsgCount = 0;
            m_pTmrMessages->start(100);
        }
    }
} // startMessageTimer

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass3* CMyClass2::startClass3Thread(const QString& i_strMyClass3ObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = i_strMyClass3ObjName;
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "startClass3Thread",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    m_strMyClass3ObjName = i_strMyClass3ObjName;

    // Class 3 Thread should be created in the context in which also the Class 2
    // instance has been created. For this a message will be sent to this so that
    // a thread context change is done.
    if( QThread::currentThread() != thread() )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
        pMsgReq->setCommand("startClass3Thread");
        POST_OR_DELETE_MESSAGE(pMsgReq);
        pMsgReq = nullptr;
    }
    else // if( QThread::currentThread() == thread() )
    {
        if( m_pMyClass3Thread == nullptr )
        {
            m_pMyClass3Thread = new CMyClass3Thread(i_strMyClass3ObjName, this);

            QObject::connect(
                m_pMyClass3Thread, &CMyClass3Thread::running,
                this, &CMyClass2::onClass3ThreadRunning,
                Qt::DirectConnection);
        }

        if( !m_pMyClass3Thread->isRunning() )
        {
            m_pMyClass3Thread->start();
        }
    }

    // It is not sufficient just to wait for the wait condition to be signalled.
    // The thread may already have been started, created the Class3 instance and invoked
    // the "onClass3ThreadRunning" slot which signalled the wait condition. A wait here
    // without a timeout may therefore result in a deadlock. And in addition before and
    // after calling "wait" it will be checked whether the Class3 instance has been created.
    if( m_pMtxWaitClass3ThreadRunning->tryLock() )
    {
        while( m_pMyClass3Thread == nullptr || m_pMyClass3Thread->getMyClass3() == nullptr )
        {
            m_pWaitClass3ThreadRunning->wait(m_pMtxWaitClass3ThreadRunning, 100);
        }
        m_pMtxWaitClass3ThreadRunning->unlock();
        m_pMyClass3 = m_pMyClass3Thread->getMyClass3();
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthRet = QString(m_pMyClass3 == nullptr ? "null" : m_pMyClass3->objectName());
        mthTracer.setMethodReturn(strMthRet.toLatin1().data());
    }

    return m_pMyClass3;

} // startClass3Thread

//------------------------------------------------------------------------------
void CMyClass2::stopClass3Thread()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "stopClass3Thread",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    if( m_pMyClass3Thread != nullptr && m_pMyClass3Thread->isRunning() )
    {
        // The thread must be destroyed from within the thread in which this
        // thread has been created. To ensure this we send a message to myself
        // so that the thread context changed is done and to avoid the crash
        // with "cannot send events to objects owned by a different thread".
        if( QThread::currentThread() != thread() )
        {
            CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
            pMsgReq->setCommand("stopClass3Thread");
            POST_OR_DELETE_MESSAGE(pMsgReq);
            pMsgReq = nullptr;
        }
        else // if( QThread::currentThread() == thread() )
        {
            m_pMyClass3Thread->quit();

            // Let the thread quit the event loop (return from exec) before calling wait
            // to get the same trace output each time.
            CSleeperThread::msleep(50);

            int iTimeout_ms = 30000; // So the value can be modified during debugging session.
            if( !m_pMyClass3Thread->wait(iTimeout_ms) )
            {
                SErrResultInfo errResultInfo = ErrResultInfoError(
                    "stopClass3Thread", EResultTimeout, "Waiting for thread to quit timed out");
                CErrLog::GetInstance()->addEntry(errResultInfo);
                m_pMyClass3Thread->terminate();
            }

            delete m_pMyClass3Thread;
            m_pMyClass3Thread = nullptr;
            m_pMyClass3 = nullptr;
        }
    }
} // stopClass3Thread

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass2::sendMuchData()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddInfo;

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* strMethod    */ "sendMuchData",
        /* strMthInArgs */ strMthInArgs.toLatin1().data() );

    if( QThread::currentThread() != thread() )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
        pMsgReq->setCommand("sendMuchData");
        POST_OR_DELETE_MESSAGE(pMsgReq);
        pMsgReq = nullptr;
    }
    else
    {
        Trace::DllIf::ELogDetailLevel detailLevelPrev = Trace::DllIf::ELogDetailLevelNone;

        if( m_pTrcAdminObj != nullptr )
        {
            detailLevelPrev = m_pTrcAdminObj->getRuntimeInfoTraceDetailLevel();
            m_pTrcAdminObj->setRuntimeInfoTraceDetailLevel(Trace::DllIf::ELogDetailLevelDebugVerbose);
        }

        CMyClass3* pMyClass3 = startClass3Thread(objectName());
        pMyClass3->sendMuchData();

        QJsonObject jsonObj;

        jsonObj.insert("Hello", "Class2");
        jsonObj.insert("Zahl1", 1.0);
        jsonObj.insert("Text1", "Text1");
        jsonObj.insert("Zahl2", 2.0);
        jsonObj.insert("Text2", "Text2");
        jsonObj.insert("Hello", "Class2");

        double fStartTime_s = System::Time::getProcTimeInSec();
        double fCurrTime_s = System::Time::getProcTimeInSec();
        double fDuration_s = 20.0;

        while ((fCurrTime_s - fStartTime_s) < fDuration_s)
        {
            jsonObj.insert("StartTime_s", fStartTime_s);
            jsonObj.insert("CurrTime_s", fCurrTime_s);
            jsonObj.insert("Duration_s", fDuration_s);

            if( mthTracer.isRuntimeInfoActive(Trace::DllIf::ELogDetailLevelDebugVerbose) )
            {
                strAddInfo = "Im sending data now for " + QString::number(fCurrTime_s - fStartTime_s, 'f', 3) + " seconds. ";
                mthTracer.trace(strAddInfo.toLatin1().data());
            }
            fCurrTime_s = System::Time::getProcTimeInSec();
            sendData1(QJsonDocument(jsonObj).toJson(QJsonDocument::Compact), fStartTime_s, fCurrTime_s, fDuration_s);
        }

        stopClass3Thread();
        pMyClass3 = nullptr;

        if( m_pTrcAdminObj != nullptr )
        {
            m_pTrcAdminObj->setRuntimeInfoTraceDetailLevel(detailLevelPrev);
        }
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass2::onClass3ThreadRunning()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "onClass3ThreadRunning",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    m_pWaitClass3ThreadRunning->notify_all();
}

//------------------------------------------------------------------------------
void CMyClass2::onTmrMessagesTimeout()
//------------------------------------------------------------------------------
{
    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "onTmrMessagesTimeout",
        /* szMthInArgs  */ "" );

    CMutexLocker mtxLocker(m_pMtxCounters);

    ++m_iMsgCount;

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevelDebug) )
    {
        QString strTrcMsg = "MsgCount=" + QString::number(m_iMsgCount);
        mthTracer.trace(strTrcMsg.toLatin1().data());
    }

    if( m_iMsgCount >= 10 && m_pTmrMessages->isActive() )
    {
        m_pTmrMessages->stop();
    }

} // onTmrMessagesTimeout

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CMyClass2::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bHandled = false;

    ZS::System::CMsg* pMsg = dynamic_cast<ZS::System::CMsg*>(i_pEv);

    if( pMsg != nullptr )
    {
        CMsgReqTest* pMsgReq = dynamic_cast<CMsgReqTest*>(i_pEv);

        // Let the first call to the method sending the event return and unlock
        // the Counter Mutex before continue to get the same trace output each time.
        if( pMsgReq != nullptr && pMsgReq->getCommand() == "instMethod" )
        {
            CSleeperThread::msleep(10);
        }
        else if( pMsgReq != nullptr && pMsgReq->getCommand() == "recursiveTraceMethod" )
        {
            CSleeperThread::msleep(10);
        }
        else if( pMsgReq != nullptr && pMsgReq->getCommand() == "startMessageTimer" )
        {
            CSleeperThread::msleep(10);
        }
        else if( pMsgReq != nullptr && pMsgReq->getCommand() == "startClass3Thread" )
        {
            CSleeperThread::msleep(10);
        }
        else if( pMsgReq != nullptr && pMsgReq->getCommand() == "stopClass3Thread" )
        {
            CSleeperThread::msleep(10);
        }
        else if( pMsgReq != nullptr && pMsgReq->getCommand() == "sendMuchData" )
        {
            CSleeperThread::msleep(10);
        }

        QString strMthInArgs;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
        {
            strMthInArgs = "{" + QString(pMsg == nullptr ? "null" : pMsg->getAddTrcInfoStr(m_pTrcAdminObj->getMethodCallsTraceDetailLevel())) + "}";
        }

        Trace::DllIf::CMethodTracer mthTracer(
            /* pTrcServer         */ m_pTrcAdminObj,
            /* iFilterDetailLevel */ EMethodTraceDetailLevelEnterLeave,
            /* szMethod           */ "event",
            /* szMethodInArgs     */ strMthInArgs.toLatin1().data() );

        if( pMsgReq != nullptr )
        {
            if( pMsgReq->getCommand() == "instMethod" )
            {
                instMethod(pMsgReq->getCommandArg());
            }
            else if( pMsgReq->getCommand() == "recursiveTraceMethod" )
            {
                recursiveTraceMethod();
            }
            else if( pMsgReq->getCommand() == "startMessageTimer" )
            {
                startMessageTimer();
            }
            else if( pMsgReq->getCommand() == "startClass3Thread" )
            {
                startClass3Thread(m_strMyClass3ObjName);
            }
            else if( pMsgReq->getCommand() == "stopClass3Thread" )
            {
                stopClass3Thread();
            }
            else if( pMsgReq->getCommand() == "sendMuchData" )
            {
                sendMuchData();
            }
            bHandled = true;
        }
    }

    if( !bHandled )
    {
        bHandled = QObject::event(i_pEv);
    }

    return bHandled;

} // event

/*==============================================================================
private: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CMyClass2::sendData1( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
//------------------------------------------------------------------------------
{
    static int s_iCount = 0;

    QString strMthInArgs;
    QString strAddInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = "Data: " + i_strData;
        strMthInArgs += ", StartTime: " + QString::number(i_fStartTime_s) + "s";
        strMthInArgs += ", CurrTime: " + QString::number(i_fStartTime_s) + "s";
        strMthInArgs += ", Duration: " + QString::number(i_fStartTime_s) + "s";
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* strMethod    */ "sendData1",
        /* strMthInArgs */ strMthInArgs.toLatin1().data() );

    ++s_iCount;

    sendData2(i_strData, i_fStartTime_s, i_fCurrTime_s, i_fDuration_s);

    if( mthTracer.isRuntimeInfoActive(Trace::DllIf::ELogDetailLevelDebugVerbose) )
    {
        strAddInfo = "Time remaining: " + QString::number(i_fDuration_s - (i_fCurrTime_s - i_fStartTime_s), 'f', 3) + " seconds.";
        mthTracer.trace(strAddInfo.toLatin1().data());
    }
    if( mthTracer.areMethodCallsActive(Trace::DllIf::EMethodTraceDetailLevelArgsNormal) )
    {
        mthTracer.setMethodReturn(s_iCount);
    }
    return s_iCount;
}

//------------------------------------------------------------------------------
int CMyClass2::sendData2( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
//------------------------------------------------------------------------------
{
    static int s_iCount = 0;

    QString strMthInArgs;
    QString strAddInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = "Data: " + i_strData;
        strMthInArgs += ", StartTime: " + QString::number(i_fStartTime_s) + "s";
        strMthInArgs += ", CurrTime: " + QString::number(i_fStartTime_s) + "s";
        strMthInArgs += ", Duration: " + QString::number(i_fStartTime_s) + "s";
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* strMethod    */ "sendData2",
        /* strMthInArgs */ strMthInArgs.toLatin1().data() );

    ++s_iCount;

    sendData3(i_strData, i_fStartTime_s, i_fCurrTime_s, i_fDuration_s);

    if( mthTracer.isRuntimeInfoActive(Trace::DllIf::ELogDetailLevelDebugVerbose) )
    {
        strAddInfo = "Time remaining: " + QString::number(i_fDuration_s - (i_fCurrTime_s - i_fStartTime_s), 'f', 3) + " seconds.";
        mthTracer.trace(strAddInfo.toLatin1().data());
    }
    if( mthTracer.areMethodCallsActive(Trace::DllIf::EMethodTraceDetailLevelArgsNormal) )
    {
        mthTracer.setMethodReturn(s_iCount);
    }
    return s_iCount;
}

//------------------------------------------------------------------------------
int CMyClass2::sendData3( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
//------------------------------------------------------------------------------
{
    static int s_iCount = 0;

    QString strMthInArgs;
    QString strAddInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = "Data: " + i_strData;
        strMthInArgs += ", StartTime: " + QString::number(i_fStartTime_s) + "s";
        strMthInArgs += ", CurrTime: " + QString::number(i_fStartTime_s) + "s";
        strMthInArgs += ", Duration: " + QString::number(i_fStartTime_s) + "s";
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* strMethod    */ "sendData3",
        /* strMthInArgs */ strMthInArgs.toLatin1().data() );

    ++s_iCount;

    if( mthTracer.isRuntimeInfoActive(Trace::DllIf::ELogDetailLevelDebugVerbose) )
    {
        strAddInfo = "Time remaining: " + QString::number(i_fDuration_s - (i_fCurrTime_s - i_fStartTime_s), 'f', 3) + " seconds.";
        mthTracer.trace(strAddInfo.toLatin1().data());
    }
    if( mthTracer.areMethodCallsActive(Trace::DllIf::EMethodTraceDetailLevelArgsNormal) )
    {
        mthTracer.setMethodReturn(s_iCount);
    }
    return s_iCount;
}
