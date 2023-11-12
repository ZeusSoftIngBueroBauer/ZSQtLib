/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qmutex.h>
#include <QtCore/qthread.h>

#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
namespace ZS
*******************************************************************************/

/* enum ESystemMsgType
==============================================================================*/

static const SEnumEntry s_arEnumStrSystemMsgType[] = {
                                                           // IdxName,     Symbol, Text
    /* 0 */ SEnumEntry( MsgProtocol::ESystemMsgTypeUndefined, "Undefined", "?",    "Undefined"    ),
    /* 1 */ SEnumEntry( MsgProtocol::ESystemMsgTypeReq,       "Req",       "R",    "Request"      ),
    /* 2 */ SEnumEntry( MsgProtocol::ESystemMsgTypeCon,       "Con",       "C",    "Confirmation" ),
    /* 3 */ SEnumEntry( MsgProtocol::ESystemMsgTypeInd,       "Ind",       "I",    "Indication"   ),
    /* 4 */ SEnumEntry( MsgProtocol::ESystemMsgTypeAck,       "Ack",       "A",    "Acknowledge"  )
};

//------------------------------------------------------------------------------
QString ZS::System::systemMsgType2Str( int i_iMsgType, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrSystemMsgType, _ZSArrLen(s_arEnumStrSystemMsgType), i_iMsgType, i_alias);
}

//------------------------------------------------------------------------------
MsgProtocol::TSystemMsgType ZS::System::str2SystemMsgType( const QString& i_str, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    MsgProtocol::TSystemMsgType systemMsgType = MsgProtocol::ESystemMsgTypeUndefined;
    int iSystemMsgType = SEnumEntry::str2Enumerator(s_arEnumStrSystemMsgType, _ZSArrLen(s_arEnumStrSystemMsgType), i_str, i_alias, Qt::CaseInsensitive);
    if( iSystemMsgType >= 0 )
    {
        systemMsgType = static_cast<MsgProtocol::TSystemMsgType>(iSystemMsgType);
    }
    return systemMsgType;
}

/* enum EBaseMsgType
==============================================================================*/

static const SEnumEntry s_arEnumStrBaseMsgType[EBaseMsgTypeCount] = {
                                                                    // IdxName,          Symbol,           Text
    /* 1001 */ SEnumEntry( EBaseMsgTypeUndefined-EBaseMsgTypeMin,      "Undefined",      "?",              "Undefined"                 ),
    /* 1002 */ SEnumEntry( EBaseMsgTypeReq-EBaseMsgTypeMin,            "Req",            "R",              "Request"                   ),
    /* 1003 */ SEnumEntry( EBaseMsgTypeCon-EBaseMsgTypeMin,            "Con",            "C",              "Confirmation"              ),
    /* 1004 */ SEnumEntry( EBaseMsgTypeInd-EBaseMsgTypeMin,            "Ind",            "I",              "Indication"                ),
    /* 1005 */ SEnumEntry( EBaseMsgTypeAck-EBaseMsgTypeMin,            "Ack",            "A",              "Acknowledge"               ),
    /* 1006 */ SEnumEntry( EBaseMsgTypeIndErr-EBaseMsgTypeMin,         "IndErr",         "IndErr",         "Error Indication"          ),
    /* 1007 */ SEnumEntry( EBaseMsgTypeReqAddLogItem-EBaseMsgTypeMin,  "ReqAddLogItem",  "ReqAddLogItem",  "Add Log Item Request"      ),
    /* 1008 */ SEnumEntry( EBaseMsgTypeReqStartTimer-EBaseMsgTypeMin,  "ReqStartTimer",  "ReqStartTimer",  "Start Timer Request"       ),
    /* 1009 */ SEnumEntry( EBaseMsgTypeConStartTimer-EBaseMsgTypeMin,  "ConStartTimer",  "ConStartTimer",  "Start Timer Confirmation"  ),
    /* 1010 */ SEnumEntry( EBaseMsgTypeReqStopTimer-EBaseMsgTypeMin,   "ReqStopTimer",   "ReqStopTimer",   "Stop Timer Request"        ),
    /* 1011 */ SEnumEntry( EBaseMsgTypeConStopTimer-EBaseMsgTypeMin,   "ConStopTimer",   "ConStopTimer",   "Stop Timer Confirmation"   ),
    /* 1012 */ SEnumEntry( EBaseMsgTypeReqContinue-EBaseMsgTypeMin,    "ReqContinue",    "ReqContinue",    "Continue Request"          ),
    /* 1013 */ SEnumEntry( EBaseMsgTypeReqAbort-EBaseMsgTypeMin,       "ReqAbort",       "ReqAbort",       "Abort Request"             ),
    /* 1014 */ SEnumEntry( EBaseMsgTypeReqPoll-EBaseMsgTypeMin,        "ReqPoll",        "ReqPoll",        "Poll Request"              ),
    /* 1015 */ SEnumEntry( EBaseMsgTypeConStartThread-EBaseMsgTypeMin, "ConStartThread", "ConStartThread", "Start Thread Confirmation" )
};

//------------------------------------------------------------------------------
QString ZS::System::baseMsgType2Str( int i_iMsgType, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    QString strMsgType = QString::number(i_iMsgType);
    int     iMsgType   = i_iMsgType;

    if( iMsgType >= EBaseMsgTypeMin && iMsgType <= EBaseMsgTypeMax )
    {
        iMsgType -= EBaseMsgTypeMin;
        strMsgType = SEnumEntry::enumerator2Str(s_arEnumStrBaseMsgType, _ZSArrLen(s_arEnumStrBaseMsgType), iMsgType, i_alias);
    }
    return strMsgType;

} // baseMsgType2Str

//------------------------------------------------------------------------------
EBaseMsgType ZS::System::str2BaseMsgType( const QString& i_str, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    EBaseMsgType baseMsgType = EBaseMsgTypeUndefined;
    int iBaseMsgType = SEnumEntry::str2Enumerator(s_arEnumStrBaseMsgType, _ZSArrLen(s_arEnumStrBaseMsgType), i_str, i_alias, Qt::CaseInsensitive);
    if( iBaseMsgType >= EBaseMsgTypeMin && iBaseMsgType <= EBaseMsgTypeMax )
    {
        baseMsgType = static_cast<EBaseMsgType>(iBaseMsgType);
    }
    return baseMsgType;
}

/* enum ECommand
==============================================================================*/

static const SEnumEntry s_arEnumStrCommand[] = {
                                                     // IdxName,     Symbol, Text
    /* 0 */ SEnumEntry( MsgProtocol::ECommandUndefined, "Undefined", "?",    "Undefined" ),
    /* 1 */ SEnumEntry( MsgProtocol::ECommandSelect,    "Sel",       "S",    "Select"    ),
    /* 2 */ SEnumEntry( MsgProtocol::ECommandUpdate,    "Upd",       "U",    "Update"    ),
    /* 3 */ SEnumEntry( MsgProtocol::ECommandInsert,    "Ins",       "I",    "Insert"    ),
    /* 4 */ SEnumEntry( MsgProtocol::ECommandDelete,    "Del",       "D",    "Delete"    )
};

//------------------------------------------------------------------------------
QString ZS::System::command2Str( int i_iCmd, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrCommand, _ZSArrLen(s_arEnumStrCommand), i_iCmd, i_alias);
}

