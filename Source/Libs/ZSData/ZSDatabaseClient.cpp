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

//#include <QtCore/qcoreapplication.h>
//#include <QtCore/qmutex.h>
#include <QtCore/qtimer.h>
#include <QtNetwork/qhostinfo.h>
//#include <QtCore/qwaitcondition.h>

#include "ZSData/ZSDatabaseClient.h"
#include "ZSData/ZSDatabaseClientGateway.h"
#include "ZSData/ZSDatabaseClientGatewayThread.h"
#include "ZSData/ZSDatabase.h"
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
using namespace ZS::Data;
using namespace ZS::Trace;


/*******************************************************************************
class CDbClient : public QObject
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrStates[] =
//------------------------------------------------------------------------------
{
    /*  0 */ SEnumEntry( CDbClient::EStateIdle,        "Idle"        ),
    /*  0 */ SEnumEntry( CDbClient::EStateUnconnected, "Unconnected" ),
    /*  1 */ SEnumEntry( CDbClient::EStateConnected,   "Connected"   ),
    /*  2 */ SEnumEntry( CDbClient::EStateRegistered,  "Registered"  ),
    /*    */ SEnumEntry( CDbClient::EStateCount,       "Count"       ),
    /*    */ SEnumEntry( CDbClient::EStateUndefined,   "---"         )
};

//------------------------------------------------------------------------------
QString CDbClient::State2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str(s_arEnumStrStates,_ZSArrLen(s_arEnumStrStates),i_iState);
}

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrRequests[] =
//------------------------------------------------------------------------------
{
    /*  0 */ SEnumEntry( CDbClient::ERequestNone,                    "None"                    ),
    /*  1 */ SEnumEntry( CDbClient::ERequestStartup,                 "Startup"                 ),
    /*  2 */ SEnumEntry( CDbClient::ERequestShutdown,                "Shutdown"                ),
    /*  3 */ SEnumEntry( CDbClient::ERequestConnect,                 "Connect"                 ),
    /*  4 */ SEnumEntry( CDbClient::ERequestDisconnect,              "Disconnect"              ),
    /*  5 */ SEnumEntry( CDbClient::ERequestUpdateHostSettings,      "UpdateHostSettings"      ),
    /*  6 */ SEnumEntry( CDbClient::ERequestRegister,                "Register"                ),
    /*  7 */ SEnumEntry( CDbClient::ERequestUnregister,              "Unregister"              ),
    /*  8 */ SEnumEntry( CDbClient::ERequestSelectSchema,            "SelectSchema"            ),
    /*  9 */ SEnumEntry( CDbClient::ERequestUpdateSchema,            "UpdateSchema"            ),
    /* 10 */ SEnumEntry( CDbClient::ERequestSelectData,              "SelectData"              ),
    /* 11 */ SEnumEntry( CDbClient::ERequestUpdateData,              "UpdateData"              ),
    /* 12 */ SEnumEntry( CDbClient::ERequestStartGatewayThread,      "StartGatewayThread"      ),
    /* 13 */ SEnumEntry( CDbClient::ERequestStopGatewayThread,       "StopGatewayThread"       ),
    /* 14 */ SEnumEntry( CDbClient::ERequestStartupGateway,          "StartupGateway"          ),
    /* 15 */ SEnumEntry( CDbClient::ERequestShutdownGateway,         "ShutdownGateway"         ),
    /* 16 */ SEnumEntry( CDbClient::ERequestConnectGateway,          "ConnectGateway"          ),
    /* 17 */ SEnumEntry( CDbClient::ERequestDisconnectGateway,       "DisconnectGateway"       ),
    /* 18 */ SEnumEntry( CDbClient::ERequestRegisterGateway,         "RegisterGateway"         ),
    /* 19 */ SEnumEntry( CDbClient::ERequestUnregisterGateway,       "UnregisterGateway"       ),
    /*    */ SEnumEntry( CDbClient::ERequestCount,                   "Count"                   ),
    /*    */ SEnumEntry( CDbClient::ERequestUndefined,               "---"                     )
};

//------------------------------------------------------------------------------
QString CDbClient::Request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrRequests, _ZSArrLen(s_arEnumStrRequests), i_iRequest );
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDbClient::CDbClient( const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMtx(nullptr),
    m_pDS(nullptr),
    // Supported protocol and connection types
    m_arProtocolTypes(),
    m_hsharSocketTypes(),
    // Default, current and new data protocol type to be adjusted
    m_protocolTypeDefault(EProtocolTypeUndefined),
    m_protocolTypeOrig(EProtocolTypeZSXML),
    m_protocolTypeCurr(EProtocolTypeUndefined),
    // Default, current and new socket type to be adjusted
    m_socketTypeDefault(ESocketTypeUndefined),
    m_socketTypeOrig(ESocketTypeInProcMsg),
    m_socketTypeCurr(ESocketTypeUndefined),
    // Default, current and new host settings to be adjusted
    m_hshHostSettingsDefault(),
    m_hshHostSettingsOrig(),
    m_hshHostSettingsCurr(),
    // Currently used socket descriptor (only valid if client is connected)
    m_cnctId(),
    m_socketDscr(),
    // Default, current and new data watch dog settings to be adjusted
    m_hshWatchDogSettingsDefault(),
    m_hshWatchDogSettingsOrig(),
    m_hshWatchDogSettingsCurr(),
    // Gateway
    m_pGatewayThread(nullptr),
    m_pGateway(nullptr),
    // State Machine
    m_state(EStateIdle),
    m_pRequestQueue(nullptr),
    m_reqDscrTimeout(),
    m_pTmrReqTimeout(nullptr),
    m_bMsgReqContinuePending(false),
    m_bIsBeingDestroyed(false),
    // Register Requests
    //m_arDSNodeSpecsRegister(),
    //m_arDSNodeSpecsSelectSchema(),
    //m_arDSNodeSpecsSelectData(),
    // Requests received by the database:
    //m_iReqMsgIdDb(-1),
    //m_iReqIdDb(-1),
    //m_pReqDb(nullptr),
    // Tracing
    m_arpTrcMsgLogObjects(),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

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
    SClientHostSettings hostSettings;
    STimerSettings      watchDogSettings;

    for( int idxProtType = 0; idxProtType < m_arProtocolTypes.size(); idxProtType++ )
    {
        cnctType.m_protocolType = m_arProtocolTypes[idxProtType];

        for( int idxSockType = 0; idxSockType < m_hsharSocketTypes[cnctType.m_protocolType].size(); idxSockType++ )
        {
            cnctType.m_socketType = m_hsharSocketTypes[cnctType.m_protocolType][idxSockType];

            /* struct SClientHostSettings {
                ESocketType  m_socketType;
                QObject*     m_pObjRemote;
                QString      m_strRemoteHostName;
                quint16      m_uRemotePort; // Port the server is listening for incoming connection requests.
                int          m_iConnectTimeout_ms;
                unsigned int m_uBufferSize; // only used by shared memory socket clients
            } */

            hostSettings.m_socketType = cnctType.m_socketType;

            if( cnctType.m_socketType == ESocketTypeTcp )
            {
                hostSettings.m_pObjRemote = nullptr;
                hostSettings.m_strRemoteHostName = QHostInfo::localHostName();
                hostSettings.m_uRemotePort = 19637;
                hostSettings.m_iConnectTimeout_ms = 10000;
                //hostSettings.m_uBufferSize = 0;
            }
            else if( cnctType.m_socketType == ESocketTypeInProcMsg )
            {
                hostSettings.m_pObjRemote = nullptr;           // setDb must be called later
                hostSettings.m_strRemoteHostName = "Db";    // pDb->objectName();
                //hostSettings.m_uRemotePort;
                hostSettings.m_iConnectTimeout_ms = 1000;
                //hostSettings.m_uBufferSize = 0;
            }

            m_hshHostSettingsOrig[cnctType] = hostSettings;
            m_hshWatchDogSettingsOrig[cnctType] = watchDogSettings;

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
CDbClient::~CDbClient()
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

    //try
    //{
    //    delete m_pReqDb;
    //}
    //catch(...)
    //{
    //}
    //m_pReqDb = nullptr;

    abortAllRequests();

    SCnctType cnctType(m_protocolTypeOrig, m_socketTypeOrig);

    SClientHostSettings hostSettingsCurr = m_hshHostSettingsOrig[cnctType];

    if( m_pGatewayThread != nullptr )
    {
        if( m_pGatewayThread->isRunning() )
        {
            m_pGatewayThread->quit();
            m_pGatewayThread->wait(10000+hostSettingsCurr.m_iConnectTimeout_ms);
        }
        try
        {
            delete m_pGatewayThread;
        }
        catch(...)
        {
        }
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
    m_pDS = nullptr;
    // Supported protocol and connection types
    //m_arProtocolTypes.clear();
    //m_hsharSocketTypes.clear();
    // Default, current and new data protocol type to be adjusted
    m_protocolTypeDefault = static_cast<EProtocolType>(0);
    m_protocolTypeOrig = static_cast<EProtocolType>(0);
    m_protocolTypeCurr = static_cast<EProtocolType>(0);
    // Default, current and new socket type to be adjusted
    m_socketTypeDefault = static_cast<ESocketType>(0);
    m_socketTypeOrig = static_cast<ESocketType>(0);
    m_socketTypeCurr = static_cast<ESocketType>(0);
    // Default, current and new host settings to be adjusted
    //m_hshHostSettingsDefault.clear();
    //m_hshHostSettingsOrig.clear();
    //m_hshHostSettingsCurr.clear();
    // Currently used socket descriptor (only valid if client is connected)
    //m_cnctId;
    //m_socketDscr;
    // Default, current and new data watch dog settings to be adjusted
    //m_hshWatchDogSettingsDefault.clear();
    //m_hshWatchDogSettingsOrig.clear();
    //m_hshWatchDogSettingsCurr.clear();
    // Gateway
    m_pGatewayThread = nullptr;
    m_pGateway = nullptr;
    // State Machine
    m_state = static_cast<EState>(0);
    m_pRequestQueue = nullptr;
    //m_reqDscrTimeout;
    m_pTmrReqTimeout = nullptr;
    m_bMsgReqContinuePending = false;
    m_bIsBeingDestroyed = false;
    // Register Requests
    //m_arDSNodeSpecsRegister.clear();
    //m_arDSNodeSpecsSelectSchema.clear();
    //m_arDSNodeSpecsSelectData.clear();
    // Requests received by the database:
    //m_iReqMsgIdDb = 0;
    //m_iReqIdDb = 0;
    //m_pReqDb = nullptr;
    // Tracing
    //m_arpTrcMsgLogObjects.clear();
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::setKeepReqDscrInExecTree( bool i_bKeep )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = bool2Str(i_bKeep);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setKeepReqDscrInExecTree",
        /* strAddInfo   */ strAddTrcInfo );

    m_pRequestQueue->setKeepReqDscrInExecTree(i_bKeep);

} // setKeepReqDscrInExecTree

//------------------------------------------------------------------------------
bool CDbClient::keepReqDscrInExecTree() const
//------------------------------------------------------------------------------
{
    return m_pRequestQueue->keepReqDscrInExecTree();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::setDataSet( CDataSet* i_pDS )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString(i_pDS == nullptr ? "nullptr" : i_pDS->getName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDataSet",
        /* strAddInfo   */ strAddTrcInfo );

    if( i_pDS != nullptr && m_pDS != nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall, "DbClient already has a DataSet");
    }

    if( m_pDS != nullptr )
    {
        QObject::disconnect(
            /* pObjSender   */ m_pDS,
            /* szSignal     */ SIGNAL(destroyed(QObject*)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDSDestroyed(QObject*)) );

    } // if( m_pDS != nullptr )

    m_pDS = i_pDS;

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

} // setDataSet

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CDbClient::getProtocolTypesCount( ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
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
QList<EProtocolType> CDbClient::getProtocolTypes( ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QList<EProtocolType> arProtocolTypes;

    ESocketType socketType = i_socketType;

    if( socketType == ESocketTypeUndefined )
    {
        if( m_socketTypeCurr != ESocketTypeUndefined )
        {
            socketType = m_socketTypeCurr;
        }
        else
        {
            socketType = m_socketTypeOrig;
        }
    }

    if( socketType == ESocketTypeCount )
    {
        arProtocolTypes = m_arProtocolTypes;
    }
    else
    {
        QHash<EProtocolType,QList<Ipc::ESocketType>>::const_iterator it;

        for( it = m_hsharSocketTypes.begin(); it != m_hsharSocketTypes.end(); it++ )
        {
            if( it.value().contains(socketType) )
            {
                arProtocolTypes.append(it.key());
            }
        }
    }

    return arProtocolTypes;

} // getProtocolTypes

//------------------------------------------------------------------------------
bool CDbClient::isProtocolTypeChangeable( ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    QList<EProtocolType> arProtocolTypes = getProtocolTypes(i_socketType);
    return (arProtocolTypes.size() > 1);
}

//------------------------------------------------------------------------------
bool CDbClient::isProtocolTypeSupported( EProtocolType i_protocolType, ESocketType i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    QList<EProtocolType> arProtocolTypes = getProtocolTypes(i_socketType);
    return arProtocolTypes.contains(i_protocolType);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CDbClient::getSocketTypesCount( EProtocolType i_protocolType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    QList<ESocketType> arSocketTypes = getSocketTypes(i_protocolType);
    return arSocketTypes.size();
}

//------------------------------------------------------------------------------
QList<ESocketType> CDbClient::getSocketTypes( EProtocolType i_protocolType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    EProtocolType protocolType = i_protocolType;

    if( protocolType == EProtocolTypeUndefined )
    {
        if( m_protocolTypeCurr != EProtocolTypeUndefined )
        {
            protocolType = m_protocolTypeCurr;
        }
        else
        {
            protocolType = m_protocolTypeOrig;
        }
    }

    QList<ESocketType> arSocketTypes;

    if( protocolType == EProtocolTypeCount )
    {
        QHash<EProtocolType,QList<Ipc::ESocketType>>::const_iterator it;

        for( it = m_hsharSocketTypes.begin(); it != m_hsharSocketTypes.end(); it++ )
        {
            for( int idxSocketType = 0; idxSocketType < it.value().size(); idxSocketType++ )
            {
                ESocketType socketType = it.value()[idxSocketType];

                if( !arSocketTypes.contains(socketType) )
                {
                    arSocketTypes.append(socketType);
                }
            }
        }
    }
    else // if( protocolType != EProtocolTypeCount )
    {
        if( m_hsharSocketTypes.contains(protocolType) )
        {
            arSocketTypes = m_hsharSocketTypes[protocolType];
        }
    }
    return arSocketTypes;

} // getSocketTypes

//------------------------------------------------------------------------------
bool CDbClient::isSocketTypeChangeable( EProtocolType i_protocolType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    QList<ESocketType> arSocketTypes = getSocketTypes(i_protocolType);
    return (arSocketTypes.size() > 1);
}

//------------------------------------------------------------------------------
bool CDbClient::isSocketTypeSupported( ESocketType i_socketType, EProtocolType i_protocolType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    QList<ESocketType> arSocketTypes = getSocketTypes(i_protocolType);
    return arSocketTypes.contains(i_socketType);
}

/*==============================================================================
public: // overridables
==============================================================================*/

////------------------------------------------------------------------------------
//bool CDbClient::connectionNeedsDatabaseName( ESocketType i_socketType, EProtocolType i_protocolType ) const
////------------------------------------------------------------------------------
//{
//    QMutexLocker mtxLocker(m_pMtx);
//    //return m_hostSettingsCurr.m_bDatabaseNameUsed;
//    return false;
//}
//
////------------------------------------------------------------------------------
//bool CDbClient::connectionNeedsUserName( ESocketType i_socketType, EProtocolType i_protocolType ) const
////------------------------------------------------------------------------------
//{
//    QMutexLocker mtxLocker(m_pMtx);
//    //return m_hostSettingsCurr.m_bUserNameUsed;
//    return false;
//}
//
////------------------------------------------------------------------------------
//bool CDbClient::connectionNeedsPassword( ESocketType i_socketType, EProtocolType i_protocolType ) const
////------------------------------------------------------------------------------
//{
//    QMutexLocker mtxLocker(m_pMtx);
//    //return m_hostSettingsCurr.m_bPasswordUsed;
//    return false;
//}

/*==============================================================================
public: // instance methods (affecting all settings)
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::setHostSettings2Default()
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
        /* strMethod    */ "setHostSettings2Default",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SCnctType cnctType(m_protocolTypeOrig, m_socketTypeOrig);

    SClientHostSettings* pClientSettingsDefault = getDefaultHostSettings(cnctType.m_protocolType, cnctType.m_socketType);
    STimerSettings*      pWatchDogSettingsDefault = getDefaultWatchDogSettings(cnctType.m_protocolType, cnctType.m_socketType);

    if( pClientSettingsDefault != nullptr )
    {
        setHostSettings(*pClientSettingsDefault, cnctType.m_protocolType);
    }
    if( pWatchDogSettingsDefault != nullptr )
    {
        setWatchDogSettings(*pWatchDogSettingsDefault, cnctType.m_protocolType, cnctType.m_socketType);
    }

} // setHostSettings2Default

/*==============================================================================
public: // instance methods changing and reading the host settings
==============================================================================*/

//------------------------------------------------------------------------------
EProtocolType CDbClient::getProtocolType( ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    EProtocolType protocolType = EProtocolTypeUndefined;

    if( i_version == ERowVersion::Original )
    {
        protocolType = m_protocolTypeOrig;
    }
    else if( i_version == ERowVersion::Current )
    {
        protocolType = m_protocolTypeCurr;
    }
    else if (i_version == ERowVersion::Default || i_version == ERowVersion::Undefined )
    {
        if( m_protocolTypeCurr != EProtocolTypeUndefined )
        {
            protocolType = m_protocolTypeCurr;
        }
        else
        {
            protocolType = m_protocolTypeOrig;
        }
    }

    return protocolType;

} // getProtocolType

//------------------------------------------------------------------------------
void CDbClient::setProtocolType( EProtocolType i_protocolType )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = protocolType2Str(i_protocolType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setProtocolType",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    bool bSettingsChanged = false;
    bool bVersionChanged = false;

    // If there is already a current version available ..
    if( m_protocolTypeCurr != EProtocolTypeUndefined )
    {
        // If the new setting is different from the current version ...
        if( m_protocolTypeCurr != i_protocolType )
        {
            // If the new setting is equal to the original version ...
            if( m_protocolTypeOrig == i_protocolType )
            {
                // .. invalidate current version. Nothing to be applied later on.
                m_protocolTypeCurr = EProtocolTypeUndefined;
                bSettingsChanged = true;
                bVersionChanged = true;
            }
            // If the new setting is different from the original version ...
            else
            {
                // ... take over new setting as the current version to be applied later on.
                m_protocolTypeCurr = i_protocolType;
                bSettingsChanged = true;
            }
        }
    }
    // If a current version is not yet available ..
    else // if( m_strNameCurr.isNull() )
    {
        // If the new setting is different from the original settings ...
        if( m_protocolTypeOrig != i_protocolType )
        {
            // ... take over new setting as the current version to be applied later on.
            m_protocolTypeCurr = i_protocolType;
            bSettingsChanged = true;
            bVersionChanged = true;
        }
    }

    if( bVersionChanged )
    {
        emit protocolTypeRowVersionChanged(this);
    }
    if( bSettingsChanged )
    {
        EProtocolType protocolType = getProtocolType();

        emit protocolTypeChanged(this, protocolType);
    }

} // setProtocolType

/*==============================================================================
public: // instance methods changing and reading the host settings
==============================================================================*/

//------------------------------------------------------------------------------
ESocketType CDbClient::getSocketType( ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    ESocketType socketType = ESocketTypeUndefined;

    if( i_version == ERowVersion::Original )
    {
        socketType = m_socketTypeOrig;
    }
    else if( i_version == ERowVersion::Current )
    {
        socketType = m_socketTypeCurr;
    }
    else if (i_version == ERowVersion::Default || i_version == ERowVersion::Undefined )
    {
        if( m_socketTypeCurr != ESocketTypeUndefined )
        {
            socketType = m_socketTypeCurr;
        }
        else
        {
            socketType = m_socketTypeOrig;
        }
    }

    return socketType;

} // getSocketType

//------------------------------------------------------------------------------
void CDbClient::setSocketType( ESocketType i_socketType )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = socketType2Str(i_socketType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setSocketType",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    bool bSettingsChanged = false;
    bool bVersionChanged = false;

    // If there is already a current version available ..
    if( m_socketTypeCurr != ESocketTypeUndefined )
    {
        // If the new setting is different from the current version ...
        if( m_socketTypeCurr != i_socketType )
        {
            // If the new setting is equal to the original version ...
            if( m_socketTypeOrig == i_socketType )
            {
                // .. invalidate current version. Nothing to be applied later on.
                m_socketTypeCurr = ESocketTypeUndefined;
                bSettingsChanged = true;
                bVersionChanged = true;
            }
            // If the new setting is different from the original version ...
            else
            {
                // ... take over new setting as the current version to be applied later on.
                m_socketTypeCurr = i_socketType;
                bSettingsChanged = true;
            }
        }
    }
    // If a current version is not yet available ..
    else // if( m_strNameCurr.isNull() )
    {
        // If the new setting is different from the original settings ...
        if( m_socketTypeOrig != i_socketType )
        {
            // ... take over new setting as the current version to be applied later on.
            m_socketTypeCurr = i_socketType;
            bSettingsChanged = true;
            bVersionChanged = true;
        }
    }

    if( bVersionChanged )
    {
        emit socketTypeRowVersionChanged(this);
    }
    if( bSettingsChanged )
    {
        ESocketType socketType = getSocketType();

        emit socketTypeChanged(this, socketType);
    }

} // setSocketType

/*==============================================================================
public: // instance methods changing and reading the host settings
==============================================================================*/

//------------------------------------------------------------------------------
CDb* CDbClient::getDb ( ERowVersion i_version ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    SClientHostSettings hostSettings = getHostSettings(EProtocolTypeZSXML, ESocketTypeInProcMsg, i_version);

    return dynamic_cast<CDb*>(hostSettings.m_pObjRemote);

} // getDb

//------------------------------------------------------------------------------
void CDbClient::setDb( CDb* i_pDb )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( i_pDb == nullptr ? "nullptr" : i_pDb->objectName() );
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDb",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    SCnctType cnctType(EProtocolTypeZSXML, ESocketTypeInProcMsg);

    bool bSettingsChanged = false;
    bool bVersionChanged = false;

    // If there is already a current version available ..
    if( m_hshHostSettingsCurr.contains(cnctType) )
    {
        // If the new setting is different from the current version ...
        if( m_hshHostSettingsCurr[cnctType].m_pObjRemote != i_pDb )
        {
            m_hshHostSettingsCurr[cnctType].m_pObjRemote = i_pDb;

            // If the new setting is equal to the original version ...
            if( m_hshHostSettingsOrig[cnctType] == m_hshHostSettingsCurr[cnctType] )
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
                bSettingsChanged = true;
            }
        }
    }
    // If a current version is not yet available ..
    else // if( m_strNameCurr.isNull() )
    {
        // If the new setting is different from the original settings ...
        if( m_hshHostSettingsOrig[cnctType].m_pObjRemote != i_pDb )
        {
            // ... take over new setting as the current version to be applied later on.
            m_hshHostSettingsCurr[cnctType] = m_hshHostSettingsOrig[cnctType];
            m_hshHostSettingsCurr[cnctType].m_pObjRemote = i_pDb;
            bSettingsChanged = true;
            bVersionChanged = true;
        }
    }

    if( bVersionChanged )
    {
        EProtocolType protocolType = getProtocolType();
        ESocketType   socketType   = getSocketType();

        emit hostSettingsRowVersionChanged(this, protocolType, socketType);
    }
    if( bSettingsChanged )
    {
        EProtocolType       protocolType = getProtocolType();
        SClientHostSettings hostSettings = getHostSettings();

        emit hostSettingsChanged(this, protocolType, hostSettings);
    }

} // setDb

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
QString CDbClient::getConnectionString(
    EProtocolType i_protocolType,
    ESocketType   i_socketType,
    ERowVersion   i_version ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    SClientHostSettings hostSettings = getHostSettings(i_protocolType, i_socketType, i_version);
    return hostSettings.getConnectionString();
}

