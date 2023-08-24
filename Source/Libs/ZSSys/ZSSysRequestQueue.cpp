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

#include <QtCore/qthread.h>

#include "ZSSys/ZSSysRequestQueue.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
class CRequestQueue : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRequestQueue::CRequestQueue(
    const QString& i_strCreatorNameSpace,
    const QString& i_strCreatorClassName,
    QObject*       i_pObjParent,
    bool           i_bKeepReqDscrInExecTree ) :
//------------------------------------------------------------------------------
    QObject(i_pObjParent),
    m_strCreatorNameSpace(i_strCreatorNameSpace),
    m_strCreatorClassName(i_strCreatorClassName),
    m_pObjParent(i_pObjParent),
    m_bKeepReqDscrInExecTree(i_bKeepReqDscrInExecTree),
    m_iLockCount(0),
    m_pReqInProgress(nullptr),
    m_pReqSyncToBeDeletedLater(nullptr),
    m_hshReqsInProgress(),
    m_iReqsPostponedMaxEntries(1000),
    m_arpReqsPostponed()
{
    if( m_pObjParent == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pObjParent == nullptr" );
    }

    setObjectName(m_pObjParent->objectName());

} // ctor

//------------------------------------------------------------------------------
CRequestQueue::CRequestQueue(
    const QString& i_strObjName,
    const QString& i_strCreatorNameSpace,
    const QString& i_strCreatorClassName,
    QObject*       i_pObjParent,
    bool           i_bKeepReqDscrInExecTree ) :
//------------------------------------------------------------------------------
    QObject(i_pObjParent),
    m_strCreatorNameSpace(i_strCreatorNameSpace),
    m_strCreatorClassName(i_strCreatorClassName),
    m_pObjParent(i_pObjParent),
    m_bKeepReqDscrInExecTree(i_bKeepReqDscrInExecTree),
    m_iLockCount(0),
    m_pReqInProgress(nullptr),
    m_pReqSyncToBeDeletedLater(nullptr),
    m_hshReqsInProgress(),
    m_iReqsPostponedMaxEntries(1000),
    m_arpReqsPostponed()
{
    if( m_pObjParent == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pObjParent == nullptr" );
    }

    setObjectName(i_strObjName);

} // ctor

//------------------------------------------------------------------------------
CRequestQueue::CRequestQueue( QObject* i_pObjParent, bool i_bKeepReqDscrInExecTree ) :
//------------------------------------------------------------------------------
    QObject(i_pObjParent),
    m_strCreatorNameSpace(),
    m_strCreatorClassName(),
    m_pObjParent(i_pObjParent),
    m_bKeepReqDscrInExecTree(i_bKeepReqDscrInExecTree),
    m_iLockCount(0),
    m_pReqInProgress(nullptr),
    m_pReqSyncToBeDeletedLater(nullptr),
    m_hshReqsInProgress(),
    m_iReqsPostponedMaxEntries(1000),
    m_arpReqsPostponed()
{
    if( m_pObjParent == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "i_pObjParent == nullptr" );
    }

    setObjectName(m_pObjParent->objectName());

} // ctor

//------------------------------------------------------------------------------
CRequestQueue::~CRequestQueue()
//------------------------------------------------------------------------------
{
    if( m_pReqSyncToBeDeletedLater != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pReqSyncToBeDeletedLater,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestDestroyed(QObject*)) );

        try
        {
            delete m_pReqSyncToBeDeletedLater;
        }
        catch(...)
        {
        }
    } // if( m_pReqSyncToBeDeletedLater != nullptr )

    if( m_pReqInProgress != nullptr )
    {
        // If created by this request queue (and therefore owned by
        // parent object of this request queue) ..
        if( m_pReqInProgress->getCreator() == m_pObjParent )
        {
            QObject::disconnect(
                /* pObjSender   */ m_pReqInProgress,
                /* szSignal     */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestDestroyed(QObject*)) );

            // .. the request can be deleted together with the request queue.
            try
            {
                deleteRequest(m_pReqInProgress);
            }
            catch(...)
            {
            }
        }
    }

    if( m_arpReqsPostponed.size() > 0 )
    {
        CRequest* pReqPostponed;
        int       idxReq;

        for( idxReq = m_arpReqsPostponed.size()-1; idxReq >= 0; idxReq-- )
        {
            pReqPostponed = m_arpReqsPostponed[idxReq];
            m_arpReqsPostponed[idxReq] = nullptr;

            QObject::disconnect(
                /* pObjSender   */ pReqPostponed,
                /* szSignal     */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestDestroyed(QObject*)) );

            try
            {
                delete pReqPostponed;
            }
            catch(...)
            {
            }
            pReqPostponed = nullptr;
        }
    }

    //m_strCreatorNameSpace;
    //m_strCreatorClassName;
    m_pObjParent = nullptr;
    m_bKeepReqDscrInExecTree = false;
    m_iLockCount = 0;
    m_pReqInProgress = nullptr;
    m_pReqSyncToBeDeletedLater = nullptr;
    //m_hshReqsInProgress.clear();
    m_iReqsPostponedMaxEntries = 0;
    //m_arpReqsPostponed.clear();

} // dtor

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CRequestQueue::createRequest(
    int            i_request,
    const QString& i_strRequest,
    qint64         i_iReqIdParent,
    int            i_iTimeout_ms,
    bool           i_bIsBlocking ) const
