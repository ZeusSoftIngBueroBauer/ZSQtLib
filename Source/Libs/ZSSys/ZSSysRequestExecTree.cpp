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

#include <QtCore/QCoreApplication>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QTimer>
#include <QtCore/QThread>

#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
class CRequestExecTree : public QObject
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

CRequestExecTree* CRequestExecTree::s_pInstance = nullptr;

QMutex CRequestExecTree::s_mtx(QMutex::Recursive);

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequestExecTree* CRequestExecTree::CreateInstance( QObject* i_pObjParent )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( s_pInstance == nullptr )
    {
        s_pInstance = new CRequestExecTree(i_pObjParent);

        // Moving the instance to another thread is not a good idea.
        // The garbage collection timer has already been created and started.
        // If created in another thread than the main thread the instance will
        // also very likely be deleted in this thread. But the garbage collection
        // timer cannot be stopped than in the context of this thread.

        //if( QCoreApplication::instance() != nullptr )
        //{
        //    if( s_pInstance->thread() != QCoreApplication::instance()->thread() )
        //    {
        //        s_pInstance->moveToThread( QCoreApplication::instance()->thread() );
        //    }
        //}
    }

    return s_pInstance;

} // CreateInstance

//------------------------------------------------------------------------------
CRequestExecTree* CRequestExecTree::GetInstance()
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    return s_pInstance;
}

//------------------------------------------------------------------------------
void CRequestExecTree::DestroyInstance()
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( s_pInstance == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultSingletonClassNotInstantiated, "CRequestExecTree");
    }

    delete s_pInstance;
    s_pInstance = nullptr;

} // DestroyInstance

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CRequestExecTree::GetAddTrcInfoStr( qint64 i_iId, int i_iDetailLevel )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CRequestExecTree::GetInstance()->lock();

    SRequestDscr* pReqDscr = CRequestExecTree::GetInstance()->findRequest(i_iId);

    if( pReqDscr == nullptr )
    {
        strAddTrcInfo = "nullptr {Id:" + QString::number(i_iId) + "}";
    }
    else
    {
        strAddTrcInfo = pReqDscr->getAddTrcInfoStr(i_iDetailLevel);
    }

    CRequestExecTree::GetInstance()->unlock();

    return strAddTrcInfo;

} // getAddTrcInfoStr