//------------------------------------------------------------------------------
MsgProtocol::TCommand ZS::System::str2Command( const QString& i_str, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    MsgProtocol::TCommand cmd = MsgProtocol::ECommandUndefined;
    int iCmd = SEnumEntry::str2Enumerator(s_arEnumStrCommand, _ZSArrLen(s_arEnumStrCommand), i_str, i_alias, Qt::CaseInsensitive);
    if( iCmd >= 0 )
    {
        cmd = static_cast<MsgProtocol::TCommand>(iCmd);
    }
    return cmd;
}


/*******************************************************************************
Global Methods
*******************************************************************************/

//------------------------------------------------------------------------------
void ZS::System::POST_OR_DELETE_MESSAGE(
    QEvent*                      i_pMsg,
    CMethodTracer*               i_pMethodTracer,
    ELogDetailLevel i_eFilterDetailLevel )
//------------------------------------------------------------------------------
{
    if( i_pMsg != nullptr )
    {
        CMsg* pMsg = dynamic_cast<CMsg*>(i_pMsg);

        QObject* pObjReceiver = nullptr;

        ELogDetailLevel eTrcDetailLevel = ELogDetailLevel::None;

        if( pMsg != nullptr )
        {
            pObjReceiver = pMsg->getReceiver();

            QString strMsgTrcInfo = "{" + pMsg->getAddTrcInfoStr(eTrcDetailLevel >= ELogDetailLevel::DebugDetailed ? 1 : 0) + "}";

            if( pObjReceiver != nullptr )
            {
                if( i_pMethodTracer != nullptr && i_pMethodTracer->isRuntimeInfoActive(i_eFilterDetailLevel) )
                {
                    QString strAddTrcInfo = "postEvent( " + strMsgTrcInfo + " )";
                    i_pMethodTracer->trace(strAddTrcInfo, i_eFilterDetailLevel);
                }
                QCoreApplication::postEvent(pObjReceiver, i_pMsg);
            }
            else // if( pObjReceiver == nullptr )
            {
                if( i_pMethodTracer != nullptr && i_pMethodTracer->isRuntimeInfoActive(i_eFilterDetailLevel) )
                {
                    QString strAddTrcInfo = "deleteEvent( " + strMsgTrcInfo + " )";
                    i_pMethodTracer->trace(strAddTrcInfo, i_eFilterDetailLevel);
                }
                delete i_pMsg;
                i_pMsg = nullptr;
            }
        } // if( pMsg != nullptr )
    } // if( i_pMsg != nullptr )
} // POST_OR_DELETE_MESSAGE


/*******************************************************************************
class CMsgSndRcvDestroyedHandler : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgSndRcvDestroyedHandler::CMsgSndRcvDestroyedHandler( CMsg* i_pMsg ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMsg(i_pMsg),
    m_pObjSender(i_pMsg->getSender()),
    m_pObjReceiver(i_pMsg->getReceiver())
{
    if( m_pObjSender != nullptr )
    {
        QObject::connect(
            m_pObjSender, &QObject::destroyed,
            this, &CMsgSndRcvDestroyedHandler::onSenderDestroyed,
            Qt::DirectConnection);
    }

    if( m_pObjReceiver != nullptr )
    {
        QObject::connect(
            m_pObjReceiver, &QObject::destroyed,
            this, &CMsgSndRcvDestroyedHandler::onReceiverDestroyed,
            Qt::DirectConnection);
    }

} // ctor

//------------------------------------------------------------------------------
CMsgSndRcvDestroyedHandler::~CMsgSndRcvDestroyedHandler()
//------------------------------------------------------------------------------
{
    m_pMsg = nullptr;
    m_pObjSender = nullptr;
    m_pObjReceiver = nullptr;

} // dtor

/*==============================================================================
public: // instance methods (called by CMsg)
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgSndRcvDestroyedHandler::onMsgSenderChanged()
//------------------------------------------------------------------------------
{
    if( m_pObjSender != nullptr )
    {
        QObject::disconnect(
            m_pObjSender, &QObject::destroyed,
            this, &CMsgSndRcvDestroyedHandler::onSenderDestroyed);
    }

    m_pObjSender = m_pMsg->getSender();

    if( m_pObjSender != nullptr )
    {
        QObject::connect(
            m_pObjSender, &QObject::destroyed,
            this, &CMsgSndRcvDestroyedHandler::onSenderDestroyed,
            Qt::DirectConnection);
    }
} // onMsgSenderChanged

//------------------------------------------------------------------------------
void CMsgSndRcvDestroyedHandler::onMsgReceiverChanged()
//------------------------------------------------------------------------------
{
    if( m_pObjReceiver != nullptr )
    {
        QObject::disconnect(
            m_pObjReceiver, &QObject::destroyed,
            this, &CMsgSndRcvDestroyedHandler::onReceiverDestroyed);
    }

    m_pObjReceiver = m_pMsg->getReceiver();

    if( m_pObjReceiver != nullptr )
    {
        QObject::connect(
            m_pObjReceiver, &QObject::destroyed,
            this, &CMsgSndRcvDestroyedHandler::onReceiverDestroyed,
            Qt::DirectConnection);
    }
} // onMsgReceiverChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgSndRcvDestroyedHandler::onSenderDestroyed( QObject* /*i_pObj*/ )
//------------------------------------------------------------------------------
{
    m_pObjSender = nullptr;
    m_pMsg->setSender(nullptr);

} // onSenderDestroyed

//------------------------------------------------------------------------------
void CMsgSndRcvDestroyedHandler::onReceiverDestroyed( QObject* /*i_pObj*/ )
//------------------------------------------------------------------------------
{
    m_pObjReceiver = nullptr;
    m_pMsg->setReceiver(nullptr);

} // onReceiverDestroyed


/*******************************************************************************
class CMsg : public QEvent
*******************************************************************************/

/*==============================================================================
private: // class members
==============================================================================*/

static qint64 s_iMsgIdCounter = 0;
static QRecursiveMutex s_mtxId;

static qint64                s_iMsgObjCtorCounter = 0;
static qint64                s_iMsgObjDtorCounter = 0;
static qint64                s_iMsgObjLiveCounter = 0;
static QHash<qint64,int>     s_hshMsgIdsLiveCounter;
static QHash<qint64,QString> s_hshMsgIdsLiveTrcInfo;
static QRecursiveMutex       s_mtxMsgIdsLiveCounter;

static bool s_bTracing = false;
static bool s_bTracingEnabled = false;

static CTrcAdminObj* s_pTrcAdminObj = nullptr;


/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
void ZS::System::SYS_MESSAGES_SET_TRACING_ENABLED( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    s_bTracingEnabled = i_bEnabled;
}

//------------------------------------------------------------------------------
void ZS::System::TRACE_LIVING_MESSAGE_OBJECTS( CMethodTracer* i_pMethodTracer )
//------------------------------------------------------------------------------
{
    if( i_pMethodTracer != nullptr )
    {
        // Use tryLock with timeout here as trying to lock the mutex may deadlock
        // a system shutdown if a thread with an event loop cannot be terminated.
        bool bMtxLocked = s_mtxMsgIdsLiveCounter.tryLock(1000); // milliseconds

        if( !s_hshMsgIdsLiveTrcInfo.isEmpty() )
        {
            QString strAddTrcInfo = "List of message objects which are still alive [";

            QHash<qint64,QString>::iterator it;

            for( it = s_hshMsgIdsLiveTrcInfo.begin(); it != s_hshMsgIdsLiveTrcInfo.end(); it++ )
            {
                if( it != s_hshMsgIdsLiveTrcInfo.begin() )
                {
                    strAddTrcInfo += ", ";
                }
                strAddTrcInfo += "{" + QString::number(it.key()) + ":" + it.value() + "}";
            }
            strAddTrcInfo += "]";
            i_pMethodTracer->trace(strAddTrcInfo, ELogDetailLevel::Debug);

        } // if( !s_hshMsgIdsLiveTrcInfo.isEmpty() )

        if( bMtxLocked )
        {
            s_mtxMsgIdsLiveCounter.unlock();
        }
    } // if( i_pMethodTracer != nullptr )

} // TRACE_LIVING_MESSAGE_OBJECTS

