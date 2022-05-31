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

#include "stdafx.h"

#include <mutex>
#include <thread>

#include "TestModule2.h"
#include "TestModule1.h"

#include "ZSIpcTrace/ZSIpcTrcDllIf.h"

using namespace ZS::Trace::DllIf;
using namespace ZS::Apps::Test::IpcTraceDllIfWinApp;


/*******************************************************************************
class CTestModule2Thread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestModule2Thread::CTestModule2Thread(
    CTestModule1*      i_pTestModule1,
    const std::string& i_strTestModule2ObjName ) :
//------------------------------------------------------------------------------
    m_strObjName(i_strTestModule2ObjName),
    m_pTestModule1(i_pTestModule1),
    m_pThread(nullptr),
    m_bRunning(false),
    m_bQuit(false),
    m_pMtx(nullptr),
    m_pTestModule2(nullptr),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace().c_str(), ClassName().c_str(), objectName().c_str());

    m_pTrcAdminObj->setTraceDetailLevel(ETraceDetailLevelRuntimeInfo);

    std::string strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TestModule1: " + std::string(i_pTestModule1 == nullptr ? "null" : i_pTestModule1->objectName());
        strMthInArgs += ", TestModule2ObjName: " + i_strTestModule2ObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs.c_str() );

    m_pMtx = new std::recursive_mutex();

} // ctor

//------------------------------------------------------------------------------
CTestModule2Thread::~CTestModule2Thread()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "dtor",
        /* szAddInfo    */ "" );

    quit();

    if( !wait(30000) )
    {
    }

    try
    {
        delete m_pTestModule2;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pMtx;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pThread;
    }
    catch(...)
    {
    }

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    //m_strObjName;
    m_pTestModule1 = nullptr;
    m_pThread = nullptr;
    m_bRunning = false;
    m_bQuit = false;
    m_pMtx = nullptr;
    m_pTestModule2 = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CTestModule2* CTestModule2Thread::getTestModule2()
//------------------------------------------------------------------------------
{
    std::string strMthRet;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "getTestModule2",
        /* strAddInfo   */ "" );

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
        strMthRet = m_pTestModule2 == nullptr ? "null" : m_pTestModule2->objectName();
        mthTracer.setMethodReturn(strMthRet.c_str());
    }

    return m_pTestModule2;

} // getTestModule2

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CTestModule2Thread::isRunning() const
//------------------------------------------------------------------------------
{
    std::string strMthRet;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "isRunning",
        /* strAddInfo   */ "" );

    std::lock_guard<std::recursive_mutex> mtxLocker(*m_pMtx);

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(m_bRunning);
    }

    return m_bRunning;

} // isRunning

//------------------------------------------------------------------------------
void CTestModule2Thread::start()
//------------------------------------------------------------------------------
{
    std::string strMthRet;
    std::string strMthAddInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "start",
        /* strAddInfo   */ "" );

    if( m_pThread == nullptr )
    {
        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo )
        {
            strMthAddInfo = " -+ new std::thread()";
            mthTracer.trace(strMthAddInfo.c_str(), ETraceDetailLevelRuntimeInfo);
        }

        m_pThread = new std::thread(&CTestModule2Thread::run, this);

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo )
        {
            strMthAddInfo = " +- new std::thread()";
            mthTracer.trace(strMthAddInfo.c_str(), ETraceDetailLevelRuntimeInfo);
        }

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo )
        {
            strMthAddInfo = " -+ std::thread->detach()";
            mthTracer.trace(strMthAddInfo.c_str(), ETraceDetailLevelRuntimeInfo);
        }

        m_pThread->detach();

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo )
        {
            strMthAddInfo = " +- std::thread->detach()";
            mthTracer.trace(strMthAddInfo.c_str(), ETraceDetailLevelRuntimeInfo);
        }
    }

} // start

//------------------------------------------------------------------------------
void CTestModule2Thread::quit()
//------------------------------------------------------------------------------
{
    std::string strMthRet;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "quit",
        /* strAddInfo   */ "" );

    std::lock_guard<std::recursive_mutex> mtxLocker(*m_pMtx);

    m_bQuit = true;

} // quit

