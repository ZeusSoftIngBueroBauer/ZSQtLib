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

$Revision: 4 $

$Author: Christian Bauer $

$History: ZSSNMPCltStdPC.h $
 * 
 * *****************  Version 4  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSSNMPCltStdPC/Include
 * 
 * *****************  Version 3  *****************
 * User: Christian Bauer Date: 10.01.10   Time: 19:20
 * Updated in $/ZSSNMPCltStdPC/Include

*******************************************************************************/

#ifndef ZSSNMPCltStdPC_h
#define ZSSNMPCltStdPC_h

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
struct ZS::System::SErrResultInfo;
class CMsgReq;
class CModelErrLog;
}
namespace Trace
{
class CTrcAdminObj;
}
namespace SNMP
{
class CExplorer;
class COidNode;

class CModelSystem;
class CModelProcessors;
class CModelStorages;

/*******************************************************************************
gobal type definitions and constants
*******************************************************************************/

typedef enum {
    EStorageTypeOther           = 0, // 1.3.6.1.2.1.25.2.1.1
    EStorageTypeRam             = 1, // 1.3.6.1.2.1.25.2.1.2
    EStorageTypeVirtualMemory   = 2, // 1.3.6.1.2.1.25.2.1.3
    EStorageTypeFixedDisk       = 3, // 1.3.6.1.2.1.25.2.1.4
    EStorageTypeRemovableDisk   = 4, // 1.3.6.1.2.1.25.2.1.5
    EStorageTypeFloppyDisk      = 5, // 1.3.6.1.2.1.25.2.1.6
    EStorageTypeCompactDisk     = 6, // 1.3.6.1.2.1.25.2.1.7
    EStorageTypeRamDisk         = 7, // 1.3.6.1.2.1.25.2.1.8
    EStorageTypeFlashMemory     = 8, // 1.3.6.1.2.1.25.2.1.9
    EStorageTypeNetworkDisk     = 9, // 1.3.6.1.2.1.25.2.1.10
    EStorageTypeCount,
    EStorageTypeUndefined
}   EStorageType;
ZSSNMPDLL_API QString storageType2Str( int i_iStorageType );
ZSSNMPDLL_API QString storageType2OidStr( int i_iStorageType );
ZSSNMPDLL_API EStorageType str2StorageType( const QString& i_strStorageType );
ZSSNMPDLL_API EStorageType oidStr2StorageType( const QString& i_strOid );


//******************************************************************************
struct ZSSNMPDLL_API SOidsStdPC
//******************************************************************************
{
public: // default ctor
    SOidsStdPC();
public: // operators
    bool operator == ( SOidsStdPC& i_oidsOther );
    bool operator != ( SOidsStdPC& i_oidsOther );
public: // struct members
    COid m_oidSystem;
    COid m_oidSysDescr;
    COid m_oidSysName;
    COid m_oidHrProcessorLoad;
    COid m_oidHrStorageType;
    COid m_oidHrStorageDescr;
    COid m_oidHrStorageAllocationUnits;
    COid m_oidHrStorageSize;
    COid m_oidHrStorageUsed;

}; // struct SOidsStdPC


//******************************************************************************
struct ZSSNMPDLL_API SAutoUpdateIntervals
//******************************************************************************
{
public: // default ctor
    SAutoUpdateIntervals();
public: // operators
    bool operator == ( const SAutoUpdateIntervals& i_intervalsOther );
    bool operator != ( const SAutoUpdateIntervals& i_intervalsOther );
public: // struct members
    int m_iOpenSession_ms;
    int m_iAllValues_ms;
    int m_iProcessorValues_ms;
    int m_iStorageValues_ms;

}; // struct SAutoUpdateIntervals

//******************************************************************************
class ZSSNMPDLL_API CCltStdPC : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    typedef enum {
        EStateInit                 = 0,
        EStateStopped              = 1,
        EStateStartedSessionClosed = 2,
        EStateStartedSessionOpened = 3,
        EStateCount,
        EStateUndefined
    }   EState;
    static QString state2Str( int i_iState );
    typedef enum {
        ERequestNone                  = 0,
        ERequestStartSession          = 1,
        ERequestCloseSession          = 2,
        ERequestShutdown              = 3,
        ERequestUpdateAllValues       = 4,
        ERequestUpdateSystemValues    = 5,
        ERequestUpdateProcessorValues = 6,
        ERequestUpdateStorageTypes    = 7,
        ERequestUpdateStorageValues   = 8,
        ERequestCount,
        ERequestUndefined
    }   ERequest;
    static QString request2Str( int i_iRequest );
public: // ctors and dtor
    CCltStdPC(
        CExplorer*     i_pExplorer,
        const QString& i_strObjName,
        const QString& i_strObjNameExplorer = "",        // only used if explorer is not yet created
        const QString& i_strObjNameSession = "",         // only used if explorer is not yet created
        QSettings*     i_pSettingsFile = NULL,
        const QString& i_strSettingsKey = "",            // empty string if pSettings == NULL
        const QString& i_strSettingsKeyExplorer = "",    // empty string if pExplorer != NULL or pSettings == NULL
        int            i_iReadSettingsInterval_ms = 0 );
    virtual ~CCltStdPC();
signals:
    void stateChanged( QObject* i_pCltStdPC, int i_iState, int i_iRequestInProgress );
    void requestExecuted( QObject* i_pCltStdPC, int i_iRequestId, const ZS::SErrResultInfo& i_errResultInfo );
    void oidsChanged( QObject* i_pCltStdPC );
    void autoUpdateIntervalsChanged( QObject* i_pCltStdPC );
    void receivedSystemValues( QObject* i_pCltStdPC );
    void receivedProcessorValues( QObject* i_pCltStdPC );
    void receivedStorageValues( QObject* i_pCltStdPC );
public: // instance methods to trigger system startup and shutdown
    void start();
    void stop();
public: // instance methods
    CExplorer* getExplorer() { return m_pExplorer; }
public: // instance methods changing and reading oids
    void setOids( SOidsStdPC& i_oids );
    SOidsStdPC getOids() const;
protected:
    void saveOids();
    void readOids();
public: // instance methods retrieving values from the SNMP host
    SRequestResult updateAllValues();
    SRequestResult updateSystemValues();
    SRequestResult updateProcessorValues();
    SRequestResult updateStorageValues();
    void setAutoUpdateIntervals( const SAutoUpdateIntervals& i_autoUpdateIntervals );
    SAutoUpdateIntervals getAutoUpdateIntervals() const;
protected slots:
    void onAutoOpenSessionTimeout();
    void onAutoUpdateAllValuesTimeout();
    void onAutoUpdateProcessorValuesTimeout();
    void onAutoUpdateStorageValuesTimeout();
protected:
    void saveAutoUpdateIntervals();
    void readAutoUpdateIntervals();
public:
    void setRequestTimeoutInMs( ERequest i_request, int i_iTimeout_ms );
    int getRequestTimeoutInMs( ERequest i_request ) const;
    double getLastRecentRequestDurationInMs( ERequest i_request ) const;
public: // instance methods (reading settings from the ini file)
    QString getSettingsKey() const;
    void setReadSettingsIntervalInMs( int i_iInterval_ms );
    int getReadSettingsIntervalInMs() const;
protected slots:
    void onTimeoutReadSettings();
public: // instance methods exploring the SNMP host
    EState getCurrentState() const;
    ERequest getRequestInProgress() const;
    bool isRequestInProgressOrPending() const;
    CModelSystem* getModelSystem();
    CModelProcessors* getModelProcessors();
    CModelStorages* getModelStorages();
protected slots: // instance methods of internal state machine
    ZS::SRequestResult openSession();
    ZS::SRequestResult closeSession();
    void onExplorerTimeout();
    void onExplorerHostSettingsChanged( QObject* i_pExplorer );
    void onExplorerStateChanged( QObject* i_pExplorer, int i_iState, int i_iRequestInProgress );
    void onExplorerRequestExecuted( QObject* i_pExplorer, int i_iRequestId, const ZS::SErrResultInfo& i_errResultInfo );
protected:
    ERequest pendingMsgType2Request( CMsgReq* i_pMsgReq );
protected: // overridables of inherited class QObject (state machine)
    virtual bool event( QEvent* i_pEv );
protected: // instance members
    QString               m_strObjName;
    CExplorer*            m_pExplorer;
    QTimer*               m_pTimerExplorer;
    EState                m_state;
    ERequest              m_requestInProgress;
    ERequest              m_subRequestInProgress;
    int                   m_iIdRequestInProgress;
    SRequestResult        m_requestResult;
    double                m_fRequestStartTime_ms;
    int                   m_ariRequestTimeouts_ms[ERequestCount];
    double                m_arfRequestDurationLastRecent_ms[ERequestCount];
    QLinkedList<CMsgReq*> m_ptrListMsgReqPending;
    CModelErrLog*         m_pModelErrLog;
    QString               m_strSettingsKey;
    QSettings*            m_pSettingsFile;
    int                   m_iReadSettingsInterval_ms;
    QTimer*               m_pTimerReadSettings;
    SOidsStdPC            m_oids;
    SAutoUpdateIntervals  m_autoUpdateIntervals;
    QTimer*               m_pTimerAutoOpenSession;
    QTimer*               m_pTimerAutoUpdateAllValues;
    QTimer*               m_pTimerAutoUpdateProcessorValues;
    QTimer*               m_pTimerAutoUpdateStorageValues;
    bool                  m_bSystemValuesRead;
    CModelSystem*         m_pModelSystem;
    CModelProcessors*     m_pModelProcessors;
    CModelStorages*       m_pModelStorages;
private: // instance members
    Trace::CTrcAdminObj*  m_pTrcAdminObj;

}; // class CCltStdPC

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNMPCltStdPC_h