//------------------------------------------------------------------------------
SClientHostSettings CDbClient::getHostSettings(
    EProtocolType i_protocolType,
    ESocketType   i_socketType,
    ERowVersion   i_version ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    ESocketType   socketType   = i_socketType;
    EProtocolType protocolType = i_protocolType;

    if( socketType == ESocketTypeUndefined )
    {
        socketType = getSocketType(i_version);
    }

    if( protocolType == EProtocolTypeUndefined )
    {
        protocolType = getProtocolType(i_version);
    }

    SClientHostSettings hostSettings;

    SCnctType cnctType(protocolType,socketType);

    if( i_version == ERowVersion::Original )
    {
        hostSettings = m_hshHostSettingsOrig[cnctType];
    }
    else if( i_version == ERowVersion::Current )
    {
        hostSettings = m_hshHostSettingsCurr.value(cnctType, SClientHostSettings());
    }
    else if (i_version == ERowVersion::Default || i_version == ERowVersion::Undefined )
    {
        if( m_hshHostSettingsCurr.contains(cnctType) )
        {
            hostSettings = m_hshHostSettingsCurr[cnctType];
        }
        else if( m_hshHostSettingsOrig.contains(cnctType) )
        {
            hostSettings = m_hshHostSettingsOrig[cnctType];
        }
    }

    return hostSettings;

} // getHostSettings

//------------------------------------------------------------------------------
int CDbClient::getConnectTimeoutInMs(
    EProtocolType i_protocolType,
    ESocketType   i_socketType,
    ERowVersion   i_version ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    SClientHostSettings hostSettings = getHostSettings(i_protocolType, i_socketType, i_version);
    return hostSettings.m_iConnectTimeout_ms;
}

//------------------------------------------------------------------------------
void CDbClient::setHostSettings(
    const SClientHostSettings& i_hostSettings,
    EProtocolType              i_protocolType )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = i_hostSettings.getConnectionString();
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setHostSettings",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    EProtocolType protocolType = i_protocolType;

    if( protocolType == EProtocolTypeUndefined )
    {
        protocolType = getProtocolType();
    }

    SCnctType cnctType(protocolType, i_hostSettings.m_socketType);

    SClientHostSettings hostSettings = i_hostSettings;

    // There is no need to set host settings for the InProcMsg socket type.
    // There are no parameters which can be changed. But if someone calls
    // this method with InProcMsg we ensure that the structure has got the
    // correct values.
    if( hostSettings.m_socketType == ESocketTypeInProcMsg )
    {
        if( hostSettings.m_pObjRemote != nullptr )
        {
            if( hostSettings.m_strRemoteHostName.isEmpty() )
            {
                hostSettings.m_strRemoteHostName = hostSettings.m_pObjRemote->objectName();
            }
            else
            {
                if( hostSettings.m_strRemoteHostName != hostSettings.m_pObjRemote->objectName() )
                {
                    throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "Invalid host settings for InProcMsg socket type" );
                }
            }
        }

        hostSettings.m_uRemotePort = 0;
        //hostSettings.m_iConnectTimeout_ms = 0;
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
        EProtocolType protocolType = getProtocolType();
        ESocketType   socketType   = getSocketType();

        emit hostSettingsRowVersionChanged(this, protocolType, socketType);
    }
    if( bSettingsChanged )
    {
        EProtocolType protocolType = getProtocolType();

        hostSettings = getHostSettings();

        emit hostSettingsChanged(this, protocolType, hostSettings);
    }

} // setHostSettings

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
EProtocolType CDbClient::getDefaultProtocolType() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_protocolTypeDefault;
}

//------------------------------------------------------------------------------
void CDbClient::setDefaultProtocolType( EProtocolType i_protocolType )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = protocolType2Str(i_protocolType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDefaultProtocolType",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_protocolTypeDefault != i_protocolType )
    {
        m_protocolTypeDefault = i_protocolType;

        emit defaultProtocolTypeChanged(this, m_protocolTypeDefault);
    }

} // setDefaultProtocolType

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
ESocketType CDbClient::getDefaultSocketType() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_socketTypeDefault;
}

//------------------------------------------------------------------------------
void CDbClient::setDefaultSocketType( ESocketType i_socketType )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = socketType2Str(i_socketType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDefaultSocketType",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_socketTypeDefault != i_socketType )
    {
        m_socketTypeDefault = i_socketType;

        emit defaultSocketTypeChanged(this, m_socketTypeDefault);
    }

} // setDefaultSocketType

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDbClient::getDefaultConnectionString(
    EProtocolType i_protocolType,
    ESocketType   i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strConnection;

    SClientHostSettings* pHostSettings = getDefaultHostSettings(i_protocolType, i_socketType);

    if( pHostSettings != nullptr )
    {
        strConnection = pHostSettings->getConnectionString();
    }

    return strConnection;

} // getDefaultConnectionString

//------------------------------------------------------------------------------
SClientHostSettings* CDbClient::getDefaultHostSettings(
    EProtocolType i_protocolType,
    ESocketType   i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    SClientHostSettings* pHostSettings = nullptr;

    ESocketType   socketType   = i_socketType;
    EProtocolType protocolType = i_protocolType;

    if( socketType == ESocketTypeUndefined )
    {
        socketType = m_socketTypeDefault;
    }

    if( protocolType == EProtocolTypeUndefined )
    {
        protocolType = m_protocolTypeDefault;
    }

    SCnctType cnctType(protocolType,socketType);

    if( m_hshHostSettingsDefault.contains(cnctType) )
    {
        CDbClient* pVThis = const_cast<CDbClient*>(this);

        pHostSettings = &pVThis->m_hshHostSettingsDefault[cnctType];
    }

    return pHostSettings;

} // getDefaultHostSettings

//------------------------------------------------------------------------------
void CDbClient::setDefaultHostSettings(
    const SClientHostSettings& i_hostSettings,
    EProtocolType              i_protocolType )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_hostSettings.getConnectionString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDefaultHostSettings",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    EProtocolType protocolType = i_protocolType;

    if( protocolType == EProtocolTypeUndefined )
    {
        protocolType = m_protocolTypeDefault;
    }

    SCnctType cnctType(protocolType, i_hostSettings.m_socketType);

    SClientHostSettings hostSettings = i_hostSettings;

    // There is no need to set host settings for the InProcMsg socket type.
    // There are no parameters which can be changed. But if someone calls
    // this method with InProcMsg we ensure that the structure has got the
    // correct values.
    if( hostSettings.m_socketType == ESocketTypeInProcMsg )
    {
        if( hostSettings.m_pObjRemote != nullptr )
        {
            if( hostSettings.m_strRemoteHostName.isEmpty() )
            {
                hostSettings.m_strRemoteHostName = hostSettings.m_pObjRemote->objectName();
            }
            else
            {
                if(& hostSettings.m_strRemoteHostName != hostSettings.m_pObjRemote->objectName() )
                {
                    throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "Invalid host settings for InProcMsg socket type" );
                }
            }
        }

        hostSettings.m_uRemotePort = 0;
        //hostSettings.m_iConnectTimeout_ms = 0;
        hostSettings.m_uBufferSize = 0;

    } // if( hostSettings.m_socketType == ESocketTypeInProcMsg )

    if( m_hshHostSettingsDefault.value(cnctType, SClientHostSettings()) != hostSettings )
    {
        m_hshHostSettingsDefault[cnctType] = hostSettings;

        emit defaultHostSettingsChanged(this, protocolType, hostSettings);
    }

} // setDefaultHostSettings

/*==============================================================================
public: // instance methods changing and reading the watch dog settings
==============================================================================*/

//------------------------------------------------------------------------------
bool CDbClient::isWatchDogTimerEnabled(
    EProtocolType i_protocolType,
    ESocketType   i_socketType,
    ERowVersion   i_version ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    STimerSettings watchDogSettings = getWatchDogSettings(i_protocolType, i_socketType, i_version);
    return watchDogSettings.m_bEnabled;
}

//------------------------------------------------------------------------------
int CDbClient::getWatchDogTimerIntervalInMs(
    EProtocolType i_protocolType,
    ESocketType   i_socketType,
    ERowVersion   i_version ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    STimerSettings watchDogSettings = getWatchDogSettings(i_protocolType, i_socketType, i_version);
    return watchDogSettings.m_iInterval_ms;
}

//------------------------------------------------------------------------------
int CDbClient::getWatchDogTimeoutInMs(
    EProtocolType i_protocolType,
    ESocketType   i_socketType,
    ERowVersion   i_version ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    STimerSettings watchDogSettings = getWatchDogSettings(i_protocolType, i_socketType, i_version);
    return watchDogSettings.m_iTimeout_ms;
}

//------------------------------------------------------------------------------
STimerSettings CDbClient::getWatchDogSettings(
    EProtocolType i_protocolType,
    ESocketType   i_socketType,
    ERowVersion   i_version ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    STimerSettings watchDogSettings(false);

    ESocketType   socketType   = i_socketType;
    EProtocolType protocolType = i_protocolType;

    if( socketType == ESocketTypeUndefined )
    {
        socketType = getSocketType(i_version);
    }

    if( protocolType == EProtocolTypeUndefined )
    {
        protocolType = getProtocolType(i_version);
    }

    SCnctType cnctType(protocolType,socketType);

    if( i_version == ERowVersion::Original )
    {
        watchDogSettings = m_hshWatchDogSettingsOrig.value(cnctType,  STimerSettings(false));
    }
    else if( i_version == ERowVersion::Current )
    {
        watchDogSettings = m_hshWatchDogSettingsCurr.value(cnctType, STimerSettings(false));
    }
    else if (i_version == ERowVersion::Default || i_version == ERowVersion::Undefined )
    {
        if( m_hshWatchDogSettingsCurr.contains(cnctType) )
        {
            watchDogSettings = m_hshWatchDogSettingsCurr[cnctType];
        }
        else if( m_hshWatchDogSettingsOrig.contains(cnctType) )
        {
            watchDogSettings = m_hshWatchDogSettingsOrig[cnctType];
        }
    }

    return watchDogSettings;

} // getWatchDogSettings

//------------------------------------------------------------------------------
void CDbClient::setWatchDogSettings(
    const STimerSettings& i_watchDogSettings,
    EProtocolType         i_protocolType,
    ESocketType           i_socketType )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "WatchDogSettings {" + i_watchDogSettings.toString() + "}";
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", SockType: " + socketType2Str(i_socketType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setWatchDogSettings",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    ESocketType   socketType   = i_socketType;
    EProtocolType protocolType = i_protocolType;

    if( socketType == ESocketTypeUndefined )
    {
        socketType = getSocketType();
    }

    if( protocolType == EProtocolTypeUndefined )
    {
        protocolType = getProtocolType();
    }

    SCnctType cnctType(protocolType,socketType);

    bool bSettingsChanged = false;
    bool bVersionChanged = false;

    // If there is already a current version available ..
    if( m_hshWatchDogSettingsCurr.contains(cnctType) )
    {
        // If the new setting is different from the current version ...
        if( m_hshWatchDogSettingsCurr[cnctType] != i_watchDogSettings )
        {
            // If the new setting is equal to the original version ...
            if( m_hshWatchDogSettingsOrig[cnctType] == i_watchDogSettings )
            {
                // .. invalidate current version. Nothing to be applied later on.
                m_hshWatchDogSettingsCurr.remove(cnctType);
                bSettingsChanged = true;
                bVersionChanged = true;
            }
            // If the new setting is different from the original version ...
            else
            {
                // ... take over new setting as the current version to be applied later on.
                m_hshWatchDogSettingsCurr[cnctType] = i_watchDogSettings;
                bSettingsChanged = true;
            }
        }
    }
    // If a current version is not yet available ..
    else // if( m_strNameCurr.isNull() )
    {
        // If the new setting is different from the original settings ...
        if( m_hshWatchDogSettingsOrig[cnctType] != i_watchDogSettings )
        {
            // ... take over new setting as the current version to be applied later on.
            m_hshWatchDogSettingsCurr[cnctType] = i_watchDogSettings;
            bSettingsChanged = true;
            bVersionChanged = true;
        }
    }

    if( bVersionChanged )
    {
        EProtocolType protocolType = getProtocolType();
        ESocketType   socketType   = getSocketType();

        emit watchDogSettingsRowVersionChanged(this, protocolType, socketType);
    }
    if( bSettingsChanged )
    {
        EProtocolType  protocolType     = getProtocolType();
        ESocketType    socketType       = getSocketType();
        STimerSettings watchDogSettings = getWatchDogSettings();

        emit watchDogSettingsChanged(this, protocolType, socketType, watchDogSettings);
    }

} // setWatchDogSettings

/*==============================================================================
public: // instance methods changing and reading the watch dog settings
==============================================================================*/

//------------------------------------------------------------------------------
STimerSettings* CDbClient::getDefaultWatchDogSettings(
    EProtocolType i_protocolType,
    ESocketType   i_socketType ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    STimerSettings* pWatchDogSettings = nullptr;

    ESocketType   socketType   = i_socketType;
    EProtocolType protocolType = i_protocolType;

    if( socketType == ESocketTypeUndefined )
    {
        socketType = m_socketTypeDefault;
    }

    if( protocolType == EProtocolTypeUndefined )
    {
        protocolType = m_protocolTypeDefault;
    }

    SCnctType cnctType(protocolType,socketType);

    if( m_hshWatchDogSettingsDefault.contains(cnctType) )
    {
        CDbClient* pVThis = const_cast<CDbClient*>(this);

        pWatchDogSettings = &pVThis->m_hshWatchDogSettingsDefault[cnctType];
    }

    return pWatchDogSettings;

} // getDefaultWatchDogSettings

//------------------------------------------------------------------------------
void CDbClient::setDefaultWatchDogSettings(
    const STimerSettings& i_watchDogSettings,
    EProtocolType         i_protocolType,
    ESocketType           i_socketType )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "WatchDogSettings {" + i_watchDogSettings.toString() + "}";
        strAddTrcInfo += ", ProtType: " + protocolType2Str(i_protocolType);
        strAddTrcInfo += ", SockType: " + socketType2Str(i_socketType);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDefaultWatchDogSettings",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    ESocketType   socketType   = i_socketType;
    EProtocolType protocolType = i_protocolType;

    if( socketType == ESocketTypeUndefined )
    {
        socketType = m_socketTypeDefault;
    }

    if( protocolType == EProtocolTypeUndefined )
    {
        protocolType = m_protocolTypeDefault;
    }

    SCnctType cnctType(protocolType, socketType);

    if( m_hshWatchDogSettingsDefault.value(cnctType, STimerSettings(false)) != i_watchDogSettings )
    {
        m_hshWatchDogSettingsDefault[cnctType] = i_watchDogSettings;

        emit defaultWatchDogSettingsChanged(this, protocolType, socketType, i_watchDogSettings);
    }

} // setDefaultWatchDogSettings

/*==============================================================================
public: // instance methods of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
SCnctId CDbClient::getCnctId() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_cnctId;
}

//------------------------------------------------------------------------------
SSocketDscr CDbClient::getSocketDscr() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_socketDscr;
}

//------------------------------------------------------------------------------
int CDbClient::getSocketId() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_socketDscr.m_iSocketId;
}

/*==============================================================================
public: // instance methods (Register and Select Requests)
==============================================================================*/

////------------------------------------------------------------------------------
//SDbClientRegSpec CDbClient::getRegistrationSpec() const
////------------------------------------------------------------------------------
//{
//    QMutexLocker mtxLocker(m_pMtx);
//    SDbClientRegSpec dbCltRegSpec(m_cnctId);
//    dbCltRegSpec.m_strClientName = objectName();
//    dbCltRegSpec.m_arDSNodeSpecs = m_arDSNodeSpecsRegister;
//    return dbCltRegSpec;
//}
//
////------------------------------------------------------------------------------
//void CDbClient::setRegisterNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs )
////------------------------------------------------------------------------------
//{
//    QMutexLocker mtxLocker(m_pMtx);
//
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "DSNodeSpecs [" + QString::number(i_arDSNodeSpecs.size()) + "]";
//
//        if( i_arDSNodeSpecs.size() > 0 )
//        {
//            strAddTrcInfo += "(";
//
//            for( int idxSpec = 0; idxSpec < i_arDSNodeSpecs.size(); idxSpec++ )
//            {
//                if( idxSpec > 0 )
//                {
//                    strAddTrcInfo += ", ";
//                }
//                strAddTrcInfo += QString::number(idxSpec) + ": ";
//                strAddTrcInfo += "{ " + i_arDSNodeSpecs[idxSpec].getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//            }
//            strAddTrcInfo += ")";
//        }
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "setRegisterNodeSpecs",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    m_arDSNodeSpecsRegister = i_arDSNodeSpecs;
//
//} // setRegisterNodeSpecs
//
////------------------------------------------------------------------------------
//void CDbClient::setSelectSchemaNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs )
////------------------------------------------------------------------------------
//{
//    QMutexLocker mtxLocker(m_pMtx);
//
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "DSNodeSpecs [" + QString::number(i_arDSNodeSpecs.size()) + "]";
//
//        if( i_arDSNodeSpecs.size() > 0 )
//        {
//            strAddTrcInfo += "(";
//
//            for( int idxSpec = 0; idxSpec < i_arDSNodeSpecs.size(); idxSpec++ )
//            {
//                if( idxSpec > 0 )
//                {
//                    strAddTrcInfo += ", ";
//                }
//                strAddTrcInfo += QString::number(idxSpec) + ": ";
//                strAddTrcInfo += "{ " + i_arDSNodeSpecs[idxSpec].getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//            }
//            strAddTrcInfo += ")";
//        }
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "setSelectSchemaNodeSpecs",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    m_arDSNodeSpecsSelectSchema = i_arDSNodeSpecs;
//
//} // setSelectSchemaNodeSpecs
//
////------------------------------------------------------------------------------
//void CDbClient::setSelectDataNodeSpecs( const QVector<SDSNodeSpec>& i_arDSNodeSpecs )
////------------------------------------------------------------------------------
//{
//    QMutexLocker mtxLocker(m_pMtx);
//
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "DSNodeSpecs [" + QString::number(i_arDSNodeSpecs.size()) + "]";
//
//        if( i_arDSNodeSpecs.size() > 0 )
//        {
//            strAddTrcInfo += "(";
//
//            for( int idxSpec = 0; idxSpec < i_arDSNodeSpecs.size(); idxSpec++ )
//            {
//                if( idxSpec > 0 )
//                {
//                    strAddTrcInfo += ", ";
//                }
//                strAddTrcInfo += QString::number(idxSpec) + ": ";
//                strAddTrcInfo += "{ " + i_arDSNodeSpecs[idxSpec].getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//            }
//            strAddTrcInfo += ")";
//        }
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "setSelectDataNodeSpecs",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    m_arDSNodeSpecsSelectData = i_arDSNodeSpecs;
//
//} // setSelectDataNodeSpecs

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CDbClient::EState CDbClient::getState() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_state;
}

//------------------------------------------------------------------------------
QString CDbClient::state2Str() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return State2Str(m_state);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDbClient::isConnected() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return (m_state >= EStateConnected);
}

