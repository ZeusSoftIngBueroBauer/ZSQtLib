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

#include <QtCore/qglobal.h>

#include "SigGen.h"
#include "App.h"
#include "Database.h"
#include "MsgTest.h"

#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::RemCmdServer;


/*******************************************************************************
Type definitions and constants
*******************************************************************************/

/*==============================================================================
enum ESigGenWaveform
==============================================================================*/

const SEnumEntry s_arEnumSigGenWaveforms[ESigGenWaveformCount] =
{
    /* 0 */ SEnumEntry( ESigGenWaveformSine,     "Sine"     )
    ///* 1 */ SEnumEntry( ESigGenWaveformSquare,   "Square"   ),
    ///* 2 */ SEnumEntry( ESigGenWaveformTriangle, "Triangle" ),
    ///* 3 */ SEnumEntry( ESigGenWaveformSawtooth, "Sawtooth" )
};

//------------------------------------------------------------------------------
QString ZS::Apps::Test::RemCmdServer::sigGenWaveform2Str( int i_iWaveform )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumSigGenWaveforms,ESigGenWaveformCount,i_iWaveform);
}

//------------------------------------------------------------------------------
ESigGenWaveform ZS::Apps::Test::RemCmdServer::str2SigGenWaveform( const QString& i_strWaveform )
//------------------------------------------------------------------------------
{
    ESigGenWaveform waveform = ESigGenWaveformUndefined;
    int iWaveform = SEnumEntry::str2Enumerator(s_arEnumSigGenWaveforms,ESigGenWaveformCount,i_strWaveform);
    if( iWaveform >= 0 && iWaveform < ESigGenWaveformCount )
    {
        waveform = static_cast<ESigGenWaveform>(iWaveform);
    }
    return waveform;

} // str2SigGenWaveform

//------------------------------------------------------------------------------
QByteArray ZS::Apps::Test::RemCmdServer::sigGenWaveform2ByteArr( int i_iWaveform )
//------------------------------------------------------------------------------
{
    QByteArray byteArr;
    QString str = SEnumEntry::enumerator2Str(s_arEnumSigGenWaveforms,ESigGenWaveformCount,i_iWaveform);
    #if QT_VERSION < 0x050000
    byteArr = str.toAscii();
    #else
    byteArr = str.toLatin1();
    #endif
    return byteArr;

} // sigGenWaveform2ByteArr

//------------------------------------------------------------------------------
ESigGenWaveform ZS::Apps::Test::RemCmdServer::byteArr2SigGenWaveform( const QByteArray& i_strWaveform )
//------------------------------------------------------------------------------
{
    ESigGenWaveform waveform = ESigGenWaveformUndefined;
    int iWaveform = SEnumEntry::str2Enumerator(s_arEnumSigGenWaveforms,ESigGenWaveformCount,i_strWaveform);
    if( iWaveform >= 0 && iWaveform < ESigGenWaveformCount )
    {
        waveform = static_cast<ESigGenWaveform>(iWaveform);
    }
    return waveform;

} // str2SigGenWaveform


/*******************************************************************************
class CSigGenThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrStates[CSigGenThread::EStateCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CSigGenThread::EStateIdle,     "Idle"     ),
    /* 1 */ SEnumEntry( CSigGenThread::EStateStarting, "Starting" ),
    /* 2 */ SEnumEntry( CSigGenThread::EStateRunning,  "Running"  )
};

//------------------------------------------------------------------------------
QString CSigGenThread::State2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrStates,EStateCount,i_iState);
}

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrRequests[CSigGenThread::ERequestCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CSigGenThread::ERequestNone,     "None"     ),
    /* 1 */ SEnumEntry( CSigGenThread::ERequestStartup,  "Startup"  ),
    /* 2 */ SEnumEntry( CSigGenThread::ERequestShutdown, "Shutdown" )
};

