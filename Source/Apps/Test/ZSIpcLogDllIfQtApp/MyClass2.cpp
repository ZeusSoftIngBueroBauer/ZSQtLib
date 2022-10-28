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
#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonobject.h>

#include "MyClass2.h"
#include "MyClass1.h"
#include "MyClass3.h"
#include "App.h"
#include "MsgTest.h"

#include "ZSIpcLog/ZSIpcLogDllIf.h"

#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysWaitCondition.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Log::DllIf;
using namespace ZS::Apps::Test::IpcLogDllIfQtApp;


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

    QString strLoggerName = NameSpace() + "::" + ClassName() + "::" + objectName();
    m_pLogger = Log::DllIf::CLogServer::GetLogger(strLoggerName.toLatin1().data());

    if( m_pLogger != nullptr )
    {
        m_pLogger->setLogLevel(Log::DllIf::ELogDetailLevelDebug);
        m_pLogger->setAddThreadName(true);
        m_pLogger->setAddDateTime(true);
        m_pLogger->setAddSystemTime(true);
        m_pLogger->setNameSpace(NameSpace().toLatin1().data());
        m_pLogger->setClassName(ClassName().toLatin1().data());
        m_pLogger->setObjectName(objectName().toLatin1().data());

        QString strMthInArgs;
        strMthInArgs = i_strMyClass2ObjName;
        strMthInArgs += ", " + QString(i_pMyClass1 == nullptr ? "nullptr" : i_pMyClass1->objectName());
        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("ctor(" + strMthInArgs + ")").toStdString().c_str());
    }

    m_pMtxWaitForClass2Created = new CMutex(ClassName() + "::" + objectName() + "::WaitClass2Created");
    m_pWaitConditionClass2Created = new CWaitCondition(ClassName() + "::" + objectName() + "::WaitClass2Created");

} // ctor

//------------------------------------------------------------------------------
CMyClass2Thread::~CMyClass2Thread()
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "dtor");
    }

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

    m_pLogger = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass2* CMyClass2Thread::waitForMyClass2Created()
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "waitForMyClass2Created");
    }

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

    if( m_pLogger != nullptr )
    {
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "run");
    }

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
    if( m_pLogger != nullptr )
    {
        QString strMthInArgs = qThreadPriority2Str(i_priority);
        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("start(" + strMthInArgs + ")").toStdString().c_str());
    }
    return QThread::start(i_priority);
}

//------------------------------------------------------------------------------
void CMyClass2Thread::quit()
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "quit");
    }
    return QThread::quit();
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
//------------------------------------------------------------------------------
bool CMyClass2Thread::wait( QDeadlineTimer i_deadline )
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        QString strMthInArgs = QString(i_deadline.isForever() ? "Forever" : QString::number(i_deadline.deadline()));
        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("wait(" + strMthInArgs + ")").toStdString().c_str());
    }
    return QThread::wait(i_deadline);
}
#endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)

//------------------------------------------------------------------------------
bool CMyClass2Thread::wait( unsigned long i_time_ms )
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        QString strMthInArgs = QString::number(i_time_ms) + " ms";
        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("wait(" + strMthInArgs + ")").toStdString().c_str());
    }
    return QThread::wait(i_time_ms);
}