//------------------------------------------------------------------------------
{
    CRequest* pReq = new CRequest(
        /* strNameSpace     */ m_strCreatorNameSpace,
        /* strClassName     */ m_strCreatorClassName,
        /* pObjCreator      */ m_pObjParent,
        /* iRequest         */ i_request,
        /* strRequest       */ i_strRequest,
        /* iReqIdParent     */ i_iReqIdParent,
        /* bKeepRequestDscr */ m_bKeepReqDscrInExecTree,
        /* iTimeout_ms      */ i_iTimeout_ms,
        /* bIsBlocking      */ i_bIsBlocking );

    // Don't move the request to the thread of the request queue.
    // Otherwise the changed signal will be queued and the "onRequestChanged"
    // slot may be called in the thread of the request queue after the
    // request has been destroyed. Request MUST be created in the thread context
    // of the request queue. For the trace server this means that "sendData"
    // may never become an asynchronous request.
    //if( m_pObjParent->thread() != QThread::currentThread() )
    //{
    //    pReq->moveToThread(m_pObjParent->thread());
    //}

    return pReq;

} // createRequest

//------------------------------------------------------------------------------
CRequest* CRequestQueue::createRequest(
    const QString& i_strCreatorNameSpace,
    const QString& i_strCreatorClassName,
    int            i_request,
    const QString& i_strRequest,
    qint64         i_iReqIdParent,
    int            i_iTimeout_ms,
    bool           i_bIsBlocking ) const
//------------------------------------------------------------------------------
{
    CRequest* pReq = new CRequest(
        /* strNameSpace     */ i_strCreatorNameSpace,
        /* strClassName     */ i_strCreatorClassName,
        /* pObjCreator      */ m_pObjParent,
        /* iRequest         */ i_request,
        /* strRequest       */ i_strRequest,
        /* iReqIdParent     */ i_iReqIdParent,
        /* bKeepRequestDscr */ m_bKeepReqDscrInExecTree,
        /* iTimeout_ms      */ i_iTimeout_ms,
        /* bIsBlocking      */ i_bIsBlocking );

    // Don't move the request to the thread of the request queue.
    // Otherwise the changed signal will be queued and the "onRequestChanged"
    // slot may be called in the thread of the request queue after the
    // request has been destroyed. Request MUST be created in the thread context
    // of the request queue. For the trace server this means that "sendData"
    // may never become an asynchronous request.
    //if( m_pObjParent->thread() != QThread::currentThread() )
    //{
    //    pReq->moveToThread(m_pObjParent->thread());
    //}

    return pReq;

} // createRequest

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
qint64 CRequestQueue::GetUniqueRequestId() const
//------------------------------------------------------------------------------
{
    return CRequest::GetUniqueRequestId();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestQueue::setCreatorNameSpace( const QString& i_strCreatorNameSpace )
//------------------------------------------------------------------------------
{
    m_strCreatorNameSpace = i_strCreatorNameSpace;
}

//------------------------------------------------------------------------------
void CRequestQueue::setCreatorClassName( const QString& i_strCreatorClassName )
//------------------------------------------------------------------------------
{
    m_strCreatorClassName = i_strCreatorClassName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestQueue::setKeepReqDscrInExecTree( bool i_bKeep )
//------------------------------------------------------------------------------
{
    m_bKeepReqDscrInExecTree = i_bKeep;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CRequestQueue::lock()
//------------------------------------------------------------------------------
{
    return ++m_iLockCount;
}

//------------------------------------------------------------------------------
int CRequestQueue::unlock()
//------------------------------------------------------------------------------
{
    if( m_iLockCount <= 0 )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInternalProgramError, "Lock Count is already 0" );
    }
    return --m_iLockCount;

} // unlock

/*==============================================================================
public: // instance methods (concerning the top post (main) request in progress)
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestQueue::setRequestInProgress( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    // There should not be any pending requests if the request is
    // going to be replaced. All pending requests should have already
    // been deleted on receiving the corresponding answers. If there
    // are pending requests on starting a new request they will be
    // considered as memory leaks (see dtor of CRequest).

    if( m_pReqInProgress != nullptr )
    {
        removeRequestInProgress();
    }

    m_pReqInProgress = i_pReq;

    QObject::connect(
        m_pReqInProgress, &QObject::destroyed,
        this, &CRequestQueue::onRequestDestroyed);

    if( m_pReqInProgress != nullptr )
    {
        m_hshReqsInProgress[m_pReqInProgress->getId()] = m_pReqInProgress;
    }

} // setRequestInProgress

//------------------------------------------------------------------------------
CRequest* CRequestQueue::takeRequestInProgress()
//------------------------------------------------------------------------------
{
    if( m_pReqInProgress == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultListIsEmpty );
    }

    CRequest* pReq = m_pReqInProgress;
    removeRequestInProgress(); // to report warnings if the request has not been processed completely.
    return pReq;

} // takeRequestInProgress

//------------------------------------------------------------------------------
void CRequestQueue::removeRequestInProgress()
//------------------------------------------------------------------------------
{
    if( m_pReqInProgress == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultListIsEmpty );
    }

    QObject::disconnect(
        /* pObjSender   */ m_pReqInProgress,
        /* szSignal     */ SIGNAL(destroyed(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestDestroyed(QObject*)) );

    m_hshReqsInProgress.remove(m_pReqInProgress->getId());

    m_pReqInProgress = nullptr;

    // There should not be any child requests in progress if the main (top most)
    // request is going to be removed. All child requests should have already
    // been deleted if they have been processed completely. If there are child
    // requests on removing the main request they will be considered as memory leaks.
    if( m_hshReqsInProgress.size() > 0 )
    {
        CRequest* pReqInProgress;
        QString   strAddErrInfo;

        QHash<qint64,CRequest*>::iterator itReq = m_hshReqsInProgress.begin();

        while( itReq != m_hshReqsInProgress.end() )
        {
            pReqInProgress = itReq.value();

            if( pReqInProgress != nullptr )
            {
                if( CErrLog::GetInstance() != nullptr )
                {
                    strAddErrInfo  = pReqInProgress->getAddTrcInfoStr(1);
                    strAddErrInfo += ": The request has not been removed from the request queue.";

                    SErrResultInfo errResultInfo(
                        /* errSource  */ pReqInProgress->getCreatorNameSpace(), pReqInProgress->getCreatorClassName(), pReqInProgress->getCreatorObjectName(), "",
                        /* result     */ EResultMemLeak,
                        /* severity   */ EResultSeverityWarning,
                        /* strAddInfo */ strAddErrInfo );
                    CErrLog::GetInstance()->addEntry(errResultInfo);

                } // if( CErrLog::GetInstance() != nullptr )
            } // if( pReqPending != nullptr )

            itReq = m_hshReqsInProgress.erase(itReq);

        } // while( itReq != m_hshReqsInProgress.end() )
    } // if( m_hshReqsInProgress.size() > 0 )

} // removeRequestInProgress

