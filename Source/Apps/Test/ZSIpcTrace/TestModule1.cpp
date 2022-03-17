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

#include "TestModule1.h"
#include "TestModule2.h"
#include "App.h"

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
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
    CMyClass1::NameSpace(), CMyClass1::ClassName(), "ZSTrcServer");

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass1::setTraceServerName( const QString& i_strServerName )
//------------------------------------------------------------------------------
{
    s_trcAdminObjRefAnchor.setServerName(i_strServerName);
}

//------------------------------------------------------------------------------
QString CMyClass1::getTraceServerName()
//------------------------------------------------------------------------------
{
    return s_trcAdminObjRefAnchor.getServerName();
}

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
CMyClass1::CMyClass1( const QString& i_strObjName, const QString& i_strMyClass2ObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strMyClass2ObjName(i_strMyClass2ObjName),
    m_pMyClass2Thread(nullptr),
    m_pMyClass2(nullptr)
{
    setObjectName(i_strObjName);

    s_trcAdminObjRefAnchor.allocTrcAdminObj();

    s_trcAdminObjRefAnchor.setTraceDetailLevel(ETraceDetailLevelMethodArgs);

    QString strMthInArgs;

    if( s_trcAdminObjRefAnchor.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs  = i_strObjName;
        strMthInArgs += ", " + i_strMyClass2ObjName;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ objectName(),
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

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

    if( m_pMyClass2Thread != nullptr && m_pMyClass2Thread->isRunning() )
    {
        m_pMyClass2Thread->quit();
        m_pMyClass2Thread->wait();
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

    mthTracer.onAdminObjAboutToBeReleased();

    s_trcAdminObjRefAnchor.releaseTrcAdminObj();

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CMyClass2* CMyClass1::createModule2()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ objectName(),
        /* strMethod    */ "createModule2",
        /* strAddInfo   */ strMthInArgs );

    if( m_pMyClass2Thread == nullptr )
    {
        m_pMyClass2Thread = new CMyClass2Thread(m_strMyClass2ObjName, this);
    }

    if( !m_pMyClass2Thread->isRunning() )
    {
        m_pMyClass2Thread->start();
    }

    const int c_iMaxWaitCount = 25;
    int iWaitCount = 0;

    if( m_pMyClass2Thread != nullptr )
    {
        m_pMyClass2 = m_pMyClass2Thread->getMyClass2();

        while( m_pMyClass2 == nullptr )
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
            m_pMyClass2 = m_pMyClass2Thread->getMyClass2();
        }
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthRet = QString(m_pMyClass2 == nullptr ? "null" : m_pMyClass2->objectName());
        mthTracer.trace(strMthRet);
    }

    return m_pMyClass2;

} // createModule2

//------------------------------------------------------------------------------
void CMyClass1::deleteModule2()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ s_trcAdminObjRefAnchor.trcAdminObj(),
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strObjName   */ objectName(),
        /* strMethod    */ "deleteModule2",
        /* strAddInfo   */ strMthInArgs );

    if( m_pMyClass2Thread != nullptr && m_pMyClass2Thread->isRunning() )
    {
        m_pMyClass2Thread->quit();
        m_pMyClass2Thread->wait();
    }

    delete m_pMyClass2Thread;
    m_pMyClass2Thread = nullptr;
    m_pMyClass2 = nullptr;

} // deleteModule2
