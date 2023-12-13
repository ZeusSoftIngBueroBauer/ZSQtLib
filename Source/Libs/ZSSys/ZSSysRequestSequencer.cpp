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

#include <QtCore/QCoreApplication>
#include <QtCore/QMutex>
#include <QtCore/QTimer>

#include "ZSSys/ZSSysRequestSequencer.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
struct SRequestSeqEntry
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SRequestSeqEntry::SRequestSeqEntry() :
//------------------------------------------------------------------------------
    m_reqEntryType(ERequestEntryTypeUndefined),
    m_reqDscr(),
    m_iReqIdPredecessor(-1),
    m_pObjFctExecute(nullptr),
    m_fctExecuteType1(nullptr),
    m_fctExecuteType2(nullptr),
    m_valExec(),
    m_pvExec(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
SRequestSeqEntry::SRequestSeqEntry( const SRequestDscr& i_reqDscr ) :
//------------------------------------------------------------------------------
    m_reqEntryType(ERequestEntryTypeUndefined),
    m_reqDscr(i_reqDscr),
    m_iReqIdPredecessor(-1),
    m_pObjFctExecute(nullptr),
    m_fctExecuteType1(nullptr),
    m_fctExecuteType2(nullptr),
    m_valExec(),
    m_pvExec(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
SRequestSeqEntry::SRequestSeqEntry( const SRequestSeqEntry& i_other ) :
//------------------------------------------------------------------------------
    m_reqEntryType(i_other.m_reqEntryType),
    m_reqDscr(i_other.m_reqDscr),
    m_iReqIdPredecessor(i_other.m_iReqIdPredecessor),
    m_pObjFctExecute(i_other.m_pObjFctExecute),
    m_fctExecuteType1(i_other.m_fctExecuteType1),
    m_fctExecuteType2(i_other.m_fctExecuteType2),
    m_valExec(i_other.m_valExec),
    m_pvExec(i_other.m_pvExec)
{
} // copy ctor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
SRequestSeqEntry& SRequestSeqEntry::operator = ( const SRequestSeqEntry& i_other )
//------------------------------------------------------------------------------
{
    m_reqEntryType      = i_other.m_reqEntryType;
    m_reqDscr           = i_other.m_reqDscr;
    m_iReqIdPredecessor = i_other.m_iReqIdPredecessor;
    m_pObjFctExecute    = i_other.m_pObjFctExecute;
    m_fctExecuteType1   = i_other.m_fctExecuteType1;
    m_fctExecuteType2   = i_other.m_fctExecuteType2;
    m_valExec           = i_other.m_valExec;
    m_pvExec            = i_other.m_pvExec;

    return *this;

} // operator =

//------------------------------------------------------------------------------
SRequestSeqEntry::~SRequestSeqEntry()
//------------------------------------------------------------------------------
{
    m_pObjFctExecute  = nullptr;
    m_fctExecuteType1 = nullptr;
    m_fctExecuteType2 = nullptr;
    m_pvExec          = nullptr;

} // dtor


/*******************************************************************************
class CRequestSequencer : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRequestSequencer::CRequestSequencer(
    const QString& i_strObjName,
    QObject*       i_pObjParent,
    bool           i_bKeepReqDscrInExecTree,
    bool           i_bTracingEnabled ) :
//------------------------------------------------------------------------------
    QObject(i_pObjParent),
    m_pMtx(nullptr),
    m_bKeepReqDscrInExecTree(i_bKeepReqDscrInExecTree),
    m_bMsgReqContinuePending(false),
    m_bInProgress(false),
    m_pReqParent(nullptr),
    m_iReqIdParent(-1),
    m_ariReqIds(),
    m_hshReqSeqs(),
    m_hshpReqs(),
    m_bTracingEnabled(i_bTracingEnabled),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    if( m_bTracingEnabled )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // The list of requests must be protected as creating and deleting
    // requests might be called from within different thread contexts.
    // On emitting signals the mutex is still very likely locked.
    // Receivers of the signal may call methods of the sequencer as a reentry.
    // So we need to use a recursive mutex to allow the same thread to access
    // the list of requests (at least to find a request item).
    m_pMtx = new QRecursiveMutex();

} // ctor

//------------------------------------------------------------------------------
CRequestSequencer::~CRequestSequencer()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    clear(); // Protects the lists by locking and unlocking the mutex.

    try
    {
        delete m_pMtx;
    }
    catch(...)
    {
    }

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pMtx = nullptr;
    m_bKeepReqDscrInExecTree = false;
    m_bMsgReqContinuePending = false;
    m_bInProgress = false;
    m_pReqParent = nullptr;
    m_iReqIdParent = 0;
    m_ariReqIds.clear();
    m_hshReqSeqs.clear();
    m_hshpReqs.clear();
    m_bTracingEnabled = false;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
qint64 CRequestSequencer::GetUniqueRequestId() const
//------------------------------------------------------------------------------
{
    return CRequest::GetUniqueRequestId();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestSequencer::setKeepReqDscrInExecTree( bool i_bKeep )
//------------------------------------------------------------------------------
{
    m_bKeepReqDscrInExecTree = i_bKeep;
}

/*==============================================================================
public: // instance methods (To add an inactive group node. Inactive group nodes cannot be executed.)
==============================================================================*/

//------------------------------------------------------------------------------
qint64 CRequestSequencer::addGroup(
    const QString& i_strCreatorNameSpace,
    const QString& i_strCreatorClassName,
    QObject*       i_pObjCreator,
    int            i_request,
    const QString& i_strRequest,
    qint64         i_iReqIdParent,
    qint64         i_iReqIdPredecessor )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Request: " + i_strRequest + " (" + QString::number(i_request) + ")";
        strAddTrcInfo += ", Creator: " + buildPathStr("::", i_strCreatorNameSpace, i_strCreatorClassName, QString(i_pObjCreator == nullptr ? "nullptr" : i_pObjCreator->objectName()) );
        strAddTrcInfo += ", ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", ReqIdPredecessor: " + QString::number(i_iReqIdPredecessor);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addGroup",
        /* strAddInfo   */ strAddTrcInfo );

    SRequestDscr reqDscr(
        /* iId          */ GetUniqueRequestId(),
        /* strNameSpace */ i_strCreatorNameSpace,
        /* strClassName */ i_strCreatorClassName,
        /* pObjCreator  */ i_pObjCreator,
        /* iRequest     */ i_request,
        /* strRequest   */ i_strRequest,
        /* iReqIdParent */ i_iReqIdParent );

    SRequestSeqEntry* pReqSeqEntry = new SRequestSeqEntry(reqDscr);

    pReqSeqEntry->m_reqEntryType = ERequestEntryTypeGroupNode;

    // On adding a request childs ids cannot be taken over as that would
    // imply using the same predecessor and execution function.
    pReqSeqEntry->m_reqDscr.m_ariChildIds.clear();

    pReqSeqEntry->m_iReqIdPredecessor = i_iReqIdPredecessor;

    add(pReqSeqEntry);

    return reqDscr.m_iId;

} // addGroup

/*==============================================================================
public: // instance methods (To add an active request node. Active request nodes cannot have children.)
==============================================================================*/

//------------------------------------------------------------------------------
qint64 CRequestSequencer::addRequest(
    const QString&   i_strCreatorNameSpace,
    const QString&   i_strCreatorClassName,
    QObject*         i_pObjCreator,
    int              i_request,
    const QString&   i_strRequest,
    QObject*         i_pObjFctExecute,
    TFctExecuteType1 i_fctExecute,
    const QVariant&  i_valExec,
    qint64           i_iReqIdParent,
    qint64           i_iReqIdPredecessor,
    int              i_iTimeout_ms,
    bool             i_bIsBlocking )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Request: " + i_strRequest + " (" + QString::number(i_request) + ")";
        strAddTrcInfo += ", Creator: " + buildPathStr("::", i_strCreatorNameSpace, i_strCreatorClassName, QString(i_pObjCreator == nullptr ? "nullptr" : i_pObjCreator->objectName()) );
        strAddTrcInfo += ", ObjFctExecute: " + QString( i_pObjFctExecute == nullptr ? "nullptr" : i_pObjFctExecute->objectName() );
        strAddTrcInfo += ", FctExecute: " + QString( i_fctExecute == nullptr ? "nullptr" : pointer2Str(i_pObjFctExecute) );
        strAddTrcInfo += ", ValExec: " + i_valExec.toString();
        strAddTrcInfo += ", ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", ReqIdPredecessor: " + QString::number(i_iReqIdPredecessor);
        strAddTrcInfo += ", TimeoutInMs: " + QString::number(i_iTimeout_ms);
        strAddTrcInfo += ", Blocking: " + bool2Str(i_bIsBlocking);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addRequest",
        /* strAddInfo   */ strAddTrcInfo );

    SRequestDscr reqDscr(
        /* iId          */ GetUniqueRequestId(),
        /* strNameSpace */ i_strCreatorNameSpace,
        /* strClassName */ i_strCreatorClassName,
        /* pObjCreator  */ i_pObjCreator,
        /* iRequest     */ i_request,
        /* strRequest   */ i_strRequest,
        /* iParentId    */ i_iReqIdParent,
        /* iTimeout     */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bIsBlocking );

    SRequestSeqEntry* pReqSeqEntry = new SRequestSeqEntry(reqDscr);

    pReqSeqEntry->m_reqEntryType = ERequestEntryTypeActiveNode;

    // On adding a request childs cannot be taken over as that would
    // imply using the same predecessor and execution function.
    pReqSeqEntry->m_reqDscr.m_ariChildIds.clear();

    pReqSeqEntry->m_iReqIdPredecessor = i_iReqIdPredecessor;
    pReqSeqEntry->m_pObjFctExecute    = i_pObjFctExecute;
    pReqSeqEntry->m_fctExecuteType1   = i_fctExecute;
    pReqSeqEntry->m_valExec           = i_valExec;

    add(pReqSeqEntry);

    return reqDscr.m_iId;

} // addRequest

