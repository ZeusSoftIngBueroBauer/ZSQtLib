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

#include <QtCore/qtimer.h>
#include <QtNetwork/qhostinfo.h>

#include "ZSData/ZSDatabase.h"
#include "ZSData/ZSDatabaseGateway.h"
#include "ZSData/ZSDatabaseGatewayThread.h"
#include "ZSData/ZSDataMsg.h"
#include "ZSData/ZSDataSet.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Data;


/*******************************************************************************
class CDb : public QObject
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrStates[] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( CDb::EStateIdle,      "Idle"      ),
    /* 1 */ SEnumEntry( CDb::EStateListening, "Listening" ),
    /*   */ SEnumEntry( CDb::EStateCount,     "Count"     ),
    /*   */ SEnumEntry( CDb::EStateUndefined, "---"       )
};

//------------------------------------------------------------------------------
QString CDb::State2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrStates,_ZSArrLen(s_arEnumStrStates),i_iState);
}

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrRequests[] =
//------------------------------------------------------------------------------
{
    /*  0 */ SEnumEntry( CDb::ERequestNone,                  "None"                  ),
    /*  1 */ SEnumEntry( CDb::ERequestStartup,               "Startup"               ),
    /*  2 */ SEnumEntry( CDb::ERequestShutdown,              "Shutdown"              ),
    /*  3 */ SEnumEntry( CDb::ERequestUpdateHostSettings,    "UpdateHostSettings"    ),
    ///*  4 */ SEnumEntry( CDb::ERequestReadSchema,            "ReadSchema"            ),
    ///*  5 */ SEnumEntry( CDb::ERequestSaveSchema,            "SaveSchema"            ),
    ///*  6 */ SEnumEntry( CDb::ERequestReadData,              "ReadData"              ),
    ///*  7 */ SEnumEntry( CDb::ERequestSaveData,              "SaveData"              ),
    ///*  8 */ SEnumEntry( CDb::ERequestSet2Default,           "Set2Default"           ),
    ///*  9 */ SEnumEntry( CDb::ERequestUpdateDbClientsSchema, "UpdateDbClientsSchema" ),
    ///* 10 */ SEnumEntry( CDb::ERequestUpdateDbClientsData,   "UpdateDbClientsData"   ),
    //// Client Requests
    ///* 11 */ SEnumEntry( CDb::ERequestConnect,               "Connect"               ),
    ///* 12 */ SEnumEntry( CDb::ERequestDisconnect,            "Disconnect"            ),
    ///* 13 */ SEnumEntry( CDb::ERequestRegister,              "Register"              ),
    ///* 14 */ SEnumEntry( CDb::ERequestUnregister,            "Unregister"            ),
    ///* 15 */ SEnumEntry( CDb::ERequestSelectSchema,          "SelectSchema"          ),
    ///* 16 */ SEnumEntry( CDb::ERequestUpdateSchema,          "UpdateSchema"          ),
    ///* 17 */ SEnumEntry( CDb::ERequestSelectData,            "SelectData"            ),
    ///* 18 */ SEnumEntry( CDb::ERequestUpdateData,            "UpdateData"            ),
        // Internal requests
    /* xx */ SEnumEntry( CDb::ERequestStartGatewayThreads,    "StartGatewayThreads"    ),
    /* xx */ SEnumEntry( CDb::ERequestStopGatewayThreads,     "StopGatewayThreads"     ),
    /* xx */ SEnumEntry( CDb::ERequestStartGatewayThread,     "StartGatewayThread"     ),
    /* xx */ SEnumEntry( CDb::ERequestStopGatewayThread,      "StopGatewayThread"      ),
    /* xx */ SEnumEntry( CDb::ERequestStartupGateways,        "StartupGateways"        ),
    /* xx */ SEnumEntry( CDb::ERequestShutdownGateways,       "ShutdownGateways"       ),
    /* xx */ SEnumEntry( CDb::ERequestStartupGateway,         "StartupGateway"         ),
    /* xx */ SEnumEntry( CDb::ERequestShutdownGateway,        "ShutdownGateway"        ),
    /*    */ SEnumEntry( CDb::ERequestCount,                  "Count"                  ),
    /*    */ SEnumEntry( CDb::ERequestUndefined,              "---"                    )
};

//------------------------------------------------------------------------------
QString CDb::Request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrRequests,_ZSArrLen(s_arEnumStrRequests),i_iRequest);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDb::CDb( const QString&  i_strObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMtx(nullptr),
    m_fctCreateDataSet(nullptr),
    m_arFctAddDataSetObjects(),
    m_pDS(nullptr),
    // Supported protocol and connection types
    m_arProtocolTypes(),
    m_hsharSocketTypes(),
    m_hshHostSettingsDefault(),
    m_hshHostSettingsOrig(),
    m_hshHostSettingsCurr(),
    // Connected and registered clients
    m_hshpGatewayThreads(),
    m_hshpGateways(),
    m_hshDbCltsConnected(),
    m_hshDbCltsRegSpecs(),
    //m_hsharpDbCltsChgDS(),
    // State machine
    m_state(EStateIdle),
    m_pRequestQueue(nullptr),
    m_reqDscrTimeout(),
    m_pTmrReqTimeout(nullptr),
    m_bMsgReqContinuePending(false),
    m_bIsBeingDestroyed(false),
    // Tracing
    m_arpTrcMsgLogObjects(),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), i_strObjName);

    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strAddTrcInfo );

    //if( i_bMultiThreadedAccess )
    //{
    //    m_pMtx = new QMutex(QMutex::Recursive);
    //}

    m_arProtocolTypes.append(EProtocolTypeZSXML);
    m_arProtocolTypes.append(EProtocolTypeZSMsg);

    m_hsharSocketTypes[EProtocolTypeZSXML].append(ESocketTypeTcp);
    m_hsharSocketTypes[EProtocolTypeZSMsg].append(ESocketTypeInProcMsg);

    SCnctType           cnctType;
    SServerHostSettings hostSettings;

    for( int idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
    {
        cnctType.m_protocolType = m_arProtocolTypes[idxProtType];

        for( int idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
        {
            cnctType.m_socketType = m_hsharSocketTypes[cnctType.m_protocolType][idxSockType];

            /*struct SServerHostSettings {
                ESocketType  m_socketType;
                QObject*     m_pObjLocal;
                QString      m_strLocalHostName; // So far only used for shm servers and shm clients. Tcp servers and tcp clients determine the local host name by themselves.
                QHostAddress m_hostAddrLocal;
                quint16      m_uLocalPort;      // Port the server is listening for incoming connection requests.
                unsigned int m_uMaxPendingConnections;
                unsigned int m_uBufferSize;     // Only used by shared memory socket clients
            } */

            hostSettings.m_socketType = cnctType.m_socketType;

            if( cnctType.m_socketType == ESocketTypeTcp )
            {
                hostSettings.m_pObjLocal = nullptr;
                hostSettings.m_strLocalHostName = QHostInfo::localHostName();
                //hostSettings.m_hostAddrLocal;
                hostSettings.m_uLocalPort = 19637;
                hostSettings.m_uMaxPendingConnections = 30;
                //hostSettings.m_uBufferSize = 0;
            }
            else if( cnctType.m_socketType == ESocketTypeInProcMsg )
            {
                hostSettings.m_pObjLocal = this;
                hostSettings.m_strLocalHostName = objectName();
                //hostSettings.m_hostAddrLocal;
                //hostSettings.m_uLocalPort = 19637;
                //hostSettings.m_uMaxPendingConnections = 0;
                //hostSettings.m_uBufferSize = 0;
            }

            m_hshHostSettingsOrig[cnctType] = hostSettings;

        } // for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
    } // for( int idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )

    // State Machine
    //--------------

    m_pRequestQueue = new CRequestQueue(
        /* strCreatorNameSpace */ nameSpace(),
        /* strCreatorClassName      */ className(),
        /* pObjParent               */ this,
        /* bKeepReqDscrInExecTree   */ false );

    m_pTmrReqTimeout = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrReqTimeout,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CDb::~CDb()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ strAddTrcInfo );

    m_bIsBeingDestroyed = true;

    abortAllRequests();

    //CChangeDataSet* pChgDS;

    //QHash<SCnctId,CChangeDataSet*>::iterator itChgDS;

    //for( itChgDS = m_hsharpDbCltsChgDS.begin(); itChgDS != m_hsharpDbCltsChgDS.end(); itChgDS++ )
    //{
    //    pChgDS = itChgDS.value();

    //    *itChgDS = nullptr;

    //    delete pChgDS;
    //    pChgDS = nullptr;

    //} // for( itChgDS = m_hsharpDbCltsChgDS.begin(); ...

    CDbGatewayThread* pGatewayThread;

    QHash<SCnctType,CDbGatewayThread*>::iterator itGatewayThreads;

    for( itGatewayThreads = m_hshpGatewayThreads.begin();
         itGatewayThreads != m_hshpGatewayThreads.end();
         itGatewayThreads++ )
    {
        pGatewayThread = itGatewayThreads.value();

        *itGatewayThreads = nullptr;

        if( pGatewayThread != nullptr )
        {
            if( pGatewayThread->isRunning() )
            {
                pGatewayThread->quit();
                pGatewayThread->wait(30000);
            }
            try
            {
                delete pGatewayThread;
            }
            catch(...)
            {
            }
        }
        pGatewayThread = nullptr;
    }

    //if( m_bSchemaSaveReadEnabled && m_pDS != nullptr && !m_strSchemaFileName.isEmpty() )
    //{
    //    m_pDS->saveSchema();
    //}

    //if( m_bDataSaveReadEnabled && m_pDS != nullptr && !m_strDataFileName.isEmpty() )
    //{
    //    m_pDS->saveData();
    //}

    try
    {
        delete m_pDS;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pMtx;
    }
    catch(...)
    {
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pMtx = nullptr;
    m_fctCreateDataSet = nullptr;
    //m_arFctAddDataSetObjects.clear();
    m_pDS = nullptr;
    // Supported protocol and connection types
    m_arProtocolTypes.clear();         // Only protocol type ZS is supported yet (implementing a SQL parser is not that simple).
    m_hsharSocketTypes.clear();        // As default only protocol type ZS and connection type InProcMsg is supported.
    m_hshHostSettingsDefault.clear();
    m_hshHostSettingsOrig.clear();
    m_hshHostSettingsCurr.clear();
    // Connected and registered clients
    m_hshpGatewayThreads.clear();
    m_hshpGateways.clear();
    m_hshDbCltsConnected.clear();
    m_hshDbCltsRegSpecs.clear();
    //m_hsharpDbCltsChgDS.clear();
    // State machine
    m_state = static_cast<EState>(0);
    m_pRequestQueue = nullptr;
    //m_reqDscrTimeout;
    m_pTmrReqTimeout = nullptr;
    m_bMsgReqContinuePending = false;
    m_bIsBeingDestroyed = false;
    // Tracing
    //m_arpTrcMsgLogObjects.clear();
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::setKeepReqDscrInExecTree( bool i_bKeep )
//------------------------------------------------------------------------------
{
    m_pRequestQueue->setKeepReqDscrInExecTree(i_bKeep);
}

//------------------------------------------------------------------------------
bool CDb::keepReqDscrInExecTree() const
//------------------------------------------------------------------------------
{
    return m_pRequestQueue->keepReqDscrInExecTree();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CDb::getProtocolTypesCount( ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int iCount = 0;

    if( i_socketType == ESocketTypeCount || i_socketType == ESocketTypeUndefined )
    {
        iCount = m_arProtocolTypes.size();
    }
    else
    {
        QList<EProtocolType> arProtocolTypes = getProtocolTypes(i_socketType);

        iCount = arProtocolTypes.size();
    }

    return iCount;

} // getProtocolTypesCount

//------------------------------------------------------------------------------
QList<EProtocolType> CDb::getProtocolTypes( ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QList<EProtocolType> arProtocolTypes;

    if( i_socketType == ESocketTypeCount || i_socketType == ESocketTypeUndefined )
    {
        arProtocolTypes = m_arProtocolTypes;
    }
    else
    {
        QHash<EProtocolType,QList<Ipc::ESocketType>>::const_iterator it;

        for( it = m_hsharSocketTypes.begin(); it != m_hsharSocketTypes.end(); it++ )
        {
            if( it.value().contains(i_socketType) )
            {
                arProtocolTypes.append(it.key());
            }
        }
    }

    return arProtocolTypes;

} // getProtocolTypes

//------------------------------------------------------------------------------
bool CDb::isProtocolTypeSupported( EProtocolType i_protocolType, ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bIsSupported = false;

    if( i_socketType == ESocketTypeCount || i_socketType == ESocketTypeUndefined )
    {
        bIsSupported = m_arProtocolTypes.contains(i_protocolType);
    }
    else
    {
        QList<EProtocolType> arProtocolTypes = getProtocolTypes(i_socketType);

        bIsSupported = arProtocolTypes.contains(i_protocolType);
    }

    return bIsSupported;

} // isProtocolTypeSupported

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CDb::getSocketTypesCount( EProtocolType i_protocolType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int iSize = 0;

    if( i_protocolType == EProtocolTypeCount || i_protocolType == EProtocolTypeUndefined )
    {
        QHash<EProtocolType,QList<ZS::Ipc::ESocketType>>::const_iterator itSockTypes;

        QList<ZS::Ipc::ESocketType> arSockTypes;

        for( itSockTypes = m_hsharSocketTypes.begin(); itSockTypes != m_hsharSocketTypes.end(); itSockTypes++ )
        {
            arSockTypes = itSockTypes.value();
            iSize += arSockTypes.size();
        }
    }
    else if( m_hsharSocketTypes.contains(i_protocolType) )
    {
        iSize = m_hsharSocketTypes[i_protocolType].size();
    }
    return iSize;

} // getSocketTypesCount

//------------------------------------------------------------------------------
QList<ESocketType> CDb::getSocketTypes( EProtocolType i_protocolType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QList<ESocketType> arSocketTypes;

    if( i_protocolType == EProtocolTypeCount || i_protocolType == EProtocolTypeUndefined )
    {
        QHash<EProtocolType,QList<ZS::Ipc::ESocketType>>::const_iterator itSockTypes;

        for( itSockTypes = m_hsharSocketTypes.begin(); itSockTypes != m_hsharSocketTypes.end(); itSockTypes++ )
        {
            arSocketTypes += itSockTypes.value();
        }
    }
    else
    {
        if( m_hsharSocketTypes.contains(i_protocolType) )
        {
            arSocketTypes = m_hsharSocketTypes[i_protocolType];
        }
    }
    return arSocketTypes;

} // getSocketTypes

//------------------------------------------------------------------------------
bool CDb::isSocketTypeSupported( ESocketType i_socketType, EProtocolType i_protocolType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bIsSupported = false;

    if( i_protocolType == EProtocolTypeCount || i_protocolType == EProtocolTypeUndefined )
    {
        QList<ESocketType> arSocketTypes = getSocketTypes();
        bIsSupported = arSocketTypes.contains(i_socketType);
    }
    else
    {
        if( m_hsharSocketTypes.contains(i_protocolType) )
        {
            bIsSupported = m_hsharSocketTypes[i_protocolType].contains(i_socketType);
        }
    }
    return bIsSupported;

} // isSocketTypeSupported

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
QString CDb::getConnectionString(
    EProtocolType i_protocolType,
    ESocketType   i_socketType,
    ERowVersion   i_version ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    SServerHostSettings hostSettings = getHostSettings(i_protocolType, i_socketType, i_version);

    return hostSettings.getConnectionString();

} // getConnectionString

//------------------------------------------------------------------------------
SServerHostSettings CDb::getHostSettings(
    EProtocolType i_protocolType,
    ESocketType   i_socketType,
    ERowVersion   i_version ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( !isSocketTypeSupported(i_socketType,i_protocolType) )
    {
        QString strAddErrInfo = "SocketType \"" + socketType2Str(i_socketType) + "\"";
        strAddErrInfo += " for protocol type \"" + protocolType2Str(i_protocolType) + "\" not supported";
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo);
    }

    SServerHostSettings hostSettings;

    SCnctType cnctType(i_protocolType, i_socketType);

    if( i_version == ERowVersion::Original )
    {
        hostSettings = m_hshHostSettingsOrig[cnctType];
    }
    else if( i_version == ERowVersion::Current )
    {
        hostSettings = m_hshHostSettingsCurr.value(cnctType, hostSettings);
    }
    else if( i_version == ERowVersion::Default || i_version == ERowVersion::Undefined )
    {
        if( m_hshHostSettingsCurr.contains(cnctType) )
        {
            hostSettings = m_hshHostSettingsCurr[cnctType];
        }
        else
        {
            hostSettings = m_hshHostSettingsOrig[cnctType];
        }
    }

    return hostSettings;

} // getHostSettings

//------------------------------------------------------------------------------
void CDb::setHostSettings(
    EProtocolType              i_protocolType,
    const SServerHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", HostSettings {" + i_hostSettings.getConnectionString() + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setHostSettings",
        /* strAddInfo   */ strAddTrcInfo );

    if( !isSocketTypeSupported(i_hostSettings.m_socketType,i_protocolType) )
    {
        QString strAddErrInfo = "SocketType \"" + socketType2Str(i_hostSettings.m_socketType) + "\"";
        strAddErrInfo += " for protocol type \"" + protocolType2Str(i_protocolType) + "\" not supported";
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo);
    }

    SCnctType cnctType(i_protocolType, i_hostSettings.m_socketType);

    SServerHostSettings hostSettings = i_hostSettings;

    // There is no need to set host settings for the InProcMsg socket type.
    // There are no parameters which can be changed. But if someone calls
    // this method with InProcMsg we ensure that the structure has got the
    // correct values.
    if( hostSettings.m_socketType == ESocketTypeInProcMsg )
    {
        // ?? Or should we throw an exception here if not set to this and objectName?
        if( hostSettings.m_pObjLocal != nullptr && hostSettings.m_pObjLocal != this )
        {
            throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "Invalid host settings for InProcMsg socket type" );
        }
        if( !hostSettings.m_strLocalHostName.isEmpty() && hostSettings.m_strLocalHostName != objectName() )
        {
            throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "Invalid host settings for InProcMsg socket type" );
        }

        hostSettings.m_pObjLocal = this;
        hostSettings.m_strLocalHostName = objectName();
        hostSettings.m_hostAddrLocal = QHostAddress();
        hostSettings.m_uLocalPort = 0;
        hostSettings.m_uMaxPendingConnections = 0;
        hostSettings.m_uBufferSize = 0;

    } // if( hostSettings.m_socketType == ESocketTypeInProcMsg )

    bool bSettingsChanged = false;
    bool bVersionChanged = false;

    // If there is already a current version available ..
    if( m_hshHostSettingsCurr.contains(cnctType) )
    {
        // If the new setting is different from the current version ...
        if( m_hshHostSettingsCurr[cnctType] != hostSettings )
        {
            // If the new setting is equal to the original version ...
            if( m_hshHostSettingsOrig[cnctType] == hostSettings )
            {
                // .. invalidate current version. Nothing to be applied later on.
                m_hshHostSettingsCurr.remove(cnctType);
                bSettingsChanged = true;
                bVersionChanged = true;
            }
            // If the new setting is different from the original version ...
            else
            {
                // ... take over new setting as the current version to be applied later on.
                m_hshHostSettingsCurr[cnctType] = hostSettings;
                bSettingsChanged = true;
            }
        } // if( m_hshHostSettingsCurr[cnctType] != hostSettings )
    } // if( m_hshHostSettingsCurr.contains(cnctType) )

    // If a current version is not yet available ..
    else // if( !m_hshHostSettingsCurr.contains(cnctType) )
    {
        // If the new setting is different from the original settings ...
        if( m_hshHostSettingsOrig[cnctType] != hostSettings )
        {
            // ... take over new setting as the current version to be applied later on.
            m_hshHostSettingsCurr[cnctType] = hostSettings;
            bSettingsChanged = true;
            bVersionChanged = true;
        }
    }

    if( bVersionChanged )
    {
        emit hostSettingsRowVersionChanged(this, cnctType.m_protocolType, cnctType.m_socketType);
    }
    if( bSettingsChanged )
    {
        hostSettings = getHostSettings(cnctType.m_protocolType, cnctType.m_socketType);

        emit hostSettingsChanged(this, cnctType.m_protocolType, hostSettings);
    }

} // setHostSettings

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDb::getDefaultConnectionString(
    EProtocolType i_protocolType,
    ESocketType   i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strConnection;

    SServerHostSettings* pHostSettings = getDefaultHostSettings(i_protocolType, i_socketType);

    if( pHostSettings != nullptr )
    {
        strConnection = pHostSettings->getConnectionString();
    }
    return strConnection;

} // getDefaultConnectionString

