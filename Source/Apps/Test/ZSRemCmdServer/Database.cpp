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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qmutex.h>

#include "Database.h"
#include "MsgTest.h"

#include "ZSRemCmd/ZSRemCmdObj.h"
#include "ZSRemCmd/ZSRemCmdServer.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::RemCmd;
using namespace ZS::Apps::Test::RemCmdServer;


/*******************************************************************************
class CDatabaseThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrStates[CDatabaseThread::EStateCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CDatabaseThread::EStateIdle,     "Idle"     ),
    /* 1 */ SEnumEntry( CDatabaseThread::EStateStarting, "Starting" ),
    /* 2 */ SEnumEntry( CDatabaseThread::EStateRunning,  "Running"  )
};

//------------------------------------------------------------------------------
QString CDatabaseThread::State2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrStates,EStateCount,i_iState);
}

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrRequests[CDatabaseThread::ERequestCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CDatabaseThread::ERequestNone,     "None"     ),
    /* 1 */ SEnumEntry( CDatabaseThread::ERequestStartup,  "Startup"  ),
    /* 2 */ SEnumEntry( CDatabaseThread::ERequestShutdown, "Shutdown" )
};

//------------------------------------------------------------------------------
QString CDatabaseThread::Request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrRequests,ERequestCount,i_iRequest);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDatabaseThread::CDatabaseThread( const QString& i_strDbObjName ) :
//------------------------------------------------------------------------------
    QThread(),
    m_strDbObjName(i_strDbObjName),
    m_state(EStateIdle),
    m_pReqInProgress(nullptr),
    m_pDb(nullptr),
    m_pRemCmdServer(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(m_strDbObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::RemCmdServer", "CDatabaseThread", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "ctor",
        /* szAddInfo    */ "" );

} // ctor

//------------------------------------------------------------------------------
CDatabaseThread::~CDatabaseThread()
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
                /* errSource     */ "ZS::Apps::Test::RemCmdServer", "CDatabaseThread", objectName(), "dtor",
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
        delete m_pDb;
    }
    catch(...)
    {
    }
    m_pDb = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pRemCmdServer = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDatabaseThread::startup( CRemCmdServer* i_pRemCmdServer, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        if( i_pRemCmdServer == nullptr )
        {
            strAddTrcInfo = "RemCmdServer: nullptr";
        }
        else
        {
            strAddTrcInfo = "RemCmdServer: " + i_pRemCmdServer->objectName();
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

    if( i_pRemCmdServer == nullptr )
    {
        throw CException( __FILE__, __LINE__, EResultArgOutOfRange );
    }
    if( m_state != EStateIdle || m_pReqInProgress != nullptr )
    {
        throw CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }

    m_pRemCmdServer = i_pRemCmdServer;

    m_pReqInProgress = new CRequest(
        /* strNameSpace */ "ZS::Apps::Test::RemCmdServer",
        /* strClassName */ "CDatabaseThread",
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
void CDatabaseThread::start()
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
void CDatabaseThread::run()
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
            m_pDb = new CDatabase(
                /* pDbThread     */ this,
                /* pRemCmdServer */ m_pRemCmdServer,
                /* strDbObjName  */ m_strDbObjName );

            CMsgConStartup* pMsgCon = new CMsgConStartup(
                /* pObjSender        */ m_pDb,
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
                delete m_pDb;
            }
            catch(...)
            {
            }
            m_pDb = nullptr;

        } // if( m_pReqInProgress->getRequest() == ERequestStartup )
    } // if( m_pReqInProgress != nullptr )

} // run

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CDatabaseThread::event( QEvent* i_pEv )
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
class CDatabase : public QObject
*******************************************************************************/