//------------------------------------------------------------------------------
bool CDbClient::isConnecting() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bIsConnecting = false;

    //if( m_pRequestQueue->getRequestInProgress()->getRequest() == ERequestConnect )
    //{
    //    bIsConnecting = true;
    //}
    //else if( m_pRequestQueue->hasPostponedRequest() )
    //{
    //    CRequest* pReq;
    //    int       idxReq;

    //    for( idxReq = m_pRequestQueue->getPostponedRequestsCount()-1; idxReq >= 0; idxReq-- )
    //    {
    //        pReq = m_pRequestQueue->getPostponedRequestByIdx(idxReq);

    //        if( pReq->getRequest() == ERequestConnect )
    //        {
    //            bIsConnecting = true;
    //            break;
    //        }
    //        else if( pReq->getRequest() == ERequestDisconnect )
    //        {
    //            break;
    //        }
    //    }
    //} // if( m_pRequestQueue->hasPostponedRequest() )

    return bIsConnecting;

} // isConnecting

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//bool CDbClient::isRegistered() const
////------------------------------------------------------------------------------
//{
//    QMutexLocker mtxLocker(m_pMtx);
//    return (m_state >= EStateRegistered);
//}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDbClient::isBusy() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return isRequestInProgress();
}

//------------------------------------------------------------------------------
bool CDbClient::isRequestInProgress(
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
CDbClient::ERequest CDbClient::getRequestInProgress() const
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
QString CDbClient::requestInProgress2Str( bool /*i_bShort*/ ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return Request2Str( getRequestInProgress() );
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::abortRequest( qint64 i_iRequestId )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = "RequestToBeAborted: " + CRequestExecTree::GetAddTrcInfoStr(i_iRequestId);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "abortRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".abortRequest";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    errResultInfo.setSeverity(EResultSeverityInfo);
    errResultInfo.setResult(EResultRequestAborted);

    //EState statePrev = m_state;

    SCnctType cnctType(m_protocolTypeOrig, m_socketTypeOrig);

    //SClientHostSettings hostSettingsCurr = m_hshHostSettingsOrig[cnctType];

    bool bIsPostponedRequest;
    int  idxPostponedRequest;

    CRequest* pReq = m_pRequestQueue->findRequestById(
        /* iRequestId               */ i_iRequestId,
        /* bIgnorePostponedRequests */ false,
        /* pbIsPostponedRequest     */ &bIsPostponedRequest,
        /* pidxPostponedRequest     */ &idxPostponedRequest);

    ZS::System::CMsgReq* pMsgReqInitiator;
    ZS::System::CMsgCon* pMsgConInitiator;

    // If a "main" request (connect, disconnect, change settings) or a postponed request has to be aborted ...
    if( pReq )
    {
        pMsgReqInitiator = pReq->takeInitiatorRequestMessage();
        pMsgConInitiator = pReq->takeInitiatorConfirmationMessage();

        if( !bIsPostponedRequest )
        {
            bool bWasConnected = isConnected();

            pReq = m_pRequestQueue->getRequestInProgress();

            errResultInfo.setAddErrInfoDscr( pReq->getRequestName(true) );

            //if( pReq->getRequest() == ERequestConnect || pReq->getRequest() == ERequestUpdateHostSettings )
            //{
            //    if( m_pGatewayThread != nullptr )
            //    {
            //        if( m_pGatewayThread->isRunning() )
            //        {
            //            m_pGatewayThread->quit();
            //            m_pGatewayThread->wait(10000+hostSettingsCurr.m_iConnectTimeout_ms);
            //        }
            //        if( m_pGatewayThread->isRunning() )
            //        {
            //            m_pGatewayThread->deleteLater();
            //        }
            //        else
            //        {
            //            try
            //            {
            //                delete m_pGatewayThread;
            //            }
            //            catch(...)
            //            {
            //            }
            //        }
            //    }

            //    m_pGatewayThread = nullptr;
            //    m_pGateway = nullptr;

            //} // if( pReq->getRequest() == ERequestConnect || pReq->getRequest() == ERequestUpdateHostSettings )

            m_state = EStateUnconnected;

            // Progress = 100 % indicates "requestFinished".
            // Please note that on emitting the "changed" signal further requests
            // may be started as a reentry by the receiver(s) of the signal.
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();

            if( bWasConnected && !isConnected() )
            {
                emit disconnected(this,m_socketDscr);
            }

            // Invalidate request in progress after emitting signals so that
            // "isRequestInProgress" returns true.
            m_pRequestQueue->setRequestInProgress(nullptr);

        } // if( !bIsPostponedRequest )

        else // if( bIsPostponedRequest )
        {
            // .. remove the request from the list of postponed requests.
            m_pRequestQueue->removePostponedRequestByIdx(idxPostponedRequest);

            // Progress = 100 % indicates "requestFinished".
            // Please note that on emitting the "changed" signal further requests
            // may be started as a reentry by the receiver(s) of the signal.
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();

        } // if( bIsPostponedRequest )

        deleteRequest(pReq);
        pReq = nullptr;

        delete pMsgReqInitiator;
        pMsgReqInitiator = nullptr;

        if( pMsgConInitiator != nullptr )
        {
            pMsgConInitiator->setErrResultInfo(errResultInfo);
            POST_OR_DELETE_MESSAGE(pMsgConInitiator, &mthTracer);
            pMsgConInitiator = nullptr;
        }

    } // if( pReq != nullptr )

    if( !m_bIsBeingDestroyed && !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // abortRequest

//------------------------------------------------------------------------------
void CDbClient::abortRequestInProgress()
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
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    CRequest* pReq = m_pRequestQueue->getRequestInProgress();

    if( pReq != nullptr )
    {
        abortRequest(pReq->getId());
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", " + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.trace(strAddTrcInfo);
    }

} // abortRequestInProgress

//------------------------------------------------------------------------------
void CDbClient::abortAllRequests()
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
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
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

        if( pReq != nullptr )
        {
            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(100);
            pReq->update();

            pMsgReqInitiator = pReq->takeInitiatorRequestMessage();
            pMsgConInitiator = pReq->takeInitiatorConfirmationMessage();

            m_pRequestQueue->removePostponedRequest(pReq);

            deleteRequest(pReq);
            pReq = nullptr;

            delete pMsgReqInitiator;
            pMsgReqInitiator = nullptr;

            if( pMsgConInitiator != nullptr )
            {
                pMsgConInitiator->setErrResultInfo(errResultInfo);
                POST_OR_DELETE_MESSAGE(pMsgConInitiator, &mthTracer);
                pMsgConInitiator = nullptr;
            }
        }

    } // while( !m_arpReqsPostponed.isEmpty() )

    // Abort "main" requests.
    //-------------------------------

    if( m_pRequestQueue->isRequestInProgress() )
    {
        bool bWasConnected = isConnected();

        // Keep the request in request queue until update is called as
        // on updating the request the "onRequestChanged" slot is called
        // which checks whether the request is in the request queue.
        pReq = m_pRequestQueue->getRequestInProgress();

        pMsgReqInitiator = pReq->takeInitiatorRequestMessage();
        pMsgConInitiator = pReq->takeInitiatorConfirmationMessage();

        errResultInfo.setAddErrInfoDscr( pReq->getRequestName(true) );

        //if( pReq->getRequest() == ERequestConnect || pReq->getRequest() == ERequestUpdateHostSettings )
        //{
        //    if( m_pGatewayThread != nullptr )
        //    {
        //        if( m_pGatewayThread->isRunning() )
        //        {
        //            m_pGatewayThread->quit();
        //            m_pGatewayThread->wait(10000+hostSettingsCurr.m_iConnectTimeout_ms);
        //        }
        //        if( m_pGatewayThread->isRunning() )
        //        {
        //            m_pGatewayThread->deleteLater();
        //        }
        //        else
        //        {
        //            try
        //            {
        //                delete m_pGatewayThread;
        //            }
        //            catch(...)
        //            {
        //            }
        //        }
        //    }

        //    m_pGatewayThread = nullptr;
        //    m_pGateway = nullptr;

        //} // if( pReq->getRequest() == ERequestConnect || pReq->getRequest() == ERequestUpdateHostSettings )

        m_state = EStateUnconnected;

        // Progress = 100 % indicates "requestFinished".
        // Please note that on emitting the "changed" signal further requests
        // may be started as a reentry by the receiver(s) of the signal.
        pReq->setErrResultInfo(errResultInfo);
        pReq->setProgressInPerCent(100);
        pReq->update();

        if( bWasConnected && !isConnected() )
        {
            emit disconnected(this,m_socketDscr);
        }

        m_pRequestQueue->removeRequestInProgress(pReq);

        deleteRequest(pReq);
        pReq = nullptr;

        delete pMsgReqInitiator;
        pMsgReqInitiator = nullptr;

        if( pMsgConInitiator != nullptr )
        {
            pMsgConInitiator->setErrResultInfo(errResultInfo);
            POST_OR_DELETE_MESSAGE(pMsgConInitiator, &mthTracer);
            pMsgConInitiator = nullptr;
        }

    } // if( requestInProgress != ERequestNone )

    if( !m_bIsBeingDestroyed && !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        strAddTrcInfo = "State: " + State2Str(m_state);

        if( !m_pRequestQueue->isIdle() )
        {
            CRequest* pReqTmp = m_pRequestQueue->getRequestInProgress();

            if( pReqTmp == nullptr )
            {
                strAddTrcInfo += ", ReqInProgress: nullptr";
            }
            else
            {
                strAddTrcInfo += ", ReqInProgress: " + pReqTmp->getRequestName(true);
                strAddTrcInfo += ", Id: " + QString::number(pReqTmp->getId());
                strAddTrcInfo += ", Creator: " + pReqTmp->getCreatorClassName() + ":" + pReqTmp->getCreatorObjectName();
            }
            if( m_pRequestQueue->hasPostponedRequest() )
            {
                strAddTrcInfo += ", PostponedRequests: " + QString::number(m_pRequestQueue->getPostponedRequestsCount());
            }
        }

        mthTracer.setMethodReturn(strAddTrcInfo);
    }

} // abortAllRequests

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::startup(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startup",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".startup";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    //abortAllExecCommandRequests();

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
        else // if( !pReq->isBlockingRequest() )
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
        } // if( !pReq->isBlockingRequest() )
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        // The "execute<Request>" method connects the "onRequestChanged" slot and sets
        // the request to be deleted later if the request is not asynchronous.
        executeStartupRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

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
        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // startup

//------------------------------------------------------------------------------
CRequest* CDbClient::shutdown(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "shutdown",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".shutdown";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    abortAllRequests();

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
        else // if( !pReq->isBlockingRequest() )
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
        } // if( !pReq->isBlockingRequest() )
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        // The "execute<Request>" method connects the "onRequestChanged" slot and sets
        // the request to be deleted later if the request is not asynchronous.
        executeShutdownRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

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
        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // shutdown

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::connect_(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "connect_",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".connect";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    //abortAllExecCommandRequests();

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestConnect,
        /* strRequest   */ Request2Str(ERequestConnect),
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
        else // if( !pReq->isBlockingRequest() )
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
        } // if( !pReq->isBlockingRequest() )
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        // The "execute<Request>" method connects the "onRequestChanged" slot and sets
        // the request to be deleted later if the request is not asynchronous.
        executeConnectRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

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
        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // connect_

//------------------------------------------------------------------------------
CRequest* CDbClient::disconnect_(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "disconnect_",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".disconnect";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    //abortAllRequests();

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestDisconnect,
        /* strRequest   */ Request2Str(ERequestDisconnect),
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
        else // if( !pReq->isBlockingRequest() )
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
        } // if( !pReq->isBlockingRequest() )
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeDisconnectRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

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
        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // disconnect_

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::updateHostSettings(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateHostSettings",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".updateHostSettings";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    //abortAllExecCommandRequests();

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
        else // if( !pReq->isBlockingRequest() )
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
        } // if( !pReq->isBlockingRequest() )
    } // if( bPostponeRequest )

    else // if( !bPostponeRequest )
    {
        // The "execute<Request>" method connects the "onRequestChanged" slot and sets
        // the request to be deleted later if the request is not asynchronous.
        executeUpdateHostSettingsRequest(pReq);
    }

    // Please note that if executing this request did not become asynchronous the
    // "execute<>Request" method above has been called and the "onRequestChanged"
    // slot method has already been disconnected from the request's changed signal.

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
        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;

} // updateHostSettings

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//CRequest* CDbClient::register_(
//    int    i_iTimeout_ms,
//    bool   i_bWait,
//    qint64 i_iReqIdParent )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
//        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
//        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "register",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".register";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    // Initialized as an asynchronous request (ResultUndefined).
//    CRequest* pReq = m_pRequestQueue->createRequest(
//        /* strNameSpace */ nameSpace(),
//        /* strClassName */ className(),
//        /* iRequest     */ ERequestRegister,
//        /* strRequest   */ Request2Str(ERequestRegister),
//        /* iReqIdParent */ i_iReqIdParent,
//        /* iTimeout_ms  */ i_iTimeout_ms,
//        /* bIsBlocking  */ i_bWait );
//
//    // Please note that the "onRequestChanged" slot should also be connected
//    // to the changed signal of the request if the request will be postponed
//    // as postponed requests may have parent request which may be aborted.
//    if( !QObject::connect(
//        /* pObjSender   */ pReq,
//        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//        /* pObjReceiver */ this,
//        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//    {
//        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//    }
//
//    CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();
//
//    bool bPostponeRequest = true;
//
//    // The request can be executed immediately if the request queue is idle (no request
//    // in progress, queue not locked and no requests postponed) or
//    // if the request method has been started as a child of the request in progress
//    // (e.g. as part of a request sequence) ..
//    if( m_pRequestQueue->isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
//    {
//        bPostponeRequest = false;
//    }
//
//    if( bPostponeRequest )
//    {
//        if( pReq->isBlockingRequest() )
//        {
//            errResultInfo.setSeverity(EResultSeverityError);
//            errResultInfo.setResult(EResultRequestRefused);
//            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );
//
//            if( CErrLog::GetInstance() != nullptr )
//            {
//                CErrLog::GetInstance()->addEntry(errResultInfo);
//            }
//
//            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
//            pReq->setErrResultInfo(errResultInfo);
//            pReq->setProgressInPerCent(100);
//            pReq->update();
//            //m_pRequestQueue->unlock();
//
//            // The request is finished and has to be deleted later.
//            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//        }
//        else // if( !pReq->isBlockingRequest() )
//        {
//            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);
//
//            if( resultPostpone != EResultSuccess )
//            {
//                pReq->setSeverity(EResultSeverityError);
//                pReq->setResult(resultPostpone);
//                pReq->setAddErrInfoDscr( "Request could not have been queued." );
//                pReq->setProgressInPerCent(100);
//                pReq->update();
//
//                // The request is finished and has to be deleted later.
//                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//            }
//        } // if( !pReq->isBlockingRequest() )
//    } // if( bPostponeRequest )
//
//    else // if( !bPostponeRequest )
//    {
//        // Please note that the request has either set as the request in progress,
//        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
//        executeRegisterRequest( pReq, pReqInProgress );
//    }
//
//    // Please note that if executing this request did not become asynchronous the
//    // "execute<>Request" method above has been called and the "onRequestChanged"
//    // slot method has already been disconnected from the request's changed signal.
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        mthTracer.setMethodReturn(strAddTrcInfo);
//    }
//
//    return pReq;
//
//} // register_
//
////------------------------------------------------------------------------------
//CRequest* CDbClient::unregister(
//    int    i_iTimeout_ms,
//    bool   i_bWait,
//    qint64 i_iReqIdParent )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
//        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
//        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "unregister",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".unregister";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    // Initialized as an asynchronous request (ResultUndefined).
//    CRequest* pReq = m_pRequestQueue->createRequest(
//        /* strNameSpace */ nameSpace(),
//        /* strClassName */ className(),
//        /* iRequest     */ ERequestUnregister,
//        /* strRequest   */ Request2Str(ERequestUnregister),
//        /* iReqIdParent */ i_iReqIdParent,
//        /* iTimeout_ms  */ i_iTimeout_ms,
//        /* bIsBlocking  */ i_bWait );
//
//    // Please note that the "onRequestChanged" slot should also be connected
//    // to the changed signal of the request if the request will be postponed
//    // as postponed requests may have parent request which may be aborted.
//    if( !QObject::connect(
//        /* pObjSender   */ pReq,
//        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//        /* pObjReceiver */ this,
//        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//    {
//        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//    }
//
//    CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();
//
//    bool bPostponeRequest = true;
//
//    // The request can be executed immediately if the request queue is idle (no request
//    // in progress, queue not locked and no requests postponed) or
//    // if the request method has been started as a child of the request in progress
//    // (e.g. as part of a request sequence) ..
//    if( m_pRequestQueue->isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
//    {
//        bPostponeRequest = false;
//    }
//
//    if( bPostponeRequest )
//    {
//        if( pReq->isBlockingRequest() )
//        {
//            errResultInfo.setSeverity(EResultSeverityError);
//            errResultInfo.setResult(EResultRequestRefused);
//            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );
//
//            if( CErrLog::GetInstance() != nullptr )
//            {
//                CErrLog::GetInstance()->addEntry(errResultInfo);
//            }
//
//            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
//            pReq->setErrResultInfo(errResultInfo);
//            pReq->setProgressInPerCent(100);
//            pReq->update();
//            //m_pRequestQueue->unlock();
//
//            // The request is finished and has to be deleted later.
//            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//        }
//        else // if( !pReq->isBlockingRequest() )
//        {
//            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);
//
//            if( resultPostpone != EResultSuccess )
//            {
//                pReq->setSeverity(EResultSeverityError);
//                pReq->setResult(resultPostpone);
//                pReq->setAddErrInfoDscr( "Request could not have been queued." );
//                pReq->setProgressInPerCent(100);
//                pReq->update();
//
//                // The request is finished and has to be deleted later.
//                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//            }
//        } // if( !pReq->isBlockingRequest() )
//    } // if( bPostponeRequest )
//
//    else // if( !bPostponeRequest )
//    {
//        // Please note that the request has either set as the request in progress,
//        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
//        executeUnregisterRequest( pReq, pReqInProgress );
//    }
//
//    // Please note that if executing this request did not become asynchronous the
//    // "execute<>Request" method above has been called and the "onRequestChanged"
//    // slot method has already been disconnected from the request's changed signal.
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        mthTracer.setMethodReturn(strAddTrcInfo);
//    }
//
//    return pReq;
//
//} // unregister

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//CRequest* CDbClient::selectSchema(
//    int    i_iTimeout_ms,
//    bool   i_bWait,
//    qint64 i_iReqIdParent )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
//        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
//        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "selectSchema",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".selectSchema";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    // Initialized as an asynchronous request (ResultUndefined).
//    CRequest* pReq = m_pRequestQueue->createRequest(
//        /* strNameSpace */ nameSpace(),
//        /* strClassName */ className(),
//        /* iRequest     */ ERequestSelectSchema,
//        /* strRequest   */ Request2Str(ERequestSelectSchema),
//        /* iReqIdParent */ i_iReqIdParent,
//        /* iTimeout_ms  */ i_iTimeout_ms,
//        /* bIsBlocking  */ i_bWait );
//
//    // Please note that the "onRequestChanged" slot should also be connected
//    // to the changed signal of the request if the request will be postponed
//    // as postponed requests may have parent request which may be aborted.
//    if( !QObject::connect(
//        /* pObjSender   */ pReq,
//        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//        /* pObjReceiver */ this,
//        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//    {
//        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//    }
//
//    CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();
//
//    bool bPostponeRequest = true;
//
//    // The request can be executed immediately if the request queue is idle (no request
//    // in progress, queue not locked and no requests postponed) or
//    // if the request method has been started as a child of the request in progress
//    // (e.g. as part of a request sequence) ..
//    if( m_pRequestQueue->isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
//    {
//        bPostponeRequest = false;
//    }
//
//    if( bPostponeRequest )
//    {
//        if( pReq->isBlockingRequest() )
//        {
//            errResultInfo.setSeverity(EResultSeverityError);
//            errResultInfo.setResult(EResultRequestRefused);
//            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );
//
//            if( CErrLog::GetInstance() != nullptr )
//            {
//                CErrLog::GetInstance()->addEntry(errResultInfo);
//            }
//
//            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
//            pReq->setErrResultInfo(errResultInfo);
//            pReq->setProgressInPerCent(100);
//            pReq->update();
//            //m_pRequestQueue->unlock();
//
//            // The request is finished and has to be deleted later.
//            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//        }
//        else // if( !pReq->isBlockingRequest() )
//        {
//            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);
//
//            if( resultPostpone != EResultSuccess )
//            {
//                pReq->setSeverity(EResultSeverityError);
//                pReq->setResult(resultPostpone);
//                pReq->setAddErrInfoDscr( "Request could not have been queued." );
//                pReq->setProgressInPerCent(100);
//                pReq->update();
//
//                // The request is finished and has to be deleted later.
//                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//            }
//        } // if( !pReq->isBlockingRequest() )
//    } // if( bPostponeRequest )
//
//    else // if( !bPostponeRequest )
//    {
//        // Please note that the request has either set as the request in progress,
//        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
//        executeSelectSchemaRequest( pReq, pReqInProgress );
//    }
//
//    // Please note that if executing this request did not become asynchronous the
//    // "execute<>Request" method above has been called and the "onRequestChanged"
//    // slot method has already been disconnected from the request's changed signal.
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        mthTracer.setMethodReturn(strAddTrcInfo);
//    }
//
//    return pReq;
//
//} // selectSchema
//
////------------------------------------------------------------------------------
//CRequest* CDbClient::updateSchema(
//    int    i_iTimeout_ms,
//    bool   i_bWait,
//    qint64 i_iReqIdParent )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
//        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
//        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "updateSchema",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".updateSchema";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    // Initialized as an asynchronous request (ResultUndefined).
//    CRequest* pReq = m_pRequestQueue->createRequest(
//        /* strNameSpace */ nameSpace(),
//        /* strClassName */ className(),
//        /* iRequest     */ ERequestUpdateSchema,
//        /* strRequest   */ Request2Str(ERequestUpdateSchema),
//        /* iReqIdParent */ i_iReqIdParent,
//        /* iTimeout_ms  */ i_iTimeout_ms,
//        /* bIsBlocking  */ i_bWait );
//
//    // Please note that the "onRequestChanged" slot should also be connected
//    // to the changed signal of the request if the request will be postponed
//    // as postponed requests may have parent request which may be aborted.
//    if( !QObject::connect(
//        /* pObjSender   */ pReq,
//        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//        /* pObjReceiver */ this,
//        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//    {
//        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//    }
//
//    CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();
//
//    bool bPostponeRequest = true;
//
//    // The request can be executed immediately if the request queue is idle (no request
//    // in progress, queue not locked and no requests postponed) or
//    // if the request method has been started as a child of the request in progress
//    // (e.g. as part of a request sequence) ..
//    if( m_pRequestQueue->isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
//    {
//        bPostponeRequest = false;
//    }
//
//    if( bPostponeRequest )
//    {
//        if( pReq->isBlockingRequest() )
//        {
//            errResultInfo.setSeverity(EResultSeverityError);
//            errResultInfo.setResult(EResultRequestRefused);
//            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );
//
//            if( CErrLog::GetInstance() != nullptr )
//            {
//                CErrLog::GetInstance()->addEntry(errResultInfo);
//            }
//
//            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
//            pReq->setErrResultInfo(errResultInfo);
//            pReq->setProgressInPerCent(100);
//            pReq->update();
//            //m_pRequestQueue->unlock();
//
//            // The request is finished and has to be deleted later.
//            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//        }
//        else // if( !pReq->isBlockingRequest() )
//        {
//            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);
//
//            if( resultPostpone != EResultSuccess )
//            {
//                pReq->setSeverity(EResultSeverityError);
//                pReq->setResult(resultPostpone);
//                pReq->setAddErrInfoDscr( "Request could not have been queued." );
//                pReq->setProgressInPerCent(100);
//                pReq->update();
//
//                // The request is finished and has to be deleted later.
//                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//            }
//        } // if( !pReq->isBlockingRequest() )
//    } // if( bPostponeRequest )
//
//    else // if( !bPostponeRequest )
//    {
//        // Please note that the request has either set as the request in progress,
//        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
//        executeUpdateSchemaRequest( pReq, pReqInProgress );
//    }
//
//    // Please note that if executing this request did not become asynchronous the
//    // "execute<>Request" method above has been called and the "onRequestChanged"
//    // slot method has already been disconnected from the request's changed signal.
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        mthTracer.setMethodReturn(strAddTrcInfo);
//    }
//
//    return pReq;
//
//} // updateSchema

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//CRequest* CDbClient::selectData(
//    int    i_iTimeout_ms,
//    bool   i_bWait,
//    qint64 i_iReqIdParent )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
//        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
//        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "selectData",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".selectData";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    // Initialized as an asynchronous request (ResultUndefined).
//    CRequest* pReq = m_pRequestQueue->createRequest(
//        /* strNameSpace */ nameSpace(),
//        /* strClassName */ className(),
//        /* iRequest     */ ERequestSelectData,
//        /* strRequest   */ Request2Str(ERequestSelectData),
//        /* iReqIdParent */ i_iReqIdParent,
//        /* iTimeout_ms  */ i_iTimeout_ms,
//        /* bIsBlocking  */ i_bWait );
//
//    // Please note that the "onRequestChanged" slot should also be connected
//    // to the changed signal of the request if the request will be postponed
//    // as postponed requests may have parent request which may be aborted.
//    if( !QObject::connect(
//        /* pObjSender   */ pReq,
//        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//        /* pObjReceiver */ this,
//        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//    {
//        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//    }
//
//    CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();
//
//    bool bPostponeRequest = true;
//
//    // The request can be executed immediately if the request queue is idle (no request
//    // in progress, queue not locked and no requests postponed) or
//    // if the request method has been started as a child of the request in progress
//    // (e.g. as part of a request sequence) ..
//    if( m_pRequestQueue->isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
//    {
//        bPostponeRequest = false;
//    }
//
//    if( bPostponeRequest )
//    {
//        if( pReq->isBlockingRequest() )
//        {
//            errResultInfo.setSeverity(EResultSeverityError);
//            errResultInfo.setResult(EResultRequestRefused);
//            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );
//
//            if( CErrLog::GetInstance() != nullptr )
//            {
//                CErrLog::GetInstance()->addEntry(errResultInfo);
//            }
//
//            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
//            pReq->setErrResultInfo(errResultInfo);
//            pReq->setProgressInPerCent(100);
//            pReq->update();
//            //m_pRequestQueue->unlock();
//
//            // The request is finished and has to be deleted later.
//            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//        }
//        else // if( !pReq->isBlockingRequest() )
//        {
//            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);
//
//            if( resultPostpone != EResultSuccess )
//            {
//                pReq->setSeverity(EResultSeverityError);
//                pReq->setResult(resultPostpone);
//                pReq->setAddErrInfoDscr( "Request could not have been queued." );
//                pReq->setProgressInPerCent(100);
//                pReq->update();
//
//                // The request is finished and has to be deleted later.
//                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//            }
//        } // if( !pReq->isBlockingRequest() )
//    } // if( bPostponeRequest )
//
//    else // if( !bPostponeRequest )
//    {
//        // Please note that the request has either set as the request in progress,
//        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
//        executeSelectDataRequest( pReq, pReqInProgress );
//    }
//
//    // Please note that if executing this request did not become asynchronous the
//    // "execute<>Request" method above has been called and the "onRequestChanged"
//    // slot method has already been disconnected from the request's changed signal.
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        mthTracer.setMethodReturn(strAddTrcInfo);
//    }
//
//    return pReq;
//
//} // selectData
//
////------------------------------------------------------------------------------
//CRequest* CDbClient::updateData(
//    int    i_iTimeout_ms,
//    bool   i_bWait,
//    qint64 i_iReqIdParent )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
//        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
//        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "updateData",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive() )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".updateData";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    // Initialized as an asynchronous request (ResultUndefined).
//    CRequest* pReq = m_pRequestQueue->createRequest(
//        /* strNameSpace */ nameSpace(),
//        /* strClassName */ className(),
//        /* iRequest     */ ERequestUpdateData,
//        /* strRequest   */ Request2Str(ERequestUpdateData),
//        /* iReqIdParent */ i_iReqIdParent,
//        /* iTimeout_ms  */ i_iTimeout_ms,
//        /* bIsBlocking  */ i_bWait );
//
//    // Please note that the "onRequestChanged" slot should also be connected
//    // to the changed signal of the request if the request will be postponed
//    // as postponed requests may have parent request which may be aborted.
//    if( !QObject::connect(
//        /* pObjSender   */ pReq,
//        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//        /* pObjReceiver */ this,
//        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//    {
//        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//    }
//
//    CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();
//
//    bool bPostponeRequest = true;
//
//    // The request can be executed immediately if the request queue is idle (no request
//    // in progress, queue not locked and no requests postponed) or
//    // if the request method has been started as a child of the request in progress
//    // (e.g. as part of a request sequence) ..
//    if( m_pRequestQueue->isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
//    {
//        bPostponeRequest = false;
//    }
//
//    if( bPostponeRequest )
//    {
//        if( pReq->isBlockingRequest() )
//        {
//            errResultInfo.setSeverity(EResultSeverityError);
//            errResultInfo.setResult(EResultRequestRefused);
//            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );
//
//            if( CErrLog::GetInstance() != nullptr )
//            {
//                CErrLog::GetInstance()->addEntry(errResultInfo);
//            }
//
//            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
//            pReq->setErrResultInfo(errResultInfo);
//            pReq->setProgressInPerCent(100);
//            pReq->update();
//            //m_pRequestQueue->unlock();
//
//            // The request is finished and has to be deleted later.
//            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//        }
//        else // if( !pReq->isBlockingRequest() )
//        {
//            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);
//
//            if( resultPostpone != EResultSuccess )
//            {
//                pReq->setSeverity(EResultSeverityError);
//                pReq->setResult(resultPostpone);
//                pReq->setAddErrInfoDscr( "Request could not have been queued." );
//                pReq->setProgressInPerCent(100);
//                pReq->update();
//
//                // The request is finished and has to be deleted later.
//                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//            }
//        } // if( !pReq->isBlockingRequest() )
//    } // if( bPostponeRequest )
//
//    else // if( !bPostponeRequest )
//    {
//        // Please note that the request has either set as the request in progress,
//        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
//        executeUpdateDataRequest( pReq, pReqInProgress );
//    }
//
//    // Please note that if executing this request did not become asynchronous the
//    // "execute<>Request" method above has been called and the "onRequestChanged"
//    // slot method has already been disconnected from the request's changed signal.
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        mthTracer.setMethodReturn(strAddTrcInfo);
//    }
//
//    return pReq;
//
//} // updateData

