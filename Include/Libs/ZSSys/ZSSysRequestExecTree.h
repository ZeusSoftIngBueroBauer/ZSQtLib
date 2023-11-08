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

#ifndef ZSSys_RequestExecTree_h
#define ZSSys_RequestExecTree_h

#include <QtCore/QHash>
#include <QtCore/QObject>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysRequest.h"

class QRecursiveMutex;
class QTimer;

namespace ZS
{
namespace System
{
//******************************************************************************
class ZSSYSDLL_API CRequestExecTree : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CRequestExecTree"; }
public: // class methods
    static CRequestExecTree* CreateInstance( QObject* i_pObjParent = nullptr );
    static CRequestExecTree* GetInstance();
    static void DestroyInstance();
public: // class methods
    static QString GetAddTrcInfoStr( qint64 i_iId, int i_iDetailLevel = 0 );
protected: // ctors and dtor (use class methods "CreateInstance" and "DestroyInstance" instead)
    CRequestExecTree( QObject* i_pObjParent = nullptr );
    virtual ~CRequestExecTree();
signals:
    void clearing();
    void cleared();
signals:
    void requestAdded( const ZS::System::SRequestDscr i_reqDscr );
    void requestRemoved( const ZS::System::SRequestDscr i_reqDscr );
    void requestChanged( const ZS::System::SRequestDscr i_reqDscr );
signals:
    void garbageCollectorChanged();
public: // instance methods
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void addRequest( CRequest* i_pReq );
    void addRequest( const SRequestDscr& i_reqDscr );
    void updateRequest( const SRequestDscr& i_reqDscr );
    void removeRequest( qint64 i_iId );
public: // instance methods
    void clear();
public: // instance methods
    SRequestDscr getDescriptor( qint64 i_iId, bool i_bTopMost = false ) const;
public: // instance methods
    QObject* getCreator( qint64 i_iId, bool i_bTopMost = false ) const;
    QString getCreatorNameSpace( qint64 i_iId, bool i_bTopMost = false ) const;
    QString getCreatorClassName( qint64 i_iId, bool i_bTopMost = false ) const;
    QString getCreatorThreadName( qint64 i_iId, bool i_bTopMost = false ) const;
    QString getCreatorObjectName( qint64 i_iId, bool i_bTopMost = false ) const;
    QString getRequestName( qint64 i_iId, bool i_bRecursive = false ) const;
public: // instance methods
    bool isBlockingRequest( qint64 i_iId, bool i_bRecursive = false ) const;
    int getTimeoutInMs( qint64 i_iId ) const;
public: // instance methods
    bool isAlive( qint64 i_iId ) const;
    bool isWaiting( qint64 i_iId ) const;
    bool waitTimedOut( qint64 i_iId ) const;
    bool wake( qint64 i_iId );
public: // instance methods
    // Only allowed for a blocking request currently waiting for finishing execution.
    // The request does not take ownership of the message.
    // The thread waiting at the blocking request to be finished is the receiver
    // of the result confirmation message and must delete the message.
    void setResultConfirmationMessage( qint64 i_iId, CMsgCon* i_pMsgCon );
    void setExecutionConfirmationMessage( qint64 i_iId, CMsgCon* i_pMsgCon );
public: // instance methods
    qint64 getParentId( qint64 i_iId, bool i_bTopMost = false ) const;
    bool isParentOf( qint64 i_iId, qint64 i_iChildId, bool i_bRecursive = false ) const;
    bool isChildOf( qint64 i_iId, qint64 i_iParentId, bool i_bRecursive = false ) const;
public: // instance methods
    void lock();
    void unlock();
public: // instance methods
    // The caller must "lock" and "unlock" the request tree while iterating through the hash.
    QHash<qint64,SRequestDscr*> getRequests() const;
    QList<SRequestDscr*> getTopMostParentRequests() const;
public: // instance methods
    // The caller must "lock" and "unlock" the request tree while using pointers to request descriptors.
    SRequestDscr* findRequest( qint64 i_iId ) const;
    SRequestDscr* findParentRequest( qint64 i_iId, qint64 i_iParentId, bool i_bRecursive = false ) const;
    SRequestDscr* findParentRequestOf( qint64 i_iId, bool i_bTopMost = false ) const;
    SRequestDscr* findChildRequest( qint64 i_iId, qint64 i_iChildId, bool i_bRecursive = false ) const;
public: // instance methods
    void setGarbageCollectorEnabled( bool i_bEnabled );
    bool isGarbageCollectorEnabled() const { return m_bTmrGarbageCollectorEnabled; }
    void setGarbageCollectorIntervalInSec( double i_fInterval_s );
    double getGarbageCollectorIntervalInSec() const { return m_fTmrGarbageCollecterInterval_s; }
    void setGarbageCollectorElapsedInSec( double i_fElapsed_s );
    double getGarbageCollectorElapsedInSec() const { return m_fTmrGarbageCollectorElapsed_s; }
protected slots:
    void onTmrGarbageCollectorTimeout();
protected: // class members
    static CRequestExecTree* s_pInstance;
    static QRecursiveMutex s_mtx;                  /*!< Mutex to protect the class methods for multithreaded access. */
protected: // instance members
    QRecursiveMutex*            m_pMtx;
    QHash<qint64,SRequestDscr*> m_hshRequestDscrs;  // Entries live longer than CRequest.
    QHash<qint64,CRequest*>     m_hshRequests;      // Only if the CRequest is still alive.
    bool                        m_bTmrGarbageCollectorEnabled;
    double                      m_fTmrGarbageCollecterInterval_s;
    double                      m_fTmrGarbageCollectorElapsed_s;
    QTimer*                     m_pTmrGarbageCollector;

}; // class CRequestExecTree

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_RequestExecTree_h