//------------------------------------------------------------------------------
qint64 CRequestSequencer::addRequest(
    const QString&   i_strCreatorNameSpace,
    const QString&   i_strCreatorClassName,
    QObject*         i_pObjCreator,
    int              i_request,
    const QString&   i_strRequest,
    QObject*         i_pObjFctExecute,
    TFctExecuteType2 i_fctExecute,
    void*            i_pvExec,
    qint64           i_iReqIdParent,
    qint64           i_iReqIdPredecessor,
    int              i_iTimeout_ms,
    bool             i_bIsBlocking )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Request: " + i_strRequest + " (" + QString::number(i_request) + ")";
        strAddTrcInfo += ", Creator: " + buildPathStr("::", i_strCreatorNameSpace, i_strCreatorClassName, QString(i_pObjCreator == nullptr ? "nullptr" : i_pObjCreator->objectName()) );
        strAddTrcInfo += ", ObjFctExecute: " + QString( i_pObjFctExecute == nullptr ? "nullptr" : i_pObjFctExecute->objectName() );
        strAddTrcInfo += ", FctExecute: " + QString( i_fctExecute == nullptr ? "nullptr" : pointer2Str(i_pObjFctExecute) );
        strAddTrcInfo += ", pvExec: " + QString( i_pvExec == nullptr ? "nullptr" : pointer2Str(i_pvExec) );
        strAddTrcInfo += ", ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", ReqIdPredecessor: " + QString::number(i_iReqIdPredecessor);
        strAddTrcInfo += ", TimeoutInMs: " + QString::number(i_iTimeout_ms);
        strAddTrcInfo += ", Blocking: " + bool2Str(i_bIsBlocking);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "addRequest",
        /* strAddInfo   */ strAddTrcInfo );

    SRequestDscr reqDscr(
        /* iId          */ GetUniqueRequestId(),
        /* strNameSpace */ i_strCreatorNameSpace,
        /* strClassName */ i_strCreatorClassName,
        /* pObjCreator  */ i_pObjCreator,
        /* iRequest     */ i_request,
        /* strRequest   */ i_strRequest,
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout     */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bIsBlocking );

    SRequestSeqEntry* pReqSeqEntry = new SRequestSeqEntry(reqDscr);

    pReqSeqEntry->m_reqEntryType = ERequestEntryTypeActiveNode;

    // On adding a request childs cannot be taken over as that would
    // imply using the same predecessor and execution function.
    pReqSeqEntry->m_reqDscr.m_ariChildIds.clear();

    pReqSeqEntry->m_iReqIdPredecessor = i_iReqIdPredecessor;
    pReqSeqEntry->m_pObjFctExecute    = i_pObjFctExecute;
    pReqSeqEntry->m_fctExecuteType2   = i_fctExecute;
    pReqSeqEntry->m_pvExec            = i_pvExec;

    add(pReqSeqEntry);

    return reqDscr.m_iId;

} // addRequest

//------------------------------------------------------------------------------
void CRequestSequencer::removeRequest( qint64 i_iReqId )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    SRequestSeqEntry* pReqSeqEntry = nullptr;

    if( m_hshReqSeqs.contains(i_iReqId) )
    {
        pReqSeqEntry = m_hshReqSeqs[i_iReqId];
    }

    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "Request {";
        strAddTrcInfo += "Id: " + QString::number(i_iReqId);
        strAddTrcInfo += ", " + QString(pReqSeqEntry == nullptr ? "nullptr" : pReqSeqEntry->m_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel));
        strAddTrcInfo += "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "removeRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( pReqSeqEntry == nullptr )
    {
        QString strAddErrInfo = "Request with id " + QString::number(i_iReqId);
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
    }

    if( pReqSeqEntry->m_reqDscr.m_iId != i_iReqId )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError );
    }

    SRequestSeqEntry* pReqSeqEntryParent = nullptr;

    if( m_hshReqSeqs.contains(pReqSeqEntry->m_reqDscr.m_iParentId) )
    {
        pReqSeqEntryParent = m_hshReqSeqs[pReqSeqEntry->m_reqDscr.m_iParentId];

        if( pReqSeqEntryParent->m_reqDscr.m_iId != pReqSeqEntry->m_reqDscr.m_iParentId )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError );
        }
    }

    clear(pReqSeqEntry);

    if( pReqSeqEntryParent != nullptr )
    {
        pReqSeqEntryParent->m_reqDscr.m_ariChildIds.removeOne(i_iReqId);
    }
    else
    {
        m_ariReqIds.removeOne(i_iReqId);
    }

    m_hshReqSeqs.remove(i_iReqId);

    delete pReqSeqEntry;
    pReqSeqEntry = nullptr;

    emit requestRemoved(i_iReqId);

} // removeRequest

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestSequencer::start( CRequest* i_pReqParent )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "ReqParent {" + QString( i_pReqParent == nullptr ? "nullptr" : i_pReqParent->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "start",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_ariReqIds.size() == 0 )
    {
        if( i_pReqParent != nullptr )
        {
            i_pReqParent->setResult(EResultSuccess);
            i_pReqParent->setProgressInPerCent(100);
            i_pReqParent->update();

            // The one who calls update for the finished request must delete the request.
            #ifdef _WINDOWS
            #pragma message(__TODO__"Introduce member RequestToBeDeletedLater")
            #endif
            // Can't be deleted as caller may access the request.
            //deleteRequest(i_pReqParent);
            //i_pReqParent = nullptr;
        }
    }
    else if( !m_bInProgress )
    {
        SRequestSeqEntry* pReqSeqEntry;
        SRequestSeqEntry* pReqSeqEntryPredecessor = nullptr;
        bool              bCanStartRequest = true;
        CRequest*         pReq;
        qint64            iReqId;
        int               idxReq;

        m_bInProgress = true;

        // To start the sequence for all request descriptors:
        // - the object state must be set to detached,
        // - the progress must be set to 0%,
        // - the result must be set to Undefined and
        // - the start and end times must be invalidated.
        for( idxReq = 0; idxReq < m_ariReqIds.size(); idxReq++ )
        {
            iReqId = m_ariReqIds[idxReq];
            pReqSeqEntry = m_hshReqSeqs[iReqId];
            reset(pReqSeqEntry,true);
        }

        if( m_pReqParent != nullptr )
        {
            QObject::disconnect(
                m_pReqParent, &CRequest::destroyed,
                this, QOverload<QObject*>::of(&CRequestSequencer::onParentRequestDestroyed));
        }

        // All top level requests will get the same parent request id provided on starting the sequence.
        m_pReqParent = i_pReqParent;

        bool bParentIsBlocking = false;
        int  iParentTimeout_ms = 0;

        bool bIsBlocking = false;
        int  iTimeout_ms = 0;

        if( m_pReqParent == nullptr )
        {
            m_iReqIdParent = -1;
        }
        else
        {
            // If the parent request is a blocking request also all the childs
            // must be executed as blocking requests.
            bParentIsBlocking = m_pReqParent->isBlockingRequest();
            iParentTimeout_ms = m_pReqParent->getTimeoutInMs();

            QObject::connect(
                m_pReqParent, &QObject::destroyed,
                this, QOverload<QObject*>::of(&CRequestSequencer::onParentRequestDestroyed));

            m_iReqIdParent = m_pReqParent->getId();

            if( m_bKeepReqDscrInExecTree )
            {
                m_pReqParent->setKeepReqDscrInExecTree(true);
            }
        }

        emit started(m_iReqIdParent);

        // The InProgress flag will be set to true by the "startRequest" method
        // if a request will be started or is not yet finished.
        //m_bInProgress = false;

        for( idxReq = 0; idxReq < m_ariReqIds.size(); idxReq++ )
        {
            iReqId = m_ariReqIds[idxReq];

            pReqSeqEntry = m_hshReqSeqs[iReqId];

            bIsBlocking = pReqSeqEntry->m_reqDscr.m_bIsBlocking;
            iTimeout_ms = pReqSeqEntry->m_reqDscr.m_iTimeout_ms;

            // If the parent request is a blocking request also all the childs
            // must be executed as blocking requests.
            bIsBlocking = bParentIsBlocking ? bParentIsBlocking : bIsBlocking;

            // If the parent has got a timeout assigned ..
            if( iParentTimeout_ms > 0 )
            {
                // .. the child requests timeout should not take longer.
                if( iTimeout_ms <= 0 || iTimeout_ms > iParentTimeout_ms )
                {
                    iTimeout_ms = iParentTimeout_ms;
                }
            }

            pReqSeqEntryPredecessor = nullptr;

            bCanStartRequest = true;

            // If the request has a predecessor the predecessor must first
            // be finished before the request can be started.
            if( pReqSeqEntry->m_iReqIdPredecessor >= 0 )
            {
                if( m_hshReqSeqs.contains(pReqSeqEntry->m_iReqIdPredecessor) )
                {
                    pReqSeqEntryPredecessor = m_hshReqSeqs[pReqSeqEntry->m_iReqIdPredecessor];
                }
            }
            if( pReqSeqEntryPredecessor != nullptr )
            {
                if( pReqSeqEntryPredecessor->m_reqDscr.m_objState != EObjState::Destroyed && pReqSeqEntryPredecessor->m_reqDscr.m_iProgress_perCent < 100 )
                {
                    bCanStartRequest = false;
                }
            }

            if( bCanStartRequest )
            {
                // All top level requests will get the same parent request id provided on starting the sequence.
                pReq = startRequest( iTimeout_ms, bIsBlocking, m_iReqIdParent, iReqId );

                if( isAsynchronousRequest(pReq) )
                {
                    if( pReq->isBlockingRequest() )
                    {
                        if( !pReq->wait() )
                        {
                            pReq->setResult(EResultTimeout);
                            pReq->setProgressInPerCent(100);
                            pReq->update();

                            // The one who calls update for the finished request must delete the request.
                            deleteRequest(pReq);
                            pReq = nullptr;
                        }
                    }
                } // if( isAsynchronousRequest(pReq) )
            } // if( bCanStartRequest )
        } // for( idxReq = m_ariReqIds.size()-1; idxReq >= 0; idxReq-- )

        m_bInProgress = !areAllTopLevelRequestsFinished();

        if( m_bInProgress )
        {
            if( m_pReqParent != nullptr && m_pReqParent->isBlockingRequest() )
            {
                if( !m_pReqParent->wait() )
                {
                    m_pReqParent->setResult(EResultTimeout);
                    m_pReqParent->setProgressInPerCent(100);
                    m_pReqParent->update();

                    // The one who calls update for the finished request must delete the request.
                    deleteRequest(m_pReqParent);
                    m_pReqParent = nullptr;
                }
            }
        }

        m_bInProgress = !isSequenceFinished();

        if( !m_bInProgress )
        {
            emit finished(m_iReqIdParent);
        }
    } // if( !m_bInProgress )

} // start

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CRequestSequencer::isSequenceFinished() const
//------------------------------------------------------------------------------
{
    bool bFinished = areAllTopLevelRequestsFinished();

    if( bFinished )
    {
        if( m_pReqParent != nullptr )
        {
            bFinished = (m_pReqParent->getProgressInPerCent() >= 100);
        }
    }
    return bFinished;

} // isSequenceFinished