//------------------------------------------------------------------------------
QString CSigGenThread::Request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrRequests,ERequestCount,i_iRequest);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CSigGenThread::CSigGenThread( const QString& i_strSigGenObjName ) :
//------------------------------------------------------------------------------
    QThread(),
    m_strSigGenObjName(i_strSigGenObjName),
    m_pDb(nullptr),
    m_state(EStateIdle),
    m_pReqInProgress(nullptr),
    m_pSigGen(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(m_strSigGenObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::RemCmdServer", "CSigGenThread", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "ctor",
        /* szAddInfo    */ "" );

} // ctor

//------------------------------------------------------------------------------
CSigGenThread::~CSigGenThread()
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
                /* errSource     */ "ZS::Apps::Test::RemCmdServer", "CSigGenThread", objectName(), "dtor",
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
        delete m_pSigGen;
    }
    catch(...)
    {
    }
    m_pSigGen = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pDb = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CSigGenThread::startup( CDatabase* i_pDb, qint64 i_iReqIdParent )
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
    if( m_state != EStateIdle || m_pReqInProgress != nullptr )
    {
        throw CException( __FILE__, __LINE__, EResultInternalStateMachineError );
    }

    m_pDb = i_pDb;

    m_pReqInProgress = new CRequest(
        /* strNameSpace */ "ZS::Apps::Test::RemCmdServer",
        /* strClassName */ "CSigGenThread",
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
void CSigGenThread::start()
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
void CSigGenThread::run()
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
            m_pSigGen = new CSigGen(
                /* pSigGenThread */ this,
                /* pDb           */ m_pDb,
                /* strObjName    */ m_strSigGenObjName );

            // Send confirmation message to event method of thread. The message
            // will be received in the context of the thread creating the thread
            // instance (the applications GUI main thread).
            CMsgConStartup* pMsgCon = new CMsgConStartup(
                /* pObjSender        */ m_pSigGen,
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
                delete m_pSigGen;
            }
            catch(...)
            {
            }
            m_pSigGen = nullptr;

        } // if( m_pReqInProgress->getRequest() == ERequestStartup )
    } // if( m_pReqInProgress != nullptr )

} // run

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CSigGenThread::event( QEvent* i_pEv )
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
class CSigGen
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CSigGen::CSigGen(
    CSigGenThread* i_pSigGenThread,
    CDatabase*     i_pDb,
    const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pSigGenThread(i_pSigGenThread),
    m_pDb(i_pDb),
    m_waveform(ESigGenWaveformSine),
    m_fYMin(-1.0),
    m_fYMax(1.0),
    m_fFreq_Hz(1.0),
    m_fStartTime_s( ZS::System::Time::getProcTimeInSec() ),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::RemCmdServer", "CSigGen", i_strObjName);

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

    m_pDb->registerSigGenClient(this);

    CMsgReqDbParGetVal* pMsgReq;

    pMsgReq = new CMsgReqDbParGetVal(
        /* pObjSender   */ this,
        /* pObjReceiver */ m_pDb,
        /* strDbPar     */ c_strDbParSigGenWaveform,
        /* iReqId       */ -1 );
    POST_OR_DELETE_MESSAGE(pMsgReq);
    pMsgReq = nullptr;

    pMsgReq = new CMsgReqDbParGetVal(
        /* pObjSender   */ this,
        /* pObjReceiver */ m_pDb,
        /* strDbPar     */ c_strDbParSigGenYMin,
        /* iReqId       */ -1 );
    POST_OR_DELETE_MESSAGE(pMsgReq);
    pMsgReq = nullptr;

    pMsgReq = new CMsgReqDbParGetVal(
        /* pObjSender   */ this,
        /* pObjReceiver */ m_pDb,
        /* strDbPar     */ c_strDbParSigGenYMax,
        /* iReqId       */ -1 );
    POST_OR_DELETE_MESSAGE(pMsgReq);
    pMsgReq = nullptr;

    pMsgReq = new CMsgReqDbParGetVal(
        /* pObjSender   */ this,
        /* pObjReceiver */ m_pDb,
        /* strDbPar     */ c_strDbParSigGenFreq,
        /* iReqId       */ -1 );
    POST_OR_DELETE_MESSAGE(pMsgReq);
    pMsgReq = nullptr;

} // ctor