/*==============================================================================
protected: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
qint64 CMsg::GetUniqueMsgId()
//------------------------------------------------------------------------------
{
    s_mtxId.lock();

    s_iMsgIdCounter++;

    if( s_iMsgIdCounter < 0 )
    {
        s_iMsgIdCounter = 0;
    }
    s_mtxId.unlock();

    return s_iMsgIdCounter;

} // GetUniqueMsgId

/*==============================================================================
protected: // ctors (pure virtual base class)
==============================================================================*/

//------------------------------------------------------------------------------
CMsg::CMsg(
    MsgProtocol::TSystemMsgType i_systemMsgType,
    int                         i_iMsgType,
    QObject*                    i_pObjSender,
    QObject*                    i_pObjReceiver,
    qint64                      i_iMsgId ) :
//------------------------------------------------------------------------------
    QEvent(static_cast<Type>(i_iMsgType)),
    m_systemMsgType(i_systemMsgType),
    m_bIsBaseMsgType(false),
    m_pObjSender(i_pObjSender),
    m_iSenderId(-1),
    m_pObjReceiver(i_pObjReceiver),
    m_iReceiverId(-1),
    m_iMsgId(i_iMsgId),
    m_strObjName(),
    m_pSndRcvDestroyedHandler(nullptr)
{
    // Use tryLock with timeout here as trying to lock the mutex may deadlock
    // a system shutdown if a thread with an event loop cannot be terminated.
    bool bMtxLocked = s_mtxMsgIdsLiveCounter.tryLock(1000); // milliseconds

    s_iMsgObjCtorCounter++;
    s_iMsgObjLiveCounter++;

    if( i_iMsgId == -1 )
    {
        m_iMsgId = GetUniqueMsgId();
    }

    m_strObjName = systemMsgType2Str(m_systemMsgType) + QString::number(m_iMsgId);

    setObjectName(m_strObjName);

    QString strAddTrcInfo;

    if( s_bTracingEnabled && s_pTrcAdminObj == nullptr )
    {
        s_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), "");
    }

    // If a method trace client is connected tracing will create an instance of CMsgReqSendData.
    // The ctor of this class again produces trace outputs whereupon again an instance of CMsgReqSendData
    // is created which again produces trace outputs whereupon again an instance of CMsgReqSendDAta
    // is created which again ....
    // To break this endless message creation recursion the static flag "inMsgCtor" is used.
    if( !s_bTracing && s_pTrcAdminObj != nullptr && s_pTrcAdminObj->isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        s_bTracing = true;

        strAddTrcInfo  = "Addr: " + pointer2Str(static_cast<QEvent*>(this));
        strAddTrcInfo += ", Content { MsgTypeId: " + QString::number(i_iMsgType) + "}";

        CTrcServer::GetInstance()->traceMethodEnter(
            /* pAdminObj     */ s_pTrcAdminObj,
            /* strObjName    */ m_strObjName,
            /* strMethod     */ "ctor",
            /* strMethodArgs */ strAddTrcInfo );

        s_bTracing = false;
    }

    if( s_hshMsgIdsLiveCounter.contains(m_iMsgId) )
    {
        s_hshMsgIdsLiveCounter[m_iMsgId]++;
    }
    else
    {
        s_hshMsgIdsLiveCounter[m_iMsgId] = 1;
    }

    strAddTrcInfo  = "MsgType: " + QString::number(i_iMsgType);
    strAddTrcInfo += ", MsgId: " + QString::number(m_iMsgId);
    strAddTrcInfo += ", Rcv: " + QString( i_pObjReceiver == nullptr ? "nullptr" : i_pObjReceiver->objectName() );
    strAddTrcInfo += ", Snd: " + QString( i_pObjSender == nullptr ? "nullptr" : i_pObjSender->objectName() );

    s_hshMsgIdsLiveTrcInfo[m_iMsgId] = strAddTrcInfo;

    // The ctor is also called if cloning an existing message for which
    // the sender and/or receiver may have already been destroyed.
    m_pSndRcvDestroyedHandler = new CMsgSndRcvDestroyedHandler(this);

    if( !s_bTracing && s_pTrcAdminObj != nullptr && s_pTrcAdminObj->isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        s_bTracing = true;

        CTrcServer::GetInstance()->traceMethodLeave(
            /* pAdminObj        */ s_pTrcAdminObj,
            /* strObjName       */ m_strObjName,
            /* strMethod        */ "ctor",
            /* strMethodReturn  */ "",
            /* strMethodOutArgs */ "" );

        s_bTracing = false;
    }

    if( bMtxLocked )
    {
        s_mtxMsgIdsLiveCounter.unlock();
    }

} // ctor

