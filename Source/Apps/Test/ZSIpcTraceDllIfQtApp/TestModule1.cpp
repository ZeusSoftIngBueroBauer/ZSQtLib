/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include "ZSIpcTrace/ZSIpcTrcDllIf.h"

#include "ZSSys/ZSSysMemLeakDump.h"

#ifdef __linux__
#include <unistd.h>
#endif

using namespace ZS::System;
using namespace ZS::Trace::DllIf;
using namespace ZS::Apps::Test::IpcTraceDllIfQtApp;


/*******************************************************************************
class CTestModule1 : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTestModule1::CTestModule1( const QString& i_strObjName, const QString& i_strTestModule2ObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strTestModule2ObjName(i_strTestModule2ObjName),
    m_pTestModule2Thread(nullptr),
    m_pTestModule2(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace().toLatin1(), ClassName().toLatin1(), objectName().toLatin1());

    m_pTrcAdminObj->setTraceDetailLevel(ETraceDetailLevelMethodArgs);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "ObjName: " + i_strObjName;
        strMthInArgs += ", TestModule2ObjName: " + i_strTestModule2ObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs.toLatin1() );

} // ctor

//------------------------------------------------------------------------------
CTestModule1::~CTestModule1()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
    m_pTestModule2Thread = nullptr;
    m_pTestModule2 = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CTestModule2* CTestModule1::createModule2()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createModule2",
        /* strAddInfo   */ strMthInArgs.toLatin1() );

    if( m_pTestModule2Thread == nullptr )
    {
        m_pTestModule2Thread = new CTestModule2Thread(this, m_strTestModule2ObjName);
    }

    if( !m_pTestModule2Thread->isRunning() )
    {
        m_pTestModule2Thread->start();
    }

    const int c_iMaxWaitCount = 25;
    int iWaitCount = 0;

    if( m_pTestModule2Thread != NULL )
    {
        m_pTestModule2 = m_pTestModule2Thread->getTestModule2();

        while( m_pTestModule2 == nullptr )
        {
            #ifdef _WINDOWS
            Sleep(200);
            #endif
            #ifdef __linux__
            usleep(200000);
            #endif

            iWaitCount++;
            if( iWaitCount > c_iMaxWaitCount )
            {
                break;
            }
            m_pTestModule2 = m_pTestModule2Thread->getTestModule2();
        }
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthRet = QString(m_pTestModule2 == nullptr ? "null" : m_pTestModule2->objectName());
        mthTracer.trace(strMthRet.toLatin1());
    }

    return m_pTestModule2;

} // createModule2

//------------------------------------------------------------------------------
void CTestModule1::deleteModule2()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "deleteModule2",
        /* strAddInfo   */ strMthInArgs.toLatin1() );

    if( m_pTestModule2Thread != nullptr && m_pTestModule2Thread->isRunning() )
    {
        m_pTestModule2Thread->quit();
        m_pTestModule2Thread->wait();
    }

    delete m_pTestModule2Thread;
    m_pTestModule2Thread = nullptr;
    m_pTestModule2 = nullptr;

} // deleteModule2
