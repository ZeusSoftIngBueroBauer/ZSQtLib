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

#include "MyClass3.h"
#include "MyClass2.h"
#include "MyClass1.h"
#include "App.h"
#include "MsgTest.h"

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysWaitCondition.h"

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
CMyClass3Thread::CMyClass3Thread( const QString& i_strMyClass3ObjName, CMyClass2* i_pMyClass2 ) :
//------------------------------------------------------------------------------
    QThread(i_pMyClass2),
    m_pMyClass2(i_pMyClass2),
    m_strMyClass3ObjName(i_strMyClass3ObjName),
    m_pMyClass3(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName("MyClass3Thread" + m_strMyClass3ObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    m_pTrcAdminObj->setMethodCallsTraceDetailLevel(EMethodTraceDetailLevel::ArgsNormal);

    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = i_strMyClass3ObjName;
        strMthInArgs += ", " + QString(i_pMyClass2 == nullptr ? "nullptr" : i_pMyClass2->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CMyClass3Thread::~CMyClass3Thread()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strMthInArgs */ "" );

    emit aboutToBeDestroyed(this, objectName());

    if( isRunning() )
    {
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
    }

    if( m_pMyClass3 != nullptr )
    {
        try
        {
            delete m_pMyClass3;
        }
        catch(...)
        {
        }
        m_pMyClass3 = nullptr;
    }

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods (reimplementing methods of base class QObject)
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass3Thread::setObjectName(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    // Please note that the method will not be traced if called in the ctor.
    // The method is called before the trace admin object is created.
    // But if the method is called to rename an already existing object the
    // method will be traced as the trace admin object is then existing.

    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setObjectName",
        /* strMthInArgs */ strMthInArgs );

    QObject::setObjectName(i_strObjName);

    if( m_pTrcAdminObj != nullptr )
    {
        m_pTrcAdminObj = CTrcServer::RenameTraceAdminObj(m_pTrcAdminObj, objectName());
    }

} // setObjectName

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
    // To always get the same trace output. Sleep a bit to let the thread starting
    // instance wait on the wait condition.
    CSleeperThread::msleep(5);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "run",
        /* strMthInArgs */ "" );

    m_pMyClass3 = new CMyClass3(m_strMyClass3ObjName, this);

    QObject::connect(
        m_pMyClass3, &CMyClass3::aboutToBeDestroyed,
        this, &CMyClass3Thread::onClass3AboutToBeDestroyed,
        Qt::DirectConnection);

    emit running();

    // To always get the same trace output. Sleep a bit to let the thread starting
    // instance wait on the wait condition.
    CSleeperThread::msleep(5);

    m_pMyClass3->recursiveTraceMethod();

    exec();

    QObject::disconnect(
        m_pMyClass3, &CMyClass3::aboutToBeDestroyed,
        this, &CMyClass3Thread::onClass3AboutToBeDestroyed);

    try
    {
        delete m_pMyClass3;
    }
    catch(...)
    {
    }
    m_pMyClass3 = nullptr;

} // run

/*==============================================================================
public: // replacing method of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass3Thread::start( QThread::Priority i_priority )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = qThreadPriority2Str(i_priority);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "start",
        /* strMthInArgs */ strMthInArgs );

    return QThread::start(i_priority);
}

//------------------------------------------------------------------------------
void CMyClass3Thread::quit()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "quit",
        /* strMthInArgs */ "" );

    return QThread::quit();
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
//------------------------------------------------------------------------------
bool CMyClass3Thread::wait( QDeadlineTimer i_deadline )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = QString(i_deadline.isForever() ? "Forever" : QString::number(i_deadline.deadline()));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "wait",
        /* strMthInArgs */ strMthInArgs );

    bool bResult = QThread::wait(i_deadline);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(bResult);
    }
    return bResult;
}
#endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)

//------------------------------------------------------------------------------
bool CMyClass3Thread::wait( unsigned long i_time_ms )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = QString::number(i_time_ms) + " ms";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "wait",
        /* strMthInArgs */ strMthInArgs );

    bool bResult = QThread::wait(i_time_ms);

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(bResult);
    }
    return bResult;
}

/*==============================================================================
protected: // replacing method of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
int CMyClass3Thread::exec()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "exec",
        /* strMthInArgs */ "" );

    int iResult = QThread::exec();

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(iResult);
    }
    return iResult;
}

//------------------------------------------------------------------------------
void CMyClass3Thread::onClass3AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    m_pMyClass3 = nullptr;
}


/*******************************************************************************
class CMyClass3 : public QObject
*******************************************************************************/

CTrcAdminObjRefAnchor CMyClass3::s_trcAdminObjRefAnchor(
    CMyClass3::NameSpace(), CMyClass3::ClassName());