/*==============================================================================
protected: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
CRequestExecTree::CRequestExecTree( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QObject(i_pObjParent),
    m_pMtx(nullptr),
    m_hshRequestDscrs(),
    m_hshRequests(),
    m_bTmrGarbageCollectorEnabled(true),
    m_fTmrGarbageCollecterInterval_s(5.0),
    m_fTmrGarbageCollectorElapsed_s(60.0),
    m_pTmrGarbageCollector(nullptr)
{
    // ctor is protected as the request execution tree should
    // only be created via class method "CreateInstance".
    //if( s_pInstance != nullptr )
    //{
    //    throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated, "CRequestExecTree" );
    //}
    //s_pInstance = this;

    #pragma push_macro("_SMSYSDBGNEW_CLIENT_BLOCK_SUBTYPE")
    #ifdef _WINDOWS
    #pragma warning( disable : 4005 )
    #endif
    #define _SMSYSDBGNEW_CLIENT_BLOCK_SUBTYPE 0

    setObjectName("theRequestExecutionTree");

    // The list of requests must be protected as creating and deleting
    // requests might be called from within different thread contexts.
    // On emitting signals the mutex is still very likely locked.
    // Receivers of the signal may call e.g. "findRequest" as a reentry.
    // So we need to use a recursive mutex to allow the same thread to access
    // the list of requess (at least to find a request item).
    m_pMtx = new QMutex(QMutex::Recursive);

    m_pTmrGarbageCollector = new QTimer(this);

    if( m_bTmrGarbageCollectorEnabled )
    {
        m_pTmrGarbageCollector->start( static_cast<int>(m_fTmrGarbageCollecterInterval_s*1000) );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrGarbageCollector,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTmrGarbageCollectorTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    #ifdef _WINDOWS
    #pragma warning( default : 4005 )
    #endif
    #pragma pop_macro("_SMSYSDBGNEW_CLIENT_BLOCK_SUBTYPE")

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRequestExecTree::~CRequestExecTree()
//------------------------------------------------------------------------------
{
    SRequestDscr* pReqDscr;

    QHash<qint64,SRequestDscr*>::iterator it;

    for( it = m_hshRequestDscrs.begin(); it != m_hshRequestDscrs.end(); it++ )
    {
        pReqDscr = it.value();

        if( pReqDscr->m_objState != EObjState::Destroyed )
        {
            QString strAddErrInfo = "Request {" + pReqDscr->getAddTrcInfoStr(2) + "} has not been destroyed.";
            SErrResultInfo errResultInfo(
                /* errSource  */ nameSpace(), className(), objectName(), "dtor",
                /* result     */ EResultMemLeak,
                /* severtiy   */ EResultSeverityError,
                /* strAddInfo */ strAddErrInfo );
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    clear();

    try
    {
        delete m_pMtx;
    }
    catch(...)
    {
    }

    QObject::disconnect(
        /* pObjSender   */ m_pTmrGarbageCollector,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTmrGarbageCollectorTimeout()) );

    m_pTmrGarbageCollector->stop();

    try
    {
        delete m_pTmrGarbageCollector;
    }
    catch(...)
    {
    }


    m_pMtx = nullptr;
    //m_hshRequestDscrs.clear();
    //m_hshRequests.clear();
    m_bTmrGarbageCollectorEnabled = false;
    m_fTmrGarbageCollecterInterval_s = 0.0;
    m_fTmrGarbageCollectorElapsed_s = 0.0;
    m_pTmrGarbageCollector = nullptr;

    s_pInstance = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestExecTree::addRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_hshRequests.contains(i_pReq->getId()) )
    {
        QString strAddErrInfo = "Request " + i_pReq->getAddTrcInfoStr() + " already added to request execution tree.";
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }

    SRequestDscr  reqDscr = i_pReq->getDescriptor();
    SRequestDscr* pReqDscr = new SRequestDscr(reqDscr);
    SRequestDscr* pReqDscrParent = nullptr;

    m_hshRequestDscrs.insert( i_pReq->getId(), pReqDscr );
    m_hshRequests.insert( i_pReq->getId(), i_pReq );

    if( i_pReq->getParentId() >= 0 )
    {
        if( m_hshRequests.contains(i_pReq->getParentId()) )
        {
            CRequest* pReqParent = m_hshRequests[i_pReq->getParentId()];

            if( pReqParent->m_dscr.m_ariChildIds.indexOf(i_pReq->getId()) < 0 )
            {
                pReqParent->m_dscr.m_ariChildIds.append(i_pReq->getId());
            }
        }

        pReqDscrParent = findRequest(i_pReq->getParentId());

        if( pReqDscrParent != nullptr )
        {
            pReqDscrParent->m_ariChildIds.append(pReqDscr->m_iId);
        }
    }

    emit requestAdded(reqDscr);

} // addRequest

//------------------------------------------------------------------------------
void CRequestExecTree::addRequest( const SRequestDscr& i_reqDscr )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_hshRequestDscrs.contains(i_reqDscr.m_iId) )
    {
        QString strAddErrInfo = "Request " + i_reqDscr.getAddTrcInfoStr() + " already added to request execution tree.";
        throw ZS::System::CException( __FILE__, __LINE__, EResultObjAlreadyInList, strAddErrInfo );
    }

    SRequestDscr* pReqDscr = new SRequestDscr(i_reqDscr);
    SRequestDscr* pReqDscrParent = nullptr;

    m_hshRequestDscrs.insert( i_reqDscr.m_iId, pReqDscr );

    if( i_reqDscr.m_iParentId >= 0 )
    {
        if( m_hshRequests.contains(i_reqDscr.m_iParentId) )
        {
            CRequest* pReqParent = m_hshRequests[i_reqDscr.m_iParentId];

            if( pReqParent->m_dscr.m_ariChildIds.indexOf(i_reqDscr.m_iId) < 0 )
            {
                pReqParent->m_dscr.m_ariChildIds.append(i_reqDscr.m_iId);
            }
        }

        pReqDscrParent = findRequest(i_reqDscr.m_iParentId);

        if( pReqDscrParent != nullptr )
        {
            pReqDscrParent->m_ariChildIds.append(pReqDscr->m_iId);
        }
    }

    emit requestAdded(i_reqDscr);

} // addRequest