/*==============================================================================
public: // overridables
==============================================================================*/

////------------------------------------------------------------------------------
//CRequest* CDbClient::connectAndRegister(
//    int    i_iTimeout_ms,
//    bool   i_bWait,
//    qint64 i_iReqIdParent )
////------------------------------------------------------------------------------
//{
//    QMutexLocker mtxLocker(m_pMtx);
//
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
//        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
//        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "connectAndRegister",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".connectAndRegister";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    //abortAllExecCommandRequests();
//
//    // Initialized as an asynchronous request (ResultUndefined).
//    CRequest* pReq = m_pRequestQueue->createRequest(
//        /* strNameSpace */ nameSpace(),
//        /* strClassName */ className(),
//        /* iRequest     */ ERequestConnectAndRegister,
//        /* strRequest   */ Request2Str(ERequestConnectAndRegister),
//        /* iReqIdParent */ i_iReqIdParent,
//        /* iTimeout_ms  */ i_iTimeout_ms,
//        /* bIsBlocking  */ i_bWait );
//
//    // Please note that the "onRequestChanged" slot should also be connected
//    // to the changed signal of the request if the request will be postponed
//    // as postponed requests may have parent request which may be aborted.
//    if( !QObject::connect(
//        /* pObjSender   */ pReq,
//        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//        /* pObjReceiver */ this,
//        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//    {
//        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//    }
//
//    CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();
//
//    bool bPostponeRequest = true;
//
//    // The request can be executed immediately if the request queue is idle (no request
//    // in progress, queue not locked and no requests postponed) or
//    // if the request method has been started as a child of the request in progress
//    // (e.g. as part of a request sequence) ..
//    if( m_pRequestQueue->isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
//    {
//        bPostponeRequest = false;
//    }
//
//    if( bPostponeRequest )
//    {
//        if( pReq->isBlockingRequest() )
//        {
//            errResultInfo.setSeverity(EResultSeverityError);
//            errResultInfo.setResult(EResultRequestRefused);
//            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );
//
//            if( CErrLog::GetInstance() != nullptr )
//            {
//                CErrLog::GetInstance()->addEntry(errResultInfo);
//            }
//
//            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
//            pReq->setErrResultInfo(errResultInfo);
//            pReq->setProgressInPerCent(100);
//            pReq->update();
//            //m_pRequestQueue->unlock();
//
//            // The request is finished and has to be deleted later.
//            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//        }
//        else // if( !pReq->isBlockingRequest() )
//        {
//            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);
//
//            if( resultPostpone != EResultSuccess )
//            {
//                pReq->setSeverity(EResultSeverityError);
//                pReq->setResult(resultPostpone);
//                pReq->setAddErrInfoDscr( "Request could not have been queued." );
//                pReq->setProgressInPerCent(100);
//                pReq->update();
//
//                // The request is finished and has to be deleted later.
//                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//            }
//        } // if( !pReq->isBlockingRequest() )
//    } // if( bPostponeRequest )
//
//    else // if( !bPostponeRequest )
//    {
//        // Please note that the request has either set as the request in progress,
//        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
//        executeConnectAndRegisterRequest( pReq, pReqInProgress );
//    }
//
//    // Please note that if executing this request did not become asynchronous the
//    // "execute<>Request" method above has been called and the "onRequestChanged"
//    // slot method has already been disconnected from the request's changed signal.
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        mthTracer.setMethodReturn(strAddTrcInfo);
//    }
//
//    return pReq;
//
//} // connectAndRegister
//
////------------------------------------------------------------------------------
//CRequest* CDbClient::disconnectAndUnregister(
//    int    i_iTimeout_ms,
//    bool   i_bWait,
//    qint64 i_iReqIdParent )
////------------------------------------------------------------------------------
//{
//    QMutexLocker mtxLocker(m_pMtx);
//
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Timeout_ms: " + QString::number(i_iTimeout_ms) + " ms";
//        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
//        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent,iAddTrcInfoDetailLevel) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "disconnectAndUnregister",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".disconnectAndUnregister";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    //abortAllExecCommandRequests();
//
//    // Initialized as an asynchronous request (ResultUndefined).
//    CRequest* pReq = m_pRequestQueue->createRequest(
//        /* strNameSpace */ nameSpace(),
//        /* strClassName */ className(),
//        /* iRequest     */ ERequestDisconnectAndUnregister,
//        /* strRequest   */ Request2Str(ERequestDisconnectAndUnregister),
//        /* iReqIdParent */ i_iReqIdParent,
//        /* iTimeout_ms  */ i_iTimeout_ms,
//        /* bIsBlocking  */ i_bWait );
//
//    // Please note that the "onRequestChanged" slot should also be connected
//    // to the changed signal of the request if the request will be postponed
//    // as postponed requests may have parent request which may be aborted.
//    if( !QObject::connect(
//        /* pObjSender   */ pReq,
//        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//        /* pObjReceiver */ this,
//        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//    {
//        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//    }
//
//    CRequest* pReqInProgress = m_pRequestQueue->getRequestInProgress();
//
//    bool bPostponeRequest = true;
//
//    // The request can be executed immediately if the request queue is idle (no request
//    // in progress, queue not locked and no requests postponed) or
//    // if the request method has been started as a child of the request in progress
//    // (e.g. as part of a request sequence) ..
//    if( m_pRequestQueue->isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
//    {
//        bPostponeRequest = false;
//    }
//
//    if( bPostponeRequest )
//    {
//        if( pReq->isBlockingRequest() )
//        {
//            errResultInfo.setSeverity(EResultSeverityError);
//            errResultInfo.setResult(EResultRequestRefused);
//            errResultInfo.setAddErrInfoDscr( "Cannot wait if request is currently in progress. Don't mix blocking calls with non blocking calls." );
//
//            if( CErrLog::GetInstance() != nullptr )
//            {
//                CErrLog::GetInstance()->addEntry(errResultInfo);
//            }
//
//            //m_pRequestQueue->lock(); // Not necessary as request queue is not idle.
//            pReq->setErrResultInfo(errResultInfo);
//            pReq->setProgressInPerCent(100);
//            pReq->update();
//            //m_pRequestQueue->unlock();
//
//            // The request is finished and has to be deleted later.
//            m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//        }
//        else // if( !pReq->isBlockingRequest() )
//        {
//            EResult resultPostpone = m_pRequestQueue->postponeRequest(pReq);
//
//            if( resultPostpone != EResultSuccess )
//            {
//                pReq->setSeverity(EResultSeverityError);
//                pReq->setResult(resultPostpone);
//                pReq->setAddErrInfoDscr( "Request could not have been queued." );
//                pReq->setProgressInPerCent(100);
//                pReq->update();
//
//                // The request is finished and has to be deleted later.
//                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReq);
//            }
//        } // if( !pReq->isBlockingRequest() )
//    } // if( bPostponeRequest )
//
//    else // if( !bPostponeRequest )
//    {
//        // Please note that the request has either set as the request in progress,
//        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
//        executeDisconnectAndUnregisterRequest( pReq, pReqInProgress );
//    }
//
//    // Please note that if executing this request did not become asynchronous the
//    // "execute<>Request" method above has been called and the "onRequestChanged"
//    // slot method has already been disconnected from the request's changed signal.
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "Request {" + QString( pReq == nullptr ? "nullptr" : pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        mthTracer.setMethodReturn(strAddTrcInfo);
//    }
//
//    return pReq;
//
//} // disconnectAndUnregister

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::addTrcMsgLogObject( QObject* i_pObj )
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

    if( m_pGateway != nullptr )
    {
        m_pGateway->addTrcMsgLogObject(i_pObj);
    }

} // addTrcMsgLogObject

//------------------------------------------------------------------------------
void CDbClient::removeTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    if( i_pObj == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObj == nullptr");
    }

    QObject* pObj;
    int      idxObj = -1;

    if( m_arpTrcMsgLogObjects.size() > 0 )
    {
        for( idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
        {
            pObj = m_arpTrcMsgLogObjects[idxObj];

            if( pObj == i_pObj )
            {
                break;
            }
        }
    }
    if( idxObj < 0 || idxObj > m_arpTrcMsgLogObjects.size() )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    m_arpTrcMsgLogObjects.removeAt(idxObj);

    if( m_pGateway != nullptr )
    {
        m_pGateway->removeTrcMsgLogObject(i_pObj);
    }

} // removeTrcMsgLogObject

/*==============================================================================
protected: // instance methods of internal state machine
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::executeNextPostponedRequest()
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
        /* strMethod    */ "executeNextPostponedRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
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

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // executeNextPostponedRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::executeStartupRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
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

    // If the gateway thread has not yet been started ...
    if( m_state == EStateIdle )
    {
        createGatewayThread();

        // StartGatewayThreads is a blocking request
        startGatewayThread(10000, i_pReq->getId());

        // StartupGateway is also a blocking request
        startupGateway( i_pReq->getTimeoutInMs(), i_pReq->getId() );

    } // if( m_state == EStateIdle )

    else // if( m_state != EStateIdle )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Server is already running." );

    } // if( m_state != EStateIdle )

    acceptChangedSettings();

    i_pReq->setErrResultInfo(errResultInfo);
    i_pReq->setProgressInPerCent(iProgress_perCent);
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
void CDbClient::executeShutdownRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
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

    if( m_state == EStateIdle )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is not started." );

    } // if( m_state == EStateIdle )

    else if( m_state == EStateUnconnected )
    {
        // ShutdownGateway is a blocking request
        shutdownGateway( i_pReq->getTimeoutInMs(), i_pReq->getId() );

        // StopGatewayThread is also a blocking request
        stopGatewayThread( i_pReq->getTimeoutInMs(), i_pReq->getId() );

    } // if( m_state == EStateUnconnected )

    else if( m_state == EStateConnected )
    {
        // ShutdownGateway is a blocking request
        shutdownGateway( i_pReq->getTimeoutInMs(), i_pReq->getId() );

        // StopGatewayThread is also a blocking request
        stopGatewayThread( i_pReq->getTimeoutInMs(), i_pReq->getId() );

    } // if( m_state == EStateConnected )

    else if( m_state == EStateRegistered )
    {
        // ShutdownGateway is a blocking request
        shutdownGateway( i_pReq->getTimeoutInMs(), i_pReq->getId() );

        // StopGatewayThread is also a blocking request
        stopGatewayThread( i_pReq->getTimeoutInMs(), i_pReq->getId() );

    } // if( m_state == EStateRegistered )

    i_pReq->setErrResultInfo(errResultInfo);
    i_pReq->setProgressInPerCent(iProgress_perCent);
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

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::executeConnectRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeConnectRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeConnectRequest";

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

    //bool bWasConnected = isConnected();

    // If the gateway thread has not yet been started ...
    if( m_state == EStateIdle )
    {
        createGatewayThread();

        // StartGatewayThreads is a blocking request
        startGatewayThread(10000, i_pReq->getId());

        // StartupGateway is also a blocking request
        startupGateway(10000, i_pReq->getId());

        CRequest* pReqConnectGatway = connectGateway(
            /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
            /* bWait        */ i_pReq->isBlockingRequest(),
            /* iReqIdParent */ i_pReq->getId() );

        if( isAsynchronousRequest(pReqConnectGatway) )
        {
            errResultInfo = pReqConnectGatway->getErrResultInfo();
            iProgress_perCent = 0;
        }
        else // if( !isAsynchronousRequest(pReqConnectGatway) )
        {
            if( pReqConnectGatway != nullptr )
            {
                errResultInfo = pReqConnectGatway->getErrResultInfo();
            }
            else
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }
        }
    } // if( m_state == EStateIdle )

    else if( m_state == EStateUnconnected )
    {
        CRequest* pReqConnectGatway = connectGateway(
            /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
            /* bWait        */ i_pReq->isBlockingRequest(),
            /* iReqIdParent */ i_pReq->getId() );

        if( isAsynchronousRequest(pReqConnectGatway) )
        {
            errResultInfo = pReqConnectGatway->getErrResultInfo();
            iProgress_perCent = 0;
        }
        else // if( !isAsynchronousRequest(pReqConnectGatway) )
        {
            if( pReqConnectGatway != nullptr )
            {
                errResultInfo = pReqConnectGatway->getErrResultInfo();
            }
            else
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }
        }
    } // if( m_state == EStateUnconnected )

    else if( m_state == EStateConnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is already connected." );

    } // if( m_state == EStateConnected )

    else if( m_state == EStateRegistered )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is already connected." );

    } // if( m_state == EStateRegistered )

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
        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
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

} // executeConnectRequest