/*==============================================================================
protected: // replacing method of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
int CMyClass2Thread::exec()
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "exec");
    }
    return QThread::exec();
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
    m_pMtxCounters(nullptr),
    m_strMyClass3ObjName(),
    m_pMyClass3Thread(nullptr),
    m_pMyClass3(nullptr),
    m_pMtxWaitClass3ThreadRunning(nullptr),
    m_pWaitClass3ThreadRunning(nullptr),
    m_pLogger(nullptr)
{
    setObjectName(i_strObjName);

    QString strLoggerName = NameSpace() + "::" + ClassName();
    m_pLogger = Log::DllIf::CLogServer::GetLogger(strLoggerName.toLatin1().data());
    if( m_pLogger != nullptr )
    {
        m_pLogger->setLogLevel(Log::DllIf::ELogDetailLevelDebug);
        m_pLogger->setAddThreadName(true);
        m_pLogger->setAddDateTime(true);
        m_pLogger->setAddSystemTime(true);
        m_pLogger->setNameSpace(NameSpace().toLatin1().data());
        m_pLogger->setClassName(ClassName().toLatin1().data());

        QString strMthInArgs;
        strMthInArgs = i_strObjName;
        strMthInArgs += ", " + QString(i_pMyClass2Thread == nullptr ? "nullptr" : i_pMyClass2Thread->objectName());
        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("ctor(" + strMthInArgs + ")").toStdString().c_str());
    }

    m_pMtxCounters = new CMutex(QMutex::Recursive, ClassName() + "::" + objectName() + "::Counters");
    m_pMtxWaitClass3ThreadRunning = new CMutex(ClassName() + "::" + objectName() + "::WaitClass3ThreadRunning");
    m_pWaitClass3ThreadRunning = new CWaitCondition(ClassName() + "::" + objectName() + "::Class3ThreadRunning");

} // ctor

//------------------------------------------------------------------------------
CMyClass2::~CMyClass2()
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "dtor");
    }

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
    m_pLogger = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass3* CMyClass2::startClass3Thread(const QString& i_strMyClass3ObjName)
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        QString strMthInArgs = i_strMyClass3ObjName;
        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("startClass3Thread(" + strMthInArgs + ")").toStdString().c_str());
    }

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
    if( m_pLogger != nullptr )
    {
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "stopClass3Thread");
    }

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
    if( m_pLogger != nullptr )
    {
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "sendMuchData");
    }

    if( QThread::currentThread() != thread() )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
        pMsgReq->setCommand("sendMuchData");
        POST_OR_DELETE_MESSAGE(pMsgReq);
        pMsgReq = nullptr;
    }
    else
    {
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

            if( m_pLogger != nullptr )
            {
                QString strLog = "Im sending data now for " + QString::number(fCurrTime_s - fStartTime_s, 'f', 3) + " seconds. ";
                m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, strLog.toLatin1().data());
            }
            fCurrTime_s = System::Time::getProcTimeInSec();
            sendData1(QJsonDocument(jsonObj).toJson(QJsonDocument::Compact), fStartTime_s, fCurrTime_s, fDuration_s);
        }

        stopClass3Thread();
        pMyClass3 = nullptr;
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass2::onClass3ThreadRunning()
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "onClass3ThreadRunning");
    }
    m_pWaitClass3ThreadRunning->notify_all();
}

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
        if( pMsgReq != nullptr && pMsgReq->getCommand() == "startClass3Thread" )
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

        if( m_pLogger != nullptr )
        {
            QString strMthInArgs = "{" + QString(pMsg == nullptr ? "null" : pMsg->getAddTrcInfoStr()) + "}";
            m_pLogger->log(
                Log::DllIf::ELogDetailLevelDebug,
                QString("event(" + strMthInArgs + ")").toStdString().c_str());
        }

        if( pMsgReq != nullptr )
        {
            if( pMsgReq->getCommand() == "startClass3Thread" )
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

    if( m_pLogger != nullptr )
    {
        QString strMthInArgs;
        strMthInArgs = "Data: " + i_strData;
        strMthInArgs += ", StartTime: " + QString::number(i_fStartTime_s) + "s";
        strMthInArgs += ", CurrTime: " + QString::number(i_fStartTime_s) + "s";
        strMthInArgs += ", Duration: " + QString::number(i_fStartTime_s) + "s";

        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("sendData1(" + strMthInArgs + ")").toStdString().c_str());
    }

    ++s_iCount;

    sendData2(i_strData, i_fStartTime_s, i_fCurrTime_s, i_fDuration_s);

    if( m_pLogger != nullptr )
    {
        QString strLog = "Time remaining: " + QString::number(i_fDuration_s - (i_fCurrTime_s - i_fStartTime_s), 'f', 3) + " seconds.";
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, strLog.toStdString().c_str());
    }
    return s_iCount;
}

//------------------------------------------------------------------------------
int CMyClass2::sendData2( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
//------------------------------------------------------------------------------
{
    static int s_iCount = 0;

    if( m_pLogger != nullptr )
    {
        QString strMthInArgs;
        strMthInArgs = "Data: " + i_strData;
        strMthInArgs += ", StartTime: " + QString::number(i_fStartTime_s) + "s";
        strMthInArgs += ", CurrTime: " + QString::number(i_fStartTime_s) + "s";
        strMthInArgs += ", Duration: " + QString::number(i_fStartTime_s) + "s";

        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("sendData2(" + strMthInArgs + ")").toStdString().c_str());
    }

    ++s_iCount;

    sendData3(i_strData, i_fStartTime_s, i_fCurrTime_s, i_fDuration_s);

    if( m_pLogger != nullptr )
    {
        QString strLog = "Time remaining: " + QString::number(i_fDuration_s - (i_fCurrTime_s - i_fStartTime_s), 'f', 3) + " seconds.";
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, strLog.toStdString().c_str());
    }
    return s_iCount;
}

//------------------------------------------------------------------------------
int CMyClass2::sendData3( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
//------------------------------------------------------------------------------
{
    static int s_iCount = 0;

    if( m_pLogger != nullptr )
    {
        QString strMthInArgs;
        strMthInArgs = "Data: " + i_strData;
        strMthInArgs += ", StartTime: " + QString::number(i_fStartTime_s) + "s";
        strMthInArgs += ", CurrTime: " + QString::number(i_fStartTime_s) + "s";
        strMthInArgs += ", Duration: " + QString::number(i_fStartTime_s) + "s";

        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("sendData3(" + strMthInArgs + ")").toStdString().c_str());
    }

    ++s_iCount;

    if( m_pLogger != nullptr )
    {
        QString strLog = "Time remaining: " + QString::number(i_fDuration_s - (i_fCurrTime_s - i_fStartTime_s), 'f', 3) + " seconds.";
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, strLog.toStdString().c_str());
    }
    return s_iCount;
}
