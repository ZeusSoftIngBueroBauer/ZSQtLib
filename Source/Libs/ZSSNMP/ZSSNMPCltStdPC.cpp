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

$History: ZSSNMPCltStdPC.cpp $
 * 
 * *****************  Version 4  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSSNMPCltStdPC/Source
 * 
 * *****************  Version 3  *****************
 * User: Christian Bauer Date: 10.01.10   Time: 19:20
 * Updated in $/ZSSNMPCltStdPC/Source

*******************************************************************************/

#include <QtCore/qcoreapplication.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtimer.h>
#include "ZSSNMP/Include/ZSSNMPCltStdPC.h"
#include "ZSSNMP/Include/ZSSNMPCltStdPCMsg.h"
#include "ZSSNMP/Include/ZSSNMPCltStdPCProcessorsModel.h"
#include "ZSSNMP/Include/ZSSNMPCltStdPCStoragesModel.h"
#include "ZSSNMP/Include/ZSSNMPCltStdPCSystemModel.h"
#include "ZSSNMP/Include/ZSSNMPExplorer.h"
#include "ZSSNMP/Include/ZSSNMPOidNode.h"
#include "ZSSNMP/Include/ZSSNMPInstanceNode.h"
#include "ZSTrace/Include/ZSTrcAdminObj.h"
#include "ZSTrace/Include/ZSTrcMethod.h"
#include "ZSTrace/Include/ZSTrcServer.h"
#include "ZSSys/Include/ZSSysEnum.h"
#include "ZSSys/Include/ZSSysErrLogModel.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysTime.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;
using namespace ZS::Trace;


/*******************************************************************************
gobal type definitions and constants
*******************************************************************************/

//------------------------------------------------------------------------------
const ZS::SEnumEntry s_arEnumStrStorageTypes[EStorageTypeCount] =
//------------------------------------------------------------------------------
{
    /*  0 */ ZS::SEnumEntry( EStorageTypeOther,         "Other",         ".1.3.6.1.2.1.25.2.1.1"  ),
    /*  1 */ ZS::SEnumEntry( EStorageTypeRam,           "Ram",           ".1.3.6.1.2.1.25.2.1.2"  ),
    /*  2 */ ZS::SEnumEntry( EStorageTypeVirtualMemory, "VirtualMemory", ".1.3.6.1.2.1.25.2.1.3"  ),
    /*  3 */ ZS::SEnumEntry( EStorageTypeFixedDisk,     "FixedDisk",     ".1.3.6.1.2.1.25.2.1.4"  ),
    /*  4 */ ZS::SEnumEntry( EStorageTypeRemovableDisk, "RemovableDisk", ".1.3.6.1.2.1.25.2.1.5"  ),
    /*  5 */ ZS::SEnumEntry( EStorageTypeFloppyDisk,    "FloppyDisk",    ".1.3.6.1.2.1.25.2.1.6"  ),
    /*  6 */ ZS::SEnumEntry( EStorageTypeCompactDisk,   "CompactDisk",   ".1.3.6.1.2.1.25.2.1.7"  ),
    /*  7 */ ZS::SEnumEntry( EStorageTypeRamDisk,       "RamDisk",       ".1.3.6.1.2.1.25.2.1.8"  ),
    /*  8 */ ZS::SEnumEntry( EStorageTypeFlashMemory,   "FlashMemory",   ".1.3.6.1.2.1.25.2.1.9"  ),
    /*  9 */ ZS::SEnumEntry( EStorageTypeNetworkDisk,   "NetworkDisk",   ".1.3.6.1.2.1.25.2.1.10" )
};

//------------------------------------------------------------------------------
QString ZS::SNMP::storageType2Str( int i_iStorageType )
//------------------------------------------------------------------------------
{
    return ZS::SEnumEntry::val2Str(s_arEnumStrStorageTypes,EStorageTypeCount,i_iStorageType,0);
}

//------------------------------------------------------------------------------
QString ZS::SNMP::storageType2OidStr( int i_iStorageType )
//------------------------------------------------------------------------------
{
    return ZS::SEnumEntry::val2Str(s_arEnumStrStorageTypes,EStorageTypeCount,i_iStorageType,1);
}

//------------------------------------------------------------------------------
EStorageType ZS::SNMP::str2StorageType( const QString& i_strStorageType )
//------------------------------------------------------------------------------
{
    EStorageType storageType = EStorageTypeUndefined;
    int iStorageType = ZS::SEnumEntry::str2Int(s_arEnumStrStorageTypes,EStorageTypeCount,i_strStorageType,Qt::CaseInsensitive,0);
    if( iStorageType >= 0 && iStorageType < EStorageTypeCount )
    {
        storageType = static_cast<EStorageType>(iStorageType);
    }
    return storageType;
}

//------------------------------------------------------------------------------
EStorageType ZS::SNMP::oidStr2StorageType( const QString& i_strStorageType )
//------------------------------------------------------------------------------
{
    EStorageType storageType = EStorageTypeUndefined;
    int iStorageType = ZS::SEnumEntry::str2Int(s_arEnumStrStorageTypes,EStorageTypeCount,i_strStorageType,Qt::CaseInsensitive,1);
    if( iStorageType >= 0 && iStorageType < EStorageTypeCount )
    {
        storageType = static_cast<EStorageType>(iStorageType);
    }
    return storageType;
}


/*******************************************************************************
struct SOidsStdPC
*******************************************************************************/

/*==============================================================================
public: // default ctor
==============================================================================*/

//------------------------------------------------------------------------------
SOidsStdPC::SOidsStdPC() :
//------------------------------------------------------------------------------
    m_oidSystem(".1.3.6.1.2.1.1"),
    m_oidSysDescr(".1.3.6.1.2.1.1.1"),
    m_oidSysName(".1.3.6.1.2.1.1.5"),
    m_oidHrProcessorLoad(".1.3.6.1.2.1.25.3.3.1.2"),
    m_oidHrStorageType(".1.3.6.1.2.1.25.2.3.1.2"),
    m_oidHrStorageDescr(".1.3.6.1.2.1.25.2.3.1.3"),
    m_oidHrStorageAllocationUnits(".1.3.6.1.2.1.25.2.3.1.4"),
    m_oidHrStorageSize(".1.3.6.1.2.1.25.2.3.1.5"),
    m_oidHrStorageUsed(".1.3.6.1.2.1.25.2.3.1.6")
{
} // ctor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool SOidsStdPC::operator == ( SOidsStdPC& i_oidsOther )
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_oidSystem != i_oidsOther.m_oidSystem )
    {
        bEqual = false;
    }
    else if( m_oidSysDescr != i_oidsOther.m_oidSysDescr )
    {
        bEqual = false;
    }
    else if( m_oidSysName != i_oidsOther.m_oidSysName )
    {
        bEqual = false;
    }
    else if( m_oidHrProcessorLoad != i_oidsOther.m_oidHrProcessorLoad )
    {
        bEqual = false;
    }
    else if( m_oidHrStorageType != i_oidsOther.m_oidHrStorageType )
    {
        bEqual = false;
    }
    else if( m_oidHrStorageDescr != i_oidsOther.m_oidHrStorageDescr )
    {
        bEqual = false;
    }
    else if( m_oidHrStorageAllocationUnits != i_oidsOther.m_oidHrStorageAllocationUnits )
    {
        bEqual = false;
    }
    else if( m_oidHrStorageSize != i_oidsOther.m_oidHrStorageSize )
    {
        bEqual = false;
    }
    else if( m_oidHrStorageUsed != i_oidsOther.m_oidHrStorageUsed )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool SOidsStdPC::operator != ( SOidsStdPC& i_oidsOther )
//------------------------------------------------------------------------------
{
    return !(*this == i_oidsOther);
}


/*******************************************************************************
struct SAutoUpdateIntervals
*******************************************************************************/

/*==============================================================================
public: // default ctor
==============================================================================*/

//------------------------------------------------------------------------------
SAutoUpdateIntervals::SAutoUpdateIntervals() :
//------------------------------------------------------------------------------
    m_iOpenSession_ms(5000),
    m_iAllValues_ms(5000),
    m_iProcessorValues_ms(0),
    m_iStorageValues_ms(0)
{
} // ctor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool SAutoUpdateIntervals::operator == ( const SAutoUpdateIntervals& i_intervalsOther )
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if( m_iOpenSession_ms != i_intervalsOther.m_iOpenSession_ms )
    {
        bEqual = false;
    }
    else if( m_iAllValues_ms != i_intervalsOther.m_iAllValues_ms )
    {
        bEqual = false;
    }
    else if( m_iProcessorValues_ms != i_intervalsOther.m_iProcessorValues_ms )
    {
        bEqual = false;
    }
    else if( m_iStorageValues_ms != i_intervalsOther.m_iStorageValues_ms )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool SAutoUpdateIntervals::operator != ( const SAutoUpdateIntervals& i_intervalsOther )
//------------------------------------------------------------------------------
{
    return !(*this == i_intervalsOther);
}