//------------------------------------------------------------------------------
void CDbClient::executeDisconnectRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeDisconnectRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeDisconnectRequest";

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

    if( m_state == EStateIdle )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is not started." );

    } // if( m_state == EStateIdle )

    else if( m_state == EStateUnconnected )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Client is not connected." );

    } // if( m_state == EStateUnconnected )

    else if( m_state == EStateConnected )
    {
        CRequest* pReqDisconnectGatway = disconnectGateway(
            /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
            /* bWait        */ i_pReq->isBlockingRequest(),
            /* iReqIdParent */ i_pReq->getId() );

        if( isAsynchronousRequest(pReqDisconnectGatway) )
        {
            errResultInfo = pReqDisconnectGatway->getErrResultInfo();
            iProgress_perCent = 0;
        }
        else // if( !isAsynchronousRequest(pReqDisconnectGatway) )
        {
            if( pReqDisconnectGatway != nullptr )
            {
                errResultInfo = pReqDisconnectGatway->getErrResultInfo();
            }
            else
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }
        }
    } // if( m_state == EStateConnected )

    else if( m_state == EStateRegistered )
    {
        CRequest* pReqUnregisterGatway = unregisterGateway(
            /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
            /* bWait        */ i_pReq->isBlockingRequest(),
            /* iReqIdParent */ i_pReq->getId() );

        if( isAsynchronousRequest(pReqUnregisterGatway) )
        {
            errResultInfo = pReqUnregisterGatway->getErrResultInfo();
            iProgress_perCent = 0;
        }
        else // if( !isAsynchronousRequest(pReqUnregisterGatway) )
        {
            if( pReqUnregisterGatway != nullptr )
            {
                errResultInfo = pReqUnregisterGatway->getErrResultInfo();
            }
            if( errResultInfo.isErrorResult() )
            {
                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }

            CRequest* pReqDisconnectGatway = disconnectGateway(
                /* iTimeout_ms  */ i_pReq->getTimeoutInMs(),
                /* bWait        */ i_pReq->isBlockingRequest(),
                /* iReqIdParent */ i_pReq->getId() );

            if( isAsynchronousRequest(pReqDisconnectGatway) )
            {
                errResultInfo = pReqDisconnectGatway->getErrResultInfo();
                iProgress_perCent = 0;
            }
            else // if( !isAsynchronousRequest(pReqDisconnectGatway) )
            {
                if( pReqDisconnectGatway != nullptr )
                {
                    errResultInfo = pReqDisconnectGatway->getErrResultInfo();
                }
                else
                {
                    errResultInfo.setErrResult(ErrResultSuccess);
                }
            }
        } // if( !isAsynchronousRequest(pReqUnregisterGatway) )
    } // if( m_state == EStateRegistered )

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
        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
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

} // executeDisconnectRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::executeUpdateHostSettingsRequest( CRequest* i_pReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
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
            QHash<SCnctType,ZS::Ipc::SClientHostSettings>::const_iterator itHostSettings;

            SClientHostSettings hostSettings;
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

    bool bHasChanges = (m_protocolTypeCurr != EProtocolTypeUndefined)
                    || (m_socketTypeCurr != ESocketTypeUndefined)
                    || !m_hshHostSettingsCurr.isEmpty()
                    || !m_hshWatchDogSettingsCurr.isEmpty();

    // If there are no changes to be updated ..
    if( !bHasChanges )
    {
        errResultInfo.setResult(EResultSuccess);

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( !bHasChanges )

    // If there are changes to be updated ..
    else // if( bHasChanges )
    {
        SErrResultInfo errResultInfoTmp = ErrResultInfoSuccess(strMth);

        //QHash<SCnctType,ZS::Ipc::SClientHostSettings>::iterator itCnct;

        EProtocolType       protType = getProtocolType();
        ESocketType         sockType = getSocketType();
        SClientHostSettings hostSettings = getHostSettings();
        STimerSettings      watchDogSettings = getWatchDogSettings();
        SCnctType           cnctType(protType, sockType);

        CRequest*              pReqUpdateGateway;
        CMsgReqChangeSettings* pMsgReq;
        //bool                   bAsynchronous = false;

        if( m_pGateway != nullptr )
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
                /* pObjReceiver     */ m_pGateway,
                /* protocolType     */ cnctType.m_protocolType,
                /* hostSettings     */ hostSettings,
                /* watchDogSettings */ watchDogSettings,
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
                iProgress_perCent = 0;

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
    } // if( bHasChanges )

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
        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
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
            QHash<SCnctType,ZS::Ipc::SClientHostSettings>::const_iterator itHostSettings;

            SClientHostSettings hostSettings;
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
protected: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CDbClient::executeRegisterRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        strAddTrcInfo += ", ReqMainInProgress {" + QString( i_pReqMainInProgress == nullptr ? "nullptr" : i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "executeRegisterRequest",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeRegisterRequest";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    QString strAddErrInfo;
//
//    int iProgress_perCent = 100;
//
//    i_pReq->lock();
//
//    m_pRequestQueue->lock();
//
//    EState statePrev = m_state;
//
//    SRequestDscr reqDscrPrev;
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrPrev = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    // If started as child request of request in progress (as part of a request sequence) ..
//    if( i_pReqMainInProgress != nullptr )
//    {
//        m_pRequestQueue->addRequestInProgress(i_pReq);
//    }
//    // If not started as child request of request in progress (as main request) ..
//    else
//    {
//        m_pRequestQueue->setRequestInProgress(i_pReq);
//    }
//
//    SRequestDscr reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    reqDscrPrev = reqDscrCurr;
//
//    //--------------------------------------------------------------------------
//    #pragma message(__TODO__"Add Code here")
//    //--------------------------------------------------------------------------
//
//    if( errResultInfo.isErrorResult() )
//    {
//        if( CErrLog::GetInstance() != nullptr )
//        {
//            CErrLog::GetInstance()->addEntry(errResultInfo);
//        }
//        iProgress_perCent = 100;
//    }
//    else if( iProgress_perCent == 100 && errResultInfo.getResult() == EResultUndefined )
//    {
//        errResultInfo.setResult(EResultSuccess);
//    }
//
//    i_pReq->setErrResultInfo(errResultInfo);
//    i_pReq->setProgressInPerCent(iProgress_perCent);
//    i_pReq->update();
//
//    if( isAsynchronousRequest(i_pReq) )
//    {
//        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
//        {
//            m_reqDscrTimeout = i_pReq->getDescriptor();
//            m_pTmrReqTimeout->start(i_pReq->getTimeoutInMs());
//        }
//
//        if( !QObject::connect(
//            /* pObjSender   */ i_pReq,
//            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//            /* pObjReceiver */ this,
//            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//        {
//            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//        }
//    }
//    else // if( !isAsynchronousRequest(i_pReq) )
//    {
//        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
//        {
//            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
//        }
//
//        // Don't delete the request right now but return the request to the caller
//        // as the caller may want to evaluate some details of the executed request.
//        // E.g. if the request has been started by a request sequencer the sequencer
//        // wants to update the request id.
//        // The request will be set at the request queue to be deleted later
//        // (on destroying the queue or if the next sync request will be set).
//        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);
//
//    } // if( !isAsynchronousRequest(i_pReq) )
//
//    EState stateCurr = m_state;
//
//    if( statePrev != m_state )
//    {
//        emit stateChanged(this, m_state);
//    }
//
//    reqDscrCurr = SRequestDscr();
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    m_pRequestQueue->unlock();
//
//    i_pReq->unlock();
//
//    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
//    {
//        executeNextPostponedRequest();
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//} // executeRegisterRequest
//
////------------------------------------------------------------------------------
//void CDbClient::executeUnregisterRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        strAddTrcInfo += ", ReqMainInProgress {" + QString( i_pReqMainInProgress == nullptr ? "nullptr" : i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "executeUnregisterRequest",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeUnregisterRequest";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    QString strAddErrInfo;
//
//    int iProgress_perCent = 100;
//
//    i_pReq->lock();
//
//    m_pRequestQueue->lock();
//
//    EState statePrev = m_state;
//
//    SRequestDscr reqDscrPrev;
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrPrev = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    // If started as child request of request in progress (as part of a request sequence) ..
//    if( i_pReqMainInProgress != nullptr )
//    {
//        m_pRequestQueue->addRequestInProgress(i_pReq);
//    }
//    // If not started as child request of request in progress (as main request) ..
//    else
//    {
//        m_pRequestQueue->setRequestInProgress(i_pReq);
//    }
//
//    SRequestDscr reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    reqDscrPrev = reqDscrCurr;
//
//    //--------------------------------------------------------------------------
//    #pragma message(__TODO__"Add Code here")
//    //--------------------------------------------------------------------------
//
//    if( errResultInfo.isErrorResult() )
//    {
//        if( CErrLog::GetInstance() != nullptr )
//        {
//            CErrLog::GetInstance()->addEntry(errResultInfo);
//        }
//        iProgress_perCent = 100;
//    }
//    else if( iProgress_perCent == 100 && errResultInfo.getResult() == EResultUndefined )
//    {
//        errResultInfo.setResult(EResultSuccess);
//    }
//
//    i_pReq->setErrResultInfo(errResultInfo);
//    i_pReq->setProgressInPerCent(iProgress_perCent);
//    i_pReq->update();
//
//    if( isAsynchronousRequest(i_pReq) )
//    {
//        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
//        {
//            m_reqDscrTimeout = i_pReq->getDescriptor();
//            m_pTmrReqTimeout->start(i_pReq->getTimeoutInMs());
//        }
//
//        if( !QObject::connect(
//            /* pObjSender   */ i_pReq,
//            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//            /* pObjReceiver */ this,
//            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//        {
//            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//        }
//    }
//    else // if( !isAsynchronousRequest(i_pReq) )
//    {
//        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
//        {
//            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
//        }
//
//        // Don't delete the request right now but return the request to the caller
//        // as the caller may want to evaluate some details of the executed request.
//        // E.g. if the request has been started by a request sequencer the sequencer
//        // wants to update the request id.
//        // The request will be set at the request queue to be deleted later
//        // (on destroying the queue or if the next sync request will be set).
//        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);
//
//    } // if( !isAsynchronousRequest(i_pReq) )
//
//    EState stateCurr = m_state;
//
//    if( statePrev != m_state )
//    {
//        emit stateChanged(this, m_state);
//    }
//
//    reqDscrCurr = SRequestDscr();
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    m_pRequestQueue->unlock();
//
//    i_pReq->unlock();
//
//    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
//    {
//        executeNextPostponedRequest();
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//} // executeUnregisterRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CDbClient::executeSelectSchemaRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        strAddTrcInfo += ", ReqMainInProgress {" + QString( i_pReqMainInProgress == nullptr ? "nullptr" : i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "executeSelectSchemaRequest",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeSelectSchemaRequest";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    QString strAddErrInfo;
//
//    int iProgress_perCent = 100;
//
//    i_pReq->lock();
//
//    m_pRequestQueue->lock();
//
//    EState statePrev = m_state;
//
//    SRequestDscr reqDscrPrev;
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrPrev = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    // If started as child request of request in progress (as part of a request sequence) ..
//    if( i_pReqMainInProgress != nullptr )
//    {
//        m_pRequestQueue->addRequestInProgress(i_pReq);
//    }
//    // If not started as child request of request in progress (as main request) ..
//    else
//    {
//        m_pRequestQueue->setRequestInProgress(i_pReq);
//    }
//
//    SRequestDscr reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    reqDscrPrev = reqDscrCurr;
//
//    //--------------------------------------------------------------------------
//    #pragma message(__TODO__"Add Code here")
//    //--------------------------------------------------------------------------
//
//    if( errResultInfo.isErrorResult() )
//    {
//        if( CErrLog::GetInstance() != nullptr )
//        {
//            CErrLog::GetInstance()->addEntry(errResultInfo);
//        }
//        iProgress_perCent = 100;
//    }
//    else if( iProgress_perCent == 100 && errResultInfo.getResult() == EResultUndefined )
//    {
//        errResultInfo.setResult(EResultSuccess);
//    }
//
//    i_pReq->setErrResultInfo(errResultInfo);
//    i_pReq->setProgressInPerCent(iProgress_perCent);
//    i_pReq->update();
//
//    if( isAsynchronousRequest(i_pReq) )
//    {
//        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
//        {
//            m_reqDscrTimeout = i_pReq->getDescriptor();
//            m_pTmrReqTimeout->start(i_pReq->getTimeoutInMs());
//        }
//
//        if( !QObject::connect(
//            /* pObjSender   */ i_pReq,
//            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//            /* pObjReceiver */ this,
//            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//        {
//            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//        }
//    }
//    else // if( !isAsynchronousRequest(i_pReq) )
//    {
//        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
//        {
//            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
//        }
//
//        // Don't delete the request right now but return the request to the caller
//        // as the caller may want to evaluate some details of the executed request.
//        // E.g. if the request has been started by a request sequencer the sequencer
//        // wants to update the request id.
//        // The request will be set at the request queue to be deleted later
//        // (on destroying the queue or if the next sync request will be set).
//        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);
//
//    } // if( !isAsynchronousRequest(i_pReq) )
//
//    EState stateCurr = m_state;
//
//    if( statePrev != m_state )
//    {
//        emit stateChanged(this, m_state);
//    }
//
//    reqDscrCurr = SRequestDscr();
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    m_pRequestQueue->unlock();
//
//    i_pReq->unlock();
//
//    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
//    {
//        executeNextPostponedRequest();
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//} // executeSelectSchemaRequest
//
////------------------------------------------------------------------------------
//void CDbClient::executeUpdateSchemaRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        strAddTrcInfo += ", ReqMainInProgress {" + QString( i_pReqMainInProgress == nullptr ? "nullptr" : i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "executeUpdateSchemaRequest",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeUpdateSchemaRequest";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    QString strAddErrInfo;
//
//    int iProgress_perCent = 100;
//
//    i_pReq->lock();
//
//    m_pRequestQueue->lock();
//
//    EState statePrev = m_state;
//
//    SRequestDscr reqDscrPrev;
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrPrev = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    // If started as child request of request in progress (as part of a request sequence) ..
//    if( i_pReqMainInProgress != nullptr )
//    {
//        m_pRequestQueue->addRequestInProgress(i_pReq);
//    }
//    // If not started as child request of request in progress (as main request) ..
//    else
//    {
//        m_pRequestQueue->setRequestInProgress(i_pReq);
//    }
//
//    SRequestDscr reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    reqDscrPrev = reqDscrCurr;
//
//    //--------------------------------------------------------------------------
//    #pragma message(__TODO__"Add Code here")
//    //--------------------------------------------------------------------------
//
//    if( errResultInfo.isErrorResult() )
//    {
//        if( CErrLog::GetInstance() != nullptr )
//        {
//            CErrLog::GetInstance()->addEntry(errResultInfo);
//        }
//        iProgress_perCent = 100;
//    }
//    else if( iProgress_perCent == 100 && errResultInfo.getResult() == EResultUndefined )
//    {
//        errResultInfo.setResult(EResultSuccess);
//    }
//
//    i_pReq->setErrResultInfo(errResultInfo);
//    i_pReq->setProgressInPerCent(iProgress_perCent);
//    i_pReq->update();
//
//    if( isAsynchronousRequest(i_pReq) )
//    {
//        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
//        {
//            m_reqDscrTimeout = i_pReq->getDescriptor();
//            m_pTmrReqTimeout->start(i_pReq->getTimeoutInMs());
//        }
//
//        if( !QObject::connect(
//            /* pObjSender   */ i_pReq,
//            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//            /* pObjReceiver */ this,
//            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//        {
//            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//        }
//    }
//    else // if( !isAsynchronousRequest(i_pReq) )
//    {
//        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
//        {
//            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
//        }
//
//        // Don't delete the request right now but return the request to the caller
//        // as the caller may want to evaluate some details of the executed request.
//        // E.g. if the request has been started by a request sequencer the sequencer
//        // wants to update the request id.
//        // The request will be set at the request queue to be deleted later
//        // (on destroying the queue or if the next sync request will be set).
//        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);
//
//    } // if( !isAsynchronousRequest(i_pReq) )
//
//    EState stateCurr = m_state;
//
//    if( statePrev != m_state )
//    {
//        emit stateChanged(this, m_state);
//    }
//
//    reqDscrCurr = SRequestDscr();
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    m_pRequestQueue->unlock();
//
//    i_pReq->unlock();
//
//    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
//    {
//        executeNextPostponedRequest();
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//} // executeUpdateSchemaRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CDbClient::executeSelectDataRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        strAddTrcInfo += ", ReqMainInProgress {" + QString( i_pReqMainInProgress == nullptr ? "nullptr" : i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "executeSelectDataRequest",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeSelectDataRequest";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    QString strAddErrInfo;
//
//    int iProgress_perCent = 100;
//
//    i_pReq->lock();
//
//    m_pRequestQueue->lock();
//
//    EState statePrev = m_state;
//
//    SRequestDscr reqDscrPrev;
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrPrev = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    // If started as child request of request in progress (as part of a request sequence) ..
//    if( i_pReqMainInProgress != nullptr )
//    {
//        m_pRequestQueue->addRequestInProgress(i_pReq);
//    }
//    // If not started as child request of request in progress (as main request) ..
//    else
//    {
//        m_pRequestQueue->setRequestInProgress(i_pReq);
//    }
//
//    SRequestDscr reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    reqDscrPrev = reqDscrCurr;
//
//    //--------------------------------------------------------------------------
//    #pragma message(__TODO__"Add Code here")
//    //--------------------------------------------------------------------------
//
//    if( errResultInfo.isErrorResult() )
//    {
//        if( CErrLog::GetInstance() != nullptr )
//        {
//            CErrLog::GetInstance()->addEntry(errResultInfo);
//        }
//        iProgress_perCent = 100;
//    }
//    else if( iProgress_perCent == 100 && errResultInfo.getResult() == EResultUndefined )
//    {
//        errResultInfo.setResult(EResultSuccess);
//    }
//
//    i_pReq->setErrResultInfo(errResultInfo);
//    i_pReq->setProgressInPerCent(iProgress_perCent);
//    i_pReq->update();
//
//    if( isAsynchronousRequest(i_pReq) )
//    {
//        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
//        {
//            m_reqDscrTimeout = i_pReq->getDescriptor();
//            m_pTmrReqTimeout->start(i_pReq->getTimeoutInMs());
//        }
//
//        if( !QObject::connect(
//            /* pObjSender   */ i_pReq,
//            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//            /* pObjReceiver */ this,
//            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//        {
//            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//        }
//    }
//    else // if( !isAsynchronousRequest(i_pReq) )
//    {
//        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
//        {
//            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
//        }
//
//        // Don't delete the request right now but return the request to the caller
//        // as the caller may want to evaluate some details of the executed request.
//        // E.g. if the request has been started by a request sequencer the sequencer
//        // wants to update the request id.
//        // The request will be set at the request queue to be deleted later
//        // (on destroying the queue or if the next sync request will be set).
//        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);
//
//    } // if( !isAsynchronousRequest(i_pReq) )
//
//    EState stateCurr = m_state;
//
//    if( statePrev != m_state )
//    {
//        emit stateChanged(this, m_state);
//    }
//
//    reqDscrCurr = SRequestDscr();
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    m_pRequestQueue->unlock();
//
//    i_pReq->unlock();
//
//    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
//    {
//        executeNextPostponedRequest();
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//} // executeSelectDataRequest
//
////------------------------------------------------------------------------------
//void CDbClient::executeUpdateDataRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        strAddTrcInfo += ", ReqMainInProgress {" + QString( i_pReqMainInProgress == nullptr ? "nullptr" : i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "executeUpdateDataRequest",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeUpdateDataRequest";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    QString strAddErrInfo;
//
//    int iProgress_perCent = 100;
//
//    i_pReq->lock();
//
//    m_pRequestQueue->lock();
//
//    EState statePrev = m_state;
//
//    SRequestDscr reqDscrPrev;
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrPrev = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    // If started as child request of request in progress (as part of a request sequence) ..
//    if( i_pReqMainInProgress != nullptr )
//    {
//        m_pRequestQueue->addRequestInProgress(i_pReq);
//    }
//    // If not started as child request of request in progress (as main request) ..
//    else
//    {
//        m_pRequestQueue->setRequestInProgress(i_pReq);
//    }
//
//    SRequestDscr reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    reqDscrPrev = reqDscrCurr;
//
//    //--------------------------------------------------------------------------
//    #pragma message(__TODO__"Add Code here")
//    //--------------------------------------------------------------------------
//
//    if( errResultInfo.isErrorResult() )
//    {
//        if( CErrLog::GetInstance() != nullptr )
//        {
//            CErrLog::GetInstance()->addEntry(errResultInfo);
//        }
//        iProgress_perCent = 100;
//    }
//    else if( iProgress_perCent == 100 && errResultInfo.getResult() == EResultUndefined )
//    {
//        errResultInfo.setResult(EResultSuccess);
//    }
//
//    i_pReq->setErrResultInfo(errResultInfo);
//    i_pReq->setProgressInPerCent(iProgress_perCent);
//    i_pReq->update();
//
//    if( isAsynchronousRequest(i_pReq) )
//    {
//        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
//        {
//            m_reqDscrTimeout = i_pReq->getDescriptor();
//            m_pTmrReqTimeout->start(i_pReq->getTimeoutInMs());
//        }
//
//        if( !QObject::connect(
//            /* pObjSender   */ i_pReq,
//            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//            /* pObjReceiver */ this,
//            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//        {
//            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//        }
//    }
//    else // if( !isAsynchronousRequest(i_pReq) )
//    {
//        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
//        {
//            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
//        }
//
//        // Don't delete the request right now but return the request to the caller
//        // as the caller may want to evaluate some details of the executed request.
//        // E.g. if the request has been started by a request sequencer the sequencer
//        // wants to update the request id.
//        // The request will be set at the request queue to be deleted later
//        // (on destroying the queue or if the next sync request will be set).
//        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);
//
//    } // if( !isAsynchronousRequest(i_pReq) )
//
//    EState stateCurr = m_state;
//
//    if( statePrev != m_state )
//    {
//        emit stateChanged(this, m_state);
//    }
//
//    reqDscrCurr = SRequestDscr();
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    m_pRequestQueue->unlock();
//
//    i_pReq->unlock();
//
//    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
//    {
//        executeNextPostponedRequest();
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//} // executeUpdateDataRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CDbClient::executeConnectAndRegisterRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        strAddTrcInfo += ", ReqMainInProgress {" + QString( i_pReqMainInProgress == nullptr ? "nullptr" : i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "executeConnectAndRegisterRequest",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeConnectAndRegisterRequest";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    QString strAddErrInfo;
//
//    int iProgress_perCent = 100;
//
//    i_pReq->lock();
//
//    m_pRequestQueue->lock();
//
//    EState statePrev = m_state;
//
//    SRequestDscr reqDscrPrev;
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrPrev = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    // If started as child request of request in progress (as part of a request sequence) ..
//    if( i_pReqMainInProgress != nullptr )
//    {
//        m_pRequestQueue->addRequestInProgress(i_pReq);
//    }
//    // If not started as child request of request in progress (as main request) ..
//    else
//    {
//        m_pRequestQueue->setRequestInProgress(i_pReq);
//    }
//
//    SRequestDscr reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    reqDscrPrev = reqDscrCurr;
//
//    //--------------------------------------------------------------------------
//    #pragma message(__TODO__"Add Code here")
//    //--------------------------------------------------------------------------
//
//    if( errResultInfo.isErrorResult() )
//    {
//        if( CErrLog::GetInstance() != nullptr )
//        {
//            CErrLog::GetInstance()->addEntry(errResultInfo);
//        }
//        iProgress_perCent = 100;
//    }
//    else if( iProgress_perCent == 100 && errResultInfo.getResult() == EResultUndefined )
//    {
//        errResultInfo.setResult(EResultSuccess);
//    }
//
//    i_pReq->setErrResultInfo(errResultInfo);
//    i_pReq->setProgressInPerCent(iProgress_perCent);
//    i_pReq->update();
//
//    if( isAsynchronousRequest(i_pReq) )
//    {
//        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
//        {
//            m_reqDscrTimeout = i_pReq->getDescriptor();
//            m_pTmrReqTimeout->start(i_pReq->getTimeoutInMs());
//        }
//
//        if( !QObject::connect(
//            /* pObjSender   */ i_pReq,
//            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//            /* pObjReceiver */ this,
//            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//        {
//            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//        }
//    }
//    else // if( !isAsynchronousRequest(i_pReq) )
//    {
//        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
//        {
//            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
//        }
//
//        // Don't delete the request right now but return the request to the caller
//        // as the caller may want to evaluate some details of the executed request.
//        // E.g. if the request has been started by a request sequencer the sequencer
//        // wants to update the request id.
//        // The request will be set at the request queue to be deleted later
//        // (on destroying the queue or if the next sync request will be set).
//        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);
//
//    } // if( !isAsynchronousRequest(i_pReq) )
//
//    EState stateCurr = m_state;
//
//    if( statePrev != m_state )
//    {
//        emit stateChanged(this, m_state);
//    }
//
//    reqDscrCurr = SRequestDscr();
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    m_pRequestQueue->unlock();
//
//    i_pReq->unlock();
//
//    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
//    {
//        executeNextPostponedRequest();
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//} // executeConnectAndRegisterRequest
//
////------------------------------------------------------------------------------
//void CDbClient::executeDisconnectAndUnregisterRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo  = "Req {" + QString( i_pReq == nullptr ? "nullptr" : i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//        strAddTrcInfo += ", ReqMainInProgress {" + QString( i_pReqMainInProgress == nullptr ? "nullptr" : i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel) ) + "}";
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "executeDisconnectAndUnregisterRequest",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".executeDisconnectAndUnregisterRequest";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    QString strAddErrInfo;
//
//    int iProgress_perCent = 100;
//
//    i_pReq->lock();
//
//    m_pRequestQueue->lock();
//
//    EState statePrev = m_state;
//
//    SRequestDscr reqDscrPrev;
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrPrev = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    // If started as child request of request in progress (as part of a request sequence) ..
//    if( i_pReqMainInProgress != nullptr )
//    {
//        m_pRequestQueue->addRequestInProgress(i_pReq);
//    }
//    // If not started as child request of request in progress (as main request) ..
//    else
//    {
//        m_pRequestQueue->setRequestInProgress(i_pReq);
//    }
//
//    SRequestDscr reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    reqDscrPrev = reqDscrCurr;
//
//    //--------------------------------------------------------------------------
//    #pragma message(__TODO__"Add Code here")
//    //--------------------------------------------------------------------------
//
//    if( errResultInfo.isErrorResult() )
//    {
//        if( CErrLog::GetInstance() != nullptr )
//        {
//            CErrLog::GetInstance()->addEntry(errResultInfo);
//        }
//        iProgress_perCent = 100;
//    }
//    else if( iProgress_perCent == 100 && errResultInfo.getResult() == EResultUndefined )
//    {
//        errResultInfo.setResult(EResultSuccess);
//    }
//
//    i_pReq->setErrResultInfo(errResultInfo);
//    i_pReq->setProgressInPerCent(iProgress_perCent);
//    i_pReq->update();
//
//    if( isAsynchronousRequest(i_pReq) )
//    {
//        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
//        {
//            m_reqDscrTimeout = i_pReq->getDescriptor();
//            m_pTmrReqTimeout->start(i_pReq->getTimeoutInMs());
//        }
//
//        if( !QObject::connect(
//            /* pObjSender   */ i_pReq,
//            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
//            /* pObjReceiver */ this,
//            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
//        {
//            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
//        }
//    }
//    else // if( !isAsynchronousRequest(i_pReq) )
//    {
//        if( m_pRequestQueue->isRequestInProgress(i_pReq->getId()) ) // as the request may have already been removed on setting the progress to 100 %
//        {
//            m_pRequestQueue->removeRequestInProgress( i_pReq->getId() );
//        }
//
//        // Don't delete the request right now but return the request to the caller
//        // as the caller may want to evaluate some details of the executed request.
//        // E.g. if the request has been started by a request sequencer the sequencer
//        // wants to update the request id.
//        // The request will be set at the request queue to be deleted later
//        // (on destroying the queue or if the next sync request will be set).
//        m_pRequestQueue->setSyncRequestToBeDeletedLater(i_pReq);
//
//    } // if( !isAsynchronousRequest(i_pReq) )
//
//    EState stateCurr = m_state;
//
//    if( statePrev != m_state )
//    {
//        emit stateChanged(this, m_state);
//    }
//
//    reqDscrCurr = SRequestDscr();
//
//    if( m_pRequestQueue->getRequestInProgress() != nullptr )
//    {
//        reqDscrCurr = m_pRequestQueue->getRequestInProgress()->getDescriptor();
//    }
//
//    if( reqDscrPrev != reqDscrCurr )
//    {
//        emit requestInProgressChanged(this, reqDscrCurr);
//    }
//
//    m_pRequestQueue->unlock();
//
//    i_pReq->unlock();
//
//    if( !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
//    {
//        executeNextPostponedRequest();
//    }
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//} // executeDisconnectAndUnregisterRequest

/*==============================================================================
protected: // overridables
==============================================================================*/

////------------------------------------------------------------------------------
//void CDbClient::onReceivedMsgConSelectSchema( CMsgConSelectSchema* i_pMsgCon )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = "MsgCon: " + i_pMsgCon->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onReceivedMsgConSelectSchema",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    if( m_pDS != nullptr )
//    {
//        //CDSSchemaChgDscr* pSchema = i_pMsgCon->getSchema(); // Don't take but let the dtor of the request message delete the data set.
//
//        //if( pSchema != nullptr )
//        //{
//        //    // The data set of the request message sent by the database contains only "Accepted" schema info.
//        //    // Sending a select schema request to the database is used to initialize the data set of the client.
//        //    // Please note that its not necessary to explicitly accept the changes.
//        //    // If the clients data set has "Current" schema changes those changes still have to be updated
//        //    // and sent to the database by the client.
//        //    // To indicate that Accepted values are to be merged the "merge" call is
//        //    // encapsulated between "beginInit" and "endInit" method calls.
//        //    m_pDS->beginInitSchema();
//        //    m_pDS->enforceConstraints(false);
//        //    //m_pDS->merge(pSchema);
//        //    m_pDS->resolveObjectReferences();
//        //    m_pDS->enforceConstraints(true);
//        //    m_pDS->endInitSchema();
//
//        //} // if( pSchema != nullptr )
//    } // if( m_pDS != nullptr )
//
//} // onReceivedMsgConSelectSchema
//
////------------------------------------------------------------------------------
//void CDbClient::onReceivedMsgConUpdateSchema( CMsgConUpdateSchema* i_pMsgCon )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = i_pMsgCon->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onReceivedMsgConUpdateSchema",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".UpdateSchema";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    if( m_pDS != nullptr )
//    {
//        //CDSSchemaChgDscr* pSchema = i_pMsgCon->getSchema(); // Don't take but let the dtor of the request message delete the data set.
//
//        //if( pSchema != nullptr )
//        //{
//        //    // Please note that its not necessary to accept the changes as the data set
//        //    // of the request message sent by the database contains only "Accepted" schema info.
//        //    // Merging "Accepted" schema info into the clients data set implicitly accepts the changes
//        //    // if the "Current" values are equal to the "Accepted" values.
//        //    // To indicate that Accepted values are to be merged the "merge" call is
//        //    // encapsulated between "beginInit" and "endInit" method calls.
//        //    // If the "Current" values still differ the data set still (or already again)
//        //    // contains changes which have to be updated and sent to the database by the client.
//        //    m_pDS->beginInitSchema();
//        //    m_pDS->enforceConstraints(false);
//        //    //m_pDS->merge(pSchema);
//        //    m_pDS->resolveObjectReferences();
//        //    m_pDS->enforceConstraints(true);
//        //    m_pDS->endInitSchema();
//
//        //} // if( pSchema != nullptr )
//    } // if( m_pDS != nullptr )
//
//} // onReceivedMsgConUpdateSchema

/*==============================================================================
protected: // overridables
==============================================================================*/

////------------------------------------------------------------------------------
//void CDbClient::onReceivedMsgConSelectData( CMsgConSelectData* i_pMsgCon )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = i_pMsgCon->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onReceivedMsgConSelectData",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".SelectData";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    if( m_pDS != nullptr )
//    {
//        //CChangeDataSet* pChgDS = i_pMsgCon->getData(); // Don't take but let the dtor of the request message delete the data set.
//
//        //if( pChgDS != nullptr )
//        //{
//        //    // The data set of the request message sent by the database contains only "Accepted" data.
//        //    // Sending a select data request to the database is used to initialize the data set of the client.
//        //    // Please note that its not necessary to explicitly accept the changes.
//        //    // If the clients data set has "Current" data changes those changes still have to be updated
//        //    // and sent to the database by the client.
//        //    // To indicate that Accepted values are to be merged the "merge" call is
//        //    // encapsulated between "beginInit" and "endInit" method calls.
//        //    m_pDS->beginInitData();
//        //    m_pDS->enforceConstraints(false);
//        //    m_pDS->merge(pChgDS);
//        //    m_pDS->enforceConstraints(true);
//        //    m_pDS->endInitData();
//
//        //} // if( pChgDS != nullptr )
//    } // if( m_pDS != nullptr )
//
//} // onReceivedMsgConSelectData
//
////------------------------------------------------------------------------------
//void CDbClient::onReceivedMsgConUpdateData( CMsgConUpdateData* i_pMsgCon )
////------------------------------------------------------------------------------
//{
//    QString strAddTrcInfo;
//
//    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
//    {
//        strAddTrcInfo = i_pMsgCon->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
//    }
//
//    CMethodTracer mthTracer(
//        /* pAdminObj    */ m_pTrcAdminObj,
//        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
//        /* strMethod    */ "onReceivedMsgConUpdateData",
//        /* strAddInfo   */ strAddTrcInfo );
//
//    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
//    {
//        strAddTrcInfo  = "State: " + State2Str(m_state);
//        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
//        mthTracer.trace(strAddTrcInfo);
//    }
//
//    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".UpdateData";
//
//    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);
//
//    if( m_pDS != nullptr )
//    {
//        //CChangeDataSet* pChgDS = i_pMsgCon->getData(); // Don't take but let the dtor of the request message delete the data set.
//
//        //if( pChgDS != nullptr )
//        //{
//        //    // Please note that its not necessary to accept the changes as the data set
//        //    // of the request message sent by the database contains only "Accepted" data.
//        //    // Merging "Accepted" data into the clients data set implicitly accepts the changes
//        //    // if the "Current" values are equal to the "Accepted" values.
//        //    // To indicate that Accepted values are to be merged the "merge" call is
//        //    // encapsulated between "beginInit" and "endInit" method calls.
//        //    // If the "Current" values still differ the data set still (or already again)
//        //    // contains changes which have to be updated and sent to the database by the client.
//        //    m_pDS->beginInitData();
//        //    m_pDS->enforceConstraints(false);
//        //    m_pDS->merge(pChgDS);
//        //    m_pDS->enforceConstraints(true);
//        //    m_pDS->endInitData();
//
//        //} // if( pChgDS != nullptr )
//    } // if( m_pDS != nullptr )
//
//} // onReceivedMsgConUpdateData

/*==============================================================================
protected: // overridables (auxiliary methods)
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::createGatewayThread()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createGatewayThread",
        /* strAddInfo   */ strAddTrcInfo );

    m_pGatewayThread = new CDbClientGatewayThread(this);

} // createGatewayThread