//------------------------------------------------------------------------------
SServerHostSettings* CDb::getDefaultHostSettings(
    EProtocolType i_protocolType,
    ESocketType   i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    SServerHostSettings* pHostSettings = nullptr;

    SCnctType cnctType(i_protocolType,i_socketType);

    if( m_hshHostSettingsDefault.contains(cnctType) )
    {
        CDb* pVThis = const_cast<CDb*>(this);

        pHostSettings = &pVThis->m_hshHostSettingsDefault[cnctType];
    }
    return pHostSettings;

} // getDefaultHostSettings

//------------------------------------------------------------------------------
void CDb::setDefaultHostSettings(
    EProtocolType              i_protocolType,
    const SServerHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", HostSettings {" + i_hostSettings.getConnectionString() + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDefaultHostSettings",
        /* strAddInfo   */ strAddTrcInfo );

    if( !isSocketTypeSupported(i_hostSettings.m_socketType,i_protocolType) )
    {
        QString strAddErrInfo = "SocketType \"" + socketType2Str(i_hostSettings.m_socketType) + "\"";
        strAddErrInfo += " for protocol type \"" + protocolType2Str(i_protocolType) + "\" not supported";
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo);
    }

    SCnctType cnctType(i_protocolType, i_hostSettings.m_socketType);

    SServerHostSettings hostSettings = i_hostSettings;

    // There is no need to set host settings for the InProcMsg socket type.
    // There are no parameters which can be changed. But if someone calls
    // this method with InProcMsg we ensure that the structure has got the
    // correct values.
    if( hostSettings.m_socketType == ESocketTypeInProcMsg )
    {
        // ?? Or should we throw an exception here if not set to this and objectName?
        if( hostSettings.m_pObjLocal != nullptr && hostSettings.m_pObjLocal != this )
        {
            throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "Invalid host settings for InProcMsg socket type" );
        }
        if( !hostSettings.m_strLocalHostName.isEmpty() && hostSettings.m_strLocalHostName != objectName() )
        {
            throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "Invalid host settings for InProcMsg socket type" );
        }

        hostSettings.m_pObjLocal = this;
        hostSettings.m_strLocalHostName = objectName();
        hostSettings.m_hostAddrLocal = QHostAddress();
        hostSettings.m_uLocalPort = 0;
        hostSettings.m_uMaxPendingConnections = 0;
        hostSettings.m_uBufferSize = 0;

    } // if( hostSettings.m_socketType == ESocketTypeInProcMsg )

    SServerHostSettings hostSettingsPrev;

    if( m_hshHostSettingsDefault.contains(cnctType) )
    {
        hostSettingsPrev = m_hshHostSettingsDefault[cnctType];
    }

    if( hostSettings != hostSettingsPrev )
    {
        m_hshHostSettingsDefault[cnctType] = hostSettings;

        emit defaultHostSettingsChanged(this, i_protocolType, hostSettings);
    }

} // setDefaultHostSettings