//------------------------------------------------------------------------------
void CRequestExecTree::updateRequest( const SRequestDscr& i_reqDscr )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bChanged = false;

    if( m_hshRequestDscrs.contains(i_reqDscr.m_iId) )
    {
        SRequestDscr* pReqDscr = m_hshRequestDscrs[i_reqDscr.m_iId];

        if( *pReqDscr != i_reqDscr )
        {
            *pReqDscr = i_reqDscr;

            bChanged = true;

            if( pReqDscr->m_objState != EObjState::Created )
            {
                if( m_hshRequests.contains(i_reqDscr.m_iId) )
                {
                    m_hshRequests.remove(i_reqDscr.m_iId);
                }
            }
        }
    } // if( m_hshRequestDscrs.contains(i_reqDscr.m_iId) )

    if( bChanged )
    {
        emit requestChanged(i_reqDscr);
    }

} // updateRequest

//------------------------------------------------------------------------------
void CRequestExecTree::removeRequest( qint64 i_iId )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( m_hshRequests.contains(i_iId) )
    {
        m_hshRequests.remove(i_iId);
    }

    if( m_hshRequestDscrs.contains(i_iId) )
    {
        SRequestDscr* pReqDscr = m_hshRequestDscrs[i_iId];

        if( pReqDscr->m_ariChildIds.size() > 0 )
        {
            qint64 iReqId;
            int    idxReq;

            for( idxReq = pReqDscr->m_ariChildIds.size()-1; idxReq >= 0; idxReq-- )
            {
                iReqId = pReqDscr->m_ariChildIds[idxReq];
                removeRequest(iReqId); // Removes the request form the child list (see below).
            }
        }

        SRequestDscr reqDscr = *pReqDscr;

        m_hshRequestDscrs.remove(i_iId);

        if( pReqDscr->m_iParentId >= 0 )
        {
            if( m_hshRequests.contains(pReqDscr->m_iParentId) )
            {
                CRequest* pReqParent = m_hshRequests[pReqDscr->m_iParentId];
                pReqParent->m_dscr.m_ariChildIds.removeOne(pReqDscr->m_iId);
            }

            SRequestDscr* pReqDscrParent = findRequest(pReqDscr->m_iParentId);

            if( pReqDscrParent != nullptr )
            {
                pReqDscrParent->m_ariChildIds.removeOne(pReqDscr->m_iId);
            }
        }

        delete pReqDscr;
        pReqDscr = nullptr;

        emit requestRemoved(reqDscr);

    } // if( m_hshRequestDscrs.contains(i_iId) )

} // removeRequest

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestExecTree::clear()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    emit clearing();

    SRequestDscr*       pReqDscr;
    SRequestDscr        reqDscr;
    QList<SRequestDscr> arReqDscrs;
    int                 idxReq;

    QHash<qint64,SRequestDscr*>::iterator it = m_hshRequestDscrs.begin();

    while( it != m_hshRequestDscrs.end() )
    {
        pReqDscr = it.value();

        reqDscr = *pReqDscr;

        arReqDscrs.append(reqDscr);

        it = m_hshRequestDscrs.erase(it);

        delete pReqDscr;
        pReqDscr = nullptr;
    }

    m_hshRequestDscrs.clear();
    m_hshRequests.clear();

    // Don't emit the signal "requestRemoved" while removing items from the
    // list to avoid a recursive access to the list while the size of the
    // list changes. First erase the list and afterwards emit the signal for
    // each item separately.
    for( idxReq = 0; idxReq < arReqDscrs.size(); idxReq++ )
    {
        reqDscr = arReqDscrs[idxReq];
        emit requestRemoved(reqDscr);
    }

    emit cleared();

} // clear

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SRequestDscr CRequestExecTree::getDescriptor( qint64 i_iId, bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    SRequestDscr reqDscr;

    reqDscr.m_iId = i_iId;

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        reqDscr = *pReqDscr;

        if( i_bTopMost )
        {
            SRequestDscr* pReqDscrTopMost = findParentRequestOf(i_iId,true);

            if( pReqDscrTopMost != nullptr )
            {
                reqDscr = *pReqDscrTopMost;
            }
        }
    } // if( pReqDscr != nullptr )

    return reqDscr;

} // getDescriptor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QObject* CRequestExecTree::getCreator( qint64 i_iId, bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QObject* pObjCreator = nullptr;

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        pObjCreator = pReqDscr->m_pObjCreator;

        if( i_bTopMost )
        {
            SRequestDscr* pReqDscrTopMost = findParentRequestOf(i_iId,true);

            if( pReqDscrTopMost != nullptr )
            {
                pObjCreator = pReqDscrTopMost->m_pObjCreator;
            }
        }
    } // if( pReqDscr != nullptr )

    return pObjCreator;

} // getCreator

