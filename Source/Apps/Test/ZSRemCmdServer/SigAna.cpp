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

#include <QtCore/qtimer.h>

#include "SigAna.h"
#include "App.h"
#include "Database.h"
#include "SigGen.h"
#include "MsgTest.h"

#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::RemCmdServer;


/*******************************************************************************
class CSigAnaThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrStates[CSigAnaThread::EStateCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CSigAnaThread::EStateIdle,     "Idle"     ),
    /* 1 */ SEnumEntry( CSigAnaThread::EStateStarting, "Starting" ),
    /* 2 */ SEnumEntry( CSigAnaThread::EStateRunning,  "Running"  )
};

//------------------------------------------------------------------------------
QString CSigAnaThread::State2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrStates,EStateCount,i_iState);
}

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrRequests[CSigAnaThread::ERequestCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CSigAnaThread::ERequestNone,     "None"     ),
    /* 1 */ SEnumEntry( CSigAnaThread::ERequestStartup,  "Startup"  ),
    /* 2 */ SEnumEntry( CSigAnaThread::ERequestShutdown, "Shutdown" )
};

//------------------------------------------------------------------------------
QString CSigAnaThread::Request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrRequests,ERequestCount,i_iRequest);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CSigAnaThread::CSigAnaThread( const QString& i_strSigAnaObjName ) :
//------------------------------------------------------------------------------
    QThread(),
    m_strSigAnaObjName(i_strSigAnaObjName),
    m_pDb(nullptr),
    m_pSigGen(nullptr),
    m_state(EStateIdle),
    m_pReqInProgress(nullptr),
    m_pSigAna(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(m_strSigAnaObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::RemCmdServer", "CSigAnaThread", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "ctor",
        /* szAddInfo    */ "" );

} // ctor

//------------------------------------------------------------------------------
CSigAnaThread::~CSigAnaThread()
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
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo(
                /* errSource     */ "ZS::Apps::Test::RemCmdServer", "CSigAnaThread", objectName(), "dtor",
                /* result        */ EResultTimeout,
                /* severity      */ EResultSeverityError,
                /* strAddErrInfo */ "Waiting for thread to quit timed out" );
            CErrLog::GetInstance()->addEntry(errResultInfo);
         }
    }

    try
    {
        delete m_pReqInProgress;
    }
    catch(...)
    {
    }
    m_pReqInProgress = nullptr;

    try
    {
        delete m_pSigAna;
    }
    catch(...)
    {
    }
    m_pSigAna = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pDb = nullptr;
    m_pSigGen = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CSigAnaThread::startup(
    CDatabase* i_pDb,
    CSigGen*   i_pSigGen,
    qint64     i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( i_pDb == nullptr )
        {
            strAddTrcInfo = "Db: nullptr";
        }
        else
        {
            strAddTrcInfo = "Db: " + i_pDb->objectName();
        }

        if( i_pSigGen == nullptr )
        {
            strAddTrcInfo = "SigGen: nullptr";
        }
        else
        {
            strAddTrcInfo = "SigGen: " + i_pSigGen->objectName();
        }

        strAddTrcInfo += ", ReqParent: " + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", State: " + State2Str(m_state);

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

    if( i_pDb == nullptr )
    {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange );
    }
    if( i_pSigGen == nullptr )
    {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange );
    }
    if( m_state != EStateIdle || m_pReqInProgress != nullptr )
    {
        throw CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }

    m_pDb = i_pDb;
    m_pSigGen = i_pSigGen;

    m_pReqInProgress = new CRequest(
        /* strNameSpace */ "ZS::Apps::Test::RemCmdServer",
        /* strClassName */ "CSigAnaThread",
        /* pObjCreator  */ this,
        /* iRequest     */ ERequestStartup,
        /* strRequest   */ Request2Str(ERequestStartup),
        /* iReqIdParent */ i_iReqIdParent );
    m_state = EStateStarting;

    start();

    return m_pReqInProgress;

} // startup

/*==============================================================================
protected: // overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CSigAnaThread::start()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "start",
        /* szAddInfo    */ "" );

    QThread::start();

} // start