//------------------------------------------------------------------------------
bool CRequestSequencer::areAllTopLevelRequestsFinished() const
//------------------------------------------------------------------------------
{
    bool bFinished = true;

    SRequestSeqEntry* pReqSeqEntry;
    qint64            iReqId;
    int               idxReq;

    for( idxReq = m_ariReqIds.size()-1; idxReq >= 0; idxReq-- )
    {
        iReqId = m_ariReqIds[idxReq];

        pReqSeqEntry = m_hshReqSeqs[iReqId];

        if( pReqSeqEntry->m_reqDscr.m_iProgress_perCent < 100 )
        {
            bFinished = false;
            break;
        }
    }
    return bFinished;

} // areAllTopLevelRequestsFinished

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestSequencer::clear()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "clear",
        /* strAddInfo   */ "" );

    if( m_ariReqIds.size() > 0 )
    {
        SRequestSeqEntry* pReqSeqEntry;
        qint64            iReqId;
        int               idxReq;

        for( idxReq = m_ariReqIds.size()-1; idxReq >= 0; idxReq-- )
        {
            iReqId = m_ariReqIds[idxReq];

            pReqSeqEntry = m_hshReqSeqs[iReqId];

            // The "clear" method removes and deletes the children of the entry.
            // The "clear" method also aborts the corresponding request and
            // removes the request id from the hash of requests in progress.
            clear(pReqSeqEntry);

            m_ariReqIds.removeLast();

            m_hshReqSeqs.remove(iReqId);

            delete pReqSeqEntry;
            pReqSeqEntry = nullptr;

            emit requestRemoved(iReqId);

        } // for( idxReq = m_ariReqIds.size()-1; idxReq >= 0; idxReq-- )
    } // if( m_ariReqIds.size() > 0 )

} // clear

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CRequestSequencer::getAddTrcInfoStr( int /*i_iDetailLevel*/ ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestSequencer::lock()
//------------------------------------------------------------------------------
{
    m_pMtx->lock();
}

//------------------------------------------------------------------------------
void CRequestSequencer::unlock()
//------------------------------------------------------------------------------
{
    m_pMtx->unlock();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SRequestSeqEntry* CRequestSequencer::getRequestSeqEntry( qint64 i_iReqId ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    SRequestSeqEntry* pReqSeqEntry = nullptr;

    if( m_hshReqSeqs.contains(i_iReqId) )
    {
        pReqSeqEntry = m_hshReqSeqs[i_iReqId];
    }
    return pReqSeqEntry;

} // getRequestSeqEntry

//------------------------------------------------------------------------------
CRequest* CRequestSequencer::getRequest( qint64 i_iReqId ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    CRequest* pReq = nullptr;

    if( m_hshpReqs.contains(i_iReqId) )
    {
        pReq = m_hshpReqs[i_iReqId];
    }
    return pReq;

} // getRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SRequestDscr CRequestSequencer::add( SRequestSeqEntry* i_pReqSeqEntry )
//------------------------------------------------------------------------------
{
    qint64 iReqIdParent = i_pReqSeqEntry->m_reqDscr.m_iParentId;

    SRequestSeqEntry* pReqSeqEntryParent = nullptr;
    SRequestSeqEntry* pReqSeqEntryPredecessor = nullptr;

    bool bParentStarted = false;
    bool bPredecessorStarted = false;

    if( iReqIdParent >= 0 )
    {
        if( m_hshReqSeqs.contains(iReqIdParent) )
        {
            pReqSeqEntryParent = m_hshReqSeqs[iReqIdParent];
        }
        if( m_hshpReqs.contains(iReqIdParent) )
        {
            bParentStarted = true;
        }
    }

    if( i_pReqSeqEntry->m_iReqIdPredecessor >= 0 )
    {
        if( m_hshReqSeqs.contains(i_pReqSeqEntry->m_iReqIdPredecessor) )
        {
            pReqSeqEntryPredecessor = m_hshReqSeqs[i_pReqSeqEntry->m_iReqIdPredecessor];
        }
        if( m_hshpReqs.contains(i_pReqSeqEntry->m_iReqIdPredecessor) )
        {
            bPredecessorStarted = true;
        }
    }

    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "ReqSeqEntry: " + i_pReqSeqEntry->m_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "add",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) )
    {
        if( pReqSeqEntryParent == nullptr )
        {
            strAddTrcInfo = "ReqSeqEntryParent: nullptr";
        }
        else
        {
            strAddTrcInfo = "ReqSeqEntryParent.ReqDscr: " + pReqSeqEntryParent->m_reqDscr.getAddTrcInfoStr(0);
        }
        strAddTrcInfo += ", ParentStarted: " + QString( bParentStarted ? "true" : "false" );

        if( pReqSeqEntryPredecessor == nullptr )
        {
            strAddTrcInfo += ", ReqSeqEntryPredecessor: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqSeqEntryPredecessor.ReqDscr: " + pReqSeqEntryPredecessor->m_reqDscr.getAddTrcInfoStr(0);
        }
        strAddTrcInfo += ", PredecessorStarted: " + QString( bPredecessorStarted ? "true" : "false" );
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_hshReqSeqs.contains(i_pReqSeqEntry->m_reqDscr.m_iId) )
    {
        QString strAddErrInfo = i_pReqSeqEntry->m_reqDscr.getAddTrcInfoStr(1);
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }
    if( i_pReqSeqEntry->m_reqDscr.m_iParentId >= 0 && pReqSeqEntryParent == nullptr )
    {
        QString strAddErrInfo = "Parent request with Id " + QString::number(i_pReqSeqEntry->m_reqDscr.m_iParentId) + " not found.";
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
    }
    if( pReqSeqEntryParent != nullptr && pReqSeqEntryParent->m_reqEntryType != ERequestEntryTypeGroupNode )
    {
        QString strAddErrInfo = "Parent request entries cannot be executed and must be of group node type.";
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, strAddErrInfo );
    }
    if( i_pReqSeqEntry->m_iReqIdPredecessor >= 0 && pReqSeqEntryPredecessor == nullptr )
    {
        QString strAddErrInfo = "Predecessor request with Id " + QString::number(i_pReqSeqEntry->m_iReqIdPredecessor) + " not found.";
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
    }
    if( pReqSeqEntryPredecessor != nullptr && pReqSeqEntryPredecessor->m_reqDscr.m_iParentId != i_pReqSeqEntry->m_reqDscr.m_iParentId )
    {
        QString strAddErrInfo = "The request and its predecessor must have the same parent";
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo );
    }

    if( i_pReqSeqEntry->m_iReqIdPredecessor < 0 )
    {
        if( pReqSeqEntryParent == nullptr )
        {
            m_ariReqIds.append(i_pReqSeqEntry->m_reqDscr.m_iId);
        }
        else if( pReqSeqEntryParent->m_reqDscr.m_ariChildIds.indexOf(i_pReqSeqEntry->m_reqDscr.m_iId) < 0 )
        {
            pReqSeqEntryParent->m_reqDscr.m_ariChildIds.append(i_pReqSeqEntry->m_reqDscr.m_iId);
        }
    } // if( i_pReqSeqEntry->m_iReqIdPredecessor < 0 )

    else // if( i_pReqSeqEntry->m_iReqIdPredecessor >= 0 )
    {
        qint64 iReqIdSibling;
        int    idxSibling;
        int    idxPredecessor = -1;

        if( pReqSeqEntryParent == nullptr )
        {
            for( idxPredecessor = -1, idxSibling = 0; idxSibling < m_ariReqIds.size(); idxSibling++ )
            {
                iReqIdSibling = m_ariReqIds[idxSibling];

                if( iReqIdSibling == i_pReqSeqEntry->m_iReqIdPredecessor )
                {
                    idxPredecessor = idxSibling;
                    break;
                }
            }

            // Should never happen (the checks above should have been sufficient).
            if( idxPredecessor < 0 )
            {
                QString strAddErrInfo = "The request and its predecessor must have the same parent";
                throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, strAddErrInfo );
            }
            else if( idxPredecessor >= m_ariReqIds.size() )
            {
                QString strAddErrInfo = "The request and its predecessor must have the same parent";
                throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, strAddErrInfo );
            }

            // Inserts value at index position i in the list. If i is size(), the value is appended to the list.
            m_ariReqIds.insert(idxPredecessor+1,i_pReqSeqEntry->m_reqDscr.m_iId);

        } // if( pReqSeqEntryParent == nullptr )

        else // if( pReqSeqEntryParent != nullptr )
        {
            for( idxSibling = 0; idxSibling < pReqSeqEntryParent->m_reqDscr.m_ariChildIds.size(); idxSibling++ )
            {
                iReqIdSibling = pReqSeqEntryParent->m_reqDscr.m_ariChildIds[idxSibling];

                if( iReqIdSibling == i_pReqSeqEntry->m_iReqIdPredecessor )
                {
                    idxPredecessor = idxSibling;
                    break;
                }
            }

            // Should never happen (the checks above should have been sufficient).
            if( idxPredecessor < 0 )
            {
                QString strAddErrInfo = "The request and its predecessor must have the same parent";
                throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, strAddErrInfo );
            }
            else if( idxPredecessor >= pReqSeqEntryParent->m_reqDscr.m_ariChildIds.size() )
            {
                QString strAddErrInfo = "The request and its predecessor must have the same parent";
                throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, strAddErrInfo );
            }

            // Inserts value at index position i in the list. If i is size(), the value is appended to the list.
            pReqSeqEntryParent->m_reqDscr.m_ariChildIds.insert(idxPredecessor+1,i_pReqSeqEntry->m_reqDscr.m_iId);

        } // if( pReqSeqEntryParent != nullptr )
    } // if( i_pReqSeqEntry->m_iReqIdPredecessor >= 0 )

    m_hshReqSeqs[i_pReqSeqEntry->m_reqDscr.m_iId] = i_pReqSeqEntry;

    emit requestAdded(i_pReqSeqEntry->m_reqDscr.m_iId);

    return i_pReqSeqEntry->m_reqDscr;

} // add