/*==============================================================================
public: // instance methods (affecting all settings)
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::setHostSettings2Default(
    EProtocolType i_protocolType,
    ESocketType   i_socketType )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", SockType: " + socketType2Str(i_socketType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setHostSettings2Default",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.trace(strAddTrcInfo);
    }

    EProtocolType protTypeMin = i_protocolType;
    EProtocolType protTypeMax = i_protocolType;
    ESocketType   sockTypeMin = i_socketType;
    ESocketType   sockTypeMax = i_socketType;

    if( i_protocolType == EProtocolTypeCount || i_protocolType == EProtocolTypeUndefined )
    {
        protTypeMin = static_cast<EProtocolType>(0);
        protTypeMax = static_cast<EProtocolType>(EProtocolTypeCount-1);
    }
    if( i_socketType == ESocketTypeCount || i_socketType == ESocketTypeUndefined )
    {
        sockTypeMin = static_cast<ESocketType>(0);
        sockTypeMax = static_cast<ESocketType>(ESocketTypeCount-1);
    }

    SCnctType cnctType;
    int       iProtType;
    int       iSockType;

    for( iProtType = protTypeMin; iProtType <= protTypeMax; iProtType++ )
    {
        for( iSockType = sockTypeMin; iSockType <= sockTypeMax; iSockType++ )
        {
            cnctType.m_protocolType = static_cast<EProtocolType>(iProtType);
            cnctType.m_socketType   = static_cast<ESocketType>(iSockType);

            if( m_hshHostSettingsDefault.contains(cnctType) )
            {
                setHostSettings(cnctType.m_protocolType, m_hshHostSettingsDefault[cnctType]);
            }
        } // for( iSockType = sockTypeMin; iSockType <= sockTypeMax; iSockType++ )
    } // for( iProtType = protTypeMin; iProtType <= protTypeMax; iProtType++ )

} // setHostSettings2Default

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QHash<SCnctId, ZS::Ipc::SSocketDscr> CDb::getClientCncts( EProtocolType i_protocolType, ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QHash<SCnctId, ZS::Ipc::SSocketDscr> hshCncts;

    if( (i_protocolType == EProtocolTypeCount || i_protocolType == EProtocolTypeUndefined)
     && (i_socketType == ESocketTypeCount || i_socketType == ESocketTypeUndefined) )
    {
        hshCncts = m_hshDbCltsConnected;
    }
    else // if( i_protocolType != EProtocolTypeCount && i_protocolType != EProtocolTypeUndefined ..
    {
        QHash<SCnctId,Ipc::SSocketDscr>::const_iterator itDbClts;

        for( itDbClts = m_hshDbCltsConnected.begin(); itDbClts != m_hshDbCltsConnected.end(); itDbClts++ )
        {
            if( i_protocolType == EProtocolTypeCount || i_protocolType == EProtocolTypeUndefined )
            {
                if( itDbClts.key().m_socketType == i_socketType )
                {
                    hshCncts[itDbClts.key()] = itDbClts.value();
                }
            }
            else if( i_socketType == ESocketTypeCount || i_socketType == ESocketTypeUndefined )
            {
                if( itDbClts.key().m_protocolType == i_protocolType )
                {
                    hshCncts[itDbClts.key()] = itDbClts.value();
                }
            }
            else if( itDbClts.key().m_protocolType == i_protocolType && itDbClts.key().m_socketType == i_socketType )
            {
                hshCncts[itDbClts.key()] = itDbClts.value();
            }
        } // for( itDbClts = m_hshDbCltsConnected.begin(); itDbClts != m_hshDbCltsConnected.end(); itDbClts++ )
    } // if( i_protocolType != EProtocolTypeCount && i_protocolType != EProtocolTypeUndefined ..

    return hshCncts;

} // getClientCncts

//------------------------------------------------------------------------------
QList<SCnctId> CDb::getClientCnctIds( EProtocolType i_protocolType, ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QList<SCnctId> arCnctIds;

    if( (i_protocolType == EProtocolTypeCount || i_protocolType == EProtocolTypeUndefined)
     && (i_socketType == ESocketTypeCount || i_socketType == ESocketTypeUndefined) )
    {
        arCnctIds = m_hshDbCltsConnected.keys();
    }
    else // if( i_protocolType != EProtocolTypeCount && i_protocolType != EProtocolTypeUndefined ..
    {
        QHash<SCnctId,Ipc::SSocketDscr>::const_iterator itDbClts;

        for( itDbClts = m_hshDbCltsConnected.begin(); itDbClts != m_hshDbCltsConnected.end(); itDbClts++ )
        {
            if( i_protocolType == EProtocolTypeCount || i_protocolType == EProtocolTypeUndefined )
            {
                if( itDbClts.key().m_socketType == i_socketType )
                {
                    arCnctIds += itDbClts.key();
                }
            }
            else if( i_socketType == ESocketTypeCount || i_socketType == ESocketTypeUndefined )
            {
                if( itDbClts.key().m_protocolType == i_protocolType )
                {
                    arCnctIds += itDbClts.key();
                }
            }
            else if( itDbClts.key().m_protocolType == i_protocolType && itDbClts.key().m_socketType == i_socketType )
            {
                arCnctIds += itDbClts.key();
            }
        } // for( itDbClts = m_hshDbCltsConnected.begin(); itDbClts != m_hshDbCltsConnected.end(); itDbClts++ )
    } // if( i_protocolType != EProtocolTypeCount && i_protocolType != EProtocolTypeUndefined ..

    return arCnctIds;

} // getClientCnctIds

//------------------------------------------------------------------------------
QList<SSocketDscr> CDb::getClientCnctDscrs( EProtocolType i_protocolType, ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QList<SSocketDscr> arCnctDscrs;

    if( (i_protocolType == EProtocolTypeCount || i_protocolType == EProtocolTypeUndefined)
     && (i_socketType == ESocketTypeCount || i_socketType == ESocketTypeUndefined) )
    {
        arCnctDscrs = m_hshDbCltsConnected.values();
    }
    else // if( i_protocolType != EProtocolTypeCount && i_protocolType != EProtocolTypeUndefined ..
    {
        QHash<SCnctId,Ipc::SSocketDscr>::const_iterator itDbClts;

        for( itDbClts = m_hshDbCltsConnected.begin(); itDbClts != m_hshDbCltsConnected.end(); itDbClts++ )
        {
            if( i_protocolType == EProtocolTypeCount || i_protocolType == EProtocolTypeUndefined )
            {
                if( itDbClts.key().m_socketType == i_socketType )
                {
                    arCnctDscrs += itDbClts.value();
                }
            }
            else if( i_socketType == ESocketTypeCount || i_socketType == ESocketTypeUndefined )
            {
                if( itDbClts.key().m_protocolType == i_protocolType )
                {
                    arCnctDscrs += itDbClts.value();
                }
            }
            else if( itDbClts.key().m_protocolType == i_protocolType && itDbClts.key().m_socketType == i_socketType )
            {
                arCnctDscrs += itDbClts.value();
            }
        } // for( itDbClts = m_hshDbCltsConnected.begin(); itDbClts != m_hshDbCltsConnected.end(); itDbClts++ )
    } // if( i_protocolType != EProtocolTypeCount && i_protocolType != EProtocolTypeUndefined ..

    return arCnctDscrs;

} // getClientCnctDscrs

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CDb::getClientCnctsCount(
    EProtocolType i_protocolType,
    ESocketType   i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int iCnctsCount = 0;

    if( (i_protocolType == EProtocolTypeCount || i_protocolType == EProtocolTypeUndefined)
     && (i_socketType == ESocketTypeCount || i_socketType == ESocketTypeUndefined) )
    {
        iCnctsCount = m_hshDbCltsConnected.size();
    }
    else // if( i_protocolType != EProtocolTypeCount && i_protocolType != EProtocolTypeUndefined ..
    {
        QHash<SCnctId,Ipc::SSocketDscr>::const_iterator itDbClts;

        for( itDbClts = m_hshDbCltsConnected.begin(); itDbClts != m_hshDbCltsConnected.end(); itDbClts++ )
        {
            if( i_protocolType == EProtocolTypeCount || i_protocolType == EProtocolTypeUndefined )
            {
                if( itDbClts.key().m_socketType == i_socketType )
                {
                    iCnctsCount++;
                }
            }
            else if( i_socketType == ESocketTypeCount || i_socketType == ESocketTypeUndefined )
            {
                if( itDbClts.key().m_protocolType == i_protocolType )
                {
                    iCnctsCount++;
                }
            }
            else if( itDbClts.key().m_protocolType == i_protocolType && itDbClts.key().m_socketType == i_socketType )
            {
                iCnctsCount++;
            }
        } // for( itDbClts = m_hshDbCltsConnected.begin(); itDbClts != m_hshDbCltsConnected.end(); itDbClts++ )
    } // if( i_protocolType != EProtocolTypeCount && i_protocolType != EProtocolTypeUndefined ..

    return iCnctsCount;

} // getClientCnctsCount

//------------------------------------------------------------------------------
SCnctId CDb::getClientCnctId(
    int           i_idxCnct,
    EProtocolType i_protocolType,
    ESocketType   i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QList<SCnctId> arCnctIds = getClientCnctIds(i_protocolType, i_socketType);

    SCnctId cnctId(i_protocolType, i_socketType);

    if( i_idxCnct >= 0 && i_idxCnct < arCnctIds.size() )
    {
        cnctId = arCnctIds[i_idxCnct];
    }

    return cnctId;

} // getClientCnctId

//------------------------------------------------------------------------------
SSocketDscr CDb::getClientCnctDscr(
    int           i_idxCnct,
    EProtocolType i_protocolType,
    ESocketType   i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    SSocketDscr cnctDscr;

    QList<SSocketDscr> arSocketDscrs = getClientCnctDscrs(i_protocolType, i_socketType);

    if( i_idxCnct >= 0 && i_idxCnct < arSocketDscrs.size() )
    {
        cnctDscr = arSocketDscrs[i_idxCnct];
    }

    return cnctDscr;

} // getClientCnctDscr

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CDb::EState CDb::getState() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_state;
}

//------------------------------------------------------------------------------
QString CDb::state2Str() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return State2Str(m_state);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDb::isListening( EProtocolType i_protocolType, ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bIsListening = false;

    if( m_arProtocolTypes.contains(i_protocolType) && m_hsharSocketTypes.contains(i_protocolType) )
    {
        if( m_hsharSocketTypes[i_protocolType].contains(i_socketType) )
        {
            if( i_socketType == ESocketTypeInProcMsg )
            {
                // If supported direct message dispatching through events is always
                // in listening state (no gateway needed).
                bIsListening = true;
            }
            else
            {
                SCnctType cnctType(i_protocolType,i_socketType);

                if( m_hshpGateways.contains(cnctType) )
                {
                    CDbGateway* pGateway = m_hshpGateways[cnctType];
                    bIsListening = (pGateway != nullptr);
                }
            }
        }
    } // if( m_arProtocolTypes.contains(i_protocolType) && m_hsharSocketTypes.contains(i_protocolType) )

    return bIsListening;

} // isListening

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDb::isConnected(
    EProtocolType i_protocolType,
    ESocketType   i_socketType,
    int           i_iSocketId ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bIsConnected = false;

    if( i_iSocketId == ESocketIdUndefined )
    {
        bIsConnected = (m_hshDbCltsConnected.size() > 0);
    }
    else
    {
        SCnctId cnctId(i_protocolType,i_socketType,i_iSocketId);
        bIsConnected = m_hshDbCltsConnected.contains(cnctId);
    }

    return bIsConnected;

} // isConnected

//------------------------------------------------------------------------------
SErrResultInfo CDb::checkSocketId(
    EProtocolType i_protocolType,
    ESocketType   i_socketType,
    int           i_iSocketId ) const
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = ErrResultInfoSuccess("checkSocketId");

    if( i_iSocketId < 0 )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Socket Id " + QString::number(i_iSocketId) + " is out of range." );
    }
    else
    {
        SCnctId cnctId(i_protocolType, i_socketType, i_iSocketId);

        if( !m_hshDbCltsConnected.contains(cnctId) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Socket Id " + QString::number(i_iSocketId) + " is out of range." );
        }
    }

    return errResultInfo;

} // checkSocketId

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDb::isBusy() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return isRequestInProgress();
}

//------------------------------------------------------------------------------
bool CDb::isRequestInProgress(
    int      i_iRequestId,
    ERequest i_request,
    bool     i_bIgnorePostponedRequests ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bRequestInProgress = false;

    // Please note that it is not sufficient just to check the "requestInProgress" member
    // variable to decide whether currently a request is in progress or not. After a request
    // has been executed the member variable "requestInProgress" is set to "None". But there
    // might still be postponed requests in the request list. Receivers of the signal may call
    // one of the public request methods as an reentry within the slots connected to those
    // signals. After emitting the "requestChanged" signal (in general within the event
    // method) the request queue is checked for postponed requests.

    //SGatewayEntry* pGatewayEntry;
    CRequest*      pReq;
    int            idxReq;

    // If checking for any request in progress ..
    if( i_iRequestId < 0 && i_request == ERequestUndefined )
    {
        if( m_pRequestQueue->isRequestInProgress() )
        {
            bRequestInProgress = true;
        }
        else if( !i_bIgnorePostponedRequests && m_pRequestQueue->hasPostponedRequest() )
        {
            bRequestInProgress = true;
        }
    }

    // If checking for a specific request by id and request type ..
    else if( i_iRequestId >= 0 && i_request != ERequestUndefined )
    {
        if( m_pRequestQueue->isRequestInProgress()
         && m_pRequestQueue->getRequestInProgress()->getRequest() == i_request
         && m_pRequestQueue->getRequestInProgress()->getId() == i_iRequestId )
        {
            bRequestInProgress = true;
        }
        if( !bRequestInProgress && !i_bIgnorePostponedRequests )
        {
            for( idxReq = 0; idxReq < m_pRequestQueue->getPostponedRequestsCount(); idxReq++ )
            {
                pReq = m_pRequestQueue->getPostponedRequestByIdx(idxReq);

                if( pReq->getRequest() == i_request && pReq->getId() == i_iRequestId )
                {
                    bRequestInProgress = true;
                    break;
                }
            }
        }
    }

    // If checking for a specific request by id ..
    else if( i_iRequestId >= 0 )
    {
        if( m_pRequestQueue->isRequestInProgress()
         && m_pRequestQueue->getRequestInProgress()->getId() == i_iRequestId )
        {
            bRequestInProgress = true;
        }
        if( !bRequestInProgress && !i_bIgnorePostponedRequests )
        {
            for( idxReq = 0; idxReq < m_pRequestQueue->getPostponedRequestsCount(); idxReq++ )
            {
                pReq = m_pRequestQueue->getPostponedRequestByIdx(idxReq);

                if( pReq->getId() == i_iRequestId )
                {
                    bRequestInProgress = true;
                    break;
                }
            }
        }
    }

    // If checking for a request by type ..
    else if( i_request != ERequestUndefined )
    {
        if( m_pRequestQueue->isRequestInProgress()
         && m_pRequestQueue->getRequestInProgress()->getRequest() == i_request )
        {
            bRequestInProgress = true;
        }
        if( !bRequestInProgress && !i_bIgnorePostponedRequests )
        {
            for( idxReq = 0; idxReq < m_pRequestQueue->getPostponedRequestsCount(); idxReq++ )
            {
                pReq = m_pRequestQueue->getPostponedRequestByIdx(idxReq);

                if( pReq->getRequest() == i_request )
                {
                    bRequestInProgress = true;
                    break;
                }
            }
        }
    }

    return bRequestInProgress;

} // isRequestInProgress

//------------------------------------------------------------------------------
CDb::ERequest CDb::getRequestInProgress() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    ERequest request = ERequestNone;

    if( m_pRequestQueue->isRequestInProgress() )
    {
        request = static_cast<ERequest>(m_pRequestQueue->getRequestInProgress()->getRequest());
    }
    return request;

} // getRequestInProgress

//------------------------------------------------------------------------------
QString CDb::requestInProgress2Str( bool /*i_bShort*/ ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return Request2Str( getRequestInProgress() );
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::abortRequest( qint64 i_iRequestId )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "ReqId: " + QString::number(i_iRequestId);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "abortRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".abortRequest";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    errResultInfo.setSeverity(EResultSeverityInfo);
    errResultInfo.setResult(EResultRequestAborted);

    //EState statePrev = m_state;

    bool bIsPostponedRequest;
    int  idxPostponedRequest;

    CRequest* pReq = m_pRequestQueue->findRequestById(
        /* iRequestId               */ i_iRequestId,
        /* bIgnorePostponedRequests */ false,
        /* pbIsPostponedRequest     */ &bIsPostponedRequest,
        /* pidxPostponedRequest     */ &idxPostponedRequest);

    if( pReq != nullptr )
    {
        pReq->abort();
        //pReq->setErrResultInfo(errResultInfo);
        //pReq->setProgressInPerCent(100);
        pReq->update(); // slot "onRequestChanged" is called.

    } // if( pReq != nullptr }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // abortRequest