//------------------------------------------------------------------------------
bool CRequestQueue::isRequestInProgress() const
//------------------------------------------------------------------------------
{
    bool bInProgress = false;

    if( m_pReqInProgress != nullptr )
    {
        bInProgress = true;
    }
    return bInProgress;

} // isRequestInProgress

/*==============================================================================
public: // instance methods (concerning any (top post (main) and sub (child) requests in progress)
==============================================================================*/

//------------------------------------------------------------------------------
bool CRequestQueue::isIdle() const
//------------------------------------------------------------------------------
{
    bool bIsIdle = true;

    if( m_pReqInProgress != nullptr || m_iLockCount > 0 || m_arpReqsPostponed.count() > 0 )
    {
        bIsIdle = false;
    }
    return bIsIdle;

} // isIdle

/*==============================================================================
public: // instance methods (concerning any (top post (main) and sub (child) requests in progress)
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestQueue::addRequestInProgress( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    if( m_pReqInProgress == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultListIsEmpty );
    }
    if( m_hshReqsInProgress.contains(i_pReq->getId()) )
    {
        QString strAddErrInfo = i_pReq->getAddTrcInfoStr(1);
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }
    m_hshReqsInProgress[i_pReq->getId()] = i_pReq;

    QObject::connect(
        i_pReq, &QObject::destroyed,
        this, &CRequestQueue::onRequestDestroyed);

} // addRequestInProgress

//------------------------------------------------------------------------------
void CRequestQueue::removeRequestInProgress( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    removeRequestInProgress(i_pReq->getId());
}

//------------------------------------------------------------------------------
bool CRequestQueue::isRequestInProgress( CRequest* i_pReq ) const
//------------------------------------------------------------------------------
{
    bool bInProgress = false;

    if( i_pReq != nullptr )
    {
        bInProgress = m_hshReqsInProgress.contains(i_pReq->getId());
    }
    return bInProgress;

} // isRequestInProgress

/*==============================================================================
public: // instance methods (concerning any (top post (main) and sub (child) requests in progress)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CRequestQueue::getRequestInProgress( qint64 i_iId ) const
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    if( m_hshReqsInProgress.contains(i_iId) )
    {
        pReq = m_hshReqsInProgress[i_iId];
    }
    return pReq;

} // getRequestInProgress

//------------------------------------------------------------------------------
CRequest* CRequestQueue::takeRequestInProgress( qint64 i_iId )
//------------------------------------------------------------------------------
{
    if( !m_hshReqsInProgress.contains(i_iId) )
    {
        QString strAddErrInfo = QString::number(i_iId);
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
    }

    CRequest* pReq = m_hshReqsInProgress[i_iId];

    removeRequestInProgress(i_iId);

    return pReq;

} // takeRequestInProgress

//------------------------------------------------------------------------------
void CRequestQueue::removeRequestInProgress( qint64 i_iId )
//------------------------------------------------------------------------------
{
    if( !m_hshReqsInProgress.contains(i_iId) )
    {
        QString strAddErrInfo = QString::number(i_iId);
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjNotInList, strAddErrInfo );
    }

    CRequest* pReq = m_hshReqsInProgress[i_iId];

    // Is it the main (top most) request ..
    if( pReq == m_pReqInProgress )
    {
        removeRequestInProgress();
    }
    else // if( pReq != m_pReqInProgress )
    {
        QObject::disconnect(
            /* pObjSender   */ pReq,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestDestroyed(QObject*)) );

        m_hshReqsInProgress.remove(i_iId);
    }

} // removeRequestInProgress

//------------------------------------------------------------------------------
bool CRequestQueue::isRequestInProgress( qint64 i_iId ) const
//------------------------------------------------------------------------------
{
    return m_hshReqsInProgress.contains(i_iId);
}