//------------------------------------------------------------------------------
QString CRequestExecTree::getCreatorNameSpace( qint64 i_iId, bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strName;

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        strName = pReqDscr->m_strCreatorNameSpace;

        if( i_bTopMost )
        {
            SRequestDscr* pReqDscrTopMost = findParentRequestOf(i_iId,true);

            if( pReqDscrTopMost != nullptr )
            {
                strName = pReqDscrTopMost->m_strCreatorNameSpace;
            }
        }
    } // if( pReqDscr != nullptr )

    return strName;

} // getCreatorNameSpace

//------------------------------------------------------------------------------
QString CRequestExecTree::getCreatorClassName( qint64 i_iId, bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strName;

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        strName = pReqDscr->m_strCreatorClassName;

        if( i_bTopMost )
        {
            SRequestDscr* pReqDscrTopMost = findParentRequestOf(i_iId,true);

            if( pReqDscrTopMost != nullptr )
            {
                strName = pReqDscrTopMost->m_strCreatorClassName;
            }
        }
    } // if( pReqDscr != nullptr )

    return strName;

} // getCreatorClassName

//------------------------------------------------------------------------------
QString CRequestExecTree::getCreatorThreadName( qint64 i_iId, bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strName;

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        strName = pReqDscr->m_strCreatorThreadName;

        if( i_bTopMost )
        {
            SRequestDscr* pReqDscrTopMost = findParentRequestOf(i_iId,true);

            if( pReqDscrTopMost != nullptr )
            {
                strName = pReqDscrTopMost->m_strCreatorThreadName;
            }
        }
    } // if( pReqDscr != nullptr )

    return strName;

} // getCreatorThreadName

//------------------------------------------------------------------------------
QString CRequestExecTree::getCreatorObjectName( qint64 i_iId, bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strName;

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        strName = pReqDscr->m_strCreatorObjName;

        if( i_bTopMost )
        {
            SRequestDscr* pReqDscrTopMost = findParentRequestOf(i_iId,true);

            if( pReqDscrTopMost != nullptr )
            {
                strName = pReqDscrTopMost->m_strCreatorObjName;
            }
        }
    } // if( pReqDscr != nullptr )

    return strName;

} // getCreatorObjectName

