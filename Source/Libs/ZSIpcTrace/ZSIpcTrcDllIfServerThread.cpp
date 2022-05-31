/*******************************************************************************

Copyright 2009 by Rohde & Schwarz, Munich

--------------------------------------------------------------------------------

Responsible: Christian Bauer, 8SPS

Content:

--------------------------------------------------------------------------------

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#include "ZSIpcTrace/ZSIpcTrcDllIfServerThread.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysTrcMthFile.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;


/*******************************************************************************
class CIpcTrcServerThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
DllIf::CIpcTrcServerThread::CIpcTrcServerThread(
    int i_iTrcDetailLevelDllIf,
    int i_iTrcDetailLevelTrcServer,
    int i_iTrcDetailLevelTrcServerMutex,
    int i_iTrcDetailLevelTrcServerIpcServer,
    int i_iTrcDetailLevelTrcServerIpcServerMutex,
    int i_iTrcDetailLevelTrcServerIpcServerGateway ) :
//------------------------------------------------------------------------------
    QThread(),
    m_iTrcDetailLevelDllIf(i_iTrcDetailLevelDllIf),
    m_iTrcDetailLevelTrcServer(i_iTrcDetailLevelTrcServer),
    m_iTrcDetailLevelTrcServerMutex(i_iTrcDetailLevelTrcServerMutex),
    m_iTrcDetailLevelTrcServerIpcServer(i_iTrcDetailLevelTrcServerIpcServer),
    m_iTrcDetailLevelTrcServerIpcServerMutex(i_iTrcDetailLevelTrcServerIpcServerMutex),
    m_iTrcDetailLevelTrcServerIpcServerGateway(i_iTrcDetailLevelTrcServerIpcServerGateway),
    m_pTrcMthFile(nullptr)
{
    setObjectName("ZSTrcServerDllIf");

    QString strLocalTrcFileAbsFilePath = CTrcServer::GetDefaultLocalTrcFileAbsoluteFilePath("System");

    m_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevelDllIf,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );

} // ctor

//------------------------------------------------------------------------------
DllIf::CIpcTrcServerThread::~CIpcTrcServerThread()
//------------------------------------------------------------------------------
{
    // The method tracer to trace method enter and method leave cannot be used here.
    // The trace method file will be destroyed before method leave is traced.
    // As a workaround the method tracers scope is left before the trace method
    // file is closed and freed.

    {   CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevelDllIf,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMthInArgs       */ "" );

    } // CMethodTracer mthTracer(

    // Delete the trace method file after the index tree as the
    // index tree may use the trace method file for method tracing.
    if( m_pTrcMthFile != nullptr )
    {
        m_pTrcMthFile->close();
        CTrcMthFile::Free(m_pTrcMthFile);
    }

    // The trace server should have been released at the end of the run method.
    // If not (for whatever unexpected reason) the server will be released here.
    if( CIpcTrcServer::GetInstance() != nullptr )
    {
        try
        {
            CIpcTrcServer::ReleaseInstance();
        }
        catch(...)
        {
        }
    }

    m_iTrcDetailLevelDllIf = 0;
    m_iTrcDetailLevelTrcServer = 0;
    m_iTrcDetailLevelTrcServerMutex = 0;
    m_iTrcDetailLevelTrcServerIpcServer = 0;
    m_iTrcDetailLevelTrcServerIpcServerMutex = 0;
    m_iTrcDetailLevelTrcServerIpcServerGateway = 0;
    m_pTrcMthFile = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CIpcTrcServerThread::isServerCreated()
//------------------------------------------------------------------------------
{
    return (CIpcTrcServer::GetInstance() != nullptr);
}

//------------------------------------------------------------------------------
bool DllIf::CIpcTrcServerThread::isServerStarted()
//------------------------------------------------------------------------------
{
    bool bServerStarted = false;

    CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance();

    if( pTrcServer != NULL )
    {
        bServerStarted = (pTrcServer->getState() == ZS::Ipc::CServer::EStateListening);
    }
    return bServerStarted;

} // isServerStarted

//------------------------------------------------------------------------------
bool DllIf::CIpcTrcServerThread::isServerShutdown()
//------------------------------------------------------------------------------
{
    bool bServerShutdown = true;

    CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance();

    if( pTrcServer != NULL )
    {
        bServerShutdown = (pTrcServer->getState() == ZS::Ipc::CServer::EStateIdle);
    }
    return bServerShutdown;

} // isServerShutdown

/*==============================================================================
public: // overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CIpcTrcServerThread::run()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevelDllIf,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "run",
        /* strMthInArgs       */ "" );

    // Create trace server
    //--------------------

    CIpcTrcServer::CreateInstance(
        m_iTrcDetailLevelTrcServer,
        m_iTrcDetailLevelTrcServerMutex,
        m_iTrcDetailLevelTrcServerIpcServer,
        m_iTrcDetailLevelTrcServerIpcServerMutex,
        m_iTrcDetailLevelTrcServerIpcServerGateway);

    try
    {
        exec();
    }

    catch( ZS::System::CException& exc )
    {
        if( QThread::currentThread() != thread() )
        {
            // This type of exception has already been logged on throwing (creating) the exception.
            CMsgErr* pMsgExc = new CMsgErr(
                /* pObjSender   */ this,
                /* pObjReceiver */ QCoreApplication::instance(),
                /* exc          */ exc,
                /* strExcType   */ "ZS::System::CException" );
            QCoreApplication::postEvent(this, pMsgExc);
            pMsgExc = nullptr;
            throw;
        }
    }
    catch(...)
    {
        if( QThread::currentThread() != thread() )
        {
            // An ZS::System::CException instance will be temporarily created.
            // This exception instance is used to create an exception error message.
            // The ctor of this exception adds an entry of this event to the error log model.
            CException exc(
                /* strFile    */ "",
                /* iLineNr    */ -1,
                /* result     */ EResultUnknownException,
                /* strAddInfo */ "" );
            CMsgErr* pMsgExc = new CMsgErr(
                /* pObjSender   */ this,
                /* pObjReceiver */ QCoreApplication::instance(),
                /* exc          */ exc,
                /* strExcType   */ "Unknown exception" );
            QCoreApplication::postEvent(this, pMsgExc);
            pMsgExc = nullptr;
            throw;
        }
    } // catch(...)

    try
    {
        CIpcTrcServer::ReleaseInstance();
    }
    catch(...)
    {
    }
} // run