/*******************************************************************************
class CCltStdPC : public QObject
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

////------------------------------------------------------------------------------
//const ZS::SEnumEntry s_arEnumStrStates[CCltStdPC::EStateCount] =
////------------------------------------------------------------------------------
//{
//    { CCltStdPC::EStateInit,                     "Init",                     "Init"                     },
//    { CCltStdPC::EStateSessionClosed,            "SessionClosed",            "SessionClosed"            },
//    { CCltStdPC::EStateSessionOpened,            "SessionOpened",            "SessionOpened"            },
//    { CCltStdPC::EStateShutdown,                 "Shutdown",                 "Shutdown"                 },
//    { CCltStdPC::EStateWaitingForSessionOpened,  "WaitingForSessionOpened",  "WaitingForSessionOpened"  },
//    { CCltStdPC::EStateWaitingForSessionClosed,  "WaitingForSessionClosed",  "WaitingForSessionClosed"  },
//    { CCltStdPC::EStateWaitingForSystemValues,   "WaitingForSystemValues",   "WaitingForSystemValues"   },
//    { CCltStdPC::EStateWaitingForProcessorLoads, "WaitingForProcessorLoads", "WaitingForProcessorLoads" },
//    { CCltStdPC::EStateWaitingForStorageTypes,   "WaitingForStorageTypes",   "WaitingForStorageTypes"   },
//    { CCltStdPC::EStateWaitingForStorageValues,  "WaitingForStorageValues",  "WaitingForStorageValues"  }
//};

//------------------------------------------------------------------------------
const ZS::SEnumEntry s_arEnumStrStates[CCltStdPC::EStateCount] =
//------------------------------------------------------------------------------
{
    ZS::SEnumEntry( CCltStdPC::EStateInit,                 "Init"                 ),
    ZS::SEnumEntry( CCltStdPC::EStateStopped,              "Stopped"              ),
    ZS::SEnumEntry( CCltStdPC::EStateStartedSessionClosed, "StartedSessionClosed" ),
    ZS::SEnumEntry( CCltStdPC::EStateStartedSessionOpened, "StartedSessionOpened" )
};

//------------------------------------------------------------------------------
QString CCltStdPC::state2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::SEnumEntry::val2Str(s_arEnumStrStates,EStateCount,i_iState);
}

//------------------------------------------------------------------------------
const ZS::SEnumEntry s_arEnumStrRequests[CCltStdPC::ERequestCount] =
//------------------------------------------------------------------------------
{
    ZS::SEnumEntry( CCltStdPC::ERequestNone,                  "None"                  ),
    ZS::SEnumEntry( CCltStdPC::ERequestStartSession,          "StartSession"          ),
    ZS::SEnumEntry( CCltStdPC::ERequestCloseSession,          "CloseSession"          ),
    ZS::SEnumEntry( CCltStdPC::ERequestShutdown,              "Shutdown"              ),
    ZS::SEnumEntry( CCltStdPC::ERequestUpdateAllValues,       "UpdateAllValues"       ),
    ZS::SEnumEntry( CCltStdPC::ERequestUpdateSystemValues,    "UpdateSystemValues"    ),
    ZS::SEnumEntry( CCltStdPC::ERequestUpdateProcessorValues, "UpdateProcessorValues" ),
    ZS::SEnumEntry( CCltStdPC::ERequestUpdateStorageTypes,    "UpdateStorageTypes"    ),
    ZS::SEnumEntry( CCltStdPC::ERequestUpdateStorageValues,   "UpdateStorageValues"   )
};

//------------------------------------------------------------------------------
QString CCltStdPC::request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return ZS::SEnumEntry::val2Str(s_arEnumStrRequests,ERequestCount,i_iRequest);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CCltStdPC::CCltStdPC(
    CExplorer*     i_pExplorer,
    const QString& i_strObjName,
    const QString& i_strObjNameExplorer,
    const QString& i_strObjNameSession,
    QSettings*     i_pSettingsFile,
    const QString& i_strSettingsKey,
    const QString& i_strSettingsKeyExplorer,
    int            i_iReadSettingsInterval_ms ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strObjName(i_strObjName),
    m_pExplorer(NULL),
    m_pTimerExplorer(NULL),
    m_state(EStateInit),
    m_requestInProgress(ERequestNone),
    m_subRequestInProgress(ERequestNone),
    m_iIdRequestInProgress(-1),
    m_requestResult(),
    m_fRequestStartTime_ms(0),
    //m_ariRequestTimeouts_ms[ERequestCount]
    //m_arfRequestDurationLastRecent_ms[ERequestCount]
    m_ptrListMsgReqPending(),
    m_pModelErrLog(NULL),
    m_strSettingsKey(i_strSettingsKey),
    m_pSettingsFile(i_pSettingsFile),
    m_iReadSettingsInterval_ms(i_iReadSettingsInterval_ms),
    m_pTimerReadSettings(NULL),
    m_oids(),
    m_autoUpdateIntervals(),
    m_pTimerAutoOpenSession(NULL),
    m_pTimerAutoUpdateAllValues(NULL),
    m_pTimerAutoUpdateProcessorValues(NULL),
    m_pTimerAutoUpdateStorageValues(NULL),
    m_bSystemValuesRead(false),
    m_pModelSystem(NULL),
    m_pModelProcessors(NULL),
    m_pModelStorages(NULL),
    m_pTrcAdminObj(NULL)
{
    int idx;

    // Setting all timeouts to default values
    for( idx = 0; idx < ERequestCount; idx++ )
    {
        m_ariRequestTimeouts_ms[idx] = 0;
        m_arfRequestDurationLastRecent_ms[idx] = 0.0;
    }

    if( m_strObjName.isEmpty() )
    {
        m_strObjName = "SNMPCltStdPC";
    }

    if( m_strSettingsKey.isEmpty() )
    {
        m_strSettingsKey = m_strObjName;
    }

    m_pTrcAdminObj = CTrcServer::getTrcAdminObj(
        /* traceType         */ ETraceTypeMth,
        /* strClassNameSpace */ "ZS::SNMP",
        /* strClassName      */ "CCltStdPC",
        /* strObjNameSpace   */ "",
        /* strObjName        */ m_strObjName );

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "ctor",
        /* strAddTrcInfo */ "" );

    if( m_pSettingsFile == NULL )
    {
        m_pModelErrLog = CModelErrLog::getInstance();
    }
    else
    {
        m_pModelErrLog = CModelErrLog::getInstance(m_pSettingsFile);

        readOids();
        saveOids(); // to ensure that the settings file exists with at least default entries

        readAutoUpdateIntervals();
        saveAutoUpdateIntervals(); // to ensure that the settings file exists with at least default entries

        m_pTimerReadSettings = new QTimer(this);

        if( !connect(
            /* pObjSender   */ m_pTimerReadSettings,
            /* szSignal     */ SIGNAL(timeout()),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTimeoutReadSettings()) ) )
        {
            throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
        }
        if( m_iReadSettingsInterval_ms > 0 )
        {
            m_pTimerReadSettings->start(m_iReadSettingsInterval_ms);
        }
    }

    // Models
    //-------

    m_pModelSystem = new CModelSystem(this);

    // As there is only one instance for both the system description and the system name node OID's
    // those OID's may be immediately evaluated after the node OID's have been read.
    SSystemValues* pSystemValues = new SSystemValues();
    pSystemValues->m_oidSysDescr = m_oids.m_oidSysDescr.toString() + ".0";
    pSystemValues->m_oidSysName = m_oids.m_oidSysName.toString() + ".0";
    m_pModelSystem->addValues(pSystemValues);
    pSystemValues = NULL;

    m_pModelProcessors = new CModelProcessors(this);
    m_pModelStorages = new CModelStorages(this);

    // Auto update timer
    //------------------

    m_pTimerAutoOpenSession = new QTimer(this);

    if( !connect(
        /* pObjSender   */ m_pTimerAutoOpenSession,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAutoOpenSessionTimeout()) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    m_pTimerAutoUpdateAllValues = new QTimer(this);

    if( !connect(
        /* pObjSender   */ m_pTimerAutoUpdateAllValues,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAutoUpdateAllValuesTimeout()) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    m_pTimerAutoUpdateProcessorValues = new QTimer(this);

    if( !connect(
        /* pObjSender   */ m_pTimerAutoUpdateProcessorValues,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAutoUpdateProcessorValuesTimeout()) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    m_pTimerAutoUpdateStorageValues = new QTimer(this);

    if( !connect(
        /* pObjSender   */ m_pTimerAutoUpdateStorageValues,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onAutoUpdateStorageValuesTimeout()) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    // State machine
    //--------------

    m_pTimerExplorer = new QTimer(this);
    m_pTimerExplorer->setSingleShot(true);

    if( !connect(
        /* pObjSender   */ m_pTimerExplorer,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onExplorerTimeout()) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pExplorer,
        /* szSignal     */ SIGNAL(hostSettingsChanged(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onExplorerHostSettingsChanged(QObject*)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pExplorer,
        /* szSignal     */ SIGNAL(stateChanged(QObject*,int,int)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onExplorerStateChanged(QObject*,int,int)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }
    if( !connect(
        /* pObjSender   */ m_pExplorer,
        /* szSignal     */ SIGNAL(requestExecuted(QObject*,int,const ZS::SErrResultInfo&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onExplorerRequestExecuted(QObject*,int,const ZS::SErrResultInfo&)) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    m_state = EStateStopped;

} // ctor

//------------------------------------------------------------------------------
CCltStdPC::~CCltStdPC()
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "dtor",
        /* strAddTrcInfo */ "" );


    try
    {
        delete m_pModelSystem;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pModelProcessors;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pModelStorages;
    }
    catch(...)
    {
    }

    m_pExplorer = NULL;
    m_pModelSystem = NULL;
    m_pModelProcessors = NULL;
    m_pModelStorages = NULL;

    m_pTimerAutoOpenSession = NULL;
    m_pTimerAutoUpdateAllValues = NULL;
    m_pTimerAutoUpdateProcessorValues = NULL;
    m_pTimerAutoUpdateStorageValues = NULL;
    m_pTimerExplorer = NULL;
    m_pTimerReadSettings = NULL;
    m_pSettingsFile = NULL;
    m_pModelErrLog = NULL;

    CTrcServer::releaseTrcAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = NULL;

} // dtor

/*==============================================================================
public: // instance methods to trigger system startup and shutdown
==============================================================================*/

//------------------------------------------------------------------------------
void CCltStdPC::start()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "start",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( m_autoUpdateIntervals.m_iOpenSession_ms > 0 && !m_pTimerAutoOpenSession->isActive() )
    {
        m_pTimerAutoOpenSession->start(m_autoUpdateIntervals.m_iOpenSession_ms);
    }
    if( m_autoUpdateIntervals.m_iAllValues_ms > 0 && !m_pTimerAutoUpdateAllValues->isActive() )
    {
        m_pTimerAutoUpdateAllValues->start(m_autoUpdateIntervals.m_iAllValues_ms);
    }
    if( m_autoUpdateIntervals.m_iProcessorValues_ms > 0 && !m_pTimerAutoUpdateProcessorValues->isActive() )
    {
        m_pTimerAutoUpdateProcessorValues->start(m_autoUpdateIntervals.m_iProcessorValues_ms);
    }
    if( m_autoUpdateIntervals.m_iStorageValues_ms > 0 && !m_pTimerAutoUpdateStorageValues->isActive() )
    {
        m_pTimerAutoUpdateStorageValues->start(m_autoUpdateIntervals.m_iStorageValues_ms);
    }

    if( m_pExplorer->getCurrentState() == CExplorer::EStateSessionOpened )
    {
        m_state = EStateStartedSessionOpened;
    }
    else
    {
        m_state = EStateStartedSessionClosed;

        onAutoOpenSessionTimeout();
    }

    emit stateChanged(this,m_state,m_requestInProgress);

} // start

//------------------------------------------------------------------------------
void CCltStdPC::stop()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "stop",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( m_pTimerAutoOpenSession->isActive() )
    {
        m_pTimerAutoOpenSession->stop();
    }
    if( m_pTimerAutoUpdateAllValues->isActive() )
    {
        m_pTimerAutoUpdateAllValues->stop();
    }
    if( m_pTimerAutoUpdateProcessorValues->isActive() )
    {
        m_pTimerAutoUpdateProcessorValues->stop();
    }
    if( m_pTimerAutoUpdateStorageValues->isActive() )
    {
        m_pTimerAutoUpdateStorageValues->stop();
    }

    m_state = EStateStopped;

    emit stateChanged(this,m_state,m_requestInProgress);

} // stop

/*==============================================================================
public: // instance methods changing and reading oids
==============================================================================*/

//------------------------------------------------------------------------------
void CCltStdPC::setOids( SOidsStdPC& i_oids )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
        strAddTrcInfo += ", system: " + i_oids.m_oidSystem.toString();
        strAddTrcInfo += ", sysDescr: " + i_oids.m_oidSysDescr.toString();
        strAddTrcInfo += ", sysName: " + i_oids.m_oidSysName.toString();
        strAddTrcInfo += ", hrProcLoad: " + i_oids.m_oidHrProcessorLoad.toString();
        strAddTrcInfo += ", hrStorType: " + i_oids.m_oidHrStorageType.toString();
        strAddTrcInfo += ", hrStorDescr: " + i_oids.m_oidHrStorageDescr.toString();
        strAddTrcInfo += ", hrStorSize: " + i_oids.m_oidHrStorageSize.toString();
        strAddTrcInfo += ", hrStorUnits: " + i_oids.m_oidHrStorageAllocationUnits.toString();
        strAddTrcInfo += ", hrStorUsed: " + i_oids.m_oidHrStorageUsed.toString();
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "setOids",
        /* strAddTrcInfo */ strAddTrcInfo );

    m_oids = i_oids;

} // setOids

//------------------------------------------------------------------------------
SOidsStdPC CCltStdPC::getOids() const
//------------------------------------------------------------------------------
{
    return m_oids;
}

/*==============================================================================
protected: // instance methods changing and reading the host settings
==============================================================================*/

//------------------------------------------------------------------------------
void CCltStdPC::saveOids()
//------------------------------------------------------------------------------
{
    if( m_pSettingsFile != NULL )
    {
        m_pSettingsFile->setValue( m_strSettingsKey + "/OidSystem", m_oids.m_oidSystem.toString() );
        m_pSettingsFile->setValue( m_strSettingsKey + "/OidSysDescr", m_oids.m_oidSysDescr.toString() );
        m_pSettingsFile->setValue( m_strSettingsKey + "/OidSysName", m_oids.m_oidSysName.toString() );
        m_pSettingsFile->setValue( m_strSettingsKey + "/OidHrProcessorLoad", m_oids.m_oidHrProcessorLoad.toString() );
        m_pSettingsFile->setValue( m_strSettingsKey + "/OidHrStorageType", m_oids.m_oidHrStorageType.toString() );
        m_pSettingsFile->setValue( m_strSettingsKey + "/OidHrStorageDescr", m_oids.m_oidHrStorageDescr.toString() );
        m_pSettingsFile->setValue( m_strSettingsKey + "/OidHrStorageSize", m_oids.m_oidHrStorageSize.toString() );
        m_pSettingsFile->setValue( m_strSettingsKey + "/OidHrStorageAllocationUnits", m_oids.m_oidHrStorageAllocationUnits.toString() );
        m_pSettingsFile->setValue( m_strSettingsKey + "/OidHrStorageUsed", m_oids.m_oidHrStorageUsed.toString() );
        m_pSettingsFile->sync();
    }

} // saveOids

//------------------------------------------------------------------------------
void CCltStdPC::readOids()
//------------------------------------------------------------------------------
{
    if( m_pSettingsFile != NULL )
    {
        m_pSettingsFile->sync();

        SOidsStdPC oidsCurr = m_oids;

        oidsCurr.m_oidSystem = m_pSettingsFile->value( m_strSettingsKey + "/OidSystem", oidsCurr.m_oidSystem.toString() ).toString();
        oidsCurr.m_oidSysDescr = m_pSettingsFile->value( m_strSettingsKey + "/OidSysDescr", oidsCurr.m_oidSysDescr.toString() ).toString();
        oidsCurr.m_oidSysName = m_pSettingsFile->value( m_strSettingsKey + "/OidSysName", oidsCurr.m_oidSysName.toString() ).toString();
        oidsCurr.m_oidHrProcessorLoad = m_pSettingsFile->value( m_strSettingsKey + "/OidHrProcessorLoad", oidsCurr.m_oidHrProcessorLoad.toString() ).toString();
        oidsCurr.m_oidHrStorageType = m_pSettingsFile->value( m_strSettingsKey + "/OidHrStorageType", oidsCurr.m_oidHrStorageType.toString() ).toString();
        oidsCurr.m_oidHrStorageDescr = m_pSettingsFile->value( m_strSettingsKey + "/OidHrStorageDescr", oidsCurr.m_oidHrStorageDescr.toString() ).toString();
        oidsCurr.m_oidHrStorageSize = m_pSettingsFile->value( m_strSettingsKey + "/OidHrStorageSize", oidsCurr.m_oidHrStorageSize.toString() ).toString();
        oidsCurr.m_oidHrStorageAllocationUnits = m_pSettingsFile->value( m_strSettingsKey + "/OidHrStorageAllocationUnits", oidsCurr.m_oidHrStorageAllocationUnits.toString() ).toString();
        oidsCurr.m_oidHrStorageUsed = m_pSettingsFile->value( m_strSettingsKey + "/OidHrStorageUsed", oidsCurr.m_oidHrStorageUsed.toString() ).toString();

        if( oidsCurr != m_oids )
        {
            bool bOidSystemChanged = false;
            bool bOidProcessorChanged = false;
            bool bOidStorageChanged = false;

            if( oidsCurr.m_oidSystem != m_oids.m_oidSystem )
            {
                bOidSystemChanged = true;
            }
            else if( oidsCurr.m_oidSysDescr != m_oids.m_oidSysDescr )
            {
                bOidSystemChanged = true;
            }
            else if( oidsCurr.m_oidSysName != m_oids.m_oidSysName )
            {
                bOidSystemChanged = true;
            }
            else if( oidsCurr.m_oidHrProcessorLoad != m_oids.m_oidHrProcessorLoad )
            {
                bOidProcessorChanged = true;
            }
            else if( oidsCurr.m_oidHrStorageType != m_oids.m_oidHrStorageType )
            {
                bOidStorageChanged = true;
            }
            else if( oidsCurr.m_oidHrStorageDescr != m_oids.m_oidHrStorageDescr )
            {
                bOidStorageChanged = true;
            }
            else if( oidsCurr.m_oidHrStorageSize != m_oids.m_oidHrStorageSize )
            {
                bOidStorageChanged = true;
            }
            else if( oidsCurr.m_oidHrStorageAllocationUnits != m_oids.m_oidHrStorageAllocationUnits )
            {
                bOidStorageChanged = true;
            }
            else if( oidsCurr.m_oidHrStorageUsed != m_oids.m_oidHrStorageUsed )
            {
                bOidStorageChanged = true;
            }

            m_oids = oidsCurr;

            if( bOidSystemChanged && m_pModelSystem != NULL && m_pModelSystem->rowCount() > 0 )
            {
                m_pModelSystem->clear();
            }
            if( bOidProcessorChanged && m_pModelProcessors != NULL && m_pModelProcessors->rowCount() > 0 )
            {
                m_pModelProcessors->clear();
            }
            if( bOidStorageChanged && m_pModelStorages != NULL && m_pModelStorages->rowCount() > 0 )
            {
                m_pModelStorages->clear();
            }

            emit oidsChanged(this);

            if( bOidSystemChanged && m_pModelSystem != NULL )
            {
                // As there is only one instance for both the system description and the system name node OID's
                // those OID's may be immediately evaluated after the node OID's have been read.
                SSystemValues* pSystemValues = new SSystemValues();
                pSystemValues->m_oidSysDescr = m_oids.m_oidSysDescr.toString() + ".0";
                pSystemValues->m_oidSysName = m_oids.m_oidSysName.toString() + ".0";
                m_pModelSystem->addValues(pSystemValues);
                pSystemValues = NULL;
                m_bSystemValuesRead = false;
            }
        }
    }

} // readOids

/*==============================================================================
public: // instance methods sending SNMP data packages to the SNMP host
==============================================================================*/

//------------------------------------------------------------------------------
ZS::SRequestResult CCltStdPC::updateAllValues()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "updateAllValues",
        /* strAddTrcInfo */ strAddTrcInfo );

    ZS::SRequestResult requestResult;

    // Please note that it is not sufficient just to check the "requestInProgress" member
    // variable to decide whether currently a request is in progress or not. The signals
    // "stateChanged" and "requestExecuted" are emitted if "requestInProgress == None".
    // Receiver of the signal may call one of the public request methods (as this one) as
    // an reentry within the slots connected to those signals. If we would not check here
    // if requests are pending on calling this method a new request would be started. But
    // after emitting the signal (in general within the event method of the class) the class
    // checks the request list for pending requests. If there are pending requests the next
    // pending request will be executed overwriting the event just started by.calling this method.
    if( m_requestInProgress != ERequestNone || m_ptrListMsgReqPending.count() > 0 )
    {
        CMsgReqWalk* pMsgReq = new CMsgReqWalk(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oid          */ COid() );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        m_ptrListMsgReqPending.append(pMsgReq);
        pMsgReq = NULL;
    }

    else // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() == 0 )
    {
        m_requestInProgress = ERequestUpdateAllValues;
        m_subRequestInProgress = ERequestUpdateSystemValues;

        emit stateChanged(this,m_state,m_requestInProgress);

        CMsgReqWalk* pMsgReq = new CMsgReqWalk(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oid          */ COid() );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
        pMsgReq = NULL;

    } // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() == 0 )

    return requestResult;

} // updateAllValues

//------------------------------------------------------------------------------
ZS::SRequestResult CCltStdPC::updateSystemValues()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "updateSystemValues",
        /* strAddTrcInfo */ strAddTrcInfo );

    ZS::SRequestResult requestResult;

    // Please note that it is not sufficient just to check the "requestInProgress" member
    // variable to decide whether currently a request is in progress or not. The signals
    // "stateChanged" and "requestExecuted" are emitted if "requestInProgress == None".
    // Receiver of the signal may call one of the public request methods (as this one) as
    // an reentry within the slots connected to those signals. If we would not check here
    // if requests are pending on calling this method a new request would be started. But
    // after emitting the signal (in general within the event method of the class) the class
    // checks the request list for pending requests. If there are pending requests the next
    // pending request will be executed overwriting the event just started by.calling this method.
    if( m_requestInProgress != ERequestNone || m_ptrListMsgReqPending.count() > 0 )
    {
        CMsgReqWalk* pMsgReq = new CMsgReqWalk(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oid          */ m_oids.m_oidSystem );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        m_ptrListMsgReqPending.append(pMsgReq);
        pMsgReq = NULL;
    }

    else // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() == 0 )
    {
        m_requestInProgress = ERequestUpdateSystemValues;
        emit stateChanged(this,m_state,m_requestInProgress);

        CMsgReqWalk* pMsgReq = new CMsgReqWalk(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oid          */ m_oids.m_oidSystem );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
        pMsgReq = NULL;

    } // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() == 0 )

    return requestResult;

} // updateSystemValues