//------------------------------------------------------------------------------
CSigGen::~CSigGen()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "dtor",
        /* szAddInfo    */ "" );

    if( m_pDb != nullptr )
    {
        m_pDb->unregisterSigGenClient(this);
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    m_pSigGenThread = nullptr;
    m_pDb = nullptr;
    m_waveform = static_cast<ESigGenWaveform>(0);
    m_fYMin = 0.0;
    m_fYMax = 0.0;
    m_fFreq_Hz = 0.0;
    m_fStartTime_s = 0.0;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CSigGen::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    switch( static_cast<int>(i_pEv->type()) )
    {
        case EMsgTypeReqDbParGetVal:
        {
            CMsgReqDbParGetVal* pMsgReq = dynamic_cast<CMsgReqDbParGetVal*>(i_pEv);

            if( pMsgReq != nullptr )
            {
                QVariant val;

                if( pMsgReq->getDbPar() == c_strDbParSigGenWaveform )
                {
                    val = static_cast<int>(m_waveform);
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigGenYMin )
                {
                    val = m_fYMin;
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigGenYMax )
                {
                    val = m_fYMax;
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigGenFreq )
                {
                    val = m_fFreq_Hz;
                }
                else if( pMsgReq->getDbPar() == c_strDbParSigAnaYVal )
                {
                    // f(t) = A * sin(2*PI*t*f) + D
                    //    A = yMax-yMin
                    //    D = (yMax-yMin)/2
                    //    t = Time/s
                    //    f = Freq/Hz = 1/T
                    //    T = Period/s

                    double fA   = fabs(m_fYMax-m_fYMin);
                    double fD   = m_fYMin + (m_fYMax-m_fYMin)/2.0;
                    double ft_s = ZS::System::Time::getProcTimeInSec() - m_fStartTime_s;
                    double fY   = fA * sin( 2.0 * ZS::System::Math::c_fPI * m_fFreq_Hz * ft_s ) + fD;

                    val = fY;
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
        } // case EMsgTypeReqDbParGetVal

        case EMsgTypeConDbParGetVal:
        {
            CMsgConDbParGetVal* pMsgCon = dynamic_cast<CMsgConDbParGetVal*>(i_pEv);

            if( pMsgCon != nullptr )
            {
                if( pMsgCon->getDbPar() == c_strDbParSigGenWaveform )
                {
                    m_waveform = static_cast<ESigGenWaveform>(pMsgCon->getVal().toInt());
                }
                else if( pMsgCon->getDbPar() == c_strDbParSigGenYMin )
                {
                    m_fYMin = pMsgCon->getVal().toDouble();
                }
                else if( pMsgCon->getDbPar() == c_strDbParSigGenYMax )
                {
                    m_fYMax = pMsgCon->getVal().toDouble();
                }
                else if( pMsgCon->getDbPar() == c_strDbParSigGenFreq )
                {
                    m_fFreq_Hz = pMsgCon->getVal().toDouble();
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
                if( pMsgInd->getDbPar() == c_strDbParSigGenWaveform )
                {
                    m_waveform = static_cast<ESigGenWaveform>(pMsgInd->getVal().toInt());
                }
                else if( pMsgInd->getDbPar() == c_strDbParSigGenYMin )
                {
                    m_fYMin = pMsgInd->getVal().toDouble();
                }
                else if( pMsgInd->getDbPar() == c_strDbParSigGenYMax )
                {
                    m_fYMax = pMsgInd->getVal().toDouble();
                }
                else if( pMsgInd->getDbPar() == c_strDbParSigGenFreq )
                {
                    m_fFreq_Hz = pMsgInd->getVal().toDouble();
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
void CSigGen::onDbDestroyed( QObject* /*i_pDb*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* szMethod     */ "onDbDestroyed",
        /* szAddInfo    */ "" );

    m_pDb = nullptr;

} // onDbDestroyed