//------------------------------------------------------------------------------
void CSigAnaThread::run()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "run",
        /* szAddInfo    */ "" );

    if( m_pReqInProgress != nullptr )
    {
        if( m_pReqInProgress->getRequest() == ERequestStartup )
        {
            m_pSigAna = new CSigAna(
                /* pSigAnaThread */ this,
                /* pDb           */ m_pDb,
                /* pSigGen       */ m_pSigGen,
                /* strObjName    */ m_strSigAnaObjName );

            // Send confirmation message to event method of thread. The message
            // will be received in the context of the thread creating the thread
            // instance (the applications GUI main thread).
            CMsgConStartup* pMsgCon = new CMsgConStartup(
                /* pObjSender        */ m_pSigAna,
                /* pObjReceiver      */ this,
                /* iReqId            */ m_pReqInProgress->getId(),
                /* iMsgIdReq         */ m_pReqInProgress->getId(),
                /* errResultInfo     */ SErrResultInfo(),
                /* iProgress_perCent */ 100 );
            POST_OR_DELETE_MESSAGE(pMsgCon);
            pMsgCon = nullptr;

            exec();

            try
            {
                delete m_pSigAna;
            }
            catch(...)
            {
            }
            m_pSigAna = nullptr;

        } // if( m_pReqInProgress->getRequest() == ERequestStartup )
    } // if( m_pReqInProgress != nullptr )

} // run

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CSigAnaThread::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    switch( static_cast<int>(i_pEv->type()) )
    {
        case EMsgTypeConStartup:
        {
            CMsgConStartup* pMsgCon = dynamic_cast<CMsgConStartup*>(i_pEv);
            if( pMsgCon == nullptr )
            {
                throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
            }

            if( pMsgCon->getProgress() >= 100 )
            {
                if( m_pReqInProgress != nullptr )
                {
                    if( m_pReqInProgress->getRequest() == ERequestStartup )
                    {
                        SErrResultInfo errResultInfo = pMsgCon->getErrResultInfo();

                        if( errResultInfo.isErrorResult() )
                        {
                            m_state = EStateIdle;
                        }
                        else
                        {
                            m_state = EStateRunning;
                        }

                        m_pReqInProgress->setErrResultInfo(errResultInfo);
                        m_pReqInProgress->setProgressInPerCent(100);
                        m_pReqInProgress->update();

                        deleteRequest(m_pReqInProgress);
                        m_pReqInProgress = nullptr;

                    } // if( m_pReqInProgress->getRequest() == ERequestStartup )
                } // if( m_pReqInProgress != nullptr )
            } // if( pMsgCon->getProgress() >= 100 )

            bEventHandled = true;
            break;
        }
        default:
        {
            break;
        }
    } // switch( i_pMsg->type() )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event


