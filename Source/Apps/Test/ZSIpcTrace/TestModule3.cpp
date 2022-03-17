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

#include "TestModule3.h"
#include "TestModule2.h"
#include "TestModule1.h"
#include "App.h"
#include "MsgTest.h"

#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::IpcTrace;


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
    setObjectName(m_strMyClass3ObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    m_pTrcAdminObj->setTraceDetailLevel(ETraceDetailLevelMethodArgs);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_strMyClass3ObjName;
        strMthInArgs += ", " + QString(i_pMyClass2 == nullptr ? "nullptr" : i_pMyClass2->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CMyClass3Thread::~CMyClass3Thread()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

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

    if( m_pMyClass2 != nullptr )
    {
        try
        {
            delete m_pMyClass2;
        }
        catch(...)
        {
        }
        m_pMyClass2 = nullptr;
    }

    m_pMyClass2 = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

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
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "run",
        /* strAddInfo   */ "" );

    m_pMyClass3 = new CMyClass3(m_strMyClass3ObjName, this);

    m_pMyClass3->recursiveTraceMethod();

    m_pMyClass3->startMessageTimer();

    exec();

    try
    {
        delete m_pMyClass3;
    }
    catch(...)
    {
    }
    m_pMyClass3 = nullptr;

} // run


/*******************************************************************************
class CMyClass3 : public QObject
*******************************************************************************/

CTrcAdminObjRefAnchor CMyClass3::s_trcAdminObjRefAnchor(
    CMyClass3::NameSpace(), CMyClass3::ClassName(), "ZSTrcServer");

CTrcAdminObjRefAnchor CMyClass3::s_trcAdminObjRefAnchorNoisyMethods(
    CMyClass3::NameSpace(), CMyClass3::ClassName() + "::NoisyMethods", "ZSTrcServer") ;

CTrcAdminObjRefAnchor CMyClass3::s_trcAdminObjRefAnchorVeryNoisyMethods(
    CMyClass3::NameSpace(), CMyClass3::ClassName() + "::VeryNoisyMethods", "ZSTrcServer") ;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass3::setTraceServerName( const QString& i_strServerName )
//------------------------------------------------------------------------------
{
    s_trcAdminObjRefAnchor.setServerName(i_strServerName);
    s_trcAdminObjRefAnchorNoisyMethods.setServerName(i_strServerName);
    s_trcAdminObjRefAnchorVeryNoisyMethods.setServerName(i_strServerName);
}

//------------------------------------------------------------------------------
QString CMyClass3::getTraceServerName()
//------------------------------------------------------------------------------
{
    return s_trcAdminObjRefAnchor.getServerName();
}

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

//------------------------------------------------------------------------------
QString CMyClass3::noisyClassMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    CTrcAdminObjRefGuard trcAdminObjGuard(&s_trcAdminObjRefAnchorNoisyMethods);

    if( trcAdminObjGuard.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ trcAdminObjGuard.trcAdminObj(),
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "noisyClassMethod",
        /* strAddInfo   */ strMthInArgs );

    strResult = "Hello World";

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet);
    }

    return strResult;

} // noisyClassMethod

//------------------------------------------------------------------------------
QString CMyClass3::veryNoisyClassMethod(const QString& i_strMthInArgs)
//------------------------------------------------------------------------------
{
    QString strResult;
    QString strMthInArgs;
    QString strMthRet;

    CTrcAdminObjRefGuard trcAdminObjGuard(&s_trcAdminObjRefAnchorVeryNoisyMethods);

    if( trcAdminObjGuard.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ trcAdminObjGuard.trcAdminObj(),
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "veryNoisyClassMethod",
        /* strAddInfo   */ strMthInArgs );

    strResult = "Hello World";

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet);
    }

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
    m_pTmrMessages(nullptr),
    m_iMsgCount(0),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjNoisyMethods(nullptr),
    m_pTrcAdminObjVeryNoisyMethods(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName(), getTraceServerName());
    m_pTrcAdminObjNoisyMethods =
        CTrcServer::GetTraceAdminObj(NameSpace(), ClassName() + "::NoisyMethods", objectName(), getTraceServerName());
    m_pTrcAdminObjVeryNoisyMethods =
        CTrcServer::GetTraceAdminObj(NameSpace(), ClassName() + "::VeryNoisyMethods", objectName(), getTraceServerName());

    m_pTrcAdminObj->setTraceDetailLevel(ETraceDetailLevelRuntimeInfo);
    m_pTrcAdminObjNoisyMethods->setTraceDetailLevel(ETraceDetailLevelMethodArgs);
    m_pTrcAdminObjVeryNoisyMethods->setTraceDetailLevel(ETraceDetailLevelMethodArgs);

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", " + QString(i_pMyClass3Thread == nullptr ? "nullptr" : i_pMyClass3Thread->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_pTmrMessages = new QTimer(this);

    if( !QObject::connect(
        /* szSender   */ m_pTmrMessages,
        /* szSignal   */ SIGNAL(timeout()),
        /* szReceiver */ this,
        /* szSlot     */ SLOT(onTmrMessagesTimeout())) )
    {
        throw ZS::System::CException(__FILE__, __LINE__, EResultSignalSlotConnectionFailed);
    }

} // ctor

