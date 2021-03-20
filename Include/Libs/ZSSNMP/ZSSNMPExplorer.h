/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

--------------------------------------------------------------------------------

$Date: 22.01.10 22:11 $

$Revision: 8 $

$Author: Christian Bauer $

$History: ZSSNMPExplorer.h $
 * 
 * *****************  Version 8  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSSNMP/Include
 * 
 * *****************  Version 7  *****************
 * User: Christian Bauer Date: 10.01.10   Time: 19:19
 * Updated in $/ZSSNMP/Include

*******************************************************************************/

#ifndef ZSSNMPExplorer_h
#define ZSSNMPExplorer_h

#include <QtCore/qobject.h>
#include <QtCore/qlinkedlist.h>
#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSNMP/Include/ZSSNMPOid.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysAux.h"

class QSettings;
class QTimer;

namespace ZS
{
namespace System
{
class CMsg;
class CMsgReq;
class CModelErrLog;
}
namespace Trace
{
class CTrcAdminObj;
}

namespace SNMP
{
class COidNode;
class CInstanceNode;
class CModelOidNodes;
class CModelInstanceNodes;
class CSession;
class CSessionThread;

//******************************************************************************
struct ZSSNMPDLL_API SHostSettings
//******************************************************************************
{
public: // default ctor
    SHostSettings();
public: // operators
    bool operator == ( const SHostSettings& i_settingsOther ) const;
    bool operator != ( const SHostSettings& i_settingsOther ) const;
public: // struct members
    QString      m_strName;
    unsigned int m_uPort;
    QString      m_strCommunity;

}; // struct SHostSettings

//******************************************************************************
class ZSSNMPDLL_API CExplorer : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum {
        EStateInit                     = 0,
        EStateSessionClosed            = 1,
        EStateSessionOpened            = 2,
        EStateShutdown                 = 3,
        EStateWaitingForSessionCreated = 4,
        EStateWaitingForSessionOpened  = 5,
        EStateWaitingForSessionClosed  = 6,
        EStateWaitingForSessionDeleted = 7,
        EStateCount
    }   EState;
    static QString state2Str( int i_iState );
    typedef enum {
        ERequestNone         = 0,
        ERequestOpenSession  = 1,
        ERequestCloseSession = 2,
        ERequestShutdown     = 3,
        ERequestGetValue     = 4,
        ERequestGetNextValue = 5,
        ERequestGetBulk      = 6,
        ERequestWalk         = 7,
        ERequestCount,
        ERequestUndefined
    }   ERequest;
    static QString request2Str( int i_iRequest );
public: // ctors and dtor
    CExplorer(
        const QString& i_strObjName,
        const QString& i_strObjNameSession,
        QSettings*     i_pSettingsFile = NULL,
        const QString& i_strSettingsKey = "",
        bool           i_bSettingsFileAutoReadEnabled = false );
    virtual ~CExplorer();
signals:
    void stateChanged( QObject* i_pExplorer, int i_iState, int i_iRequestInProgress );
    void requestExecuted( QObject* i_pExplorer, int i_iRequestId, const ZS::SErrResultInfo& i_errResultInfo );
    void sendingMessage( QObject* i_pExplorer, ZS::CMsg* i_pMsg );
    void receivedMessage( QObject* i_pExplorer, ZS::CMsg* i_pMsg );
    void hostSettingsChanged( QObject* i_pExplorer );
    void requestTimeoutsChanged( QObject* i_pExplorer );
public: // instance methods
    QString getName() const;
    QString getSessionName() const;
public: // instance methods
    void setErrLogModelOutputEnabled( bool i_bEnabled );
    bool isErrLogModelOutputEnabled() const;
public: // instance methods to trigger system startup and shutdown
    SRequestResult openSession();
    SRequestResult closeSession();
    void shutdown();  // synchronous request
public: // instance methods changing and reading the host settings
    SRequestResult setHostSettings( const SHostSettings& i_hostSettings );
    SHostSettings getHostSettings() const;
    void saveHostSettings();
    void readHostSettings();
public: // instance methods sending SNMP data packages to the SNMP host
    SRequestResult requestValue( const QString& i_strOid );
    SRequestResult requestValues( const TOidPtrList& i_oidPtrList ); // The explorer takes ownership of the COid instances
    SRequestResult requestNextValue( const QString& i_strOid );
    SRequestResult requestBulk( const QString& i_strOid );
    SRequestResult walk( const QString& i_strOid );
    void abortRequest( int i_iRequestId ); // immediately aborts the specified request (synchronous request)
public: // instance methods (state machine timeouts)
    void setRequestTimeoutInMs( ERequest i_request, int i_iTimeout_ms );
    int getRequestTimeoutInMs( ERequest i_request ) const;
    double getLastRecentRequestDurationInMs( ERequest i_request ) const;
public: // instance methods (reading settings from the ini file)
    QString getSettingsKey() const;
    void setReadSettingsIntervalInMs( int i_iInterval_ms, bool i_bSettingsFileAutoReadEnabled = true );
    int getReadSettingsIntervalInMs() const;
protected slots:
    void onTimeoutReadSettings();
public: // instance methods exploring the SNMP host
    EState getCurrentState() const;
    ERequest getRequestInProgress() const;
    bool isRequestInProgressOrPending() const;
    COidNode* findOidNode( COid& i_oid );
    COidNode* findOidNode( const QString& i_strOid );
    CInstanceNode* findInstanceNode( const QString& i_strOid );
    CModelOidNodes* getModelOidNodes();
    CModelInstanceNodes* getModelInstanceNodes();
protected slots: // instance methods of internal state machine
    void onSessionTimeout();
    void onSessionCreated( QObject* i_pSessionThread, QObject* i_pSession );
protected:
    ERequest pendingMsgType2Request( CMsgReq* i_pMsgReq );
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
protected: // instance members
    QString               m_strObjName;
    QString               m_strObjNameSession;
    EState                m_state;
    ERequest              m_requestInProgress;
    int                   m_iIdRequestInProgress;
    int                   m_iIdRequestWaitingFor;
    double                m_fRequestStartTime_ms;
    int                   m_ariRequestTimeouts_ms[ERequestCount];
    double                m_arfRequestDurationLastRecent_ms[ERequestCount];
    COid*                 m_pOidRootWalkRequest;
    QLinkedList<CMsgReq*> m_ptrListMsgReqPending;
    CModelErrLog*         m_pModelErrLog;
    QString               m_strSettingsKey;
    QSettings*            m_pSettingsFile;
    bool                  m_bSettingsFileAutoReadEnabled;
    int                   m_iReadSettingsInterval_ms;
    QTimer*               m_pTimerReadSettings;
    SHostSettings         m_hostSettings;
    bool                  m_bErrLogModelOutputEnabled;
    CModelOidNodes*       m_pModelOidNodes;
    CModelInstanceNodes*  m_pModelInstanceNodes;
    QTimer*               m_pTimerSession;
    CSessionThread*       m_pSessionThread;
    CSession*             m_pSession;
private: // instance members
    Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CExplorer

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPExplorer_h
