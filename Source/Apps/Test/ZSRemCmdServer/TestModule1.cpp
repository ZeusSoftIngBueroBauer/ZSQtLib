/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include "TestModule1.h"
#include "TestModule2.h"
#include "App.h"

#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::RemCmdServer;


/*******************************************************************************
class CTestModule1 : public QObject
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/* enum ERequest
==============================================================================*/

static const SEnumEntry s_arEnumEntryRequests[CTestModule1::ERequestCount] = {
    /* 0: */ SEnumEntry( CApplication::ERequestNone,     "None"     ),
    /* 1: */ SEnumEntry( CApplication::ERequestStartup,  "Startup"  ),
    /* 2: */ SEnumEntry( CApplication::ERequestShutdown, "Shutdown" )
};

//------------------------------------------------------------------------------
QString CTestModule1::request2Str( int i_request )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumEntryRequests, _ZSArrLen(s_arEnumEntryRequests), i_request );
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestModule1::CTestModule1(
    const QString& i_strObjName,
    const QString& i_strTestModule2ObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strTestModule2ObjName(i_strTestModule2ObjName),
    m_pTestModule2Thread(nullptr),
    m_pTestModule2(nullptr),
    m_pReqInProgress(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::RemCmdServer", "CTestModule1", i_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "ctor",
        /* szAddInfo    */ "" );

} // ctor

//------------------------------------------------------------------------------
CTestModule1::~CTestModule1()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "dtor",
        /* szAddInfo    */ "" );

    if( m_pTestModule2Thread != nullptr && m_pTestModule2Thread->isRunning() )
    {
        m_pTestModule2Thread->quit();
        m_pTestModule2Thread->wait();
    }

    try
    {
        delete m_pReqInProgress;
    }
    catch(...)
    {
    }
    m_pReqInProgress = nullptr;

    delete m_pTestModule2Thread;
    m_pTestModule2Thread = nullptr;
    m_pTestModule2 = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CTestModule1::startup( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqParent: " + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel);

        if( m_pReqInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqInProgress: " + m_pReqInProgress->getRequestName(true);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "startup",
        /* szAddInfo    */ strAddTrcInfo );

    if( m_pReqInProgress == nullptr )
    {
        m_pReqInProgress = new CRequest(
            /* strNameSpace */ "ZS::Apps::Test::RemCmdServer",
            /* strClassName */ "CTestModule2",
            /* pObjCreator  */ this,
            /* iRequest     */ ERequestStartup,
            /* strRequest   */ "Startup",
            /* iReqIdParent */ i_iReqIdParent );

        m_pTestModule2Thread = new CTestModule2Thread(
            /* pTestModule1          */ this,
            /* strObjNameTestModule2 */ m_strTestModule2ObjName );

        if( !QObject::connect(
            /* pObjSender   */ m_pTestModule2Thread,
            /* szSignal     */ SIGNAL(createdTestModule2(QObject*,QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTestModule2Created(QObject*,QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        if( !QObject::connect(
            /* pObjSender   */ m_pTestModule2Thread,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTestModule2ThreadDestroyed(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        m_pTestModule2Thread->start();

    } // if( m_pReqInProgress == nullptr )

   return m_pReqInProgress;

} // startup

//------------------------------------------------------------------------------
CRequest* CTestModule1::shutdown( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "ReqParent: " + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel);

        if( m_pReqInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqInProgress: " + m_pReqInProgress->getRequestName(true);
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "shutdown",
        /* szAddInfo    */ strAddTrcInfo );

    if( m_pReqInProgress == nullptr )
    {
        m_pReqInProgress = new CRequest(
            /* strNameSpace */ "ZS::Apps::Test::Trace",
            /* strClassName */ "CTestModule2",
            /* pObjCreator  */ this,
            /* iRequest     */ ERequestShutdown,
            /* strRequest   */ "Shutdown",
            /* iReqIdParent */ i_iReqIdParent );

        if( m_pTestModule2Thread != nullptr && m_pTestModule2Thread->isRunning() )
        {
            m_pTestModule2Thread->quit();
            m_pTestModule2Thread->wait();
        }

        delete m_pTestModule2Thread;
        m_pTestModule2Thread = nullptr;
        m_pTestModule2 = nullptr;

        m_pReqInProgress->setErrResult(ErrResultSuccess);
        m_pReqInProgress->setProgressInPerCent(100);
        m_pReqInProgress->update();

        deleteRequest(m_pReqInProgress);
        m_pReqInProgress = nullptr;

    } // if( m_pReqInProgress == nullptr )

    return m_pReqInProgress;

} // shutdown

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTestModule1::onTestModule2Created( QObject* i_pThread, QObject* i_pTestModule )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pThread == nullptr )
        {
            strAddTrcInfo = "Thread: nullptr";
        }
        else
        {
            strAddTrcInfo = "Thread: " + i_pThread->objectName();
        }

        if( i_pTestModule == nullptr )
        {
            strAddTrcInfo = "TestModule: nullptr";
        }
        else
        {
            strAddTrcInfo = "TestModule: " + i_pTestModule->objectName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onTestModule2Created",
        /* szAddInfo    */ strAddTrcInfo );

    CTestModule2Thread* pTestModule2Thread = dynamic_cast<CTestModule2Thread*>(i_pThread);

    if( m_pTestModule2Thread != pTestModule2Thread )
    {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange );
    }

    m_pTestModule2 = dynamic_cast<CTestModule2*>(i_pTestModule);

    // Use "direct connection" to avoid that the "onDestroyed" slot is called after
    // the object has already been completely destroyed.
    if( !QObject::connect(
        /* pObjSender     */ m_pTestModule2,
        /* szSignal       */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onTestModule2Destroyed(QObject*)),
        /* connectionType */ Qt::DirectConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( m_pReqInProgress != nullptr )
    {
        if( m_pReqInProgress->getRequest() != ERequestStartup )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, "Startup request was not in progress" );
        }

        m_pReqInProgress->setErrResult(ErrResultSuccess);
        m_pReqInProgress->setProgressInPerCent(100);
        m_pReqInProgress->update();

        deleteRequest(m_pReqInProgress);
        m_pReqInProgress = nullptr;

    } // if( m_pReqInProgress != nullptr )

} // onTestModule2Created

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTestModule1::onTestModule2ThreadDestroyed( QObject* i_pThread )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pThread == nullptr )
        {
            strAddTrcInfo = "Thread: nullptr";
        }
        else
        {
            strAddTrcInfo = "Thread: " + i_pThread->objectName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTestModule2ThreadDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pThread == m_pTestModule2Thread )
    {
        m_pTestModule2Thread = nullptr;
    }

} // onTestModule2ThreadDestroyed

//------------------------------------------------------------------------------
void CTestModule1::onTestModule2Destroyed( QObject* i_pTestModule )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pTestModule == nullptr )
        {
            strAddTrcInfo = "TestModule: nullptr";
        }
        else
        {
            // Use "direct connection" to avoid that the "onDestroyed" slot is called after
            // the object has already been completely destroyed and we could not access
            // the object calling "objectName" anymore.
            strAddTrcInfo = "TestModule: " + i_pTestModule->objectName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onTestModule2Destroyed",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pTestModule == m_pTestModule2 )
    {
        m_pTestModule2 = nullptr;
    }

} // onTestModule2Destroyed
