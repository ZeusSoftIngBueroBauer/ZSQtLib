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

#include "TestModule1.h"
#include "TestModule2.h"

#include "ZSIpcTrace/ZSIpcTrcDllIf.h"

using namespace ZS::Trace::DllIf;
using namespace ZS::Apps::Test::IpcTraceDllIfWinApp;


/*******************************************************************************
class CTestModule1
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestModule1::CTestModule1( const std::string& i_strObjName, const std::string& i_strTestModule2ObjName ) :
//------------------------------------------------------------------------------
    m_strObjName(i_strObjName),
    m_strTestModule2ObjName(i_strTestModule2ObjName),
    m_pTestModule2Thread(nullptr),
    m_pTestModule2(nullptr),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace().c_str(), ClassName().c_str(), objectName().c_str());

    m_pTrcAdminObj->setMethodCallsTraceDetailLevel(EMethodTraceDetailLevelArgsNormal);

    std::string stdstrMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevelArgsNormal )
    {
        stdstrMthInArgs = std::string("ObjName: ") + objectName();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ stdstrMthInArgs.c_str() );

} // ctor

//------------------------------------------------------------------------------
CTestModule1::~CTestModule1()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    if( m_pTestModule2Thread != nullptr && m_pTestModule2Thread->isRunning() )
    {
        m_pTestModule2Thread->quit();
        m_pTestModule2Thread->wait();
    }

    try
    {
        delete m_pTestModule2Thread;
    }
    catch(...)
    {
    }

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    //m_strObjName;
    //m_strTestModule2ObjName;
    m_pTestModule2Thread = nullptr;
    m_pTestModule2 = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CTestModule2* CTestModule1::createModule2()
//------------------------------------------------------------------------------
{
    std::string strMthInArgs;
    std::string strMthAddInfo;
    std::string strMthRet;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* strMethod    */ "createModule2",
        /* strAddInfo   */ strMthInArgs.c_str() );

    if( m_pTestModule2Thread == nullptr )
    {
        m_pTestModule2Thread = new CTestModule2Thread(this, m_strTestModule2ObjName);
    }

    if( !m_pTestModule2Thread->isRunning() )
    {
        m_pTestModule2Thread->start();
    }

    const int c_iMaxWaitCount = 1000000;
    int iWaitCount = 0;

    if( m_pTestModule2Thread != nullptr )
    {
        bool bThreadRunning = m_pTestModule2Thread->isRunning();

        m_pTestModule2 = m_pTestModule2Thread->getTestModule2();

        while( !bThreadRunning && m_pTestModule2 == nullptr )
        {
            #ifdef _WINDOWS
            Sleep(100);
            #endif
            #ifdef __linux__
            usleep(100000);
            #endif

            iWaitCount++;
            if( iWaitCount > c_iMaxWaitCount )
            {
                break;
            }
            bThreadRunning = m_pTestModule2Thread->isRunning();
            m_pTestModule2 = m_pTestModule2Thread->getTestModule2();
        }
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
        strMthRet = std::string(m_pTestModule2 == nullptr ? "null" : m_pTestModule2->objectName());
        mthTracer.trace(strMthRet.c_str());
    }

    return m_pTestModule2;

} // createModule2

//------------------------------------------------------------------------------
void CTestModule1::deleteModule2()
//------------------------------------------------------------------------------
{
    std::string strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevelArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevelEnterLeave,
        /* strMethod    */ "deleteModule2",
        /* strAddInfo   */ strMthInArgs.c_str() );

    if( m_pTestModule2Thread != nullptr && m_pTestModule2Thread->isRunning() )
    {
        m_pTestModule2Thread->quit();
        m_pTestModule2Thread->wait();
    }

    delete m_pTestModule2Thread;
    m_pTestModule2Thread = nullptr;
    m_pTestModule2 = nullptr;

} // deleteModule2