/*==============================================================================
public: // class methods (Remote Commands, Signal Generator)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDatabase::OnExecuteRemCmdSourFunShap(
    QObject*                    i_pObjFctExecute,
    const ZS::Ipc::SSocketDscr& /*i_socketDscr*/,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs,
    QByteArray&                 o_byteArrResults,
    qint64                      /*i_iReqIdParent*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDatabase* pDb = dynamic_cast<CDatabase*>(i_pObjFctExecute);

    if( pDb == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast );
    }

    if( i_bIsQuery )
    {
        ESigGenWaveform sigGenWaveform = pDb->getSigGenWaveform();

        o_byteArrResults = sigGenWaveform2ByteArr(sigGenWaveform);
    }
    else // if( !i_bIsQuery )
    {
        if( !i_byteArrArgs.isEmpty() )
        {
            ESigGenWaveform sigGenWaveform = str2SigGenWaveform(i_byteArrArgs);

            if( sigGenWaveform >= 0 && sigGenWaveform < ESigGenWaveformCount )
            {
                pDb->setSigGenWaveform(sigGenWaveform);
            }
        }
    } // if( !i_bIsQuery )

    return pReq;

} // OnExecuteRemCmdSourFunShap

//------------------------------------------------------------------------------
CRequest* CDatabase::OnExecuteRemCmdSourValMax(
    QObject*                    i_pObjFctExecute,
    const ZS::Ipc::SSocketDscr& /*i_socketDscr*/,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs,
    QByteArray&                 o_byteArrResults,
    qint64                      /*i_iReqIdParent*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDatabase* pDb = dynamic_cast<CDatabase*>(i_pObjFctExecute);

    if( pDb == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast );
    }

    if( i_bIsQuery )
    {
        double fVal = pDb->getSigGenYMax();

        o_byteArrResults = QByteArray::number(fVal,'f',3);
    }
    else // if( !i_bIsQuery )
    {
        if( !i_byteArrArgs.isEmpty() )
        {
            bool   bConverted;
            double fVal = i_byteArrArgs.toDouble(&bConverted);

            if( bConverted )
            {
                pDb->setSigGenYMax(fVal);
            }
        }
    } // if( !i_bIsQuery )

    return pReq;

} // OnExecuteRemCmdSourValMax

//------------------------------------------------------------------------------
CRequest* CDatabase::OnExecuteRemCmdSourValMin(
    QObject*                    i_pObjFctExecute,
    const ZS::Ipc::SSocketDscr& /*i_socketDscr*/,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs,
    QByteArray&                 o_byteArrResults,
    qint64                      /*i_iReqIdParent*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDatabase* pDb = dynamic_cast<CDatabase*>(i_pObjFctExecute);

    if( pDb == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast );
    }

    if( i_bIsQuery )
    {
        double fVal = pDb->getSigGenYMin();

        o_byteArrResults = QByteArray::number(fVal,'f',3);
    }

    else // if( !i_bIsQuery )
    {
        if( !i_byteArrArgs.isEmpty() )
        {
            bool   bConverted;
            double fVal = i_byteArrArgs.toDouble(&bConverted);

            if( bConverted )
            {
                pDb->setSigGenYMin(fVal);
            }
        }

    } // if( !i_bIsQuery )

    return pReq;

} // OnExecuteRemCmdSourValMin

/*==============================================================================
public: // class methods (Remote Commands, Signal Analyzer)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDatabase::OnExecuteRemCmdRead(
    QObject*                    i_pObjFctExecute,
    const ZS::Ipc::SSocketDscr& /*i_socketDscr*/,
    bool                        i_bIsQuery,
    const QByteArray&           /*i_byteArrArgs*/,
    QByteArray&                 o_byteArrResults,
    qint64                      /*i_iReqIdParent*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDatabase* pDb = dynamic_cast<CDatabase*>(i_pObjFctExecute);

    if( pDb == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast );
    }

    if( i_bIsQuery )
    {
        double fVal = pDb->getSigAnaYVal();

        o_byteArrResults = QByteArray::number(fVal,'f',3);
    }

    return pReq;

} // OnExecuteRemCmdRead

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDatabase::CDatabase(
    CDatabaseThread* i_pDbThread,
    CRemCmdServer*   i_pRemCmdServer,
    const QString&   i_strDbObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pDbThread(i_pDbThread),
    m_pRemCmdServer(i_pRemCmdServer),
    // Signal generator
    m_pMtxSigGenClients(nullptr),
    m_arpObjSigGenClients(),
    m_sigGenWaveform(ESigGenWaveformSine),
    m_fSigGenYMin(-1.0),
    m_fSigGenYMax(1.0),
    m_fSigGenFreq_Hz(1.0),
    // Signal analyzer
    m_pMtxSigAnaClients(nullptr),
    m_arpObjSigAnaClients(),
    m_fSigAnaSamplingRate_Hz(10.0),
    m_fSigAnaYVal(0.0),
    // Remote Commands (Signal Generator)
    m_pRemCmdObjSourFunShap(nullptr),
    m_pRemCmdObjSourValMax(nullptr),
    m_pRemCmdObjSourValMin(nullptr),
    // Remote Commands (Signal Analyzer)
    m_pRemCmdObjRead(nullptr),
    // Tracing
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strDbObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::RemCmdServer", "CDatabase", i_strDbObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "ctor",
        /* szAddInfo    */ "" );

    m_pMtxSigGenClients = new QMutex();
    m_pMtxSigAnaClients = new QMutex();

    // Remote Commands (Signal Generator)
    //-----------------------------------

    if( m_pRemCmdServer != nullptr )
    {
        m_pRemCmdObjSourFunShap = new CRemCmdObj(
            /* pServer        */ m_pRemCmdServer,
            /* strPath        */ "SOURCE:FUNCTION:SHAPE",
            /* pObjFctExecute */ this,
            /* pFctExecute    */ OnExecuteRemCmdSourFunShap );

        m_pRemCmdObjSourValMax = new CRemCmdObj(
            /* pServer        */ m_pRemCmdServer,
            /* strPath        */ "SOURCE:VALUE:MAX",
            /* pObjFctExecute */ this,
            /* pFctExecute    */ OnExecuteRemCmdSourValMax );

        m_pRemCmdObjSourValMin = new CRemCmdObj(
            /* pServer        */ m_pRemCmdServer,
            /* strPath        */ "SOURCE:VALUE:MIN",
            /* pObjFctExecute */ this,
            /* pFctExecute    */ OnExecuteRemCmdSourValMin );

    } // if( m_pRemCmdServer != nullptr )

    // Remote Commands (Signal Generator)
    //-----------------------------------

    if( m_pRemCmdServer != nullptr )
    {
        m_pRemCmdObjRead = new CRemCmdObj(
            /* pServer        */ m_pRemCmdServer,
            /* strPath        */ "READ",
            /* pObjFctExecute */ this,
            /* pFctExecute    */ OnExecuteRemCmdRead );

    } // if( m_pRemCmdServer != nullptr )

} // ctor