//------------------------------------------------------------------------------
CMsg::CMsg( const MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    //QObject(),
    QEvent(static_cast<QEvent::Type>(i_pMsgHdr->m_msgType)),
    m_systemMsgType(i_pMsgHdr->m_systemMsgType),
    m_bIsBaseMsgType(false),
    m_pObjSender(nullptr),
    m_iSenderId(i_pMsgHdr->m_idSender),
    m_pObjReceiver(nullptr),
    m_iReceiverId(i_pMsgHdr->m_idReceiver),
    m_iMsgId(i_pMsgHdr->m_idMsg),
    m_strObjName(),
    m_pSndRcvDestroyedHandler(nullptr)
{
    // Use tryLock with timeout here as trying to lock the mutex may deadlock
    // a system shutdown if a thread with an event loop cannot be terminated.
    bool bMtxLocked = s_mtxMsgIdsLiveCounter.tryLock(1000); // milliseconds

    s_iMsgObjCtorCounter++;
    s_iMsgObjLiveCounter++;

    m_strObjName = systemMsgType2Str(m_systemMsgType) + QString::number(m_iMsgId);

    setObjectName(m_strObjName);

    QString strAddTrcInfo;

    if( s_bTracingEnabled && s_pTrcAdminObj == nullptr )
    {
        s_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), "");
    }

    // If a method trace client is connected tracing will create an instance of CMsgReqSendData.
    // The ctor of this class again produces trace outputs whereupon again an instance of CMsgReqSendData
    // is created which again produces trace outputs whereupon again an instance of CMsgReqSendDAta
    // is created which again ....
    // To break this endless message creation recursion the static flag "tracing" is used.
    // But messages may be created by different threads and therefore this flag would have to be
    // protected by a mutex slowing down the application.
    if( !s_bTracing && s_pTrcAdminObj != nullptr && s_pTrcAdminObj->isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        s_bTracing = true;

        strAddTrcInfo  = "Addr: " + pointer2Str(static_cast<QEvent*>(this));
        strAddTrcInfo += ", Content { MsgTypeId: " + QString::number(i_pMsgHdr->m_msgType) + "}";

        CTrcServer::GetInstance()->traceMethodEnter(
            /* pAdminObj     */ s_pTrcAdminObj,
            /* strObjName    */ m_strObjName,
            /* strMethod     */ "ctor",
            /* strMethodArgs */ strAddTrcInfo );

        s_bTracing = false;
    }

    if( s_hshMsgIdsLiveCounter.contains(m_iMsgId) )
    {
        s_hshMsgIdsLiveCounter[m_iMsgId]++;
    }
    else
    {
        s_hshMsgIdsLiveCounter[m_iMsgId] = 1;
    }

    strAddTrcInfo  = "MsgType: " + QString::number(i_pMsgHdr->m_msgType);
    strAddTrcInfo += ", MsgId: " + QString::number(i_pMsgHdr->m_idMsg);
    strAddTrcInfo += ", RcvId: " + QString::number(i_pMsgHdr->m_idReceiver);
    strAddTrcInfo += ", SndId: " + QString::number(i_pMsgHdr->m_idSender);

    s_hshMsgIdsLiveTrcInfo[m_iMsgId] = strAddTrcInfo;

    // The ctor is also called if cloning an existing message for which
    // the sender and/or receiver may have already been destroyed.
    m_pSndRcvDestroyedHandler = new CMsgSndRcvDestroyedHandler(this);

    if( !s_bTracing && s_pTrcAdminObj != nullptr && s_pTrcAdminObj->isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        s_bTracing = true;

        CTrcServer::GetInstance()->traceMethodLeave(
            /* pAdminObj        */ s_pTrcAdminObj,
            /* strObjName       */ m_strObjName,
            /* strMethod        */ "ctor",
            /* strMethodReturn  */ "",
            /* strMethodOutArgs */ "" );

        s_bTracing = false;
    }

    if( bMtxLocked )
    {
        s_mtxMsgIdsLiveCounter.unlock();
    }

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsg::~CMsg()
//------------------------------------------------------------------------------
{
    // Use tryLock with timeout here as trying to lock the mutex may deadlock
    // a system shutdown if a thread with an event loop cannot be terminated.
    bool bMtxLocked = s_mtxMsgIdsLiveCounter.tryLock(1000); // milliseconds

    if( !s_bTracing && s_pTrcAdminObj != nullptr && s_pTrcAdminObj->isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        s_bTracing = true;

        QString strAddTrcInfo = "Addr: " + pointer2Str(static_cast<QEvent*>(this));

        CTrcServer::GetInstance()->traceMethodEnter(
            /* pAdminObj     */ s_pTrcAdminObj,
            /* strObjName    */ m_strObjName,
            /* strMethod     */ "dtor",
            /* strMethodArgs */ strAddTrcInfo );

        s_bTracing = false;
    }

    // The ctor is also called if cloning an existing message for which
    // the sender and/or receiver may have already been destroyed.
    try
    {
        delete m_pSndRcvDestroyedHandler;
    }
    catch(...)
    {
    }
    m_pSndRcvDestroyedHandler = nullptr;

    s_iMsgObjDtorCounter++;

    s_hshMsgIdsLiveCounter[m_iMsgId]--;

    if( s_hshMsgIdsLiveCounter[m_iMsgId] == 0 )
    {
        s_hshMsgIdsLiveCounter.remove(m_iMsgId);
        s_hshMsgIdsLiveTrcInfo.remove(m_iMsgId);
    }

    if( s_iMsgObjLiveCounter == 0 )
    {
        QString strAddErrInfo = "More message objects have been destroyed than have been created.";
        SErrResultInfo errResultInfo = ErrResultInfoCritical("dtor", EResultInternalProgramError, strAddErrInfo);

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else
    {
        s_iMsgObjLiveCounter--;
    }

    if( !s_bTracing && s_pTrcAdminObj != nullptr && s_pTrcAdminObj->isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        s_bTracing = true;

        CTrcServer::GetInstance()->traceMethodLeave(
            /* pAdminObj        */ s_pTrcAdminObj,
            /* strObjName       */ m_strObjName,
            /* strMethod        */ "dtor",
            /* strMethodReturn  */ "",
            /* strMethodOutArgs */ "" );

        s_bTracing = false;
    }

    if( s_iMsgObjLiveCounter == 0 && s_pTrcAdminObj != nullptr )
    {
        CTrcServer::ReleaseTraceAdminObj(s_pTrcAdminObj);
        s_pTrcAdminObj = nullptr;
    }

    if( bMtxLocked )
    {
        s_mtxMsgIdsLiveCounter.unlock();
    }

    m_systemMsgType = static_cast<MsgProtocol::TSystemMsgType>(0);
    m_bIsBaseMsgType = false;
    m_pObjSender = nullptr;
    m_iSenderId = 0;
    m_pObjReceiver = nullptr;
    m_iReceiverId = 0;
    m_iMsgId = 0;
    //m_strObjName;
    m_pSndRcvDestroyedHandler = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsg::setSender( QObject* i_pObjSender )
//------------------------------------------------------------------------------
{
    m_pObjSender = i_pObjSender;

    m_pSndRcvDestroyedHandler->onMsgSenderChanged();

} // setSender

//------------------------------------------------------------------------------
QString CMsg::getSenderName() const
//------------------------------------------------------------------------------
{
    QString strName;

    if( m_pObjSender != nullptr )
    {
        strName = m_pObjSender->objectName();
    }
    return strName;

} // getSenderName

//------------------------------------------------------------------------------
void CMsg::setSenderId( qint64 i_iSenderId )
//------------------------------------------------------------------------------
{
    m_iSenderId = i_iSenderId;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsg::setReceiver( QObject* i_pObjReceiver )
//------------------------------------------------------------------------------
{
    m_pObjReceiver = i_pObjReceiver;

    m_pSndRcvDestroyedHandler->onMsgReceiverChanged();

} // setReceiver

//------------------------------------------------------------------------------
QString CMsg::getReceiverName() const
//------------------------------------------------------------------------------
{
    QString strName;

    if( m_pObjReceiver != nullptr )
    {
        strName = m_pObjReceiver->objectName();
    }
    return strName;

} // getReceiverName

//------------------------------------------------------------------------------
void CMsg::setReceiverId( qint64 i_iReceiverId )
//------------------------------------------------------------------------------
{
    m_iReceiverId = i_iReceiverId;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsg::setMsgId( qint64 i_iMsgId )
//------------------------------------------------------------------------------
{
    m_iMsgId = i_iMsgId;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsg::setObjectName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    m_strObjName = i_strObjName;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsg::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type());
}

//------------------------------------------------------------------------------
QString CMsg::getAddTrcInfoStr( int /*i_iDetailLevel*/, EContentToStrFormat /*i_format*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = "Id: " + QString::number(m_iMsgId);

    strAddTrcInfo += ", MsgTypeId: " + msgTypeToStr();

    if( m_pObjSender == nullptr )
    {
        strAddTrcInfo += ", Snd: nullptr";
    }
    else
    {
        strAddTrcInfo += ", Snd: " + m_pObjSender->objectName();
    }
    if( m_pObjReceiver == nullptr )
    {
        strAddTrcInfo += ", Rcv: nullptr";
    }
    else
    {
        strAddTrcInfo += ", Rcv: " + m_pObjReceiver->objectName();
    }
    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsg::serialize() const
//------------------------------------------------------------------------------
{
    QString strSnd = getSenderName();
    QString strRcv = getReceiverName();

    int iMsgHeaderSize = sizeof(MsgProtocol::SMsgHeader);

    int iCharSize = sizeof(QChar);

    MsgProtocol::TBlockLen buffLen = iCharSize * (strSnd.length() + strRcv.length());

    qint16 buffIdxSenderName = -1;
    qint16 buffIdxReceiverName = -1;

    if( !strSnd.isEmpty() )
    {
        buffIdxSenderName = 0;
    }
    if( !strRcv.isEmpty() )
    {
        buffIdxReceiverName = strSnd.length() * iCharSize;
    }

    QByteArray byteArr( iMsgHeaderSize + buffLen, 0x00 );

    STimeStamp tm;

    MsgProtocol::SMsgHeader* pMsgHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    pMsgHdr->m_protocolVersion     = MsgProtocol::VERSION;
    pMsgHdr->m_timeStampDate       = tm.toTimeStampDate();
    pMsgHdr->m_timeStampTime       = tm.toTimeStampTime();
    pMsgHdr->m_systemMsgType       = m_systemMsgType;
    pMsgHdr->m_msgType             = type();
    pMsgHdr->m_idSender            = -1;
    pMsgHdr->m_buffIdxSenderName   = buffIdxSenderName;
    pMsgHdr->m_idReceiver          = -1;
    pMsgHdr->m_buffIdxReceiverName = buffIdxReceiverName;
    pMsgHdr->m_idMsg               = m_iMsgId;
    pMsgHdr->m_idRequest           = -1;
    pMsgHdr->m_flag                = 0;
    pMsgHdr->m_resultSeverity      = EResultSeveritySuccess;
    pMsgHdr->m_result              = EResultSuccess;
    pMsgHdr->m_iProgress_perCent   = 0;
    pMsgHdr->m_iFillByte           = 0;
    pMsgHdr->m_blockLenBuffer      = buffLen;
    pMsgHdr->m_blockLenData        = 0;

    char* pcBuff = reinterpret_cast<char*>(pMsgHdr) + iMsgHeaderSize;

    if( !strSnd.isEmpty() )
    {
        memcpy( pcBuff + buffIdxSenderName, strSnd.data(), iCharSize * strSnd.length() );
    }
    if( !strRcv.isEmpty() )
    {
        memcpy( pcBuff + buffIdxReceiverName, strRcv.data(), iCharSize * strRcv.length() );
    }

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgReq : public CMsg
*******************************************************************************/

/*==============================================================================
protected: // ctors (pure virtual base class)
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReq::CMsgReq(
    int      i_iMsgType,
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsg(
        /* systemMsgType */ MsgProtocol::ESystemMsgTypeReq,
        /* iMsgType      */ i_iMsgType,
        /* pObjSender    */ i_pObjSender,
        /* pObjReceiver  */ i_pObjReceiver,
        /* iMsgId        */ i_iMsgId ),
    m_bMustBeConfirmed(i_bMustBeConfirmed),
    m_iTimeout_ms(0),
    m_bIsBlockingRequest(false),
    m_iReqId(i_iReqId)
{
} // ctor

//------------------------------------------------------------------------------
CMsgReq::CMsgReq( const MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsg(i_pMsgHdr),
    m_bMustBeConfirmed(i_pMsgHdr->m_flag == MsgProtocol::EFlagAck),
    m_iTimeout_ms(0),
    m_bIsBlockingRequest(false),
    m_iReqId(i_pMsgHdr->m_idRequest)
{
} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReq::~CMsgReq()
//------------------------------------------------------------------------------
{
    m_bMustBeConfirmed = false;
    m_iTimeout_ms = 0;
    m_bIsBlockingRequest = false;
    m_iReqId = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgReq::setMustBeConfirmed( bool i_bMustBeConfirmed )
//------------------------------------------------------------------------------
{
    m_bMustBeConfirmed = i_bMustBeConfirmed;
}

//------------------------------------------------------------------------------
void CMsgReq::setTimeoutInMs( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    m_iTimeout_ms = i_iTimeout_ms;
}

//------------------------------------------------------------------------------
void CMsgReq::setIsBlockingRequest( bool i_bIsBlocking )
//------------------------------------------------------------------------------
{
    m_bIsBlockingRequest = i_bIsBlocking;
}

//------------------------------------------------------------------------------
void CMsgReq::setRequestId( qint64 i_iId )
//------------------------------------------------------------------------------
{
    m_iReqId = i_iId;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReq::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsg::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", Conf:" + bool2Str(m_bMustBeConfirmed);
    strAddTrcInfo += ", TimeoutMs:" + QString::number(m_iTimeout_ms);
    strAddTrcInfo += ", IsBlocking:" + bool2Str(m_bIsBlockingRequest);
    strAddTrcInfo += ", ReqId:" + QString::number(m_iReqId);

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgReq::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsg::serialize();

    MsgProtocol::SMsgHeader* pMsgHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    pMsgHdr->m_idRequest = m_iReqId;
    pMsgHdr->m_flag      = m_bMustBeConfirmed;

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgCon : public CMsg
*******************************************************************************/

/*==============================================================================
protected: // ctors (pure virtual base class)
==============================================================================*/

//------------------------------------------------------------------------------
CMsgCon::CMsgCon(
    int                   i_iMsgType,
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsg(
        /* systemMsgType */ MsgProtocol::ESystemMsgTypeCon,
        /* iMsgType      */ i_iMsgType,
        /* pObjSender    */ i_pObjSender,
        /* pObjReceiver  */ i_pObjReceiver,
        /* iMsgId        */ i_iMsgIdReq ),
    m_iReqId(i_iReqId),
    m_errResultInfo(i_errResultInfo),
    m_iProgressInPerCent(i_iProgressInPerCent)
{
} // ctor

//------------------------------------------------------------------------------
CMsgCon::CMsgCon( const MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsg(i_pMsgHdr),
    m_iReqId(i_pMsgHdr->m_idRequest),
    m_errResultInfo(
        /* errSource */ nameSpace(), className(), objectName(), "ctor",
        /* result    */ static_cast<ZS::System::EResult>(i_pMsgHdr->m_result),
        /* severity  */ static_cast<ZS::System::EResultSeverity>(i_pMsgHdr->m_resultSeverity) ),
    m_iProgressInPerCent(i_pMsgHdr->m_iProgress_perCent)
{
} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgCon::~CMsgCon()
//------------------------------------------------------------------------------
{
    m_iReqId = 0;
    //m_errResultInfo;
    m_iProgressInPerCent = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgCon::setRequestId( qint64 i_iId )
//------------------------------------------------------------------------------
{
    m_iReqId = i_iId;
}

//------------------------------------------------------------------------------
void CMsgCon::setErrResultInfo( const SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    m_errResultInfo = i_errResultInfo;
}

//------------------------------------------------------------------------------
void CMsgCon::setErrResult( const SErrResult& i_errResult )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setErrResult(i_errResult);
}

//------------------------------------------------------------------------------
void CMsgCon::setSeverity( EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setSeverity(i_severity);
}

//------------------------------------------------------------------------------
void CMsgCon::setResult( EResult i_result )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setResult(i_result);
}

//------------------------------------------------------------------------------
void CMsgCon::setErrSource( const SErrSource& i_errSource )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setErrSource(i_errSource);
}

//------------------------------------------------------------------------------
void CMsgCon::setAddErrInfoDscr( const QString& i_strAddErrInfoDscr )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setAddErrInfoDscr(i_strAddErrInfoDscr);
}

//------------------------------------------------------------------------------
void CMsgCon::setProgress( int i_iProgressInPerCent )
//------------------------------------------------------------------------------
{
    m_iProgressInPerCent = i_iProgressInPerCent;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgCon::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsg::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", ReqId:" + QString::number(m_iReqId);
    strAddTrcInfo += ", Result:" + getResultStr();
    strAddTrcInfo += ", Progress:" + QString::number(m_iProgressInPerCent);

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgCon::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsg::serialize();

    MsgProtocol::SMsgHeader* pMsgHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    pMsgHdr->m_idRequest         = m_iReqId;
    pMsgHdr->m_resultSeverity    = m_errResultInfo.getSeverity();
    pMsgHdr->m_result            = m_errResultInfo.getResult();
    pMsgHdr->m_iProgress_perCent = m_iProgressInPerCent;

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgInd : public CMsg
*******************************************************************************/

/*==============================================================================
protected: // ctors (pure virtual base class)
==============================================================================*/

//------------------------------------------------------------------------------
CMsgInd::CMsgInd(
    int      i_iMsgType,
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeAcknowledged,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsg(
        /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
        /* iMsgType      */ i_iMsgType,
        /* pObjSender    */ i_pObjSender,
        /* pObjReceiver  */ i_pObjReceiver,
        /* iMsgId        */ i_iMsgId ),
    m_bMustBeAcknowledged(i_bMustBeAcknowledged),
    m_iTimeout_ms(0),
    m_bIsBlockingRequest(false),
    m_iReqId(i_iReqId)
{
} // ctor

//------------------------------------------------------------------------------
CMsgInd::CMsgInd( const MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsg(i_pMsgHdr),
    m_bMustBeAcknowledged(i_pMsgHdr->m_flag == MsgProtocol::EFlagAck),
    m_iTimeout_ms(0),
    m_bIsBlockingRequest(false),
    m_iReqId(i_pMsgHdr->m_idRequest)
{
} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgInd::~CMsgInd()
//------------------------------------------------------------------------------
{
    m_bMustBeAcknowledged = false;
    m_iTimeout_ms = 0;
    m_bIsBlockingRequest = false;
    m_iReqId = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgInd::setMustBeAcknowledged( bool i_bMustBeAcknowledged )
//------------------------------------------------------------------------------
{
    m_bMustBeAcknowledged = i_bMustBeAcknowledged;
}

//------------------------------------------------------------------------------
void CMsgInd::setTimeoutInMs( int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    m_iTimeout_ms = i_iTimeout_ms;
}

//------------------------------------------------------------------------------
void CMsgInd::setIsBlockingRequest( bool i_bIsBlocking )
//------------------------------------------------------------------------------
{
    m_bIsBlockingRequest = i_bIsBlocking;
}

//------------------------------------------------------------------------------
void CMsgInd::setRequestId( qint64 i_iId )
//------------------------------------------------------------------------------
{
    m_iReqId = i_iId;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgInd::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsg::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", Ack:" + bool2Str(m_bMustBeAcknowledged);
    strAddTrcInfo += ", TimeoutMs:" + QString::number(m_iTimeout_ms);
    strAddTrcInfo += ", IsBlocking:" + bool2Str(m_bIsBlockingRequest);
    strAddTrcInfo += ", ReqId:" + QString::number(m_iReqId);

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgInd::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsg::serialize();

    MsgProtocol::SMsgHeader* pMsgHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    pMsgHdr->m_idRequest = m_iReqId;
    pMsgHdr->m_flag      = m_bMustBeAcknowledged;

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgAck : public CMsg
*******************************************************************************/

/*==============================================================================
protected: // ctors (pure virtual base class)
==============================================================================*/

//------------------------------------------------------------------------------
CMsgAck::CMsgAck(
    int                   i_iMsgType,
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdInd,
    const SErrResultInfo& i_errResultInfo,
    int                   i_iProgressInPerCent ) :
//------------------------------------------------------------------------------
    CMsg(
        /* systemMsgType */ MsgProtocol::ESystemMsgTypeAck,
        /* iMsgType      */ i_iMsgType,
        /* pObjSender    */ i_pObjSender,
        /* pObjReceiver  */ i_pObjReceiver,
        /* iMsgId        */ i_iMsgIdInd ),
    m_iReqId(i_iReqId),
    m_errResultInfo(i_errResultInfo),
    m_iProgressInPerCent(i_iProgressInPerCent)
{
} // ctor

//------------------------------------------------------------------------------
CMsgAck::CMsgAck( const MsgProtocol::SMsgHeader* i_pMsgHdr ) :
//------------------------------------------------------------------------------
    CMsg(i_pMsgHdr),
    m_iReqId(i_pMsgHdr->m_idRequest),
    m_errResultInfo(
        /* errSource */ nameSpace(), className(), objectName(), "ctor",
        /* result    */ static_cast<ZS::System::EResult>(i_pMsgHdr->m_result),
        /* severity  */ static_cast<ZS::System::EResultSeverity>(i_pMsgHdr->m_resultSeverity) ),
    m_iProgressInPerCent(i_pMsgHdr->m_iProgress_perCent)
{
} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgAck::~CMsgAck()
//------------------------------------------------------------------------------
{
    m_iReqId = 0;
    //m_errResultInfo;
    m_iProgressInPerCent = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgAck::setRequestId( qint64 i_iId )
//------------------------------------------------------------------------------
{
    m_iReqId = i_iId;
}

//------------------------------------------------------------------------------
void CMsgAck::setErrResultInfo( const SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    m_errResultInfo = i_errResultInfo;
}

//------------------------------------------------------------------------------
void CMsgAck::setErrResult( const SErrResult& i_errResult )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setErrResult(i_errResult);
}

//------------------------------------------------------------------------------
void CMsgAck::setSeverity( EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setSeverity(i_severity);
}

//------------------------------------------------------------------------------
void CMsgAck::setResult( EResult i_result )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setResult(i_result);
}

//------------------------------------------------------------------------------
void CMsgAck::setErrSource( const SErrSource& i_errSource )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setErrSource(i_errSource);
}

//------------------------------------------------------------------------------
void CMsgAck::setAddErrInfoDscr( const QString& i_strAddErrInfoDscr )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setAddErrInfoDscr(i_strAddErrInfoDscr);
}

//------------------------------------------------------------------------------
void CMsgAck::setProgress( int i_iProgressInPerCent )
//------------------------------------------------------------------------------
{
    m_iProgressInPerCent = i_iProgressInPerCent;
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgAck::getAddTrcInfoStr( int i_iDetailLevel, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo = CMsg::getAddTrcInfoStr(i_iDetailLevel,i_format);

    strAddTrcInfo += ", ReqId:" + QString::number(m_iReqId);
    strAddTrcInfo += ", Result:" + getResultStr();
    strAddTrcInfo += ", Progress:" + QString::number(m_iProgressInPerCent);

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QByteArray CMsgAck::serialize() const
//------------------------------------------------------------------------------
{
    QByteArray byteArr = CMsg::serialize();

    MsgProtocol::SMsgHeader* pMsgHdr = reinterpret_cast<MsgProtocol::SMsgHeader*>(byteArr.data());

    pMsgHdr->m_idRequest         = m_iReqId;
    pMsgHdr->m_resultSeverity    = m_errResultInfo.getSeverity();
    pMsgHdr->m_result            = m_errResultInfo.getResult();
    pMsgHdr->m_iProgress_perCent = m_iProgressInPerCent;

    return byteArr;

} // serialize


/*******************************************************************************
class CMsgErr : public CMsg
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgErr::CMsgErr(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    const SErrResultInfo& i_errResultInfo,
    qint64                i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgInd(
        /* iMsgType            */ EBaseMsgTypeIndErr,
        /* pObjSender          */ i_pObjSender,
        /* pObjReceiver        */ i_pObjReceiver,
        /* bMustBeAcknowledged */ false,
        /* iMsgId              */ i_iMsgId ),
    m_bExc(false),
    m_strExcType(),
    m_errResultInfo(i_errResultInfo)
{
    m_bIsBaseMsgType = true;

} // ctor

//------------------------------------------------------------------------------
CMsgErr::CMsgErr(
    QObject*          i_pObjSender,
    QObject*          i_pObjReceiver,
    const CException& i_exc,
    const QString&    i_strExcType,
    qint64            i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgInd(
        /* iMsgType            */ EBaseMsgTypeIndErr,
        /* pObjSender          */ i_pObjSender,
        /* pObjReceiver        */ i_pObjReceiver,
        /* bMustBeAcknowledged */ false,
        /* iMsgId              */ i_iMsgId ),
    m_bExc(true),
    m_strExcType(i_strExcType),
    m_errResultInfo(i_exc.getErrResultInfo())
{
    m_bIsBaseMsgType = true;

    if( !m_errResultInfo.isErrorResult() )
    {
        m_errResultInfo = SErrResultInfo(
            /* errSource     */ "ZS::System", "CException", i_strExcType, "",
            /* result        */ i_exc.getResult(),
            /* severity      */ EResultSeverityCritical,
            /* strAddErrInfo */ i_exc.getAddErrInfo() );
    }

} // ctor

//------------------------------------------------------------------------------
CMsgErr::~CMsgErr()
//------------------------------------------------------------------------------
{
    m_bExc = false;
    //m_strExcType;
    //m_errResultInfo;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgErr::setErrResultInfo( const SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    m_errResultInfo = i_errResultInfo;
}

//------------------------------------------------------------------------------
void CMsgErr::setErrResult( const SErrResult& i_errResult )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setErrResult(i_errResult);
}

//------------------------------------------------------------------------------
void CMsgErr::setSeverity( EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setSeverity(i_severity);
}

//------------------------------------------------------------------------------
void CMsgErr::setResult( EResult i_result )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setResult(i_result);
}

//------------------------------------------------------------------------------
void CMsgErr::setErrSource( const SErrSource& i_errSource )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setErrSource(i_errSource);
}

//------------------------------------------------------------------------------
void CMsgErr::setAddErrInfoDscr( const QString& i_strAddErrInfoDscr )
//------------------------------------------------------------------------------
{
    m_errResultInfo.setAddErrInfoDscr(i_strAddErrInfoDscr);
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgErr::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsgErr* pMsgCloned = new CMsgErr(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
        /* errResultInfo */ m_errResultInfo,
        /* iMsgId        */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    if( m_bExc )
    {
        pMsgCloned->m_bExc = true;
        pMsgCloned->m_strExcType = m_strExcType;
    }

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgAck* CMsgErr::createAcknowledgeMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgErr::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (ZS::Error)";
}

//------------------------------------------------------------------------------
QString CMsgErr::getAddTrcInfoStr( int /*i_iDetailLevel*/, ZS::System::EContentToStrFormat i_format )
//------------------------------------------------------------------------------
{
    QString strContent;
    QString strTblBeg;
    QString strTblEnd;
    QString strRowBeg;
    QString strRowEnd;
    QString strClmBeg;
    QString strClmEnd;

    if( i_format == EContentToStrFormat::HtmlText )
    {
        strTblBeg = "<table>";
        strTblEnd = "</table>";
        strRowBeg = "<tr>";
        strRowEnd = "</tr>";
        strClmBeg = "<td>";
        strClmEnd = "</td>";
    }
    else
    {
        strRowEnd = "\n";
        strClmEnd = " ";
    }
    strContent += strTblBeg;
    strContent += strRowBeg;
    strContent += strClmBeg + "Severity:" + strClmEnd;
    strContent += strClmBeg + m_errResultInfo.getSeverityStr() + strClmEnd;
    strContent += strRowEnd;
    strContent += strRowBeg;
    strContent += strClmBeg + "Result:" + strClmEnd;
    strContent += strClmBeg + m_errResultInfo.getResultStr() + strClmEnd;
    strContent += strRowEnd;
    strContent += strRowBeg;
    strContent += strClmBeg + "ErrorSource: " + strClmEnd;
    strContent += strClmBeg + m_errResultInfo.getErrSource().toString() + strClmEnd;
    strContent += strRowEnd;
    strContent += strRowBeg;
    strContent += strClmBeg + "AddInfo:" + strClmEnd;
    strContent += strClmBeg + m_errResultInfo.getAddErrInfoDscr() + strClmEnd;
    strContent += strRowEnd;
    strContent += strTblEnd;
    return strContent;

} // getAddTrcInfoStr


/*******************************************************************************
class CMsgReqAddLogItem : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqAddLogItem::CMsgReqAddLogItem(
    QObject*         i_pObjSender,
    QObject*         i_pObjReceiver,
    ETransmitDir     i_transmitDir,
    bool             i_bBold,
    const QString&   i_strMsg,
    const QDateTime& i_dtUtc,
    double           i_fSysTime_s ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EBaseMsgTypeReqAddLogItem,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ false ),
    m_transmitDir(i_transmitDir),
    m_bBold(i_bBold),
    m_strMsg(i_strMsg),
    m_dtUtc(i_dtUtc),
    m_fSysTime_s(i_fSysTime_s)
{
    m_bIsBaseMsgType = true;

} // ctor

//------------------------------------------------------------------------------
CMsgReqAddLogItem::~CMsgReqAddLogItem()
//------------------------------------------------------------------------------
{
    m_transmitDir = static_cast<ETransmitDir>(0);
    m_bBold = false;
    //m_strMsg;
    //m_dtUtc;
    m_fSysTime_s = 0.0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CMsgReqAddLogItem::setDateTimeUtc( const QDateTime& i_dtUtc )
//------------------------------------------------------------------------------
{
    m_dtUtc = i_dtUtc;
}

//------------------------------------------------------------------------------
void CMsgReqAddLogItem::setSystemTimeInSec( double i_fSysTime_s )
//------------------------------------------------------------------------------
{
    m_fSysTime_s = i_fSysTime_s;
}

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqAddLogItem::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsg* pMsgCloned = new CMsgReqAddLogItem(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
        /* transmitDir   */ m_transmitDir,
        /* bBold         */ m_bBold,
        /* strMsg        */ m_strMsg );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqAddLogItem::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqAddLogItem::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (ZS::ReqAddLogItem)";
}


/*******************************************************************************
class CMsgReqStartTimer : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqStartTimer::CMsgReqStartTimer(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    int      i_iTimerId,
    bool     i_bSingleShot,
    int      i_iInterval_ms,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EBaseMsgTypeReqStartTimer,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgId           */ i_iMsgId ),
    m_iTimerId(i_iTimerId),
    m_bSingleShot(i_bSingleShot),
    m_iInterval_ms(i_iInterval_ms)
{
    m_bIsBaseMsgType = true;

} // ctor

//------------------------------------------------------------------------------
CMsgReqStartTimer::~CMsgReqStartTimer()
//------------------------------------------------------------------------------
{
    m_iTimerId = 0;
    m_bSingleShot = false;
    m_iInterval_ms = 0;

} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqStartTimer::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsg* pMsgCloned = new CMsgReqStartTimer(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* iTimerId         */ m_iTimerId,
        /* bSingleShot      */ m_bSingleShot,
        /* iInterval_ms     */ m_iInterval_ms,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqStartTimer::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    CMsgConStartTimer* pMsgCon = new CMsgConStartTimer(
        /* pObjSender    */ m_pObjReceiver,
        /* pObjReceiver  */ m_pObjSender,
        /* iTimerId      */ m_iTimerId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ SErrResultInfo() );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqStartTimer::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (ZS::ReqStartTimer)";
}


/*******************************************************************************
class CMsgConStartTimer : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConStartTimer::CMsgConStartTimer(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    int                   i_iTimerId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType          */ EBaseMsgTypeConStartTimer,
        /* pObjSender        */ i_pObjSender,
        /* pObjReceiver      */ i_pObjReceiver,
        /* iReqId            */ -1,
        /* iMsgIdReq         */ i_iMsgIdReq,
        /* errResultInfo     */ i_errResultInfo,
        /* iProgress_perCent */ 100 ),
    m_iTimerId(i_iTimerId)
{
    m_bIsBaseMsgType = true;

} // ctor

//------------------------------------------------------------------------------
CMsgConStartTimer::~CMsgConStartTimer()
//------------------------------------------------------------------------------
{
    m_iTimerId = 0;

} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConStartTimer::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsg* pMsgCloned = new CMsgConStartTimer(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* iTimerId         */ m_iTimerId,
        /* iMsgIdReq        */ m_iMsgId,
        /* errResultInfo    */ m_errResultInfo );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConStartTimer::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (ZS::ConStartTimer)";
}


/*******************************************************************************
class CMsgReqStopTimer : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqStopTimer::CMsgReqStopTimer(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    int      i_iTimerId,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EBaseMsgTypeReqStopTimer,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgId           */ i_iMsgId ),
    m_iTimerId(i_iTimerId)
{
    m_bIsBaseMsgType = true;

} // ctor

//------------------------------------------------------------------------------
CMsgReqStopTimer::~CMsgReqStopTimer()
//------------------------------------------------------------------------------
{
    m_iTimerId = 0;

} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqStopTimer::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsg* pMsgCloned = new CMsgReqStopTimer(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* iTimerId         */ m_iTimerId,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqStopTimer::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    // The receiver of the request message becomes the sender and
    // the sender of the request message becomes the receiver
    // of the confirmation message.
    CMsgConStopTimer* pMsgCon = new CMsgConStopTimer(
        /* pObjSender    */ m_pObjReceiver,
        /* pObjReceiver  */ m_pObjSender,
        /* iTimerId      */ m_iTimerId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ SErrResultInfo() );

    pMsgCon->setSenderId(m_iReceiverId);
    pMsgCon->setReceiverId(m_iSenderId);

    return pMsgCon;

} // createConfirmationMessage

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqStopTimer::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (ZS::ReqStopTimer)";
}