//------------------------------------------------------------------------------
bool CTestModule2Thread::wait( unsigned long i_uTime_ms )
//------------------------------------------------------------------------------
{
    std::string strMthInArgs;
    std::string strMthRet;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Time: " + std::to_string(i_uTime_ms) + " ms";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "wait",
        /* strAddInfo   */ strMthInArgs.c_str() );

    std::chrono::time_point<std::chrono::steady_clock> timeStart = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> timeCurr;

    while( m_bRunning )
    {
        Sleep(200);

        timeCurr = std::chrono::steady_clock::now();

        auto timeDuration = timeCurr - timeStart;

        std::chrono::milliseconds timeDuration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timeDuration);

        if( timeDuration_ms > std::chrono::milliseconds(i_uTime_ms) )
        {
            break;
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(!m_bRunning);
    }

    return !m_bRunning;

} // wait

//------------------------------------------------------------------------------
int CTestModule2Thread::exec()
//------------------------------------------------------------------------------
{
    int iResult = 0;

    std::string strMthRet;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "exec",
        /* strAddInfo   */ "" );

    std::chrono::time_point<std::chrono::steady_clock> timeStart = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> timeCurr;

    while( !m_bQuit )
    {
        Sleep(1000);

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo )
        {
            timeCurr = std::chrono::steady_clock::now();
            auto timeDuration = timeCurr - timeStart;
            std::chrono::milliseconds timeDuration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timeDuration);
            std::string strMthAddInfo = "Thread running for " + std::to_string(timeDuration_ms.count()) + " ms";
            mthTracer.trace(strMthAddInfo.c_str(), ETraceDetailLevelRuntimeInfo);
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(iResult);
    }

    return iResult;

} // exec

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTestModule2Thread::run()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "run",
        /* szAddInfo    */ "" );

    {   std::lock_guard<std::recursive_mutex> mtxLocker(*m_pMtx);
        m_bRunning = true;
    }

    m_pTestModule2 = new CTestModule2(this, objectName());

    m_pTestModule2->recursiveTraceMethod();

    exec();

    {   std::lock_guard<std::recursive_mutex> mtxLocker(*m_pMtx);
        m_bRunning = false;
    }

    try
    {
        delete m_pTestModule2;
    }
    catch(...)
    {
    }
    m_pTestModule2 = nullptr;

} // run


/*******************************************************************************
class CTestModule2 : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestModule2::CTestModule2(
    CTestModule2Thread* i_pTestModule2Thread,
    const std::string&  i_strObjName ) :
//------------------------------------------------------------------------------
    m_strObjName(i_strObjName),
    m_pTestModule2Thread(i_pTestModule2Thread),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace().c_str(), ClassName().c_str(), objectName().c_str());

    m_pTrcAdminObj->setTraceDetailLevel(ETraceDetailLevelRuntimeInfo);

    std::string strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TestModule2Thread: " + std::string(i_pTestModule2Thread == nullptr ? "null" : i_pTestModule2Thread->objectName());
        strMthInArgs += ", ObjName: " + i_strObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs.c_str() );

} // ctor

//------------------------------------------------------------------------------
CTestModule2::~CTestModule2()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "dtor",
        /* szAddInfo    */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pTestModule2Thread = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CTestModule2::recursiveTraceMethod()
//------------------------------------------------------------------------------
{
    static int s_iCount = 0;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "recursiveTraceMethod",
        /* szAddInfo    */ "" );

    ++s_iCount;

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        std::string strTrcMsg = "Count=" + std::to_string(s_iCount);
        mthTracer.trace(strTrcMsg.c_str(), ETraceDetailLevelRuntimeInfo);
    }

    if( s_iCount <= 10 )
    {
        Sleep(100);

        recursiveTraceMethod();
    }

    --s_iCount;

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(s_iCount);
    }

    return s_iCount;

} // recursiveTraceMethod
