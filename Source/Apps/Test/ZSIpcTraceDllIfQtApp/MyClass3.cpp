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

#include "MyClass3.h"
#include "MyClass2.h"
#include "MyClass1.h"
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

    m_pTrcAdminObj = Trace::DllIf::CTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        ClassName().toLatin1().data(),
        objectName().toLatin1().data());

    m_pTrcAdminObj->setMethodCallsTraceDetailLevel(EMethodTraceDetailLevelArgsNormal);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevelArgsNormal )
    {
        strMthInArgs = i_strMyClass3ObjName;
        strMthInArgs += ", " + QString(i_pMyClass2 == nullptr ? "nullptr" : i_pMyClass2->objectName());
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "ctor",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

} // ctor

//------------------------------------------------------------------------------
CMyClass3Thread::~CMyClass3Thread()
//------------------------------------------------------------------------------
{
    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "dtor",
        /* szMthInArgs  */ "" );

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

    Trace::DllIf::CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
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

    if( m_pTrcAdminObj != nullptr )
    {
        // The Dll Interface is Qt free and there are no signals and slots.
        // The method tracer will not be informed if the trace admin object is going
        // to be deleted. To avoid a crash with accessing a deleted object invalidate
        // the trace admin object at the method tracer before renaming the trace admin
        // object (as this may delete the admin object of the Dll Interface).
        mthTracer.onAdminObjAboutToBeReleased();

        m_pTrcAdminObj = Trace::DllIf::CTrcServer::RenameTraceAdminObj(
            m_pTrcAdminObj, objectName().toLatin1().data());
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

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "run",
        /* szMthInArgs  */ "" );

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

    m_pMyClass3->recursiveTraceMethod();

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
void CMyClass3Thread::quit()
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
bool CMyClass3Thread::wait( QDeadlineTimer i_deadline )
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
#endif // #if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)

//------------------------------------------------------------------------------
bool CMyClass3Thread::wait( unsigned long i_time_ms )
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
int CMyClass3Thread::exec()
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
void CMyClass3Thread::onClass3AboutToBeDestroyed(QObject* i_pObj, const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    m_pMyClass3 = nullptr;
}


/*******************************************************************************
class CMyClass3 : public QObject
*******************************************************************************/

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

    Trace::DllIf::CTrcAdminObj* pTrcAdminObj =
        Trace::DllIf::CTrcServer::GetTraceAdminObj(
            NameSpace().toLatin1().data(),
            ClassName().toLatin1().data());

    if( pTrcAdminObj != nullptr && pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "classMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    strResult = "Hello World";

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet.toLatin1().data());
    }

    Trace::DllIf::CTrcServer::ReleaseTraceAdminObj(pTrcAdminObj);
    pTrcAdminObj = nullptr;

    return strResult;

} // classMethod

//------------------------------------------------------------------------------
QString CMyClass3::noisyClassMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    Trace::DllIf::CTrcAdminObj* pTrcAdminObj =
        Trace::DllIf::CTrcServer::GetTraceAdminObj(
            NameSpace().toLatin1().data(),
            QString(ClassName() + + "::NoisyMethods").toLatin1().data());

    if( pTrcAdminObj != nullptr && pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "noisyClassMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    strResult = "Hello World";

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet.toLatin1().data());
    }

    Trace::DllIf::CTrcServer::ReleaseTraceAdminObj(pTrcAdminObj);
    pTrcAdminObj = nullptr;

    return strResult;

} // noisyClassMethod

