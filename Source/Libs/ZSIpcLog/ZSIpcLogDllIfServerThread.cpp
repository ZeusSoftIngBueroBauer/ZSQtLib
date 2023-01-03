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

#include "ZSIpcLog/ZSIpcLogDllIfServerThread.h"
#include "ZSIpcLog/ZSIpcLogServer.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysTrcMthFile.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Log;


/*******************************************************************************
class CIpcLogServerThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
DllIf::CIpcLogServerThread::CIpcLogServerThread() :
//------------------------------------------------------------------------------
    QThread()
{
    setObjectName("ZSLogServerDllIf");
}

//------------------------------------------------------------------------------
DllIf::CIpcLogServerThread::~CIpcLogServerThread()
//------------------------------------------------------------------------------
{
    // The log server should have been released at the end of the run method.
    // If not (for whatever unexpected reason) the server will be released here.
    if( CIpcLogServer::GetInstance() != nullptr )
    {
        try
        {
            CIpcLogServer::ReleaseInstance();
        }
        catch(...)
        {
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool DllIf::CIpcLogServerThread::isServerCreated()
//------------------------------------------------------------------------------
{
    return (CIpcLogServer::GetInstance() != nullptr);
}

//------------------------------------------------------------------------------
bool DllIf::CIpcLogServerThread::isServerStarted()
//------------------------------------------------------------------------------
{
    bool bServerStarted = false;

    CIpcLogServer* pLogServer = CIpcLogServer::GetInstance();

    if( pLogServer != NULL )
    {
        bServerStarted = (pLogServer->getState() == ZS::Ipc::CServer::EStateListening);
    }
    return bServerStarted;
}

//------------------------------------------------------------------------------
bool DllIf::CIpcLogServerThread::isServerShutdown()
//------------------------------------------------------------------------------
{
    bool bServerShutdown = true;

    CIpcLogServer* pLogServer = CIpcLogServer::GetInstance();

    if( pLogServer != NULL )
    {
        bServerShutdown = (pLogServer->getState() == ZS::Ipc::CServer::EStateIdle);
    }
    return bServerShutdown;
}

/*==============================================================================
public: // overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void DllIf::CIpcLogServerThread::run()
//------------------------------------------------------------------------------
{
    // Create log server
    //------------------

    CIpcLogServer::CreateInstance();

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
        CIpcLogServer::ReleaseInstance();
    }
    catch(...)
    {
    }
} // run
