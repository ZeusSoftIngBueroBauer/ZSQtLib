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
    setObjectName(ClassName() + m_strMyClass3ObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        ClassName().toLatin1().data(),
        objectName().toLatin1().data());

    m_pTrcAdminObj->setMethodCallsTraceDetailLevel(ETraceDetailLevelMethodCallsArgsNormal);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= ETraceDetailLevelMethodCallsArgsNormal )
    {
        strMthInArgs = i_strMyClass3ObjName;
        strMthInArgs += ", " + QString(i_pMyClass2 == nullptr ? "nullptr" : i_pMyClass2->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "ctor",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

} // ctor

//------------------------------------------------------------------------------
CMyClass3Thread::~CMyClass3Thread()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "dtor",
        /* szMthInArgs  */ "" );

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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthInArgs = i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
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

        CTrcServer::RenameTraceAdminObj(&m_pTrcAdminObj, objectName().toLatin1().data());
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
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthInArgs = qThreadPriority2Str(i_priority);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "start",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    return QThread::start(i_priority);
}

//------------------------------------------------------------------------------
void CMyClass3Thread::quit()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "quit",
        /* szMthInArgs  */ "" );

    return QThread::quit();
}

//------------------------------------------------------------------------------
bool CMyClass3Thread::wait( QDeadlineTimer i_deadline )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthInArgs = QString(i_deadline.isForever() ? "Forever" : QString::number(i_deadline.deadline()));
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "wait",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    bool bResult = QThread::wait(i_deadline);

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        mthTracer.setMethodReturn(bResult);
    }
    return bResult;
}

//------------------------------------------------------------------------------
bool CMyClass3Thread::wait( unsigned long i_time_ms )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthInArgs = QString::number(i_time_ms) + " ms";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "wait",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    bool bResult = QThread::wait(i_time_ms);

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
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
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "exec",
        /* szMthInArgs  */ "" );

    int iResult = QThread::exec();

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
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

    CTrcAdminObj* pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        ClassName().toLatin1().data());

    if( pTrcAdminObj != nullptr && pTrcAdminObj->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "classMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    strResult = "Hello World";

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet.toLatin1().data());
    }

    CTrcServer::ReleaseTraceAdminObj(pTrcAdminObj);
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

    CTrcAdminObj* pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        QString(ClassName() + + "::NoisyMethods").toLatin1().data());

    if( pTrcAdminObj != nullptr && pTrcAdminObj->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "noisyClassMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    strResult = "Hello World";

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet.toLatin1().data());
    }

    CTrcServer::ReleaseTraceAdminObj(pTrcAdminObj);
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

    CTrcAdminObj* pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        QString(ClassName() + + "::VeryNoisyMethods").toLatin1().data());

    if( pTrcAdminObj != nullptr && pTrcAdminObj->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "veryNoisyClassMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    strResult = "Hello World";

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet.toLatin1().data());
    }

    CTrcServer::ReleaseTraceAdminObj(pTrcAdminObj);
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

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        ClassName().toLatin1().data(),
        objectName().toLatin1().data());
    m_pTrcAdminObjNoisyMethods = CTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        QString(ClassName() + "::NoisyMethods").toLatin1().data(),
        objectName().toLatin1().data());
    m_pTrcAdminObjVeryNoisyMethods = CTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(),
        QString(ClassName() + "::VeryNoisyMethods").toLatin1().data(),
        objectName().toLatin1().data());

    m_pTrcAdminObj->setMethodCallsTraceDetailLevel(ETraceDetailLevelMethodCallsArgsNormal);
    m_pTrcAdminObjNoisyMethods->setMethodCallsTraceDetailLevel(ETraceDetailLevelMethodCallsArgsNormal);
    m_pTrcAdminObjVeryNoisyMethods->setMethodCallsTraceDetailLevel(ETraceDetailLevelMethodCallsArgsNormal);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", " + QString(i_pMyClass3Thread == nullptr ? "nullptr" : i_pMyClass3Thread->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "ctor",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    m_pMtxCounters = new CMutex(QMutex::Recursive, ClassName() + "::" + objectName() + "::Counters");

} // ctor

//------------------------------------------------------------------------------
CMyClass3::~CMyClass3()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthInArgs = i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "setObjectName",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    QObject::setObjectName(i_strObjName);

    if( m_pMtxCounters != nullptr )
    {
        m_pMtxCounters->setObjectName(ClassName() + "::" + objectName() + "::Counters");
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

        CTrcServer::RenameTraceAdminObj(&m_pTrcAdminObj, objectName().toLatin1().data());
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "instMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    strResult = "Hello World";

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
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

    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "noisyInstMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    strResult = "Hello World";

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
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

    if( m_pTrcAdminObjVeryNoisyMethods != nullptr && m_pTrcAdminObjVeryNoisyMethods->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjVeryNoisyMethods,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
        /* szMethod     */ "veryNoisyInstMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    strResult = "Hello World";

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
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
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
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
        if( mthTracer.isRuntimeInfoActive(ETraceDetailLevelRuntimeInfoDebugNormal) )
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

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
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
        if( pMsgReq != nullptr && pMsgReq->getCommand() == "recursiveTraceMethod" )
        {
            CSleeperThread::msleep(10);
        }

        QString strMthInArgs;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(ETraceDetailLevelMethodCallsArgsNormal) )
        {
            strMthInArgs = "{" + QString(pMsg == nullptr ? "null" : pMsg->getAddTrcInfoStr(m_pTrcAdminObj->getMethodCallsTraceDetailLevel())) + "}";
        }

        CMethodTracer mthTracer(
            /* pTrcServer         */ m_pTrcAdminObj,
            /* iFilterDetailLevel */ ETraceDetailLevelMethodCallsEnterLeave,
            /* strMethod          */ "event",
            /* strMethodInArgs    */ strMthInArgs.toLatin1().data() );

        bHandled = true;

        if( pMsgReq != nullptr )
        {
            if( pMsgReq->getCommand() == "recursiveTraceMethod" )
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
