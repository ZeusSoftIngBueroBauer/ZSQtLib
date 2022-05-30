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

#include <QtCore/qthread.h>

#include "MyClass1.h"
#include "MyClass2.h"

#include "ZSIpcTrace/ZSIpcTrcDllIf.h"

#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysWaitCondition.h"

#include "ZSSys/ZSSysMemLeakDump.h"

#ifdef __linux__
#include <unistd.h>
#endif

using namespace ZS::System;
using namespace ZS::Trace::DllIf;
using namespace ZS::Apps::Test::IpcTraceDllIfQtApp;


/*******************************************************************************
class CMyClass1 : public QObject
*******************************************************************************/

CTrcAdminObj* CMyClass1::s_pTrcAdminObj = nullptr;
int CMyClass1::s_iInstCount = 0;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CMyClass1::classMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    CTrcAdminObj* pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace().toLatin1().data(), ClassName().toLatin1().data());

    if( pTrcAdminObj != nullptr && pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "classMethod",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    strResult = "Hello World";

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet.toLatin1().data());
    }

    CTrcServer::ReleaseTraceAdminObj(pTrcAdminObj);
    pTrcAdminObj = nullptr;

    return strResult;

} // classMethod

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass1::CMyClass1( const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strMyClass2ObjName(),
    m_pMyClass2Thread(nullptr),
    m_pMyClass2(nullptr),
    m_pMtxWaitClass2ThreadRunning(nullptr),
    m_pWaitClass2ThreadRunning(nullptr)
{
    setObjectName(i_strObjName);

    if( s_iInstCount == 0 )
    {
        s_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
            NameSpace().toLatin1().data(), ClassName().toLatin1().data());
        if( s_pTrcAdminObj != nullptr )
        {
            s_pTrcAdminObj->setTraceDetailLevel(ETraceDetailLevelMethodArgs);
        }
    }

    ++s_iInstCount;

    QString strMthInArgs;

    if( s_pTrcAdminObj != nullptr && s_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szObjName    */ objectName().toLatin1().data(),
        /* szMethod     */ "ctor",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    m_pMtxWaitClass2ThreadRunning = new CMutex(ClassName() + "::" + objectName() + "::WaitClass2ThreadRunning");
    m_pWaitClass2ThreadRunning = new CWaitCondition(ClassName() + "::" + objectName() + "::Class2ThreadRunning");

} // ctor

//------------------------------------------------------------------------------
CMyClass1::~CMyClass1()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szObjName    */ objectName().toLatin1().data(),
        /* szMethod     */ "dtor",
        /* szMthInArgs  */ "" );

    emit aboutToBeDestroyed(this, objectName());

    stopClass2Thread();

    try
    {
        delete m_pMyClass2Thread;
    }
    catch(...)
    {
    }
    m_pMyClass2Thread = nullptr;
    m_pMyClass2 = nullptr;

    try
    {
        delete m_pWaitClass2ThreadRunning;
    }
    catch(...)
    {
    }
    m_pWaitClass2ThreadRunning = nullptr;

    try
    {
        delete m_pMtxWaitClass2ThreadRunning;
    }
    catch(...)
    {
    }
    m_pMtxWaitClass2ThreadRunning = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    --s_iInstCount;

    if( s_iInstCount == 0 )
    {
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObj);
        s_pTrcAdminObj = nullptr;
    }

} // dtor

