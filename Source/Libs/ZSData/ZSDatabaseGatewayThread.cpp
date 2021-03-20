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

#include "ZSData/ZSDatabaseGatewayThread.h"
#include "ZSData/ZSDatabaseGatewayZSXML.h"
#include "ZSData/ZSDatabase.h"
#include "ZSData/ZSDataMsg.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Data;


/*******************************************************************************
class CDbGatewayThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDbGatewayThread::CDbGatewayThread(
    EProtocolType        i_protocolType,
    ZS::Ipc::ESocketType i_socketType,
    CDb*                 i_pDb ) :
//------------------------------------------------------------------------------
    QThread(),
    m_pDb(i_pDb),
    m_protocolType(i_protocolType),
    m_socketType(i_socketType),
    m_pGateway(nullptr),
    m_iReqIdStart(-1),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( m_pDb->objectName() + "Gateway" ); // set name of thread

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CDbGatewayThread::~CDbGatewayThread()
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
            SErrResultInfo errResultInfo = ErrResultInfoError("dtor", EResultTimeout, "Waiting for thread to quit timed out");
            CErrLog::GetInstance()->addEntry(errResultInfo);
         }
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDb = nullptr;
    m_protocolType = static_cast<EProtocolType>(0);
    m_socketType = static_cast<ESocketType>(0);
    m_pGateway = nullptr;
    m_iReqIdStart = 0;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDbGatewayThread::start( qint64 i_iReqId )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "ReqId: " + QString::number(i_iReqId);

        if( i_iReqId >= 0 && m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
        {
            int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
            strAddTrcInfo += " (" + CRequestExecTree::GetAddTrcInfoStr(i_iReqId,iAddTrcInfoDetailLevel) + ")";
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "start",
        /* strAddInfo   */ strAddTrcInfo );

    m_iReqIdStart = i_iReqId;

    QThread::start();

} // start

/*==============================================================================
protected: // hiding public overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CDbGatewayThread::start()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "start",
        /* strAddInfo   */ strAddTrcInfo );

    QThread::start();

} // start

/*==============================================================================
public: // must overridables (overridables of base class QThread)
==============================================================================*/

//------------------------------------------------------------------------------
void CDbGatewayThread::run()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "run",
        /* strAddInfo   */ "" );

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".run";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    if( m_protocolType == EProtocolTypeZSXML )
    {
        if( m_socketType == ESocketTypeTcp )
        {
            m_pGateway = new CDbGatewayZSXML(
                /* socketType */ m_socketType,
                /* pDb        */ m_pDb,
                /* pThread    */ this );
        }
        else if( m_socketType == ESocketTypeInProcMsg )
        {
            // No gateway necessary. Messages are directly sent from client
            // to event method of database object.
        }
        else // if( m_socketType != ESocketTypeTcp && m_socketType != ESocketTypeInProcMsg )
        {
            strAddErrInfo = "Socket type \"" + socketType2Str(m_socketType) + "\" is not supported.";
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultOptionNotSupported);
            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
    }
    else // if( m_protocolType != EProtocolTypeZSXML )
    {
        strAddErrInfo = "Protocol type \"" + protocolType2Str(m_protocolType) + "\" is not supported.";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultOptionNotSupported);
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    if( m_iReqIdStart >= 0 )
    {
        CRequestExecTree* pReqExecTree = CRequestExecTree::GetInstance();

        bool bIsBlockingRequest = false;

        if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(m_iReqIdStart) )
        {
            bIsBlockingRequest = true;

            // The thread starting object must have called "wait" on the
            // wait condition. Otherwise "wakeAll" may never return. To ensure
            // this we force this thread to sleep for a moment ...
            while( !pReqExecTree->isWaiting(m_iReqIdStart) && !pReqExecTree->waitTimedOut(m_iReqIdStart) )
            {
                msleep(10);
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqStartThread.WaitCondition.wakeAll()" );
            }

            if( !pReqExecTree->wake(m_iReqIdStart) )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultInternalStateMachineError);
                errResultInfo.setAddErrInfoDscr("Waking up thread waiting for start thread request failed");

                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- ReqStartThread.WaitCondition.wakeAll(): " + errResultInfo.getResultStr() );
            }
        } // if( pReqExecTree != nullptr && pReqExecTree->isBlockingRequest(m_iReqIdStartThread) )

        if( !bIsBlockingRequest )
        {
            CMsgConStartup* pMsgConStartup = new CMsgConStartup(
                /* pObjSender        */ m_pGateway,
                /* pObjReceiver      */ m_pDb,
                /* iReqId            */ m_iReqIdStart,
                /* iMsgIdReq         */ -1,
                /* errResultInfo     */ errResultInfo,
                /* iProgress_perCent */ 100 );
            POST_OR_DELETE_MESSAGE(pMsgConStartup, &mthTracer, ETraceDetailLevelRuntimeInfo);
            pMsgConStartup = nullptr;
        }
    } // if( m_iReqIdStart >= 0 )

    try
    {
        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "-+ QThread::exec()" );
        }

        exec();

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "+- QThread::exec()" );
        }
    }
    catch( CException* )
    {
        // This type of exception has already been logged on throwing (creating) the exception.
    }
    catch( CException& )
    {
        // This type of exception has already been logged on throwing (creating) the exception.
    }
    catch(...)
    {
        errResultInfo.setSeverity(EResultSeverityCritical);
        errResultInfo.setResult(EResultUnknownException);

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    /*--------------------------------------------------------------------------
    !! Note on deleting worker instances like the gateway instance created !!
    !! in the context of worker threads like this gateway thread.          !!

    Can't delete the gateway object in the dtor of the thread as it has been
    created in the context of the gateway thread. The dtor of the thread is
    (or must) be called from within the thread from which the thread has
    been created and started. So the worker gateway instance must be deleted
    on leaving the "run" method of the thread.
    --------------------------------------------------------------------------*/

    try
    {
        delete m_pGateway;
    }
    catch(...)
    {
    }
    m_pGateway = nullptr;

} // run