//------------------------------------------------------------------------------
ZS::SRequestResult CCltStdPC::updateProcessorValues()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "updateProcessorValues",
        /* strAddTrcInfo */ strAddTrcInfo );

    ZS::SRequestResult requestResult;

    // Please note that it is not sufficient just to check the "requestInProgress" member
    // variable to decide whether currently a request is in progress or not. The signals
    // "stateChanged" and "requestExecuted" are emitted if "requestInProgress == None".
    // Receiver of the signal may call one of the public request methods (as this one) as
    // an reentry within the slots connected to those signals. If we would not check here
    // if requests are pending on calling this method a new request would be started. But
    // after emitting the signal (in general within the event method of the class) the class
    // checks the request list for pending requests. If there are pending requests the next
    // pending request will be executed overwriting the event just started by.calling this method.
    if( m_requestInProgress != ERequestNone || m_ptrListMsgReqPending.count() > 0 )
    {
        CMsgReqWalk* pMsgReq = new CMsgReqWalk(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oid          */ m_oids.m_oidHrProcessorLoad );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        m_ptrListMsgReqPending.append(pMsgReq);
        pMsgReq = NULL;
    }

    else // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() == 0 )
    {
        m_requestInProgress = ERequestUpdateProcessorValues;
        emit stateChanged(this,m_state,m_requestInProgress);

        CMsgReqWalk* pMsgReq = new CMsgReqWalk(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oid          */ m_oids.m_oidHrProcessorLoad );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
        pMsgReq = NULL;

    } // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() == 0 )

    return requestResult;

} // updateProcessorValues

//------------------------------------------------------------------------------
ZS::SRequestResult CCltStdPC::updateStorageValues()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "updateStorageValues",
        /* strAddTrcInfo */ strAddTrcInfo );

    ZS::SRequestResult requestResult;

    // Please note that it is not sufficient just to check the "requestInProgress" member
    // variable to decide whether currently a request is in progress or not. The signals
    // "stateChanged" and "requestExecuted" are emitted if "requestInProgress == None".
    // Receiver of the signal may call one of the public request methods (as this one) as
    // an reentry within the slots connected to those signals. If we would not check here
    // if requests are pending on calling this method a new request would be started. But
    // after emitting the signal (in general within the event method of the class) the class
    // checks the request list for pending requests. If there are pending requests the next
    // pending request will be executed overwriting the event just started by.calling this method.
    if( m_requestInProgress != ERequestNone || m_ptrListMsgReqPending.count() > 0 )
    {
        CMsgReqWalk* pMsgReq = new CMsgReqWalk(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oid          */ m_oids.m_oidHrStorageType );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        m_ptrListMsgReqPending.append(pMsgReq);
        pMsgReq = NULL;
    }

    else // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() == 0 )
    {
        m_requestInProgress = ERequestUpdateStorageValues;
        m_subRequestInProgress = ERequestUpdateStorageTypes;
        emit stateChanged(this,m_state,m_requestInProgress);

        CMsgReqWalk* pMsgReq = new CMsgReqWalk(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oid          */ m_oids.m_oidHrStorageType );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
        pMsgReq = NULL;

    } // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() == 0 )

    return requestResult;

} // updateStorageValues

//------------------------------------------------------------------------------
void CCltStdPC::setAutoUpdateIntervals( const SAutoUpdateIntervals& i_autoUpdateIntervals )
//------------------------------------------------------------------------------
{
    if( m_autoUpdateIntervals != i_autoUpdateIntervals )
    {
        if( m_pTimerAutoOpenSession->isActive() )
        {
            m_pTimerAutoOpenSession->stop();
        }
        if( m_pTimerAutoUpdateAllValues->isActive() )
        {
            m_pTimerAutoUpdateAllValues->stop();
        }
        if( m_pTimerAutoUpdateProcessorValues->isActive() )
        {
            m_pTimerAutoUpdateProcessorValues->stop();
        }
        if( m_pTimerAutoUpdateStorageValues->isActive() )
        {
            m_pTimerAutoUpdateStorageValues->stop();
        }

        m_autoUpdateIntervals = i_autoUpdateIntervals;

        saveAutoUpdateIntervals();

        if( m_state == EStateStartedSessionClosed || m_state == EStateStartedSessionOpened )
        {
            if( m_autoUpdateIntervals.m_iOpenSession_ms > 0 )
            {
                m_pTimerAutoOpenSession->start(m_autoUpdateIntervals.m_iOpenSession_ms);
            }
            if( m_autoUpdateIntervals.m_iAllValues_ms > 0 )
            {
                m_pTimerAutoUpdateAllValues->start(m_autoUpdateIntervals.m_iAllValues_ms);
            }
            if( m_autoUpdateIntervals.m_iProcessorValues_ms > 0 )
            {
                m_pTimerAutoUpdateProcessorValues->start(m_autoUpdateIntervals.m_iProcessorValues_ms);
            }
            if( m_autoUpdateIntervals.m_iStorageValues_ms > 0 )
            {
                m_pTimerAutoUpdateStorageValues->start(m_autoUpdateIntervals.m_iProcessorValues_ms);
            }
        }
        emit autoUpdateIntervalsChanged(this);
    }

} // setAutoUpdateIntervals

//------------------------------------------------------------------------------
SAutoUpdateIntervals CCltStdPC::getAutoUpdateIntervals() const
//------------------------------------------------------------------------------
{
    return m_autoUpdateIntervals;
}

//------------------------------------------------------------------------------
void CCltStdPC::onAutoOpenSessionTimeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(1) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 1,
        /* szMethod      */ "onAutoOpenSessionTimeout",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( m_state == EStateStartedSessionClosed && m_requestInProgress == ERequestNone )
    {
        // Send the request only if the request is not already pending.
        // Otherwise - if the interval is faster than the time necessary
        // for the request - the list of pending requests may grow and grow ...

        CMsgReq* pMsgReq;
        bool     bRequestFound = false;

        // .. search the request in the list of pending requests.
        QLinkedList<CMsgReq*>::iterator it = m_ptrListMsgReqPending.begin();

        while( it != m_ptrListMsgReqPending.end() )
        {
            pMsgReq = *it;

            if( pMsgReq->getMsgType() == EMsgTypeCltStdPCReqStartSession )
            {
                CMsgReqCltStdPCStartSession* pMsgReqStartSession = dynamic_cast<CMsgReqCltStdPCStartSession*>(pMsgReq);

                if( pMsgReqStartSession != NULL )
                {
                    bRequestFound = true;
                    break;
                }
            }
            ++it;
        }

        if( !bRequestFound )
        {
            openSession();
        }

    } // if( m_state == EStateStartedSessionClosed && m_requestInProgress == ERequestNone )

} // onAutoOpenSessionTimeout

//------------------------------------------------------------------------------
void CCltStdPC::onAutoUpdateAllValuesTimeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(1) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 1,
        /* szMethod      */ "onAutoUpdateAllValuesTimeout",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( m_state == EStateStartedSessionOpened && m_requestInProgress == ERequestNone )
    {
        // Send the update request only if the request is not already pending.
        // Otherwise - if the update interval is faster than the time necessary
        // for the update request - the list of pending requests may grow and grow ...

        CMsgReq* pMsgReq;
        bool     bRequestFound = false;

        // .. search the request in the list of pending requests.
        QLinkedList<CMsgReq*>::iterator it = m_ptrListMsgReqPending.begin();

        while( it != m_ptrListMsgReqPending.end() )
        {
            pMsgReq = *it;

            if( pMsgReq->getMsgType() == EMsgTypeReqWalk )
            {
                CMsgReqWalk* pMsgReqWalk = dynamic_cast<CMsgReqWalk*>(pMsgReq);

                if( pMsgReqWalk != NULL )
                {
                    COid* pOid = pMsgReqWalk->getOid(false);

                    if( pOid != NULL )
                    {
                        COid oidAllValues; // because of linux compilation

                        if( *pOid == oidAllValues )
                        {
                            bRequestFound = true;
                            break;
                        }
                    }
                }
            }
            ++it;
        }

        if( !bRequestFound )
        {
            updateAllValues();
        }

    } // if( m_state == EStateStartedSessionOpened && m_requestInProgress == ERequestNone )

} // onAutoUpdateAllValuesTimeout

