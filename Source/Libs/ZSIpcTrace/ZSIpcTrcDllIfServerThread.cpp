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
    const QString& i_strServerName,
    int            i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    QThread(),
    m_strServerName(i_strServerName),
    m_pQtAppCreatedByDllIf(NULL),
    m_iTrcDetailLevel(i_iTrcDetailLevel),
    m_pTrcMthFile(nullptr),
    m_bTrcServerCreatedByMe(false)
{
    setObjectName(i_strServerName + "DllIf");

    #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    QString strAdminObjFileAbsFilePath;
    QString strLocalTrcFileAbsFilePath;

    CTrcServer::GetDefaultFilePaths(strAdminObjFileAbsFilePath, strLocalTrcFileAbsFilePath);

    m_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );

    #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

} // ctor

//------------------------------------------------------------------------------
DllIf::CIpcTrcServerThread::~CIpcTrcServerThread()
//------------------------------------------------------------------------------
{
    // The method tracer to trace method enter and method leave cannot be used here.
    // The trace method file will be destroyed before method leave is traced.
    // As a workaround the method tracers scope is left before the trace method
    // file is closed and freed.

    #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    {   CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
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
    }

    CTrcMthFile::Free(m_pTrcMthFile);

    #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    if( m_bTrcServerCreatedByMe )
    {
        try
        {
            CIpcTrcServer::DestroyInstance(m_strServerName);
        }
        catch(...)
        {
        }
    }

    if( m_pQtAppCreatedByDllIf != NULL )
    {
        try
        {
            delete m_pQtAppCreatedByDllIf;
        }
        catch(...)
        {
        }
    }

    //m_strServerName;
    m_pQtAppCreatedByDllIf = nullptr;
    m_iTrcDetailLevel = 0;
    m_pTrcMthFile = nullptr;
    m_bTrcServerCreatedByMe = false;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CIpcTrcServerThread::isServerCreated()
//------------------------------------------------------------------------------
{
    return (CIpcTrcServer::GetInstance(m_strServerName) != nullptr);
}

//------------------------------------------------------------------------------
bool DllIf::CIpcTrcServerThread::isServerStarted()
//------------------------------------------------------------------------------
{
    bool bServerStarted = false;

    CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance(m_strServerName);

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

    CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance(m_strServerName);

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
    #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "run",
        /* strMthInArgs       */ "" );

    #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    // Create trace server
    //--------------------

    if( CIpcTrcServer::GetInstance(m_strServerName) == nullptr )
    {
        CIpcTrcServer::CreateInstance(m_strServerName, m_iTrcDetailLevel);

        if( CIpcTrcServer::GetInstance(m_strServerName) != nullptr )
        {
            m_bTrcServerCreatedByMe = true;
        }
    }

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

    if( m_bTrcServerCreatedByMe )
    {
        try
        {
            CIpcTrcServer::DestroyInstance(m_strServerName);
        }
        catch(...)
        {
        }
        m_bTrcServerCreatedByMe = false;
    }

    if( m_pQtAppCreatedByDllIf != NULL )
    {
        try
        {
            delete m_pQtAppCreatedByDllIf;
        }
        catch(...)
        {
        }
        m_pQtAppCreatedByDllIf = NULL;
    }

} // run