//------------------------------------------------------------------------------
void CDbClient::destroyGatewayThread()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "destroyGatewayThread",
        /* strAddInfo   */ strAddTrcInfo );

    delete m_pGatewayThread;
    m_pGatewayThread = nullptr;

} // destroyGatewayThread

/*==============================================================================
protected: // overridables (auxiliary methods, blocking (synchronous) methods)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::startGatewayThread(
    int    i_iTimeout_ms,
    qint64 i_iReqIdParent )
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
        /* strMethod    */ "startGatewayThread",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".startGatewayThread";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqStartThread = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestStartGatewayThread,
        /* strRequest   */ Request2Str(ERequestStartGatewayThread),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ true );

    m_pRequestQueue->addRequestInProgress(pReqStartThread);

    //int iProgress_perCent = 100;

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        mthTracer.trace( "-+ GatewayThread.start()" );
    }

    m_pGatewayThread->start( pReqStartThread->getId() );

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
        errResultInfo.setErrResult(ErrResultSuccess);

        m_pGateway = m_pGatewayThread->getGateway();

        if( m_pGateway != nullptr )
        {
            for( int idxObj = 0; idxObj < m_arpTrcMsgLogObjects.size(); idxObj++ )
            {
                QObject* pObj = m_arpTrcMsgLogObjects[idxObj];

                CDbClientGateway* pGateway = dynamic_cast<CDbClientGateway*>(m_pGateway);

                if( pGateway != nullptr )
                {
                    pGateway->addTrcMsgLogObject(pObj);
                }
            }
        }
    }
    else // if( !pReqStartThread->wait() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultTimeout);
        errResultInfo.setAddErrInfoDscr( "Waiting for StartThread timed out." );
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        mthTracer.trace( "+- ReqStartThread.wait(): " + errResultInfo.getAddErrInfoDscr() );
    }

    pReqStartThread->setErrResultInfo(errResultInfo);
    pReqStartThread->setProgressInPerCent(100);
    pReqStartThread->update();

    // Please note that StartGatewayThread is an internal blocking request which may not become asynchronous.
    m_pRequestQueue->removeRequestInProgress( pReqStartThread->getId() );

    // The one who calls update for the finished request must delete the request.
    deleteRequest(pReqStartThread);
    pReqStartThread = nullptr;

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
        strAddTrcInfo = QString(pReqStartThread == nullptr ? "Success" : pReqStartThread->getAddTrcInfoStr(iAddTrcInfoDetailLevel));
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqStartThread; // = nullptr

} // startGatewayThread

//------------------------------------------------------------------------------
CRequest* CDbClient::stopGatewayThread(
    int    i_iTimeout_ms,
    qint64 i_iReqIdParent )
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
        /* strMethod    */ "stopGatewayThread",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".stopGatewayThread";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqStopThread = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestStopGatewayThread,
        /* strRequest   */ Request2Str(ERequestStopGatewayThread),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ true );

    m_pRequestQueue->addRequestInProgress(pReqStopThread);

    //int iProgress_perCent = 100;

    if( m_pGatewayThread == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Gateway thread not created." );

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else // if( m_pGatewayThread != nullptr )
    {
        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "-+ GatewayThread.quit()" );
        }

        m_pGatewayThread->quit();

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "+- GatewayThread.quit()" );
        }

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "-+ ReqStopThread.wait()" );
        }

        if( m_pGatewayThread->wait(i_iTimeout_ms <= 0 ? ULONG_MAX : static_cast<unsigned long>(i_iTimeout_ms)) )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }
        else
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );
        }

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "+- ReqStopThread.wait(): " + errResultInfo.getAddErrInfoDscr() );
        }
    } // if( m_pGatewayThread != nullptr )

    pReqStopThread->setErrResultInfo(errResultInfo);
    pReqStopThread->setProgressInPerCent(100);
    pReqStopThread->update();

    // Please note that StopThread is an internal blocking request which may not become asynchronous.
    m_pRequestQueue->removeRequestInProgress( pReqStopThread->getId() );

    // The one who calls update for the finished request must delete the request.
    deleteRequest(pReqStopThread);
    pReqStopThread = nullptr;

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
        strAddTrcInfo = QString(pReqStopThread == nullptr ? "Success" : pReqStopThread->getAddTrcInfoStr(iAddTrcInfoDetailLevel));
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqStopThread; // = nullptr

} // stopGatewayThread

//------------------------------------------------------------------------------
CRequest* CDbClient::startupGateway( int i_iTimeout_ms, qint64 i_iReqIdParent )
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
        /* strMethod    */ "startupGateway",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".startupGateway";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqStartupGateway = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestStartupGateway,
        /* strRequest   */ Request2Str(ERequestStartupGateway),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ true );

    m_pRequestQueue->addRequestInProgress(pReqStartupGateway);

    //int iProgress_perCent = 100;

    if( m_pGateway == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Gateway is not created." );

    } // if( m_pGateway == nullptr )

    else // if( m_pGateway != nullptr )
    {
        EProtocolType       protocolType     = getProtocolType();
        SClientHostSettings hostSettings     = getHostSettings();
        STimerSettings      watchDogSettings = getWatchDogSettings();

        CMsgReqStartup* pMsgReq = new CMsgReqStartup(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pGateway,
            /* protocolType     */ protocolType,
            /* hostSettings     */ hostSettings,
            /* watchDogSettings */ watchDogSettings,
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
                errResultInfo = pMsgCon->getErrResultInfo();
                delete pMsgCon;
                pMsgCon = nullptr;
            }
            else // if( pMsgCon == nullptr )
            {
                errResultInfo = pReqStartupGateway->getErrResultInfo();
            }
        }
        else // if( !pReqStartupGateway->wait() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for startup confirmation timed out." );
        }

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "+- ReqStartupGateway.wait(): " + errResultInfo.getAddErrInfoDscr() );
        }
    } // if( m_pGateway != nullptr )

    if( errResultInfo.isErrorResult() && CErrLog::GetInstance() != nullptr )
    {
        CErrLog::GetInstance()->addEntry(errResultInfo);
    }

    pReqStartupGateway->setErrResultInfo(errResultInfo);
    pReqStartupGateway->setProgressInPerCent(100);
    pReqStartupGateway->update();

    // Please note that StartupGateway is an internal blocking request which may not become asynchronous.
    m_pRequestQueue->removeRequestInProgress( pReqStartupGateway->getId() );

    // The one who calls update for the finished request must delete the request.
    deleteRequest(pReqStartupGateway);
    pReqStartupGateway = nullptr;

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReqStartupGateway == nullptr ? "Success" : pReqStartupGateway->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqStartupGateway; // = nullptr

} // startupGateway

//------------------------------------------------------------------------------
CRequest* CDbClient::shutdownGateway( int i_iTimeout_ms, qint64 i_iReqIdParent )
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
        /* strMethod    */ "shutdownGateway",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".shutdownGateway";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqShutdownGateway = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestShutdownGateway,
        /* strRequest   */ Request2Str(ERequestShutdownGateway),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ true );

    m_pRequestQueue->addRequestInProgress(pReqShutdownGateway);

    //int iProgress_perCent = 100;

    if( m_pGateway == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Gateway is not created." );

    } // if( m_pGateway == nullptr )

    else // if( m_pGateway != nullptr )
    {
        CMsgReqShutdown* pMsgReq = new CMsgReqShutdown(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pGateway,
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
                errResultInfo = pMsgCon->getErrResultInfo();
                delete pMsgCon;
                pMsgCon = nullptr;
            }
            else // if( pMsgCon == nullptr )
            {
                errResultInfo = pReqShutdownGateway->getErrResultInfo();
            }
        }
        else // if( !pReqShutdownGateway->wait() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for shutdown confirmation timed out." );
        }

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            mthTracer.trace( "+- ReqShutdownGateway.wait(): " + errResultInfo.getAddErrInfoDscr() );
        }
    } // if( m_pGateway != nullptr )

    if( errResultInfo.isErrorResult() && CErrLog::GetInstance() != nullptr )
    {
        CErrLog::GetInstance()->addEntry(errResultInfo);
    }

    pReqShutdownGateway->setErrResultInfo(errResultInfo);
    pReqShutdownGateway->setProgressInPerCent(100);
    pReqShutdownGateway->update();

    // Please note that StartupGateway is an internal blocking request which may not become asynchronous.
    m_pRequestQueue->removeRequestInProgress( pReqShutdownGateway->getId() );

    // The one who calls update for the finished request must delete the request.
    deleteRequest(pReqShutdownGateway);
    pReqShutdownGateway = nullptr;

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReqShutdownGateway == nullptr ? "Success" : pReqShutdownGateway->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqShutdownGateway; // = nullptr

} // shutdownGateway