//------------------------------------------------------------------------------
void CDb::abortRequestInProgress()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "abortRequestInProgress",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.trace(strAddTrcInfo);
    }

    CRequest* pReq = m_pRequestQueue->getRequestInProgress();

    if( pReq != nullptr )
    {
        abortRequest(pReq->getId());
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // abortRequestInProgress

//------------------------------------------------------------------------------
void CDb::abortAllRequests()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "abortAllRequests",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".abortAllRequests";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    errResultInfo.setSeverity(EResultSeverityInfo);
    errResultInfo.setResult(EResultRequestAborted);

    CRequest*            pReq;
    ZS::System::CMsgReq* pMsgReqInitiator;
    ZS::System::CMsgCon* pMsgConInitiator;

    // Abort postponed requests.
    //--------------------------

    while( m_pRequestQueue->hasPostponedRequest() )
    {
        // Keep the request in request queue until update is called as
        // on updating the request the "onRequestChanged" slot is called
        // which checks whether the request is in the request queue.
        pReq = m_pRequestQueue->getLastPostponedRequest();

        // Progress = 100 % indicates "requestFinished".
        // Please note that on emitting the "changed" signal further requests
        // may be started as a reentry by the receiver(s) of the signal.
        pReq->setErrResultInfo(errResultInfo);
        pReq->setProgressInPerCent(100);
        pReq->update();

        m_pRequestQueue->removePostponedRequest(pReq);

        pMsgReqInitiator = pReq->takeInitiatorRequestMessage();
        pMsgConInitiator = pReq->takeInitiatorConfirmationMessage();

        try
        {
            delete pMsgReqInitiator;
        }
        catch(...)
        {
        }
        pMsgReqInitiator = nullptr;

        if( pMsgConInitiator != nullptr )
        {
            pMsgConInitiator->setErrResultInfo(errResultInfo);
            POST_OR_DELETE_MESSAGE(pMsgConInitiator, &mthTracer);
            pMsgConInitiator = nullptr;
        }
    }

    // Abort requests in progres
    //--------------------------

    if( m_pRequestQueue->isRequestInProgress() )
    {
        abortRequestInProgress();
    }

    if( !m_bIsBeingDestroyed && !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // abortAllRequests

/*==============================================================================
public: // asynchronous request methods (may be queued)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::startup(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms);
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startup",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + state2Str();
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".startup";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestStartup,
        /* strRequest   */ Request2Str(ERequestStartup),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    // Please note that the "onRequestChanged" slot should also be connected
    // to the changed signal of the request if the request will be postponed
    // as postponed requests may have parent request which may be aborted.
    if( !QObject::connect(
        /* pObjSender   */ pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    bool bIsParentReqInProgress = m_pRequestQueue->isRequestInProgress(i_iReqIdParent);
    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_pRequestQueue->isIdle() || bIsParentReqInProgress )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        if( pReq->isBlockingRequest() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }

            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();
            //m_pRequestQueue->unlock();

            // The request is finished and has to be deleted later.
            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
        }
        else
        {
            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);

            if( resultPostpone != EResultSuccess )
            {
                pReq->setSeverity(EResultSeverityError);
                pReq->setResult(resultPostpone);
                pReq->setAddErrInfoDscr( "Request could not have been queued." );
                pReq->setProgressInPerCent(100);
                pReq->update();

                // The request is finished and has to be deleted later.
                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
            }
        }
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeStartupRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.trace(strAddTrcInfo);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Request { " + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + " }";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // startup

//------------------------------------------------------------------------------
CRequest* CDb::shutdown(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms);
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "shutdown",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + state2Str();
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".shutdown";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestShutdown,
        /* strRequest   */ Request2Str(ERequestShutdown),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    // Please note that the "onRequestChanged" slot should also be connected
    // to the changed signal of the request if the request will be postponed
    // as postponed requests may have parent request which may be aborted.
    if( !QObject::connect(
        /* pObjSender   */ pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    bool bIsParentReqInProgress = m_pRequestQueue->isRequestInProgress(i_iReqIdParent);
    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_pRequestQueue->isIdle() || bIsParentReqInProgress )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        if( pReq->isBlockingRequest() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }

            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();
            //m_pRequestQueue->unlock();

            // The request is finished and has to be deleted later.
            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
        }
        else
        {
            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);

            if( resultPostpone != EResultSuccess )
            {
                pReq->setSeverity(EResultSeverityError);
                pReq->setResult(resultPostpone);
                pReq->setAddErrInfoDscr( "Request could not have been queued." );
                pReq->setProgressInPerCent(100);
                pReq->update();

                // The request is finished and has to be deleted later.
                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
            }
        }
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeShutdownRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.trace(strAddTrcInfo);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Request { " + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + " }";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // shutdown

//------------------------------------------------------------------------------
CRequest* CDb::updateHostSettings(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms);
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateHostSettings",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + state2Str();
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".updateHostSettings";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestUpdateHostSettings,
        /* strRequest   */ Request2Str(ERequestUpdateHostSettings),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    // Please note that the "onRequestChanged" slot should also be connected
    // to the changed signal of the request if the request will be postponed
    // as postponed requests may have parent request which may be aborted.
    if( !QObject::connect(
        /* pObjSender   */ pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    bool bIsParentReqInProgress = m_pRequestQueue->isRequestInProgress(i_iReqIdParent);
    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_pRequestQueue->isIdle() || bIsParentReqInProgress )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        if( pReq->isBlockingRequest() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultRequestRefused);
            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }

            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();
            //m_pRequestQueue->unlock();

            // The request is finished and has to be deleted later.
            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
        }
        else
        {
            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);

            if( resultPostpone != EResultSuccess )
            {
                pReq->setSeverity(EResultSeverityError);
                pReq->setResult(resultPostpone);
                pReq->setAddErrInfoDscr( "Request could not have been queued." );
                pReq->setProgressInPerCent(100);
                pReq->update();

                // The request is finished and has to be deleted later.
                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
            }
        }
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeUpdateHostSettingsRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.trace(strAddTrcInfo);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Request { " + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + " }";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // updateHostSettings

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::addTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( i_pObj == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObj == nullptr");
    }

    QObject* pObj = nullptr;
    int      idxObj;

    for( idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
    {
        pObj = m_arpTrcMsgLogObjects[idxObj];

        if( pObj == i_pObj )
        {
            throw CException(__FILE__,__LINE__,EResultObjAlreadyInList);
        }
    }

    m_arpTrcMsgLogObjects.append(i_pObj);

    //CDbGateway* pGateway;
    //SCnctType   cnctType;
    //int         idxProtType;
    //int         idxSockType;

    //for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
    //{
    //    cnctType.m_protocolType = m_arProtocolTypes[idxProtType];

    //    for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
    //    {
    //        cnctType.m_socketType = m_hsharSocketTypes[cnctType.m_protocolType][idxSockType];

    //        if( m_hshpGateways.contains(cnctType) )
    //        {
    //            pGateway = m_hshpGateways[cnctType];

    //            if( pGateway != nullptr )
    //            {
    //                pGateway->addTrcMsgLogObject(i_pObj);
    //            }
    //        }
    //    }
    //}

} // addTrcMsgLogObject

//------------------------------------------------------------------------------
void CDb::removeTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( i_pObj == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObj == nullptr");
    }

    if( m_arpTrcMsgLogObjects.contains(i_pObj) )
    {
        m_arpTrcMsgLogObjects.removeOne(i_pObj);
    }

    //CDbGateway* pGateway;
    //SCnctType   cnctType;
    //int         idxProtType;
    //int         idxSockType;

    //for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
    //{
    //    cnctType.m_protocolType = m_arProtocolTypes[idxProtType];

    //    for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
    //    {
    //        cnctType.m_socketType = m_hsharSocketTypes[cnctType.m_protocolType][idxSockType];

    //        if( m_hshpGateways.contains(cnctType) )
    //        {
    //            pGateway = m_hshpGateways[cnctType];

    //            if( pGateway != nullptr )
    //            {
    //                pGateway->removeTrcMsgLogObject(i_pObj);
    //            }
    //        }
    //    }
    //}

} // removeTrcMsgLogObject

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::createDataSet()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createDataSet",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_fctCreateDataSet != nullptr )
    {
        m_pDS = m_fctCreateDataSet();
    }
    else
    {
        m_pDS = new CDataSet( objectName() );
    }

    if( m_pDS != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ m_pDS,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDSDestroyed(QObject*)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( m_pDS != nullptr )

} // createDataSet

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::executeNextPostponedRequest()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeNextPostponedRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + state2Str();
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
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
            POST_OR_DELETE_MESSAGE(pMsgReq,&mthTracer);
        }
    } // if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.trace(strAddTrcInfo);
    }

} // executeNextPostponedRequest