//------------------------------------------------------------------------------
QString CRequestExecTree::getRequestName( qint64 i_iId, bool i_bRecursive ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strName;

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        strName = pReqDscr->m_strRequest;

        if( i_bRecursive )
        {
            SRequestDscr* pReqDscrParent = findParentRequestOf(i_iId);

            while( pReqDscrParent != nullptr )
            {
                strName.insert( 0, pReqDscrParent->m_strRequest + "/" );

                pReqDscrParent = findParentRequestOf(pReqDscrParent->m_iId);
            }
        }
    } // if( pReqDscr != nullptr )

    return strName;

} // getRequestName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CRequestExecTree::isBlockingRequest( qint64 i_iId, bool i_bRecursive ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bIs = false;

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        bIs = pReqDscr->m_bIsBlocking;

        if( !bIs && i_bRecursive )
        {
            SRequestDscr* pReqDscrParent = findParentRequestOf(i_iId);

            while( pReqDscrParent != nullptr )
            {
                bIs = pReqDscrParent->m_bIsBlocking;

                if( bIs )
                {
                    break;
                }
            }
        }
    } // if( pReqDscr != nullptr )

    return bIs;

} // isBlockingRequest

//------------------------------------------------------------------------------
int CRequestExecTree::getTimeoutInMs( qint64 i_iId ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int iTimeout_ms = 0;

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        iTimeout_ms = pReqDscr->m_iTimeout_ms;
    }

    return iTimeout_ms;

} // getTimeoutInMs

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CRequestExecTree::isAlive( qint64 i_iId ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bIs = false;

    if( m_hshRequests.contains(i_iId) )
    {
        CRequest* pReq = m_hshRequests[i_iId];

        if( pReq != nullptr )
        {
            bIs = true;
        }
    }
    return bIs;

} // isAlive

//------------------------------------------------------------------------------
bool CRequestExecTree::isWaiting( qint64 i_iId ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bIs = false;

    if( m_hshRequests.contains(i_iId) )
    {
        CRequest* pReq = m_hshRequests[i_iId];

        if( pReq != nullptr )
        {
            bIs = pReq->isWaiting();
        }
    }
    return bIs;

} // isWaiting

//------------------------------------------------------------------------------
bool CRequestExecTree::waitTimedOut( qint64 i_iId ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bTimedOut = false;

    if( m_hshRequests.contains(i_iId) )
    {
        CRequest* pReq = m_hshRequests[i_iId];

        if( pReq != nullptr )
        {
            bTimedOut = pReq->waitTimedOut();
        }
    }
    return bTimedOut;

} // waitTimedOut

//------------------------------------------------------------------------------
bool CRequestExecTree::wake( qint64 i_iId )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bWaked = false;

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        if( isWaiting(i_iId) )
        {
            if( m_hshRequests.contains(i_iId) )
            {
                CRequest* pReq = m_hshRequests[i_iId];

                if( pReq != nullptr )
                {
                    bWaked = pReq->wake();
                }
            }
        }
    } // if( pReqDscr != nullptr )

    return bWaked;

} // wake

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestExecTree::setResultConfirmationMessage( qint64 i_iId, CMsgCon* i_pMsgCon )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        if( m_hshRequests.contains(i_iId) )
        {
            CRequest* pReq = m_hshRequests[i_iId];

            if( pReq != nullptr )
            {
                pReq->setResultConfirmationMessage(i_pMsgCon);
            }
        }
    } // if( pReqDscr != nullptr )

} // setResultConfirmationMessage

//------------------------------------------------------------------------------
void CRequestExecTree::setExecutionConfirmationMessage( qint64 i_iId, CMsgCon* i_pMsgCon )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        if( m_hshRequests.contains(i_iId) )
        {
            CRequest* pReq = m_hshRequests[i_iId];

            if( pReq != nullptr )
            {
                pReq->setExecutionConfirmationMessage(i_pMsgCon);
            }
        }
    } // if( pReqDscr != nullptr )

} // setExecutionConfirmationMessage

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
qint64 CRequestExecTree::getParentId( qint64 i_iId, bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    qint64 iParentId = -1;

    SRequestDscr* pReqDscrParent = findParentRequestOf(i_iId,i_bTopMost);

    if( pReqDscrParent != nullptr )
    {
        iParentId = pReqDscrParent->m_iId;
    }
    return iParentId;

} // getParentId