CTrcAdminObjRefAnchor CMyClass3::s_trcAdminObjRefAnchorNoisyMethods(
    CMyClass3::NameSpace(), CMyClass3::ClassName() + "::NoisyMethods");

CTrcAdminObjRefAnchor CMyClass3::s_trcAdminObjRefAnchorVeryNoisyMethods(
    CMyClass3::NameSpace(), CMyClass3::ClassName() + "::VeryNoisyMethods");

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CMyClass3::classMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    CTrcAdminObjRefGuard trcAdminObjGuard(&s_trcAdminObjRefAnchor);

    if( trcAdminObjGuard.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ trcAdminObjGuard.trcAdminObj(),
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "classMethod",
        /* strMthInArgs */ strMthInArgs );

    strResult = "Hello World";

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet);
    }

    return strResult;

} // classMethod

//------------------------------------------------------------------------------
QString CMyClass3::noisyClassMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    CTrcAdminObjRefGuard trcAdminObjGuard(&s_trcAdminObjRefAnchorNoisyMethods);

    if( trcAdminObjGuard.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ trcAdminObjGuard.trcAdminObj(),
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "noisyClassMethod",
        /* strMthInArgs */ strMthInArgs );

    strResult = "Hello World";

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet);
    }

    return strResult;

} // noisyClassMethod

//------------------------------------------------------------------------------
QString CMyClass3::veryNoisyClassMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    CTrcAdminObjRefGuard trcAdminObjGuard(&s_trcAdminObjRefAnchorVeryNoisyMethods);

    if( trcAdminObjGuard.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ trcAdminObjGuard.trcAdminObj(),
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "veryNoisyClassMethod",
        /* strMthInArgs */ strMthInArgs );

    strResult = "Hello World";

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet);
    }

    return strResult;

} // veryNoisyClassMethod

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass3::CMyClass3( const QString& i_strObjName, CMyClass3Thread* i_pMyClass3Thread ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMyClass3Thread(i_pMyClass3Thread),
    m_pMtxCounters(nullptr),
    m_iRecursionCount(0),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjNoisyMethods(nullptr),
    m_pTrcAdminObjVeryNoisyMethods(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::NoisyMethods", objectName());
    m_pTrcAdminObjVeryNoisyMethods = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::VeryNoisyMethods", objectName());

    m_pTrcAdminObj->setMethodCallsTraceDetailLevel(EMethodTraceDetailLevel::ArgsNormal);
    m_pTrcAdminObjNoisyMethods->setMethodCallsTraceDetailLevel(EMethodTraceDetailLevel::ArgsNormal);
    m_pTrcAdminObjVeryNoisyMethods->setMethodCallsTraceDetailLevel(EMethodTraceDetailLevel::ArgsNormal);

    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", " + QString(i_pMyClass3Thread == nullptr ? "nullptr" : i_pMyClass3Thread->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    m_pMtxCounters = new CRecursiveMutex(ClassName() + "-" + objectName() + "-Counters");
    #else
    m_pMtxCounters = new CMutex(ClassName() + "-" + objectName() + "-Counters", QMutex::Recursive);
    #endif

} // ctor

//------------------------------------------------------------------------------
CMyClass3::~CMyClass3()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strMthInArgs */ "" );

    emit aboutToBeDestroyed(this, objectName());

    try
    {
        delete m_pMtxCounters;
    }
    catch(...)
    {
    }
    m_pMtxCounters = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjVeryNoisyMethods);

    m_pMyClass3Thread = nullptr;
    m_iRecursionCount = 0;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;
    m_pTrcAdminObjVeryNoisyMethods = nullptr;

} // dtor

/*==============================================================================
public: // instance methods (reimplementing methods of base class QObject)
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass3::setObjectName(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    // Please note that the method will not be traced if called in the ctor.
    // The method is called before the trace admin object is created.
    // But if the method is called to rename an already existing object the
    // method will be traced as the trace admin object is then existing.

    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setObjectName",
        /* strMthInArgs */ strMthInArgs );

    QObject::setObjectName(i_strObjName);

    if( m_pMtxCounters != nullptr )
    {
        m_pMtxCounters->setObjectName(ClassName() + "-" + objectName() + "-Counters");
    }

    // Should be the last so that the method tracer traces leave method
    // not before the child objects have been renamed.
    if( m_pTrcAdminObj != nullptr )
    {
        m_pTrcAdminObj = CTrcServer::RenameTraceAdminObj(m_pTrcAdminObj, objectName());
    }
    if( m_pTrcAdminObjNoisyMethods != nullptr )
    {
        m_pTrcAdminObjNoisyMethods =
            CTrcServer::RenameTraceAdminObj(m_pTrcAdminObjNoisyMethods, objectName());
    }
    if( m_pTrcAdminObjVeryNoisyMethods != nullptr )
    {
        m_pTrcAdminObjVeryNoisyMethods =
            CTrcServer::RenameTraceAdminObj(m_pTrcAdminObjVeryNoisyMethods, objectName());
    }

} // setObjectName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CMyClass3::instMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "instMethod",
        /* strMthInArgs */ strMthInArgs );

    if( QThread::currentThread() != thread() )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
        pMsgReq->setCommand("instMethod");
        pMsgReq->setCommandArg(i_strMthInArgs);
        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::Debug);
        pMsgReq = nullptr;
        strResult = "You here from me later ...";
    }
    else // if( QThread::currentThread() == thread() )
    {
        strResult = "Hello World";
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet);
    }

    return strResult;

} // instMethod