/*==============================================================================
protected: // overridables of state machine
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::executeStartupRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req { " + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + " }";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeStartupRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeStartupRequest";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    //QString strAddErrInfo;

    //int iProgress_perCent = 100;

    i_pReq->lock();

    m_pRequestQueue->lock();

    // If the request has been started as a child of the current request in progress ..
    if( m_pRequestQueue->isRequestInProgress() )
    {
        m_pRequestQueue->addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_pRequestQueue->setRequestInProgress(i_pReq);
    }

    emit requestInProgressChanged( this, i_pReq->getDescriptor() );

    EState statePrev = m_state;

    // If none of the gateway threads has been started yet ...
    if( m_state == EStateIdle )
    {
        createGatewayThreads();

        // StartGatewayThreads is a blocking request
        startGatewayThreads(10000, i_pReq->getId());

        // StartupGateways is also a blocking request
        startupGateways( i_pReq->getTimeoutInMs(), i_pReq->getId() );

    } // if( m_state == EStateIdle )

    else // if( m_state != EStateIdle )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Server is already running." );

    } // if( m_state != EStateIdle )

    acceptChangedSettings();

    i_pReq->setErrResultInfo(errResultInfo);
    i_pReq->setProgressInPerCent(100);
    i_pReq->update();

    // Please note that all called methods have been blocking requests and the overall startup
    // request may therefore not become asynchronous. And please also note that on calling update
    // for the finished request the request has been removed from the request queue by the
    // "onRequestChanged" slot and therefore the following two statements are superfluous but
    // have been kept for the sake of clarification.
    if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) )
    {
        m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
    }

    // Please note that the "onRequestChanged" slot is connected with the "changed"
    // signal of the request. If the request has been finished and updated a view
    // lines above the slot will be called disconnecting itself from the signal.

    // Don't delete the request right now but return the request to the caller
    // as the caller may want to evaluate some details of the executed request.
    // E.g. if the request has been started by a request sequencer the sequencer
    // wants to update the request id.
    // The request will be set at the request queue to be deleted later
    // (on destroying the queue or if the next sync request will be set).
    m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);

    if( statePrev != m_state )
    {
        emit stateChanged(this, m_state);
    }

    m_pRequestQueue->unlock();

    i_pReq->unlock();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeStartupRequest

//------------------------------------------------------------------------------
void CDb::executeShutdownRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req { " + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + " }";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeShutdownRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeShutdownRequest";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    //QString strAddErrInfo;

    //int iProgress_perCent = 100;

    i_pReq->lock();

    m_pRequestQueue->lock();

    // If the request has been started as a child of the current request in progress ..
    if( m_pRequestQueue->isRequestInProgress() )
    {
        m_pRequestQueue->addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_pRequestQueue->setRequestInProgress(i_pReq);
    }

    emit requestInProgressChanged( this, i_pReq->getDescriptor() );

    EState statePrev = m_state;

    if( m_state == EStateListening )
    {
        // ShutdownGateways is a blocking request
        shutdownGateways( i_pReq->getTimeoutInMs(), i_pReq->getId() );

        // StopGatewayThreads is also a blocking request
        stopGatewayThreads( i_pReq->getTimeoutInMs(), i_pReq->getId() );

    } // if( m_state == EStateListening )

    else // if( m_state != EStateListening )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Server is not started." );

    } // if( m_state != EStateListening )

    i_pReq->setErrResultInfo(errResultInfo);
    i_pReq->setProgressInPerCent(100);
    i_pReq->update();

    // Please note that all called methods have been blocking requests and the overall startup
    // request may therefore not become asynchronous. And please also note that on calling update
    // for the finished request the request has been removed from the request queue by the
    // "onRequestChanged" slot and therefore the following two statements are superfluous but
    // have been kept for the sake of clarification.
    if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) )
    {
        m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
    }

    // Please note that the "onRequestChanged" slot is connected with the "changed"
    // signal of the request. If the request has been finished and updated a view
    // lines above the slot will be called disconnecting itself from the signal.

    // Don't delete the request right now but return the request to the caller
    // as the caller may want to evaluate some details of the executed request.
    // E.g. if the request has been started by a request sequencer the sequencer
    // wants to update the request id.
    // The request will be set at the request queue to be deleted later
    // (on destroying the queue or if the next sync request will be set).
    m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);

    if( statePrev != m_state )
    {
        emit stateChanged(this, m_state);
    }

    m_pRequestQueue->unlock();

    i_pReq->unlock();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeShutdownRequest

//------------------------------------------------------------------------------
void CDb::executeUpdateHostSettingsRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req { " + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + " }";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeUpdateHostSettingsRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";

        if( !m_hshHostSettingsCurr.isEmpty() )
        {
            QHash<SCnctType,ZS::Ipc::SServerHostSettings>::const_iterator itHostSettings;

            SServerHostSettings hostSettings;
            SCnctType           cnctType;

            for( itHostSettings = m_hshHostSettingsCurr.begin(); itHostSettings != m_hshHostSettingsCurr.end(); itHostSettings++ )
            {
                cnctType     = itHostSettings.key();
                hostSettings = itHostSettings.value();

                strAddTrcInfo = ", [" + cnctType.toString() + ":";
                strAddTrcInfo = " SettingsCurr {" + hostSettings.getConnectionString(iAddTrcInfoDetailLevel)  + "}";
                strAddTrcInfo = ", SettingsOrig {" + hostSettings.getConnectionString(iAddTrcInfoDetailLevel)  + "}]";
            }
        }
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeUpdateHostSettingsRequest";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    int iProgress_perCent = 100;

    i_pReq->lock();

    m_pRequestQueue->lock();

    // If the request has been started as a child of the current request in progress ..
    if( m_pRequestQueue->isRequestInProgress() )
    {
        m_pRequestQueue->addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_pRequestQueue->setRequestInProgress(i_pReq);
    }

    emit requestInProgressChanged( this, i_pReq->getDescriptor() );

    EState statePrev = m_state;

    // If there are no changes to be updated ..
    if( m_hshHostSettingsCurr.isEmpty() )
    {
        errResultInfo.setResult(EResultSuccess);

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();
    }

    // If there are changes to be updated ..
    else // if( !m_hshHostSettingsCurr.isEmpty() )
    {
        SErrResultInfo errResultInfoTmp = ErrResultInfoSuccess(strMth);

        QHash<SCnctType,ZS::Ipc::SServerHostSettings>::iterator itCnct;

        SCnctType              cnctType;
        CDbGateway*            pGateway;
        CRequest*              pReqUpdateGateway;
        CMsgReqChangeSettings* pMsgReq;
        SServerHostSettings    hostSettings;
        bool                   bAsynchronous = false;

        for( itCnct = m_hshHostSettingsCurr.begin(); itCnct != m_hshHostSettingsCurr.end(); itCnct++ )
        {
            cnctType = itCnct.key();
            hostSettings = itCnct.value();

            pGateway = m_hshpGateways.value(cnctType, nullptr);

            if( pGateway != nullptr )
            {
                // Initialized as an asynchronous request (ResultUndefined).
                pReqUpdateGateway = m_pRequestQueue->createRequest(
                    /* strNameSpace */ nameSpace(),
                    /* strClassName */ className(),
                    /* iRequest     */ ERequestUpdateHostSettings,
                    /* strRequest   */ Request2Str(ERequestUpdateHostSettings),
                    /* iReqIdParent */ i_pReq->getId(),
                    /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
                    /* bIsBlocking  */ i_pReq->isBlockingRequest() );

                m_pRequestQueue->addRequestInProgress(pReqUpdateGateway);

                pMsgReq = new CMsgReqChangeSettings(
                    /* pObjSender       */ this,
                    /* pObjReceiver     */ pGateway,
                    /* protocolType     */ cnctType.m_protocolType,
                    /* hostSettings     */ hostSettings,
                    /* bMustBeConfirmed */ true,
                    /* iReqId           */ pReqUpdateGateway->getId() );

                pMsgReq->setTimeoutInMs(i_pReq->getTimeoutInMs());
                pMsgReq->setIsBlockingRequest(i_pReq->isBlockingRequest());

                POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);
                pMsgReq = nullptr;

                if( pReqUpdateGateway->isBlockingRequest() )
                {
                    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                    {
                        mthTracer.trace( "-+ ReqUpdateGateway.wait()" );
                    }

                    if( pReqUpdateGateway->wait() )
                    {
                        CMsgConChangeSettings* pMsgCon = dynamic_cast<CMsgConChangeSettings*>(i_pReq->takeExecutionConfirmationMessage());

                        if( pMsgCon != nullptr )
                        {
                            errResultInfoTmp = pMsgCon->getErrResultInfo();

                            pReqUpdateGateway->setErrResultInfo(errResultInfoTmp);
                            pReqUpdateGateway->setProgressInPerCent(100);
                            pReqUpdateGateway->update();

                            delete pMsgCon;
                            pMsgCon = nullptr;
                        }
                        else
                        {
                            errResultInfoTmp = pReqUpdateGateway->getErrResultInfo();
                        }
                    }
                    else // if( !pReqUpdateGateway->wait() )
                    {
                        errResultInfoTmp.setSeverity(EResultSeverityError);
                        errResultInfoTmp.setResult(EResultTimeout);
                        errResultInfoTmp.setAddErrInfoDscr( "Waiting for change settings confirmation from gateway timed out." );

                        pReqUpdateGateway->setErrResultInfo(errResultInfoTmp);
                        pReqUpdateGateway->setProgressInPerCent(100);
                        pReqUpdateGateway->update();
                    }

                    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                    {
                        mthTracer.trace( "+- ReqUpdateGateway.wait(): " + errResultInfoTmp.getAddErrInfoDscr() );
                    }
                } // if( pReqUpdateGateway->isBlockingRequest() )

                if( isAsynchronousRequest(pReqUpdateGateway) )
                {
                    bAsynchronous = true;

                    if( pReqUpdateGateway->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
                    {
                        m_reqDscrTimeout = pReqUpdateGateway->getDescriptor();
                        m_pTmrReqTimeout->start(pReqUpdateGateway->getTimeoutInMs());
                    }

                    // Please note that the "onRequestChanged" slot for internal requests which
                    // cannot be postponed will only be connected if the request becomes asynchronous.
                    if( !QObject::connect(
                        /* pObjSender   */ pReqUpdateGateway,
                        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                        /* pObjReceiver */ this,
                        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                    }
                }
                else // if( !isAsynchronousRequest(pReqUpdateGateway) )
                {
                    if( m_pRequestQueue->isRequestInProgress(pReqUpdateGateway->getId()) ) // as the request may have already been removed on setting the progress to 100 %
                    {
                        m_pRequestQueue->removeRequestInProgress( pReqUpdateGateway->getId() );
                    }

                    // The one who calls update for the finished request must delete the request.
                    deleteRequest(pReqUpdateGateway);
                    pReqUpdateGateway = nullptr;

                } // if( !isAsynchronousRequest(pReqUpdateGateway) )

                if( errResultInfoTmp.isErrorResult() && CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfoTmp);
                }
            } // if( pGateway != nullptr )
        } // for( itCnct = m_hshHostSettingsCurr.begin(); itCnct != m_hshHostSettingsCurr.end(); itCnct++ )

        if( bAsynchronous )
        {
            iProgress_perCent = 50;
        }
    } // if( !m_hshHostSettingsCurr.isEmpty() )

    acceptChangedSettings();

    i_pReq->setErrResultInfo(errResultInfo);
    i_pReq->setProgressInPerCent(iProgress_perCent);
    i_pReq->update();

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start(i_pReq->getTimeoutInMs());
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Pleae note that the request may have already been removed from the request queue
        // by the "onRequestChanged" slot on calling update for the finished request.
        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) )
        {
            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
        }

        // Please note that the "onRequestChanged" slot is connected with the "changed"
        // signal of the request. If the request has been finished and updated a view
        // lines above the slot will be called disconnecting itself from the signal.

        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).
        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);

    } // if( !isAsynchronousRequest(i_pReq) )

    if( statePrev != m_state )
    {
        emit stateChanged(this, m_state);
    }

    m_pRequestQueue->unlock();

    i_pReq->unlock();

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";

        if( !m_hshHostSettingsCurr.isEmpty() )
        {
            QHash<SCnctType,ZS::Ipc::SServerHostSettings>::const_iterator itHostSettings;

            SServerHostSettings hostSettings;
            SCnctType           cnctType;

            for( itHostSettings = m_hshHostSettingsCurr.begin(); itHostSettings != m_hshHostSettingsCurr.end(); itHostSettings++ )
            {
                cnctType     = itHostSettings.key();
                hostSettings = itHostSettings.value();

                strAddTrcInfo = ", [" + cnctType.toString() + ":";
                strAddTrcInfo = " SettingsCurr {" + hostSettings.getConnectionString(iAddTrcInfoDetailLevel)  + "}";
                strAddTrcInfo = ", SettingsOrig {" + hostSettings.getConnectionString(iAddTrcInfoDetailLevel)  + "}]";
            }
        }
        mthTracer.trace(strAddTrcInfo);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeUpdateHostSettingsRequest

/*==============================================================================
protected: // overridables (auxiliary methods)
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::createGatewayThreads()
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createGatewayThreads",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".createGatewayThreads";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    SCnctType     cnctType;
    EProtocolType protType;
    ESocketType   sockType;
    int           idxProtType;
    int           idxSockType;

    for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
    {
        protType = m_arProtocolTypes[idxProtType];
        cnctType.m_protocolType = protType;

        if( protType == EProtocolTypeZSXML || protType == EProtocolTypeZSMsg )
        {
            for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
            {
                sockType = m_hsharSocketTypes[protType][idxSockType];
                cnctType.m_socketType = sockType;

                if( sockType == ESocketTypeTcp )
                {
                    m_hshpGatewayThreads[cnctType] = new CDbGatewayThread(
                        /* protocolType */ protType,
                        /* sockectType  */ sockType,
                        /* pDb          */ this );
                }
                else if( sockType == ESocketTypeInProcMsg )
                {
                    // No gateway necessary. Messages are directly sent from client
                    // to event method of database object.
                }
                else // if( sockType != ESocketTypeTcp && sockType != ESocketTypeInProcMsg )
                {
                    strAddErrInfo = "Socket type \"" + socketType2Str(sockType) + "\" is not supported.";
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultOptionNotSupported);
                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                }
            } // for( idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
        } // if( protType == EProtocolTypeZSXML || protType == EProtocolTypeZSMsg )

        else // if( protType != EProtocolTypeZSXML && protType != EProtocolTypeZSMsg )
        {
            strAddErrInfo = "Protocol type \"" + protocolType2Str(protType) + "\" is not supported.";
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultOptionNotSupported);
            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        } // if( protType != EProtocolTypeZSXML && protType != EProtocolTypeZSMsg )
    } // for( idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )

} // createGatewayThreads

//------------------------------------------------------------------------------
void CDb::destroyGatewayThreads()
//------------------------------------------------------------------------------
{
    // Not necessary as method only called internally if mutex is already locked:
    //QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "destroyGatewayThreads",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".destroyGatewayThreads";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    QHash<SCnctType,CDbGatewayThread*>::iterator itThreads;

    SCnctType         cnctType;
    CDbGatewayThread* pGatewayThread;

    for( itThreads = m_hshpGatewayThreads.begin(); itThreads != m_hshpGatewayThreads.end(); itThreads++ )
    {
        cnctType       = itThreads.key();
        pGatewayThread = itThreads.value();

        if( pGatewayThread->isFinished() )
        {
            try
            {
                delete pGatewayThread;
            }
            catch(...)
            {
            }
        }
        else
        {
            pGatewayThread->deleteLater();
        }

        pGatewayThread = nullptr;

        m_hshpGatewayThreads[cnctType] = nullptr;
        m_hshpGateways[cnctType] = nullptr;

    } // for( itThreads = m_hshpGatewayThreads.begin(); itThreads != m_hshpGatewayThreads.end(); itThreads++ )

    m_hshpGatewayThreads.clear();
    m_hshpGateways.clear();

} // destroyGatewayThreads

/*==============================================================================
protected: // overridables (auxiliary methods)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::startGatewayThreads( int i_iTimeout_ms, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startGatewayThreads",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".startGatewayThreads";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    CRequest* pReqStartThreads = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestStartGatewayThreads,
        /* strRequest   */ Request2Str(ERequestStartGatewayThreads),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ true );

    m_pRequestQueue->addRequestInProgress(pReqStartThreads);

    //int iProgress_perCent = 100;

    if( m_hshpGatewayThreads.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "No gateway threads created." );

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else // if( !m_hshpGatewayThreads.isEmpty() )
    {
        SErrResultInfo errResultInfoTmp = ErrResultInfoSuccess(strMth);

        QHash<SCnctType,CDbGatewayThread*>::iterator itThreads;

        SCnctType         cnctType;
        CDbGatewayThread* pGatewayThread;
        CDbGateway*       pGateway;
        CRequest*         pReqStartThread;

        for( itThreads = m_hshpGatewayThreads.begin(); itThreads != m_hshpGatewayThreads.end(); itThreads++ )
        {
            cnctType       = itThreads.key();
            pGatewayThread = itThreads.value();

            pReqStartThread = m_pRequestQueue->createRequest(
                /* strNameSpace */ nameSpace(),
                /* strClassName */ className(),
                /* iRequest     */ ERequestStartGatewayThread,
                /* strRequest   */ Request2Str(ERequestStartGatewayThread),
                /* iReqIdParent */ pReqStartThreads->getId(),
                /* iTimeout_ms  */ i_iTimeout_ms,
                /* bIsBlocking  */ true );

            m_pRequestQueue->addRequestInProgress(pReqStartThread);

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ GatewayThread.start()" );
            }

            pGatewayThread->start( pReqStartThread->getId() );

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- GatewayThread.start()" );
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqStartThread.wait()" );
            }

            if( pReqStartThread->wait() )
            {
                errResultInfoTmp = ErrResultSuccess;

                pGateway = pGatewayThread->getGateway();
                m_hshpGateways[cnctType] = pGateway;

                if( pGateway != nullptr )
                {
                    for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
                    {
                        QObject* pObj = m_arpTrcMsgLogObjects[idxObj];

                        pGateway->addTrcMsgLogObject(pObj);
                    }
                }
            }
            else // if( !pReqStartThread->wait() )
            {
                errResultInfoTmp.setSeverity(EResultSeverityError);
                errResultInfoTmp.setResult(EResultTimeout);
                errResultInfoTmp.setAddErrInfoDscr( "Waiting for StartThread timed out." );
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- ReqStartThread.wait(): " + errResultInfoTmp.getAddErrInfoDscr() );
            }

            pReqStartThread->setErrResultInfo(errResultInfoTmp);
            pReqStartThread->setProgressInPerCent(100);
            pReqStartThread->update();

            // Please note that StartGatewayThread is an internal blocking request which may not become asynchronous.
            m_pRequestQueue->removeRequestInProgress( pReqStartThread->getId() );

            // The one who calls update for the finished request must delete the request.
            deleteRequest(pReqStartThread);
            pReqStartThread = nullptr;

            if( errResultInfoTmp.isErrorResult() && CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfoTmp);
            }
        } // for( itThreads = m_hshpGatewayThreads.begin(); ...
    } // if( !m_hshpGatewayThreads.isEmpty() )

    pReqStartThreads->setErrResultInfo(errResultInfo);
    pReqStartThreads->setProgressInPerCent(100);
    pReqStartThreads->update();

    // Please note that StartThreads is an internal blocking request which may not become asynchronous.
    m_pRequestQueue->removeRequestInProgress( pReqStartThreads->getId() );

    // The one who calls update for the finished request must delete the request.
    deleteRequest(pReqStartThreads);
    pReqStartThreads = nullptr;

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = QString(pReqStartThreads == nullptr ? "Success" : pReqStartThreads->getAddTrcInfoStr(iAddTrcInfoDetailLevel));
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqStartThreads; // = nullptr

} // startGatewayThreads