//------------------------------------------------------------------------------
bool CRequestExecTree::isParentOf( qint64 i_iId, qint64 i_iChildId, bool i_bRecursive ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bIs = false;

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        if( i_iChildId < 0 ) // means is parent of any ..
        {
            bIs = true;
        }
        else // if( i_iChildId >= 0 )
        {
            SRequestDscr* pReqDscrChild = findChildRequest(i_iId,i_iChildId,i_bRecursive);

            if( pReqDscrChild != nullptr )
            {
                bIs = true;
            }
        } // if( i_iChildId >= 0 )
    } // if( pReqDscr != nullptr )

    return bIs;

} // isParentOf

//------------------------------------------------------------------------------
bool CRequestExecTree::isChildOf( qint64 i_iId, qint64 i_iParentId, bool i_bRecursive ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bIs = false;

    SRequestDscr* pReqDscr = findRequest(i_iId);

    if( pReqDscr != nullptr )
    {
        SRequestDscr* pReqDscrParent = nullptr;

        if( i_iParentId < 0 )
        {
            pReqDscrParent = findParentRequestOf(i_iId);
        }
        else
        {
            pReqDscrParent = findParentRequest(i_iId,i_iParentId,i_bRecursive);
        }
        if( pReqDscrParent != nullptr )
        {
            bIs = true;
        }
    } // if( pReqDscr != nullptr )

    return bIs;

} // isChildOf

////------------------------------------------------------------------------------
//int CRequestExecTree::getChildsCount() const
////------------------------------------------------------------------------------
//{
//    QMutexLocker mtxLocker(m_pMtx);
//
//    int iChildCount = 0;
//
//    SRequestDscr* pReqDscr = findRequest(m_dscr.m_iId);
//
//    if( pReqDscr != nullptr )
//    {
//        iChildCount = pReqDscr->m_ariChildIds.size();
//    }
//
//    return iChildCount;
//
//} // getChildsCount

////------------------------------------------------------------------------------
//qint64 CRequestExecTree::getChildId( qint64 i_iId, int i_idx ) const
////------------------------------------------------------------------------------
//{
//    QMutexLocker mtxLocker(m_pMtx);
//
//    qint64 iChildId = -1;
//
//    SRequestDscr* pReqDscr = findRequest(m_dscr.m_iId);
//
//    if( pReqDscr != nullptr )
//    {
//        if( i_idx >= 0 && i_idx < pReqDscr->m_ariChildIds.size() )
//        {
//            iChildId = pReqDscr->m_ariChildIds[i_idx];
//        }
//    }
//
//    return iChildId;
//
//} // getChildId

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestExecTree::lock()
//------------------------------------------------------------------------------
{
    m_pMtx->lock();
}

