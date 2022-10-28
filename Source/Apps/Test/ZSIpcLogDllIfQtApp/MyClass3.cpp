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

#include "MyClass3.h"
#include "MyClass2.h"
#include "MyClass1.h"
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
    m_pLogger(nullptr)
{
    setObjectName("MyClass3Thread" + m_strMyClass3ObjName);

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
        strMthInArgs = i_strMyClass3ObjName;
        strMthInArgs += ", " + QString(i_pMyClass2 == nullptr ? "nullptr" : i_pMyClass2->objectName());
        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("ctor(" + strMthInArgs + ")").toStdString().c_str());
    }

} // ctor

//------------------------------------------------------------------------------
CMyClass3Thread::~CMyClass3Thread()
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

    m_pLogger = nullptr;

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
    // To always get the same trace output. Sleep a bit to let the thread starting
    // instance wait on the wait condition.
    CSleeperThread::msleep(5);

    if( m_pLogger != nullptr )
    {
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "run");
    }

    m_pMyClass3 = new CMyClass3(m_strMyClass3ObjName, this);

    if( !QObject::connect(
        /* pObjSender   */ m_pMyClass3,
        /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onClass3AboutToBeDestroyed(QObject*, const QString&)),
        /* cnctType     */ Qt::DirectConnection) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    emit running();

    // To always get the same trace output. Sleep a bit to let the thread starting
    // instance wait on the wait condition.
    CSleeperThread::msleep(5);

    exec();

    QObject::disconnect(
        /* pObjSender   */ m_pMyClass3,
        /* szSignal     */ SIGNAL(aboutToBeDestroyed(QObject*, const QString&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onClass3AboutToBeDestroyed(QObject*, const QString&)) );

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
void CMyClass3Thread::quit()
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
bool CMyClass3Thread::wait( QDeadlineTimer i_deadline )
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        QString strMthInArgs = QString(i_deadline.isForever() ? "Forever" : QString::number(i_deadline.deadline()));
        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("start(" + strMthInArgs + ")").toStdString().c_str());
    }
    return QThread::wait(i_deadline);
}
#endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)

//------------------------------------------------------------------------------
bool CMyClass3Thread::wait( unsigned long i_time_ms )
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        QString strMthInArgs = QString::number(i_time_ms) + " ms";
        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("start(" + strMthInArgs + ")").toStdString().c_str());
    }
    return QThread::wait(i_time_ms);
}

/*==============================================================================
protected: // replacing method of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
int CMyClass3Thread::exec()
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "exec");
    }
    return QThread::exec();
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

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass3::CMyClass3( const QString& i_strObjName, CMyClass3Thread* i_pMyClass3Thread ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMyClass3Thread(i_pMyClass3Thread),
    m_pMtxCounters(nullptr),
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
        strMthInArgs += ", " + QString(i_pMyClass3Thread == nullptr ? "nullptr" : i_pMyClass3Thread->objectName());
        m_pLogger->log(
            Log::DllIf::ELogDetailLevelDebug,
            QString("ctor(" + strMthInArgs + ")").toStdString().c_str());
    }

    m_pMtxCounters = new CMutex(QMutex::Recursive, ClassName() + "::" + objectName() + "::Counters");

} // ctor

//------------------------------------------------------------------------------
CMyClass3::~CMyClass3()
//------------------------------------------------------------------------------
{
    if( m_pLogger != nullptr )
    {
        m_pLogger->log(Log::DllIf::ELogDetailLevelDebug, "dtor");
    }

    emit aboutToBeDestroyed(this, objectName());

    try
    {
        delete m_pMtxCounters;
    }
    catch(...)
    {
    }
    m_pMtxCounters = nullptr;

    m_pMyClass3Thread = nullptr;
    m_pLogger = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass3::sendMuchData()
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
        QJsonObject jsonObj;

        jsonObj.insert("Hello", "Class3");
        jsonObj.insert("Zahl1", 1.0);
        jsonObj.insert("Text1", "Text1");
        jsonObj.insert("Zahl2", 2.0);
        jsonObj.insert("Text2", "Text2");
        jsonObj.insert("Hello", "Class3");

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
    }
}

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
        if( pMsgReq != nullptr && pMsgReq->getCommand() == "recursiveTraceMethod" )
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

        bHandled = true;

        if( pMsgReq != nullptr )
        {
            if( pMsgReq->getCommand() == "sendMuchData" )
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
int CMyClass3::sendData1( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
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
int CMyClass3::sendData2( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
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
int CMyClass3::sendData3( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
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