//------------------------------------------------------------------------------
void CCltStdPC::onAutoUpdateProcessorValuesTimeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(1) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 1,
        /* szMethod      */ "onAutoUpdateProcessorValuesTimeout",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( m_state == EStateStartedSessionOpened && m_requestInProgress == ERequestNone )
    {
        // Send the update request only if the request is not already pending.
        // Otherwise - if the update interval is faster than the time necessary
        // for the update request - the list of pending requests may grow and grow ...

        CMsgReq* pMsgReq;
        bool     bRequestFound = false;

        // .. search the request in the list of pending requests.
        QLinkedList<CMsgReq*>::iterator it = m_ptrListMsgReqPending.begin();

        while( it != m_ptrListMsgReqPending.end() )
        {
            pMsgReq = *it;

            if( pMsgReq->getMsgType() == EMsgTypeReqWalk )
            {
                CMsgReqWalk* pMsgReqWalk = dynamic_cast<CMsgReqWalk*>(pMsgReq);

                if( pMsgReqWalk != NULL )
                {
                    COid* pOid = pMsgReqWalk->getOid(false);

                    if( pOid != NULL )
                    {
                        if( *pOid == m_oids.m_oidHrProcessorLoad )
                        {
                            bRequestFound = true;
                            break;
                        }
                    }
                }
            }
            ++it;
        }

        if( !bRequestFound )
        {
            updateProcessorValues();
        }

    } // if( m_state == EStateStartedSessionOpened && m_requestInProgress == ERequestNone )

} // onAutoUpdateProcessorValuesTimeout

//------------------------------------------------------------------------------
void CCltStdPC::onAutoUpdateStorageValuesTimeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(1) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 1,
        /* szMethod      */ "onAutoUpdateStorageValuesTimeout",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( m_state == EStateStartedSessionOpened && m_requestInProgress == ERequestNone )
    {
        // Send the update request only if the request is not already pending.
        // Otherwise - if the update interval is faster than the time necessary
        // for the update request - the list of pending requests may grow and grow ...

        CMsgReq* pMsgReq;
        bool     bRequestFound = false;

        // .. search the request in the list of pending requests.
        QLinkedList<CMsgReq*>::iterator it = m_ptrListMsgReqPending.begin();

        while( it != m_ptrListMsgReqPending.end() )
        {
            pMsgReq = *it;

            if( pMsgReq->getMsgType() == EMsgTypeReqWalk )
            {
                CMsgReqWalk* pMsgReqWalk = dynamic_cast<CMsgReqWalk*>(pMsgReq);

                if( pMsgReqWalk != NULL )
                {
                    COid* pOid = pMsgReqWalk->getOid(false);

                    if( pOid != NULL )
                    {
                        if( *pOid == m_oids.m_oidHrStorageType )
                        {
                            bRequestFound = true;
                            break;
                        }
                    }
                }
            }
            ++it;
        }

        if( !bRequestFound )
        {
            updateStorageValues();
        }

    } // if( m_state == EStateStartedSessionOpened && m_requestInProgress == ERequestNone )

} // onAutoUpdateStorageValuesTimeout

//------------------------------------------------------------------------------
void CCltStdPC::saveAutoUpdateIntervals()
//------------------------------------------------------------------------------
{
    if( m_pSettingsFile != NULL )
    {
        m_pSettingsFile->setValue( m_strSettingsKey + "/AutoOpenSessionIntervalInMs", m_autoUpdateIntervals.m_iOpenSession_ms );
        m_pSettingsFile->setValue( m_strSettingsKey + "/AutoUpdateIntervalAllValuesInMs", m_autoUpdateIntervals.m_iAllValues_ms );
        m_pSettingsFile->setValue( m_strSettingsKey + "/AutoUpdateIntervalProcessorValuesInMs", m_autoUpdateIntervals.m_iProcessorValues_ms );
        m_pSettingsFile->setValue( m_strSettingsKey + "/AutoUpdateIntervalStorageValuesInMs", m_autoUpdateIntervals.m_iStorageValues_ms );
        m_pSettingsFile->sync();
    }

} // saveAutoUpdateIntervals

//------------------------------------------------------------------------------
void CCltStdPC::readAutoUpdateIntervals()
//------------------------------------------------------------------------------
{
    if( m_pSettingsFile != NULL )
    {
        m_pSettingsFile->sync();

        SAutoUpdateIntervals autoUpdateIntervalsCurr = m_autoUpdateIntervals;

        autoUpdateIntervalsCurr.m_iOpenSession_ms     = m_pSettingsFile->value( m_strSettingsKey + "/AutoOpenSessionIntervalInMs", m_autoUpdateIntervals.m_iOpenSession_ms ).toInt();
        autoUpdateIntervalsCurr.m_iAllValues_ms       = m_pSettingsFile->value( m_strSettingsKey + "/AutoUpdateIntervalAllValuesInMs", autoUpdateIntervalsCurr.m_iAllValues_ms ).toInt();
        autoUpdateIntervalsCurr.m_iProcessorValues_ms = m_pSettingsFile->value( m_strSettingsKey + "/AutoUpdateIntervalProcessorValuesInMs", autoUpdateIntervalsCurr.m_iProcessorValues_ms ).toInt();
        autoUpdateIntervalsCurr.m_iStorageValues_ms   = m_pSettingsFile->value( m_strSettingsKey + "/AutoUpdateIntervalStorageValuesInMs", autoUpdateIntervalsCurr.m_iStorageValues_ms ).toInt();

        if( autoUpdateIntervalsCurr != m_autoUpdateIntervals )
        {
            m_autoUpdateIntervals = autoUpdateIntervalsCurr;

            emit autoUpdateIntervalsChanged(this);
        }
    }

} // readAutoUpdateIntervals

/*==============================================================================
public: // instance methods (state machine timeouts)
==============================================================================*/

//------------------------------------------------------------------------------
void CCltStdPC::setRequestTimeoutInMs( ERequest i_request, int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    if( i_request >= 0 && i_request < ERequestCount )
    {
        m_ariRequestTimeouts_ms[i_request] = i_iTimeout_ms;
    }
}

//------------------------------------------------------------------------------
int CCltStdPC::getRequestTimeoutInMs( ERequest i_request ) const
//------------------------------------------------------------------------------
{
    int iTimeout_ms = 10000;

    if( i_request >= 0 && i_request < ERequestCount )
    {
        iTimeout_ms = m_ariRequestTimeouts_ms[i_request];
    }
    return iTimeout_ms;

} // getRequestTimeoutInMs

//------------------------------------------------------------------------------
double CCltStdPC::getLastRecentRequestDurationInMs( ERequest i_request ) const
//------------------------------------------------------------------------------
{
    double fDuration_ms = 10000.0;

    if( i_request >= 0 && i_request < ERequestCount )
    {
        fDuration_ms = m_arfRequestDurationLastRecent_ms[i_request];
    }
    return fDuration_ms;

} // getLastRecentRequestDurationInMs

/*==============================================================================
public: // instance methods (reading settings from the ini file)
==============================================================================*/

//------------------------------------------------------------------------------
QString CCltStdPC::getSettingsKey() const
//------------------------------------------------------------------------------
{
    return m_strSettingsKey;
}

//------------------------------------------------------------------------------
int CCltStdPC::getReadSettingsIntervalInMs() const
//------------------------------------------------------------------------------
{
    return m_iReadSettingsInterval_ms;
}

//------------------------------------------------------------------------------
void CCltStdPC::setReadSettingsIntervalInMs( int i_iInterval_ms )
//------------------------------------------------------------------------------
{
    if( m_iReadSettingsInterval_ms != i_iInterval_ms )
    {
        if( m_pTimerReadSettings != NULL && m_pTimerReadSettings->isActive() )
        {
            m_pTimerReadSettings->stop();
        }

        m_iReadSettingsInterval_ms = i_iInterval_ms;

        if( m_pTimerReadSettings != NULL && m_iReadSettingsInterval_ms > 0 )
        {
            m_pTimerReadSettings->start(m_iReadSettingsInterval_ms);
        }
    }

} // setReadSettingsIntervalInMs

/*==============================================================================
protected slots: // continuously read the settings file
==============================================================================*/

//------------------------------------------------------------------------------
void CCltStdPC::onTimeoutReadSettings()
//------------------------------------------------------------------------------
{
    readOids();
    readAutoUpdateIntervals();

} // onTimeoutReadSettings

/*==============================================================================
public: // instance methods exploring the SNMP host
==============================================================================*/

//------------------------------------------------------------------------------
CCltStdPC::EState CCltStdPC::getCurrentState() const
//------------------------------------------------------------------------------
{
    return m_state;
}

//------------------------------------------------------------------------------
CCltStdPC::ERequest CCltStdPC::getRequestInProgress() const
//------------------------------------------------------------------------------
{
    return m_requestInProgress;
}

//------------------------------------------------------------------------------
bool CCltStdPC::isRequestInProgressOrPending() const
//------------------------------------------------------------------------------
{
    return ( (m_requestInProgress != ERequestNone) || (m_ptrListMsgReqPending.count() > 0) );
}

//------------------------------------------------------------------------------
CModelSystem* CCltStdPC::getModelSystem()
//------------------------------------------------------------------------------
{
    return m_pModelSystem;
}

//------------------------------------------------------------------------------
CModelProcessors* CCltStdPC::getModelProcessors()
//------------------------------------------------------------------------------
{
    return m_pModelProcessors;
}

//------------------------------------------------------------------------------
CModelStorages* CCltStdPC::getModelStorages()
//------------------------------------------------------------------------------
{
    return m_pModelStorages;
}

/*==============================================================================
protected slots: // instance methods of internal state machine
==============================================================================*/

//------------------------------------------------------------------------------
ZS::SRequestResult CCltStdPC::openSession()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(1) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 1,
        /* szMethod      */ "openSession",
        /* strAddTrcInfo */ strAddTrcInfo );

    ZS::SRequestResult requestResult;

    // Please note that it is not sufficient just to check the "requestInProgress" member
    // variable to decide whether currently a request is in progress or not. The signals
    // "stateChanged" and "requestExecuted" are emitted if "requestInProgress == None".
    // Receiver of the signal may call one of the public request methods (as this one) as
    // an reentry within the slots connected to those signals. If we would not check here
    // if requests are pending on calling this method a new request would be started. But
    // after emitting the signal (in general within the event method of the class) the class
    // checks the request list for pending requests. If there are pending requests the next
    // pending request will be executed overwriting the event just started by.calling this method.
    if( m_requestInProgress != ERequestNone || m_ptrListMsgReqPending.count() > 0 )
    {
        CMsgReqCltStdPCStartSession* pMsgReq = new CMsgReqCltStdPCStartSession(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oids         */ m_oids );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        m_ptrListMsgReqPending.append(pMsgReq);
        pMsgReq = NULL;
    }

    else // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() == 0 )
    {
        m_requestInProgress = ERequestStartSession; // set it here and not before switch because of addInfo in state machine exception
        emit stateChanged(this,m_state,m_requestInProgress);

        CMsgReqCltStdPCStartSession* pMsgReq = new CMsgReqCltStdPCStartSession(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oids         */ m_oids );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
        pMsgReq = NULL;

    } // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() == 0 )

    return requestResult;

} // openSession

//------------------------------------------------------------------------------
ZS::SRequestResult CCltStdPC::closeSession()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(1) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 1,
        /* szMethod      */ "closeSession",
        /* strAddTrcInfo */ strAddTrcInfo );

    ZS::SRequestResult requestResult;

    // Please note that it is not sufficient just to check the "requestInProgress" member
    // variable to decide whether currently a request is in progress or not. The signals
    // "stateChanged" and "requestExecuted" are emitted if "requestInProgress == None".
    // Receiver of the signal may call one of the public request methods (as this one) as
    // an reentry within the slots connected to those signals. If we would not check here
    // if requests are pending on calling this method a new request would be started. But
    // after emitting the signal (in general within the event method of the class) the class
    // checks the request list for pending requests. If there are pending requests the next
    // pending request will be executed overwriting the event just started by.calling this method.
    if( m_requestInProgress != ERequestNone || m_ptrListMsgReqPending.count() > 0 )
    {
        CMsgReqCloseSession* pMsgReq = new CMsgReqCloseSession(
            /* pObjSender   */ this,
            /* pObjReceiver */ this );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        m_ptrListMsgReqPending.append(pMsgReq);
        pMsgReq = NULL;
    }

    else // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() == 0 )
    {
        m_requestInProgress = ERequestCloseSession; // set it here and not before switch because of addInfo in state machine exception
        emit stateChanged(this,m_state,m_requestInProgress);

        CMsgReqCloseSession* pMsgReq = new CMsgReqCloseSession(
            /* pObjSender   */ this,
            /* pObjReceiver */ this );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
        pMsgReq = NULL;

    } // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() == 0 )

    return requestResult;

} // closeSession

