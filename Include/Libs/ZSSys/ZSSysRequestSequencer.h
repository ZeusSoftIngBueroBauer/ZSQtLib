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

#ifndef ZSSys_RequestSequencer_h
#define ZSSys_RequestSequencer_h

#include <QtCore/QObject>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysRequest.h"

class QMutex;
class QTimer;

namespace ZS
{
namespace System
{
class CMsgReq;
class CRequest;
class CTrcAdminObj;

// Please note that not all request classes may support blocking method calls and just ignore the passed WaitTimeout.
// Other request classes may just support blocking method calls and will use their own wait timeout.
// To decide whether the caller of the request method has to wait blocking the thread the timeout property of the
// returned request instance must be checked. The initial start method of the request sequencer itself is a blocking
// call if an initial wait timeout != 0 has been set. If the parent request is a blocking method call all child requests
// must return synchronously and must therefore also be blocking calls.
typedef ZS::System::CRequest* (*TFctExecuteType1)( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, QVariant& i_valExec );
typedef ZS::System::CRequest* (*TFctExecuteType2)( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec );

//******************************************************************************
enum ERequestEntryType
//******************************************************************************
{
    ERequestEntryTypeGroupNode,     // Group nodes cannot be executed.
    ERequestEntryTypeActiveNode,    // Active nodes cannot have children.
    ERequestEntryTypeCount,
    ERequestEntryTypeUndefined
};
ZSSYSDLL_API QString requestEntryType2Str( int i_entryType );
ZSSYSDLL_API ERequestEntryType str2RequestEntryType( const QString& i_str );

//******************************************************************************
struct ZSSYSDLL_API SRequestSeqEntry
//******************************************************************************
{
public: // ctors and dtor
    SRequestSeqEntry();
    SRequestSeqEntry( const SRequestDscr& i_reqDscr );
    SRequestSeqEntry( const SRequestSeqEntry& i_reqSeqEntry );
    SRequestSeqEntry& operator = ( const SRequestSeqEntry& i_reqSeqEntry );
    ~SRequestSeqEntry();
public: // struct members
    ERequestEntryType m_reqEntryType;
    SRequestDscr      m_reqDscr;
    qint64            m_iReqIdPredecessor;
    QObject*          m_pObjFctExecute;
    TFctExecuteType1  m_fctExecuteType1;
    TFctExecuteType2  m_fctExecuteType2;
    QVariant          m_valExec;
    void*             m_pvExec;

}; // struct SRequestSeqEntry

//******************************************************************************
class ZSSYSDLL_API CRequestSequencer : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CRequestSequencer"; }
public: // ctors and dtor
    CRequestSequencer(
        const QString& i_strObjName,
        QObject*       i_pObjParent,
        bool           i_bKeepReqDscrInExecTree = false,
        bool           i_bTracingEnabled = true );
    ~CRequestSequencer();
signals:
    void requestAdded( qint64 i_iReqId );
    void requestRemoved( qint64 i_iReqId );
    void requestChanged( qint64 i_iReqIdOld, qint64 i_iReqIdNew );
signals:
    void started( qint64 i_iReqId );    // As set as the parent request id for the start method.
    void finished( qint64 i_iReqId );   // As set as the parent request id for the start method.
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    bool isMethodTracingEnabled() const { return m_bTracingEnabled; }  // Trace Server/Clients must not trace themselves (endless recursion)
public: // instance methods
    qint64 GetUniqueRequestId() const;
public: // instance methods
    void setKeepReqDscrInExecTree( bool i_bKeep );
    bool getKeepReqDscrInExecTree() const { return m_bKeepReqDscrInExecTree; }
public: // instance methods (To add an inactive group node. Inactive group nodes cannot be executed.)
    qint64 addGroup(
        const QString& i_strCreatorNameSpace,
        const QString& i_strCreatorClassName,
        QObject*       i_pObjCreator,       // An object name should have been assigned to the request creating object.
        int            i_request,
        const QString& i_strRequest,
        qint64         i_iReqIdParent,
        qint64         i_iReqIdPredecessor = -1 );
public: // instance methods (To add an active request node. Active request nodes cannot have children.)
    qint64 addRequest(
        const QString&   i_strCreatorNameSpace,
        const QString&   i_strCreatorClassName,
        QObject*         i_pObjCreator,     // An object name should have been assigned to the request creating object.
        int              i_request,
        const QString&   i_strRequest,
        QObject*         i_pObjFctExecute,
        TFctExecuteType1 i_fctExecute,
        const QVariant&  i_valExec,
        qint64           i_iReqIdParent,
        qint64           i_iReqIdPredecessor = -1,
        int              i_iTimeout_ms       = 0, // <= 0: Infinite (No Timeout)
        bool             i_bIsBlocking       = false );
    qint64 addRequest(
        const QString&   i_strCreatorNameSpace,
        const QString&   i_strCreatorClassName,
        QObject*         i_pObjCreator,     // An object name should have been assigned to the request creating object.
        int              i_request,
        const QString&   i_strRequest,
        QObject*         i_pObjFctExecute,
        TFctExecuteType2 i_fctExecute,
        void*            i_pvExec,
        qint64           i_iReqIdParent,
        qint64           i_iReqIdPredecessor = -1,
        int              i_iTimeout_ms       = 0, // <= 0: Infinite (No Timeout)
        bool             i_bIsBlocking       = false );
    void removeRequest( qint64 i_iReqId );
public: // instance methods
    void start( CRequest* i_pReqParent = nullptr );
public: // instance methods
    bool isSequenceFinished() const;                // Also checks whether the parent request is finished.
    bool areAllTopLevelRequestsFinished() const;    // Checks all top level requests (m_ariReqIds).
    bool isInProgress() const { return m_bInProgress; }
public: // instance methods
    void clear();
public: // instance methods
    QString getAddTrcInfoStr( int i_iDetailLevel = 0 ) const;
public: // instance methods
    void lock();
    void unlock();
public: // instance methods
    QList<qint64> getRequestIds() const { return m_ariReqIds; }   // The caller must "lock" and "unlock" the request sequencer while iterating through the list.
    SRequestSeqEntry* getRequestSeqEntry( qint64 i_iReqId ) const;
    CRequest* getRequest( qint64 i_iReqId ) const;
protected: // instance methods
    SRequestDscr add( SRequestSeqEntry* i_pReqSeqEntry );
    CRequest* startRequest( int i_iTimeout_ms, bool i_bIsBlocking, qint64 i_iReqIdParent, qint64 i_iReqId );
protected: // instance methods
    void clear( SRequestSeqEntry* i_pReqSeqEntry ); // Removes and deletes all children together with the referenced request descriptors. The specified entry itself will be removed and its request descriptor will be deleted. The entry itself will not be deleted.
protected: // instance methods
    void reset( SRequestSeqEntry* i_pReqSeqEntry, bool i_bRecursive = true );
    CRequest* getParentRequest( SRequestSeqEntry* i_pReqSeqEntry );
    void updateParentRequest( SRequestSeqEntry* i_pReqSeqEntry );
protected slots:
    void onParentRequestDestroyed( QObject* i_pReqParent );
protected: // instance methods
    void onParentRequestDestroyed( SRequestSeqEntry* i_pReqSeqEntry );
protected slots:
    void onRequestChanged( ZS::System::SRequestDscr i_reqDscr );
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
protected: // instance members
    QMutex*                         m_pMtx;
    bool                            m_bKeepReqDscrInExecTree;
    bool                            m_bMsgReqContinuePending;
    bool                            m_bInProgress;
    CRequest*                       m_pReqParent; // All top level requests will get the same parent request id provided on starting the sequence.
    qint64                          m_iReqIdParent;
    QList<qint64>                   m_ariReqIds;
    QHash<qint64,SRequestSeqEntry*> m_hshReqSeqs;
    QHash<qint64,CRequest*>         m_hshpReqs;
    bool                            m_bTracingEnabled;
    CTrcAdminObj*                   m_pTrcAdminObj;

}; // class CRequestSequencer

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_RequestSequencer_h
