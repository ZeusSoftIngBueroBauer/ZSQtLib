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

/*==============================================================================

Introduction (Overview)
=======================

The request classes (CRequestExecTree, CRequestQueue, and CRequest) are used
to handle asynchronous requests keeping information about the requests in progress.

On starting a request several asynchronous sub requests might be necessary to be
started. Those sub requests might be processed in parallel ("at the same time")
or in serial (one after another). For this request items can have a parent request
item and several child request items and are all hierarchically stored in a tree
of request items. If a request is currently in progress (if the instance executing
the request is "busy") further requests may not be started immediately but must
be postponed to be executed after the request in progress has been finished.
The state of the current request in progress and a list of postponed requests is
stored in a request queue.

To use the request classes the singleton class "CRequestExecTree" must be
created on system startup (e.g. as a member variable of "theApp").

Each class handling asynchronous requests should get a member of class
"CRequestQueue".

On calling an asynchronous request method first it has to be checked if currently
the request class is busy. If busy the request method must be postponed in the
request queue. If idle the requests can be immediately started.

On starting a request an instance of class "CRequest" should be created via the
request queue's method "createRequest". If a sub request is started the state of
this "pending" sub request should be stored in the request queue by adding a
descriptor of the pending request to the request queue.

The called asynchronous request method must return an instance of class "CRequest".
If the request is executed asynchronously the caller must connect a slot
"onRequestChanged" to the "changed" signal of the request instance.

If a request is executed the current progress must be updated whereupon the "changed"
signal is emitted. If the progress is set to 100% the request has been finished and
the "onRequestChanged" slot may start further sub requests or may finish the
request in progress.

Implementing Classes Providing Asynchronous Requests
====================================================

KOMMENTAR MUSS UEBERARBEITET WERDEN (STIMMT NICHT MEHR SO GANZ)

// Noch zu ergaenzen:
//-------------------
// On calling "update" and emitting the "changed" signal further request methods
// may be called as reentries by the signal receivers. Those requests must not be
// immediately started as further below "executeNextPostponedRequest" is called and
// if there are postponed requests two requests would be in progress at the same
// time (the postponed request would overwrite the request in progress which
// might have been started by the "changed" signal receiver). This is why
// "setRequestInProgress(nullptr)" is called after "pReqInProgress->update()"
// so that "isRequestInProgress" returns true. To be on save side within the
// request methods "isIdle" should be called to detect whether the request may
// be immediately started.

On using (or implementing) classes executing asynchronous requests using request
items and emitting the "requestExecuted" signal a good choice for the signature of
the "requestExecuted" signal would be:

    void changed( ZS::System::SRequestDscr );

The "changed" signal is emitted to

1.  indicate the initial start of a request (Progress_perCent = 0) or to
2.  indicate intermediate results via "Data" (0 < Progress_perCent < 100) or to
3.  indicate that the request has been executed completely (Progress_perCent = 100).
    In this case "errResultInfo" indicates whether the request execution was
    successful or not.

Example:
--------

On clicking an "Connect" button the user wants to connect a TCP client with
a remote server.

0.  Provide the following instance member variables to handle the requests
    in progress.

    CRequest*           m_pReqInProgress;   // Request item of the request started by this class.
    int                     m_iReqIdPending;        // Unique id (key) of the child request item started by calling a request method of another class.
    CClient::ERequest       m_requestPending;       // Enum value to identify the kind of the started child request (e.g. ERequestChangeSettings).

1.  First connect a "onClientRequestExecuted" slot with the "requestExecuted"
    signal of the client. Usually this has already been done within the ctor
    of the widget hosting the "Connect" button.

    if( !QObject::connect(
        |* pObjSender   *| m_pClient,
        |* szSignal     *| SIGNAL(requestExecuted(QObject*,int,int,int,ZS::System::SErrResultInfo&,void*)),
        |* pObjReceiver *| this,
        |* szSlot       *| SLOT(onIpcClientRequestExecuted(QObject*,int,int,int,ZS::System::SErrResultInfo&,void*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

2.  Create a top request item for the connect request within the
    "onButtonClicked" slot method.

    m_pReqInProgress = CRequest::create(
        |* strNameSpace   *| "ZS::Ipc::GUI",
        |* strClassName   *| "CWdgtIpcClient",
        |* pObjCreator    *| this,
        |* iRequest       *| CClient::ERequestConnect,
        |* strRequest     *| CClient::Request2Str(CClient::ERequestConnect),
        |* pReqParent *| nullptr );

    Also the state "Connecting" should be indicated via the GUI controls and some
    GUI controls should become disabled (e.g. "Apply Settings") whereas others might
    become visible and enabled (e.g. "Abort").

3.  The client will be requested to connect with the server using the
    asynchronous "connect" method. This method takes the top level request
    item as the parent request item and returns a request item to describe
    the started child request.

    CRequest* pReqPending = m_pClient->connect(i_iReqIdParent);

5.  Now check whether the request has been executed synchronously or
    asynchronously. E.g. if the client was not connected to a server the
    client may immediately take over the new as the current settings.
    If the client is connected with a server this connection may first
    has to be closed in order to apply the settings (e.g. on changing the
    remote host address or remote port). In case the request is executed
    asynchronously connect "onRequestDestroying" slots to the "destroying"
    signal of the newly created request items.

    // In case the request has been executed synchronously ..
    if( !isAsynchronousRequest(pReqPending) )
    {
        // .. the request has been executed completely and the top
        // level request item has to be deleted.
        pReqPending = nullptr; // deleted as child of RequestInProgress
        CRequest* pReqInProgress = m_pReqInProgress;
        m_pReqInProgress = nullptr; // Invalidate current request item as "onRequestDestroying" will be called.
        CRequest::delete(pReqInProgress);
        pReqInProgress = nullptr;
    }

    // In case the request is executed asynchronously ..
    else // if( isAsynchronousRequest(pReqPending) )
    {
        // .. store the unique id of the request and a enum value describing
        // the started request.
        m_iReqIdPending  = pReqPending->getId();
        m_requestPending = CClient::ERequestConnect;

        // Connect to the destroying signals of the created request items to
        // get informed if the request should be aborted.
        if( !QObject::connect(
            * pObjSender   * m_pReqInProgress,
            * szSignal     * SIGNAL(destroying(ZS::System::CRequest*,int,int,int,const ZS::System::SErrResultInfo&)),
            * pObjReceiver * this,
            * szSlot       * SLOT(onRequestDestroying(ZS::System::CRequest*,int,int,int,const ZS::System::SErrResultInfo&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            * pObjSender   * pReqPending,
            * szSignal     * SIGNAL(destroying(ZS::System::CRequest*,int,int,int,const ZS::System::SErrResultInfo&)),
            * pObjReceiver * this,
            * szSlot       * SLOT(onRequestDestroying(ZS::System::CRequest*,int,int,int,const ZS::System::SErrResultInfo&)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }

6.  Within the "onClientRequestExecuted" slot check whether the request has been
    executed completely. Take also into account the the client may emit the
    "requestExecuted" signal not just for the "changeSettings" request triggered
    by me but may also process requests triggered by other classes. The client has
    got a list of postponed requests and may have postponed the "changeSettings"
    requests to be executed after the current request in progress and all other
    previously postponed requests have been executed.

    void CWdgtIpcClient::onIpcClientRequestExecuted(
        QObject*            i_pClient,
        int                 i_iRequestId,
        int                 i_iRequest,
        int                 i_iProgress_perCent,
        ZS::System::SErrResultInfo& i_errResultInfo,
        void*               i_pvData )
    {
        // If it was the child request started by me ...
        if( m_pReqInProgress != nullptr && i_iRequestId == m_iReqIdPending )
        {
            // If the request has been initially started ...
            if( i_iProgress_perCent == 0 )
            {
                // Requests may be postponed. If all previously requests have been executed
                // and a postponed request is started the "requestExecuted" signal is emitted
                // with "Progress_perCent = 0".
            }
            // If the request has been initially started ...
            else if( i_iProgress_perCent > 0 && i_iProgress_perCent < 100 )
            {
                // Requests taking a long time may indicate intermediate results. In this case
                // the "requestExecuted" signal is emitted with "0 < Progress_perCent < 100"
                // and intermediate results (if any) are provided via the "Data" pointer.
            }
            // If the request has been executed completely ...
            else if( i_iProgress_perCent == 100 )
            {
                // If the connect request was in progress and its execution has been reported by the client ...
                if( m_pReqInProgress->getRequest() == CClient::ERequestConnect && i_iRequest == CClient::ERequestConnect )
                {
                    m_iReqIdPending  = -1;
                    m_requestPending = CClient::ERequestUndefined;

                    // Before deleting the request item either the "onRequestDestroying" slot
                    // should be disconnected or the instance member "m_pReqInProgress"
                    // should be invalidated so that the "destroying" signal will be ignored
                    // within the "onRequestDestroying" slot.
                    CRequest* pReqInProgress = m_pReqInProgress;
                    m_pReqInProgress = nullptr;
                    CRequest::delete(pReqInProgress);
                    pReqInProgress = nullptr;

                    The state "Connected" should be indicated via the GUI controls and some
                    GUI controls should become invisible and disabled (e.g. "Abort") whereas
                    others might become visible and enabled (e.g. "Connect" or "Apply Settings").
                }
                ....
            }
        }
    }

7.  Within the "onRequestDestroying" slot check whether the request in progress has been aborted.

    void CWdgtIpcClient::onRequestDestroying(
        ZS::System::CRequest*         i_pReq,
        int                       i_iReqId,
        int                       i_iRequest,
        int                       i_iProgress_perCent,
        const ZS::System::SErrResultInfo& i_errResultInfo )
    {
        // If the child request has been aborted ...
        if( i_iReqId == m_iReqIdPending )
        {
            // This can only happen if the whole request has been aborted by deleting the top
            // level request item. In this case the child request item is deleted as a child
            // of the top level request item. The top level request item will also be deleted
            // and this event will be handled further below.
            // Please note that if the sub request has been executed completely the request
            // executed signal has been emitted before and "ReqIdPending" and "requestPending"
            // have already been invalidated.
            m_iReqIdPending  = -1;
            m_requestPending = CClient::ERequestUndefined;
        }

        // If the top level request has been aborted ...
        else if( i_pReq == m_pReqInProgress )
        {
            // Invalidate the request in progress markers.
            m_pReqInProgress = nullptr;
            m_iReqIdPending  = -1;
            m_requestPending = CClient::ERequestUndefined;

            The current state should be indicated via the GUI controls and some
            GUI controls should become invisible and disabled others might become
            visible and enabled.
        }
    }

Further Hints:
==============

KOMMENTAR MUSS UEBERARBEITET WERDEN (STIMMT NICHT MEHR SO GANZ)

Visualization of Request Trees
------------------------------

Using a tree item model the executed request tree may be visualized to the user.

A progress bar may indicated the current progress of the request.

Aborting Requests in Progress
-----------------------------

Because request items belonging to the same top request item are linked in
parent child  relations to each other it is also possible to abort the whole
request by deleting the top request item. On deleting the top request item each
the tree of request items will be recursively processed deleting all child
request items. Classes which have started an asynchronous request and have
created a request item and have been connected to the "destroying" signal of
the request item will be informed that the request in progress has to be aborted
as the request item is going to be destroyed.

Please note that request items may be deleted for several reasons:

- If the originally started request has been processed completely this
  request will be deleted together with all child requests.
- If a request has been started as a child request in a separate thread
  and if this thread is terminated while the request is executed because
  an error occurred.
- If the originally started request has been aborted by the user this
  request will be deleted together with all child requests.

Use the argument "Progress_perCent" provided with the "destroying" signal to decide
whether the request was aborted (Progress_perCent < 100) or whether the request
has been executed completely (Progress_perCent == 100). The argument "errResultInfo"
indicates whether the request has been executed with an error or with success.

Please note that signals may be queued through different threads meaning that if
the request item is deleted from within the context of another thread the request
item could have already been deleted if the "onRequestDestroying" slot is called.
So you should not access the request item within the "onRequestDestroying" slot
(but you can use the pointer of the deleted object to compare it with the stored
pointer to the request item in progress). One example for the scenario described
above is if the parent request item is living in another thread, has been executed
completely and will be destroyed together with all its children. Using
"DirectConnection" may help in most cases - but may also be dangerous if the
corresponding code is not thread safe.

On processing the slot of the "destroying" signal you should also not call
methods of the request items admin object singleton class "CRequestExecTree".
Therefore you should call none of the static methods of class "CRequest"
(e.g. "find") as this very likely leads to a deadlock. This is because the
mutex protecting the list of request items has been locked on destroying
the request item via the "delete" method. The "find" method would be
called as a reentry to the admin object instance while processing the "delete"
method and the already locked mutex would be recursively locked again. And you
should not emit the "requestExecuted" signal from within the "onRequestDestroying"
slot for the request item which is currently being destroyed.

Please also note that before deleting request items you should use the
"canBeDeleted" method to check whether the item can be deleted or not.
If the item is locked or if the item is a child of another item an exception
will be thrown if the item is deleted.

You should take into account that if the instance executing requests is living
in another thread than the instance starting the request the "requestExecuted"
signal will be queued through the threads (if not "DirectConnection" for
the signal/slot connection has been chosen). In this case the request item might
have already been deleted if the "onRequestExecuted" slot method is processed.
This is why you should avoid providing the request item within the signature of
the "requestExecuted" signal.

On emitting the "requestExecuted" signal the request items got to be locked
before emitting the signal. On emitting the "requestExecuted" signal the
originally started request might have been executed completely and therefore
the class triggering the request will delete the top level request item.
On deleting the top level request item the whole tree of request items will
be cleared and all request items will be deleted. So as a reentry on emitting
the "requestExecuted" signal the "RequestInProgress" (for which the signal is
emitted) may also be deleted as a child item of the original top level request
item. To prevent the request item in progress from deletion the item got to
be locked before emitting the "requestExecuted" signal. After the signal has
been emitted the item has to be unlocked. After unlocking the item the item
has to be deleted. But use the "canBeChecked" method before deleting the item.

    CRequest* pReqInProgress = m_pReqInProgress;
    m_pReqInProgress = nullptr; // Invalidate request in progress as slot "onRequestDestroying" may be called as a reentry.

    pReqInProgress->lock();
    emit requestExecuted( this, pReqInProgress ); // This may delete the top level request item clearing the whole tree of request items.
    pReqInProgress->lock();

    if( pReqInProgress->canBeDeleted(pReqTmp) )
    {
        CRequest::delete(pReqInProgress); // "onRequestDestroying" will be called as a reentry.
    }
    pReqInProgress = nullptr;

==============================================================================*/