/*==============================================================================
public: // instance methods (concerning any (top post (main) and sub (child) requests in progress)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CRequestQueue::getRequestInProgressByType( int i_request ) const
//------------------------------------------------------------------------------
{
    QHash<qint64,CRequest*>::const_iterator itReq = m_hshReqsInProgress.begin();

    CRequest* pReq = nullptr;

    while( itReq != m_hshReqsInProgress.end() )
    {
        CRequest* pReqTmp = itReq.value();

        if( pReqTmp != nullptr && pReqTmp->getRequest() == i_request )
        {
            pReq = pReqTmp;
        }
        itReq++;

    } // while( itReq != m_hshReqsInProgress.end() )

    return pReq;

} // getRequestInProgressByType

//------------------------------------------------------------------------------
CRequest* CRequestQueue::takeRequestInProgressByType( int i_request )
//------------------------------------------------------------------------------
{
    QHash<qint64,CRequest*>::const_iterator itReq = m_hshReqsInProgress.begin();

    CRequest* pReq = nullptr;

    while( itReq != m_hshReqsInProgress.end() )
    {
        CRequest* pReqTmp = itReq.value();

        if( pReqTmp != nullptr && pReqTmp->getRequest() == i_request )
        {
            pReq = pReqTmp;
        }
        itReq++;

    } // while( itReq != m_hshReqsInProgress.end() )

    if( pReq != nullptr )
    {
        removeRequestInProgress(pReq);
    }
    return pReq;

} // takeRequestInProgressByType

//------------------------------------------------------------------------------
void CRequestQueue::removeRequestInProgressByType( int i_request )
//------------------------------------------------------------------------------
{
    QHash<qint64,CRequest*>::const_iterator itReq = m_hshReqsInProgress.begin();

    CRequest* pReq = nullptr;

    while( itReq != m_hshReqsInProgress.end() )
    {
        CRequest* pReqTmp = itReq.value();

        if( pReqTmp != nullptr && pReqTmp->getRequest() == i_request )
        {
            pReq = pReqTmp;
        }
        itReq++;

    } // while( itReq != m_hshReqsInProgress.end() )

    if( pReq != nullptr )
    {
        removeRequestInProgress(pReq);
    }

} // removeRequestInProgressByType

//------------------------------------------------------------------------------
bool CRequestQueue::isRequestInProgressByType( int i_request ) const
//------------------------------------------------------------------------------
{
    QHash<qint64,CRequest*>::const_iterator itReq = m_hshReqsInProgress.begin();

    CRequest* pReq = nullptr;

    while( itReq != m_hshReqsInProgress.end() )
    {
        CRequest* pReqTmp = itReq.value();

        if( pReqTmp != nullptr && pReqTmp->getRequest() == i_request )
        {
            pReq = pReqTmp;
        }
        itReq++;

    } // while( itReq != m_hshReqsInProgress.end() )

    return (pReq != nullptr);

} // isRequestInProgressByType

/*==============================================================================
public: // instance methods (concerning any (top post (main) and sub (child) requests in progress)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CRequestQueue::findRequestById(
    qint64 i_iId,
    bool   i_bIgnorePostponedRequests,
    bool*  o_pbIsPostponedRequest,
    int*   o_pidxPostponedRequest ) const
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    bool bIsPostponed = false;
    int  idxReqPostponed = -1;

    if( m_hshReqsInProgress.contains(i_iId) )
    {
        pReq = m_hshReqsInProgress[i_iId];
    }
    else if( !i_bIgnorePostponedRequests )
    {
        CRequest* pReqTmp;
        int       idxReq;

        for( idxReq = 0; idxReq < m_arpReqsPostponed.size(); idxReq++ )
        {
            pReqTmp = m_arpReqsPostponed[idxReq];

            if( pReqTmp != nullptr && pReqTmp->getId() == i_iId )
            {
                pReq = pReqTmp;
                bIsPostponed = true;
                idxReqPostponed = idxReq;
                break;
            }
        }
    } // if( !i_bIgnorePostponedRequests )

    if( o_pbIsPostponedRequest != nullptr )
    {
        *o_pbIsPostponedRequest = bIsPostponed;
    }
    if( o_pidxPostponedRequest != nullptr )
    {
        *o_pidxPostponedRequest = idxReqPostponed;
    }

    return pReq;

} // findRequestById

//------------------------------------------------------------------------------
QList<CRequest*> CRequestQueue::findRequestsByType( int i_request, bool i_bIgnorePostponedRequests ) const
//------------------------------------------------------------------------------
{
    QList<CRequest*> arpReqs;

    QHash<qint64,CRequest*>::const_iterator itReq = m_hshReqsInProgress.begin();

    CRequest* pReq;

    while( itReq != m_hshReqsInProgress.end() )
    {
        pReq = itReq.value();

        if( pReq != nullptr && pReq->getRequest() == i_request )
        {
            arpReqs.append(pReq);
        }
        itReq++;

    } // while( itReq != m_hshReqsInProgress.end() )

    if( !i_bIgnorePostponedRequests )
    {
        int idxReq;

        for( idxReq = 0; idxReq < m_arpReqsPostponed.size(); idxReq++ )
        {
            pReq = m_arpReqsPostponed[idxReq];

            if( pReq != nullptr && pReq->getRequest() == i_request )
            {
                arpReqs.append(pReq);
                break;
            }
        }
    } // if( !i_bIgnorePostponedRequests )

    return arpReqs;

} // findRequestsByType

//------------------------------------------------------------------------------
QList<CRequest*> CRequestQueue::findRequestsByType(
    int            i_request,
    const QString& i_strCreatorObjName,
    const QString& i_strCreatorClassName,
    const QString& i_strCreatorNameSpace,
    bool           i_bIgnorePostponedRequests ) const
//------------------------------------------------------------------------------
{
    QList<CRequest*> arpReqs;

    QHash<qint64,CRequest*>::const_iterator itReq;

    CRequest* pReq;

    for( itReq = m_hshReqsInProgress.begin(); itReq != m_hshReqsInProgress.end(); itReq++ )
    {
        pReq = itReq.value();

        if( i_request >= 0 && pReq->getRequest() != i_request )
        {
            pReq = nullptr;
        }
        else if( !i_strCreatorObjName.isEmpty() && pReq->getCreatorObjectName().compare(i_strCreatorObjName,Qt::CaseInsensitive) != 0 )
        {
            pReq = nullptr;
        }
        else if( !i_strCreatorClassName.isEmpty() && pReq->getCreatorClassName().compare(i_strCreatorClassName,Qt::CaseInsensitive) != 0 )
        {
            pReq = nullptr;
        }
        else if( !i_strCreatorNameSpace.isEmpty() && pReq->getCreatorClassName().compare(i_strCreatorNameSpace,Qt::CaseInsensitive) != 0 )
        {
            pReq = nullptr;
        }
        if( pReq != nullptr )
        {
            arpReqs.append(pReq);
        }
    } // for( itReq = m_hshReqsInProgress.begin(); itReq != m_hshReqsInProgress.end(); itReq++ )

    if( !i_bIgnorePostponedRequests )
    {
        int idxReq;

        for( idxReq = 0; idxReq < m_arpReqsPostponed.size(); idxReq++ )
        {
            pReq = m_arpReqsPostponed[idxReq];

            if( pReq != nullptr )
            {
                if( i_request >= 0 && pReq->getRequest() != i_request )
                {
                    pReq = nullptr;
                }
                else if( !i_strCreatorObjName.isEmpty() && pReq->getCreatorObjectName().compare(i_strCreatorObjName,Qt::CaseInsensitive) != 0 )
                {
                    pReq = nullptr;
                }
                else if( !i_strCreatorClassName.isEmpty() && pReq->getCreatorClassName().compare(i_strCreatorClassName,Qt::CaseInsensitive) != 0 )
                {
                    pReq = nullptr;
                }
                else if( !i_strCreatorNameSpace.isEmpty() && pReq->getCreatorClassName().compare(i_strCreatorNameSpace,Qt::CaseInsensitive) != 0 )
                {
                    pReq = nullptr;
                }
                if( pReq != nullptr )
                {
                    arpReqs.append(pReq);
                }
                arpReqs.append(pReq);
            }
        } // for( idxReq = 0; idxReq < m_arpReqsPostponed.size(); idxReq++ )
    } // if( !i_bIgnorePostponedRequests )

    return arpReqs;

} // findRequestsByType

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestQueue::setSyncRequestToBeDeletedLater( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    if( m_pReqSyncToBeDeletedLater != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pReqSyncToBeDeletedLater,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestDestroyed(QObject*)) );

        if( m_pReqSyncToBeDeletedLater != i_pReq )
        {
            deleteRequest(m_pReqSyncToBeDeletedLater);
        }
    }

    m_pReqSyncToBeDeletedLater = i_pReq;

    // This method may be explicitly called with "i_pReq == nullptr" to
    // delete the stored request. This is useful during shutdown if
    // the queue would be destroyed after the request exec tree.
    // E.g. if the request queue has not been created with the new operator.
    if( m_pReqSyncToBeDeletedLater != nullptr )
    {
        QObject::connect(
            m_pReqSyncToBeDeletedLater, &QObject::destroyed,
            this, &CRequestQueue::onRequestDestroyed,
            Qt::DirectConnection);

        m_pReqSyncToBeDeletedLater->setObjState(EObjState::Detached);
        m_pReqSyncToBeDeletedLater->update();
    }

} // setSyncRequestToBeDeletedLater

/*==============================================================================
public: // instance methods (postponing requests)
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestQueue::setMaxPostponedRequestEntries( int i_iMaxEntries )
//------------------------------------------------------------------------------
{
    m_iReqsPostponedMaxEntries = i_iMaxEntries;
}

//------------------------------------------------------------------------------
EResult CRequestQueue::postponeRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    EResult result = EResultSuccess;

    if( m_arpReqsPostponed.size() < m_iReqsPostponedMaxEntries )
    {
        QObject::connect(
            i_pReq, &QObject::destroyed,
            this, &CRequestQueue::onRequestDestroyed);

        m_arpReqsPostponed.append(i_pReq);
    }
    else
    {
        result = EResultListIsFull;
    }
    return result;

} // postponeRequest

//------------------------------------------------------------------------------
CRequest* CRequestQueue::getPostponedRequestByIdx( int i_idx ) const
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    if( i_idx >= 0 && i_idx < m_arpReqsPostponed.size() )
    {
        pReq = m_arpReqsPostponed[i_idx];
    }
    return pReq;

} // getPostponedRequestByIdx

//------------------------------------------------------------------------------
CRequest* CRequestQueue::takePostponedRequestByIdx( int i_idx )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    if( i_idx >= 0 && i_idx < m_arpReqsPostponed.size() )
    {
        pReq = m_arpReqsPostponed.takeAt(i_idx);

        QObject::disconnect(
            /* pObjSender   */ pReq,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestDestroyed(QObject*)) );
    }

    return pReq;

} // takePostponedRequestByIdx