/*******************************************************************************
class CMsgConStopTimer : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConStopTimer::CMsgConStopTimer(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    int                   i_iTimerId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType          */ EBaseMsgTypeConStopTimer,
        /* pObjSender        */ i_pObjSender,
        /* pObjReceiver      */ i_pObjReceiver,
        /* iReqId            */ -1,
        /* iMsgIdReq         */ i_iMsgIdReq,
        /* errResultInfo     */ i_errResultInfo,
        /* iProgress_perCent */ 100 ),
    m_iTimerId(i_iTimerId)
{
    m_bIsBaseMsgType = true;

} // ctor

//------------------------------------------------------------------------------
CMsgConStopTimer::~CMsgConStopTimer()
//------------------------------------------------------------------------------
{
    m_iTimerId = 0;

} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConStopTimer::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsg* pMsgCloned = new CMsgConStopTimer(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* iTimerId         */ m_iTimerId,
        /* iMsgIdReq        */ m_iMsgId,
        /* errResultInfo    */ m_errResultInfo );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConStopTimer::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (ZS::ConStopTimer)";
}


/*******************************************************************************
class CMsgReqContinue : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqContinue::CMsgReqContinue(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EBaseMsgTypeReqContinue,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgId           */ i_iMsgId )
{
    m_bIsBaseMsgType = true;

} // ctor