#ifndef ZSSys_Request_h
#define ZSSys_Request_h

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysAux.h"

class QMutex;
class QThread;
class QWaitCondition;

namespace ZS
{
namespace System
{
class CMsgCon;
class CMsgReq;
class CMsgInd;
class CRequest;
class CRequestExecTree;
class CRequestQueue;

//******************************************************************************
struct ZSSYSDLL_API SRequestDscr
//******************************************************************************
{
public: // ctors
    SRequestDscr();
    SRequestDscr(
        qint64         i_iId,
        const QString& i_strCreatorNameSpace,
        const QString& i_strCreatorClassName,
        QObject*       i_pObjCreator,
        int            i_request,
        const QString& i_strRequest,
        qint64         i_iParentId,
        int            i_iTimeout_ms = 0,           // <= 0: Infinite (No Timeout)
        bool           i_bIsBlocking = false );
    SRequestDscr( const SRequestDscr& i_other );    // pvResultData will not be taken over.
    ~SRequestDscr();
public: // operators
    SRequestDscr& operator = ( const SRequestDscr& i_other );   // pvResultData will not be taken over.
    bool operator == ( const SRequestDscr& i_other ) const;     // pvResultData, ParentId, PendingRequestIds and ChildIds will not be taken into account.
    bool operator != ( const SRequestDscr& i_other ) const;
public: // struct methods
    bool isAsynchronousRequest() const { return (m_errResultInfo.getResult() == EResultUndefined); }
public: // struct methods
    bool isBeingAborted() const { return (m_objStateAborting == EObjState::Destroying); }
    bool hasBeenAborted() const { return (m_objStateAborting == EObjState::Destroyed); }
public: // struct methods
    // Sometimes it is necessary to keep the original parent id and the child ids of the request descriptor.
    void initFrom( const SRequestDscr& i_other );  // pvResultData, ParentId, PendingRequestIds and ChildIds will not be taken over.
public: // struct methods
    QString getAddTrcInfoStr( int i_iDetailLevel = 0 ) const;
public: // struct members
    qint64         m_iId;           // Unique id (key) of the request descriptor. May be different from "m_pRequest->getId()".
    QObject*       m_pObjCreator;
    QString        m_strCreatorNameSpace;
    QString        m_strCreatorClassName;
    QString        m_strCreatorObjName;
    QString        m_strCreatorThreadName;
    QThread*       m_pThread;       // Current active thread from which the request instance is created (not necessarily the same as the "pObjCreater->thread()").
    QString        m_strThreadName;
    int            m_request;       // Id of the processed request within the request executing class (e.g. "ERequestStartup").
    QString        m_strRequest;
    QString        m_strDscr;
    QString        m_strToolTip;
    int            m_iLockCount;
    int            m_iProgress_perCent;
    SErrResultInfo m_errResultInfo;
    QVariant       m_valResult;
    CMsgCon*       m_pMsgConResult;
    void*          m_pvResultData;
    EObjState      m_objStateAborting;  // Detached: Abort not in progress; Destroying: Abort is in progress; Destroyed: Request has been aborted.
    EObjState      m_objState;
    bool           m_bIsBlocking;
    int            m_iTimeout_ms;
    double         m_fTimeStart_s;
    double         m_fTimeEnd_s;
    qint64         m_iParentId;
    QList<qint64>  m_ariChildIds;    // Child requests. Each child request must be processed completely in order for this request to be finished.

}; // struct SRequestDscr


//******************************************************************************
class ZSSYSDLL_API CRequest : public QObject
//******************************************************************************
{
friend class CRequestExecTree;
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CRequest"; }
public: // class methods
    static qint64 GetUniqueRequestId();
public: // ctors and dtor
    CRequest(
        const QString& i_strCreatorNameSpace,
        const QString& i_strCreatorClassName,
        QObject*       i_pObjCreator,
        int            i_request,
        const QString& i_strRequest,
        qint64         i_iParentId,
        bool           i_bKeepReqDscrInExecTree = false,
        int            i_iTimeout_ms = 0,           // 0: Infinite (No Timeout), < 0 (-1): Use default from somewhere else (connection setting etc.)
        bool           i_bIsBlocking = false );
    CRequest( const SRequestDscr& i_reqDscr );
    virtual ~CRequest();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
private: // ctors and operators which are not allowed
    CRequest( const CRequest& i_other );
    CRequest& operator = ( const CRequest& i_other );
    bool operator == ( const CRequest& i_other ) const;
    bool operator != ( const CRequest& i_other ) const;
signals:
    // Please note that the progress values 0 and 100 are reserved to indicate the start and the end of a request.
    // The signal is also emitted if the request instance will be destroyed with "dscr.m_objState = EObjState::Destroyed".
    void changed( ZS::System::SRequestDscr i_dscr );
public: // instance methods
    void setKeepReqDscrInExecTree( bool i_bKeep );
    bool getKeepReqDscrInExecTree() const { return m_bKeepReqDscrInExecTree; }
public: // instance methods
    bool isAsynchronousRequest() const { return (m_dscr.m_errResultInfo.getResult() == EResultUndefined); }
public: // instance methods
    // If a request is processed completely the one calling "update" usually must
    // delete the request. But when using blocking method calls where sub requests
    // are started the request may not be deleted at the end of the sequence but
    // must be kept for the caller of the blocking method call. For this the caller
    // creating the request must lock the request. As long as a request is locked
    // the request may not be deleted.
    int lock();
    int unlock();
    bool isLocked() const { return (m_dscr.m_iLockCount > 0); }
public: // instance methods
    bool wait();
    bool wake();
    bool isWaiting() const { return m_bIsWaiting; }
    bool waitTimedOut() const { return m_bWaitTimedOut; }
public: // instance methods
    void update(); // after changing members you need to call update so that the "changed" signal is emitted.
public: // instance methods
    qint64 getId() const { return m_dscr.m_iId; }
public: // instance methods
    SRequestDscr getDescriptor() const { return m_dscr; }
    SRequestDscr getDescriptor( bool i_bTopMost ) const;
public: // instance methods
    QObject* getCreator( bool i_bTopMost = false ) const; // may return nullptr as the creator may be destroyed before the request
    QString getCreatorNameSpace( bool i_bTopMost = false ) const;
    QString getCreatorClassName( bool i_bTopMost = false ) const;
    QString getCreatorThreadName( bool i_bTopMost = false ) const;
    QString getCreatorObjectName( bool i_bTopMost = false ) const;
public: // instance methods
    QThread* getThread() const { return m_dscr.m_pThread; }             // may return nullptr as the thread may be destroyed before the request
    QString getThreadName() const { return m_dscr.m_strThreadName; }    // may return an empty string as the thread may be destroyed before the request
public: // instance methods
    int getRequest() const { return m_dscr.m_request; }
    QString getRequestName( bool i_bRecursive = false ) const;
    QString getDescription() const { return m_dscr.m_strDscr; }
    QString getToolTip() const { return m_dscr.m_strToolTip; }
public: // instance methods
    int getProgressInPerCent() const { return m_dscr.m_iProgress_perCent; }
public: // instance methods
    bool isErrorResult() const { return m_dscr.m_errResultInfo.isErrorResult(); }
    SErrResultInfo getErrResultInfo() const { return m_dscr.m_errResultInfo; }
    SErrResult getErrResult() const { return m_dscr.m_errResultInfo.getErrResult(); }
    EResultSeverity getSeverity() const { return m_dscr.m_errResultInfo.getSeverity(); }
    QString getSeverityStr() const { return m_dscr.m_errResultInfo.getSeverityStr(); }
    EResult getResult() const { return m_dscr.m_errResultInfo.getResult(); }
    QString getResultStr() const { return m_dscr.m_errResultInfo.getResultStr(); }
    QString getAddErrInfoDscr() const { return m_dscr.m_errResultInfo.getAddErrInfoDscr(); }
    SErrSource getErrSource() const { return m_dscr.m_errResultInfo.getErrSource(); }
public: // instance methods
    QVariant getResultValue() const { return m_dscr.m_valResult; }
    void* getResultData() const { return m_dscr.m_pvResultData; }
    void* takeResultData();
    CMsgCon* getResultConfirmationMessage() const { return m_dscr.m_pMsgConResult; }
    CMsgCon* takeResultConfirmationMessage();
public: // instance methods
    EObjState getObjState() const { return m_dscr.m_objState; }
public: // instance methods
    bool isBeingAborted() const { return (m_dscr.m_objStateAborting == EObjState::Destroying); }
    bool hasBeenAborted() const { return (m_dscr.m_objStateAborting == EObjState::Destroyed); }
public: // instance methods
    void setIsBlockingRequest( bool i_bIsBlocking );
    bool isBlockingRequest() const { return m_dscr.m_bIsBlocking; }
public: // instance methods
    void setTimeoutInMs( int i_iTimeout_ms );
    int getTimeoutInMs() const { return m_dscr.m_iTimeout_ms; }
public: // instance methods
    double getStartTimeInSec() const { return m_dscr.m_fTimeStart_s; }
    double getEndTimeInSec() const { return m_dscr.m_fTimeEnd_s; }
    double getDurationInSec() const;
    QString getDurationAsStrInBestUnit() const;
public: // instance methods
    QList<qint64> getChildIds() const { return m_dscr.m_ariChildIds; }
public: // instance methods
    void setParentId( qint64 i_iParentId );
    qint64 getParentId( bool i_bTopMost = false ) const;
    bool isParentOf( qint64 i_iChildId = -1, bool i_bRecursive = false ) const; // -1 to check whether the request has any child
    bool isChildOf( qint64 i_iParentId = -1, bool i_bRecursive = false ) const; // -1 to check whether the request has any parent
public: // instance methods
    void setDescription( const QString& i_strDscr );
    void setToolTip( const QString& i_strToolTip );
public: // instance methods
    void setProgressInPerCent( int i_iProgress_perCent );
public: // instance methods
    void setErrResultInfo( const SErrResultInfo& i_errResultInfo );
    void setErrResult( const SErrResult& i_errResult );
public: // instance methods
    void setSeverity( EResultSeverity i_severity );
    void setResult( EResult i_result );
    void setAddErrInfoDscr( const QString& i_strAddInfo );
    void setErrSource( const SErrSource& i_errSource );
public: // instance methods
    void setResultValue( const QVariant& i_val );
    void setResultConfirmationMessage( CMsgCon* i_pMsgCon );    // the request does not take ownership of the message
    void setResultData( void* i_pvData );                       // the request does not take ownership of the data block
public: // instance methods
    void setObjState( EObjState i_objState );
public: // instance methods
    void abort();
    void setHasBeenAborted();
public: // instance methods
    bool hasExecutionValueOrData() const;
    bool hasExecutionValueOrData( const QString& i_strKey ) const;
public: // instance methods
    bool hasExecutionValue() const;
    bool hasExecutionValue( const QString& i_strKey ) const;
    QVariant getExecutionValue( const QString& i_strKey ) const;
    void setExecutionValue( const QString& i_strKey, const QVariant& i_val );
    void removeExecutionValue( const QString& i_strKey );
public: // instance methods
    bool hasExecutionData() const;
    bool hasExecutionData( const QString& i_strKey ) const;
    void* getExecutionData( const QString& i_strKey ) const;
    void* takeExecutionData( const QString& i_strKey );
    void setExecutionData( const QString& i_strKey, void* i_pvData );   // the request does not take ownership of the data block
public: // instance methods
    CMsgReq* getExecutionRequestMessage() const { return m_pMsgReqExec; }
    CMsgReq* takeExecutionRequestMessage();
    void setExecutionRequestMessage( CMsgReq* i_pMsgReq );              // the request does not take ownership of the message
public: // instance methods
    CMsgCon* getExecutionConfirmationMessage() const { return m_pMsgConExec; }
    CMsgCon* takeExecutionConfirmationMessage();
    void setExecutionConfirmationMessage( CMsgCon* i_pMsgCon );         // the request does not take ownership of the message
public: // instance methods
    CMsgInd* getExecutionIndicationMessage() const { return m_pMsgIndExec; }
    CMsgInd* takeExecutionIndicationMessage();
    void setExecutionIndicationMessage( CMsgInd* i_pMsgInd );           // the request does not take ownership of the message
public: // instance methods
    CMsgReq* getInitiatorRequestMessage() const { return m_pMsgReqInitiator; }
    CMsgReq* takeInitiatorRequestMessage();
    void setInitiatorRequestMessage( CMsgReq* i_pMsgReq );              // the request takes ownership of the message
public: // instance methods
    CMsgCon* getInitiatorConfirmationMessage() const { return m_pMsgConInitiator; }
    CMsgCon* takeInitiatorConfirmationMessage();
    void setInitiatorConfirmationMessage( CMsgCon* i_pMsgCon );         // the request takes ownership of the message
public: // instance methods
    CMsgInd* getInitiatorIndicationMessage() const { return m_pMsgIndInitiator; }
    CMsgInd* takeInitiatorIndicationMessage();
    void setInitiatorIndicationMessage( CMsgInd* i_pMsgInd );           // the request takes ownership of the message
public: // instance methods
    QString getAddTrcInfoStr( int i_iDetailLevel = 0 ) const;
protected slots:
    void onCreatorDestroyed( QObject* i_pObjCreator );
    void onThreadFinished();
protected: // instance members
    SRequestDscr            m_dscr;
    QMutex*                 m_pMtxWaitCondition;
    QWaitCondition*         m_pWaitCondition;
    bool                    m_bIsWaiting;
    bool                    m_bWokenUp;
    bool                    m_bWaitTimedOut;
    bool                    m_bChanged;
    bool                    m_bKeepReqDscrInExecTree;
    QHash<QString,QVariant> m_hshvalsExec;      // The request executing class may temporarily store values for later processing.
    QHash<QString,void*>    m_hshpvsExec;       // The request executing class may temporarily store data for later processing.
    CMsgReq*                m_pMsgReqExec;      // The request executing class may temporarily store a request message for later processing.
    CMsgCon*                m_pMsgConExec;      // The request executing class may temporarily store a received confirmaton messages for later processing.
    CMsgInd*                m_pMsgIndExec;      // The request executing class may temporarily store an indication message for later processing.
    CMsgReq*                m_pMsgReqInitiator; // Request message initiating the request.
    CMsgCon*                m_pMsgConInitiator; // Confirmation message which should be sent to the initiator of the request.
    CMsgInd*                m_pMsgIndInitiator; // Indication message initiating the request.

}; // class CRequest


/*******************************************************************************
Macros and global methods
*******************************************************************************/

//------------------------------------------------------------------------------
ZSSYSDLL_API bool deleteResultConfirmationMessage( CRequest* i_pReq, QObject* i_pObjMsgReceiver );
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ZSSYSDLL_API bool isAsynchronousRequest( CRequest* i_pReq );
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
inline bool deleteRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    bool bDeleted = false;

    if( i_pReq != nullptr && !i_pReq->isLocked() )
    {
        if( i_pReq->getObjState() == EObjState::Created || i_pReq->getObjState() == EObjState::Detached )
        {
            delete i_pReq;
            bDeleted = true;
        }
    }
    return bDeleted;

} // deleteRequest

//------------------------------------------------------------------------------
inline bool isSuccessResult( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    bool bSuccess = true;

    if( i_pReq != nullptr )
    {
        bSuccess = !i_pReq->isErrorResult();
    }
    return bSuccess;

} // isSuccessResult

//------------------------------------------------------------------------------
inline bool isErrorResult( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    bool bError = false;

    if( i_pReq != nullptr )
    {
        bError = i_pReq->isErrorResult();
    }
    return bError;

} // isErrorResult

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_Request_h