//------------------------------------------------------------------------------
CRequest* CRequestSequencer::startRequest(
    int    i_iTimeout_ms,
    bool   i_bIsBlocking,
    qint64 i_iReqIdParent,
    qint64 i_iReqId )
//------------------------------------------------------------------------------
{
    SRequestSeqEntry* pReqSeqEntry = m_hshReqSeqs[i_iReqId];

    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "Request {";
        strAddTrcInfo += "Id: " + QString::number(i_iReqId);
        strAddTrcInfo += ", " + QString(pReqSeqEntry == nullptr ? "nullptr" : pReqSeqEntry->m_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel));
        strAddTrcInfo += "}";
        strAddTrcInfo += ", Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", IsBlocking: " + bool2Str(i_bIsBlocking);
        strAddTrcInfo += ", ReqIdParent: " + QString::number(i_iReqIdParent);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "startRequest",
        /* strAddInfo   */ strAddTrcInfo );

    CRequest* pReq = nullptr;

    SRequestSeqEntry* pReqSeqEntryChild;
    SRequestSeqEntry* pReqSeqEntryPredecessor = nullptr;
    CRequest*         pReqInactiveNode = nullptr;
    qint64            iReqIdNew = i_iReqId;
    int               idxReq;
    bool              bIsAsynchronous = false;

    if( pReqSeqEntry->m_reqDscr.m_objState == EObjState::Detached )
    {
        pReqSeqEntry->m_reqDscr.m_fTimeStart_s = ZS::System::Time::getProcTimeInSec();

        bool bActiveNode = false;

        if( pReqSeqEntry->m_fctExecuteType1 != nullptr )
        {
            bActiveNode = true;
            pReq = pReqSeqEntry->m_fctExecuteType1( pReqSeqEntry->m_pObjFctExecute, i_iTimeout_ms, i_bIsBlocking, i_iReqIdParent, pReqSeqEntry->m_valExec );
        }
        else if( pReqSeqEntry->m_fctExecuteType2 != nullptr )
        {
            bActiveNode = true;
            pReq = pReqSeqEntry->m_fctExecuteType2( pReqSeqEntry->m_pObjFctExecute, i_iTimeout_ms, i_bIsBlocking, i_iReqIdParent, pReqSeqEntry->m_pvExec );
        }

        // If it was a node to give other requests a parent ...
        if( !bActiveNode )
        {
            // .. a dummy request instance will be created so that in the request
            // execution tree the parent child relationship will be visualized.
            pReqSeqEntry->m_reqDscr.m_iParentId = i_iReqIdParent;
            pReq = new CRequest(pReqSeqEntry->m_reqDscr);

            // If the group node has no children ..
            if( pReqSeqEntry->m_reqDscr.m_ariChildIds.size() == 0 )
            {
                // .. this is unusual (very likely a config error) but we handle the request as finished.
                pReq->setResult(EResultSuccess);
                pReq->setProgressInPerCent(100);
                pReq->update();

                // The one who calls update for the finished request must delete the request.
                deleteRequest(pReq);
                pReq = nullptr;
            }
            else
            {
                pReq->setResult(EResultUndefined);
                pReq->setProgressInPerCent(0);
                pReq->update();
            }

            pReqInactiveNode = pReq;

        } // if( !bActiveNode )

        // If the request function returned nullptr (synchronous request with no result) ..
        else if( /* bActiveNode && */ pReq == nullptr )
        {
            pReqSeqEntry->m_reqDscr.m_errResultInfo.setResult(EResultSuccess);
            pReqSeqEntry->m_reqDscr.m_fTimeEnd_s = ZS::System::Time::getProcTimeInSec();
            pReqSeqEntry->m_reqDscr.m_iProgress_perCent = 100;
            pReqSeqEntry->m_reqDscr.m_objState = EObjState::Destroyed;
            pReqSeqEntry->m_reqDscr.m_iParentId = i_iReqIdParent;

            #ifdef _WINDOWS
            #pragma message(__TODO__"Don't call slot directly")
            #endif
            throw CException(__FILE__, __LINE__, EResultMethodNotYetImplemented, "Dont call onRequestChanged slot directly");
            //onRequestChanged(pReqSeqEntry->m_reqDscr);

        } // if( pReq == nullptr )

        // If the request function returned a pointer to a request instance ..
        else // if( /* bActiveNode && */ pReq != nullptr )
        {
            // Store reference to started (and maybe already finished) request instance.
            // This Id is not equal to the Id passed to the startRequest method. The Id
            // passed is the temporary created Id on adding the request sequence entry
            // to the sequence tree.
            iReqIdNew = pReq->getId();
            m_hshpReqs[iReqIdNew] = pReq;

            if( m_bKeepReqDscrInExecTree )
            {
                pReq->setKeepReqDscrInExecTree(m_bKeepReqDscrInExecTree);
            }

            if( isAsynchronousRequest(pReq) )
            {
                //m_bInProgress = true;
                bIsAsynchronous = true;

                // Only connect the signal if the request was asynchronous.
                // Synchronous requests may be deleted by the request queue of
                // the executer sometimes later.
                QObject::connect(
                    pReq, &CRequest::changed,
                    this, &CRequestSequencer::onRequestChanged);
            }

            // Should be anyway the case ...
            if( iReqIdNew != i_iReqId )
            {
                SRequestSeqEntry* pReqSeqEntryParent;
                SRequestSeqEntry* pReqSeqEntrySibling;
                qint64            iReqIdParent;
                qint64            iReqIdSibling;
                qint64            iReqIdChild;

                bool bTopLevelRequest = (m_ariReqIds.indexOf(i_iReqId) >= 0);

                // Top level request?
                if( bTopLevelRequest )
                {
                    // Update request id in list of child requests of the parent.
                    idxReq = m_ariReqIds.indexOf(i_iReqId);
                    m_ariReqIds[idxReq] = iReqIdNew;

                    // Update request id in siblings which have this request as the predecessor.
                    for( ++idxReq; idxReq < m_ariReqIds.size(); idxReq++ )
                    {
                        iReqIdSibling = m_ariReqIds[idxReq];
                        pReqSeqEntrySibling = m_hshReqSeqs[iReqIdSibling];
                        if( pReqSeqEntrySibling->m_iReqIdPredecessor == i_iReqId )
                        {
                            pReqSeqEntrySibling->m_iReqIdPredecessor = iReqIdNew;
                            emit requestChanged( pReqSeqEntrySibling->m_reqDscr.m_iId, pReqSeqEntrySibling->m_reqDscr.m_iId );
                        }
                    }
                }

                // Not a top level request?
                else // if( !bTopLevelRequest )
                {
                    // Update request id in list of child requests of the parent.
                    iReqIdParent = pReqSeqEntry->m_reqDscr.m_iParentId;
                    pReqSeqEntryParent = m_hshReqSeqs[iReqIdParent];
                    idxReq = pReqSeqEntryParent->m_reqDscr.m_ariChildIds.indexOf(i_iReqId);
                    pReqSeqEntryParent->m_reqDscr.m_ariChildIds[idxReq] = iReqIdNew;
                    emit requestChanged( pReqSeqEntryParent->m_reqDscr.m_iId, pReqSeqEntryParent->m_reqDscr.m_iId );

                    // Update request id in siblings which have this request as the predecessor.
                    for( ++idxReq; idxReq < pReqSeqEntryParent->m_reqDscr.m_ariChildIds.size(); idxReq++ )
                    {
                        iReqIdSibling = pReqSeqEntryParent->m_reqDscr.m_ariChildIds[idxReq];
                        pReqSeqEntrySibling = m_hshReqSeqs[iReqIdSibling];
                        if( pReqSeqEntrySibling->m_iReqIdPredecessor == i_iReqId )
                        {
                            pReqSeqEntrySibling->m_iReqIdPredecessor = iReqIdNew;
                            emit requestChanged( pReqSeqEntrySibling->m_reqDscr.m_iId, pReqSeqEntrySibling->m_reqDscr.m_iId );
                        }
                    }
                } // if( !bTopLevelRequest )

                // Update request id in list of my child requests.
                for( idxReq = 0; idxReq < pReqSeqEntry->m_reqDscr.m_ariChildIds.size(); idxReq++ )
                {
                    iReqIdChild = pReqSeqEntry->m_reqDscr.m_ariChildIds[idxReq];
                    pReqSeqEntryChild = m_hshReqSeqs[iReqIdChild];
                    pReqSeqEntryChild->m_reqDscr.m_iParentId = iReqIdNew;
                    emit requestChanged( pReqSeqEntryChild->m_reqDscr.m_iId, pReqSeqEntryChild->m_reqDscr.m_iId );
                }

                // Update request id in hash.
                m_hshReqSeqs.remove(i_iReqId);
                m_hshReqSeqs[iReqIdNew] = pReqSeqEntry;

            } // if( iReqIdNew != i_iReqId )

            // We need to keep the child ids of the request descriptors in the sequence.
            pReqSeqEntry->m_reqDscr.initFrom( pReq->getDescriptor() );

            // But the parent id will be changed.
            pReqSeqEntry->m_reqDscr.m_iParentId = i_iReqIdParent;

            if( pReqSeqEntry->m_reqDscr.m_objState != EObjState::Created && pReqSeqEntry->m_reqDscr.m_objState != EObjState::Detached )
            {
                QString strAddErrInfo = "ObjState of a newly created request must be Created but is " + CEnumObjState::toString(pReqSeqEntry->m_reqDscr.m_objState);
                throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, strAddErrInfo );
            }

            if( pReqInactiveNode == nullptr && !isAsynchronousRequest(pReq) )
            {
                // Please note that the inactive group request will be deleted
                // after its children have been processed.
                pReqSeqEntry->m_reqDscr.m_objState = EObjState::Destroyed;
                emit requestChanged( pReqSeqEntry->m_reqDscr.m_iId, pReqSeqEntry->m_reqDscr.m_iId );

                // And the node must be removed from the hash with requests again.
                m_hshpReqs.remove(pReqSeqEntry->m_reqDscr.m_iId);

                m_bInProgress = !isSequenceFinished();

                // If the request has a parent group node the progress of the parent node has to be updated.
                // If all child requests of the parent node have been processed completely the parent node
                // has been finished and the inactive dummy request has to be destroyed.
                if( pReqSeqEntry->m_reqDscr.m_iParentId >= 0 )
                {
                    updateParentRequest(pReqSeqEntry);

                    // Synchronous requests may have been set at the request queue to be deleted later.
                    // On updating the parent request to 100% the parent request as well may be set at
                    // the request queue to be deleted later deleting the request instance returned
                    // the the request function. To avoid access violations we cannot access the synchronous
                    // request instance anymore if 
                    if( !bIsAsynchronous )
                    {
                        CRequest* pReqParent = getParentRequest(pReqSeqEntry);

                        if( pReqParent != nullptr && pReqParent->getProgressInPerCent() >= 100 )
                        {
                            pReq = nullptr;
                        }
                    }
                } // if( pReqSeqEntry->m_reqDscr.m_iParentId >= 0 )
            } // if( pReqInactiveNode == nullptr && !isAsynchronousRequest(pReq) )
        } // if( pReq != nullptr )

        emit requestChanged(i_iReqId,iReqIdNew);

    } // if( pReqSeqEntry->m_reqDscr.m_objState == EObjState::Detached )

    CRequest* pReqChild;
    qint64    iReqIdChild;
    bool      bStartRequest;
    int       iTimeout_ms;
    bool      bIsBlocking;

    for( idxReq = 0; idxReq < pReqSeqEntry->m_reqDscr.m_ariChildIds.size(); idxReq++ )
    {
        iReqIdChild = pReqSeqEntry->m_reqDscr.m_ariChildIds[idxReq];

        pReqSeqEntryChild = m_hshReqSeqs[iReqIdChild];

        bIsBlocking = pReqSeqEntryChild->m_reqDscr.m_bIsBlocking;
        iTimeout_ms = pReqSeqEntryChild->m_reqDscr.m_iTimeout_ms;

        // If the parent request is a blocking request also all the childs
        // must be executed as blocking requests.
        bIsBlocking = bIsBlocking ? i_bIsBlocking : bIsBlocking;

        // If the parent has got a timeout assigned ..
        if( i_iTimeout_ms > 0 )
        {
            // .. the child requests timeout should not take longer.
            if( iTimeout_ms <= 0 || iTimeout_ms > i_iTimeout_ms )
            {
                iTimeout_ms = i_iTimeout_ms;
            }
        }

        pReqSeqEntryPredecessor = nullptr;

        bStartRequest = true;

        // If the request has a predecessor the predecessor must first
        // be finished before the request can be started.
        if( pReqSeqEntryChild->m_iReqIdPredecessor >= 0 )
        {
            if( m_hshReqSeqs.contains(pReqSeqEntryChild->m_iReqIdPredecessor) )
            {
                pReqSeqEntryPredecessor = m_hshReqSeqs[pReqSeqEntryChild->m_iReqIdPredecessor];
            }
        }
        if( pReqSeqEntryPredecessor != nullptr )
        {
            if( pReqSeqEntryPredecessor->m_reqDscr.m_objState != EObjState::Destroyed && pReqSeqEntryPredecessor->m_reqDscr.m_iProgress_perCent < 100 )
            {
                bStartRequest = false;
            }
        }

        // If the request has not yet been started ..
        if( bStartRequest && pReqSeqEntryChild->m_reqDscr.m_objState == EObjState::Detached )
        {
            pReqChild = startRequest( iTimeout_ms, bIsBlocking, iReqIdNew, iReqIdChild );

            if( isAsynchronousRequest(pReqChild) )
            {
                //m_bInProgress = true;
                bIsAsynchronous = true;
            }
        } // if( pReqSeqEntryChild->m_reqDscr.m_objState == EObjState::Detached )

        // If the request has already been started but not yet finished ..
        else if( pReqSeqEntryChild->m_reqDscr.m_objState == EObjState::Created && pReqSeqEntryChild->m_reqDscr.m_iProgress_perCent < 100 )
        {
            bIsAsynchronous = true;
        }

        // If the request has already been started and has been finished ..
        // .. check next request.

    } // for( idxReq = 0; idxReq < pReqSeqEntry->m_reqDscr.m_ariChildIds.size(); idxReq++ )

    // If it was just a node to give other requests a parent and if the group node
    // has no childrens and was therefore executed synchronously ...
    if( pReqInactiveNode != nullptr && !bIsAsynchronous )
    {
        if( pReq == pReqInactiveNode )
        {
            pReq = nullptr;
        }

        // .. the dummy request instance will be deleted again.
        delete pReqInactiveNode;
        pReqInactiveNode = nullptr;

        // Please note that as the request was not asynchronous the "onRequestChanged" slot was not connected
        // with the changed signal of the request and thus we have to call the slot explicitly.
        pReqSeqEntry->m_reqDscr.m_objState = EObjState::Destroyed;
        emit requestChanged( pReqSeqEntry->m_reqDscr.m_iId, pReqSeqEntry->m_reqDscr.m_iId );

        m_bInProgress = !isSequenceFinished();

        if( m_bInProgress )
        {
            // If the request has a parent group node the progress of the parent node has to be updated.
            // If all child requests of the parent node have been processed completely the parent node
            // has been finished and the inactive dummy request has to be destroyed.
            if( pReqSeqEntry->m_reqDscr.m_iParentId >= 0 )
            {
                updateParentRequest(pReqSeqEntry);

                // Synchronous requests may have been set at the request queue to be deleted later.
                // On updating the parent request to 100% the parent request as well may be set at
                // the request queue to be deleted later deleting the request instance returned
                // the the request function. To avoid access violations we cannot access the synchronous
                // request instance anymore if 
                if( !bIsAsynchronous )
                {
                    CRequest* pReqParent = getParentRequest(pReqSeqEntry);

                    if( pReqParent != nullptr && pReqParent->getProgressInPerCent() >= 100 )
                    {
                        pReq = nullptr;
                    }
                }
            } // if( pReqSeqEntry->m_reqDscr.m_iParentId >= 0 )
        } // if( m_bInProgress )

        // If the request has been finished ..
        if( pReqSeqEntry->m_reqDscr.m_iProgress_perCent >= 100 )
        {
            m_bInProgress = !isSequenceFinished();

            if( !m_bInProgress )
            {
                emit finished(m_iReqIdParent);
            }
        } // if( pReqSeqEntry->m_reqDscr.m_iProgress_perCent >= 100 )
    } // if( pReqInactiveNode != nullptr && !bIsAsynchronous )

    if( pReq != nullptr && bIsAsynchronous )
    {
        if( pReq->getProgressInPerCent() >= 100 )
        {
            pReq->setProgressInPerCent(50);
        }
    }

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = QString( pReq == nullptr ? "SUCCESS" : "{" + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}" );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // startRequest