//------------------------------------------------------------------------------
CMsgReqContinue::~CMsgReqContinue()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqContinue::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsg* pMsgCloned = new CMsgReqContinue(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqContinue::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqContinue::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (ZS::ReqContinue)";
}


/*******************************************************************************
class CMsgReqAbort : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqAbort::CMsgReqAbort(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EBaseMsgTypeReqAbort,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgId           */ i_iMsgId )
{
    m_bIsBaseMsgType = true;

} // ctor

//------------------------------------------------------------------------------
CMsgReqAbort::~CMsgReqAbort()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqAbort::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsg* pMsgCloned = new CMsgReqAbort(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqAbort::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqAbort::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (ZS::ReqAbort)";
}


/*******************************************************************************
class CMsgReqPoll : public CMsgReq
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgReqPoll::CMsgReqPoll(
    QObject* i_pObjSender,
    QObject* i_pObjReceiver,
    bool     i_bMustBeConfirmed,
    qint64   i_iReqId,
    qint64   i_iMsgId ) :
//------------------------------------------------------------------------------
    CMsgReq(
        /* iMsgType         */ EBaseMsgTypeReqPoll,
        /* pObjSender       */ i_pObjSender,
        /* pObjReceiver     */ i_pObjReceiver,
        /* bMustBeConfirmed */ i_bMustBeConfirmed,
        /* iReqId           */ i_iReqId,
        /* iMsgId           */ i_iMsgId )
{
    m_bIsBaseMsgType = true;

} // ctor