//------------------------------------------------------------------------------
void CRequestExecTree::unlock()
//------------------------------------------------------------------------------
{
    m_pMtx->unlock();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QHash<qint64,SRequestDscr*> CRequestExecTree::getRequests() const
//------------------------------------------------------------------------------
{
    #ifdef _DEBUG

    //QMutexLocker mtxLocker(m_pMtx);

    //SRequestDscr* pReqDscr;
    //SRequestDscr* pReqDscrParent;
    //SRequestDscr* pReqDscrTopMostParent;

    //QHash<qint64,SRequestDscr*>::const_iterator itHshRequests;

    //for( itHshRequests = m_hshRequestDscrs.begin(); itHshRequests != m_hshRequestDscrs.end(); itHshRequests++ )
    //{
    //    pReqDscr = itHshRequests.value();

    //    if( pReqDscr->m_iParentId >= 0 )
    //    {
    //        pReqDscrParent = findParentRequestOf(pReqDscr->m_iId,false);
    //        pReqDscrTopMostParent = findParentRequestOf(pReqDscr->m_iId,true);
    //    }
    //}

    #endif // #ifdef _DEBUG

    return m_hshRequestDscrs;

} // getRequests

//------------------------------------------------------------------------------
QList<SRequestDscr*> CRequestExecTree::getTopMostParentRequests() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QList<SRequestDscr*>        arRequestDscrs;
    QHash<qint64,SRequestDscr*> hshRequestDscrs;

    QHash<qint64,SRequestDscr*>::const_iterator itc = m_hshRequestDscrs.begin();

    SRequestDscr* pReqDscrTopMostParent = nullptr;
    SRequestDscr* pReqDscr;

    while( itc != m_hshRequestDscrs.end() )
    {
        pReqDscr = itc.value();

        if( pReqDscr != nullptr )
        {
            if( pReqDscr->m_iParentId >= 0 )
            {
                pReqDscrTopMostParent = findParentRequestOf(pReqDscr->m_iId,true);

                if( pReqDscrTopMostParent != nullptr )
                {
                    if( !hshRequestDscrs.contains(pReqDscrTopMostParent->m_iId) )
                    {
                        hshRequestDscrs[pReqDscrTopMostParent->m_iId] = pReqDscrTopMostParent;
                    }
                }
            }
            else
            {
                if( !hshRequestDscrs.contains(pReqDscr->m_iId) )
                {
                    hshRequestDscrs[pReqDscr->m_iId] = pReqDscr;
                }
            }
        }
        itc++;
    }

    QHash<qint64,SRequestDscr*>::iterator itv = hshRequestDscrs.begin();

    while( itv != hshRequestDscrs.end() )
    {
        arRequestDscrs.append(itv.value());
        itv++;
    }

    return arRequestDscrs;

} // getTopMostParentRequest

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SRequestDscr* CRequestExecTree::findRequest( qint64 i_iId ) const
//------------------------------------------------------------------------------
{
    // The caller must "lock" and "unlock" the request tree while using pointers to request descriptors.
    //QMutexLocker mtxLocker(m_pMtx);

    SRequestDscr* pReqDscr = nullptr;

    if( m_hshRequestDscrs.contains(i_iId) )
    {
        pReqDscr = m_hshRequestDscrs[i_iId];
    }
    return pReqDscr;

} // findRequest

//------------------------------------------------------------------------------
SRequestDscr* CRequestExecTree::findParentRequest(
    qint64 i_iId,
    qint64 i_iParentId,
    bool   i_bRecursive ) const
//------------------------------------------------------------------------------
{
    // The caller must "lock" and "unlock" the request tree while using pointers to request descriptors.
    //QMutexLocker mtxLocker(m_pMtx);

    SRequestDscr* pReqDscrParent = nullptr;
    SRequestDscr* pReqDscr = nullptr;

    if( m_hshRequestDscrs.contains(i_iId) )
    {
        pReqDscr = m_hshRequestDscrs[i_iId];

        if( pReqDscr->m_iParentId == i_iParentId )
        {
            pReqDscrParent = findRequest(i_iParentId);
        }
        else if( i_bRecursive )
        {
            pReqDscrParent = findParentRequest(pReqDscr->m_iParentId,i_iParentId,i_bRecursive);
        }
    }
    return pReqDscrParent;

} // findParentRequest

//------------------------------------------------------------------------------
SRequestDscr* CRequestExecTree::findParentRequestOf( qint64 i_iId, bool i_bTopMost ) const
//------------------------------------------------------------------------------
{
    // The caller must "lock" and "unlock" the request tree while using pointers to request descriptors.
    //QMutexLocker mtxLocker(m_pMtx);

    SRequestDscr* pReqDscrParent = nullptr;
    SRequestDscr* pReqDscr = nullptr;

    if( m_hshRequestDscrs.contains(i_iId) )
    {
        pReqDscr = m_hshRequestDscrs[i_iId];

        if( pReqDscr->m_iParentId >= 0 )
        {
            if( m_hshRequestDscrs.contains(pReqDscr->m_iParentId) )
            {
                pReqDscrParent = m_hshRequestDscrs[pReqDscr->m_iParentId];
            }

            if( i_bTopMost && pReqDscrParent != nullptr )
            {
                qint64 iParentId = pReqDscrParent->m_iParentId;

                while( iParentId >= 0 && m_hshRequestDscrs.contains(iParentId) )
                {
                    pReqDscrParent = m_hshRequestDscrs[iParentId];

                    iParentId = -1;

                    if( pReqDscrParent != nullptr )
                    {
                        iParentId = pReqDscrParent->m_iParentId;
                    }
                }
            }
        }
    }
    return pReqDscrParent;

} // findParentRequestOf