//------------------------------------------------------------------------------
QString CMyClass3::noisyInstMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    if( areMethodCallsActive(m_pTrcAdminObjNoisyMethods, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "noisyInstMethod",
        /* strMthInArgs */ strMthInArgs );

    if( QThread::currentThread() != thread() )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
        pMsgReq->setCommand("noisyInstMethod");
        pMsgReq->setCommandArg(i_strMthInArgs);
        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::Debug);
        pMsgReq = nullptr;
        strResult = "You here from me later ...";
    }
    else // if( QThread::currentThread() == thread() )
    {
        strResult = "Hello World";
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet);
    }

    return strResult;

} // noisyInstMethod

//------------------------------------------------------------------------------
QString CMyClass3::veryNoisyInstMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    if( m_pTrcAdminObjVeryNoisyMethods != nullptr && m_pTrcAdminObjVeryNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjVeryNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "veryNoisyInstMethod",
        /* strMthInArgs */ strMthInArgs );

    if( QThread::currentThread() != thread() )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
        pMsgReq->setCommand("veryNoisyInstMethod");
        pMsgReq->setCommandArg(i_strMthInArgs);
        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::Debug);
        pMsgReq = nullptr;
        strResult = "You here from me later ...";
    }
    else // if( QThread::currentThread() == thread() )
    {
        strResult = "Hello World";
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet);
    }

    return strResult;

} // veryNoisyInstMethod

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CMyClass3::recursiveTraceMethod()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "recursiveTraceMethod",
        /* strMthInArgs */ "" );

    CMutexLocker mtxLocker(m_pMtxCounters);

    if( QThread::currentThread() != thread() )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
        pMsgReq->setCommand("recursiveTraceMethod");
        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::Debug);
        pMsgReq = nullptr;
    }
    else // if( QThread::currentThread() == thread() )
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            QString strTrcMsg = "RecursionCount=" + QString::number(m_iRecursionCount);
            mthTracer.trace(strTrcMsg);
        }

        ++m_iRecursionCount;

        if( m_iRecursionCount <= 10 )
        {
            recursiveTraceMethod();
        }
        --m_iRecursionCount;
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal))
    {
        mthTracer.setMethodReturn(m_iRecursionCount);
    }

    return m_iRecursionCount;

} // recursiveTraceMethod

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
        CMsgReqTest* pMsgReq = dynamic_cast<CMsgReqTest*>(i_pEv);

        // Let the first call to the method sending the event return and unlock
        // the Counter Mutex before continue to get the same trace output each time.
        if( pMsgReq != nullptr && pMsgReq->getCommand() == "instMethod" )
        {
            CSleeperThread::msleep(10);
        }
        else if( pMsgReq != nullptr && pMsgReq->getCommand() == "noisyInstMethod" )
        {
            CSleeperThread::msleep(10);
        }
        else if( pMsgReq != nullptr && pMsgReq->getCommand() == "veryNoisyInstMethod" )
        {
            CSleeperThread::msleep(10);
        }
        else if( pMsgReq != nullptr && pMsgReq->getCommand() == "recursiveTraceMethod" )
        {
            CSleeperThread::msleep(10);
        }

        QString strMthInArgs;

        if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
        {
            strMthInArgs = "{" + QString(pMsg == nullptr ? "null" : pMsg->getAddTrcInfoStr()) + "}";
        }

        CMethodTracer mthTracer(
            /* pTrcServer         */ m_pTrcAdminObj,
            /* iFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strMethod          */ "event",
            /* strMethodInArgs    */ strMthInArgs );

        bHandled = true;

        if( pMsgReq != nullptr )
        {
            if( pMsgReq->getCommand() == "instMethod" )
            {
                instMethod(pMsgReq->getCommandArg());
            }
            else if( pMsgReq->getCommand() == "noisyInstMethod" )
            {
                noisyInstMethod(pMsgReq->getCommandArg());
            }
            else if( pMsgReq->getCommand() == "veryNoisyInstMethod" )
            {
                veryNoisyInstMethod(pMsgReq->getCommandArg());
            }
            else if( pMsgReq->getCommand() == "recursiveTraceMethod" )
            {
                recursiveTraceMethod();
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