/*******************************************************************************
class CSigAna
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CSigAna::CSigAna(
    CSigAnaThread* i_pSigAnaThread,
    CDatabase*     i_pDb,
    CSigGen*       i_pSigGen,
    const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pSigAnaThread(i_pSigAnaThread),
    m_pDb(i_pDb),
    m_pSigGen(i_pSigGen),
    m_fSamplingRate_Hz(10.0),
    m_fYVal(0.0),
    m_pTmrSample(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::RemCmdServer", "CSigAna", i_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "ctor",
        /* szAddInfo    */ "" );

    // Use "direct connection" to avoid that the "onDestroyed" slot is called after
    // the object has already been completely destroyed.
    if( !QObject::connect(
        /* pObjSender     */ m_pDb,
        /* szSignal       */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onDbDestroyed(QObject*)),
        /* connectionType */ Qt::DirectConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Use "direct connection" to avoid that the "onDestroyed" slot is called after
    // the object has already been completely destroyed.
    if( !QObject::connect(
        /* pObjSender     */ m_pSigGen,
        /* szSignal       */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onSigGenDestroyed(QObject*)),
        /* connectionType */ Qt::DirectConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pTmrSample = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender     */ m_pTmrSample,
        /* szSignal       */ SIGNAL(timeout()),
        /* pObjReceiver   */ this,
        /* szSlot         */ SLOT(onTmrSampleTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( m_fSamplingRate_Hz > 0.0 )
    {
        int iTimeout_ms = 1000.0 / m_fSamplingRate_Hz;

        m_pTmrSample->start(iTimeout_ms);
    }

    m_pDb->registerSigAnaClient(this);

    CMsgReqDbParGetVal* pMsgReq;

    pMsgReq = new CMsgReqDbParGetVal(
        /* pObjSender   */ this,
        /* pObjReceiver */ m_pDb,
        /* strDbPar     */ c_strDbParSigAnaSamplingRate,
        /* iReqId       */ -1 );
    POST_OR_DELETE_MESSAGE(pMsgReq);
    pMsgReq = nullptr;

} // ctor

//------------------------------------------------------------------------------
CSigAna::~CSigAna()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "dtor",
        /* szAddInfo    */ "" );

    if( m_pDb != nullptr )
    {
        m_pDb->unregisterSigAnaClient(this);
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pSigAnaThread = nullptr;
    m_pDb = nullptr;
    m_pSigGen = nullptr;
    m_fSamplingRate_Hz = 0.0;
    m_fYVal = 0.0;
    m_pTmrSample = nullptr;

} // dtor

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CSigAna::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    switch( static_cast<int>(i_pEv->type()) )
    {
        case EMsgTypeConDbParGetVal:
        {
            CMsgConDbParGetVal* pMsgCon = dynamic_cast<CMsgConDbParGetVal*>(i_pEv);

            if( pMsgCon != nullptr )
            {
                if( pMsgCon->getDbPar() == c_strDbParSigAnaSamplingRate )
                {
                    m_fSamplingRate_Hz = pMsgCon->getVal().toDouble();

                    if( m_pTmrSample->isActive() )
                    {
                        m_pTmrSample->stop();
                    }

                    if( m_fSamplingRate_Hz > 0.0 )
                    {
                        int iTimeout_ms = 1000.0 / m_fSamplingRate_Hz;

                        m_pTmrSample->start(iTimeout_ms);
                    }
                }

                else if( pMsgCon->getDbPar() == c_strDbParSigAnaYVal ) // read only
                {
                    m_fYVal = pMsgCon->getVal().toDouble();

                    // Current value has been queried from signal generator.
                    // Forward measure value to database:
                    if( m_pDb != nullptr )
                    {
                        CMsgReqDbParSetVal* pMsgReq = new CMsgReqDbParSetVal(
                            /* pObjSender       */ this,
                            /* pObjReceiver     */ m_pDb,
                            /* strDbPar         */ pMsgCon->getDbPar(),
                            /* val              */ m_fYVal,
                            /* bMustBeConfirmed */ true,
                            /* iReqId           */ -1 );
                        POST_OR_DELETE_MESSAGE(pMsgReq);
                        pMsgReq = nullptr;
                    }
                }
            }
            bEventHandled = true;
            break;
        } // case EMsgTypeConDbParGetVal

        case EMsgTypeIndDbParValChanged:
        {
            CMsgIndDbParValChanged* pMsgInd = dynamic_cast<CMsgIndDbParValChanged*>(i_pEv);

            if( pMsgInd != nullptr )
            {
                if( pMsgInd->getDbPar() == c_strDbParSigAnaSamplingRate )
                {
                    m_fSamplingRate_Hz = pMsgInd->getVal().toDouble();

                    if( m_pTmrSample->isActive() )
                    {
                        m_pTmrSample->stop();
                    }

                    if( m_fSamplingRate_Hz > 0.0 )
                    {
                        int iTimeout_ms = 1000.0 / m_fSamplingRate_Hz;

                        m_pTmrSample->start(iTimeout_ms);
                    }
                }
            }
            bEventHandled = true;
            break;
        } // case EMsgTypeIndDbParValChanged

        default:
        {
            break;
        }

    } // switch( i_pMsg->type() )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CSigAna::onTmrSampleTimeout()
//------------------------------------------------------------------------------
{
    if( m_pSigGen != nullptr )
    {
        CMsgReqDbParGetVal* pMsgReq = new CMsgReqDbParGetVal(
            /* pObjSender   */ this,
            /* pObjReceiver */ m_pSigGen,
            /* strDbPar     */ c_strDbParSigAnaYVal,
            /* iReqId       */ -1 );
        POST_OR_DELETE_MESSAGE(pMsgReq);
        pMsgReq = nullptr;
    }

} // onTmrSampleTimeout

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CSigAna::onDbDestroyed( QObject* /*i_pDb*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onDbDestroyed",
        /* szAddInfo    */ "" );

    m_pDb = nullptr;

} // onDbDestroyed

//------------------------------------------------------------------------------
void CSigAna::onSigGenDestroyed( QObject* /*i_pSigGen*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onSigGenDestroyed",
        /* szAddInfo    */ "" );

    m_pSigGen = nullptr;

} // onSigGenDestroyed