//------------------------------------------------------------------------------
SRequestDscr* CRequestExecTree::findChildRequest(
    qint64 i_iId,
    qint64 i_iChildId,
    bool   i_bRecursive ) const
//------------------------------------------------------------------------------
{
    // The caller must "lock" and "unlock" the request tree while using pointers to request descriptors.
    //QMutexLocker mtxLocker(m_pMtx);

    SRequestDscr* pReqDscrChild = nullptr;
    SRequestDscr* pReqDscr = nullptr;

    if( m_hshRequestDscrs.contains(i_iId) )
    {
        pReqDscr = m_hshRequestDscrs[i_iId];

        int idxChild;

        for( idxChild = 0; idxChild < pReqDscr->m_ariChildIds.size(); idxChild++ )
        {
            if( pReqDscr->m_ariChildIds[idxChild] == i_iChildId )
            {
                pReqDscrChild = findRequest(i_iChildId);
                break;
            }
        }

        if( (pReqDscrChild == nullptr) && i_bRecursive )
        {
            for( idxChild = 0; idxChild < pReqDscr->m_ariChildIds.size(); idxChild++ )
            {
                pReqDscrChild = findChildRequest(pReqDscr->m_ariChildIds[idxChild],i_iChildId,i_bRecursive);

                if( pReqDscrChild != nullptr )
                {
                    break;
                }
            }
        }
    }

    return pReqDscrChild;

} // findChildRequest

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestExecTree::setGarbageCollectorEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( m_bTmrGarbageCollectorEnabled != i_bEnabled )
    {
        m_bTmrGarbageCollectorEnabled = i_bEnabled;

        if( m_bTmrGarbageCollectorEnabled )
        {
            m_pTmrGarbageCollector->start( static_cast<int>(m_fTmrGarbageCollecterInterval_s*1000) );
        }
        else
        {
            m_pTmrGarbageCollector->stop();
        }
        emit garbageCollectorChanged();
    }

} // setGarbageCollectorEnabled

//------------------------------------------------------------------------------
void CRequestExecTree::setGarbageCollectorIntervalInSec( double i_fInterval_s )
//------------------------------------------------------------------------------
{
    if( m_fTmrGarbageCollecterInterval_s != i_fInterval_s )
    {
        m_fTmrGarbageCollecterInterval_s = i_fInterval_s;
        emit garbageCollectorChanged();

        m_pTmrGarbageCollector->setInterval( static_cast<int>(m_fTmrGarbageCollecterInterval_s*1000) );
    }

} // setGarbageCollectorIntervalInSec

//------------------------------------------------------------------------------
void CRequestExecTree::setGarbageCollectorElapsedInSec( double i_fElapsed_s )
//------------------------------------------------------------------------------
{
    if( m_fTmrGarbageCollectorElapsed_s != i_fElapsed_s )
    {
        m_fTmrGarbageCollectorElapsed_s = i_fElapsed_s;
        emit garbageCollectorChanged();
    }

} // setGarbageCollectorElapsedInSec

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CRequestExecTree::onTmrGarbageCollectorTimeout()
//------------------------------------------------------------------------------
{
    QList<SRequestDscr*> arReqDscrs = getTopMostParentRequests();

    SRequestDscr* pReqDscr;
    int           idxReq;
    double        fElapsed_s;

    for( idxReq = 0; idxReq < arReqDscrs.size(); idxReq++ )
    {
        pReqDscr = arReqDscrs[idxReq];

        if( pReqDscr->m_objState == EObjState::Destroyed )
        {
            fElapsed_s = ZS::System::Time::getProcTimeInSec() - pReqDscr->m_fTimeEnd_s;

            if( fElapsed_s > m_fTmrGarbageCollectorElapsed_s )
            {
                removeRequest(pReqDscr->m_iId);
            }
        }
    }

} // onTmrGarbageCollectorTimeout