//------------------------------------------------------------------------------
CMsgReqPoll::~CMsgReqPoll()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgReqPoll::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsg* pMsgCloned = new CMsgReqPoll(
        /* pObjSender       */ m_pObjSender,
        /* pObjReceiver     */ m_pObjReceiver,
        /* bMustBeConfirmed */ m_bMustBeConfirmed,
        /* iReqId           */ m_iReqId,
        /* iMsgId           */ m_iMsgId );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

//------------------------------------------------------------------------------
CMsgCon* CMsgReqPoll::createConfirmationMessage( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    throw ZS::System::CException( __FILE__, __LINE__, EResultMethodNotYetImplemented );
}

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgReqPoll::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (ZS::ReqPoll)";
}


/*******************************************************************************
class CMsgConStartThread : public CMsgCon
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMsgConStartThread::CMsgConStartThread(
    QObject*              i_pObjSender,
    QObject*              i_pObjReceiver,
    qint64                i_iReqId,
    qint64                i_iMsgIdReq,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CMsgCon(
        /* iMsgType          */ EBaseMsgTypeConStartThread,
        /* pObjSender        */ i_pObjSender,
        /* pObjReceiver      */ i_pObjReceiver,
        /* iReqId            */ i_iReqId,
        /* iMsgIdReq         */ i_iMsgIdReq,
        /* errResultInfo     */ i_errResultInfo,
        /* iProgress_perCent */ 100 )
{
    m_bIsBaseMsgType = true;

} // ctor

//------------------------------------------------------------------------------
CMsgConStartThread::~CMsgConStartThread()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // must overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
CMsg* CMsgConStartThread::clone( ECopyDepth /*i_copyDepth*/ )
//------------------------------------------------------------------------------
{
    CMsg* pMsgCloned = new CMsgConStartThread(
        /* pObjSender    */ m_pObjSender,
        /* pObjReceiver  */ m_pObjReceiver,
        /* iReqId        */ m_iReqId,
        /* iMsgIdReq     */ m_iMsgId,
        /* errResultInfo */ m_errResultInfo );

    pMsgCloned->setSenderId(m_iSenderId);
    pMsgCloned->setReceiverId(m_iReceiverId);

    return pMsgCloned;

} // clone

/*==============================================================================
public: // overridables of base class CMsg
==============================================================================*/

//------------------------------------------------------------------------------
QString CMsgConStartThread::msgTypeToStr() const
//------------------------------------------------------------------------------
{
    return QString::number(type()) + " (ZS::ConStartThread)";
}
