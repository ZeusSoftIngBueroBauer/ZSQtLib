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

#include "MyClass2.h"
#include "MyClass1.h"
#include "MyClass3.h"
#include "App.h"
#include "MsgTest.h"

#include "ZSIpcLog/ZSIpcLogServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysWaitCondition.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Apps::Test::IpcLog;


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
    m_pLogger(nullptr)
{
    setObjectName("MyClass2Thread" + m_strMyClass2ObjName);

    m_pMtxWaitForClass2Created = new CMutex(ClassName() + "::" + objectName() + "::WaitClass2Created");
    m_pWaitConditionClass2Created = new CWaitCondition(ClassName() + "::" + objectName() + "::WaitClass2Created");

} // ctor

//------------------------------------------------------------------------------
CMyClass2Thread::~CMyClass2Thread()
//------------------------------------------------------------------------------
{
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

    QObject::setObjectName(i_strObjName);

    if( m_pMtxWaitForClass2Created != nullptr )
    {
        m_pMtxWaitForClass2Created->setObjectName(ClassName() + "::" + objectName() + "::WaitClass2Created");
    }
    if( m_pWaitConditionClass2Created != nullptr )
    {
        m_pWaitConditionClass2Created->setObjectName(ClassName() + "::" + objectName() + "::WaitClass2Created");
    }

    // Should be the last so that the method tracer traces leave method
    // not before the child objects have been renamed.
    if( m_pLogger != nullptr )
    {
        CLogServer::RenameLogger(&m_pLogger, objectName());
    }

} // setObjectName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass2* CMyClass2Thread::waitForMyClass2Created()
//------------------------------------------------------------------------------
{
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

    m_pMyClass2 = new CMyClass2(m_strMyClass2ObjName, this);

    if( !QObject::connect(
        /* pObjSender   */ m_pMyClass2,
        /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onClass2AboutToBeDestroyed(QObject*, const QString&)),
        /* cnctType     */ Qt::DirectConnection) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // To always get the same trace output. Sleep a bit to let the thread starting
    // instance wait on the wait condition.
    CSleeperThread::msleep(5);

    exec();

    QObject::disconnect(
        /* pObjSender   */ m_pMyClass2,
        /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onClass2AboutToBeDestroyed(QObject*, const QString&)) );

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
    return QThread::start(i_priority);
}

//------------------------------------------------------------------------------
void CMyClass2Thread::quit()
//------------------------------------------------------------------------------
{
    return QThread::quit();
}

//------------------------------------------------------------------------------
bool CMyClass2Thread::wait( QDeadlineTimer i_deadline )
//------------------------------------------------------------------------------
{
    bool bResult = QThread::wait(i_deadline);
    return bResult;
}

//------------------------------------------------------------------------------
bool CMyClass2Thread::wait( unsigned long i_time_ms )
//------------------------------------------------------------------------------
{
    bool bResult = QThread::wait(i_time_ms);
    return bResult;
}

/*==============================================================================
protected: // replacing method of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
int CMyClass2Thread::exec()
//------------------------------------------------------------------------------
{
    int iResult = QThread::exec();
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
    m_pLogger(nullptr)
{
    setObjectName(i_strObjName);

    m_pTmrMessages = new QTimer(this);

    if( !QObject::connect(
        /* szSender   */ m_pTmrMessages,
        /* szSignal   */ SIGNAL(timeout()),
        /* szReceiver */ this,
        /* szSlot     */ SLOT(onTmrMessagesTimeout())) )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultSignalSlotConnectionFailed);
    }

    m_pMtxCounters = new CMutex(QMutex::Recursive, ClassName() + "::" + objectName() + "::Counters");
    m_pMtxWaitClass3ThreadRunning = new CMutex(ClassName() + "::" + objectName() + "::WaitClass3ThreadRunning");
    m_pWaitClass3ThreadRunning = new CWaitCondition(ClassName() + "::" + objectName() + "::Class3ThreadRunning");

} // ctor

//------------------------------------------------------------------------------
CMyClass2::~CMyClass2()
//------------------------------------------------------------------------------
{
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

    m_pMyClass2Thread = nullptr;
    m_pTmrMessages = nullptr;
    m_iRecursionCount = 0;
    m_iMsgCount = 0;
    m_pLogger = nullptr;

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

    QObject::setObjectName(i_strObjName);

    if( m_pMtxCounters != nullptr )
    {
        m_pMtxCounters->setObjectName(ClassName() + "::" + objectName() + "::Counters");
    }
    if( m_pMtxWaitClass3ThreadRunning != nullptr )
    {
        m_pMtxWaitClass3ThreadRunning->setObjectName(ClassName() + "::" + objectName() + "::WaitClass3ThreadRunning");
    }
    if( m_pWaitClass3ThreadRunning != nullptr )
    {
        m_pWaitClass3ThreadRunning->setObjectName(ClassName() + "::" + objectName() + "::Class3ThreadRunning");
    }

    // Should be the last so that the method tracer traces leave method
    // not before the child objects have been renamed.
    if( m_pLogger != nullptr )
    {
        CLogServer::RenameLogger(&m_pLogger, objectName());
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
    return strResult;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CMyClass2::recursiveTraceMethod()
//------------------------------------------------------------------------------
{
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
        ++m_iRecursionCount;

        if( m_iRecursionCount <= 10 )
        {
            recursiveTraceMethod();
        }
        --m_iRecursionCount;
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

           if( !QObject::connect(
                /* pObjSender   */ m_pMyClass3Thread,
                /* szSignal     */ SIGNAL(running()),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onClass3ThreadRunning()),
                /* cnctType     */ Qt::DirectConnection) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
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

    return m_pMyClass3;

} // startClass3Thread

//------------------------------------------------------------------------------
void CMyClass2::stopClass3Thread()
//------------------------------------------------------------------------------
{
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
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass2::onClass3ThreadRunning()
//------------------------------------------------------------------------------
{
    m_pWaitClass3ThreadRunning->notify_all();
}

//------------------------------------------------------------------------------
void CMyClass2::onTmrMessagesTimeout()
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtxCounters);

    ++m_iMsgCount;

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
            bHandled = true;
        }
    }

    if( !bHandled )
    {
        bHandled = QObject::event(i_pEv);
    }

    return bHandled;

} // event