//------------------------------------------------------------------------------
void CRequestSequencer::clear( SRequestSeqEntry* i_pReqSeqEntry )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "Request {";
        strAddTrcInfo += i_pReqSeqEntry == nullptr ? "nullptr" : i_pReqSeqEntry->m_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += "}";
    }

    QString strMth = "clear";

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ strMth,
        /* strAddInfo   */ strAddTrcInfo );

    SRequestSeqEntry* pReqSeqEntry;
    qint64            iReqId;
    int               idxReq;

    // First recursively remove and delete all children.
    if( i_pReqSeqEntry != nullptr && i_pReqSeqEntry->m_reqDscr.m_ariChildIds.size() > 0 )
    {
        for( idxReq = i_pReqSeqEntry->m_reqDscr.m_ariChildIds.size()-1; idxReq >= 0; idxReq-- )
        {
            iReqId = i_pReqSeqEntry->m_reqDscr.m_ariChildIds[idxReq];

            pReqSeqEntry = m_hshReqSeqs[iReqId];

            if( pReqSeqEntry != nullptr )
            {
                clear(pReqSeqEntry);
            }

            i_pReqSeqEntry->m_reqDscr.m_ariChildIds.removeLast();

            m_hshReqSeqs.remove(iReqId);

            delete pReqSeqEntry;
            pReqSeqEntry = nullptr;

            emit requestRemoved(iReqId);
        }
    } // if( i_pReqSeqEntry != nullptr && i_pReqSeqEntry->m_reqDscr.m_ariChildIds.size() > 0 )

    // If the corresponding request has already been started ..
    if( m_hshpReqs.contains(i_pReqSeqEntry->m_reqDscr.m_iId) )
    {
        CRequest* pReq = m_hshpReqs[i_pReqSeqEntry->m_reqDscr.m_iId];

        m_hshpReqs.remove(i_pReqSeqEntry->m_reqDscr.m_iId);

        // Please note that the owner of the request is the creator of the request.
        // On receiving the "changed" signal (on applying the "update" method) the
        // creator must abort the request in process and delete the request.
        if( pReq->getProgressInPerCent() < 100 )
        {
            SErrResultInfo errResultInfo(
                /* errSource     */ NameSpace(), ClassName(), objectName(), strMth,
                /* result        */ EResultRequestAborted,
                /* severity      */ EResultSeverityError,
                /* strAddErrInfo */ "Removed from request sequencer" );
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();

            // The one who calls update for the finished request must delete the request.
            deleteRequest(pReq);
            pReq = nullptr;
        }
    } // if( m_hshpReqs.contains(i_pReqSeqEntry->m_reqDscr.m_iId) )

} // clear

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestSequencer::reset( SRequestSeqEntry* i_pReqSeqEntry, bool i_bRecursive )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "Request {";
        strAddTrcInfo += i_pReqSeqEntry == nullptr ? "nullptr" : i_pReqSeqEntry->m_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += "}";
        strAddTrcInfo += ", Recursive: " + QString( i_bRecursive ? "true" : "false" );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "reset",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_bRecursive )
    {
        SRequestSeqEntry* pReqSeqEntry;
        qint64            iReqId;
        int               idxReq;

        if( i_pReqSeqEntry->m_reqDscr.m_ariChildIds.size() > 0 )
        {
            for( idxReq = i_pReqSeqEntry->m_reqDscr.m_ariChildIds.size()-1; idxReq >= 0; idxReq-- )
            {
                iReqId = i_pReqSeqEntry->m_reqDscr.m_ariChildIds[idxReq];
                pReqSeqEntry = m_hshReqSeqs[iReqId];
                reset(pReqSeqEntry,i_bRecursive);
            }
        }
    } // if( i_bRecursive )

    bool bChanged = false;

    if( i_pReqSeqEntry->m_reqDscr.m_objState != EObjState::Detached )
    {
        i_pReqSeqEntry->m_reqDscr.m_objState = EObjState::Detached;
        bChanged = true;
    }
    if( i_pReqSeqEntry->m_reqDscr.m_errResultInfo != SErrResultInfo(EResultUndefined,EResultSeveritySuccess) )
    {
        i_pReqSeqEntry->m_reqDscr.m_errResultInfo = SErrResultInfo(EResultUndefined,EResultSeveritySuccess);
        bChanged = true;
    }
    if( i_pReqSeqEntry->m_reqDscr.m_iProgress_perCent != 0 )
    {
        i_pReqSeqEntry->m_reqDscr.m_iProgress_perCent = 0;
        bChanged = true;
    }
    if( i_pReqSeqEntry->m_reqDscr.m_fTimeStart_s != 0.0 )
    {
        i_pReqSeqEntry->m_reqDscr.m_fTimeStart_s = 0.0;
        bChanged = true;
    }
    if( i_pReqSeqEntry->m_reqDscr.m_fTimeEnd_s != 0.0 )
    {
        i_pReqSeqEntry->m_reqDscr.m_fTimeEnd_s = 0.0;
        bChanged = true;
    }

    if( bChanged )
    {
        emit requestChanged( i_pReqSeqEntry->m_reqDscr.m_iId, i_pReqSeqEntry->m_reqDscr.m_iId );
    }

} // reset