//------------------------------------------------------------------------------
CRequest* CDb::stopGatewayThreads( int i_iTimeout_ms, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "stopGatewayThreads",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".stopGatewayThreads";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    CRequest* pReqStopThreads = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestStopGatewayThreads,
        /* strRequest   */ Request2Str(ERequestStopGatewayThreads),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ true );

    m_pRequestQueue->addRequestInProgress(pReqStopThreads);

    //int iProgress_perCent = 100;

    if( m_hshpGatewayThreads.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "No gateway threads created." );

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else // if( !m_hshpGatewayThreads.isEmpty() )
    {
        SErrResultInfo errResultInfoTmp = ErrResultInfoSuccess(strMth);

        QHash<SCnctType,CDbGatewayThread*>::iterator itThreads;

        SCnctType         cnctType;
        CDbGatewayThread* pGatewayThread;
        CRequest*         pReqStopThread;

        for( itThreads = m_hshpGatewayThreads.begin(); itThreads != m_hshpGatewayThreads.end(); itThreads++ )
        {
            cnctType       = itThreads.key();
            pGatewayThread = itThreads.value();

            pReqStopThread = m_pRequestQueue->createRequest(
                /* strNameSpace */ nameSpace(),
                /* strClassName */ className(),
                /* iRequest     */ ERequestStopGatewayThread,
                /* strRequest   */ Request2Str(ERequestStopGatewayThread),
                /* iReqIdParent */ pReqStopThreads->getId(),
                /* iTimeout_ms  */ i_iTimeout_ms,
                /* bIsBlocking  */ true );

            m_pRequestQueue->addRequestInProgress(pReqStopThread);

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ GatewayThread.quit()" );
            }

            pGatewayThread->quit();

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- GatewayThread.quit()" );
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqStopThread.wait()" );
            }

            if( pGatewayThread->wait(i_iTimeout_ms <= 0 ? ULONG_MAX : static_cast<unsigned long>(i_iTimeout_ms)) )
            {
                errResultInfoTmp = ErrResultSuccess;
            }
            else
            {
                errResultInfoTmp.setSeverity(EResultSeverityError);
                errResultInfoTmp.setResult(EResultTimeout);
                errResultInfoTmp.setAddErrInfoDscr( "Waiting for StopThread timed out." );
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- ReqStopThread.wait(): " + errResultInfoTmp.getAddErrInfoDscr() );
            }

            pReqStopThread->setErrResultInfo(errResultInfoTmp);
            pReqStopThread->setProgressInPerCent(100);
            pReqStopThread->update();

            // Please note that StartGatewayThread is an internal blocking request which may not become asynchronous.
            m_pRequestQueue->removeRequestInProgress( pReqStopThread->getId() );

            // The one who calls update for the finished request must delete the request.
            deleteRequest(pReqStopThread);
            pReqStopThread = nullptr;

            if( errResultInfoTmp.isErrorResult() && CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfoTmp);
            }
        } // for( itThreads = m_hshpGatewayThreads.begin(); ...

    } // if( !m_hshpGatewayThreads.isEmpty() )

    pReqStopThreads->setErrResultInfo(errResultInfo);
    pReqStopThreads->setProgressInPerCent(100);
    pReqStopThreads->update();

    // Please note that StopThread is an internal blocking request which may not become asynchronous.
    m_pRequestQueue->removeRequestInProgress( pReqStopThreads->getId() );

    // The one who calls update for the finished request must delete the request.
    deleteRequest(pReqStopThreads);
    pReqStopThreads = nullptr;

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = QString(pReqStopThreads == nullptr ? "Success" : pReqStopThreads->getAddTrcInfoStr(iAddTrcInfoDetailLevel));
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqStopThreads; // = nullptr

} // stopGatewayThreads

/*==============================================================================
protected: // overridables (auxiliary methods)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::startupGateways( int i_iTimeout_ms, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startupGateways",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".startupGateways";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    CRequest* pReqStartupGateways = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestStartupGateways,
        /* strRequest   */ Request2Str(ERequestStartupGateways),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ true );

    m_pRequestQueue->addRequestInProgress(pReqStartupGateways);

    int iProgress_perCent = 100;

    if( m_hshpGateways.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "No gateways created." );

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else // if( !m_hshpGateways.isEmpty() )
    {
        SErrResultInfo errResultInfoTmp = ErrResultInfoSuccess(strMth);

        QHash<SCnctType,CDbGateway*>::iterator itGtws;

        SCnctType           cnctType;
        CDbGateway*         pGateway;
        CRequest*           pReqStartupGateway;
        CMsgReqStartup*     pMsgReq;
        SServerHostSettings hostSettings;

        for( itGtws = m_hshpGateways.begin(); itGtws != m_hshpGateways.end(); itGtws++ )
        {
            cnctType = itGtws.key();
            pGateway = itGtws.value();

            pReqStartupGateway = m_pRequestQueue->createRequest(
                /* strNameSpace */ nameSpace(),
                /* strClassName */ className(),
                /* iRequest     */ ERequestStartupGateway,
                /* strRequest   */ Request2Str(ERequestStartupGateway),
                /* iReqIdParent */ pReqStartupGateways->getId(),
                /* iTimeout_ms  */ i_iTimeout_ms,
                /* bIsBlocking  */ true );

            m_pRequestQueue->addRequestInProgress(pReqStartupGateway);

            hostSettings = getHostSettings(cnctType.m_protocolType, cnctType.m_socketType);

            pMsgReq = new CMsgReqStartup(
                /* pObjSender       */ this,
                /* pObjReceiver     */ pGateway,
                /* protocolType     */ cnctType.m_protocolType,
                /* hostSettings     */ hostSettings,
                /* bMustBeConfirmed */ true,
                /* iReqId           */ pReqStartupGateway->getId() );

            pMsgReq->setTimeoutInMs(i_iTimeout_ms);
            pMsgReq->setIsBlockingRequest(true);

            POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqStartupGateway.wait()" );
            }

            if( pReqStartupGateway->wait() )
            {
                CMsgConStartup* pMsgCon = dynamic_cast<CMsgConStartup*>(pReqStartupGateway->takeExecutionConfirmationMessage());

                if( pMsgCon != nullptr )
                {
                    errResultInfoTmp = pMsgCon->getErrResultInfo();

                    pReqStartupGateway->setErrResultInfo(errResultInfoTmp);
                    pReqStartupGateway->setProgressInPerCent(100);
                    pReqStartupGateway->update();

                    delete pMsgCon;
                    pMsgCon = nullptr;
                }
                else
                {
                    errResultInfoTmp = pReqStartupGateway->getErrResultInfo();
                }
            }
            else // if( !pReqStartupGateway->wait() )
            {
                errResultInfoTmp.setSeverity(EResultSeverityError);
                errResultInfoTmp.setResult(EResultTimeout);
                errResultInfoTmp.setAddErrInfoDscr( "Waiting for startup confirmation timed out." );

                pReqStartupGateway->setErrResultInfo(errResultInfoTmp);
                pReqStartupGateway->setProgressInPerCent(100);
                pReqStartupGateway->update();
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- ReqStartupGateway.wait(): " + errResultInfoTmp.getAddErrInfoDscr() );
            }

            // Please note that StartupGateway is an internal blocking request which may not become asynchronous.
            m_pRequestQueue->removeRequestInProgress( pReqStartupGateway->getId() );

            // The one who calls update for the finished request must delete the request.
            deleteRequest(pReqStartupGateway);
            pReqStartupGateway = nullptr;

            if( errResultInfoTmp.isErrorResult() && CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfoTmp);
            }
        } // for( itGtws = m_hshpGateways.begin(); itGtws != m_hshpGateways.end(); itGtws++ )
    } // if( !m_hshpGateways.isEmpty() )

    pReqStartupGateways->setErrResultInfo(errResultInfo);
    pReqStartupGateways->setProgressInPerCent(iProgress_perCent);
    pReqStartupGateways->update();

    // Please note that StartupGateways is an internal blocking request which may not become asynchronous.
    m_pRequestQueue->removeRequestInProgress( pReqStartupGateways->getId() );

    // The one who calls update for the finished request must delete the request.
    deleteRequest(pReqStartupGateways);
    pReqStartupGateways = nullptr;

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReqStartupGateways == nullptr ? "Success" : pReqStartupGateways->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqStartupGateways; // = nullptr

} // startupGateways