//------------------------------------------------------------------------------
CDatabase::~CDatabase()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "dtor",
        /* szAddInfo    */ "" );

    // Remote commands (Signal Generator)
    //-----------------------------------

    try
    {
        delete m_pRemCmdObjSourFunShap;
    }
    catch(...)
    {
    }
    m_pRemCmdObjSourFunShap = nullptr;

    try
    {
        delete m_pRemCmdObjSourValMax;
    }
    catch(...)
    {
    }
    m_pRemCmdObjSourValMax = nullptr;

    try
    {
        delete m_pRemCmdObjSourValMin;
    }
    catch(...)
    {
    }
    m_pRemCmdObjSourValMin = nullptr;

    // Remote commands (Signal Analyzer)
    //-----------------------------------

    try
    {
        delete m_pRemCmdObjRead;
    }
    catch(...)
    {
    }
    m_pRemCmdObjRead = nullptr;

    // Signal generator
    //-----------------

    try
    {
        delete m_pMtxSigGenClients;
    }
    catch(...)
    {
    }
    m_pMtxSigGenClients = nullptr;

    // Signal analyzer
    //-----------------

    try
    {
        delete m_pMtxSigAnaClients;
    }
    catch(...)
    {
    }
    m_pMtxSigAnaClients = nullptr;

    // Tracing
    //-----------------

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    // Reset
    //------

    m_pDbThread = nullptr;
    m_pRemCmdServer = nullptr;
    // Signal generator
    m_pMtxSigGenClients = nullptr;
    m_arpObjSigGenClients.clear();
    m_sigGenWaveform = static_cast<ESigGenWaveform>(0);
    m_fSigGenYMin = 0.0;
    m_fSigGenYMax = 0.0;
    m_fSigGenFreq_Hz = 0.0;
    // Signal analyzer
    m_pMtxSigAnaClients = nullptr;
    m_arpObjSigAnaClients.clear();
    m_fSigAnaSamplingRate_Hz = 0.0;
    m_fSigAnaYVal = 0.0;
    // Remote commands
    m_pRemCmdObjSourFunShap = nullptr;
    // Tracing
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDatabase::registerSigGenClient( QObject* i_pObjClient )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtxSigGenClients);

    m_arpObjSigGenClients.append(i_pObjClient);

} // registerSigGenClient

//------------------------------------------------------------------------------
void CDatabase::unregisterSigGenClient( QObject* i_pObjClient )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtxSigGenClients);

    m_arpObjSigGenClients.removeOne(i_pObjClient);

} // unregisterSigGenClient

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDatabase::registerSigAnaClient( QObject* i_pObjClient )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtxSigAnaClients);

    m_arpObjSigAnaClients.append(i_pObjClient);

} // registerSigAnaClient

//------------------------------------------------------------------------------
void CDatabase::unregisterSigAnaClient( QObject* i_pObjClient )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtxSigAnaClients);

    m_arpObjSigAnaClients.removeOne(i_pObjClient);

} // unregisterSigAnaClient

/*==============================================================================
public: // instance methods( Signal Generator)
==============================================================================*/