//------------------------------------------------------------------------------
QString CMyClass3::veryNoisyClassMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    Trace::DllIf::CTrcAdminObj* pTrcAdminObj =
        Trace::DllIf::CTrcServer::GetTraceAdminObj(
            NameSpace().toLatin1().data(),
            QString(ClassName() + + "::VeryNoisyMethods").toLatin1().data());

    if( pTrcAdminObj != nullptr && pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "veryNoisyClassMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    strResult = "Hello World";

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet.toLatin1().data());
    }

    Trace::DllIf::CTrcServer::ReleaseTraceAdminObj(pTrcAdminObj);
    pTrcAdminObj = nullptr;

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

    m_pTrcAdminObj = Trace::DllIf::CTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        ClassName().toLatin1().data(),
        objectName().toLatin1().data());
    m_pTrcAdminObjNoisyMethods = Trace::DllIf::CTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        QString(ClassName() + "::NoisyMethods").toLatin1().data(),
        objectName().toLatin1().data());
    m_pTrcAdminObjVeryNoisyMethods = Trace::DllIf::CTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        QString(ClassName() + "::VeryNoisyMethods").toLatin1().data(),
        objectName().toLatin1().data());

    m_pTrcAdminObj->setMethodCallsTraceDetailLevel(EMethodTraceDetailLevelArgsNormal);
    m_pTrcAdminObjNoisyMethods->setMethodCallsTraceDetailLevel(EMethodTraceDetailLevelArgsNormal);
    m_pTrcAdminObjVeryNoisyMethods->setMethodCallsTraceDetailLevel(EMethodTraceDetailLevelArgsNormal);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", " + QString(i_pMyClass3Thread == nullptr ? "nullptr" : i_pMyClass3Thread->objectName());
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "ctor",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    m_pMtxCounters = new CRecursiveMutex(ClassName() + "-" + objectName() + "-Counters");

} // ctor

//------------------------------------------------------------------------------
CMyClass3::~CMyClass3()
//------------------------------------------------------------------------------
{
    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "dtor",
        /* szMthInArgs  */ "" );

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

    Trace::DllIf::CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    Trace::DllIf::CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);
    Trace::DllIf::CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjVeryNoisyMethods);

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

    // Should be the last so that the method tracer traces leave method
    // not before the child objects have been renamed.
    if( m_pTrcAdminObj != nullptr )
    {
        // The Dll Interface is Qt free and there are no signals and slots.
        // The method tracer will not be informed if the trace admin object is going
        // to be deleted. To avoid a crash with accessing a deleted object invalidate
        // the trace admin object at the method tracer before renaming the trace admin
        // object (as this may delete the admin object of the Dll Interface).
        mthTracer.onAdminObjAboutToBeReleased();

        m_pTrcAdminObj = Trace::DllIf::CTrcServer::RenameTraceAdminObj(
            m_pTrcAdminObj, objectName().toLatin1().data());
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

//------------------------------------------------------------------------------
QString CMyClass3::noisyInstMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "noisyInstMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    if( QThread::currentThread() != thread() )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
        pMsgReq->setCommand("noisyInstMethod");
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

} // noisyInstMethod

//------------------------------------------------------------------------------
QString CMyClass3::veryNoisyInstMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    if( m_pTrcAdminObjVeryNoisyMethods != nullptr && m_pTrcAdminObjVeryNoisyMethods->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    Trace::DllIf::CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjVeryNoisyMethods,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* szMethod     */ "veryNoisyInstMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    if( QThread::currentThread() != thread() )
    {
        CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
        pMsgReq->setCommand("veryNoisyInstMethod");
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

} // veryNoisyInstMethod

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CMyClass3::recursiveTraceMethod()
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

//------------------------------------------------------------------------------
void CMyClass3::sendMuchData()
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

            if( mthTracer.isRuntimeInfoActive(Trace::DllIf::ELogDetailLevelDebugVerbose) )
            {
                strAddInfo = "Im sending data now for " + QString::number(fCurrTime_s - fStartTime_s, 'f', 3) + " seconds. ";
                mthTracer.trace(strAddInfo.toLatin1().data());
            }
            fCurrTime_s = System::Time::getProcTimeInSec();
            sendData1(QJsonDocument(jsonObj).toJson(QJsonDocument::Compact), fStartTime_s, fCurrTime_s, fDuration_s);
        }

        if( m_pTrcAdminObj != nullptr )
        {
            m_pTrcAdminObj->setRuntimeInfoTraceDetailLevel(detailLevelPrev);
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

        QString strMthInArgs;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
        {
            strMthInArgs = "{" + QString(pMsg == nullptr ? "null" : pMsg->getAddTrcInfoStr(m_pTrcAdminObj->getMethodCallsTraceDetailLevel())) + "}";
        }

        Trace::DllIf::CMethodTracer mthTracer(
            /* pTrcServer         */ m_pTrcAdminObj,
            /* iFilterDetailLevel */ EMethodTraceDetailLevelEnterLeave,
            /* strMethod          */ "event",
            /* strMethodInArgs    */ strMthInArgs.toLatin1().data() );

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
int CMyClass3::sendData1( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
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
int CMyClass3::sendData2( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
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
int CMyClass3::sendData3( const QString& i_strData, double i_fStartTime_s, double i_fCurrTime_s, double i_fDuration_s )
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