//------------------------------------------------------------------------------
CRequest* CDb::shutdownGateways( int i_iTimeout_ms, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "shutdownGateways",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".shutdownGateways";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    CRequest* pReqShutdownGateways = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestShutdownGateways,
        /* strRequest   */ Request2Str(ERequestShutdownGateways),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ true );

    m_pRequestQueue->addRequestInProgress(pReqShutdownGateways);

    int iProgress_perCent = 100;

    if( m_hshpGateways.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "No gateways created." );

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else // if( !m_hshpGateways.isEmpty() )
    {
        SErrResultInfo errResultInfoTmp = ErrResultInfoSuccess(strMth);

        QHash<SCnctType,CDbGateway*>::iterator itGtws;

        SCnctType        cnctType;
        CDbGateway*      pGateway;
        CRequest*        pReqShutdownGateway;
        CMsgReqShutdown* pMsgReq;

        for( itGtws = m_hshpGateways.begin(); itGtws != m_hshpGateways.end(); itGtws++ )
        {
            cnctType = itGtws.key();
            pGateway = itGtws.value();

            pReqShutdownGateway = m_pRequestQueue->createRequest(
                /* strNameSpace */ nameSpace(),
                /* strClassName */ className(),
                /* iRequest     */ ERequestShutdownGateway,
                /* strRequest   */ Request2Str(ERequestShutdownGateway),
                /* iReqIdParent */ pReqShutdownGateways->getId(),
                /* iTimeout_ms  */ i_iTimeout_ms,
                /* bIsBlocking  */ true );

            m_pRequestQueue->addRequestInProgress(pReqShutdownGateway);

            pMsgReq = new CMsgReqShutdown(
                /* pObjSender       */ this,
                /* pObjReceiver     */ pGateway,
                /* bMustBeConfirmed */ true,
                /* iReqId           */ pReqShutdownGateway->getId() );

            pMsgReq->setTimeoutInMs(i_iTimeout_ms);
            pMsgReq->setIsBlockingRequest(true);

            POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqShutdownGateway.wait()" );
            }

            if( pReqShutdownGateway->wait() )
            {
                CMsgConShutdown* pMsgCon = dynamic_cast<CMsgConShutdown*>(pReqShutdownGateway->takeExecutionConfirmationMessage());

                if( pMsgCon != nullptr )
                {
                    errResultInfoTmp = pMsgCon->getErrResultInfo();

                    pReqShutdownGateway->setErrResultInfo(errResultInfoTmp);
                    pReqShutdownGateway->setProgressInPerCent(100);
                    pReqShutdownGateway->update();

                    delete pMsgCon;
                    pMsgCon = nullptr;
                }
                else // if( pMsgCon == nullptr )
                {
                    errResultInfoTmp = pReqShutdownGateway->getErrResultInfo();
                }
            }
            else // if( !pReqShutdownGateway->wait() )
            {
                errResultInfoTmp.setSeverity(EResultSeverityError);
                errResultInfoTmp.setResult(EResultTimeout);
                errResultInfoTmp.setAddErrInfoDscr( "Waiting for shutdown confirmation timed out." );

                pReqShutdownGateway->setErrResultInfo(errResultInfoTmp);
                pReqShutdownGateway->setProgressInPerCent(100);
                pReqShutdownGateway->update();
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- ReqShutdownGateway.wait(): " + errResultInfoTmp.getAddErrInfoDscr() );
            }

            // Please note that ShutdownGateway is an internal blocking request which may not become asynchronous.
            m_pRequestQueue->removeRequestInProgress( pReqShutdownGateway->getId() );

            // The one who calls update for the finished request must delete the request.
            deleteRequest(pReqShutdownGateway);
            pReqShutdownGateway = nullptr;

            if( errResultInfoTmp.isErrorResult() && CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfoTmp);
            }
        } // for( itGtws = m_hshpGateways.begin(); itGtws != m_hshpGateways.end(); itGtws++ )
    } // if( !m_hshpGateways.isEmpty() )

    pReqShutdownGateways->setErrResultInfo(errResultInfo);
    pReqShutdownGateways->setProgressInPerCent(iProgress_perCent);
    pReqShutdownGateways->update();

    // Please note that ShutdownGateways is an internal blocking request which may not become asynchronous.
    m_pRequestQueue->removeRequestInProgress( pReqShutdownGateways->getId() );

    // The one who calls update for the finished request must delete the request.
    deleteRequest(pReqShutdownGateways);
    pReqShutdownGateways = nullptr;

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReqShutdownGateways == nullptr ? "Success" : pReqShutdownGateways->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqShutdownGateways; // = nullptr

} // shutdownGateways

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::acceptChangedSettings()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "acceptChangedSettings",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + state2Str();
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QHash<SCnctType,SServerHostSettings>::iterator itHostSettings;

    SCnctType           cnctType;
    SServerHostSettings hostSettings;
    QList<SCnctType>    arCnctsAccepted;
    int                 idxCnct;

    for( itHostSettings = m_hshHostSettingsCurr.begin(); itHostSettings != m_hshHostSettingsCurr.end(); itHostSettings++ )
    {
        cnctType = itHostSettings.key();
        hostSettings = itHostSettings.value();

        m_hshHostSettingsOrig[cnctType] = hostSettings;
        arCnctsAccepted << cnctType;
    }

    m_hshHostSettingsCurr.clear();

    for( idxCnct = 0; idxCnct < arCnctsAccepted.size(); idxCnct++ )
    {
        cnctType = arCnctsAccepted[idxCnct];

        emit hostSettingsRowVersionChanged(this, cnctType.m_protocolType, cnctType.m_socketType);
    }

    arCnctsAccepted.clear();

} // acceptChangedSettings

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::onRequestTimeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRequestTimeout",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.trace(strAddTrcInfo);
    }

    m_pTmrReqTimeout->stop();

    CRequest* pReq = m_pRequestQueue->getRequestInProgress(m_reqDscrTimeout.m_iId);

    if( pReq != nullptr )
    {
        SErrResultInfo errResultInfo = ErrResultInfoSuccess("onRequestTimeout");
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultTimeout);
        errResultInfo.setAddErrInfoDscr(pReq->getRequestName());
        m_pRequestQueue->lock();
        pReq->lock();
        pReq->setErrResultInfo(errResultInfo);
        pReq->setProgressInPerCent(100);
        pReq->update();
        pReq->unlock();
        m_pRequestQueue->unlock();

        // The one who calls update for the finished request must delete the request.
        deleteRequest(pReq); // The "onRequestChanged" slot will be called again. The request has already been removed from the queue.
        pReq = nullptr;
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "ReqQueue { " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        mthTracer.trace(strAddTrcInfo);
    }

} // onRequestTimeout

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::onRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + state2Str();
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".onRequestChanged";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QString strAddErrInfo;

    bool bIsPostponedRequest = false;
    int  idxPostponedRequest = -1;

    CRequest* pReqChanged = m_pRequestQueue->findRequestById(
        /* iRequestId               */ i_reqDscr.m_iId,
        /* bIgnorePostponedRequests */ false,
        /* pbIsPostponedRequest     */ &bIsPostponedRequest,
        /* pidxPostponedRequest     */ &idxPostponedRequest );

    if( pReqChanged == nullptr )
    {
        strAddErrInfo = "Slot method called for request " + i_reqDscr.getAddTrcInfoStr() + " but request is not in request queue.";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidMethodCall);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    } // if( pReqChanged == nullptr )

    CRequest* pReqMainInProgress  = m_pRequestQueue->getRequestInProgress();

    if( pReqMainInProgress == nullptr )
    {
        strAddErrInfo = "Slot method called for request " + i_reqDscr.getAddTrcInfoStr() + " while no request is in progress.";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidMethodCall);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    } // if( pReqMainInProgress == nullptr )

    if( !errResultInfo.isErrorResult() )
    {
        SRequestDscr reqDscrMainRequest = pReqMainInProgress->getDescriptor();
        bool         bIsMainRequest     = false;
        EState       statePrev          = m_state;
        SRequestDscr reqDscrPrev;

        if( pReqChanged == pReqMainInProgress )
        {
            bIsMainRequest = true;
            reqDscrPrev = i_reqDscr;
        }

        // Please note that this slot method may also be called for postponed requests
        // if the parent of those requests is e.g. aborted.
        if( !bIsPostponedRequest )
        {
            emit requestInProgressChanged(this, i_reqDscr);
        }

        m_pRequestQueue->lock();

        // Please note that not all error results should end up in the error log file.
        // This depends which request could not have been successfully processed.

        //qint64 iReqIdMainParent = pReqMainInProgress->getParentId();

        //bool bMainRequestIsWaitingRequest = false;

        //if( iReqIdMainParent >= 0 )
        //{
        //    bMainRequestIsWaitingRequest = CRequestExecTree::GetInstance()->isWaiting(iReqIdMainParent);
        //}

        // If the changed request has been finished ...
        if( i_reqDscr.m_iProgress_perCent >= 100 )
        {
            // Please note that the "onRequestChanged" slot is also connected
            // to the changed signal of postponed requests.
            if( bIsPostponedRequest )
            {
                m_pRequestQueue->removePostponedRequest(pReqChanged);
            }
            else // if( !bIsPostponedRequest )
            {
                if( m_pTmrReqTimeout->isActive() )
                {
                    m_pTmrReqTimeout->stop();
                }
            } // if( !bIsPostponedRequest )

            m_pRequestQueue->removeRequestInProgress(i_reqDscr.m_iId);

            // On further changes to the request this slot should not be called anymore.
            QObject::disconnect(
                /* pObjSender   */ pReqChanged,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        } // if( i_reqDscr.m_iProgress_perCent >= 100 )

        // Handle the changed request
        //---------------------------

        // The database does not trigger requests of "foreign" classes.
        // So the following string compare is unnecessary as this slot may only
        // be called if the server itself updates the request.
        //if( i_reqDscr.m_strCreatorClassName.compare(className(),Qt::CaseInsensitive) == 0 )

        if( !bIsPostponedRequest )
        {
            // Confirmation messages received from the gateway.
            //CMsgCon* pMsgConExec = dynamic_cast<CMsgCon*>(pReqChanged->getExecutionConfirmationMessage());

            switch( i_reqDscr.m_request )
            {
                case ERequestStartup:
                {
                    // If the request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        if( i_reqDscr.m_errResultInfo.isErrorResult() )
                        {
                            // stopGatewayThreads is a blocking request method.
                            stopGatewayThreads();

                            m_state = EStateIdle;
                        }
                        else // if( !i_reqDscr.m_errResultInfo.isErrorResult() )
                        {
                            m_state = EStateListening;
                        }
                    } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                    break;
                }
                case ERequestShutdown:
                {
                    // If the request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        m_state = EStateIdle;
                    }
                    break;
                }
                case ERequestUpdateHostSettings:
                {
                    break;
                }

                // Internal blocking requests
                case ERequestStartGatewayThreads:
                case ERequestStopGatewayThreads:
                case ERequestStartGatewayThread:
                case ERequestStopGatewayThread:
                case ERequestStartupGateways:
                case ERequestShutdownGateways:
                case ERequestStartupGateway:
                case ERequestShutdownGateway:
                {
                    strAddErrInfo = "Slot method called for blocking request " + i_reqDscr.getAddTrcInfoStr();
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInvalidMethodCall);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);

                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                    break;
                }
                default:
                {
                    strAddErrInfo = "Slot method called for unknown request " + i_reqDscr.getAddTrcInfoStr();
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInvalidMethodCall);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);

                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                    break;
                }
            } // switch( i_reqDscr.m_request )
        } // if( !bIsPostponedRequest )

        // If the main request has been finished ...
        if( i_reqDscr.m_iProgress_perCent >= 100 && bIsMainRequest )
        {
            if( !pReqChanged->isLocked() )
            {
                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReqChanged);
            }
        } // if( i_reqDscr.m_iProgress_perCent >= 100 && bIsMainRequest )

        SRequestDscr reqDscrCurr;

        CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();

        if( pReqInProgress != nullptr )
        {
            reqDscrCurr = pReqInProgress->getDescriptor();
        }

        if( reqDscrPrev != reqDscrCurr )
        {
            if( reqDscrCurr.m_objState == EObjState::Created )
            {
                emit requestInProgressChanged(this, reqDscrCurr);
            }
        }

        if( statePrev != m_state )
        {
            emit stateChanged(this, m_state);
        }

        m_pRequestQueue->unlock();

        if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
        {
            executeNextPostponedRequest();
        }
    } // if( !errResultInfo.isErrorResult() )

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // onRequestChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::onDSDestroyed( QObject* /*i_pDS*/ )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDSDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    m_pDS = nullptr;

} // onDSDestroyed

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CDb::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pMsg->type() >= QEvent::User )
    {
        QString strAddTrcInfo;
        //QString strAddErrInfo;

        CMsg* pMsg = nullptr;

        try
        {
            pMsg = dynamic_cast<CMsg*>(i_pMsg);
        }
        catch(...)
        {
            pMsg = nullptr;
        }

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
        {
            int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
            strAddTrcInfo = "Msg {" + QString( pMsg == nullptr ? "nullptr" : pMsg->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        if( pMsg != nullptr )
        {
            QMutexLocker mtxLocker(m_pMtx);

            bEventHandled = true;

            SErrResultInfo errResultInfo = ErrResultInfoSuccess(pMsg->msgTypeToStr());

            if( pMsg->isBaseMsgType() )
            {
                if( pMsg->getMsgType() == ZS::System::EBaseMsgTypeIndErr )
                {
                    CMsgErr* pMsgErr = dynamic_cast<CMsgErr*>(pMsg);
                    if( pMsgErr == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultInvalidMessageContent,strAddTrcInfo);
                    }

                    errResultInfo = pMsgErr->getErrResultInfo();

                    if( CErrLog::GetInstance() != nullptr && !errResultInfo.isAddedToErrLogModel() )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                        errResultInfo.setAddedToErrLogModel();
                    }
                } // if( pMsg->getMsgType() == ZS::System::EBaseMsgTypeIndErr )

                else if( pMsg->getMsgType() == EBaseMsgTypeReqContinue )
                {
                    m_bMsgReqContinuePending = false;

                    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
                    {
                        CRequest* pReqPostponed = m_pRequestQueue->takeFirstPostponedRequest();

                        if( pReqPostponed != nullptr )
                        {
                            switch( pReqPostponed->getRequest() )
                            {
                                case ERequestStartup:
                                {
                                    executeStartupRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestShutdown:
                                {
                                    executeShutdownRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestUpdateHostSettings:
                                {
                                    executeUpdateHostSettingsRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                            } // switch( pReqPostponed->getRequest() )
                        } // if( pReqPostponed != nullptr )

                        delete pReqPostponed;
                        pReqPostponed = nullptr;

                    } // if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
                } // if( pMsg->getMsgType() == EBaseMsgTypeReqContinue )
            } // if( pMsg->isBaseMsgType() )

            else // if( !pMsg->isBaseMsgType() )
            {
                if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                {
                    int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
                    strAddTrcInfo  = "State: " + State2Str(m_state);
                    strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
                    mthTracer.trace(strAddTrcInfo);
                }

                CRequest* pReq;

                EState statePrev = m_state;

                if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )
                {
                    switch( static_cast<int>(pMsg->type()) )
                    {
                        case EMsgTypeReqConnect:
                        {
                            ZS::Data::CMsgReqConnect* pMsgReq = dynamic_cast<ZS::Data::CMsgReqConnect*>(pMsg);
                            if( pMsgReq == nullptr )
                            {
                                throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                            }

                            CMsgConConnect* pMsgCon = nullptr;

                            if( pMsgReq->mustBeConfirmed() )
                            {
                                pMsgCon = dynamic_cast<CMsgConConnect*>(pMsgReq->createConfirmationMessage(ECopyDepth::Deep));
                            }

                            EProtocolType       protocolType    = pMsgReq->getProtocolType();
                            SClientHostSettings cltHostSettings = pMsgReq->getHostSettings();
                            SSocketDscr         socketDscr      = cltHostSettings.getSocketDscr();

                            socketDscr.m_pObjRemote        = pMsgReq->getSender();
                            socketDscr.m_strRemoteHostName = pMsgReq->getSender()->objectName();
                            socketDscr.m_pObjLocal         = this;
                            socketDscr.m_strLocalHostName  = objectName();

                            // Please note that the server could have been shutdown in the meantime .
                            if( m_state != EStateListening || getRequestInProgress() == ERequestShutdown )
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultRequestRefused);
                                errResultInfo.setAddErrInfoDscr("Server is not listening");
                            }
                            else // if( m_state == EStateListening && getRequestInProgress() != ERequestShutdown )
                            {
                                // Hash of clients connected via sockets at the corresponding gateway.
                                // Key is built of protocol type, connection type and client object address.
                                SCnctId cnctId( protocolType, socketDscr.m_socketType, socketDscr.m_pObjRemote );

                                if( !m_arProtocolTypes.contains(cnctId.m_protocolType) )
                                {
                                    errResultInfo.setSeverity(EResultSeverityError);
                                    errResultInfo.setResult(EResultOptionNotSupported);
                                    errResultInfo.setAddErrInfoDscr( "Protocol " + protocolType2Str(cnctId.m_protocolType) + " not supported." );
                                }
                                else if( !m_hsharSocketTypes[cnctId.m_protocolType].contains(cnctId.m_socketType) )
                                {
                                    errResultInfo.setSeverity(EResultSeverityError);
                                    errResultInfo.setResult(EResultOptionNotSupported);
                                    errResultInfo.setAddErrInfoDscr( "Protocol " + protocolType2Str(cnctId.m_protocolType) + " in combination with socket type " + socketType2Str(cnctId.m_socketType) + " not supported." );
                                }
                                else if( m_hshDbCltsConnected.contains(cnctId) )
                                {
                                    errResultInfo.setSeverity(EResultSeverityError);
                                    errResultInfo.setResult(EResultSocketIdAlreadyUsed);
                                    errResultInfo.setAddErrInfoDscr( "Database client '" + socketDscr.getConnectionString() + "' is already connected." );
                                }
                                else
                                {
                                    socketDscr.m_socketState = ESocketStateConnected;

                                    m_hshDbCltsConnected[cnctId] = socketDscr;

                                    if( errResultInfo.getResult() == EResultUndefined )
                                    {
                                        errResultInfo.setResult(EResultSuccess);
                                    }
                                    emit connected(this, socketDscr);
                                }
                            } // if( m_state == EStateListening && getRequestInProgress() != ERequestShutdown )

                            if( pMsgCon != nullptr )
                            {
                                pMsgCon->setSocketDscr(socketDscr);
                                pMsgCon->setErrResultInfo(errResultInfo);
                                pMsgCon->setProgress(100);
                                POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer);
                                pMsgCon = nullptr;
                            }
                            break;
                        } // case EMsgTypeReqConnect

                        case EMsgTypeReqDisconnect:
                        {
                            ZS::Data::CMsgReqDisconnect* pMsgReq = dynamic_cast<ZS::Data::CMsgReqDisconnect*>(pMsg);
                            if( pMsgReq == nullptr )
                            {
                                throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                            }

                            CMsgConDisconnect* pMsgCon = nullptr;

                            if( pMsgReq->mustBeConfirmed() )
                            {
                                pMsgCon = dynamic_cast<CMsgConDisconnect*>(pMsgReq->createConfirmationMessage(ECopyDepth::Deep));
                            }

                            EProtocolType protocolType = pMsgReq->getProtocolType();
                            SSocketDscr   socketDscr   = pMsgReq->getSocketDscr();

                            socketDscr.m_pObjRemote        = pMsgReq->getSender();
                            socketDscr.m_strRemoteHostName = pMsgReq->getSender()->objectName();
                            socketDscr.m_pObjLocal         = this;
                            socketDscr.m_strLocalHostName  = objectName();

                            if( m_state != EStateListening || getRequestInProgress() == ERequestShutdown )
                            {
                                errResultInfo.setSeverity(EResultSeverityError);
                                errResultInfo.setResult(EResultRequestRefused);
                                errResultInfo.setAddErrInfoDscr("Server is not listening");
                            }
                            else // if( m_state == EStateListening && getRequestInProgress() != ERequestShutdown )
                            {
                                // Hash of clients connected via sockets at the corresponding gateway.
                                // Key is built of protocol type, connection type and client object address.
                                SCnctId cnctId( protocolType, socketDscr.m_socketType, socketDscr.m_pObjRemote );

                                if( errResultInfo.getResult() != EResultSuccess )
                                {
                                    if( CErrLog::GetInstance() != nullptr )
                                    {
                                        CErrLog::GetInstance()->addEntry(errResultInfo);
                                    }
                                }
                                else
                                {
                                    if( !m_arProtocolTypes.contains(cnctId.m_protocolType) )
                                    {
                                        errResultInfo.setSeverity(EResultSeverityError);
                                        errResultInfo.setResult(EResultOptionNotSupported);
                                        errResultInfo.setAddErrInfoDscr( "Protocol " + protocolType2Str(cnctId.m_protocolType) + " not supported." );
                                    }
                                    else if( !m_hsharSocketTypes[cnctId.m_protocolType].contains(cnctId.m_socketType) )
                                    {
                                        errResultInfo.setSeverity(EResultSeverityError);
                                        errResultInfo.setResult(EResultOptionNotSupported);
                                        errResultInfo.setAddErrInfoDscr( "Protocol " + protocolType2Str(cnctId.m_protocolType) + " in combination with socket type " + socketType2Str(cnctId.m_socketType) + " not supported." );
                                    }
                                    else if( !m_hshDbCltsConnected.contains(cnctId) )
                                    {
                                        errResultInfo.setSeverity(EResultSeverityError);
                                        errResultInfo.setResult(EResultSocketIdAlreadyUsed);
                                        errResultInfo.setAddErrInfoDscr( "Database client '" + socketDscr.getConnectionString() + "' is not connected." );
                                    }
                                    else
                                    {
                                        m_hshDbCltsConnected.remove(cnctId);
                                    }
                                    socketDscr.m_socketState = ESocketStateUnconnected;

                                    emit disconnected(this, socketDscr);
                                }
                            } // if( m_state == EStateListening && getRequestInProgress() != ERequestShutdown )

                            if( pMsgCon != nullptr )
                            {
                                pMsgCon->setErrResultInfo(errResultInfo);
                                pMsgCon->setProgress(100);
                                POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer);
                                pMsgCon = nullptr;
                            }
                            break;
                        } // case EMsgTypeReqDisconnect

                        default:
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultInvalidMessage);
                            errResultInfo.setAddErrInfoDscr( pMsg->msgTypeToStr() );

                            if( CErrLog::GetInstance() != nullptr )
                            {
                                CErrLog::GetInstance()->addEntry(errResultInfo);
                            }
                            else
                            {
                                throw ZS::System::CException( __FILE__, __LINE__, errResultInfo );
                            }
                            break;
                        }
                    } // switch( pMsg->type() )
                } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )

                else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeInd )
                {
                    //----------------------------------------------------------
                    // Indication messages sent by the gateway to this server.
                    //----------------------------------------------------------

                    // Please note that messages may be sent by the gateway right before the gateway
                    // is destroyed. In this case the message may be received right after the gateway
                    // has been destroyed, we can't access the message sender (the gateway) anymore
                    // and also a dynamic_cast from the message sender to the gateway object will
                    // result in an access violation exception.

                    switch( static_cast<int>(pMsg->type()) )
                    {
                        case EMsgTypeIndConnected:
                        {
                            ZS::Data::CMsgIndConnected* pMsgInd = dynamic_cast<ZS::Data::CMsgIndConnected*>(pMsg);
                            if( pMsgInd == nullptr )
                            {
                                throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgIndConnected == nullptr! " + strAddTrcInfo );
                            }

                            // Please note that the server could have been shutdown in the meantime .
                            if( m_state == EStateListening && getRequestInProgress() != ERequestShutdown )
                            {
                                // Hash of clients connected via sockets at the corresponding gateway.
                                // Key is built of protocol type, connection type and socket id.
                                EProtocolType protocolType = pMsgInd->getProtocolType();
                                SSocketDscr   socketDscr   = pMsgInd->getSocketDscr();

                                SCnctId cnctId( protocolType, socketDscr.m_socketType, socketDscr.m_iSocketId );

                                if( !m_arProtocolTypes.contains(cnctId.m_protocolType) )
                                {
                                    errResultInfo.setSeverity(EResultSeverityError);
                                    errResultInfo.setResult(EResultOptionNotSupported);
                                    errResultInfo.setAddErrInfoDscr( "Protocol " + protocolType2Str(cnctId.m_protocolType) + " not supported." );
                                }
                                else if( !m_hsharSocketTypes[cnctId.m_protocolType].contains(cnctId.m_socketType) )
                                {
                                    errResultInfo.setSeverity(EResultSeverityError);
                                    errResultInfo.setResult(EResultOptionNotSupported);
                                    errResultInfo.setAddErrInfoDscr( "Protocol " + protocolType2Str(cnctId.m_protocolType) + " in combination with socket type " + socketType2Str(cnctId.m_socketType) + " not supported." );
                                }
                                else if( m_hshDbCltsConnected.contains(cnctId) )
                                {
                                    errResultInfo.setSeverity(EResultSeverityError);
                                    errResultInfo.setResult(EResultSocketIdAlreadyUsed);
                                    errResultInfo.setAddErrInfoDscr( "Database client '" + socketDscr.getConnectionString() + "' is already connected." );
                                }
                                else
                                {
                                    m_hshDbCltsConnected[cnctId] = socketDscr;

                                    if( errResultInfo.getResult() == EResultUndefined )
                                    {
                                        errResultInfo.setResult(EResultSuccess);
                                    }
                                    emit connected(this, socketDscr);
                                }
                            } // if( m_state == EStateListening && getRequestInProgress() != ERequestShutdown )
                            break;
                        } // case EMsgTypeIndConnected

                        case EMsgTypeIndDisconnected:
                        {
                            ZS::Data::CMsgIndDisconnected* pMsgInd = dynamic_cast<ZS::Data::CMsgIndDisconnected*>(pMsg);
                            if( pMsgInd == nullptr )
                            {
                                throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgIndDisconnected == nullptr! " + strAddTrcInfo );
                            }

                            // Please note that the server could have been shutdown in the meantime .
                            if( m_state == EStateListening && getRequestInProgress() != ERequestShutdown )
                            {
                                EProtocolType protocolType = pMsgInd->getProtocolType();
                                SSocketDscr   socketDscr   = pMsgInd->getSocketDscr();

                                SCnctId cnctId( protocolType, socketDscr.m_socketType, socketDscr.m_iSocketId );

                                errResultInfo = checkSocketId(protocolType, socketDscr.m_socketType, socketDscr.m_iSocketId);

                                if( errResultInfo.getResult() != EResultSuccess )
                                {
                                    if( CErrLog::GetInstance() != nullptr )
                                    {
                                        CErrLog::GetInstance()->addEntry(errResultInfo);
                                    }
                                }
                                else
                                {
                                    if( !m_arProtocolTypes.contains(cnctId.m_protocolType) )
                                    {
                                        errResultInfo.setSeverity(EResultSeverityError);
                                        errResultInfo.setResult(EResultOptionNotSupported);
                                        errResultInfo.setAddErrInfoDscr( "Protocol " + protocolType2Str(cnctId.m_protocolType) + " not supported." );
                                    }
                                    else if( !m_hsharSocketTypes[cnctId.m_protocolType].contains(cnctId.m_socketType) )
                                    {
                                        errResultInfo.setSeverity(EResultSeverityError);
                                        errResultInfo.setResult(EResultOptionNotSupported);
                                        errResultInfo.setAddErrInfoDscr( "Protocol " + protocolType2Str(cnctId.m_protocolType) + " in combination with socket type " + socketType2Str(cnctId.m_socketType) + " not supported." );
                                    }
                                    else if( !m_hshDbCltsConnected.contains(cnctId) )
                                    {
                                        errResultInfo.setSeverity(EResultSeverityError);
                                        errResultInfo.setResult(EResultSocketIdAlreadyUsed);
                                        errResultInfo.setAddErrInfoDscr( "Database client '" + socketDscr.getConnectionString() + "' is not connected." );
                                    }
                                    else
                                    {
                                        m_hshDbCltsConnected.remove(cnctId);
                                    }
                                    socketDscr.m_socketState = ESocketStateUnconnected;

                                    emit disconnected(this, socketDscr);
                                }
                            } // if( m_state == EStateListening && getRequestInProgress() != ERequestShutdown )
                            break;
                        } // case EMsgTypeIndDisconnected

                        default:
                        {
                            errResultInfo.setSeverity(EResultSeverityError);
                            errResultInfo.setResult(EResultInvalidMessage);
                            errResultInfo.setAddErrInfoDscr( pMsg->msgTypeToStr() );

                            if( CErrLog::GetInstance() != nullptr )
                            {
                                CErrLog::GetInstance()->addEntry(errResultInfo);
                            }
                            else
                            {
                                throw ZS::System::CException( __FILE__, __LINE__, errResultInfo );
                            }
                            break;
                        }
                    } // switch( pMsgInd->type() )
                } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeInd )

                else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeCon )
                {
                    CMsgCon* pMsgCon = dynamic_cast<CMsgCon*>(pMsg);
                    if( pMsgCon == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }

                    pReq = m_pRequestQueue->getRequestInProgress(pMsgCon->getRequestId());

                    if( pReq != nullptr )
                    {
                        SErrResultInfo errResultInfo = pMsgCon->getErrResultInfo();
                        int            iProgress_perCent = pMsgCon->getProgress();

                        m_pRequestQueue->lock();

                        // Please note that the message (QEvent) will be deleted by Qt's event dispatcher.
                        // The confirmation may only be temporarily accessed by receivers of the request
                        // changed signal while handling the slot method.
                        pReq->setExecutionConfirmationMessage(pMsgCon);
                        pReq->setErrResultInfo(errResultInfo);
                        pReq->setProgressInPerCent(iProgress_perCent);
                        pReq->lock();
                        pReq->update(); // "onRequestChanged" is called as a reentry removing the request from the queue.
                        pReq->unlock();
                        pReq->setExecutionConfirmationMessage(nullptr); // Qt will delete the message.

                        if( iProgress_perCent >= 100 )
                        {
                            // The one who calls update for the finished request must delete the request.
                            deleteRequest(pReq); // The "onRequestChanged" slot will be called again. The request has already been removed from the queue.
                            pReq = nullptr;
                        }

                        m_pRequestQueue->unlock();

                        if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
                        {
                            executeNextPostponedRequest();
                        }
                    } // if( pReq != nullptr )
                } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeCon )

                if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
                {
                    int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
                    strAddTrcInfo  = "State: " + State2Str(m_state);
                    strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
                    mthTracer.trace(strAddTrcInfo);
                }

                if( statePrev != m_state )
                {
                    emit stateChanged(this,m_state);
                }
            } // if( !pMsg->isBaseMsgType() )
        } // if( pMsg != nullptr )
    } // if( i_pMsg->type() >= QEvent::User )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;

} // event