//------------------------------------------------------------------------------
CMyClass3::~CMyClass3()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjNoisyMethods);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjVeryNoisyMethods);

    m_pMyClass3Thread = nullptr;
    m_pTmrMessages = nullptr;
    m_iMsgCount = 0;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjNoisyMethods = nullptr;
    m_pTrcAdminObjVeryNoisyMethods = nullptr;

} // dtor

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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "instMethod",
        /* strAddInfo   */ strMthInArgs );

    strResult = "Hello World";

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet);
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

    if( m_pTrcAdminObjNoisyMethods != nullptr && m_pTrcAdminObjNoisyMethods->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjNoisyMethods,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "noisyInstMethod",
        /* strAddInfo   */ strMthInArgs );

    strResult = "Hello World";

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet);
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

    if( m_pTrcAdminObjVeryNoisyMethods != nullptr && m_pTrcAdminObjVeryNoisyMethods->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_strMthInArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjVeryNoisyMethods,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "veryNoisyInstMethod",
        /* strAddInfo   */ strMthInArgs );

    strResult = "Hello World";

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthRet = strResult;
        mthTracer.setMethodReturn(strMthRet);
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
    static int s_iCount = 0;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "recursiveTraceMethod",
        /* strAddInfo   */ "" );

    ++s_iCount;

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        QString strTrcMsg = "Count=" + QString::number(s_iCount);
        mthTracer.trace(strTrcMsg, ETraceDetailLevelRuntimeInfo);
    }

    if( s_iCount <= 10 )
    {
        recursiveTraceMethod();
    }

    --s_iCount;

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        mthTracer.setMethodReturn(s_iCount);
    }

    return s_iCount;

} // recursiveTraceMethod

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass3::startMessageTimer()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startMessageTimer",
        /* strAddInfo   */ "" );

    if( !m_pTmrMessages->isActive() )
    {
        m_iMsgCount = 0;
        m_pTmrMessages->start(100);
    }
} // startMessageTimer

/*==============================================================================
protected: // slots
==============================================================================*/

//------------------------------------------------------------------------------
void CMyClass3::onTmrMessagesTimeout()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelRuntimeInfo,
        /* strMethod    */ "onTmrMessagesTimeout",
        /* strAddInfo   */ "" );

    CMsgReqTest* pMsgReq = new CMsgReqTest(this, this);
    POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);
    pMsgReq = nullptr;

} // onTmrMessagesTimeout

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
        ++m_iMsgCount;

        if( m_iMsgCount >= 100 && m_pTmrMessages->isActive() )
        {
            m_pTmrMessages->stop();
        }

        QString strMthInArgs;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = "Msg: " + QString(pMsg == nullptr ? "null" : pMsg->msgTypeToStr());
        }

        CMethodTracer mthTracer(
            /* pTrcServer         */ m_pTrcAdminObj,
            /* iFilterDetailLevel */ ETraceDetailLevelRuntimeInfo,
            /* strMethod          */ "event",
            /* strMethodInArgs    */ strMthInArgs );

        bHandled = true;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelMethodArgs )
        {
            mthTracer.setMethodReturn(m_iMsgCount);
        }
    }

    if( !bHandled )
    {
        bHandled = QObject::event(i_pEv);
    }

    return bHandled;

} // event