//------------------------------------------------------------------------------
void CDatabase::setSigGenWaveform( ESigGenWaveform i_sigGenWaveform )
//------------------------------------------------------------------------------
{
    m_sigGenWaveform = i_sigGenWaveform;

    QMutexLocker mtxLocker(m_pMtxSigGenClients);

    QObject* pObjClient;
    int      idxClt;

    for( idxClt = 0; idxClt < m_arpObjSigGenClients.size(); idxClt++ )
    {
        pObjClient = m_arpObjSigGenClients[idxClt];

        CMsgIndDbParValChanged* pMsgInd = new CMsgIndDbParValChanged(
            /* pObjSender   */ this,
            /* pObjReceiver */ pObjClient,
            /* strDbPar     */ c_strDbParSigGenWaveform,
            /* val          */ m_sigGenWaveform );
        POST_OR_DELETE_MESSAGE(pMsgInd);
        pMsgInd = nullptr;
    }

} // setSigGenWaveform

//------------------------------------------------------------------------------
void CDatabase::setSigGenYMin( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fSigGenYMin = i_fVal;

    QMutexLocker mtxLocker(m_pMtxSigGenClients);

    QObject* pObjClient;
    int      idxClt;

    for( idxClt = 0; idxClt < m_arpObjSigGenClients.size(); idxClt++ )
    {
        pObjClient = m_arpObjSigGenClients[idxClt];

        CMsgIndDbParValChanged* pMsgInd = new CMsgIndDbParValChanged(
            /* pObjSender   */ this,
            /* pObjReceiver */ pObjClient,
            /* strDbPar     */ c_strDbParSigGenYMin,
            /* val          */ m_fSigGenYMin );
        POST_OR_DELETE_MESSAGE(pMsgInd);
        pMsgInd = nullptr;
    }

} // setSigGenYMin

//------------------------------------------------------------------------------
void CDatabase::setSigGenYMax( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fSigGenYMax = i_fVal;

    QMutexLocker mtxLocker(m_pMtxSigGenClients);

    QObject* pObjClient;
    int      idxClt;

    for( idxClt = 0; idxClt < m_arpObjSigGenClients.size(); idxClt++ )
    {
        pObjClient = m_arpObjSigGenClients[idxClt];

        CMsgIndDbParValChanged* pMsgInd = new CMsgIndDbParValChanged(
            /* pObjSender   */ this,
            /* pObjReceiver */ pObjClient,
            /* strDbPar     */ c_strDbParSigGenYMax,
            /* val          */ m_fSigGenYMax );
        POST_OR_DELETE_MESSAGE(pMsgInd);
        pMsgInd = nullptr;
    }

} // setSigGenYMax

//------------------------------------------------------------------------------
void CDatabase::setSigGenFreqInHz( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fSigGenFreq_Hz = i_fVal;

    QMutexLocker mtxLocker(m_pMtxSigGenClients);

    QObject* pObjClient;
    int      idxClt;

    for( idxClt = 0; idxClt < m_arpObjSigGenClients.size(); idxClt++ )
    {
        pObjClient = m_arpObjSigGenClients[idxClt];

        CMsgIndDbParValChanged* pMsgInd = new CMsgIndDbParValChanged(
            /* pObjSender   */ this,
            /* pObjReceiver */ pObjClient,
            /* strDbPar     */ c_strDbParSigGenFreq,
            /* val          */ m_fSigGenFreq_Hz );
        POST_OR_DELETE_MESSAGE(pMsgInd);
        pMsgInd = nullptr;
    }

} // setSigGenFreqInHz

/*==============================================================================
public: // instance methods( Signal Analyzer)
==============================================================================*/

//------------------------------------------------------------------------------
void CDatabase::setSigAnaSamplingRateInHz( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fSigAnaSamplingRate_Hz = i_fVal;

    QMutexLocker mtxLocker(m_pMtxSigAnaClients);

    QObject* pObjClient;
    int      idxClt;

    for( idxClt = 0; idxClt < m_arpObjSigAnaClients.size(); idxClt++ )
    {
        pObjClient = m_arpObjSigAnaClients[idxClt];

        CMsgIndDbParValChanged* pMsgInd = new CMsgIndDbParValChanged(
            /* pObjSender   */ this,
            /* pObjReceiver */ pObjClient,
            /* strDbPar     */ c_strDbParSigAnaSamplingRate,
            /* val          */ m_fSigAnaSamplingRate_Hz );
        POST_OR_DELETE_MESSAGE(pMsgInd);
        pMsgInd = nullptr;
    }

} // setSigAnaSamplingRateInHz