//------------------------------------------------------------------------------
CRequest* CRequestQueue::getFirstPostponedRequest()
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    if( m_arpReqsPostponed.size() > 0 )
    {
        pReq = m_arpReqsPostponed[0];
    }
    return pReq;

} // getFirstPostponedRequest

//------------------------------------------------------------------------------
CRequest* CRequestQueue::takeFirstPostponedRequest()
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    if( m_arpReqsPostponed.size() > 0 )
    {
        pReq = m_arpReqsPostponed.takeFirst();

        QObject::disconnect(
            /* pObjSender   */ pReq,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestDestroyed(QObject*)) );
    }

    return pReq;

} // takeFirstPostponedRequest

//------------------------------------------------------------------------------
CRequest* CRequestQueue::getLastPostponedRequest()
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    if( m_arpReqsPostponed.size() > 0 )
    {
        pReq = m_arpReqsPostponed[m_arpReqsPostponed.size()-1];
    }
    return pReq;

} // getLastPostponedRequest

//------------------------------------------------------------------------------
CRequest* CRequestQueue::takeLastPostponedRequest()
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    if( m_arpReqsPostponed.size() > 0 )
    {
        pReq = m_arpReqsPostponed.takeLast();

        QObject::disconnect(
            /* pObjSender   */ pReq,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestDestroyed(QObject*)) );
    }

    return pReq;

} // takeLastPostponedRequest