//------------------------------------------------------------------------------
void CCltStdPC::onExplorerTimeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "onExplorerTimeout",
        /* strAddTrcInfo */ strAddTrcInfo );

    CMsgIndTimeout* pMsgInd = new CMsgIndTimeout(
        /* pObjSender   */ this,
        /* pObjReceiver */ this );
    QCoreApplication::postEvent(pMsgInd->getReceiver(),pMsgInd);
    pMsgInd = NULL;

} // onExplorerTimeout

//------------------------------------------------------------------------------
void CCltStdPC::onExplorerHostSettingsChanged( QObject* /*i_pExplorer*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "onExplorerHostSettingsChanged",
        /* strAddTrcInfo */ strAddTrcInfo );

    SSystemValues* pSystemValues = m_pModelSystem->getValues(0);

    if( pSystemValues == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalProgramError,"System OID's not in model");
    }

    // Abort current update requests ...
    switch( m_requestInProgress )
    {
        case ERequestStartSession:
        case ERequestUpdateAllValues:
        case ERequestUpdateSystemValues:
        case ERequestUpdateProcessorValues:
        case ERequestUpdateStorageTypes:
        case ERequestUpdateStorageValues:
        {
            SErrResultInfo errResultInfo;

            errResultInfo.setSeverity(ZS::EResultSeverityError);
            errResultInfo.setResult(ZS::EResultRequestAborted);
            errResultInfo.setResultStr(ZS::result2Str(ZS::EResultRequestAborted));
            errResultInfo.setErrSource(m_strObjName);
            errResultInfo.setAddErrInfoDscr("Host Settings Have Been Changed");

            m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
            m_requestInProgress = ERequestNone;
            m_subRequestInProgress = ERequestNone;
            emit requestExecuted(this,m_iIdRequestInProgress,errResultInfo);
            emit stateChanged(this,m_state,m_requestInProgress);

            // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
            // "requestInProgress" has been set to "None".and that receivers of the signals may call
            // the public request methods of the class as an reentry within the slots connected to
            // those signals. Those reentry requests need to be deferred and inserted into the list
            // of pending requests if currently requests are pending. Otherwise an already pending
            // request would overwrite the new request as this pending request will be executed now.

            // The request has been executed. Check if there are pending requests ...
            if( m_ptrListMsgReqPending.count() > 0 )
            {
                CMsgReq* pMsgReqPending = m_ptrListMsgReqPending.takeFirst();

                if( pMsgReqPending != NULL )
                {
                    m_requestInProgress = pendingMsgType2Request(pMsgReqPending);

                    switch( m_requestInProgress )
                    {
                        case ERequestUpdateAllValues:
                        {
                            m_subRequestInProgress = ERequestUpdateSystemValues;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                    emit stateChanged(this,m_state,m_requestInProgress);
                    QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                    pMsgReqPending = NULL;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    } // switch( m_requestInProgress )

    pSystemValues->m_dateTimeUpdate = QDateTime();
    pSystemValues->m_strSysDescrValue = "";
    pSystemValues->m_strSysNameValue  = "";
    m_pModelSystem->setValues(0,pSystemValues);
    m_bSystemValuesRead = false;

    m_pModelProcessors->clear();
    m_pModelStorages->clear();

} // onExplorerHostSettingsChanged

//------------------------------------------------------------------------------
void CCltStdPC::onExplorerStateChanged( QObject*, int i_iState, int i_iRequestInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(i_iState);
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(i_iRequestInProgress);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "onExplorerStateChanged",
        /* strAddTrcInfo */ strAddTrcInfo );

    EState statePrev = m_state;

    if( i_iState == CExplorer::EStateSessionOpened )
    {
        if( m_state == EStateStartedSessionClosed )
        {
            m_state = EStateStartedSessionOpened;
        }
    }
    else
    {
        if( m_state == EStateStartedSessionOpened )
        {
            m_state = EStateStartedSessionClosed;
        }
    }

    if( statePrev != m_state )
    {
        emit stateChanged(this,m_state,m_requestInProgress);
    }

} // onExplorerStateChanged

//------------------------------------------------------------------------------
void CCltStdPC::onExplorerRequestExecuted( QObject*, int i_iRequestId, const ZS::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
        strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
        strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
        strAddTrcInfo += ", RequestIdWaitingFor:" + QString::number(m_requestResult.m_iPendingRequestId);
        strAddTrcInfo += ", RequestIdReceived:" + QString::number(i_iRequestId);
        strAddTrcInfo += ", Result:" + i_errResultInfo.getResultStr();
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* szMethod      */ "onExplorerRequestExecuted",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( m_requestResult.m_iPendingRequestId == i_iRequestId )
    {
        if( m_pTimerExplorer->isActive() )
        {
            m_pTimerExplorer->stop();
        }

        if( m_requestInProgress == ERequestStartSession || m_requestInProgress == ERequestCloseSession )
        {
            EResult result = i_errResultInfo.getResult();

            if( result != EResultSuccess )
            {
                QString strProposal;

                switch( result )
                {
                    case EResultSessionOpenError:
                    {
                        strProposal = "Check Host Settings (IP address, community string, etc.)";
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                if( m_pModelErrLog != NULL && !i_errResultInfo.isAddedToErrLogModel() )
                {
                    m_pModelErrLog->addEntry(
                        /* dateTime    */ QDateTime::currentDateTime(),
                        /* severity    */ i_errResultInfo.getSeverity(),
                        /* iErrNr      */ i_errResultInfo.getResult(),
                        /* strErrNr    */ i_errResultInfo.getResultStr(),
                        /* strSource   */ i_errResultInfo.getErrSource(),
                        /* strAddTrcInfo  */ i_errResultInfo.getAddErrInfoDscr(),
                        /* strProposal */ strProposal );
                }

                m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                m_requestInProgress = ERequestNone;
                m_subRequestInProgress = ERequestNone;
                emit requestExecuted(this,m_iIdRequestInProgress,i_errResultInfo);
                emit stateChanged(this,m_state,m_requestInProgress);

            } // if( result != EResultSuccess )

            else // if( result == EResultSuccess )
            {
                m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                m_requestInProgress = ERequestNone;
                m_subRequestInProgress = ERequestNone;
                emit requestExecuted(this,m_iIdRequestInProgress,i_errResultInfo);
                emit stateChanged(this,m_state,m_requestInProgress);

            } // if( result == EResultSuccess )

            // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
            // "requestInProgress" has been set to "None".and that receivers of the signals may call
            // the public request methods of the class as an reentry within the slots connected to
            // those signals. Those reentry requests need to be deferred and inserted into the list
            // of pending requests if currently requests are pending. Otherwise an already pending
            // request would overwrite the new request as this pending request will be executed now.

        } // if( m_requestInProgress == ERequestStartSession || m_requestInProgress == ERequestCloseSession )

        else if( m_requestInProgress == ERequestUpdateSystemValues || m_subRequestInProgress == ERequestUpdateSystemValues )
        {
            SSystemValues* pSystemValues = m_pModelSystem->getValues(0);

            if( pSystemValues == NULL )
            {
                throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalProgramError,"System OID's not in model");
            }

            EResult result = i_errResultInfo.getResult();

            if( result != EResultSuccess )
            {
                if( m_pModelErrLog != NULL && !i_errResultInfo.isAddedToErrLogModel() )
                {
                    m_pModelErrLog->addEntry(
                        /* dateTime    */ QDateTime::currentDateTime(),
                        /* severity    */ i_errResultInfo.getSeverity(),
                        /* iErrNr      */ i_errResultInfo.getResult(),
                        /* strErrNr    */ i_errResultInfo.getResultStr(),
                        /* strSource   */ i_errResultInfo.getErrSource(),
                        /* strAddTrcInfo  */ i_errResultInfo.getAddErrInfoDscr(),
                        /* strProposal */ "Check Settings (IP address, community string, etc.)" );
                }

                m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;

                pSystemValues->m_dateTimeUpdate = QDateTime();
                pSystemValues->m_strSysDescrValue = "";
                pSystemValues->m_strSysNameValue  = "";
                m_pModelSystem->setValues(0,pSystemValues);

                m_bSystemValuesRead = false;

                m_requestInProgress = ERequestNone;
                m_subRequestInProgress = ERequestNone;
                emit requestExecuted(this,m_iIdRequestInProgress,i_errResultInfo);
                emit stateChanged(this,m_state,m_requestInProgress);

                // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                // "requestInProgress" has been set to "None".and that receivers of the signals may call
                // the public request methods of the class as an reentry within the slots connected to
                // those signals. Those reentry requests need to be deferred and inserted into the list
                // of pending requests if currently requests are pending. Otherwise an already pending
                // request would overwrite the new request as this pending request will be executed now.

                // If the system values cannot be read the SNMP service is very likely not accessible.
                // Either the community string is wrong or the SNMP service is not installed or running.
                // If no other request is currently pending we are going to close the session.
                if( m_ptrListMsgReqPending.count() == 0 )
                {
                    closeSession();
                }
            } // if( result != EResultSuccess )

            else // if( result == EResultSuccess )
            {
                CInstanceNode* pInstanceNode;
                CValue*        pValue;

                // Get (or better "find") system description value
                //------------------------------------------------

                // Please note that we cannot store the reference to the node in our class as
                // the explorer may delete the whole model e.g. if the host setting are changed.
                COidNode* pOidNodeSysDescr = m_pExplorer->findOidNode(m_oids.m_oidSysDescr);

                if( pOidNodeSysDescr != NULL )
                {
                    pInstanceNode = pOidNodeSysDescr->getInstanceNode(0);

                    if( pInstanceNode != NULL )
                    {
                        pValue = pInstanceNode->getValue();

                        if( pValue != NULL )
                        {
                            pSystemValues->m_strSysDescrValue = pValue->getValueString();
                        }
                    }
                }

                // Get (or better "find") system name value
                //-----------------------------------------

                // Please note that we cannot store the reference to the node in our class as
                // the explorer may delete the whole model e.g. if the host setting are changed.
                COidNode* pOidNodeSysName = m_pExplorer->findOidNode(m_oids.m_oidSysName);

                if( pOidNodeSysName != NULL )
                {
                    pInstanceNode = pOidNodeSysName->getInstanceNode(0);

                    if( pInstanceNode != NULL )
                    {
                        pValue = pInstanceNode->getValue();

                        if( pValue != NULL )
                        {
                            pSystemValues->m_strSysNameValue = pValue->getValueString();
                        }
                    }
                }

                // Update values in the model
                //---------------------------

                pSystemValues->m_dateTimeUpdate = QDateTime::currentDateTime();
                m_pModelSystem->setValues(0,pSystemValues);

                m_bSystemValuesRead = true;

                emit receivedSystemValues(this);

                if( m_requestInProgress == ERequestUpdateSystemValues )
                {
                    // The request has been executed
                    //------------------------------

                    m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                    m_requestInProgress = ERequestNone;
                    emit requestExecuted(this,m_iIdRequestInProgress,i_errResultInfo);
                    emit stateChanged(this,m_state,m_requestInProgress);

                    // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                    // "requestInProgress" has been set to "None".and that receivers of the signals may call
                    // the public request methods of the class as an reentry within the slots connected to
                    // those signals. Those reentry requests need to be deferred and inserted into the list
                    // of pending requests if currently requests are pending. Otherwise an already pending
                    // request would overwrite the new request as this pending request will be executed now.

                } // if( m_requestInProgress == ERequestUpdateSystemValues )

                else // if( m_requestInProgress == ERequestUpdateAllValues )
                {
                    // Query processor loads
                    //----------------------

                    m_subRequestInProgress = ERequestUpdateProcessorValues;
                    emit stateChanged(this,m_state,m_requestInProgress);

                    m_requestResult = m_pExplorer->walk(m_oids.m_oidHrProcessorLoad.toString());

                    if( !m_requestResult.isAsynchronousRequest() )
                    {
                        m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;

                        // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                        // "requestInProgress" has been set to "None".and that receivers of the signals may call
                        // the public request methods of the class as an reentry within the slots connected to
                        // those signals. Those reentry requests need to be deferred and inserted into the list
                        // of pending requests if currently requests are pending. Otherwise an already pending
                        // request would overwrite the new request as this pending request will be executed now.

                        // Trigger next sub request.
                        onExplorerRequestExecuted( m_pExplorer, m_requestResult.m_iPendingRequestId, SErrResultInfo() );
                    }
                } // if( m_requestInProgress == ERequestUpdateAllValues )
            } // if( result == EResultSuccess )
        } // if( m_requestInProgress == ERequestUpdateSystemValues || m_subRequestInProgress == ERequestUpdateSystemValues )

        else if( m_requestInProgress == ERequestUpdateProcessorValues || m_subRequestInProgress == ERequestUpdateProcessorValues )
        {
            EResult result = i_errResultInfo.getResult();

            if( result != EResultSuccess )
            {
                QString strProposal;

                switch( result )
                {
                    case EResultSessionOpenError:
                    {
                        strProposal = "Check Host Settings (IP address, community string, etc.)";
                        m_bSystemValuesRead = false;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                if( m_pModelErrLog != NULL && !i_errResultInfo.isAddedToErrLogModel() )
                {
                    m_pModelErrLog->addEntry(
                        /* dateTime    */ QDateTime::currentDateTime(),
                        /* severity    */ i_errResultInfo.getSeverity(),
                        /* iErrNr      */ i_errResultInfo.getResult(),
                        /* strErrNr    */ i_errResultInfo.getResultStr(),
                        /* strSource   */ i_errResultInfo.getErrSource(),
                        /* strAddTrcInfo  */ i_errResultInfo.getAddErrInfoDscr(),
                        /* strProposal */ strProposal );
                }

                m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                m_requestInProgress = ERequestNone;
                m_subRequestInProgress = ERequestNone;
                emit requestExecuted(this,m_iIdRequestInProgress,i_errResultInfo);
                emit stateChanged(this,m_state,m_requestInProgress);

                m_pModelProcessors->clear();

                // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                // "requestInProgress" has been set to "None".and that receivers of the signals may call
                // the public request methods of the class as an reentry within the slots connected to
                // those signals. Those reentry requests need to be deferred and inserted into the list
                // of pending requests if currently requests are pending. Otherwise an already pending
                // request would overwrite the new request as this pending request will be executed now.

            } // if( result != EResultSuccess )

            else // if( result == EResultSuccess )
            {
                SProcessorValues* pProcessorValues;
                int               iRowCount;
                int               idxRow;
                CInstanceNode*    pInstanceNode;
                int               iInstanceNodeCount;
                int               idxInstance;
                CValue*           pValue;

                // Transfer values from explorer oid model into my processor model
                //----------------------------------------------------------------

                // Please note that we cannot store the reference to the node in our class as
                // the explorer may delete the whole model e.g. if the host setting are changed.
                COidNode* pOidNodeHrProcessorLoad = m_pExplorer->findOidNode(m_oids.m_oidHrProcessorLoad);

                if( pOidNodeHrProcessorLoad == NULL )
                {
                    m_pModelProcessors->clear();
                }
                else
                {
                    iInstanceNodeCount = pOidNodeHrProcessorLoad->getInstanceNodesCount();
                    iRowCount = m_pModelProcessors->rowCount();

                    if( iInstanceNodeCount != iRowCount )
                    {
                        m_pModelProcessors->clear();
                        iRowCount = 0;
                    }

                    for( idxInstance = 0, idxRow = 0; idxInstance < iInstanceNodeCount; idxInstance++ )
                    {
                        pInstanceNode = pOidNodeHrProcessorLoad->getInstanceNode(idxInstance);

                        if( pInstanceNode != NULL )
                        {
                            pValue = pInstanceNode->getValue();

                            if( pValue != NULL )
                            {
                                if( idxRow < iRowCount )
                                {
                                    pProcessorValues = m_pModelProcessors->getValues(idxRow);
                                }
                                else
                                {
                                    pProcessorValues = new SProcessorValues();
                                }
                                if( pProcessorValues != NULL )
                                {
                                    pProcessorValues->m_dateTimeUpdate = QDateTime::currentDateTime();
                                    pProcessorValues->m_iInstNr = pInstanceNode->getInstanceNumber();
                                    pProcessorValues->m_oidLoad = *pValue->getOid();
                                    pProcessorValues->m_iLoad_PerCent = pValue->getValueInt();
                                }
                                if( idxRow < iRowCount )
                                {
                                    m_pModelProcessors->setValues(idxRow,pProcessorValues);
                                    idxRow++;
                                }
                                else
                                {
                                    idxRow = m_pModelProcessors->addValues(pProcessorValues);
                                }
                            }
                        }
                    }
                }

                emit receivedProcessorValues(this);

                if( m_requestInProgress == ERequestUpdateProcessorValues )
                {
                    // The request has been executed
                    //------------------------------

                    m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                    m_requestInProgress = ERequestNone;
                    m_subRequestInProgress = ERequestNone;
                    emit requestExecuted(this,m_iIdRequestInProgress,i_errResultInfo);
                    emit stateChanged(this,m_state,m_requestInProgress);

                    // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                    // "requestInProgress" has been set to "None".and that receivers of the signals may call
                    // the public request methods of the class as an reentry within the slots connected to
                    // those signals. Those reentry requests need to be deferred and inserted into the list
                    // of pending requests if currently requests are pending. Otherwise an already pending
                    // request would overwrite the new request as this pending request will be executed now.

                } // if( m_requestInProgress == ERequestUpdateProcessorValues )

                else // if( m_requestInProgress == ERequestUpdateAllValues )
                {
                    // Query storage types
                    //----------------------

                    m_subRequestInProgress = ERequestUpdateStorageTypes;
                    emit stateChanged(this,m_state,m_requestInProgress);

                    m_requestResult = m_pExplorer->walk(m_oids.m_oidHrStorageType.toString());

                    if( !m_requestResult.isAsynchronousRequest() )
                    {
                        m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;

                        // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                        // "requestInProgress" has been set to "None".and that receivers of the signals may call
                        // the public request methods of the class as an reentry within the slots connected to
                        // those signals. Those reentry requests need to be deferred and inserted into the list
                        // of pending requests if currently requests are pending. Otherwise an already pending
                        // request would overwrite the new request as this pending request will be executed now.

                        // Trigger next sub request.
                        onExplorerRequestExecuted( m_pExplorer, m_requestResult.m_iPendingRequestId, SErrResultInfo() );
                    }
                } // if( m_requestInProgress == ERequestUpdateAllValues )
            } // if( result == EResultSuccess )
        } // if( m_requestInProgress == ERequestUpdateProcessorValues || m_subRequestInProgress == ERequestUpdateProcessorValues )

        else if( m_subRequestInProgress == ERequestUpdateStorageTypes )
        {
            EResult result = i_errResultInfo.getResult();

            if( result != EResultSuccess )
            {
                QString strProposal;

                switch( result )
                {
                    case EResultSessionOpenError:
                    {
                        strProposal = "Check Host Settings (IP address, community string, etc.)";
                        m_bSystemValuesRead = false;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                if( m_pModelErrLog != NULL && !i_errResultInfo.isAddedToErrLogModel() )
                {
                    m_pModelErrLog->addEntry(
                        /* dateTime    */ QDateTime::currentDateTime(),
                        /* severity    */ i_errResultInfo.getSeverity(),
                        /* iErrNr      */ i_errResultInfo.getResult(),
                        /* strErrNr    */ i_errResultInfo.getResultStr(),
                        /* strSource   */ i_errResultInfo.getErrSource(),
                        /* strAddTrcInfo  */ i_errResultInfo.getAddErrInfoDscr(),
                        /* strProposal */ strProposal );
                }

                m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                m_requestInProgress = ERequestNone;
                m_subRequestInProgress = ERequestNone;
                emit requestExecuted(this,m_iIdRequestInProgress,i_errResultInfo);
                emit stateChanged(this,m_state,m_requestInProgress);

                m_pModelStorages->clear();

                // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                // "requestInProgress" has been set to "None".and that receivers of the signals may call
                // the public request methods of the class as an reentry within the slots connected to
                // those signals. Those reentry requests need to be deferred and inserted into the list
                // of pending requests if currently requests are pending. Otherwise an already pending
                // request would overwrite the new request as this pending request will be executed now.
            }

            else // if( result == EResultSuccess )
            {
                TOidPtrList     oidPtrListReq;
                SStorageValues* pStorageValues;
                int             iRowCount;
                int             idxRow;
                CInstanceNode*  pInstanceNode;
                CValue*         pValue;
                int             iInstanceNodeCount;
                int             idxInstance;
                EStorageType    storageType;

                // Transfer values from explorer oid model into my storage model
                //--------------------------------------------------------------

                // Please note that we cannot store the reference to the node in our class as
                // the explorer may delete the whole model e.g. if the host setting are changed.
                COidNode* pOidNodeHrStorageType = m_pExplorer->findOidNode(m_oids.m_oidHrStorageType);

                if( pOidNodeHrStorageType == NULL )
                {
                    m_pModelStorages->clear();
                }
                else
                {
                    iInstanceNodeCount = pOidNodeHrStorageType->getInstanceNodesCount();

                    // Detect number of storage types we are interested in ..
                    for( idxInstance = 0, iRowCount = 0; idxInstance < iInstanceNodeCount; idxInstance++ )
                    {
                        pInstanceNode = pOidNodeHrStorageType->getInstanceNode(idxInstance);

                        if( pInstanceNode != NULL )
                        {
                            pValue = pInstanceNode->getValue();

                            if( pValue != NULL )
                            {
                                storageType = oidStr2StorageType(pValue->getValueString());

                                switch( storageType )
                                {
                                    case EStorageTypeRam:
                                    case EStorageTypeVirtualMemory:
                                    case EStorageTypeFixedDisk:
                                    {
                                        iRowCount++;
                                        break;
                                    }
                                    case EStorageTypeOther:
                                    case EStorageTypeRemovableDisk:
                                    case EStorageTypeFloppyDisk:
                                    case EStorageTypeCompactDisk:
                                    case EStorageTypeRamDisk:
                                    case EStorageTypeFlashMemory:
                                    case EStorageTypeNetworkDisk:
                                    default:
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    // If the number of storages we are interested in has been changed ..
                    if( iRowCount != m_pModelStorages->rowCount() )
                    {
                        m_pModelStorages->clear();
                        iRowCount = 0;
                    }

                    // Store the instance OID's of the type of storages we are interested in in the model ..
                    for( idxInstance = 0, idxRow = 0; idxInstance < iInstanceNodeCount; idxInstance++ )
                    {
                        pInstanceNode = pOidNodeHrStorageType->getInstanceNode(idxInstance);

                        if( pInstanceNode != NULL )
                        {
                            pValue = pInstanceNode->getValue();

                            if( pValue != NULL )
                            {
                                storageType = oidStr2StorageType(pValue->getValueString());

                                switch( storageType )
                                {
                                    case EStorageTypeRam:
                                    case EStorageTypeVirtualMemory:
                                    case EStorageTypeFixedDisk:
                                    {
                                        if( idxRow < iRowCount )
                                        {
                                            pStorageValues = m_pModelStorages->getValues(idxRow);
                                        }
                                        else
                                        {
                                            pStorageValues = new SStorageValues();
                                        }
                                        if( pStorageValues != NULL )
                                        {
                                            pStorageValues->m_dateTimeUpdate = QDateTime::currentDateTime();
                                            pStorageValues->m_iInstNr = pInstanceNode->getInstanceNumber();
                                            pStorageValues->m_oidType = *pValue->getOid();
                                            pStorageValues->m_strTypeValue = pValue->getValueString();
                                            pStorageValues->m_storageType = oidStr2StorageType(pValue->getValueString());
                                        }
                                        if( idxRow < iRowCount )
                                        {
                                            m_pModelStorages->setValues(idxRow,pStorageValues);
                                            idxRow++;
                                        }
                                        else
                                        {
                                            idxRow = m_pModelStorages->addValues(pStorageValues);
                                        }
                                        break;
                                    }
                                    case EStorageTypeOther:
                                    case EStorageTypeRemovableDisk:
                                    case EStorageTypeFloppyDisk:
                                    case EStorageTypeCompactDisk:
                                    case EStorageTypeRamDisk:
                                    case EStorageTypeFlashMemory:
                                    case EStorageTypeNetworkDisk:
                                    default:
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                // Query storage values
                //----------------------

                // Only the type of storages we are interested in are part of the model ...
                iRowCount = m_pModelStorages->rowCount();
                for( idxRow = 0; idxRow < iRowCount; idxRow++ )
                {
                    pStorageValues = m_pModelStorages->getValues(idxRow);
                    if( pStorageValues != NULL )
                    {
                        // The storage type will be requested again to ensure that the type has
                        // not been changed in the meantime on removing or plug in e.g. an USB stick.
                        oidPtrListReq.append( new COid( m_oids.m_oidHrStorageType.toString() + "." + QString::number(pStorageValues->m_iInstNr) ) );
                        oidPtrListReq.append( new COid( m_oids.m_oidHrStorageDescr.toString() + "." + QString::number(pStorageValues->m_iInstNr) ) );
                        oidPtrListReq.append( new COid( m_oids.m_oidHrStorageAllocationUnits.toString() + "." + QString::number(pStorageValues->m_iInstNr) ) );
                        oidPtrListReq.append( new COid( m_oids.m_oidHrStorageSize.toString() + "." + QString::number(pStorageValues->m_iInstNr) ) );
                        oidPtrListReq.append( new COid( m_oids.m_oidHrStorageUsed.toString() + "." + QString::number(pStorageValues->m_iInstNr) ) );
                    }
                }

                if( oidPtrListReq.count() > 0 )
                {
                    m_subRequestInProgress = ERequestUpdateStorageValues;
                    emit stateChanged(this,m_state,m_requestInProgress);

                    m_requestResult = m_pExplorer->requestValues(oidPtrListReq);

                    if( !m_requestResult.isAsynchronousRequest() )
                    {
                        m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;

                        // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                        // "requestInProgress" has been set to "None".and that receivers of the signals may call
                        // the public request methods of the class as an reentry within the slots connected to
                        // those signals. Those reentry requests need to be deferred and inserted into the list
                        // of pending requests if currently requests are pending. Otherwise an already pending
                        // request would overwrite the new request as this pending request will be executed now.

                        // Trigger next sub request.
                        onExplorerRequestExecuted( m_pExplorer, m_requestResult.m_iPendingRequestId, SErrResultInfo() );
                    }
                } // if( oidPtrListReq.count() > 0 )

                else // if( oidPtrListReq.count() == 0 )
                {
                    m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                    m_requestInProgress = ERequestNone;
                    m_subRequestInProgress = ERequestNone;
                    emit requestExecuted(this,m_iIdRequestInProgress,i_errResultInfo);
                    emit stateChanged(this,m_state,m_requestInProgress);

                    // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                    // "requestInProgress" has been set to "None".and that receivers of the signals may call
                    // the public request methods of the class as an reentry within the slots connected to
                    // those signals. Those reentry requests need to be deferred and inserted into the list
                    // of pending requests if currently requests are pending. Otherwise an already pending
                    // request would overwrite the new request as this pending request will be executed now.

                } // if( oidPtrListReq.count() == 0 )
            } // if( result == EResultSuccess )
        } // if( m_subRequestInProgress == ERequestUpdateStorageTypes )

        else if( m_requestInProgress == ERequestUpdateStorageValues || m_subRequestInProgress == ERequestUpdateStorageValues )
        {
            EResult result = i_errResultInfo.getResult();

            if( result != EResultSuccess )
            {
                QString strProposal;

                switch( result )
                {
                    case EResultSessionOpenError:
                    {
                        strProposal = "Check Host Settings (IP address, community string, etc.)";
                        m_bSystemValuesRead = false;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                if( m_pModelErrLog != NULL && !i_errResultInfo.isAddedToErrLogModel() )
                {
                    m_pModelErrLog->addEntry(
                        /* dateTime    */ QDateTime::currentDateTime(),
                        /* severity    */ i_errResultInfo.getSeverity(),
                        /* iErrNr      */ i_errResultInfo.getResult(),
                        /* strErrNr    */ i_errResultInfo.getResultStr(),
                        /* strSource   */ i_errResultInfo.getErrSource(),
                        /* strAddTrcInfo  */ i_errResultInfo.getAddErrInfoDscr(),
                        /* strProposal */ strProposal );
                }

                m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                m_requestInProgress = ERequestNone;
                m_subRequestInProgress = ERequestNone;
                emit requestExecuted(this,m_iIdRequestInProgress,i_errResultInfo);
                emit stateChanged(this,m_state,m_requestInProgress);

                m_pModelStorages->clear();

                // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                // "requestInProgress" has been set to "None".and that receivers of the signals may call
                // the public request methods of the class as an reentry within the slots connected to
                // those signals. Those reentry requests need to be deferred and inserted into the list
                // of pending requests if currently requests are pending. Otherwise an already pending
                // request would overwrite the new request as this pending request will be executed now.
            }

            else // if( result == EResultSuccess )
            {
                SStorageValues* pStorageValues;
                int             iRowCount;
                int             idxRow;
                CInstanceNode*  pInstanceNode;
                CValue*         pValue;
                int             iInstanceNodeCount;
                int             idxInstance;
                EStorageType    storageType;

                // Transfer values from explorer oid model into processor load model
                //------------------------------------------------------------------

                // Please note that we cannot store the reference to the node in our class as
                // the explorer may delete the whole model e.g. if the host setting are changed.
                COidNode* pOidNodeHrStorageType  = m_pExplorer->findOidNode(m_oids.m_oidHrStorageType);
                COidNode* pOidNodeHrStorageDescr = m_pExplorer->findOidNode(m_oids.m_oidHrStorageDescr);
                COidNode* pOidNodeHrStorageUnits = m_pExplorer->findOidNode(m_oids.m_oidHrStorageAllocationUnits);
                COidNode* pOidNodeHrStorageSize  = m_pExplorer->findOidNode(m_oids.m_oidHrStorageSize);
                COidNode* pOidNodeHrStorageUsed  = m_pExplorer->findOidNode(m_oids.m_oidHrStorageUsed);

                if( pOidNodeHrStorageType == NULL
                 || pOidNodeHrStorageDescr == NULL
                 || pOidNodeHrStorageUnits == NULL
                 || pOidNodeHrStorageSize == NULL
                 || pOidNodeHrStorageUsed == NULL )
                {
                    m_pModelStorages->clear();
                }
                else
                {
                    iInstanceNodeCount = pOidNodeHrStorageType->getInstanceNodesCount();

                    // Again detect number of storage types we are interested in ..
                    for( idxInstance = 0, iRowCount = 0; idxInstance < iInstanceNodeCount; idxInstance++ )
                    {
                        pInstanceNode = pOidNodeHrStorageType->getInstanceNode(idxInstance);

                        if( pInstanceNode != NULL )
                        {
                            pValue = pInstanceNode->getValue();

                            if( pValue != NULL )
                            {
                                storageType = oidStr2StorageType(pValue->getValueString());

                                switch( storageType )
                                {
                                    case EStorageTypeRam:
                                    case EStorageTypeVirtualMemory:
                                    case EStorageTypeFixedDisk:
                                    {
                                        iRowCount++;
                                        break;
                                    }
                                    case EStorageTypeOther:
                                    case EStorageTypeRemovableDisk:
                                    case EStorageTypeFloppyDisk:
                                    case EStorageTypeCompactDisk:
                                    case EStorageTypeRamDisk:
                                    case EStorageTypeFlashMemory:
                                    case EStorageTypeNetworkDisk:
                                    default:
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    // If the number of storages we are interested in has been changed since
                    // the last time the storage types have been queried ...
                    if( iRowCount != m_pModelStorages->rowCount() )
                    {
                        m_pModelStorages->clear();
                        iRowCount = 0;
                    }
                    // If the number of storages we are interested in is still the same ...
                    else
                    {
                        for( idxRow = 0; idxRow < iRowCount; idxRow++ )
                        {
                            pStorageValues = m_pModelStorages->getValues(idxRow);

                            if( pStorageValues != NULL )
                            {
                                pStorageValues->m_dateTimeUpdate = QDateTime::currentDateTime();

                                // Copy storage descriptions from explorer model into storages model ..
                                iInstanceNodeCount = pOidNodeHrStorageDescr->getInstanceNodesCount();
                                for( idxInstance = 0; idxInstance < iInstanceNodeCount; idxInstance++ )
                                {
                                    pInstanceNode  = pOidNodeHrStorageDescr->getInstanceNode(idxInstance);
                                    if( pInstanceNode != NULL && static_cast<int>(pInstanceNode->getInstanceNumber()) == pStorageValues->m_iInstNr )
                                    {
                                        pValue = pInstanceNode->getValue();
                                        if( pValue != NULL )
                                        {
                                            pStorageValues->m_oidDescr = *pValue->getOid();
                                            pStorageValues->m_strDescrValue = pValue->getValueString();
                                        }
                                    }
                                }

                                // Copy storage allocation units from explorer model into storages model ..
                                iInstanceNodeCount = pOidNodeHrStorageUnits->getInstanceNodesCount();
                                for( idxInstance = 0; idxInstance < iInstanceNodeCount; idxInstance++ )
                                {
                                    pInstanceNode  = pOidNodeHrStorageUnits->getInstanceNode(idxInstance);
                                    if( pInstanceNode != NULL && static_cast<int>(pInstanceNode->getInstanceNumber()) == pStorageValues->m_iInstNr )
                                    {
                                        pValue = pInstanceNode->getValue();
                                        if( pValue != NULL )
                                        {
                                            pStorageValues->m_oidAllocationUnits = *pValue->getOid();
                                            pStorageValues->m_iAllocationUnits_byte = pValue->getValueInt();
                                        }
                                    }
                                }

                                // Copy storage size from explorer model into storages model ..
                                iInstanceNodeCount = pOidNodeHrStorageSize->getInstanceNodesCount();
                                for( idxInstance = 0; idxInstance < iInstanceNodeCount; idxInstance++ )
                                {
                                    pInstanceNode  = pOidNodeHrStorageSize->getInstanceNode(idxInstance);
                                    if( pInstanceNode != NULL && static_cast<int>(pInstanceNode->getInstanceNumber()) == pStorageValues->m_iInstNr )
                                    {
                                        pValue = pInstanceNode->getValue();
                                        if( pValue != NULL )
                                        {
                                            pStorageValues->m_oidSize = *pValue->getOid();
                                            pStorageValues->m_iSize_AllocationUnits = pValue->getValueInt();
                                        }
                                    }
                                }

                                // Copy storage used from explorer model into storages model ..
                                iInstanceNodeCount = pOidNodeHrStorageUsed->getInstanceNodesCount();
                                for( idxInstance = 0; idxInstance < iInstanceNodeCount; idxInstance++ )
                                {
                                    pInstanceNode  = pOidNodeHrStorageUsed->getInstanceNode(idxInstance);
                                    if( pInstanceNode != NULL && static_cast<int>(pInstanceNode->getInstanceNumber()) == pStorageValues->m_iInstNr )
                                    {
                                        pValue = pInstanceNode->getValue();
                                        if( pValue != NULL )
                                        {
                                            pStorageValues->m_oidUsed = *pValue->getOid();
                                            pStorageValues->m_iUsed_AllocationUnits = pValue->getValueInt();
                                        }
                                    }
                                }

                                // Update model data (emit dataChanged signal for table view)
                                m_pModelStorages->setValues(idxInstance,pStorageValues);

                            } // if( pStorageValues != NULL )
                        } // for( idxStorage < iStorageValuesCount )
                    } // if( iStorageValuesCount == m_pModelStorages->rowCount() )
                } // if( m_oidNodes.m_pHrStorageType == NULL ...

                m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                m_requestInProgress = ERequestNone;
                m_subRequestInProgress = ERequestNone;
                emit requestExecuted(this,m_iIdRequestInProgress,i_errResultInfo);
                emit stateChanged(this,m_state,m_requestInProgress);

                emit receivedStorageValues(this);

                // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                // "requestInProgress" has been set to "None".and that receivers of the signals may call
                // the public request methods of the class as an reentry within the slots connected to
                // those signals. Those reentry requests need to be deferred and inserted into the list
                // of pending requests if currently requests are pending. Otherwise an already pending
                // request would overwrite the new request as this pending request will be executed now.

            } // if( result == EResultSuccess )
        } // if( m_requestInProgress == ERequestUpdateStorageValues || m_subRequestInProgress == ERequestUpdateStorageValues )

        // If the request has been executed check if there are pending requests ...
        if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() > 0 )
        {
            CMsgReq* pMsgReqPending = m_ptrListMsgReqPending.takeFirst();

            if( pMsgReqPending != NULL )
            {
                m_requestInProgress = pendingMsgType2Request(pMsgReqPending);

                switch( m_requestInProgress )
                {
                    case ERequestUpdateAllValues:
                    {
                        m_subRequestInProgress = ERequestUpdateSystemValues;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                emit stateChanged(this,m_state,m_requestInProgress);
                QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                pMsgReqPending = NULL;
            }
        } // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() > 0 )

    } // if( m_requestRestult.m_iPendingRequestId == i_iRequestId )

} // onExplorerRequestExecuted

//------------------------------------------------------------------------------
CCltStdPC::ERequest CCltStdPC::pendingMsgType2Request( ZS::CMsgReq* i_pMsgReq )
//------------------------------------------------------------------------------
{
    if( i_pMsgReq == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"i_pMsgReq == NULL");
    }

    ERequest request = ERequestUndefined;

    switch( i_pMsgReq->getMsgType() )
    {
        case EMsgTypeCltStdPCReqStartSession:
        {
            request = ERequestStartSession;
            break;
        }
        case EMsgTypeReqCloseSession:
        {
            request = ERequestCloseSession;
            break;
        }
        case EMsgTypeReqWalk:
        {
            CMsgReqWalk* pMsgReq = dynamic_cast<CMsgReqWalk*>(i_pMsgReq);

            if( pMsgReq != NULL )
            {
                COid* pOid = pMsgReq->getOid(false);

                if( *pOid == m_oids.m_oidSystem )
                {
                    request = ERequestUpdateSystemValues;
                }
                else if( *pOid == m_oids.m_oidHrProcessorLoad )
                {
                    request = ERequestUpdateProcessorValues;
                }
                else if( *pOid == m_oids.m_oidHrStorageType )
                {
                    request = ERequestUpdateStorageValues;
                }
                else // no oid set
                {
            request = ERequestUpdateAllValues;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
    if( request == ERequestUndefined )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessage,i_pMsgReq->msgTypeToStr());
    }
    return request;

} // pendingMsgType2Request

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CCltStdPC::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( m_requestInProgress != ERequestNone )
    {
        ZS::CMsg* pMsg = dynamic_cast<ZS::CMsg*>(i_pEv);

        if( pMsg != NULL )
        {
            QString strAddTrcInfo;

            if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
            {
                strAddTrcInfo  = "State:" + state2Str(m_state);
                strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
                strAddTrcInfo += ", SubRequestInProgress:" + request2Str(m_subRequestInProgress);
                strAddTrcInfo += ", ExplorerState:" + CExplorer::state2Str(m_pExplorer->getCurrentState());
                strAddTrcInfo += ", ExplorerRequestInProgress:" + CExplorer::request2Str(m_pExplorer->getRequestInProgress());
                strAddTrcInfo += ", Event:" + pMsg->msgTypeToStr();
            }

            M_TRACE_INST_METHOD(
                /* pAdminObj     */ m_pTrcAdminObj,
                /* iDetailLevel  */ 0,
                /* szMethod      */ "event",
                /* strAddTrcInfo */ strAddTrcInfo );

            switch( pMsg->type() )
            {
                //------------------------------------------------------------------
                case EMsgTypeCltStdPCReqStartSession:
                //------------------------------------------------------------------
                {
                    CMsgReqCltStdPCStartSession* pMsgReq = dynamic_cast<CMsgReqCltStdPCStartSession*>(pMsg);
                    if( pMsgReq == NULL )
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                    }

                    if( m_requestInProgress != ERequestStartSession )
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
                    }

                    SOidsStdPC oidsMsgReq = pMsgReq->getOids(); // because of linux compilation

                    if( m_oids != oidsMsgReq )
                    {
                        m_oids = oidsMsgReq;

                        saveOids();

                        if( m_pModelSystem->rowCount() > 0 )
                        {
                            m_pModelSystem->clear();
                        }
                        if( m_pModelProcessors->rowCount() > 0 )
                        {
                            m_pModelProcessors->clear();
                        }
                        if( m_pModelStorages->rowCount() > 0 )
                        {
                            m_pModelStorages->clear();
                        }

                        // As there is only one instance for both the system description and the system name node OID's
                        // those OID's may be immediately evaluated after the node OID's have been read.
                        SSystemValues* pSystemValues = new SSystemValues();
                        pSystemValues->m_oidSysDescr = m_oids.m_oidSysDescr.toString() + ".0";
                        pSystemValues->m_oidSysName = m_oids.m_oidSysName.toString() + ".0";
                        m_pModelSystem->addValues(pSystemValues);
                        pSystemValues = NULL;
                        m_bSystemValuesRead = false;

                        emit oidsChanged(this);

                    } // if( m_oids != oidsMsgReq )

                    m_requestResult = m_pExplorer->openSession();

                    if( m_requestResult.isAsynchronousRequest() )
                    {
                        m_iIdRequestInProgress = pMsgReq->getMsgId();
                        m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                        emit stateChanged(this,m_state,m_requestInProgress);

                        if( m_ariRequestTimeouts_ms[m_requestInProgress] > 0 )
                        {
                            m_pTimerExplorer->start(m_ariRequestTimeouts_ms[m_requestInProgress]);
                        }
                    }
                    else
                    {
                        m_requestInProgress = ERequestNone;
                        m_subRequestInProgress = ERequestNone;
                        emit requestExecuted(this,m_iIdRequestInProgress,SErrResultInfo());
                        emit stateChanged(this,m_state,m_requestInProgress);

                        // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                        // "requestInProgress" has been set to "None".and that receivers of the signals may call
                        // the public request methods of the class as an reentry within the slots connected to
                        // those signals. Those reentry requests need to be deferred and inserted into the list
                        // of pending requests if currently requests are pending. Otherwise an already pending
                        // request would overwrite the new request as this pending request will be executed now.
                    }
                    break;
                } // case EMsgTypeCltStdPCReqStartSession

                //------------------------------------------------------------------
                case EMsgTypeReqCloseSession:
                //------------------------------------------------------------------
                {
                    CMsgReqCloseSession* pMsgReq = dynamic_cast<CMsgReqCloseSession*>(pMsg);
                    if( pMsgReq == NULL )
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                    }

                    if( m_requestInProgress != ERequestCloseSession )
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
                    }

                    m_requestResult = m_pExplorer->closeSession();

                    if( m_requestResult.isAsynchronousRequest() )
                    {
                        m_iIdRequestInProgress = pMsgReq->getMsgId();
                        m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                        emit stateChanged(this,m_state,m_requestInProgress);

                        if( m_ariRequestTimeouts_ms[m_requestInProgress] > 0 )
                        {
                            m_pTimerExplorer->start(m_ariRequestTimeouts_ms[m_requestInProgress]);
                        }
                    }
                    else
                    {
                        m_requestInProgress = ERequestNone;
                        m_subRequestInProgress = ERequestNone;
                        emit stateChanged(this,m_state,m_requestInProgress);

                        // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                        // "requestInProgress" has been set to "None".and that receivers of the signals may call
                        // the public request methods of the class as an reentry within the slots connected to
                        // those signals. Those reentry requests need to be deferred and inserted into the list
                        // of pending requests if currently requests are pending. Otherwise an already pending
                        // request would overwrite the new request as this pending request will be executed now.
                    }
                    break;
                } // case EMsgTypeReqCloseSession

                //------------------------------------------------------------------
                case EMsgTypeReqWalk:
                //------------------------------------------------------------------
                {
                    CMsgReqWalk* pMsgReq = dynamic_cast<CMsgReqWalk*>(pMsg);
                    if( pMsgReq == NULL )
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                    }

                    if( m_requestInProgress != ERequestUpdateAllValues
                     && m_requestInProgress != ERequestUpdateSystemValues
                     && m_requestInProgress != ERequestUpdateProcessorValues
                     && m_requestInProgress != ERequestUpdateStorageTypes
                     && m_requestInProgress != ERequestUpdateStorageValues )
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
                    }

                    m_requestResult.m_result = EResultSuccess;

                    // If the explorers session is still open ..
                    if( m_pExplorer->getCurrentState() != CExplorer::EStateSessionClosed
                     && m_pExplorer->getCurrentState() != CExplorer::EStateWaitingForSessionClosed
                     && m_pExplorer->getCurrentState() != CExplorer::EStateWaitingForSessionDeleted )
                    {
                        COid* pOid = pMsgReq->getOid(false);

                        if( pOid != NULL )
                        {
                            if( *pOid == m_oids.m_oidSystem )
                            {
                                SSystemValues* pSystemValues = m_pModelSystem->getValues(0);

                                if( pSystemValues == NULL )
                                {
                                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalProgramError,"System OID's not in model");
                                }

                                TOidPtrList oidPtrListReq;

                                oidPtrListReq.append( new COid(pSystemValues->m_oidSysDescr) );
                                oidPtrListReq.append( new COid(pSystemValues->m_oidSysName) );

                                m_requestResult = m_pExplorer->requestValues(oidPtrListReq);

                                if( m_requestResult.isAsynchronousRequest() )
                                {
                                    m_iIdRequestInProgress = pMsgReq->getMsgId();
                                    m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                                    emit stateChanged(this,m_state,m_requestInProgress);
                                }
                            }
                            else if( *pOid == m_oids.m_oidHrProcessorLoad )
                            {
                                m_requestResult = m_pExplorer->walk(m_oids.m_oidHrProcessorLoad.toString());

                                if( m_requestResult.isAsynchronousRequest() )
                                {
                                    m_iIdRequestInProgress = pMsgReq->getMsgId();
                                    m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                                    emit stateChanged(this,m_state,m_requestInProgress);
                                }
                            }
                            else if( *pOid == m_oids.m_oidHrStorageType )
                            {
                                m_requestResult = m_pExplorer->walk(m_oids.m_oidHrStorageType.toString());

                                if( m_requestResult.isAsynchronousRequest() )
                                {
                                    m_iIdRequestInProgress = pMsgReq->getMsgId();
                                    m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                                    emit stateChanged(this,m_state,m_requestInProgress);
                                }
                            }
                            else // no oid set
                            {
                                if( !m_bSystemValuesRead )
                                {
                                    SSystemValues* pSystemValues = m_pModelSystem->getValues(0);

                                    if( pSystemValues == NULL )
                                    {
                                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalProgramError,"System OID's not in model");
                                    }

                                    TOidPtrList oidPtrListReq;

                                    oidPtrListReq.append( new COid(pSystemValues->m_oidSysDescr) );
                                    oidPtrListReq.append( new COid(pSystemValues->m_oidSysName) );

                                    m_requestResult = m_pExplorer->requestValues(oidPtrListReq);

                                    if( m_requestResult.isAsynchronousRequest() )
                                    {
                                        m_iIdRequestInProgress = pMsgReq->getMsgId();
                                        m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                                        emit stateChanged(this,m_state,m_requestInProgress);

                                        if( m_ariRequestTimeouts_ms[m_requestInProgress] > 0 )
                                        {
                                            m_pTimerExplorer->start(m_ariRequestTimeouts_ms[m_requestInProgress]);
                                        }
                                    }
                                }
                                else // if( m_bSystemValuesRead )
                                {
                                    m_requestResult = m_pExplorer->walk(m_oids.m_oidHrProcessorLoad.toString());

                                    if( m_requestResult.isAsynchronousRequest() )
                                    {
                                        m_iIdRequestInProgress = pMsgReq->getMsgId();
                                        m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                                        emit stateChanged(this,m_state,m_requestInProgress);
                                    }
                                } // if( m_bSystemValuesRead )
                            } // if no oid set
                        } // if( pOid != NULL )
                    } // if( m_pExplorer->getCurrentState() != CExplorer::EStateSessionClosed ...

                    if( !m_requestResult.isAsynchronousRequest() )
                    {
                        SErrResultInfo errResultInfo;
                        QString        strAddTrcInfo;
                        COid*          pOid = pMsgReq->getOid(false);

                        if( pOid != NULL )
                        {
                            strAddTrcInfo = pMsgReq->msgTypeToStr() + " " + pOid->toString(EOidFormatFull);
                        }
                        else
                        {
                            strAddTrcInfo = pMsgReq->msgTypeToStr();
                        }
                        errResultInfo.setSeverity(ZS::EResultSeverityError);
                        errResultInfo.setResult(ZS::EResultSessionNotOpen);
                        errResultInfo.setResultStr(ZS::result2Str(ZS::EResultSessionNotOpen));
                        errResultInfo.setErrSource(m_strObjName);
                        errResultInfo.setAddErrInfoDscr(strAddTrcInfo);

                        m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                        m_requestInProgress = ERequestNone;
                        m_subRequestInProgress = ERequestNone;
                        emit stateChanged(this,m_state,m_requestInProgress);
                        emit requestExecuted(this,pMsgReq->getMsgId(),errResultInfo);

                        // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                        // "requestInProgress" has been set to "None".and that receivers of the signals may call
                        // the public request methods of the class as an reentry within the slots connected to
                        // those signals. Those reentry requests need to be deferred and inserted into the list
                        // of pending requests if currently requests are pending. Otherwise an already pending
                        // request would overwrite the new request as this pending request will be executed now.

                    } // if( !m_requestResult.isAsynchronousRequest() )

                    break;
                } // case EMsgTypeReqWalk

                //------------------------------------------------------------------
                case EMsgTypeIndTimeout:
                //------------------------------------------------------------------
                {
                    CMsgIndTimeout* pMsgInd = dynamic_cast<CMsgIndTimeout*>(pMsg);
                    if( pMsgInd == NULL )
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                    }

                    SErrResultInfo errResultInfo;

                    errResultInfo.setSeverity(ZS::EResultSeverityError);
                    errResultInfo.setResult(ZS::EResultTimeout);
                    errResultInfo.setResultStr(ZS::result2Str(ZS::EResultTimeout));
                    errResultInfo.setErrSource(m_strObjName);
                    errResultInfo.setAddErrInfoDscr(strAddTrcInfo);

                    if( m_pModelErrLog != NULL )
                    {
                        m_pModelErrLog->addEntry(
                            /* dateTime    */ QDateTime::currentDateTime(),
                            /* severity    */ errResultInfo.getSeverity(),
                            /* iErrNr      */ errResultInfo.getResult(),
                            /* strErrNr    */ errResultInfo.getResultStr(),
                            /* strSource   */ errResultInfo.getErrSource(),
                            /* strAddTrcInfo  */ errResultInfo.getAddErrInfoDscr(),
                            /* strProposal */ "" );
                    }

                    m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                    m_requestInProgress = ERequestNone;
                    m_subRequestInProgress = ERequestNone;
                    emit requestExecuted(this,m_iIdRequestInProgress,errResultInfo);
                    emit stateChanged(this,m_state,m_requestInProgress);

                    // Please note that the signals "stateChanged" and "requestExecuted" are emitted after
                    // "requestInProgress" has been set to "None".and that receivers of the signals may call
                    // the public request methods of the class as an reentry within the slots connected to
                    // those signals. Those reentry requests need to be deferred and inserted into the list
                    // of pending requests if currently requests are pending. Otherwise an already pending
                    // request would overwrite the new request as this pending request will be executed now.
                    break;
                } // case EMsgTypeIndTimeout

                //------------------------------------------------------------------
                default:
                //------------------------------------------------------------------
                {
                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessage,strAddTrcInfo);
                }
            } // switch( pMsg->type() )

        } // if( pMsg != NULL )

        // If the request has been executed check if there are pending requests ...
        if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() > 0 )
        {
            CMsgReq* pMsgReqPending = m_ptrListMsgReqPending.takeFirst();

            if( pMsgReqPending != NULL )
            {
                m_requestInProgress = pendingMsgType2Request(pMsgReqPending);

                switch( m_requestInProgress )
                {
                    case ERequestUpdateAllValues:
                    {
                        m_subRequestInProgress = ERequestUpdateSystemValues;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                emit stateChanged(this,m_state,m_requestInProgress);
                QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                pMsgReqPending = NULL;
            }
        } // if( m_requestInProgress == ERequestNone && m_ptrListMsgReqPending.count() > 0 )

    } // if( m_requestInProgress != ERequestNone )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event