//------------------------------------------------------------------------------
CRequest* CRequestSequencer::getParentRequest( SRequestSeqEntry* i_pReqSeqEntry )
//------------------------------------------------------------------------------
{
    CRequest* pReqParent = nullptr;

    // Root entry ..
    if( i_pReqSeqEntry->m_reqDscr.m_iParentId == m_iReqIdParent && m_pReqParent != nullptr )
    {
        pReqParent = m_pReqParent;
    }
    // Any group entry underneath the root ..
    else if( m_hshpReqs.contains(i_pReqSeqEntry->m_reqDscr.m_iParentId) )
    {
        pReqParent = m_hshpReqs[i_pReqSeqEntry->m_reqDscr.m_iParentId];
    }
    return pReqParent;

} // getParentRequest

//------------------------------------------------------------------------------
void CRequestSequencer::updateParentRequest( SRequestSeqEntry* i_pReqSeqEntry )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "Request {";
        strAddTrcInfo += i_pReqSeqEntry == nullptr ? "nullptr" : i_pReqSeqEntry->m_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateParentRequest",
        /* strAddInfo   */ strAddTrcInfo );

    // Root entry ..
    if( i_pReqSeqEntry->m_reqDscr.m_iParentId == m_iReqIdParent && m_pReqParent != nullptr )
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) )
        {
            int iAddTrcInfoDetailLevel = 0;
            if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
            else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

            strAddTrcInfo  = "ParentRequest {";
            strAddTrcInfo += m_pReqParent == nullptr ? "nullptr" : m_pReqParent->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += "}";
            mthTracer.trace(strAddTrcInfo);
        }

        SRequestSeqEntry* pReqSeqEntryChild;
        qint64            iReqIdChild;
        double            fProgress_perCent = 0.0;
        double            fChildProgress_perCent;
        bool              bInProgress = false;
        int               idxReq;

        for( idxReq = 0; idxReq < m_ariReqIds.size(); idxReq++ )
        {
            iReqIdChild = m_ariReqIds[idxReq];

            pReqSeqEntryChild = m_hshReqSeqs[iReqIdChild];

            if( pReqSeqEntryChild->m_reqDscr.m_objState == EObjState::Destroyed )
            {
                fChildProgress_perCent = 100;
            }
            else if( pReqSeqEntryChild->m_reqDscr.m_iProgress_perCent >= 100 )
            {
                fChildProgress_perCent = pReqSeqEntryChild->m_reqDscr.m_iProgress_perCent;
            }
            else
            {
                bInProgress = true;
                fChildProgress_perCent = pReqSeqEntryChild->m_reqDscr.m_iProgress_perCent;
            }
            fChildProgress_perCent = fChildProgress_perCent / m_ariReqIds.size();
            fProgress_perCent += fChildProgress_perCent;
        }

        int iProgress_perCent = static_cast<int>(fProgress_perCent);

        // There might be an inaccuracy when calculating the progress in per cent.
        if( !bInProgress )
        {
            iProgress_perCent = 100;
        }

        SErrResultInfo errResultInfo = m_pReqParent->getErrResultInfo();

        if( !bInProgress && iProgress_perCent >= 100 )
        {
            if( errResultInfo.getResult() == EResultUndefined || errResultInfo.getResult() == EResultSuccessIntermediateResults )
            {
                errResultInfo.setResult(EResultSuccess);
            }
        }

        m_pReqParent->setErrResultInfo(errResultInfo);
        m_pReqParent->setProgressInPerCent(iProgress_perCent);
        m_pReqParent->update();

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) )
        {
            int iAddTrcInfoDetailLevel = 0;
            if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
            else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

            strAddTrcInfo  = "ParentRequest {";
            strAddTrcInfo += m_pReqParent == nullptr ? "nullptr" : m_pReqParent->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += "}";
            mthTracer.trace(strAddTrcInfo);
        }
    } // if( i_pReqSeqEntry->m_reqDscr.m_iParentId == m_iReqIdParent )

    // Any group entry underneath the root ..
    else if( m_hshReqSeqs.contains(i_pReqSeqEntry->m_reqDscr.m_iParentId) )
    {
        SRequestSeqEntry* pReqSeqEntryParent = m_hshReqSeqs[i_pReqSeqEntry->m_reqDscr.m_iParentId];

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::DebugDetailed) )
        {
            int iAddTrcInfoDetailLevel = 0;
            if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
            else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

            strAddTrcInfo  = "ParentRequest {";
            strAddTrcInfo += pReqSeqEntryParent == nullptr ? "nullptr" : pReqSeqEntryParent->m_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += "}";
            mthTracer.trace(strAddTrcInfo);
        }

        if( pReqSeqEntryParent != nullptr )
        {
            SRequestSeqEntry* pReqSeqEntryChild;
            qint64            iReqIdChild;
            double            fProgress_perCent = 0.0;
            double            fChildProgress_perCent;
            bool              bInProgress = false;
            int               idxReq;

            for( idxReq = 0; idxReq < pReqSeqEntryParent->m_reqDscr.m_ariChildIds.size(); idxReq++ )
            {
                iReqIdChild = pReqSeqEntryParent->m_reqDscr.m_ariChildIds[idxReq];

                pReqSeqEntryChild = m_hshReqSeqs[iReqIdChild];

                if( pReqSeqEntryChild->m_reqDscr.m_objState == EObjState::Destroyed )
                {
                    fChildProgress_perCent = 100;
                }
                else if( pReqSeqEntryChild->m_reqDscr.m_iProgress_perCent >= 100 )
                {
                    fChildProgress_perCent = pReqSeqEntryChild->m_reqDscr.m_iProgress_perCent;
                }
                else
                {
                    bInProgress = true;
                    fChildProgress_perCent = pReqSeqEntryChild->m_reqDscr.m_iProgress_perCent;
                }
                fChildProgress_perCent = fChildProgress_perCent / pReqSeqEntryParent->m_reqDscr.m_ariChildIds.size();
                fProgress_perCent += fChildProgress_perCent;
            }

            int iProgress_perCent = static_cast<int>(fProgress_perCent);

            SErrResultInfo errResultInfo = pReqSeqEntryParent->m_reqDscr.m_errResultInfo;

            if( !bInProgress && iProgress_perCent >= 100 )
            {
                if( errResultInfo.getResult() == EResultUndefined || errResultInfo.getResult() == EResultSuccessIntermediateResults )
                {
                    errResultInfo.setResult(EResultSuccess);
                }
            }

            pReqSeqEntryParent->m_reqDscr.m_errResultInfo = errResultInfo;
            pReqSeqEntryParent->m_reqDscr.m_iProgress_perCent = iProgress_perCent;

            emit requestChanged( pReqSeqEntryParent->m_reqDscr.m_iId, pReqSeqEntryParent->m_reqDscr.m_iId );

            if( m_hshpReqs.contains(pReqSeqEntryParent->m_reqDscr.m_iId) )
            {
                CRequest* pReqInactiveNode = m_hshpReqs[pReqSeqEntryParent->m_reqDscr.m_iId];

                pReqInactiveNode->setErrResultInfo(pReqSeqEntryParent->m_reqDscr.m_errResultInfo);
                pReqInactiveNode->setProgressInPerCent(pReqSeqEntryParent->m_reqDscr.m_iProgress_perCent);
                pReqInactiveNode->update();

                if( pReqInactiveNode->getResult() != EResultUndefined && pReqInactiveNode->getProgressInPerCent() >= 100 )
                {
                    deleteRequest(pReqInactiveNode);
                    pReqInactiveNode = nullptr;
                }
            }
        } // if( pReqSeqEntryParent != nullptr )

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            int iAddTrcInfoDetailLevel = 0;
            if( m_pTrcAdminObj->getRuntimeInfoTraceDetailLevel() >= ELogDetailLevel::DebugVerbose ) iAddTrcInfoDetailLevel = 2;
            else if( m_pTrcAdminObj->getRuntimeInfoTraceDetailLevel() >= ELogDetailLevel::DebugDetailed ) iAddTrcInfoDetailLevel = 1;

            strAddTrcInfo  = "ParentRequest {";
            strAddTrcInfo += pReqSeqEntryParent == nullptr ? "nullptr" : pReqSeqEntryParent->m_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += "}";
            mthTracer.trace(strAddTrcInfo);
        }
    } // if( m_hshReqSeqs.contains(i_pReqSeqEntry->m_reqDscr.m_iParentId) )

} // updateParentRequest

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestSequencer::onParentRequestDestroyed( QObject* /*i_pReqParent*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onParentRequestDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    m_pReqParent = nullptr;

    // If the parent request is destroyed the sequence got to be aborted.
    SRequestSeqEntry* pReqSeqEntry;
    qint64            iReqId;
    int               idxReq;

    for( idxReq = 0; idxReq < m_ariReqIds.size(); idxReq++ )
    {
        iReqId = m_ariReqIds[idxReq];
        pReqSeqEntry = m_hshReqSeqs[iReqId];
        onParentRequestDestroyed(pReqSeqEntry);
    }

    m_bInProgress = !isSequenceFinished();

    if( m_bInProgress )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError );
    }

    emit finished(m_iReqIdParent);

} // onParentRequestDestroyed

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestSequencer::onParentRequestDestroyed( SRequestSeqEntry* i_pReqSeqEntry )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo  = "Request {";
        strAddTrcInfo += i_pReqSeqEntry == nullptr ? "nullptr" : i_pReqSeqEntry->m_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onParentRequestDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    SRequestSeqEntry* pReqSeqEntry;
    qint64            iReqId;
    int               idxReq;

    if( i_pReqSeqEntry->m_reqDscr.m_ariChildIds.size() > 0 )
    {
        for( idxReq = i_pReqSeqEntry->m_reqDscr.m_ariChildIds.size()-1; idxReq >= 0; idxReq-- )
        {
            iReqId = i_pReqSeqEntry->m_reqDscr.m_ariChildIds[idxReq];
            pReqSeqEntry = m_hshReqSeqs[iReqId];
            onParentRequestDestroyed(pReqSeqEntry);
        }
    }

    bool bChanged = false;

    if( i_pReqSeqEntry->m_reqDscr.m_objState == EObjState::Detached )
    {
        i_pReqSeqEntry->m_reqDscr.m_objState = EObjState::Destroyed;
        bChanged = true;
    }
    if( i_pReqSeqEntry->m_reqDscr.m_errResultInfo.getSeverity() < EResultSeverityError )
    {
        i_pReqSeqEntry->m_reqDscr.m_errResultInfo = SErrResultInfo(EResultRequestAborted,EResultSeverityError);
        bChanged = true;
    }
    if( i_pReqSeqEntry->m_reqDscr.m_iProgress_perCent != 100 )
    {
        i_pReqSeqEntry->m_reqDscr.m_iProgress_perCent = 100;
        bChanged = true;
    }
    if( i_pReqSeqEntry->m_reqDscr.m_fTimeStart_s == 0.0 )
    {
        i_pReqSeqEntry->m_reqDscr.m_fTimeStart_s = ZS::System::Time::getProcTimeInSec();
        bChanged = true;
    }
    if( i_pReqSeqEntry->m_reqDscr.m_fTimeEnd_s == 0.0 )
    {
        i_pReqSeqEntry->m_reqDscr.m_fTimeEnd_s  = ZS::System::Time::getProcTimeInSec();
        bChanged = true;
    }

    if( bChanged )
    {
        emit requestChanged( i_pReqSeqEntry->m_reqDscr.m_iId, i_pReqSeqEntry->m_reqDscr.m_iId );
    }

} // onParentRequestDestroyed

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestSequencer::onRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        int iAddTrcInfoDetailLevel = 0;
        if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iAddTrcInfoDetailLevel = 2;
        else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iAddTrcInfoDetailLevel = 1;

        strAddTrcInfo = "Request {" + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onRequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_hshReqSeqs.contains(i_reqDscr.m_iId) )
    {
        CRequest* pReq = m_hshpReqs.value(i_reqDscr.m_iId, nullptr);

        if( pReq != nullptr && (i_reqDscr.m_objState == EObjState::Destroyed || i_reqDscr.m_iProgress_perCent >= 100) )
        {
            m_hshpReqs.remove(i_reqDscr.m_iId);

            QObject::disconnect(
                pReq, &CRequest::changed,
                this, &CRequestSequencer::onRequestChanged);
        }

        SRequestSeqEntry* pReqSeqEntry = m_hshReqSeqs[i_reqDscr.m_iId];

        // We need to keep the child ids of the request descriptors in the sequence.
        pReqSeqEntry->m_reqDscr.initFrom(i_reqDscr);

        emit requestChanged(i_reqDscr.m_iId,i_reqDscr.m_iId);

        // Please note that a request may emit the changed signal several times.
        // If the request has been finished the progress is set to 100%. If the
        // request has been finished the whole sequence may be finished and the
        // finished signal will be emitted. If the request is destroyed (after
        // it has been finished) the object state is set to Destroyed and the
        // changed signal is emitted again. To avoid that the finished signal
        // of the sequence is emitted twice we need to check whether the sequence
        // is still in progress.

        m_bInProgress = !isSequenceFinished();

        if( m_bInProgress )
        {
            // If the request has a parent group node the progress of the parent node has to be updated.
            // If all child requests of the parent node have been processed completely the parent node
            // has been finished and the inactive dummy request has to be destroyed.
            if( pReqSeqEntry->m_reqDscr.m_iParentId >= 0 )
            {
                updateParentRequest(pReqSeqEntry);
            }

            // If the request has been finished ..
            if( pReqSeqEntry->m_reqDscr.m_iProgress_perCent >= 100 )
            {
                m_bInProgress = !isSequenceFinished();

                //// Check whether all requests have been finished.
                //qint64 iReqId;
                //QHash<qint64,SRequestSeqEntry*>::iterator it = m_hshReqSeqs.begin();
                //while( it != m_hshReqSeqs.end() )
                //{
                //    iReqId = it.key();
                //    pReqSeqEntry = it.value();

                //    // If the request has not yet been started ..
                //    if( pReqSeqEntry->m_reqDscr.m_objState == EObjState::Detached )
                //    {
                //        m_bInProgress = true;
                //        break;
                //    }
                //    // If the request has already been started but has not already been finished ..
                //    else if( pReqSeqEntry->m_reqDscr.m_objState == EObjState::Created && pReqSeqEntry->m_reqDscr.m_iProgress_perCent < 100 )
                //    {
                //        m_bInProgress = true;
                //        break;
                //    }
                //    it++;

                //} // while( it != m_hshReqSeqs.end() )

                if( m_bInProgress )
                {
                    if( !m_bMsgReqContinuePending )
                    {
                        m_bMsgReqContinuePending = true;

                        CMsgReqContinue* pMsgReq = new CMsgReqContinue(
                            /* pObjSender       */ this,
                            /* pObjReceiver     */ this,
                            /* bMustBeConfirmed */ false,
                            /* iReqId           */ -1,
                            /* iMsgId           */ -1 );
                        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ELogDetailLevel::DebugDetailed);
                    }
                }
                else // if( !m_bInProgress )
                {
                    emit finished(m_iReqIdParent);
                }
            } // if( pReqSeqEntry->m_reqDscr.m_iProgress_perCent >= 100 )
        } // if( m_bInProgress )
    } // if( m_hshReqSeqs.contains(i_reqDscr.m_iId) )

} // onRequestChanged

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CRequestSequencer::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    CMsg* pMsg = dynamic_cast<CMsg*>(i_pEv);

    if( pMsg != nullptr )
    {
        QString strAddTrcInfo;

        if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
        {
            int iMsgTrcInfoDetailLevel = 0;
            if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsVerbose ) iMsgTrcInfoDetailLevel = 2;
            else if( m_pTrcAdminObj->getMethodCallsTraceDetailLevel() >= EMethodTraceDetailLevel::ArgsDetailed ) iMsgTrcInfoDetailLevel = 1;

            strAddTrcInfo = pMsg->getAddTrcInfoStr(iMsgTrcInfoDetailLevel);
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* eDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        if( static_cast<int>(i_pEv->type()) == static_cast<int>(EBaseMsgTypeReqContinue) )
        {
            CMsgReqContinue* pMsgReq = dynamic_cast<CMsgReqContinue*>(i_pEv);

            if( pMsgReq == nullptr )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgReqContinue == nullptr! " );
            }

            bEventHandled = true;

            m_bMsgReqContinuePending = false;

            SRequestSeqEntry* pReqSeqEntry;
            SRequestSeqEntry* pReqSeqEntryPredecessor = nullptr;
            bool              bCanStartRequest = true;
            CRequest*         pReq;
            qint64            iReqId;
            int               idxReq;
            int               iTimeout_ms;
            bool              bIsBlocking;

            // The InProgress flag will be set again to true if not all requests have been finished.
            bool bInProgressPrev = !isSequenceFinished();

            for( idxReq = 0; idxReq < m_ariReqIds.size(); idxReq++ )
            {
                iReqId = m_ariReqIds[idxReq];

                pReqSeqEntry = m_hshReqSeqs[iReqId];

                iTimeout_ms = pReqSeqEntry->m_reqDscr.m_iTimeout_ms;
                bIsBlocking = pReqSeqEntry->m_reqDscr.m_bIsBlocking;

                // If the request has not been finished
                // (don't check for Detached here as we want also want to start child requests) ..
                if( pReqSeqEntry->m_reqDscr.m_iProgress_perCent < 100 )
                {
                    pReqSeqEntryPredecessor = nullptr;

                    bCanStartRequest = true;

                    // If the request has a predecessor the predecessor must first
                    // be finished before the request can be started.
                    if( pReqSeqEntry->m_iReqIdPredecessor >= 0 )
                    {
                        if( m_hshReqSeqs.contains(pReqSeqEntry->m_iReqIdPredecessor) )
                        {
                            pReqSeqEntryPredecessor = m_hshReqSeqs[pReqSeqEntry->m_iReqIdPredecessor];
                        }
                    }
                    if( pReqSeqEntryPredecessor != nullptr )
                    {
                        if( pReqSeqEntryPredecessor->m_reqDscr.m_objState != EObjState::Destroyed && pReqSeqEntryPredecessor->m_reqDscr.m_iProgress_perCent < 100 )
                        {
                            bCanStartRequest = false;
                        }
                    }

                    if( bCanStartRequest )
                    {
                        // All top level requests will get the same parent request id provided on starting the sequence.
                        pReq = startRequest(iTimeout_ms, bIsBlocking, m_iReqIdParent, iReqId);

                        if( isAsynchronousRequest(pReq) )
                        {
                            if( pReq->isBlockingRequest() )
                            {
                                if( !pReq->wait() )
                                {
                                    pReq->setResult(EResultTimeout);
                                    pReq->setProgressInPerCent(100);
                                    pReq->update();

                                    // The one who calls update for the finished request must delete the request.
                                    deleteRequest(pReq);
                                    pReq = nullptr;
                                }
                            }
                        } // if( isAsynchronousRequest(pReq) )
                    } // if( bCanStartRequest )
                } // if( pReqSeqEntry->m_reqDscr.m_iProgress_perCent < 100 )
            } // for( idxReq = m_ariReqIds.size()-1; idxReq >= 0; idxReq-- )

            m_bInProgress = !isSequenceFinished();

            if( !m_bInProgress && bInProgressPrev != m_bInProgress )
            {
                emit finished(m_iReqIdParent);
            }

        } // if( i_pEv->type() == EBaseMsgTypeReqContinue )
    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event