//------------------------------------------------------------------------------
CRequest* CRequestQueue::findPostponedRequestById( qint64 i_iId, int* o_pidx ) const
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;
    int       idxReqPostponed = -1;
    CRequest* pReqTmp;
    int       idxReq;

    for( idxReq = 0; idxReq < m_arpReqsPostponed.size(); idxReq++ )
    {
        pReqTmp = m_arpReqsPostponed[idxReq];

        if( pReqTmp != nullptr && pReqTmp->getId() == i_iId )
        {
            pReq = pReqTmp;
            idxReqPostponed = idxReq;
            break;
        }
    }
    if( o_pidx != nullptr )
    {
        *o_pidx = idxReqPostponed;
    }
    return pReq;

} // findPostponedRequestById

//------------------------------------------------------------------------------
CRequest* CRequestQueue::takePostponedRequestById( qint64 i_iId )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;
    int       idxReqPostponed = -1;
    CRequest* pReqTmp;
    int       idxReq;

    for( idxReq = 0; idxReq < m_arpReqsPostponed.size(); idxReq++ )
    {
        pReqTmp = m_arpReqsPostponed[idxReq];

        if( pReqTmp != nullptr && pReqTmp->getId() == i_iId )
        {
            idxReqPostponed = idxReq;
            break;
        }
    }
    if( idxReqPostponed >= 0 )
    {
        pReq = takePostponedRequestByIdx(idxReqPostponed);
    }
    return pReq;

} // takePostponedRequestById

//------------------------------------------------------------------------------
void CRequestQueue::removePostponedRequestByIdx( int i_idx )
//------------------------------------------------------------------------------
{
    if( i_idx >= 0 && i_idx < m_arpReqsPostponed.size() )
    {
        CRequest* pReq = m_arpReqsPostponed[i_idx];

        QObject::disconnect(
            /* pObjSender   */ pReq,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestDestroyed(QObject*)) );

        m_arpReqsPostponed.removeAt(i_idx);

     } // if( i_idx >= 0 && i_idx < m_arpReqsPostponed.size() )

} // removePostponedRequestByIdx

//------------------------------------------------------------------------------
void CRequestQueue::removePostponedRequestById( qint64 i_iId )
//------------------------------------------------------------------------------
{
    CRequest* pReqTmp;
    int       idxReq;
    int       idxReqPostponed = -1;

    for( idxReq = 0; idxReq < m_arpReqsPostponed.size(); idxReq++ )
    {
        pReqTmp = m_arpReqsPostponed[idxReq];

        if( pReqTmp != nullptr && pReqTmp->getId() == i_iId )
        {
            idxReqPostponed = idxReq;
            break;
        }
    }

    if( idxReqPostponed >= 0 )
    {
        CRequest* pReq = m_arpReqsPostponed[idxReqPostponed];

        QObject::disconnect(
            /* pObjSender   */ pReq,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestDestroyed(QObject*)) );

        m_arpReqsPostponed.removeAt(idxReqPostponed);
    }

} // removePostponedRequestById

//------------------------------------------------------------------------------
void CRequestQueue::removePostponedRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    CRequest* pReqTmp;
    int       idxReq;
    int       idxReqPostponed = -1;

    for( idxReq = 0; idxReq < m_arpReqsPostponed.size(); idxReq++ )
    {
        pReqTmp = m_arpReqsPostponed[idxReq];

        if( pReqTmp != nullptr && pReqTmp == i_pReq )
        {
            idxReqPostponed = idxReq;
            break;
        }
    }

    if( idxReqPostponed >= 0 )
    {
        CRequest* pReq = m_arpReqsPostponed[idxReqPostponed];

        QObject::disconnect(
            /* pObjSender   */ pReq,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestDestroyed(QObject*)) );

        m_arpReqsPostponed.removeAt(idxReqPostponed);
    }

} // removePostponedRequest