/*==============================================================================
public: // instance methods (reimplementing methods of base class QObject)
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass1::setObjectName(const QString& i_strObjName)
//------------------------------------------------------------------------------
{
    // This class uses a class tracing trace admin object.
    // One trace admin object for all instances of this class.

    // !! The trace admin object does not have an object name
    // !! assigned and cannot be renamed.

    // Don't trace this method if initially called in the ctor.
    // If QObject::setObjectName() would be called after creating the method tracer
    // "CMyClass::setObjectName(Inst1)" would be output.
    // If QObject::setObjectName() would be called before creating the method tracer
    // "CMyClass::Inst1.setObjectName(Inst1)" would be output.
    // Tracing "setObjectName" makes only sense if an already existing object is renamed.

    // If called in the ctor the current object name is empty.
    // But an empty object name may be also a valid object name.
    // As a flag to indicate whether the method is called by the ctor an instance
    // member is used which will be created after calling "setObjectName".
    if( m_pMtxWaitClass2ThreadRunning == nullptr )
    {
        QObject::setObjectName(i_strObjName);
    }
    // If an already existing object has to be renamed the mutex is created.
    else
    {
        QString strMthInArgs;

        if( s_pTrcAdminObj != nullptr && s_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
        {
            strMthInArgs = i_strObjName;
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ s_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* szObjName    */ objectName().toLatin1().data(),
            /* szMethod     */ "setObjectName",
            /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

        QObject::setObjectName(i_strObjName);

        m_pMtxWaitClass2ThreadRunning->setObjectName(ClassName() + "::" + objectName() + "::WaitClass2ThreadRunning");
        m_pWaitClass2ThreadRunning->setObjectName(ClassName() + "::" + objectName() + "::Class2ThreadRunning");
    }

} // setObjectName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass2* CMyClass1::startClass2Thread(const QString& i_strMyClass2ObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( s_pTrcAdminObj != nullptr && s_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strMyClass2ObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szObjName    */ objectName().toLatin1().data(),
        /* szMethod     */ "startClass2Thread",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    m_strMyClass2ObjName = i_strMyClass2ObjName;

    if( m_pMyClass2Thread == nullptr )
    {
        m_pMyClass2Thread = new CMyClass2Thread(i_strMyClass2ObjName, this);

        QObject::connect(
            m_pMyClass2Thread, &CMyClass2Thread::running,
            this, &CMyClass1::onClass2ThreadRunning,
            Qt::DirectConnection);
    }

    if( !m_pMyClass2Thread->isRunning() )
    {
        m_pMyClass2Thread->start();

        // It is not sufficient just to wait for the wait condition to be signalled.
        // The thread may already have been started, created the Class3 instance and invoked
        // the "onClass3ThreadRunning" slot which signalled the wait condition. A wait here
        // without a timeout may therefore result in a deadlock. And in addition before and
        // after calling "wait" it will be checked whether the Class3 instance has been created.
        if( m_pMtxWaitClass2ThreadRunning->tryLock() )
        {
            while( m_pMyClass2Thread->getMyClass2() == nullptr )
            {
                m_pWaitClass2ThreadRunning->wait(m_pMtxWaitClass2ThreadRunning, 100);
            }
            m_pMtxWaitClass2ThreadRunning->unlock();
            m_pMyClass2 = m_pMyClass2Thread->getMyClass2();
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthRet = QString(m_pMyClass2 == nullptr ? "null" : m_pMyClass2->objectName());
        mthTracer.setMethodReturn(strMthRet.toLatin1().data());
    }

    return m_pMyClass2;

} // startClass2Thread

//------------------------------------------------------------------------------
void CMyClass1::stopClass2Thread()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ objectName().toLatin1().data(),
        /* szMethod     */ "stopClass2Thread",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    if( m_pMyClass2Thread != nullptr && m_pMyClass2Thread->isRunning() )
    {
        m_pMyClass2Thread->quit();

        // Let the thread quit the event loop (return from exec) before calling wait
        // to get the same trace output each time.
        CSleeperThread::msleep(50);

        int iTimeout_ms = 30000; // So the value can be modified during debugging session.
        if( !m_pMyClass2Thread->wait(iTimeout_ms) )
        {
            SErrResultInfo errResultInfo = ErrResultInfoError(
                "stopClass2Thread", EResultTimeout, "Waiting for thread to quit timed out");
            CErrLog::GetInstance()->addEntry(errResultInfo);
            m_pMyClass2Thread->terminate();
        }
    }

    delete m_pMyClass2Thread;
    m_pMyClass2Thread = nullptr;
    m_pMyClass2 = nullptr;

} // stopClass2Thread

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass1::onClass2ThreadRunning()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szObjName    */ objectName().toLatin1().data(),
        /* szMethod     */ "onClass2ThreadRunning",
        /* szMthInArgs  */ strMthInArgs.toLatin1().data() );

    m_pWaitClass2ThreadRunning->notify_all();
}
