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
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysTrcMethod.h"

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

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
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
    m_bCtorReady(false)
{
    setObjectName(i_strObjName);

    s_trcAdminObjRefAnchor.allocTrcAdminObj();

    s_trcAdminObjRefAnchor.setMethodCallsTraceDetailLevel(EMethodTraceDetailLevel::ArgsNormal);

    QString strMthInArgs;

    if( s_trcAdminObjRefAnchor.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ i_strObjName,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

    m_bCtorReady = true;

} // ctor

//------------------------------------------------------------------------------
CMyClass1::~CMyClass1()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "dtor",
        /* strMthInArgs */ "" );

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

    m_bCtorReady = false;

    mthTracer.onAdminObjAboutToBeReleased();

    s_trcAdminObjRefAnchor.releaseTrcAdminObj();

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
    // member is used which will be set after calling "setObjectName".
    if( !m_bCtorReady )
    {
        QObject::setObjectName(i_strObjName);
    }
    // If an existing object has to be renamed the ctor ready flag is set.
    else
    {
        QString strMthInArgs;

        if( s_trcAdminObjRefAnchor.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
        {
            strMthInArgs = i_strObjName;
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
            /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strObjName   */ objectName(),
            /* strMethod    */ "setObjectName",
            /* strMthInArgs */ strMthInArgs );

        QObject::setObjectName(i_strObjName);
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

    if( s_trcAdminObjRefAnchor.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = i_strMyClass2ObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "startClass2Thread",
        /* strMthInArgs */ strMthInArgs );

    m_strMyClass2ObjName = i_strMyClass2ObjName;

    if( m_pMyClass2Thread == nullptr )
    {
        m_pMyClass2Thread = new CMyClass2Thread(i_strMyClass2ObjName, this);
    }

    if( !m_pMyClass2Thread->isRunning() )
    {
        m_pMyClass2Thread->start();
        m_pMyClass2 = m_pMyClass2Thread->waitForMyClass2Created();
    }

    if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
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
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strObjName   */ objectName(),
        /* strMethod    */ "stopClass2Thread",
        /* strMthInArgs */ strMthInArgs );

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