//------------------------------------------------------------------------------
bool CRequestQueue::isPostponedRequest( CRequest* i_pReq ) const
//------------------------------------------------------------------------------
{
    bool      bIsPostponed = false;
    CRequest* pReq;
    int       idxReq;

    for( idxReq = 0; idxReq < m_arpReqsPostponed.size(); idxReq++ )
    {
        pReq = m_arpReqsPostponed[idxReq];

        if( pReq != nullptr && pReq == i_pReq )
        {
            bIsPostponed = true;
            break;
        }
    }
    return bIsPostponed;

} // isPostponedRequest

//------------------------------------------------------------------------------
bool CRequestQueue::isPostponedRequest( qint64 i_iId ) const
//------------------------------------------------------------------------------
{
    bool      bIsPostponed = false;
    CRequest* pReq;
    int       idxReq;

    for( idxReq = 0; idxReq < m_arpReqsPostponed.size(); idxReq++ )
    {
        pReq = m_arpReqsPostponed[idxReq];

        if( pReq != nullptr && pReq->getId() == i_iId )
        {
            bIsPostponed = true;
            break;
        }
    }
    return bIsPostponed;

} // isPostponedRequest

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CRequestQueue::getAddTrcInfoStr( int i_iDetailLevel ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    strAddTrcInfo = "ReqInProgress: ";

    if( m_pReqInProgress == nullptr )
    {
        strAddTrcInfo += "nullptr";
    }
    else // if( m_pReqInProgress != nullptr )
    {
        if( i_iDetailLevel >= 2 )
        {
            if( !m_pReqInProgress->getCreatorNameSpace().isEmpty() || !m_pReqInProgress->getCreatorClassName().isEmpty() || !m_pReqInProgress->getCreatorObjectName().isEmpty() )
            {
                if( !m_pReqInProgress->getCreatorNameSpace().isEmpty() )
                {
                    strAddTrcInfo += m_pReqInProgress->getCreatorNameSpace();
                    if( !m_pReqInProgress->getCreatorClassName().isEmpty() )
                    {
                        strAddTrcInfo += "::";
                    }
                }
                if( !m_pReqInProgress->getCreatorNameSpace().isEmpty() )
                {
                    strAddTrcInfo += m_pReqInProgress->getCreatorClassName();
                    if( !m_pReqInProgress->getCreatorObjectName().isEmpty() )
                    {
                        strAddTrcInfo += ":";
                    }
                }
                if( !m_pReqInProgress->getCreatorObjectName().isEmpty() )
                {
                    strAddTrcInfo += m_pReqInProgress->getCreatorObjectName();
                    strAddTrcInfo += ".";
                }
            }
        }
        else if( i_iDetailLevel >= 1 )
        {
            if( !m_pReqInProgress->getCreatorObjectName().isEmpty() )
            {
                strAddTrcInfo += m_pReqInProgress->getCreatorObjectName() + ".";
            }
        }
        strAddTrcInfo += m_pReqInProgress->getRequestName();

        if( i_iDetailLevel >= 1 )
        {
            strAddTrcInfo += " (Id: " + QString::number(m_pReqInProgress->getId());
            strAddTrcInfo += ", Progress: " + QString::number(m_pReqInProgress->getProgressInPerCent());
            strAddTrcInfo += ", Result: " + m_pReqInProgress->getResultStr() + ")";
        }
    } // if( m_pReqInProgress != nullptr )

    // Please note that the request in progress (main request) is already referenced
    // in "m_hshReqsInProgress". This is not a child request.
    if( m_hshReqsInProgress.size() > 1 )
    {
        strAddTrcInfo += ", ChildReqsInProgress [" + QString::number(m_hshReqsInProgress.size()-1) + "]";

        if( i_iDetailLevel >= 1 )
        {
            CRequest* pReqInProgress;

            QString strAddTrcInfoChildReq;

            QHash<qint64,CRequest*>::const_iterator itReq = m_hshReqsInProgress.begin();

            while( itReq != m_hshReqsInProgress.end() )
            {
                pReqInProgress = itReq.value();

                // The main (top most) request has already been reported.
                if( pReqInProgress != m_pReqInProgress )
                {
                    if( !strAddTrcInfoChildReq.isEmpty() )
                    {
                        strAddTrcInfoChildReq += ", ";
                    }

                    if( pReqInProgress == nullptr )
                    {
                        strAddTrcInfoChildReq += "nullptr";
                    }
                    else // if( pReqInProgress != nullptr )
                    {
                        if( i_iDetailLevel >= 2 )
                        {
                            // If the same request has several pending requests the request will be reported several times.
                            if( !pReqInProgress->getCreatorNameSpace().isEmpty() || !pReqInProgress->getCreatorClassName().isEmpty() || !pReqInProgress->getCreatorObjectName().isEmpty() )
                            {
                                if( !pReqInProgress->getCreatorNameSpace().isEmpty() )
                                {
                                    strAddTrcInfoChildReq += pReqInProgress->getCreatorNameSpace();
                                    if( !pReqInProgress->getCreatorClassName().isEmpty() )
                                    {
                                        strAddTrcInfoChildReq += "::";
                                    }
                                }
                                if( !pReqInProgress->getCreatorNameSpace().isEmpty() )
                                {
                                    strAddTrcInfoChildReq += pReqInProgress->getCreatorClassName();
                                    if( !pReqInProgress->getCreatorObjectName().isEmpty() )
                                    {
                                        strAddTrcInfoChildReq += ":";
                                    }
                                }
                                if( !pReqInProgress->getCreatorObjectName().isEmpty() )
                                {
                                    strAddTrcInfoChildReq += pReqInProgress->getCreatorObjectName();
                                    strAddTrcInfoChildReq += ".";
                                }
                            }
                        }
                        else if( i_iDetailLevel >= 1 )
                        {
                            if( !pReqInProgress->getCreatorObjectName().isEmpty() )
                            {
                                strAddTrcInfoChildReq += pReqInProgress->getCreatorObjectName() + ".";
                            }
                        }
                        strAddTrcInfoChildReq += pReqInProgress->getRequestName();
                        strAddTrcInfoChildReq += " (Id: " + QString::number(pReqInProgress->getId()) + ")";

                    } // if( pReqInProgress != nullptr )
                } // if( pReqInProgress != m_pReqInProgress )

                itReq++;

            } // while( itReq != m_hshReqsInProgress.end() )

            strAddTrcInfo += "{" + strAddTrcInfoChildReq + "}";

        } // if( i_iDetailLevel >= 1 )
    } // if( m_hshReqsInProgress.size() > 0 )

    strAddTrcInfo += ", PostponedReqs [" + QString::number(m_arpReqsPostponed.size()) + "]";

    if( m_arpReqsPostponed.size() > 0 )
    {
        if( i_iDetailLevel >= 1 && m_arpReqsPostponed.size() > 0 )
        {
            CRequest* pReqPostponed;
            int       idxReq;

            strAddTrcInfo += "{";

            for( idxReq = 0; idxReq < m_arpReqsPostponed.size(); idxReq++ )
            {
                pReqPostponed = m_arpReqsPostponed[idxReq];

                if( idxReq > 0 )
                {
                    strAddTrcInfo += ", ";
                }

                if( pReqPostponed == nullptr )
                {
                    strAddTrcInfo += "nullptr";
                }
                else // if( pReqPostponed != nullptr )
                {
                    if( i_iDetailLevel >= 2 )
                    {
                        if( !pReqPostponed->getCreatorNameSpace().isEmpty() || !pReqPostponed->getCreatorClassName().isEmpty() || !pReqPostponed->getCreatorObjectName().isEmpty() )
                        {
                            if( !pReqPostponed->getCreatorNameSpace().isEmpty() )
                            {
                                strAddTrcInfo += pReqPostponed->getCreatorNameSpace();
                                if( !pReqPostponed->getCreatorClassName().isEmpty() )
                                {
                                    strAddTrcInfo += "::";
                                }
                            }
                            if( !pReqPostponed->getCreatorNameSpace().isEmpty() )
                            {
                                strAddTrcInfo += pReqPostponed->getCreatorClassName();
                                if( !pReqPostponed->getCreatorObjectName().isEmpty() )
                                {
                                    strAddTrcInfo += ":";
                                }
                            }
                            if( !pReqPostponed->getCreatorObjectName().isEmpty() )
                            {
                                strAddTrcInfo += pReqPostponed->getCreatorObjectName();
                                strAddTrcInfo += ".";
                            }
                        }
                    }
                    else if( i_iDetailLevel >= 1 )
                    {
                        if( !pReqPostponed->getCreatorObjectName().isEmpty() )
                        {
                            strAddTrcInfo += pReqPostponed->getCreatorObjectName() + ".";
                        }
                    }
                    strAddTrcInfo += pReqPostponed->getRequestName();
                    strAddTrcInfo += " (Id: " + QString::number(pReqPostponed->getId()) + ")";

                } // if( pReqPostponed != nullptr )
            } // for( idxReq = 0; idxReq < m_arpReqsPostponed.size(); idxReq++ )

            strAddTrcInfo += "}";

        } // if( i_iDetailLevel >= 1 )
    } // if( m_arpReqsPostponed.size() > 0 )

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestQueue::onRequestDestroyed( QObject* i_pReq )
//------------------------------------------------------------------------------
{
    if( i_pReq == m_pReqInProgress )
    {
        m_pReqInProgress = nullptr;
    }
    else if( i_pReq == m_pReqSyncToBeDeletedLater )
    {
        m_pReqSyncToBeDeletedLater = nullptr;
    }
    else if( m_arpReqsPostponed.contains(reinterpret_cast<CRequest*>(i_pReq)) )
    {
        m_arpReqsPostponed.removeOne(reinterpret_cast<CRequest*>(i_pReq));
    }
    else
    {
        // If object is destroyed dyncamic_cast to CRequest* is no longer possible.
        QHash<qint64,CRequest*>::iterator itReq;

        for( itReq = m_hshReqsInProgress.begin(); itReq != m_hshReqsInProgress.end(); itReq++ )
        {
            if( itReq.value() == i_pReq )
            {
                m_hshReqsInProgress.remove( itReq.key() );
                break;
            }
        }
    }

} // onRequestDestroyed