//------------------------------------------------------------------------------
void CDatabase::setSigAnaYVal( double i_fVal )
//------------------------------------------------------------------------------
{
    m_fSigAnaYVal = i_fVal;

    QMutexLocker mtxLocker(m_pMtxSigAnaClients);

    QObject* pObjClient;
    int      idxClt;

    for( idxClt = 0; idxClt < m_arpObjSigAnaClients.size(); idxClt++ )
    {
        pObjClient = m_arpObjSigAnaClients[idxClt];

        CMsgIndDbParValChanged* pMsgInd = new CMsgIndDbParValChanged(
            /* pObjSender   */ this,
            /* pObjReceiver */ pObjClient,
            /* strDbPar     */ c_strDbParSigAnaYVal,
            /* val          */ m_fSigAnaYVal );
        POST_OR_DELETE_MESSAGE(pMsgInd);
        pMsgInd = nullptr;
    }

} // setSigAnaYVal

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CDatabase::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    switch( static_cast<int>(i_pEv->type()) )
    {
        case EMsgTypeReqDbParSetVal:
        {
            CMsgReqDbParSetVal* pMsgReq = dynamic_cast<CMsgReqDbParSetVal*>(i_pEv);

            if( pMsgReq != nullptr )
            {
                if( pMsgReq->getDbPar() == c_strDbParSigGenWaveform )
                {
                    if( pMsgReq->getVal().toInt() >= 0 && pMsgReq->getVal().toInt() < ESigGenWaveformCount )
                    {
                        setSigGenWaveform( static_cast<ESigGenWaveform>(pMsgReq->getVal().toInt()) );
                    }
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigGenYMin )
                {
                    setSigGenYMin( pMsgReq->getVal().toDouble() );
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigGenYMax )
                {
                    setSigGenYMax( pMsgReq->getVal().toDouble() );
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigGenFreq )
                {
                    setSigGenFreqInHz( pMsgReq->getVal().toDouble() );
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigAnaSamplingRate )
                {
                    setSigAnaSamplingRateInHz( pMsgReq->getVal().toDouble() );
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigAnaYVal )
                {
                    setSigAnaYVal( pMsgReq->getVal().toDouble() );
                }

                if( pMsgReq->mustBeConfirmed() )
                {
                    CMsgConDbParSetVal* pMsgCon = new CMsgConDbParSetVal(
                        /* pObjSender        */ this,
                        /* pObjReceiver      */ pMsgReq->getSender(),
                        /* strDbPar          */ pMsgReq->getDbPar(),
                        /* val               */ pMsgReq->getVal(),
                        /* iReqId            */ pMsgReq->getRequestId(),
                        /* iMsgIdReq         */ pMsgReq->getMsgId(),
                        /* errResultInfo     */ SErrResultInfo(),
                        /* iProgress_perCent */ 100 );
                    POST_OR_DELETE_MESSAGE(pMsgCon);
                    pMsgCon = nullptr;
                }
            }

            bEventHandled = true;
            break;
        } // case EMsgTypeReqDbParSetVal:

        case EMsgTypeReqDbParGetVal:
        {
            CMsgReqDbParGetVal* pMsgReq = dynamic_cast<CMsgReqDbParGetVal*>(i_pEv);

            if( pMsgReq != nullptr )
            {
                QVariant val;

                if( pMsgReq->getDbPar() == c_strDbParSigGenWaveform )
                {
                    val = static_cast<int>(m_sigGenWaveform);
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigGenYMin )
                {
                    val = m_fSigGenYMin;
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigGenYMax )
                {
                    val = m_fSigGenYMax;
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigGenFreq )
                {
                    val = m_fSigGenFreq_Hz;
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigAnaSamplingRate )
                {
                    val = m_fSigAnaSamplingRate_Hz;
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigAnaYVal )
                {
                    val = m_fSigAnaYVal;
                }

                CMsgConDbParGetVal* pMsgCon = new CMsgConDbParGetVal(
                    /* pObjSender        */ this,
                    /* pObjReceiver      */ pMsgReq->getSender(),
                    /* strDbPar          */ pMsgReq->getDbPar(),
                    /* val               */ val,
                    /* iReqId            */ pMsgReq->getRequestId(),
                    /* iMsgIdReq         */ pMsgReq->getMsgId(),
                    /* errResultInfo     */ SErrResultInfo(),
                    /* iProgress_perCent */ 100 );
                POST_OR_DELETE_MESSAGE(pMsgCon);
                pMsgCon = nullptr;
            }

            bEventHandled = true;
            break;
        } // case EMsgTypeReqDbParGetVal:

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