/*==============================================================================
protected: // overridables (auxiliary methods, optional blocking (synchronous) or unsynchronous methods)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDbClient::connectGateway(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "connectGateway",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".connectGateway";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqConnectGateway = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestConnectGateway,
        /* strRequest   */ Request2Str(ERequestConnectGateway),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    m_pRequestQueue->addRequestInProgress(pReqConnectGateway);

    int iProgress_perCent = 100;

    if( m_pGateway == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Gateway is not created." );

    } // if( m_pGateway == nullptr )

    else // if( m_pGateway != nullptr )
    {
        EProtocolType       protocolType     = getProtocolType();
        SClientHostSettings hostSettings     = getHostSettings();
        STimerSettings      watchDogSettings = getWatchDogSettings();

        hostSettings.m_iConnectTimeout_ms = i_iTimeout_ms;

        CMsgReqConnect* pMsgReq = new CMsgReqConnect(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pGateway,
            /* protocolType     */ protocolType,
            /* hostSettings     */ hostSettings,
            /* watchDogSettings */ watchDogSettings,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ pReqConnectGateway->getId() );

        pMsgReq->setTimeoutInMs(i_iTimeout_ms);
        pMsgReq->setIsBlockingRequest(i_bWait);

        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);

        if( pReqConnectGateway->isBlockingRequest() )
        {
            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqConnectGateway.wait()" );
            }

            if( pReqConnectGateway->wait() )
            {
                CMsgConConnect* pMsgCon = dynamic_cast<CMsgConConnect*>(pReqConnectGateway->takeExecutionConfirmationMessage());

                if( pMsgCon != nullptr )
                {
                    errResultInfo = pMsgCon->getErrResultInfo();
                    delete pMsgCon;
                    pMsgCon = nullptr;
                }
                else // if( pMsgCon == nullptr )
                {
                    errResultInfo = pReqConnectGateway->getErrResultInfo();
                }
            }
            else // if( !pReqShutdownGateway->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for connect confirmation timed out." );
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- ReqConnectGateway.wait(): " + errResultInfo.getAddErrInfoDscr() );
            }
        } // if( pReqConnectGateway->isBlockingRequest() )

        else if( !pReqConnectGateway->isBlockingRequest() )
        {
            iProgress_perCent = 0;
            errResultInfo.setErrResult(ErrResultUndefined);
        }
    } // if( m_pGateway != nullptr )

    if( errResultInfo.isErrorResult() && CErrLog::GetInstance() != nullptr )
    {
        CErrLog::GetInstance()->addEntry(errResultInfo);
    }

    // Please note that the "onRequestChanged" slot is called on invoking the update method of the request instance.
    // If the request is finished (Progress = 100%) the "onRequestChanged" slot sets the state of the client correspondingly.
    pReqConnectGateway->setErrResultInfo(errResultInfo);
    pReqConnectGateway->setProgressInPerCent(iProgress_perCent);
    pReqConnectGateway->update();

    if( isAsynchronousRequest(pReqConnectGateway) )
    {
        if( !QObject::connect(
            /* pObjSender   */ pReqConnectGateway,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        if( pReqConnectGateway->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = pReqConnectGateway->getDescriptor();
            m_pTmrReqTimeout->start( pReqConnectGateway->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(pReqConnectGateway) )
    {
        m_pRequestQueue->removeRequestInProgress( pReqConnectGateway->getId() );

        // The one who calls update for the finished request must delete the request.
        deleteRequest(pReqConnectGateway);
        pReqConnectGateway = nullptr;
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReqConnectGateway == nullptr ? "Success" : pReqConnectGateway->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqConnectGateway; // could be nullptr

} // connectGateway

//------------------------------------------------------------------------------
CRequest* CDbClient::disconnectGateway(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "disconnectGateway",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".disconnectGateway";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqDisconnectGateway = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestDisconnectGateway,
        /* strRequest   */ Request2Str(ERequestDisconnectGateway),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    m_pRequestQueue->addRequestInProgress(pReqDisconnectGateway);

    int iProgress_perCent = 100;

    if( m_pGateway == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Gateway is not created." );

    } // if( m_pGateway == nullptr )

    else // if( m_pGateway != nullptr )
    {
        EProtocolType protocolType = getProtocolType();
        SSocketDscr   socketDscr   = getSocketDscr();

        CMsgReqDisconnect* pMsgReq = new CMsgReqDisconnect(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pGateway,
            /* protocolType     */ protocolType,
            /* socketDscr       */ socketDscr,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ pReqDisconnectGateway->getId() );

        pMsgReq->setTimeoutInMs(i_iTimeout_ms);
        pMsgReq->setIsBlockingRequest(i_bWait);

        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);

        if( pReqDisconnectGateway->isBlockingRequest() )
        {
            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqDisconnectGateway.wait()" );
            }

            if( pReqDisconnectGateway->wait() )
            {
                CMsgConDisconnect* pMsgCon = dynamic_cast<CMsgConDisconnect*>(pReqDisconnectGateway->takeExecutionConfirmationMessage());

                if( pMsgCon != nullptr )
                {
                    errResultInfo = pMsgCon->getErrResultInfo();
                    delete pMsgCon;
                    pMsgCon = nullptr;
                }
                else // if( pMsgCon == nullptr )
                {
                    errResultInfo = pReqDisconnectGateway->getErrResultInfo();
                }
            }
            else // if( !pReqShutdownGateway->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for connect confirmation timed out." );
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- ReqDisconnectGateway.wait(): " + errResultInfo.getAddErrInfoDscr() );
            }
        } // if( pReqDisconnectGateway->isBlockingRequest() )

        else if( !pReqDisconnectGateway->isBlockingRequest() )
        {
            iProgress_perCent = 0;
            errResultInfo.setErrResult(ErrResultUndefined);
        }
    } // if( m_pGateway != nullptr )

    if( errResultInfo.isErrorResult() && CErrLog::GetInstance() != nullptr )
    {
        CErrLog::GetInstance()->addEntry(errResultInfo);
    }

    // Please note that the "onRequestChanged" slot is called on invoking the update method of the request instance.
    // If the request is finished (Progress = 100%) the "onRequestChanged" slot sets the state of the client correspondingly.
    pReqDisconnectGateway->setErrResultInfo(errResultInfo);
    pReqDisconnectGateway->setProgressInPerCent(iProgress_perCent);
    pReqDisconnectGateway->update();

    if( isAsynchronousRequest(pReqDisconnectGateway) )
    {
        if( !QObject::connect(
            /* pObjSender   */ pReqDisconnectGateway,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        if( pReqDisconnectGateway->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = pReqDisconnectGateway->getDescriptor();
            m_pTmrReqTimeout->start( pReqDisconnectGateway->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(pReqDisconnectGateway) )
    {
        m_pRequestQueue->removeRequestInProgress( pReqDisconnectGateway->getId() );

        // The one who calls update for the finished request must delete the request.
        deleteRequest(pReqDisconnectGateway);
        pReqDisconnectGateway = nullptr;
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReqDisconnectGateway == nullptr ? "Success" : pReqDisconnectGateway->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqDisconnectGateway; // = nullptr

} // disconnectGateway

//------------------------------------------------------------------------------
CRequest* CDbClient::registerGateway(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "registerGateway",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".registerGateway";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqRegisterGateway = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestRegisterGateway,
        /* strRequest   */ Request2Str(ERequestRegisterGateway),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    m_pRequestQueue->addRequestInProgress(pReqRegisterGateway);

    int iProgress_perCent = 100;

    if( m_pGateway == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Gateway is not created." );

    } // if( m_pGateway == nullptr )

    else // if( m_pGateway != nullptr )
    {
        EProtocolType protocolType = getProtocolType();
        ESocketType   socketType   = getSocketType();
        SCnctId       cnctId(protocolType, socketType);

        CMsgReqRegister* pMsgReq = new CMsgReqRegister(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pGateway,
            /* cnctId           */ cnctId,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ pReqRegisterGateway->getId() );

        pMsgReq->setTimeoutInMs(i_iTimeout_ms);
        pMsgReq->setIsBlockingRequest(i_bWait);

        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);

        if( pReqRegisterGateway->isBlockingRequest() )
        {
            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqRegisterGateway.wait()" );
            }

            if( pReqRegisterGateway->wait() )
            {
                CMsgConRegister* pMsgCon = dynamic_cast<CMsgConRegister*>(pReqRegisterGateway->takeExecutionConfirmationMessage());

                if( pMsgCon != nullptr )
                {
                    errResultInfo = pMsgCon->getErrResultInfo();
                    delete pMsgCon;
                    pMsgCon = nullptr;
                }
                else // if( pMsgCon == nullptr )
                {
                    errResultInfo = pReqRegisterGateway->getErrResultInfo();
                }
            }
            else // if( !pReqShutdownGateway->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for connect confirmation timed out." );
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- ReqRegisterGateway.wait(): " + errResultInfo.getAddErrInfoDscr() );
            }
        } // if( pReqRegisterGateway->isBlockingRequest() )

        else if( !pReqRegisterGateway->isBlockingRequest() )
        {
            iProgress_perCent = 0;
            errResultInfo.setErrResult(ErrResultUndefined);
        }
    } // if( m_pGateway != nullptr )

    if( errResultInfo.isErrorResult() && CErrLog::GetInstance() != nullptr )
    {
        CErrLog::GetInstance()->addEntry(errResultInfo);
    }

    // Please note that the "onRequestChanged" slot is called on invoking the update method of the request instance.
    // If the request is finished (Progress = 100%) the "onRequestChanged" slot sets the state of the client correspondingly.
    pReqRegisterGateway->setErrResultInfo(errResultInfo);
    pReqRegisterGateway->setProgressInPerCent(iProgress_perCent);
    pReqRegisterGateway->update();

    if( isAsynchronousRequest(pReqRegisterGateway) )
    {
        if( !QObject::connect(
            /* pObjSender   */ pReqRegisterGateway,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        if( pReqRegisterGateway->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = pReqRegisterGateway->getDescriptor();
            m_pTmrReqTimeout->start( pReqRegisterGateway->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(pReqRegisterGateway) )
    {
        m_pRequestQueue->removeRequestInProgress( pReqRegisterGateway->getId() );

        // The one who calls update for the finished request must delete the request.
        deleteRequest(pReqRegisterGateway);
        pReqRegisterGateway = nullptr;
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReqRegisterGateway == nullptr ? "Success" : pReqRegisterGateway->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqRegisterGateway; // = nullptr

} // registerGateway

//------------------------------------------------------------------------------
CRequest* CDbClient::unregisterGateway(
    int    i_iTimeout_ms,
    bool   i_bWait,
    qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "unregisterGateway",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QString strMth = nameSpace() + "::" + className() + "::" + objectName() + ".unregisterGateway";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqUnregisterGateway = m_pRequestQueue->createRequest(
        /* strNameSpace */ nameSpace(),
        /* strClassName */ className(),
        /* iRequest     */ ERequestUnregisterGateway,
        /* strRequest   */ Request2Str(ERequestUnregisterGateway),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    m_pRequestQueue->addRequestInProgress(pReqUnregisterGateway);

    int iProgress_perCent = 100;

    if( m_pGateway == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultRequestRefused);
        errResultInfo.setAddErrInfoDscr( "Gateway is not created." );

    } // if( m_pGateway == nullptr )

    else // if( m_pGateway != nullptr )
    {
        EProtocolType protocolType = getProtocolType();
        ESocketType   socketType   = getSocketType();
        SCnctId       cnctId(protocolType, socketType);

        CMsgReqUnregister* pMsgReq = new CMsgReqUnregister(
            /* pObjSender       */ this,
            /* pObjReceiver     */ m_pGateway,
            /* cnctId           */ cnctId,
            /* bMustBeConfirmed */ true,
            /* iReqId           */ pReqUnregisterGateway->getId() );

        pMsgReq->setTimeoutInMs(i_iTimeout_ms);
        pMsgReq->setIsBlockingRequest(i_bWait);

        POST_OR_DELETE_MESSAGE(pMsgReq, &mthTracer, ETraceDetailLevelRuntimeInfo);

        if( pReqUnregisterGateway->isBlockingRequest() )
        {
            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "-+ ReqUnregisterGateway.wait()" );
            }

            if( pReqUnregisterGateway->wait() )
            {
                CMsgConUnregister* pMsgCon = dynamic_cast<CMsgConUnregister*>(pReqUnregisterGateway->takeExecutionConfirmationMessage());

                if( pMsgCon != nullptr )
                {
                    errResultInfo = pMsgCon->getErrResultInfo();
                    delete pMsgCon;
                    pMsgCon = nullptr;
                }
                else // if( pMsgCon == nullptr )
                {
                    errResultInfo = pReqUnregisterGateway->getErrResultInfo();
                }
            }
            else // if( !pReqShutdownGateway->wait() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for connect confirmation timed out." );
            }

            if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
            {
                mthTracer.trace( "+- ReqUnregisterGateway.wait(): " + errResultInfo.getAddErrInfoDscr() );
            }
        } // if( pReqUnregisterGateway->isBlockingRequest() )

        else if( !pReqUnregisterGateway->isBlockingRequest() )
        {
            iProgress_perCent = 0;
            errResultInfo.setErrResult(ErrResultUndefined);
        }
    } // if( m_pGateway != nullptr )

    if( errResultInfo.isErrorResult() && CErrLog::GetInstance() != nullptr )
    {
        CErrLog::GetInstance()->addEntry(errResultInfo);
    }

    // Please note that the "onRequestChanged" slot is called on invoking the update method of the request instance.
    // If the request is finished (Progress = 100%) the "onRequestChanged" slot sets the state of the client correspondingly.
    pReqUnregisterGateway->setErrResultInfo(errResultInfo);
    pReqUnregisterGateway->setProgressInPerCent(iProgress_perCent);
    pReqUnregisterGateway->update();

    if( isAsynchronousRequest(pReqUnregisterGateway) )
    {
        if( !QObject::connect(
            /* pObjSender   */ pReqUnregisterGateway,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        if( pReqUnregisterGateway->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = pReqUnregisterGateway->getDescriptor();
            m_pTmrReqTimeout->start( pReqUnregisterGateway->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(pReqUnregisterGateway) )
    {
        m_pRequestQueue->removeRequestInProgress( pReqUnregisterGateway->getId() );

        // The one who calls update for the finished request must delete the request.
        deleteRequest(pReqUnregisterGateway);
        pReqUnregisterGateway = nullptr;
    }

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = QString( pReqUnregisterGateway == nullptr ? "Success" : pReqUnregisterGateway->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqUnregisterGateway; // = nullptr

} // unregisterGateway

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::acceptChangedSettings()
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
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    if( m_protocolTypeCurr != EProtocolTypeUndefined )
    {
        m_protocolTypeOrig = m_protocolTypeCurr;
        m_protocolTypeCurr = EProtocolTypeUndefined;
    }

    if( m_socketTypeCurr != ESocketTypeUndefined )
    {
        m_socketTypeOrig = m_socketTypeCurr;
        m_socketTypeCurr = ESocketTypeUndefined;
    }

    QHash<SCnctType,SClientHostSettings>::iterator itHostSettings;
    QHash<SCnctType,STimerSettings>::iterator      itWatchDogSettings;

    SCnctType           cnctType;
    SClientHostSettings hostSettings;
    STimerSettings      watchDogSettings;
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

    for( itWatchDogSettings = m_hshWatchDogSettingsCurr.begin(); itWatchDogSettings != m_hshWatchDogSettingsCurr.end(); itWatchDogSettings++ )
    {
        cnctType = itWatchDogSettings.key();
        watchDogSettings = itWatchDogSettings.value();

        m_hshWatchDogSettingsOrig[cnctType] = watchDogSettings;
        arCnctsAccepted << cnctType;
    }

    m_hshWatchDogSettingsCurr.clear();

    for( idxCnct = 0; idxCnct < arCnctsAccepted.size(); idxCnct++ )
    {
        cnctType = arCnctsAccepted[idxCnct];

        emit watchDogSettingsRowVersionChanged(this, cnctType.m_protocolType, cnctType.m_socketType);
    }

    arCnctsAccepted.clear();

} // acceptChangedSettings

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::onRequestTimeout()
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
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    m_pTmrReqTimeout->stop();

    CRequest* pReq = m_pRequestQueue->getRequestInProgress(m_reqDscrTimeout.m_iId);

    if( pReq != nullptr )
    {
        SErrResultInfo errResultInfo = ErrResultInfoError("onRequestTimeout", EResultTimeout, pReq->getRequestName());
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
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

} // onRequestTimeout

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::onRequestChanged( ZS::System::SRequestDscr i_reqDscr )
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
        strAddTrcInfo  = "State: " + State2Str(m_state);
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
        //bool       bIsMainRequest     = false;
        EState       statePrev          = m_state;
        SRequestDscr reqDscrPrev;

        if( pReqChanged == pReqMainInProgress )
        {
            //bIsMainRequest = true;
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
                //-------------------------------------
                // Public request methods
                //-------------------------------------
                case ERequestStartup:
                {
                    // If the request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        if( i_reqDscr.m_errResultInfo.isErrorResult() )
                        {
                            // stopGatewayThread is a blocking request method.
                            stopGatewayThread();

                            m_state = EStateIdle;
                        }
                        else // if( !i_reqDscr.m_errResultInfo.isErrorResult() )
                        {
                            m_state = EStateUnconnected;
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

                case ERequestConnect:
                {
                    // If the request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        if( !i_reqDscr.m_errResultInfo.isErrorResult() )
                        {
                            // Client has been connected.
                            if( m_state == EStateUnconnected )
                            {
                                m_state = EStateConnected;
                                emit stateChanged( this, m_state );
                            }
                        } // if( !i_reqDscr.m_errResultInfo.isErrorResult() )
                    } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                    break;
                } // case ERequestConnect

                case ERequestDisconnect:
                {
                    // If the request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        // Client has been disconnected.
                        if( m_state == EStateConnected || m_state == EStateRegistered )
                        {
                            m_state = EStateUnconnected;
                            emit stateChanged( this, m_state );
                        }
                    } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                    break;
                } // case ERequestDisconnect

                case ERequestRegister:
                {
                    // If the request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        if( !i_reqDscr.m_errResultInfo.isErrorResult() )
                        {
                            if( m_state == EStateConnected )
                            {
                                m_state = EStateRegistered;
                                emit stateChanged( this, m_state );
                            }
                        } // if( !i_reqDscr.m_errResultInfo.isErrorResult() )
                    } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                    break;
                } // case ERequestRegister

                case ERequestUnregister:
                {
                    // If the request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        if( !i_reqDscr.m_errResultInfo.isErrorResult() )
                        {
                            if( m_state == EStateRegistered )
                            {
                                m_state = EStateConnected;
                                emit stateChanged( this, m_state );
                            }
                        } // if( !i_reqDscr.m_errResultInfo.isErrorResult() )
                    } // if( i_reqDscr.m_iProgress_perCent >= 100 )
                    break;
                } // case ERequestUnregister

                //case ERequestSelectSchema:
                //case ERequestUpdateSchema:
                //case ERequestSelectData:
                //case ERequestUpdateData:
                //{
                //    // Current request in progress has been confirmed.
                //    // Handled by virtual methods "onReceivedMsgCon<Request>"
                //    // so that derived classes can reimplement the methods.
                //    break;
                //}

                //-------------------------------------
                // Internal unblocking requests
                //-------------------------------------
                case ERequestConnectGateway:
                case ERequestDisconnectGateway:
                {
                    // If sub request has been finished ..
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        CRequest* pReqParent = m_pRequestQueue->findRequestById(i_reqDscr.m_iParentId);

                        if( pReqParent != nullptr )
                        {
                            int iProgressParent_perCent = 100;

                            // Generic code for updating parent request not needed as the parent requests
                            // of ConnectGateway and DisconnectGateway may only have the one child request
                            // which has been updated here.
                            //int iChildReqsInProgress = 0;
                            //// Check whether siblings are still in progress (other child requests of the parent of the executed request) ..
                            //QList<qint64> ariChildIds = pReqParent->getChildIds();
                            //for( int idxChild = 0; idxChild < ariChildIds.size(); idxChild++ )
                            //{
                            //    if( m_pRequestQueue->isRequestInProgress(ariChildIds[idxChild]) )
                            //    {
                            //        iChildReqsInProgress++;
                            //    }
                            //}

                            //iProgressParent_perCent = 100 / (iChildReqsInProgress+1);

                            pReqParent->lock();
                            pReqParent->setErrResultInfo(i_reqDscr.m_errResultInfo);
                            pReqParent->setProgressInPerCent(iProgressParent_perCent);
                            pReqParent->update();
                            pReqParent->unlock();

                            // The one who calls update for the finished request must delete the request
                            // (if the request is not locked etc.).
                            if( iProgressParent_perCent >= 100 && !pReqParent->isLocked() )
                            {
                                m_pRequestQueue->setSyncRequestToBeDeletedLater(pReqParent);
                            }
                        } // if( pReqParent != nullptr )
                    } // if( i_reqDscr.m_iProgress_perCent >= 100 )

                    // The request sequencer will call the next request method.
                    break;
                } // case ERequestDisconnectGateway

                //-------------------------------------
                // Internal blocking requests
                //-------------------------------------
                case ERequestStartGatewayThread:
                case ERequestStopGatewayThread:
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
void CDbClient::onDbDestroyed( QObject* /*i_pDb*/ )
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
        /* strMethod    */ "onDbDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    QHash<SCnctType,Ipc::SClientHostSettings>::Iterator itHostSettings;

    for( itHostSettings = m_hshHostSettingsDefault.begin(); itHostSettings != m_hshHostSettingsDefault.end(); itHostSettings++ )
    {
        itHostSettings.value().m_pObjRemote = nullptr;
    }

    for( itHostSettings = m_hshHostSettingsOrig.begin(); itHostSettings != m_hshHostSettingsOrig.end(); itHostSettings++ )
    {
        itHostSettings.value().m_pObjRemote = nullptr;
    }

    for( itHostSettings = m_hshHostSettingsCurr.begin(); itHostSettings != m_hshHostSettingsCurr.end(); itHostSettings++ )
    {
        itHostSettings.value().m_pObjRemote = nullptr;
    }

    m_socketDscr.m_pObjRemote = nullptr;

    //try
    //{
    //    delete m_pReqDb;
    //}
    //catch(...)
    //{
    //}
    //m_pReqDb = nullptr;
    //m_iReqMsgIdDb = -1;
    //m_iReqIdDb = -1;

    // If using a gateway (socket connection to database) the disconnected indication message
    // will be sent by the gateway if the client is currently connected with the database.
    // If not using a gateway the indication message will not be received.
    //if( m_pGateway == nullptr )
    //{
    //    m_pRequestQueue->lock();

    //    if( m_pRequestQueue->isRequestInProgress() )
    //    {
    //        CRequest* pReq = m_pRequestQueue->getRequestInProgress();

    //        SErrResultInfo errResultInfo = pReq->getErrResultInfo();

    //        switch( pReq->getRequest() )
    //        {
    //            case ERequestConnect:
    //            case ERequestDisconnect:
    //            case ERequestRegister:
    //            case ERequestUnregister:
    //            case ERequestSelectSchema:
    //            case ERequestUpdateSchema:
    //            case ERequestSelectData:
    //            case ERequestUpdateData:
    //            {
    //                errResultInfo.setSeverity(EResultSeverityError);
    //                errResultInfo.setResult(EResultRequestAborted);
    //                errResultInfo.setAddErrInfoDscr("Connection to database has been lost");
    //                pReq->lock();
    //                pReq->setErrResultInfo(errResultInfo);
    //                pReq->setProgressInPerCent(100);
    //                pReq->update();
    //                pReq->unlock();
    //                deleteRequest(pReq);
    //                pReq = nullptr;
    //                break;
    //            }
    //            default:
    //            {
    //                break;
    //            }
    //        } // switch( pReq->getRequest() )
    //    } // if( m_pRequestQueue->isRequestInProgress() )

    //    if( m_state >= EStateConnected )
    //    {
    //        EState statePrev = m_state;
    //        m_state = EStateUnconnected;
    //        emit stateChanged( this, m_state );
    //    }

    //    m_pRequestQueue->unlock();

    //} // if( m_pGateway == nullptr )

} // onDbDestroyed

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CDbClient::onDSDestroyed( QObject* /*i_pDS*/ )
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
        /* strMethod    */ "onDSDestroyed",
        /* strAddInfo   */ strAddTrcInfo );

    if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "State: " + State2Str(m_state);
        strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    setDataSet(nullptr);

} // onDSDestroyed

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CDbClient::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    CMsg* pMsg = dynamic_cast<CMsg*>(i_pMsg);

    if( pMsg != nullptr )
    {
        bEventHandled = true;

        QMutexLocker mtxLocker(m_pMtx);

        QString strAddTrcInfo;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
        {
            int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
            strAddTrcInfo = "Msg {" + pMsg->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
            strAddTrcInfo  = "State: " + State2Str(m_state);
            strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
            mthTracer.trace(strAddTrcInfo);
        }

        QString strMth = nameSpace() + "::" + className() + "::" + objectName() + "." +  pMsg->msgTypeToStr();

        SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

        EState statePrev = m_state;

        bool bWasConnected = isConnected();

        if( pMsg->isBaseMsgType() )
        {
            if( pMsg->getMsgType() == ZS::System::EBaseMsgTypeIndErr )
            {
                CMsgErr* pMsgErr = dynamic_cast<CMsgErr*>(pMsg);
                if( pMsgErr == nullptr )
                {
                    throw CException(__FILE__,__LINE__,EResultInvalidMessage);
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
                            //case ERequestConnect:
                            //{
                            //    executeConnectRequest(pReqPostponed);
                            //    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                            //    break;
                            //}
                            //case ERequestDisconnect:
                            //{
                            //    executeDisconnectRequest(pReqPostponed);
                            //    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                            //    break;
                            //}
                            //case ERequestRegister:
                            //{
                            //    executeRegisterRequest(pReqPostponed);
                            //    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                            //    break;
                            //}
                            //case ERequestUnregister:
                            //{
                            //    executeUnregisterRequest(pReqPostponed);
                            //    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                            //    break;
                            //}
                            //case ERequestSelectSchema:
                            //{
                            //    executeSelectSchemaRequest(pReqPostponed);
                            //    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                            //    break;
                            //}
                            //case ERequestUpdateSchema:
                            //{
                            //    executeUpdateSchemaRequest(pReqPostponed);
                            //    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                            //    break;
                            //}
                            //case ERequestSelectData:
                            //{
                            //    executeSelectDataRequest(pReqPostponed);
                            //    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                            //    break;
                            //}
                            //case ERequestUpdateData:
                            //{
                            //    executeUpdateDataRequest(pReqPostponed);
                            //    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                            //    break;
                            //}
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

        else if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeInd )
        {
            switch( pMsg->getMsgType() )
            {
                case EMsgTypeIndDisconnected:
                {
                    ZS::Data::CMsgIndDisconnected* pMsgInd = dynamic_cast<ZS::Data::CMsgIndDisconnected*>(i_pMsg);
                    if( pMsgInd == nullptr )
                    {
                        throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgIndDisconnected == nullptr! " + strAddTrcInfo );
                    }

                    //// The disconnected indication message is only of interest if sent by the connection gateway.
                    //// The command execution gateways may also send disconnected indications after the command
                    //// has been executed. But this does affect the overall connection to the database.
                    //if( pMsgInd->getSender() == m_pGateway )
                    //{
                    //    if( pMsgInd->getSocketId() != 0 )
                    //    {
                    //        QString strAddTrcInfo = "event( ";
                    //        strAddTrcInfo += "MsgIndDisconnected: SocketId = " + QString::number(pMsgInd->getSocketId()) + " != 0 )";
                    //        throw ZS::System::CException( __FILE__, __LINE__, EResultSocketIdOutOfRange, strAddTrcInfo );
                    //    }

                    //    if( CErrLog::GetInstance() != nullptr )
                    //    {
                    //        CErrLog::GetInstance()->addEntry(
                    //            /* dateTime    */ QDateTime::currentDateTime(),
                    //            /* severity    */ EResultSeverityInfo,
                    //            /* iErrNr      */ EResultRemoteHostClosed,
                    //            /* strErrNr    */ ZS::result2Str(EResultRemoteHostClosed),
                    //            /* strSource   */  objectName() + ".event" ,
                    //            /* strAddInfo  */ m_hostSettingsCurr.m_strRemoteHostName + ":" + QString::number(m_hostSettingsCurr.m_uServerListenPort) + ":" + QString::number(m_hostSettingsCurr.m_uRemotePort),
                    //            /* strProposal */ "Check Remote Host Settings" );
                    //    }

                    //    abortAllExecCommandRequests(); // if there are any

                    //    bool bRequestAborted = false;

                    //    pReqInProgress = m_pRequestQueue->getRequestInProgress();

                    //    if( pReqInProgress != nullptr && pReqInProgress->getRequest() == ERequestDisconnect )
                    //    {
                    //        if( m_state == EStateWaitingForDisconnected )
                    //        {
                    //            // There might be further pending request which have to be executed
                    //            // after emitting the "stateChanged" and "disconnected" signals ..
                    //            bRequestAborted = true;

                    //            pReqInProgress->setErrResultInfo(errResultInfo);
                    //            pReqInProgress->setProgressInPerCent(100);
                    //            pReqInProgress->update();

                    //            CMsgCon* pMsgConReq = pReqInProgress->takeConfirmationMessage();

                    //            // Invalidate request in progress after emitting signals so that
                    //            // "isRequestInProgress" returns true.
                    //            m_pRequestQueue->setRequestInProgress(nullptr);

                    //            deleteRequest(pReqInProgress);
                    //            pReqInProgress = nullptr;

                    //            if( pMsgConReq != nullptr )
                    //            {
                    //                pMsgConReq->setErrResultInfo(errResultInfo);
                    //                POST_OR_DELETE_MESSAGE(pMsgConReq,&mthTracer);
                    //                pMsgConReq = nullptr;
                    //            }
                    //        }
                    //    }

                    //    if( m_pGatewayThread != nullptr )
                    //    {
                    //        m_state = EStateWaitingForGatewayDeleted;

                    //        if( m_pGatewayThread->isRunning() )
                    //        {
                    //            m_pGatewayThread->quit();
                    //            m_pGatewayThread->wait(10000+m_hostSettingsCurr.m_iConnectTimeout_ms);
                    //        }
                    //        // Still running (wait timed out) ..
                    //        if( m_pGatewayThread->isRunning() )
                    //        {
                    //            m_pGatewayThread->deleteLater();
                    //        }
                    //        else
                    //        {
                    //            try
                    //            {
                    //                delete m_pGatewayThread;
                    //            }
                    //            catch(...)
                    //            {
                    //            }
                    //        }
                    //        m_pGatewayThread = nullptr;
                    //        m_pGateway = nullptr;
                    //    }

                    //    m_state = EStateUnconnected;

                    //    if( bRequestAborted && !m_pRequestQueue->isRequestInProgress() && m_pRequestQueue->hasPostponedRequest() )
                    //    {
                    //        // The current request has been refused but executed. Check if there are pending requests.
                    //        executeNextPostponedRequest();
                    //    }
                    //} // if( pMsgInd->getSender() == m_pGateway )
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
            } // switch( pMsg->getMsgType() )
        } // if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeInd )

        else if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeCon )
        {
            ZS::System::CMsgCon* pMsgCon = dynamic_cast<ZS::System::CMsgCon*>(pMsg);
            if( pMsgCon == nullptr )
            {
                throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
            }

            CRequest* pReq = m_pRequestQueue->getRequestInProgress(pMsgCon->getRequestId());

            if( pReq != nullptr )
            {
                switch( pMsg->getMsgType() )
                {
                    case EMsgTypeConConnect:
                    {
                        ZS::Data::CMsgConConnect* pMsgCon = dynamic_cast<ZS::Data::CMsgConConnect*>(i_pMsg);
                        if( pMsgCon == nullptr )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgConConnect == nullptr! " + strAddTrcInfo );
                        }

                        // Please note that on changing the protocol type the gateway has been replaced
                        // and that a connect request message has been sent to the newly created gateway
                        // sending back the connect confirmation message. So also the connection confirmation
                        // may be received while the change settings request is in progress.
                        if( m_socketTypeOrig == ESocketTypeInProcMsg )
                        {
                            m_cnctId = SCnctId(m_protocolTypeOrig, m_socketTypeOrig, this);
                        }
                        else
                        {
                            m_cnctId = SCnctId(m_protocolTypeOrig, m_socketTypeOrig, pMsgCon->getSocketId());
                        }
                        m_socketDscr = pMsgCon->getSocketDscr();
                        break;
                    } // case EMsgTypeConConnect

                    case EMsgTypeConDisconnect:
                    {
                        ZS::Data::CMsgConDisconnect* pMsgCon = dynamic_cast<ZS::Data::CMsgConDisconnect*>(i_pMsg);
                        if( pMsgCon == nullptr )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgConDisconnect == nullptr! " + strAddTrcInfo );
                        }

                        m_cnctId = SCnctId();
                        m_socketDscr = SSocketDscr();

                        //errResultInfo = pMsgCon->getErrResultInfo();

                        //pReqInProgress = m_pRequestQueue->getRequestInProgress();

                        //// If we received the confirmation message to our request message
                        //// (and if the request has not been aborted in the meantime) ..
                        //if( pReqInProgress != nullptr && pReqInProgress->getId() == pMsgCon->getRequestId() )
                        //{
                        //    if( pReqInProgress->getRequest() == ERequestDisconnect )
                        //    {
                        //        if( m_state == EStateWaitingForDisconnected )
                        //        {
                        //            if( m_pGatewayThread != nullptr )
                        //            {
                        //                m_state = EStateWaitingForGatewayDeleted;

                        //                if( m_pGatewayThread->isRunning() )
                        //                {
                        //                    m_pGatewayThread->quit();
                        //                    m_pGatewayThread->wait(10000+m_hostSettingsCurr.m_iConnectTimeout_ms);
                        //                }
                        //                if( m_pGatewayThread->isRunning() )
                        //                {
                        //                    m_pGatewayThread->deleteLater();
                        //                }
                        //                else
                        //                {
                        //                    try
                        //                    {
                        //                        delete m_pGatewayThread;
                        //                    }
                        //                    catch(...)
                        //                    {
                        //                    }
                        //                }

                        //                m_pGatewayThread = nullptr;
                        //                m_pGateway = nullptr;

                        //            } // if( m_pGatewayThread != nullptr )

                        //            m_state = EStateUnconnected;

                        //            // Progress = 100 % indicates "requestFinished".
                        //            // Please note that on emitting the "changed" signal further requests
                        //            // may be started as a reentry by the receiver(s) of the signal. Those
                        //            // requests must be postponed but will be executed on calling
                        //            // "executeNextPostponedRequest" below. So we keep the request in progress
                        //            // in the request queue while executing the changed signal.
                        //            pReqInProgress->setErrResultInfo(pMsgCon->getErrResultInfo());
                        //            pReqInProgress->setProgressInPerCent(100);
                        //            pReqInProgress->update();

                        //            CMsgCon* pMsgConReq = pReqInProgress->takeConfirmationMessage();

                        //            // The current request has been executed. On emitting signals a new
                        //            // request may have already been started and postponed ...
                        //            m_pRequestQueue->setRequestInProgress(nullptr);

                        //            deleteRequest(pReqInProgress);
                        //            pReqInProgress = nullptr;

                        //            if( pMsgConReq != nullptr )
                        //            {
                        //                pMsgConReq->setErrResultInfo(errResultInfo);
                        //                POST_OR_DELETE_MESSAGE(pMsgConReq,&mthTracer);
                        //                pMsgConReq = nullptr;
                        //            }
                        //            if( m_pRequestQueue->hasPostponedRequest() )
                        //            {
                        //                executeNextPostponedRequest();
                        //            }

                        //        } // if( m_state == EStateWaitingForDisconnected )
                        //    } // if( pReqInProgress->getRequest() == ERequestDisconnect )
                        //} // if( pReqInProgress != nullptr && m_pRequestQueue->isPendingRequest(pMsgCon->getMsgId()) )
                        break;
                    } // case EMsgTypeConDisconnect

                    case EMsgTypeConChangeSettings:
                    {
                        ZS::Data::CMsgConChangeSettings* pMsgCon = dynamic_cast<ZS::Data::CMsgConChangeSettings*>(i_pMsg);
                        if( pMsgCon == nullptr )
                        {
                            throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgConChangedSettings == nullptr! " + strAddTrcInfo );
                        }

                        m_socketDscr = pMsgCon->getClientSocketDscr();

                        //errResultInfo = pMsgCon->getErrResultInfo();

                        //pReqInProgress = m_pRequestQueue->getRequestInProgress();

                        //// If we received the confirmation message to our request message
                        //// (and if the request has not been aborted in the meantime) ..
                        //if( pReqInProgress != nullptr && pReqInProgress->getId() == pMsgCon->getRequestId() )
                        //{
                        //    if( pReqInProgress->getRequest() == ERequestUpdateHostSettings )
                        //    {
                        //        if( m_state == EStateWaitingForSettingsChanged )
                        //        {
                        //            m_errResultInfoLastConnectRequest = errResultInfo;

                        //            if( errResultInfo.isErrorResult() )
                        //            {
                        //                if( CErrLog::GetInstance() != nullptr && !errResultInfo.isAddedToErrLogModel() )
                        //                {
                        //                    CErrLog::GetInstance()->addEntry(
                        //                        /* dateTime    */ QDateTime::currentDateTime(),
                        //                        /* severity    */ errResultInfo.getSeverity(),
                        //                        /* iErrNr      */ errResultInfo.getResult(),
                        //                        /* strErrNr    */ errResultInfo.getResultStr(),
                        //                        /* strSource   */ errResultInfo.getErrSource(),
                        //                        /* strAddInfo  */ errResultInfo.getAddErrInfoDscr(),
                        //                        /* strProposal */ "Check Remote Host Settings" );
                        //                    errResultInfo.setAddedToErrLogModel();
                        //                }
                        //            }
                        //            else
                        //            {
                        //                m_hostSettingsCurr = pMsgCon->getConnectionSettings();
                        //                m_watchDogSettingsCurr = pMsgCon->getWatchDogSettings();
                        //            }

                        //            // Progress = 100 % indicates "requestFinished".
                        //            // Please note that on emitting the "changed" signal further requests
                        //            // may be started as a reentry by the receiver(s) of the signal. Those
                        //            // requests must be postponed but will be executed on calling
                        //            // "executeNextPostponedRequest" below. So we keep the request in progress
                        //            // in the request queue while executing the changed signal.
                        //            pReqInProgress->setErrResultInfo(errResultInfo);
                        //            pReqInProgress->setProgressInPerCent(100);
                        //            pReqInProgress->update();

                        //            CMsgCon* pMsgConReq = pReqInProgress->takeConfirmationMessage();

                        //            // The current request has been executed. On emitting signals a new
                        //            // request may have already been started and postponed ...
                        //            m_pRequestQueue->setRequestInProgress(nullptr);

                        //            deleteRequest(pReqInProgress);
                        //            pReqInProgress = nullptr;

                        //            if( pMsgConReq != nullptr )
                        //            {
                        //                pMsgConReq->setErrResultInfo(errResultInfo);
                        //                POST_OR_DELETE_MESSAGE(pMsgConReq,&mthTracer);
                        //                pMsgConReq = nullptr;
                        //            }
                        //            if( m_pRequestQueue->hasPostponedRequest() )
                        //            {
                        //                executeNextPostponedRequest();
                        //            }

                        //        } // if( m_state == EStateWaitingForSettingsChanged )
                        //    } // if( pReqInProgress->getRequest() == ERequestUpdateHostSettings )
                        //} // if( pReqInProgress != nullptr && m_pRequestQueue->isPendingRequest(pMsgCon->getMsgId()) )
                        break;
                    } // case EMsgTypeConChangeSettings

                    case EMsgTypeConRegister:
                    {
                        break;
                    }

                    case EMsgTypeConUnregister:
                    {
                        break;
                    }

                    //case EMsgTypeConSelectSchema:
                    //{
                    //    ZS::Data::CMsgConSelectSchema* pMsgConSelectSchema = dynamic_cast<ZS::Data::CMsgConSelectSchema*>(pMsgCon);
                    //    if( pMsgConSelectSchema == nullptr )
                    //    {
                    //        throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgConSelectSchema == nullptr! " + strAddTrcInfo );
                    //    }
                    //    onReceivedMsgConSelectSchema(pMsgConSelectSchema);
                    //    break;
                    //}

                    //case EMsgTypeConUpdateSchema:
                    //{
                    //    ZS::Data::CMsgConUpdateSchema* pMsgConUpdateSchema = dynamic_cast<ZS::Data::CMsgConUpdateSchema*>(pMsgCon);
                    //    if( pMsgConUpdateSchema == nullptr )
                    //    {
                    //        throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgConUpdateSchema == nullptr! " + strAddTrcInfo );
                    //    }
                    //    onReceivedMsgConUpdateSchema(pMsgConUpdateSchema);
                    //    break;
                    //}

                    //case EMsgTypeConSelectData:
                    //{
                    //    ZS::Data::CMsgConSelectData* pMsgConSelectData = dynamic_cast<ZS::Data::CMsgConSelectData*>(pMsgCon);
                    //    if( pMsgConSelectData == nullptr )
                    //    {
                    //        throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgConSelectData == nullptr! " + strAddTrcInfo );
                    //    }
                    //    onReceivedMsgConSelectData(pMsgConSelectData);
                    //    break;
                    //}

                    //case EMsgTypeConUpdateData:
                    //{
                    //    ZS::Data::CMsgConUpdateData* pMsgConUpdateData = dynamic_cast<ZS::Data::CMsgConUpdateData*>(pMsgCon);
                    //    if( pMsgConUpdateData == nullptr )
                    //    {
                    //        throw ZS::System::CException( __FILE__, __LINE__, EResultMessageTypeMismatch, "MsgConUpdateData == nullptr! " + strAddTrcInfo );
                    //    }
                    //    onReceivedMsgConUpdateData(pMsgConUpdateData);
                    //    break;
                    //}

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
                } // switch( pMsg->getMsgType() )

                SErrResultInfo errResultInfo     = pMsgCon->getErrResultInfo();
                int            iProgress_perCent = pMsgCon->getProgress();

                m_pRequestQueue->lock();

                pReq->setErrResultInfo(errResultInfo);
                pReq->setProgressInPerCent(iProgress_perCent);
                pReq->lock();
                pReq->update(); // "onRequestChanged" is called as a reentry removing the request from the queue if request is finished.
                pReq->unlock();

                if( iProgress_perCent >= 100 )
                {
                    // The one who calls update for the finished request must delete the request.
                    deleteRequest(pReq); // The "onRequestChanged" slot will be called again. The request has already been removed from the queue.
                    pReq = nullptr;
                }
                m_pRequestQueue->unlock();

            } // if( pReq != nullptr )
        } // if( pMsg->getSystemMsgType() == ZS::System::MsgProtocol::ESystemMsgTypeCon )

        if( bWasConnected && !isConnected() )
        {
            emit disconnected(this,m_socketDscr);
        }
        else if( !bWasConnected && isConnected() )
        {
            emit connected(this,m_socketDscr);
        }

        if( statePrev != m_state )
        {
            emit stateChanged(this,m_state);
        }

        if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
        {
            int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
            strAddTrcInfo  = "State: " + State2Str(m_state);
            strAddTrcInfo += ", ReqQueue {" + m_pRequestQueue->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
            mthTracer.trace(strAddTrcInfo);
        }

    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;

} // event
