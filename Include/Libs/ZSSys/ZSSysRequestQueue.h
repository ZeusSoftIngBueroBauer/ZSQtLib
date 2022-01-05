/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSSys_RequestQueue_h
#define ZSSys_RequestQueue_h

#include <QtCore/QObject>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysRequest.h"

namespace ZS
{
namespace System
{
class CMsgCon;

//******************************************************************************
class ZSSYSDLL_API CRequestQueue : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CRequestQueue(
        const QString& i_strCreatorNameSpace,
        const QString& i_strCreatorClassName,
        QObject*       i_pObjParent,    // Object name of the queue = "i_pObjParent->objectName()"
        bool           i_bKeepReqDscrInExecTree = false );
    CRequestQueue(
        const QString& i_strObjName,    // Object name of the queue.
        const QString& i_strCreatorNameSpace,
        const QString& i_strCreatorClassName,
        QObject*       i_pObjParent,
        bool           i_bKeepReqDscrInExecTree = false );
    CRequestQueue(
        QObject* i_pObjParent,          // Object name of the queue = "i_pObjParent->objectName()"
        bool     i_bKeepReqDscrInExecTree = false );
    ~CRequestQueue();
public: // instance methods
    // The request instance is only created but not added to the request queue.
    // If requests are created from within different thread context the request
    // queue must be protected by a mutex.
    CRequest* createRequest(
        int            i_request,
        const QString& i_strRequest,
        qint64         i_iParentId,
        int            i_iTimeout_ms = 0,           // 0: Invalid; < 0 (-1): Infinite
        bool           i_bIsBlocking = false ) const;
    CRequest* createRequest(
        const QString& i_strCreatorNameSpace,
        const QString& i_strCreatorClassName,
        int            i_request,
        const QString& i_strRequest,
        qint64         i_iParentId,
        int            i_iTimeout_ms = 0,           // 0: Invalid; < 0 (-1): Infinite
        bool           i_bIsBlocking = false ) const;
public: // instance methods
    qint64 GetUniqueRequestId() const;
public: // instance methods
    void setCreatorNameSpace( const QString& i_strCreatorNameSpace );
    QString getCreatorNameSpace() const { return m_strCreatorNameSpace; }
    void setCreatorClassName( const QString& i_strCreatorClassName );
    QString getCreatorClassName() const { return m_strCreatorClassName; }
public: // instance methods
    QObject* getParent() const { return m_pObjParent; }
public: // instance methods
    void setKeepReqDscrInExecTree( bool i_bKeep );
    bool keepReqDscrInExecTree() const { return m_bKeepReqDscrInExecTree; }
public: // instance methods
    // On calling "update" the request emits the changed signal.
    // As a reentry receiver of the signal may call further request methods.
    // Those request methods must not be executed immediately but must be
    // postponed. Otherwise "isIdle" called at the end of a request returns
    // true and the request method would be executed twice. For this the request
    // queue must be locked before updating the request as a locked request queue
    // returns true for "isIdle".
    int lock();
    int unlock();
    bool isLocked() const { return (m_iLockCount > 0); }
public: // instance methods (concerning the top post (main) request in progress)
    void setRequestInProgress( CRequest* i_pReq ); // "i_pReq == nullptr" removes the current main (top most) request.
    CRequest* getRequestInProgress() const { return m_pReqInProgress; }
    CRequest* takeRequestInProgress();  // Removes (but does not delete) the main (top most) request.
    void removeRequestInProgress();     // Removes (but does not delete) the main (top most) request.
    bool isRequestInProgress() const;
public: // instance methods (concerning any (top post (main) and sub (child) requests in progress)
    // Please note that it is not sufficient just to check the "requestInProgress"
    // member variable to decide whether currently a request is in progress or not.
    // The requests "changed" signal is usually emitted if the current request has
    // been executed ("requestInProgress == nullptr") but before a postponed request is
    // taken from the list of postponed requests to become the current "requestInProgress".
    // Receiver of the requests "changed" signal may call one of the public request
    // methods of the request queue instance and may start further requests as a reentry
    // within the "onRequestChanged" slot. To ensure that requests will not be started
    // immediately on emitting the "changed" signal the queue must be locked before and
    // unlocked afterwards. Before executing a request method it must be checked whether
    // the queue is idle which means that:
    // - there is currently no request in progress,
    // - the queue is not locked and
    // - there are no postponed requests.
    bool isIdle() const;
public: // instance methods (concerning any (top post (main) and sub (child) requests in progress)
    void addRequestInProgress( CRequest* i_pReq );          // The main (top most) request must have been set before. The parent request (if any) must have already been assigned to the new added request.
    void removeRequestInProgress( CRequest* i_pReq );       // Removes (but does not delete) the request.
    bool isRequestInProgress( CRequest* i_pReq ) const;
public: // instance methods (concerning any (top post (main) and sub (child) requests in progress)
    CRequest* getRequestInProgress( qint64 i_iId ) const;
    CRequest* takeRequestInProgress( qint64 i_iId );        // Removes (but does not delete) the request.
    void removeRequestInProgress( qint64 i_iId );           // Removes (but does not delete) the request.
    bool isRequestInProgress( qint64 i_iId ) const;
public: // instance methods (concerning any (top post (main) and sub (child) requests in progress)
    CRequest* getRequestInProgressByType( int i_request ) const;
    CRequest* takeRequestInProgressByType( int i_request ); // Removes (but does not delete) the request.
    void removeRequestInProgressByType( int i_request );    // Removes (but does not delete) the request.
    bool isRequestInProgressByType( int i_request ) const;
public: // instance methods (concerning any (top post (main) and sub (child) requests in progress)
    CRequest* findRequestById( qint64 i_iId, bool i_bIgnorePostponedRequests = false, bool* o_pbIsPostponedRequest = nullptr, int* o_pidxPostponedRequest = nullptr ) const;
    QList<CRequest*> findRequestsByType( int i_request, bool i_bIgnorePostponedRequests = false ) const;
    QList<CRequest*> findRequestsByType(
        int            i_request,
        const QString& i_strCreatorObjName,         // empty string to ignore ObjName
        const QString& i_strCreatorClassName,       // empty string to ignore ClassName
        const QString& i_strCreatorNameSpace,  // empty string to ignore NameSpace
        bool           i_bIgnorePostponedRequests = false ) const;
public: // instance methods
    void setSyncRequestToBeDeletedLater( CRequest* i_pReq );
public: // instance methods (postponing requests)
    int getMaxPostponedRequestEntries() const { return m_iReqsPostponedMaxEntries; }
    void setMaxPostponedRequestEntries( int i_iMaxEntries );
    EResult postponeRequest( CRequest* i_pReq ); // may return EResultSuccess or EResultListIsFull
    int getPostponedRequestsCount() const { return m_arpReqsPostponed.size(); }
    CRequest* getPostponedRequestByIdx( int i_idx = 0 ) const;
    CRequest* takePostponedRequestByIdx( int i_idx = 0 );
    CRequest* getFirstPostponedRequest();
    CRequest* takeFirstPostponedRequest();
    CRequest* getLastPostponedRequest();
    CRequest* takeLastPostponedRequest();
    CRequest* findPostponedRequestById( qint64 i_iId, int* o_pidx = nullptr ) const;
    CRequest* takePostponedRequestById( qint64 i_iId );
    void removePostponedRequestByIdx( int i_idx );   // the postponed request will not be deleted
    void removePostponedRequestById( qint64 i_iId ); // the postponed request will not be deleted
    void removePostponedRequest( CRequest* i_pReq ); // the postponed request will not be deleted
    bool hasPostponedRequest() const { return (m_arpReqsPostponed.size() > 0); }
    bool isPostponedRequest( CRequest* i_pReq ) const;
    bool isPostponedRequest( qint64 i_iId ) const;
public: // instance methods
    QString getAddTrcInfoStr( int i_iDetailLevel = 0 ) const;
protected slots:
    void onRequestDestroyed( QObject* i_pReq );
protected: // instance members
    QString                 m_strCreatorNameSpace;
    QString                 m_strCreatorClassName;
    QObject*                m_pObjParent;
    bool                    m_bKeepReqDscrInExecTree;
    int                     m_iLockCount;
    CRequest*               m_pReqInProgress;               // The request in progress may have sub (child) requests in progress.
    CRequest*               m_pReqSyncToBeDeletedLater;
    QHash<qint64,CRequest*> m_hshReqsInProgress;            // To speed up e.g. "isRequestInProgress". The key is the Id of the request in progress.
    int                     m_iReqsPostponedMaxEntries;     // Maximum number of entries which can be postponed in list
    QList<CRequest*>        m_arpReqsPostponed;

}; // class CRequestQueue

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_RequestQueue_h
