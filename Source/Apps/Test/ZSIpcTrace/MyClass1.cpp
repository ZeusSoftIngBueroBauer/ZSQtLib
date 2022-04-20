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
#include "App.h"

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysWaitCondition.h"

#include "ZSSys/ZSSysMemLeakDump.h"

#ifdef __linux__
#include <unistd.h>
#endif

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::IpcTrace;


/*******************************************************************************
class CMyClass1 : public QObject
*******************************************************************************/

CTrcAdminObjRefAnchor CMyClass1::s_trcAdminObjRefAnchor(
    CMyClass1::NameSpace(), CMyClass1::ClassName());

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

    CTrcAdminObjRefGuard trcAdminObjGuard(&s_trcAdminObjRefAnchor);

    if( trcAdminObjGuard.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ trcAdminObjGuard.trcAdminObj(),
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "classMethod",
        /* strAddInfo   */ strMthInArgs );

    strResult = "Hello World";

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet);
    }

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

    s_trcAdminObjRefAnchor.allocTrcAdminObj();

    s_trcAdminObjRefAnchor.setTraceDetailLevel(ETraceDetailLevelMethodArgs);

    QString strMthInArgs;

    if( s_trcAdminObjRefAnchor.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ objectName(),
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_pMtxWaitClass2ThreadRunning = new CMutex(ClassName() + "::" + i_strObjName + "::WaitClass2ThreadRunning");
    m_pWaitClass2ThreadRunning = new CWaitCondition(ClassName() + "::" + i_strObjName + "::Class2ThreadRunning");

} // ctor

//------------------------------------------------------------------------------
CMyClass1::~CMyClass1()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ objectName(),
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    emit aboutToBeDestroyed(objectName());

    if( m_pMyClass2Thread != nullptr && m_pMyClass2Thread->isRunning() )
    {
        m_pMyClass2Thread->quit();

        if( !m_pMyClass2Thread->wait(1000) )
        {
            m_pMyClass2Thread->terminate();
        }
    }

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

    s_trcAdminObjRefAnchor.releaseTrcAdminObj();

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass2* CMyClass1::startClass2Thread(const QString& i_strMyClass2ObjName)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( s_trcAdminObjRefAnchor.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = i_strMyClass2ObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ objectName(),
        /* strMethod    */ "startClass2Thread",
        /* strAddInfo   */ strMthInArgs );

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
        mthTracer.setMethodReturn(strMthRet);
    }

    return m_pMyClass2;

} // startClass2Thread

//------------------------------------------------------------------------------
void CMyClass1::stopClass2Thread()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ objectName(),
        /* strMethod    */ "stopClass2Thread",
        /* strAddInfo   */ strMthInArgs );

    if( m_pMyClass2Thread != nullptr && m_pMyClass2Thread->isRunning() )
    {
        m_pMyClass2Thread->quit();
        // Let the thread quit the event loop (return from exec) before calling wait
        // to get the same trace output each time.
        CSleeperThread::msleep(50);
        m_pMyClass2Thread->wait();
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
        /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ objectName(),
        /* strMethod    */ "onClass2ThreadRunning",
        /* strAddInfo   */ strMthInArgs );

    m_pWaitClass2ThreadRunning->notify_all();
}
