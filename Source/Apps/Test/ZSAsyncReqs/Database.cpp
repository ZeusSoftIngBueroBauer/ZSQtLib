/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qtimer.h>
#include <QtCore/qwaitcondition.h>
#include <QtNetwork/qhostinfo.h>

#include "Database.h"
#include "DatabaseGateway.h"
#include "Msg.h"

#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysRequestSequencer.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysSleeperThread.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Apps::Test::AsyncRequests;
using namespace ZS::Apps::Test::AsyncRequests::Database;


/*******************************************************************************
Type definitions and constants
*******************************************************************************/

/*==============================================================================
enum Database::EState
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrStates[EStateCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( EStateCreated,      "Created"      ),
    /* 1 */ SEnumEntry( EStateRunning,      "Running"      ),
    /* 2 */ SEnumEntry( EStateDisconnected, "Disconnected" ),
    /* 3 */ SEnumEntry( EStateConnected,    "Connected"    ),
    /* 4 */ SEnumEntry( EStateRegistered,   "Registered"   )
};

//------------------------------------------------------------------------------
QString Database::State2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str( s_arEnumStrStates, EStateCount, i_iState );
}

/*==============================================================================
enum Database::ERequest
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrRequests[ERequestCount] =
//------------------------------------------------------------------------------
{
    /*  0 */ SEnumEntry( ERequestNone,              "None"              ),
    /*  1 */ SEnumEntry( ERequestStartup,           "Startup"           ),
    /*  2 */ SEnumEntry( ERequestShutdown,          "Shutdown"          ),
    /*  3 */ SEnumEntry( ERequestReadSchema,        "ReadSchema"        ),
    /*  4 */ SEnumEntry( ERequestSaveSchema,        "SaveSchema"        ),
    /*  5 */ SEnumEntry( ERequestReadData,          "ReadData"          ),
    /*  6 */ SEnumEntry( ERequestSaveData,          "SaveData"          ),
    /*  7 */ SEnumEntry( ERequestSet2Default,       "Set2Default"       ),
    /*  8 */ SEnumEntry( ERequestChangeSettings,    "ChangeSettings"    ),
    /*  9 */ SEnumEntry( ERequestConnect,           "Connect"           ),
    /* 10 */ SEnumEntry( ERequestDisconnect,        "Disconnect"        ),
    /* 11 */ SEnumEntry( ERequestRegister,          "Register"          ),
    /* 12 */ SEnumEntry( ERequestUnregister,        "Unregister"        ),
    /* 13 */ SEnumEntry( ERequestSelectSchema,      "SelectSchema"      ),
    /* 14 */ SEnumEntry( ERequestUpdateSchema,      "UpdateSchema"      ),
    /* 15 */ SEnumEntry( ERequestSelectData,        "SelectData"        ),
    /* 16 */ SEnumEntry( ERequestUpdateData,        "UpdateData"        ),
    /* 17 */ SEnumEntry( ERequestStartThread,       "StartThread"       ),
    /* 18 */ SEnumEntry( ERequestStopThread,        "StopThread"        ),
    /* 19 */ SEnumEntry( ERequestDbStartup,         "DbStartup"         ),
    /* 20 */ SEnumEntry( ERequestDbShutdown,        "DbShutdown"        ),
    /* 21 */ SEnumEntry( ERequestDbConnect,         "DbConnect"         ),
    /* 22 */ SEnumEntry( ERequestDbDisconnect,      "DbDisconnect"      ),
    /* 23 */ SEnumEntry( ERequestDbUpdateSchema,    "DbUpdateSchema"    ),
    /* 24 */ SEnumEntry( ERequestDbCltUpdateSchema, "DbCltUpdateSchema" ),
    /* 25 */ SEnumEntry( ERequestDbUpdateData,      "DbUpdateData"      ),
    /* 26 */ SEnumEntry( ERequestDbCltUpdateData,   "DbCltUpdateData"   )
};

//------------------------------------------------------------------------------
QString Database::Request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str( s_arEnumStrRequests, ERequestCount, i_iRequest );
}

/*==============================================================================
enum Database::ETimeout
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::System::SEnumEntry s_arEnumStrTimeouts[ETimeoutCount] =
//------------------------------------------------------------------------------
{
    /* 0 */ SEnumEntry( ETimeoutInfinite, "Infinite", c_iTimeoutInfinite_ms ),
    /* 1 */ SEnumEntry( ETimeoutShort,    "Short",    c_iTimeoutShort_ms    ),
    /* 2 */ SEnumEntry( ETimeoutMedium,   "Medium",   c_iTimeoutMedium_ms   ),
    /* 3 */ SEnumEntry( ETimeoutLong,     "Long",     c_iTimeoutLong_ms     )
};

//------------------------------------------------------------------------------
QString Database::timeout2Str( int i_iTimeout )
//------------------------------------------------------------------------------
{
    return ZS::System::SEnumEntry::enumerator2Str( s_arEnumStrTimeouts, ETimeoutCount, i_iTimeout );
}

//------------------------------------------------------------------------------
ETimeout Database::str2Timeout( const QString& i_str )
//------------------------------------------------------------------------------
{
    ETimeout timeout = ETimeoutUndefined;
    int iVal = SEnumEntry::str2Enumerator(s_arEnumStrTimeouts,ETimeoutCount,i_str);
    if( iVal >= 0 && iVal < ETimeoutCount )
    {
        timeout = static_cast<ETimeout>(iVal);
    }
    return timeout;

} // str2Timeout

//------------------------------------------------------------------------------
int Database::getTimeoutInMs( int i_iTimeout )
//------------------------------------------------------------------------------
{
    int iTimeout_ms = c_iTimeoutInfinite_ms;

    if( i_iTimeout >= 0 && i_iTimeout < ETimeoutCount )
    {
        iTimeout_ms = ZS::System::SEnumEntry::enumerator2Val( s_arEnumStrTimeouts, ETimeoutCount, i_iTimeout ).toInt();
    }
    return iTimeout_ms;

} // getTimeoutInMs

//------------------------------------------------------------------------------
int Database::getTimeoutInMs( const QString& i_str )
//------------------------------------------------------------------------------
{
    int iTimeout_ms = c_iTimeoutInfinite_ms;

    int iTimeout = ZS::System::SEnumEntry::str2Enumerator( s_arEnumStrTimeouts, ETimeoutCount, i_str );

    if( iTimeout >= 0 && iTimeout < ETimeoutCount )
    {
        iTimeout_ms = ZS::System::SEnumEntry::enumerator2Val( s_arEnumStrTimeouts, ETimeoutCount, iTimeout ).toInt();
    }
    return iTimeout_ms;

} // getTimeoutInMs


namespace ZS
{
namespace Apps
{
namespace Test
{
namespace AsyncRequests
{
/*******************************************************************************
struct SDbCltSocketDscr
*******************************************************************************/

//------------------------------------------------------------------------------
struct SDbCltSocketDscr
//------------------------------------------------------------------------------
{
public: // ctors
    SDbCltSocketDscr( Ipc::ESocketType i_socketType, QObject* i_pDbCltOrGtw, int i_idxDbCltOrSocketId ) :
        m_socketType(i_socketType),
        m_pDbCltOrGtw(i_pDbCltOrGtw),
        m_idxDbCltOrSocketId(i_idxDbCltOrSocketId)
    {
    }
public: // struct methods
    QString getAddTrcInfoStr() const
    {
        QString strAddTrcInfo = "SocketType: " + Ipc::socketType2Str(m_socketType);
        strAddTrcInfo += ", Clt: " + m_pDbCltOrGtw->objectName();
        strAddTrcInfo += ", CltId: " + QString::number(m_idxDbCltOrSocketId);
        return strAddTrcInfo;
    }
public: // struct members
    Ipc::ESocketType m_socketType;
    QObject*         m_pDbCltOrGtw;
    int              m_idxDbCltOrSocketId;

}; // struct SDbCltSocketDscr

} // namespace AsyncRequests

} // namespace Test

} // namespace Apps

} // namespace ZS


/*******************************************************************************
class CDb : public QObject
*******************************************************************************/

/*==============================================================================
public: // class methods (startup request sequencer execution functions)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::StartGatewayThread( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, QVariant& i_valExec )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        bool bConverted = false;

        int iSocketType = i_valExec.toInt(&bConverted);

        if( bConverted && iSocketType >= 0 && iSocketType < Ipc::ESocketTypeCount )
        {
            Ipc::ESocketType socketType = static_cast<Ipc::ESocketType>(iSocketType);
            pReq = pDb->startGatewayThread( i_iTimeout_ms, i_bWait, i_iReqIdParent, socketType );
        }
    }
    return pReq;

} // StartGatewayThread

//------------------------------------------------------------------------------
CRequest* CDb::StopGatewayThread( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, QVariant& i_valExec )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        bool bConverted = false;

        int iSocketType = i_valExec.toInt(&bConverted);

        if( bConverted && iSocketType >= 0 && iSocketType < Ipc::ESocketTypeCount )
        {
            Ipc::ESocketType socketType = static_cast<Ipc::ESocketType>(iSocketType);
            pReq = pDb->stopGatewayThread( i_iTimeout_ms, i_bWait, i_iReqIdParent, socketType );
        }
    }
    return pReq;

} // StopGatewayThread

//------------------------------------------------------------------------------
CRequest* CDb::StartupGateway( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, QVariant& i_valExec )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        bool bConverted = false;

        int iSocketType = i_valExec.toInt(&bConverted);

        if( bConverted && iSocketType >= 0 && iSocketType < Ipc::ESocketTypeCount )
        {
            Ipc::ESocketType socketType = static_cast<Ipc::ESocketType>(iSocketType);
            pReq = pDb->startupGateway( i_iTimeout_ms, i_bWait, i_iReqIdParent, socketType );
        }
    }
    return pReq;

} // StartupGateway

//------------------------------------------------------------------------------
CRequest* CDb::ShutdownGateway( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, QVariant& i_valExec )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        bool bConverted = false;

        int iSocketType = i_valExec.toInt(&bConverted);

        if( bConverted && iSocketType >= 0 && iSocketType < Ipc::ESocketTypeCount )
        {
            Ipc::ESocketType socketType = static_cast<Ipc::ESocketType>(iSocketType);
            pReq = pDb->shutdownGateway( i_iTimeout_ms, i_bWait, i_iReqIdParent, socketType );
        }
    }
    return pReq;

} // ShutdownGateway

/*==============================================================================
public: // class methods (startup request sequencer execution functions)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::ReadSchema( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        pReq = pDb->readSchema(i_iReqIdParent);
    }
    return pReq;

} // ReadSchema

//------------------------------------------------------------------------------
CRequest* CDb::ReadData( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        pReq = pDb->readData(i_iReqIdParent);
    }
    return pReq;

} // ReadData

//------------------------------------------------------------------------------
CRequest* CDb::Set2Default( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        pReq = pDb->set2Default(i_iReqIdParent);
    }
    return pReq;

} // Set2Default

/*==============================================================================
public: // class methods (shutdown request sequencer execution functions)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::SaveSchema( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        pReq = pDb->saveSchema(i_iReqIdParent);
    }
    return pReq;

} // SaveSchema

//------------------------------------------------------------------------------
CRequest* CDb::SaveData( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        pReq = pDb->saveData(i_iReqIdParent);
    }
    return pReq;

} // SaveData

/*==============================================================================
public: // class methods (execute commands request sequencer execution functions)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::UpdateDbSchema( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        pReq = pDb->updateDbSchema(i_iReqIdParent);
    }
    return pReq;

} // UpdateDbSchema

//------------------------------------------------------------------------------
CRequest* CDb::UpdateDbCltSchema( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        pReq = pDb->updateDbCltSchema( i_iTimeout_ms, i_bWait, i_iReqIdParent, reinterpret_cast<SDbCltSocketDscr*>(i_pvExec) );
    }
    return pReq;

} // UpdateDbCltSchema

/*==============================================================================
public: // class methods (execute commands request sequencer execution functions)
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::UpdateDbData( QObject* i_pObjFctExecute, int /*i_iTimeout_ms*/, bool /*i_bWait*/, qint64 i_iReqIdParent, void* /*i_pvExec*/ )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        pReq = pDb->updateDbData(i_iReqIdParent);
    }
    return pReq;

} // UpdateDbData

//------------------------------------------------------------------------------
CRequest* CDb::UpdateDbCltData( QObject* i_pObjFctExecute, int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, void* i_pvExec )
//------------------------------------------------------------------------------
{
    CRequest* pReq = nullptr;

    CDb* pDb = dynamic_cast<CDb*>(i_pObjFctExecute);

    if( pDb != nullptr )
    {
        pReq = pDb->updateDbCltData( i_iTimeout_ms, i_bWait, i_iReqIdParent, reinterpret_cast<SDbCltSocketDscr*>(i_pvExec) );
    }
    return pReq;

} // UpdateDbCltData

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDb::CDb( const QString& i_strDbObjName ) :
//------------------------------------------------------------------------------
    QObject(),
    // Direct message connections to clients (message dispatch):
    m_arpDbCltsConnected(),
    m_arpDbCltsRegistered(),
    // Connections to clients via gateways (e.g. through TCP/IP sockets):
    m_arHostSettingsGtw(Ipc::ESocketTypeCount),
    m_arpDbGtwThread(Ipc::ESocketTypeCount),
    m_arpDbGtw(Ipc::ESocketTypeCount),
    m_arhshSocketDscrsDbCltsConnected(Ipc::ESocketTypeCount),
    m_arariSocketIdsDbCltsRegistered(Ipc::ESocketTypeCount),
    // Requests:
    m_pReqSeqrStartup(nullptr),
    m_pReqSeqrShutdown(nullptr),
    m_pReqSeqrExecCmd(nullptr),
    m_requestQueue(this,true),
    m_reqDscrTimeout(),
    m_pTmrReqTimeout(nullptr),
    m_bMsgReqContinuePending(false),
    // Simulating asynchronous requests:
    m_reqSimInProgress(ERequestNone),
    m_iReqSimDurationMin_ms(100),
    m_iReqSimDurationMax_ms(200),
    m_iReqSimDurationStepsMax(5),
    m_iReqSimDurationStepsCurr(0),
    m_bReqModeSingleShot(false),
    m_bReqSingleShot(false),
    m_pTrcAdminObj(nullptr)
{
    int iSocketType;

    setObjectName(i_strDbObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::Apps::Test::AsyncRequests", "CDb", objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    Ipc::SServerHostSettings hostSettings;

    //hostSettings.m_protocolType = Data::EProtocolTypeZSXML;
    hostSettings.m_socketType = Ipc::ESocketTypeUndefined;
    // Socket connection parameters (valid after connection is established)
    hostSettings.m_strLocalHostName = QHostInfo::localHostName();
    hostSettings.m_uLocalPort = 19673;
    hostSettings.m_uMaxPendingConnections = 30;
    hostSettings.m_uBufferSize = 4096;
    // Database specific parameters
    //hostSettings.m_strDriverType = "";
    //hostSettings.m_bDatabaseNameUsed = false;
    //hostSettings.m_strDatabaseName = "";
    //hostSettings.m_bUserNameUsed = false;
    //hostSettings.m_strUserName = "";
    //hostSettings.m_bPasswordUsed = false;
    //hostSettings.m_strPassword = "";

    for( iSocketType = 0; iSocketType < Ipc::ESocketTypeCount; iSocketType++ )
    {
        hostSettings.m_socketType = static_cast<Ipc::ESocketType>(iSocketType);

        m_arHostSettingsGtw[iSocketType] = hostSettings;
    }

    m_pTmrReqTimeout = new QTimer(this);

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrReqTimeout,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Want to trace the sequencer. So it must be created after the trace server.
    // But before the central widget which creates the models accessing the sequencers.
    // In addition the request sequencers must be destroyed before the request queue.
    // On clearing the sequencer the sequence might access (abort) requests
    // which have already been destroyed by the request queue.
    m_pReqSeqrStartup = new CRequestSequencer(
        /* strObjName             */ "ZS::Apps::Test::AsyncRequests::CDb::" + objectName() + ".startup",
        /* pObjParent             */ this,
        /* bKeepReqDscrInExecTree */ false );

    m_pReqSeqrShutdown = new CRequestSequencer(
        /* strObjName             */ "ZS::Apps::Test::AsyncRequests::CDb::" + objectName() + "shutdown",
        /* pObjParent             */ this,
        /* bKeepReqDscrInExecTree */ false );

    // <RequestSequencer> Startup
    //----------------------------

    qint64 iReqIdStartGatewayThreads = m_pReqSeqrStartup->addGroup(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDb",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestStartThread,
        /* strRequest               */ Request2Str(ERequestStartThread),
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ -1 );

    for( iSocketType = 0; iSocketType < Ipc::ESocketTypeCount; iSocketType++ )
    {
        qint64 iReqIdStartGatewayThread = m_pReqSeqrStartup->addRequest(
            /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
            /* strCreatorClassName      */ "CDb",
            /* pObjCreator              */ this,
            /* iRequest                 */ ERequestStartThread,
            /* strRequest               */ Request2Str(ERequestStartThread),
            /* pvObjFctExecute          */ this,
            /* fctExecute               */ StartGatewayThread,
            /* valExec                  */ iSocketType,
            /* iReqIdParent             */ iReqIdStartGatewayThreads,
            /* iReqIdPredecessor        */ -1 );

        /*qint64 iReqIdStartupGateway =*/ m_pReqSeqrStartup->addRequest(
            /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
            /* strCreatorClassName      */ "CDb",
            /* pObjCreator              */ this,
            /* iRequest                 */ ERequestDbStartup,
            /* strRequest               */ Request2Str(ERequestDbStartup),
            /* pvObjFctExecute          */ this,
            /* fctExecute               */ StartupGateway,
            /* valExec                  */ iSocketType,
            /* iReqIdParent             */ iReqIdStartGatewayThreads,
            /* iReqIdPredecessor        */ iReqIdStartGatewayThread );
    }

    qint64 iReqIdReadSchema = m_pReqSeqrStartup->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDb",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestReadSchema,
        /* strRequest               */ Request2Str(ERequestReadSchema),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ ReadSchema,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdStartGatewayThreads );

    qint64 iReqIdReadData = m_pReqSeqrStartup->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDb",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestReadData,
        /* strRequest               */ Request2Str(ERequestReadData),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ ReadData,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdReadSchema );

    /*qint64 iReqIdSet2Default =*/ m_pReqSeqrStartup->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDb",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestSet2Default,
        /* strRequest               */ Request2Str(ERequestSet2Default),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ Set2Default,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdReadData );

    // <RequestSequencer> Shutdown
    //----------------------------

    qint64 iReqIdSaveData = m_pReqSeqrShutdown->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDb",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestSaveData,
        /* strRequest               */ Request2Str(ERequestSaveData),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ SaveData,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ -1 );

    qint64 iReqIdSaveSchema = m_pReqSeqrShutdown->addRequest(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDb",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestSaveSchema,
        /* strRequest               */ Request2Str(ERequestSaveSchema),
        /* pvObjFctExecute          */ this,
        /* fctExecute               */ SaveSchema,
        /* pvExec                   */ nullptr,
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdSaveData );

    qint64 iReqIdStopGatewayThreads = m_pReqSeqrShutdown->addGroup(
        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strCreatorClassName      */ "CDb",
        /* pObjCreator              */ this,
        /* iRequest                 */ ERequestStopThread,
        /* strRequest               */ Request2Str(ERequestStopThread),
        /* iReqIdParent             */ -1,
        /* iReqIdPredecessor        */ iReqIdSaveSchema );

    for( iSocketType = 0; iSocketType < Ipc::ESocketTypeCount; iSocketType++ )
    {
        qint64 iReqIdShutdownGateway = m_pReqSeqrShutdown->addRequest(
            /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
            /* strCreatorClassName      */ "CDb",
            /* pObjCreator              */ this,
            /* iRequest                 */ ERequestDbShutdown,
            /* strRequest               */ Request2Str(ERequestDbShutdown),
            /* pvObjFctExecute          */ this,
            /* fctExecute               */ ShutdownGateway,
            /* valExec                  */ iSocketType,
            /* iReqIdParent             */ iReqIdStopGatewayThreads,
            /* iReqIdPredecessor        */ -1 );

        /*qint64 iReqIdStopGatewayThread =*/ m_pReqSeqrShutdown->addRequest(
            /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
            /* strCreatorClassName      */ "CDb",
            /* pObjCreator              */ this,
            /* iRequest                 */ ERequestStopThread,
            /* strRequest               */ Request2Str(ERequestStopThread),
            /* pvObjFctExecute          */ this,
            /* fctExecute               */ StopGatewayThread,
            /* valExec                  */ iSocketType,
            /* iReqIdParent             */ iReqIdStopGatewayThreads,
            /* iReqIdPredecessor        */ iReqIdShutdownGateway );
    }

} // ctor

//------------------------------------------------------------------------------
CDb::~CDb()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // The request sequencers must be destroyed before the request queue.
    // On clearing the sequencer the sequence might access (abort) requests
    // which have already been destroyed by the request queue.

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("dtor");

    try
    {
        delete m_pReqSeqrExecCmd;
    }
    catch(...)
    {
    }
    m_pReqSeqrExecCmd = nullptr;

    try
    {
        delete m_pReqSeqrShutdown;
    }
    catch(...)
    {
    }
    m_pReqSeqrShutdown = nullptr;

    try
    {
        delete m_pReqSeqrStartup;
    }
    catch(...)
    {
    }
    m_pReqSeqrStartup = nullptr;

    int iSocketType;

    for( iSocketType = 0; iSocketType < Ipc::ESocketTypeCount; iSocketType++ )
    {
        if( m_arpDbGtwThread[iSocketType] != nullptr )
        {
            m_arpDbGtwThread[iSocketType]->quit();

            if( !m_arpDbGtwThread[iSocketType]->wait(5000) )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultTimeout);
                errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );

                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
                m_arpDbGtwThread[iSocketType]->deleteLater();
                m_arpDbGtwThread[iSocketType] = nullptr;
            }
            else
            {
                try
                {
                    delete m_arpDbGtwThread[iSocketType];
                }
                catch(...)
                {
                }
                m_arpDbGtwThread[iSocketType] = nullptr;
            }
        } // if( m_arpDbGtwThread[iSocketType] != nullptr )

        m_arpDbGtw[iSocketType] = nullptr;
    }

    m_pTmrReqTimeout = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::setReqModeSingleShot( bool i_bModeSingleShot )
//------------------------------------------------------------------------------
{
    m_bReqModeSingleShot = i_bModeSingleShot;
}

//------------------------------------------------------------------------------
void CDb::executeReqSingleShot()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqSimInProgress: " + Request2Str(m_reqSimInProgress);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeReqSingleShot",
        /* strAddInfo   */ strAddTrcInfo );

    m_bReqSingleShot = true;

    if( m_reqSimInProgress != ERequestNone )
    {
        double fTimeElapsed_us = ZS::System::Time::getProcTimeInMicroSec();
        double fTimeElapsed_ms = ZS::System::Time::getProcTimeInMilliSec()+0.001;
        double fSRand = 1000.0*fTimeElapsed_ms - fTimeElapsed_us;
        int    iSRand = 100+static_cast<int>(1000.0*fSRand);

        qsrand(iSRand);

        int iTimeout_ms = m_iReqSimDurationMin_ms + (qrand() % m_iReqSimDurationMax_ms);

        if( iTimeout_ms > 0 )
        {
            QTimer::singleShot( iTimeout_ms, this, SLOT(onExecuteReqSingleShotTimeout()) );
        }
        else
        {
            if( m_bReqModeSingleShot )
            {
                m_bReqSingleShot = true;
            }
            m_iReqSimDurationStepsCurr = m_iReqSimDurationStepsMax;
            onExecuteReqSingleShotTimeout();
        }
    }

} // executeReqSingleShot

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::startup( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startup",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestStartup,
        /* strRequest   */ Request2Str(ERequestStartup),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        m_requestQueue.postponeRequest(pReq);

        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeStartupRequest( pReq, pReqInProgress );
    }

    return pReq;

} // startup

//------------------------------------------------------------------------------
CRequest* CDb::shutdown( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "shutdown",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestShutdown,
        /* strRequest   */ Request2Str(ERequestShutdown),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeShutdownRequest( pReq, pReqInProgress );
    }

    return pReq;

} // shutdown

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::readSchema( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "readSchema",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestReadSchema,
        /* strRequest   */ Request2Str(ERequestReadSchema),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeReadSchemaRequest( pReq, pReqInProgress );
    }

    return pReq;

} // readSchema

//------------------------------------------------------------------------------
CRequest* CDb::saveSchema( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "saveSchema",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestSaveSchema,
        /* strRequest   */ Request2Str(ERequestSaveSchema),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeSaveSchemaRequest( pReq, pReqInProgress );
    }

    return pReq;

} // saveSchema

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::readData( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "readData",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestReadData,
        /* strRequest   */ Request2Str(ERequestReadData),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeReadDataRequest( pReq, pReqInProgress );
    }

    return pReq;

} // readData

//------------------------------------------------------------------------------
CRequest* CDb::saveData( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "saveData",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestSaveData,
        /* strRequest   */ Request2Str(ERequestSaveData),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeSaveDataRequest( pReq, pReqInProgress );
    }

    return pReq;

} // saveData

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::set2Default( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "set2Default",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestSet2Default,
        /* strRequest   */ Request2Str(ERequestSet2Default),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_iReqIdParent) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeSet2DefaultRequest( pReq, pReqInProgress );
    }

    return pReq;

} // set2Default

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::onReceivedMsgReqConnect( CMsgReqConnect* i_pMsgReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "MsgReq: " + i_pMsgReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedMsgReqConnect",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestConnect,
        /* strRequest   */ Request2Str(ERequestConnect),
        /* iReqIdParent */ i_pMsgReq->getRequestId(),
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    // On cloning the message no deep copy is necessary as only the sender of the message is needed.
    // But we store the message object as the message will be informed if the sender will be destroyed.
    pReq->setInitiatorRequestMessage( dynamic_cast<CMsgReq*>(i_pMsgReq->clone()) );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_pMsgReq->getRequestId()) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeConnectRequest( pReq, pReqInProgress );
    }

    return pReq;

} // onReceivedMsgReqConnect

//------------------------------------------------------------------------------
CRequest* CDb::onReceivedMsgReqDisconnect( CMsgReqDisconnect* i_pMsgReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "MsgReq: " + i_pMsgReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedMsgReqDisconnect",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestDisconnect,
        /* strRequest   */ Request2Str(ERequestDisconnect),
        /* iReqIdParent */ i_pMsgReq->getRequestId(),
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    // On cloning the message no deep copy is necessary as only the sender of the message is needed.
    // But we store the message object as the message will be informed if the sender will be destroyed.
    pReq->setInitiatorRequestMessage( dynamic_cast<CMsgReq*>(i_pMsgReq->clone()) );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_pMsgReq->getRequestId()) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeDisconnectRequest( pReq, pReqInProgress );
    }

    return pReq;

} // onReceivedMsgReqDisconnect

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::onReceivedMsgReqRegister( CMsgReqRegister* i_pMsgReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "MsgReq: " + i_pMsgReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedMsgReqRegister",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestRegister,
        /* strRequest   */ Request2Str(ERequestRegister),
        /* iReqIdParent */ i_pMsgReq->getRequestId(),
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    // On cloning the message no deep copy is necessary as only the sender of the message is needed.
    // But we store the message object as the message will be informed if the sender will be destroyed.
    pReq->setInitiatorRequestMessage( dynamic_cast<CMsgReq*>(i_pMsgReq->clone()) );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_pMsgReq->getRequestId()) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeRegisterRequest( pReq, pReqInProgress );
    }

    return pReq;

} // onReceivedMsgReqRegister

//------------------------------------------------------------------------------
CRequest* CDb::onReceivedMsgReqUnregister( CMsgReqUnregister* i_pMsgReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "MsgReq: " + i_pMsgReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedMsgReqUnregister",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestUnregister,
        /* strRequest   */ Request2Str(ERequestUnregister),
        /* iReqIdParent */ i_pMsgReq->getRequestId(),
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    // On cloning the message no deep copy is necessary as only the sender of the message is needed.
    // But we store the message object as the message will be informed if the sender will be destroyed.
    pReq->setInitiatorRequestMessage( dynamic_cast<CMsgReq*>(i_pMsgReq->clone()) );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_pMsgReq->getRequestId()) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeUnregisterRequest( pReq, pReqInProgress );
    }

    return pReq;

} // onReceivedMsgReqUnregister

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::onReceivedMsgReqSelectSchema( CMsgReqSelectSchema* i_pMsgReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "MsgReq: " + i_pMsgReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedMsgReqSelectSchema",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestSelectSchema,
        /* strRequest   */ Request2Str(ERequestSelectSchema),
        /* iReqIdParent */ i_pMsgReq->getRequestId(),
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    // On cloning the message no deep copy is necessary as only the sender of the message is needed.
    // But we store the message object as the message will be informed if the sender will be destroyed.
    pReq->setInitiatorRequestMessage( dynamic_cast<CMsgReq*>(i_pMsgReq->clone()) );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_pMsgReq->getRequestId()) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeSelectSchemaRequest( pReq, pReqInProgress );
    }

    return pReq;

} // onReceivedMsgReqSelectSchema

//------------------------------------------------------------------------------
CRequest* CDb::onReceivedMsgReqUpdateSchema( CMsgReqUpdateSchema* i_pMsgReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "MsgReq: " + i_pMsgReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedMsgReqUpdateSchema",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestUpdateSchema,
        /* strRequest   */ Request2Str(ERequestUpdateSchema),
        /* iReqIdParent */ i_pMsgReq->getRequestId(),
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    // On cloning the message no deep copy is necessary as only the sender of the message is needed.
    // But we store the message object as the message will be informed if the sender will be destroyed.
    pReq->setInitiatorRequestMessage( dynamic_cast<CMsgReq*>(i_pMsgReq->clone()) );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_pMsgReq->getRequestId()) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeUpdateSchemaRequest( pReq, pReqInProgress );
    }

    return pReq;

} // onReceivedMsgReqUpdateSchema

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::onReceivedMsgReqSelectData( CMsgReqSelectData* i_pMsgReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "MsgReq: " + i_pMsgReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedMsgReqSelectData",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestSelectData,
        /* strRequest   */ Request2Str(ERequestSelectData),
        /* iReqIdParent */ i_pMsgReq->getRequestId(),
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    // On cloning the message no deep copy is necessary as only the sender of the message is needed.
    // But we store the message object as the message will be informed if the sender will be destroyed.
    pReq->setInitiatorRequestMessage( dynamic_cast<CMsgReq*>(i_pMsgReq->clone()) );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_pMsgReq->getRequestId()) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeSelectDataRequest( pReq, pReqInProgress );
    }

    return pReq;

} // onReceivedMsgReqSelectData

//------------------------------------------------------------------------------
CRequest* CDb::onReceivedMsgReqUpdateData( CMsgReqUpdateData* i_pMsgReq )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "MsgReq: " + i_pMsgReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedMsgReqUpdateData",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReq = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestUpdateData,
        /* strRequest   */ Request2Str(ERequestUpdateData),
        /* iReqIdParent */ i_pMsgReq->getRequestId(),
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    // On cloning the message no deep copy is necessary as only the sender of the message is needed.
    // But we store the message object as the message will be informed if the sender will be destroyed.
    pReq->setInitiatorRequestMessage( dynamic_cast<CMsgReq*>(i_pMsgReq->clone()) );

    CRequest* pReqInProgress = m_requestQueue.getRequestInProgress();

    bool bPostponeRequest = true;

    // The request can be executed immediately if the request queue is idle (no request
    // in progress, queue not locked and no requests postponed) or
    // if the request method has been started as a child of the request in progress
    // (e.g. as part of a request sequence) ..
    if( m_requestQueue.isIdle() || (pReqInProgress != nullptr && pReqInProgress->getId() == i_pMsgReq->getRequestId()) )
    {
        bPostponeRequest = false;
    }

    if( bPostponeRequest )
    {
        EResult resultPostpone = m_requestQueue.postponeRequest(pReq);

        if( resultPostpone != EResultSuccess )
        {
            pReq->setSeverity(EResultSeverityError);
            pReq->setResult(resultPostpone);
            pReq->setAddErrInfoDscr( "Request could not have been queued." );
        }
    }
    else // if( !bPostponeRequest )
    {
        // Please note that the request has either set as the request in progress,
        // added as a child request in progress or is set as SyncRequestToBeDeletedLater.
        executeUpdateDataRequest( pReq, pReqInProgress );
    }

    return pReq;

} // onReceivedMsgReqUpdateData

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::onReceivedMsgIndConnected( CMsgIndConnected* i_pMsgInd )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "MsgInd: " + i_pMsgInd->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedMsgIndConnected",
        /* strAddInfo   */ strAddTrcInfo );

    Ipc::SSocketDscr socketDscr = i_pMsgInd->getSocketDscr();

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onReceivedMsgIndConnected");

    QString strAddErrInfo;

    if( m_arhshSocketDscrsDbCltsConnected[socketDscr.m_socketType].contains(socketDscr.m_iSocketId) )
    {
        strAddErrInfo  = socketDscr.getConnectionString();
        strAddErrInfo += "; Socket id " + QString::number(socketDscr.m_iSocketId) + " is already used.";

        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjAlreadyInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }
    else
    {
        m_arhshSocketDscrsDbCltsConnected[socketDscr.m_socketType][socketDscr.m_iSocketId] = i_pMsgInd->getSocketDscr();
    }

} // onReceivedMsgIndConnected

//------------------------------------------------------------------------------
void CDb::onReceivedMsgIndDisconnected( CMsgIndDisconnected* i_pMsgInd )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "MsgInd: " + i_pMsgInd->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedMsgIndDisconnected",
        /* strAddInfo   */ strAddTrcInfo );

    Ipc::SSocketDscr socketDscr = i_pMsgInd->getSocketDscr();

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onReceivedMsgIndDisconnected");

    QString strAddErrInfo;

    if( !m_arhshSocketDscrsDbCltsConnected[socketDscr.m_socketType].contains(socketDscr.m_iSocketId) )
    {
        strAddErrInfo  = socketDscr.getConnectionString();
        strAddErrInfo += "; Socket id " + QString::number(socketDscr.m_iSocketId) + " not found.";

        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }
    else
    {
        m_arhshSocketDscrsDbCltsConnected[socketDscr.m_socketType].remove(socketDscr.m_iSocketId);
    }

} // onReceivedMsgIndDisconnected

/*==============================================================================
protected: // overridables (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::executeNextPostponedRequest()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeNextPostponedRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
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
            POST_OR_DELETE_MESSAGE(pMsgReq);
        }
    } // if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )

} // executeNextPostponedRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::executeStartupRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeStartupRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    // SelectSchema, SelectData, Set2Default are executed in the context of the database thread.
    // The sequence may not be a blocking call as a thread cannot wait on itself.
    m_pReqSeqrStartup->start(i_pReq);

    if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeStartupRequest

//------------------------------------------------------------------------------
void CDb::executeShutdownRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeShutdownRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    // Savechema, SaveData are executed in the context of the database thread.
    // The sequence may not be a blocking call as a thread cannot wait on itself.
    m_pReqSeqrShutdown->start(i_pReq);

    if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeShutdownRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::executeReadSchemaRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeReadSchemaRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestReadSchema;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( !m_bReqModeSingleShot )

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeReadSchemaRequest

//------------------------------------------------------------------------------
void CDb::executeSaveSchemaRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeSaveSchemaRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestSaveSchema;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( !m_bReqModeSingleShot )

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeSaveSchemaRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::executeReadDataRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeReadDataRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestReadData;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( !m_bReqModeSingleShot )

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeReadDataRequest

//------------------------------------------------------------------------------
void CDb::executeSaveDataRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeSaveDataRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestSaveData;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( !m_bReqModeSingleShot )

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeSaveDataRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::executeSet2DefaultRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeSet2DefaultRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestSet2Default;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( !m_bReqModeSingleShot )

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeSet2DefaultRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::executeConnectRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeConnectRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    CMsgReqConnect* pMsgReq = dynamic_cast<CMsgReqConnect*>(i_pReq->takeInitiatorRequestMessage());

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( pMsgReq == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr( "Connect request message not accessible." );
    }
    else // if( pMsgReq != nullptr )
    {
        QObject* pDbClt = pMsgReq->getSender();

        if( pDbClt == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Sender of connect request message not accessible." );
        }
        else if( m_arpDbCltsConnected.contains(pDbClt) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr( "Database client " + pDbClt->objectName() + " is already connected." );
        }
        else
        {
            m_arpDbCltsConnected.append(pDbClt);

            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setResult(EResultSuccess);
            }
        }

        delete pMsgReq;
        pMsgReq = nullptr;

    } // if( pMsgReq != nullptr )

    if( !errResultInfo.isErrorResult() && m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestConnect;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( !m_bReqModeSingleShot )

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeConnectRequest

//------------------------------------------------------------------------------
void CDb::executeDisconnectRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeDisconnectRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    CMsgReqDisconnect* pMsgReq = dynamic_cast<CMsgReqDisconnect*>(i_pReq->takeInitiatorRequestMessage());

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( pMsgReq == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr( "Disconnect request message not accessible." );
    }
    else // if( pMsgReq != nullptr )
    {
        QObject* pDbClt = pMsgReq->getSender();

        if( pDbClt == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Sender of disconnect request message not accessible." );
        }
        else if( !m_arpDbCltsConnected.contains(pDbClt) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjNotInList);
            errResultInfo.setAddErrInfoDscr( "Database Client " + pDbClt->objectName() + " is not connected" );
        }
        else
        {
            m_arpDbCltsConnected.removeOne(pDbClt);

            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setResult(EResultSuccess);
            }
        }

        delete pMsgReq;
        pMsgReq = nullptr;
    }

    if( !errResultInfo.isErrorResult() && m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestDisconnect;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( !m_bReqModeSingleShot )

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeDisconnectRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::executeRegisterRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeRegisterRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    CMsgReqRegister* pMsgReq = dynamic_cast<CMsgReqRegister*>(i_pReq->takeInitiatorRequestMessage());

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( pMsgReq == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr( "Register request message not accessible." );
    }
    else // if( pMsgReq != nullptr )
    {
        QObject*              pMsgSender = pMsgReq->getSender();
        QObject*              pDbClt = nullptr;
        CDbGateway*           pDbGtw = nullptr;
        Ipc::ESocketType socketType = Ipc::ESocketTypeUndefined;
        int                   iSocketId = pMsgReq->getSenderId();

        if( pMsgSender == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Sender of request message not accessible." );
        }
        else if( m_arpDbCltsConnected.contains(pMsgSender) )
        {
            pDbClt = pMsgSender;

            if( m_arpDbCltsRegistered.contains(pMsgSender) )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultObjAlreadyInList);
                errResultInfo.setAddErrInfoDscr( "Client " + pMsgReq->getSenderName() + " is already registered." );
            }
            else
            {
                m_arpDbCltsRegistered.append(pDbClt);
            }
        }
        else
        {
            pDbGtw = dynamic_cast<CDbGateway*>(pMsgSender);

            if( pDbGtw != nullptr )
            {
                int iSocketType = m_arpDbGtw.indexOf(pDbGtw);

                if( iSocketType < 0 || iSocketType > m_arpDbGtw.size() )
                {
                    pDbGtw = nullptr;
                }
                else
                {
                    socketType = static_cast<Ipc::ESocketType>(iSocketType);

                    if( !m_arhshSocketDscrsDbCltsConnected[socketType].contains(iSocketId) )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultObjNotInList);
                        errResultInfo.setAddErrInfoDscr( "Client " + pMsgReq->getSenderName() + " is not connected." );
                    }
                    else if( m_arariSocketIdsDbCltsRegistered[socketType].contains(iSocketId) )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultObjAlreadyInList);
                        errResultInfo.setAddErrInfoDscr( "Client " + pMsgReq->getSenderName() + " is already registered." );
                    }
                    else
                    {
                        m_arariSocketIdsDbCltsRegistered[socketType].append(iSocketId);
                    }
                }
            }
        }

        if( pDbClt == nullptr && pDbGtw == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Invalid sender of request message." );
        }

        delete pMsgReq;
        pMsgReq = nullptr;

    } // if( pMsgReq != nullptr )

    if( !errResultInfo.isErrorResult() && m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestRegister;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( !m_bReqModeSingleShot )

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeRegisterRequest

//------------------------------------------------------------------------------
void CDb::executeUnregisterRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeUnregisterRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    CMsgReqUnregister* pMsgReq = dynamic_cast<CMsgReqUnregister*>(i_pReq->takeInitiatorRequestMessage());

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( pMsgReq == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr( "Unregister request message not accessible." );
    }
    else // if( pMsgReq != nullptr )
    {
        QObject*         pMsgSender = pMsgReq->getSender();
        QObject*         pDbClt = nullptr;
        CDbGateway*      pDbGtw = nullptr;
        Ipc::ESocketType socketType = Ipc::ESocketTypeUndefined;
        int              iSocketId = pMsgReq->getSenderId();

        if( pMsgSender == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Sender of request message not accessible." );
        }
        else if( m_arpDbCltsConnected.contains(pMsgSender) )
        {
            pDbClt = pMsgSender;

            if( !m_arpDbCltsRegistered.contains(pMsgSender) )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultObjNotInList);
                errResultInfo.setAddErrInfoDscr( "Client " + pMsgReq->getSenderName() + " is not registered." );
            }
            else
            {
                m_arpDbCltsRegistered.removeOne(pDbClt);
            }
        }
        else
        {
            pDbGtw = dynamic_cast<CDbGateway*>(pMsgSender);

            if( pDbGtw != nullptr )
            {
                int iSocketType = m_arpDbGtw.indexOf(pDbGtw);

                if( iSocketType < 0 || iSocketType > m_arpDbGtw.size() )
                {
                    pDbGtw = nullptr;
                }
                else
                {
                    socketType = static_cast<Ipc::ESocketType>(iSocketType);

                    if( !m_arhshSocketDscrsDbCltsConnected[socketType].contains(iSocketId) )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultObjNotInList);
                        errResultInfo.setAddErrInfoDscr( "Client " + pMsgReq->getSenderName() + " is not connected." );
                    }
                    else if( !m_arariSocketIdsDbCltsRegistered[socketType].contains(iSocketId) )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultObjNotInList);
                        errResultInfo.setAddErrInfoDscr( "Client " + pMsgReq->getSenderName() + " is not registered." );
                    }
                    else
                    {
                        m_arariSocketIdsDbCltsRegistered[socketType].removeOne(iSocketId);
                    }
                }
            }
        }

        if( pDbClt == nullptr && pDbGtw == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Invalid sender of request message." );
        }

    } // if( pMsgReq != nullptr )

    if( !errResultInfo.isErrorResult() && m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestUnregister;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( !m_bReqModeSingleShot )

    delete pMsgReq;
    pMsgReq = nullptr;

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeUnregisterRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::executeSelectSchemaRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeSelectSchemaRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    CMsgReq* pMsgReq = dynamic_cast<CMsgReq*>(i_pReq->takeInitiatorRequestMessage());

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( pMsgReq == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr( "Request message not accessible." );
    }
    else // if( pMsgReq != nullptr )
    {
        QObject*         pMsgSender = pMsgReq->getSender();
        QObject*         pDbClt = nullptr;
        CDbGateway*      pDbGtw = nullptr;
        Ipc::ESocketType socketType = Ipc::ESocketTypeUndefined;
        int              iSocketId = pMsgReq->getSenderId();

        if( pMsgSender == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Sender of request message not accessible." );
        }
        else if( m_arpDbCltsConnected.contains(pMsgSender) )
        {
            pDbClt = pMsgSender;
        }
        else
        {
            pDbGtw = dynamic_cast<CDbGateway*>(pMsgSender);

            if( pDbGtw != nullptr )
            {
                int iSocketType = m_arpDbGtw.indexOf(pDbGtw);

                if( iSocketType < 0 || iSocketType > m_arpDbGtw.size() )
                {
                    pDbGtw = nullptr;
                }
                else
                {
                    socketType = static_cast<Ipc::ESocketType>(iSocketType);

                    if( !m_arhshSocketDscrsDbCltsConnected[socketType].contains(iSocketId) )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultObjNotInList);
                        errResultInfo.setAddErrInfoDscr( "Client " + pMsgReq->getSenderName() + " is not connected." );
                    }
                }
            }
        }

        if( pDbClt == nullptr && pDbGtw == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Invalid sender of request message." );
        }

    } // if( pMsgReq != nullptr )

    if( !errResultInfo.isErrorResult() && m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestSelectSchema;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( !m_bReqModeSingleShot )

    delete pMsgReq;
    pMsgReq = nullptr;

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeSelectSchemaRequest

//------------------------------------------------------------------------------
void CDb::executeUpdateSchemaRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeUpdateSchemaRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    CMsgReq* pMsgReq = dynamic_cast<CMsgReq*>(i_pReq->takeInitiatorRequestMessage());

    QObject*         pMsgSender = nullptr;
    QObject*         pDbCltSender = nullptr;
    CDbGateway*      pDbGtwSender = nullptr;
    Ipc::ESocketType socketTypeSender = Ipc::ESocketTypeUndefined;
    int              iSocketIdSender = -1;

    QObject*    pDbClt;
    CDbGateway* pDbGtw;
    int         idxDbClt;
    int         iSocketType;
    int         iSocketId;

    if( pMsgReq == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr( "Request message not accessible." );
    }
    else // if( pMsgReq != nullptr )
    {
        pMsgSender = pMsgReq->getSender();
        iSocketIdSender = pMsgReq->getSenderId();

        if( pMsgSender == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Sender of request message not accessible." );
        }
        else if( m_arpDbCltsConnected.contains(pMsgSender) )
        {
            pDbCltSender = pMsgSender;
        }
        else
        {
            pDbGtwSender = dynamic_cast<CDbGateway*>(pMsgSender);

            if( pDbGtwSender != nullptr )
            {
                iSocketType = m_arpDbGtw.indexOf(pDbGtwSender);

                if( iSocketType < 0 || iSocketType > m_arpDbGtw.size() )
                {
                    pDbGtwSender = nullptr;
                }
                else
                {
                    socketTypeSender = static_cast<Ipc::ESocketType>(iSocketType);

                    if( !m_arhshSocketDscrsDbCltsConnected[socketTypeSender].contains(iSocketIdSender) )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultObjNotInList);
                        errResultInfo.setAddErrInfoDscr( "Client " + pMsgReq->getSenderName() + " is not connected." );
                    }
                }
            }
        }

        if( pDbCltSender == nullptr && pDbGtwSender == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Invalid sender of request message." );
        }
    } // if( pMsgReq != nullptr )

    if( !errResultInfo.isErrorResult() )
    {
        delete m_pReqSeqrExecCmd;

        m_pReqSeqrExecCmd = new CRequestSequencer(
            /* strObjName             */ "ZS::Apps::Test::AsyncRequests::CDb::" + objectName() + ".execCmd",
            /* pObjParent             */ this,
            /* bKeepReqDscrInExecTree */ false );

        qint64 iReqIdDbUpdateSchema = m_pReqSeqrExecCmd->addRequest(
            /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
            /* strCreatorClassName      */ "CDb",
            /* pObjCreator              */ this,
            /* iRequest                 */ ERequestDbUpdateSchema,
            /* strRequest               */ Request2Str(ERequestDbUpdateSchema),
            /* pvObjFctExecute          */ this,
            /* fctExecute               */ UpdateDbSchema,
            /* pvExec                   */ nullptr,
            /* iReqIdParent             */ -1,
            /* iReqIdPredecessor        */ -1 );

        // The UpdateSchema request got to be forwarded to each registered client
        // exluding the client initiating this request.

        for( idxDbClt = 0; idxDbClt < m_arpDbCltsRegistered.size(); idxDbClt++ )
        {
            pDbClt = m_arpDbCltsRegistered[idxDbClt];

            if( pDbClt != nullptr && pDbClt != pDbCltSender )
            {
                /*qint64 iReqIdDbCltUpdateSchema =*/ m_pReqSeqrExecCmd->addRequest(
                    /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                    /* strCreatorClassName      */ "CDb",
                    /* pObjCreator              */ this,
                    /* iRequest                 */ ERequestDbCltUpdateSchema,
                    /* strRequest               */ Request2Str(ERequestDbCltUpdateSchema),
                    /* pvObjFctExecute          */ this,
                    /* fctExecute               */ UpdateDbCltSchema,
                    /* pvExec                   */ new SDbCltSocketDscr( Ipc::ESocketTypeUndefined, pDbClt, idxDbClt ),
                    /* iReqIdParent             */ -1,
                    /* iReqIdPredecessor        */ iReqIdDbUpdateSchema );
            }
        }

        for( iSocketType = 0; iSocketType < m_arariSocketIdsDbCltsRegistered.size(); iSocketType++ )
        {
            pDbGtw = m_arpDbGtw[iSocketType];

            for( idxDbClt = 0; idxDbClt < m_arariSocketIdsDbCltsRegistered[iSocketType].size(); idxDbClt++ )
            {
                iSocketId = m_arariSocketIdsDbCltsRegistered[iSocketType][idxDbClt];

                if( pDbGtw != pDbGtwSender || iSocketId != iSocketIdSender )
                {
                    /*qint64 iReqIdDbCltUpdateSchema =*/ m_pReqSeqrExecCmd->addRequest(
                        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                        /* strCreatorClassName      */ "CDb",
                        /* pObjCreator              */ this,
                        /* iRequest                 */ ERequestDbCltUpdateSchema,
                        /* strRequest               */ Request2Str(ERequestDbCltUpdateSchema),
                        /* pvObjFctExecute          */ this,
                        /* fctExecute               */ UpdateDbCltSchema,
                        /* pvExec                   */ new SDbCltSocketDscr( static_cast<Ipc::ESocketType>(iSocketType), pDbGtw, iSocketId ),
                        /* iReqIdParent             */ -1,
                        /* iReqIdPredecessor        */ iReqIdDbUpdateSchema );
                }
            }
        }

        m_pReqSeqrExecCmd->start(i_pReq);

    } // if( !errResultInfo.isErrorResult() )

    else // if( errResultInfo.isErrorResult() )
    {
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( errResultInfo.isErrorResult() )

    delete pMsgReq;
    pMsgReq = nullptr;

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeUpdateSchemaRequest

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::executeSelectDataRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeSelectDataRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    CMsgReq* pMsgReq = dynamic_cast<CMsgReq*>(i_pReq->takeInitiatorRequestMessage());

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    if( pMsgReq == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr( "Request message not accessible." );
    }
    else // if( pMsgReq != nullptr )
    {
        QObject*         pMsgSender = pMsgReq->getSender();
        QObject*         pDbClt = nullptr;
        CDbGateway*      pDbGtw = nullptr;
        Ipc::ESocketType socketType = Ipc::ESocketTypeUndefined;
        int              iSocketId = pMsgReq->getSenderId();

        if( pMsgSender == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Sender of request message not accessible." );
        }
        else if( m_arpDbCltsConnected.contains(pMsgSender) )
        {
            pDbClt = pMsgSender;
        }
        else
        {
            pDbGtw = dynamic_cast<CDbGateway*>(pMsgSender);

            if( pDbGtw != nullptr )
            {
                int iSocketType = m_arpDbGtw.indexOf(pDbGtw);

                if( iSocketType < 0 || iSocketType > m_arpDbGtw.size() )
                {
                    pDbGtw = nullptr;
                }
                else
                {
                    socketType = static_cast<Ipc::ESocketType>(iSocketType);

                    if( !m_arhshSocketDscrsDbCltsConnected[socketType].contains(iSocketId) )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultObjNotInList);
                        errResultInfo.setAddErrInfoDscr( "Client " + pMsgReq->getSenderName() + " is not connected." );
                    }
                }
            }
        }

        if( pDbClt == nullptr && pDbGtw == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Invalid sender of request message." );
        }

    } // if( pMsgReq != nullptr )

    if( !errResultInfo.isErrorResult() && m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestSelectData;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( !m_bReqModeSingleShot )

    delete pMsgReq;
    pMsgReq = nullptr;

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeSelectDataRequest

//------------------------------------------------------------------------------
void CDb::executeUpdateDataRequest( CRequest* i_pReq, CRequest* i_pReqMainInProgress )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo = "Req: " + i_pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel);

        if( i_pReqMainInProgress == nullptr )
        {
            strAddTrcInfo += ", ReqMainInProgress: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ReqMainInProgress: " + i_pReqMainInProgress->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeUpdateDataRequest",
        /* strAddInfo   */ strAddTrcInfo );

    if( !QObject::connect(
        /* pObjSender   */ i_pReq,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // If started as child request of request in progress (as part of a request sequence) ..
    if( i_pReqMainInProgress != nullptr )
    {
        m_requestQueue.addRequestInProgress(i_pReq);
    }
    // If not started as child request of request in progress (as main request) ..
    else
    {
        m_requestQueue.setRequestInProgress(i_pReq);
    }

    i_pReq->lock();

    m_requestQueue.lock();

    SErrResultInfo errResultInfo = i_pReq->getErrResultInfo();

    CMsgReq* pMsgReq = dynamic_cast<CMsgReq*>(i_pReq->takeInitiatorRequestMessage());

    QObject*         pMsgSender = nullptr;
    QObject*         pDbCltSender = nullptr;
    CDbGateway*      pDbGtwSender = nullptr;
    Ipc::ESocketType socketTypeSender = Ipc::ESocketTypeUndefined;
    int              iSocketIdSender = -1;

    QObject*    pDbClt;
    CDbGateway* pDbGtw;
    int         idxDbClt;
    int         iSocketType;
    int         iSocketId;

    if( pMsgReq == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInternalProgramError);
        errResultInfo.setAddErrInfoDscr( "Request message not accessible." );
    }
    else // if( pMsgReq != nullptr )
    {
        pMsgSender = pMsgReq->getSender();
        iSocketIdSender = pMsgReq->getSenderId();

        if( pMsgSender == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Sender of request message not accessible." );
        }
        else if( m_arpDbCltsConnected.contains(pMsgSender) )
        {
            pDbCltSender = pMsgSender;
        }
        else
        {
            pDbGtwSender = dynamic_cast<CDbGateway*>(pMsgSender);

            if( pDbGtwSender != nullptr )
            {
                iSocketType = m_arpDbGtw.indexOf(pDbGtwSender);

                if( iSocketType < 0 || iSocketType > m_arpDbGtw.size() )
                {
                    pDbGtwSender = nullptr;
                }
                else
                {
                    socketTypeSender = static_cast<Ipc::ESocketType>(iSocketType);

                    if( !m_arhshSocketDscrsDbCltsConnected[socketTypeSender].contains(iSocketIdSender) )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultObjNotInList);
                        errResultInfo.setAddErrInfoDscr( "Client " + pMsgReq->getSenderName() + " is not connected." );
                    }
                }
            }
        }

        if( pDbCltSender == nullptr && pDbGtwSender == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInternalProgramError);
            errResultInfo.setAddErrInfoDscr( "Invalid sender of request message." );
        }
    } // if( pMsgReq != nullptr )

    if( !errResultInfo.isErrorResult() )
    {
        delete m_pReqSeqrExecCmd;

        m_pReqSeqrExecCmd = new CRequestSequencer(
            /* strObjName             */ "ZS::Apps::Test::AsyncRequests::CDb::" + objectName() + ".execCmd",
            /* pObjParent             */ this,
            /* bKeepReqDscrInExecTree */ false );

        qint64 iReqIdDbUpdateData = m_pReqSeqrExecCmd->addRequest(
            /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
            /* strCreatorClassName      */ "CDb",
            /* pObjCreator              */ this,
            /* iRequest                 */ ERequestDbUpdateData,
            /* strRequest               */ Request2Str(ERequestDbUpdateData),
            /* pvObjFctExecute          */ this,
            /* fctExecute               */ UpdateDbData,
            /* pvExec                   */ nullptr,
            /* iReqIdParent             */ -1,
            /* iReqIdPredecessor        */ -1 );

        // The UpdateData request got to be forwarded to each registered client
        // exluding the client initiating this request.

        for( idxDbClt = 0; idxDbClt < m_arpDbCltsRegistered.size(); idxDbClt++ )
        {
            pDbClt = m_arpDbCltsRegistered[idxDbClt];

            if( pDbClt != pDbCltSender )
            {
                /*qint64 iReqIdDbCltUpdateData =*/ m_pReqSeqrExecCmd->addRequest(
                    /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                    /* strCreatorClassName      */ "CDb",
                    /* pObjCreator              */ this,
                    /* iRequest                 */ ERequestDbCltUpdateData,
                    /* strRequest               */ Request2Str(ERequestDbCltUpdateData),
                    /* pvObjFctExecute          */ this,
                    /* fctExecute               */ UpdateDbCltData,
                    /* pvExec                   */ new SDbCltSocketDscr( Ipc::ESocketTypeUndefined, pDbClt, idxDbClt ),
                    /* iReqIdParent             */ -1,
                    /* iReqIdPredecessor        */ iReqIdDbUpdateData );
            }
        }

        for( iSocketType = 0; iSocketType < m_arariSocketIdsDbCltsRegistered.size(); iSocketType++ )
        {
            pDbGtw = m_arpDbGtw[iSocketType];

            for( idxDbClt = 0; idxDbClt < m_arariSocketIdsDbCltsRegistered[iSocketType].size(); idxDbClt++ )
            {
                iSocketId = m_arariSocketIdsDbCltsRegistered[iSocketType][idxDbClt];

                if( pDbGtw != pDbGtwSender || iSocketId != iSocketIdSender )
                {
                    /*qint64 iReqIdDbCltUpdateData =*/ m_pReqSeqrExecCmd->addRequest(
                        /* strCreatorNameSpace */ "ZS::Apps::Test::AsyncRequests",
                        /* strCreatorClassName      */ "CDb",
                        /* pObjCreator              */ this,
                        /* iRequest                 */ ERequestDbCltUpdateSchema,
                        /* strRequest               */ Request2Str(ERequestDbCltUpdateSchema),
                        /* pvObjFctExecute          */ this,
                        /* fctExecute               */ UpdateDbCltSchema,
                        /* pvExec                   */ new SDbCltSocketDscr( static_cast<Ipc::ESocketType>(iSocketType), pDbGtw, iSocketId ),
                        /* iReqIdParent             */ -1,
                        /* iReqIdPredecessor        */ iReqIdDbUpdateData );
                }
            }
        }

        m_pReqSeqrExecCmd->start(i_pReq);

    } // if( !errResultInfo.isErrorResult() )

    else // if( errResultInfo.isErrorResult() )
    {
        i_pReq->setErrResultInfo(errResultInfo);
        i_pReq->setProgressInPerCent(100);
        i_pReq->update();

    } // if( errResultInfo.isErrorResult() )

    delete pMsgReq;
    pMsgReq = nullptr;

    if( isAsynchronousRequest(i_pReq) )
    {
        if( i_pReq->getTimeoutInMs() > 0 && !m_pTmrReqTimeout->isActive() )
        {
            m_reqDscrTimeout = i_pReq->getDescriptor();
            m_pTmrReqTimeout->start( i_pReq->getTimeoutInMs() );
        }
    }
    else // if( !isAsynchronousRequest(i_pReq) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ i_pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(i_pReq);
    }

    m_requestQueue.unlock();

    i_pReq->unlock();

    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
    {
        executeNextPostponedRequest();
    }

} // executeUpdateDataRequest

/*==============================================================================
protected slots: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDb::onExecuteReqSingleShotTimeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqSimInProgress: " + Request2Str(m_reqSimInProgress);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onExecuteReqSingleShotTimeout",
        /* strAddInfo   */ strAddTrcInfo );

    bool bExecute = true;

    if( m_reqSimInProgress == ERequestNone )
    {
        bExecute = false;
    }
    else if( m_bReqModeSingleShot )
    {
        bExecute = m_bReqSingleShot;

        if( m_bReqSingleShot )
        {
            m_bReqSingleShot = false;
        }
    }

    if( bExecute )
    {
        CRequest* pReq = m_requestQueue.getRequestInProgressByType(m_reqSimInProgress);

        if( pReq != nullptr )
        {
            SErrResultInfo errResultInfo( pReq->getErrResultInfo() );

            qint64 iReqId = pReq->getId();

            m_iReqSimDurationStepsCurr++;

            int iProgress_perCent = 100;

            if( m_iReqSimDurationStepsMax > 0 )
            {
                iProgress_perCent = (100*m_iReqSimDurationStepsCurr)/m_iReqSimDurationStepsMax;

                if( iProgress_perCent >= 100 )
                {
                    iProgress_perCent = 100;
                }
            }

            if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) )
            {
                strAddTrcInfo = "Progress: " + QString::number(iProgress_perCent) + "%";
                mthTracer.trace(strAddTrcInfo);
            }

            m_requestQueue.lock();

            //// If the request has been finished ...
            //if( iProgress_perCent >= 100 )
            //{
            //    // .. it has to be removed from the request queue before updating the request.
            //    // If the current request would be a child of another request (e.g. as part
            //    // of a request sequencer) a "on<Request>Finished" slot may be called while the child
            //    // request is still in the queues request hash list. And parent requests can
            //    // only be removed if all children have been removed.
            //    m_requestQueue.removeRequestInProgress( pReq->getId() );

            //    // Set Result = Success if the request has been finished
            //    // (keep Result = Undefined as long as the request is in progress).
            //    pReq->setErrResultInfo(errResultInfo);
            //}

            if( iProgress_perCent >= 100 )
            {
                errResultInfo.setResult(EResultSuccess);
            }
            else // if( iProgress_perCent < 100 )
            {
                errResultInfo.setResult(EResultSuccessIntermediateResults);
            }

            pReq->setErrResultInfo(errResultInfo);
            pReq->setProgressInPerCent(iProgress_perCent);

            // On updating the request "onRequestChanged" is called which may remove the request from
            // the request queue or may even delete the request if the request has been finished or aborted.
            // To avoid that the request is being deleted within the "onRequestChanged" slot the request will be locked.
            pReq->lock();
            pReq->update();
            pReq->unlock();

            if( iProgress_perCent >= 100 )
            {
                // The one who calls update for the finished request must delete the request.
                deleteRequest(pReq); // The "onRequestChanged" slot will be called again. The request has already been removed from the queue.
                pReq = nullptr;
            }

            // If the request is still in progress ..
            if( m_requestQueue.isRequestInProgress(iReqId) )
            {
                if( !m_bReqModeSingleShot )
                {
                    double fTimeElapsed_us = ZS::System::Time::getProcTimeInMicroSec();
                    double fTimeElapsed_ms = ZS::System::Time::getProcTimeInMilliSec()+0.001;
                    double fSRand = 1000.0*fTimeElapsed_ms - fTimeElapsed_us;
                    int    iSRand = 100+static_cast<int>(1000.0*fSRand);

                    qsrand(iSRand);

                    int iTimeout_ms = m_iReqSimDurationMin_ms + (qrand() % m_iReqSimDurationMax_ms);

                    QTimer::singleShot( iTimeout_ms, this, SLOT(onExecuteReqSingleShotTimeout()) );
                }
            } // if( iProgress_perCent < 100 )

            m_requestQueue.unlock();

        } // if( pReq != nullptr )
    } // if( bExecute )

} // onExecuteReqSingleShotTimeout

/*==============================================================================
protected: // internal request sequencer methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::startGatewayThread( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, ZS::Ipc::ESocketType i_socketType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", SocketType: " + Ipc::socketType2Str(i_socketType);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startGatewayThread",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("startGatewayThread");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqStartThread = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestStartThread,
        /* strRequest   */ Request2Str(ERequestStartThread),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    if( !QObject::connect(
        /* pObjSender   */ pReqStartThread,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_requestQueue.addRequestInProgress(pReqStartThread);

    m_arpDbGtwThread[i_socketType] = new CDbGatewayThread(
        /* strObjName */ objectName() + Ipc::socketType2Str(i_socketType) + "Gateway",
        /* socketType */ i_socketType,
        /* pDb        */ this );

    QMutex         mtx;
    QWaitCondition waitCond;
    mtx.lock();
    m_arpDbGtwThread[i_socketType]->start(&waitCond);
    waitCond.wait(&mtx);
    mtx.unlock();

    m_arpDbGtw[i_socketType] = m_arpDbGtwThread[i_socketType]->getGateway();

    // StartThread is an internal sequencer request. No one else may have
    // signal/slot connections to this request. So there is no need to lock
    // the queue on updating this request (but its necessary to update the
    // request to indicate the progress in the request execution tree).
    m_requestQueue.removeRequestInProgress( pReqStartThread->getId() );
    //m_requestQueue.lock();
    pReqStartThread->setErrResultInfo(errResultInfo);
    pReqStartThread->setProgressInPerCent(100);
    pReqStartThread->update();
    //m_requestQueue.unlock();

    // Don't delete the request right now but return the request to the caller
    // as the caller may want to evaluate some details of the executed request.
    // E.g. if the request has been started by a request sequencer the sequencer
    // wants to update the request id.
    // The request will be set at the request queue to be deleted later
    // (on destroying the queue or if the next sync request will be set).

    // The request has been finished and will be deleted sometimes later.
    // This doesn't matter me ...
    QObject::disconnect(
        /* pObjSender   */ pReqStartThread,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

    m_requestQueue.setSyncRequestToBeDeletedLater(pReqStartThread);

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReqStartThread->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqStartThread;

} // startGatewayThread

//------------------------------------------------------------------------------
CRequest* CDb::stopGatewayThread( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, ZS::Ipc::ESocketType i_socketType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", SocketType: " + Ipc::socketType2Str(i_socketType);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "stopGatewayThread",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("stopGatewayThread");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqStopThread = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestStopThread,
        /* strRequest   */ Request2Str(ERequestStopThread),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    if( m_arpDbGtwThread[i_socketType] != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ pReqStopThread,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }

        m_requestQueue.addRequestInProgress(pReqStopThread);

        m_arpDbGtwThread[i_socketType]->quit();

        unsigned long iThreadWaitTimeout_ms = static_cast<unsigned long>(pReqStopThread->getTimeoutInMs());

        if( iThreadWaitTimeout_ms <= 0 )
        {
            iThreadWaitTimeout_ms = ULONG_MAX;
        }

        if( !m_arpDbGtwThread[i_socketType]->wait(iThreadWaitTimeout_ms) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for StopThread timed out." );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
            m_arpDbGtwThread[i_socketType]->deleteLater();
            m_arpDbGtwThread[i_socketType] = nullptr;
        }
        else
        {
            try
            {
                delete m_arpDbGtwThread[i_socketType];
            }
            catch(...)
            {
            }
            m_arpDbGtwThread[i_socketType] = nullptr;
        }

        m_requestQueue.removeRequestInProgress( pReqStopThread->getId() );

    } // if( m_pDbCltGtwThread != nullptr )

    m_arpDbGtw[i_socketType] = nullptr;

    // StopThread is an internal sequencer request. No one else may have
    // signal/slot connections to this request. So there is no need to lock
    // the queue on updating this request (but its necessary to update the
    // request to indicate the progress in the request execution tree).
    //m_requestQueue.lock();
    pReqStopThread->setErrResultInfo(errResultInfo);
    pReqStopThread->setProgressInPerCent(100);
    pReqStopThread->update();
    //m_requestQueue.unlock();

    // Don't delete the request right now but return the request to the caller
    // as the caller may want to evaluate some details of the executed request.
    // E.g. if the request has been started by a request sequencer the sequencer
    // wants to update the request id.
    // The request will be set at the request queue to be deleted later
    // (on destroying the queue or if the next sync request will be set).

    // The request has been finished and will be deleted sometimes later.
    // This doesn't matter me ...
    QObject::disconnect(
        /* pObjSender   */ pReqStopThread,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

    m_requestQueue.setSyncRequestToBeDeletedLater(pReqStopThread);

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReqStopThread->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqStopThread;

} // stopGatewayThread

//------------------------------------------------------------------------------
CRequest* CDb::startupGateway( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, ZS::Ipc::ESocketType i_socketType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", SocketType: " + Ipc::socketType2Str(i_socketType);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startupGateway",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("startupGateway");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqDbStartup = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestDbStartup,
        /* strRequest   */ Request2Str(ERequestDbStartup),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    if( !QObject::connect(
        /* pObjSender   */ pReqDbStartup,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_requestQueue.addRequestInProgress(pReqDbStartup);

    QObject* pMsgReceiver = m_arpDbGtw[i_socketType];

    CMsgReqStartup* pMsgReq = new CMsgReqStartup(
        /* pObjSender       */ this,
        /* pObjReceiver     */ pMsgReceiver,
        /* bMustBeConfirmed */ true,
        /* iReqId           */ pReqDbStartup->getId(),
        /* iMsgId           */ -1 );
    pMsgReq->setHostSettings( m_arHostSettingsGtw[i_socketType] );

    POST_OR_DELETE_MESSAGE(pMsgReq);

    if( pReqDbStartup->isBlockingRequest() )
    {
        if( !pReqDbStartup->wait() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for startup confirmation timed out." );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }

        // DbStartup is an internal sequencer request. No one else may have
        // signal/slot connections to this request. So there is no need to lock
        // the queue on updating this request (but its necessary to update the
        // request to indicate the progress in the request execution tree).
        m_requestQueue.removeRequestInProgress( pReqDbStartup->getId() );
        //m_requestQueue.lock();
        pReqDbStartup->setErrResultInfo(errResultInfo);
        pReqDbStartup->setProgressInPerCent(100);
        pReqDbStartup->update();
        //m_requestQueue.unlock();
        deleteResultConfirmationMessage(pReqDbStartup, this);

    } // if( pReqDbStartup->isBlockingRequest() )

    else // if( !pReqDbStartup->isBlockingRequest() )
    {
        if( i_iTimeout_ms > 0 )
        {
            m_reqDscrTimeout = pReqDbStartup->getDescriptor();
            m_pTmrReqTimeout->start(i_iTimeout_ms);
        }
    }

    if( !isAsynchronousRequest(pReqDbStartup) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ pReqDbStartup,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(pReqDbStartup);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReqDbStartup->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqDbStartup;

} // startupGateway

//------------------------------------------------------------------------------
CRequest* CDb::shutdownGateway( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, ZS::Ipc::ESocketType i_socketType )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", SocketType: " + Ipc::socketType2Str(i_socketType);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "shutdownGateway",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("shutdownGateway");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqDbShutdown = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestDbShutdown,
        /* strRequest   */ Request2Str(ERequestDbShutdown),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    if( !QObject::connect(
        /* pObjSender   */ pReqDbShutdown,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_requestQueue.addRequestInProgress(pReqDbShutdown);

    QObject* pMsgReceiver = m_arpDbGtw[i_socketType];

    CMsgReqShutdown* pMsgReq = new CMsgReqShutdown(
        /* pObjSender       */ this,
        /* pObjReceiver     */ pMsgReceiver,
        /* bMustBeConfirmed */ true,
        /* iReqId           */ pReqDbShutdown->getId(),
        /* iMsgId           */ -1 );

    POST_OR_DELETE_MESSAGE(pMsgReq);

    if( pReqDbShutdown->isBlockingRequest() )
    {
        if( !pReqDbShutdown->wait() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for shutdown confirmation timed out." );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }

        // DbStartup is an internal sequencer request. No one else may have
        // signal/slot connections to this request. So there is no need to lock
        // the queue on updating this request (but its necessary to update the
        // request to indicate the progress in the request execution tree).
        m_requestQueue.removeRequestInProgress( pReqDbShutdown->getId() );
        //m_requestQueue.lock();
        pReqDbShutdown->setErrResultInfo(errResultInfo);
        pReqDbShutdown->setProgressInPerCent(100);
        pReqDbShutdown->update();
        //m_requestQueue.unlock();
        deleteResultConfirmationMessage(pReqDbShutdown, this);

    } // if( pReqDbShutdown->isBlockingRequest() )

    else // if( !pReqDbShutdown->isBlockingRequest() )
    {
        if( i_iTimeout_ms > 0 )
        {
            m_reqDscrTimeout = pReqDbShutdown->getDescriptor();
            m_pTmrReqTimeout->start(i_iTimeout_ms);
        }
    }

    if( !isAsynchronousRequest(pReqDbShutdown) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ pReqDbShutdown,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(pReqDbShutdown);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReqDbShutdown->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqDbShutdown;

} // shutdownGateway

/*==============================================================================
protected: // internal request sequencer methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::updateDbSchema( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateDbSchema",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqDbUpdateSchema = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestDbUpdateSchema,
        /* strRequest   */ Request2Str(ERequestDbUpdateSchema),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    if( !QObject::connect(
        /* pObjSender   */ pReqDbUpdateSchema,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_requestQueue.addRequestInProgress(pReqDbUpdateSchema);

    SErrResultInfo errResultInfo = pReqDbUpdateSchema->getErrResultInfo();

    if( m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestDbUpdateSchema;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        pReqDbUpdateSchema->setErrResultInfo(errResultInfo);
        pReqDbUpdateSchema->setProgressInPerCent(100);
        pReqDbUpdateSchema->update();

    } // if( !m_bReqModeSingleShot )

    if( !isAsynchronousRequest(pReqDbUpdateSchema) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ pReqDbUpdateSchema,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(pReqDbUpdateSchema);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReqDbUpdateSchema->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqDbUpdateSchema;

} // updateDbSchema

//------------------------------------------------------------------------------
CRequest* CDb::updateDbCltSchema( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, SDbCltSocketDscr* i_pDbCltSocketDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ReqIdParent: " + QString::number(i_iReqIdParent);

        if( i_pDbCltSocketDscr == nullptr )
        {
            strAddTrcInfo += ", DbCltSocketDscr: nullptr";
        }
        else
        {
            strAddTrcInfo += ", DbCltSocketDscr: " + i_pDbCltSocketDscr->getAddTrcInfoStr();
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateDbCltSchema",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("updateDbCltSchema");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqDbCltUpdateSchema = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestDbCltUpdateSchema,
        /* strRequest   */ Request2Str(ERequestDbCltUpdateSchema),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    if( !QObject::connect(
        /* pObjSender   */ pReqDbCltUpdateSchema,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_requestQueue.addRequestInProgress(pReqDbCltUpdateSchema);

    CMsgReqUpdateSchema* pMsgReq = new CMsgReqUpdateSchema(
        /* pObjSender       */ this,
        /* pObjReceiver     */ i_pDbCltSocketDscr->m_pDbCltOrGtw,
        /* bMustBeConfirmed */ true,
        /* iReqId           */ pReqDbCltUpdateSchema->getId(),
        /* iMsgId           */ -1 );
    pMsgReq->setReceiverId(i_pDbCltSocketDscr->m_idxDbCltOrSocketId);
    POST_OR_DELETE_MESSAGE(pMsgReq);

    if( pReqDbCltUpdateSchema->isBlockingRequest() )
    {
        if( !pReqDbCltUpdateSchema->wait() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for DbCltUpdateSchema timed out." );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }

        // DbCltUpdateSchema is an internal sequencer request. No one else may have
        // signal/slot connections to this request. So there is no need to lock
        // the queue on updating this request (but its necessary to update the
        // request to indicate the progress in the request execution tree).
        m_requestQueue.removeRequestInProgress( pReqDbCltUpdateSchema->getId() );
        //m_requestQueue.lock();
        pReqDbCltUpdateSchema->setErrResultInfo(errResultInfo);
        pReqDbCltUpdateSchema->setProgressInPerCent(100);
        pReqDbCltUpdateSchema->update();
        //m_requestQueue.unlock();
        deleteResultConfirmationMessage(pReqDbCltUpdateSchema, this);

        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ pReqDbCltUpdateSchema,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(pReqDbCltUpdateSchema);

    } // if( pReqDbCltUpdateSchema->isBlockingRequest() )

    else // if( !pReqDbCltUpdateSchema->isBlockingRequest() )
    {
        if( i_iTimeout_ms > 0 )
        {
            m_reqDscrTimeout = pReqDbCltUpdateSchema->getDescriptor();
            m_pTmrReqTimeout->start(i_iTimeout_ms);
        }
    }

    if( !isAsynchronousRequest(pReqDbCltUpdateSchema) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ pReqDbCltUpdateSchema,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(pReqDbCltUpdateSchema);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReqDbCltUpdateSchema->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    delete i_pDbCltSocketDscr;
    i_pDbCltSocketDscr = nullptr;

    return pReqDbCltUpdateSchema;

} // updateDbCltSchema

/*==============================================================================
protected: // internal request sequencer methods
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CDb::updateDbData( qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "ReqIdParent: " + QString::number(i_iReqIdParent);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateDbData",
        /* strAddInfo   */ strAddTrcInfo );

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqDbUpdateData = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestDbUpdateData,
        /* strRequest   */ Request2Str(ERequestDbUpdateData),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ 0,
        /* bIsBlocking  */ false );

    if( !QObject::connect(
        /* pObjSender   */ pReqDbUpdateData,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_requestQueue.addRequestInProgress(pReqDbUpdateData);

    SErrResultInfo errResultInfo = pReqDbUpdateData->getErrResultInfo();

    if( m_bReqModeSingleShot ) // Allows simulating timeouts
    {
        m_reqSimInProgress = ERequestDbUpdateData;
        m_iReqSimDurationStepsCurr = 0;

        executeReqSingleShot();
    }
    else // if( errResultInfo.isErrorResult() || !m_bReqModeSingleShot )
    {
        if( errResultInfo.getResult() == EResultUndefined )
        {
            errResultInfo.setErrResult(ErrResultSuccess);
        }

        pReqDbUpdateData->setErrResultInfo(errResultInfo);
        pReqDbUpdateData->setProgressInPerCent(100);
        pReqDbUpdateData->update();

    } // if( !m_bReqModeSingleShot )

    if( !isAsynchronousRequest(pReqDbUpdateData) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ pReqDbUpdateData,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(pReqDbUpdateData);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReqDbUpdateData->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReqDbUpdateData;

} // updateDbData

//------------------------------------------------------------------------------
CRequest* CDb::updateDbCltData( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent, SDbCltSocketDscr* i_pDbCltSocketDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;

        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ReqIdParent: " + QString::number(i_iReqIdParent);

        if( i_pDbCltSocketDscr == nullptr )
        {
            strAddTrcInfo += ", DbCltSocketDscr: nullptr";
        }
        else
        {
            strAddTrcInfo += ", DbCltSocketDscr: " + i_pDbCltSocketDscr->getAddTrcInfoStr();
        }
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateDbCltData",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("updateDbCltData");

    // Initialized as an asynchronous request (ResultUndefined).
    CRequest* pReqDbCltUpdateData = m_requestQueue.createRequest(
        /* strNameSpace */ "ZS::Apps::Test::AsyncRequests",
        /* strClassName */ "CDb",
        /* iRequest     */ ERequestDbCltUpdateData,
        /* strRequest   */ Request2Str(ERequestDbCltUpdateData),
        /* iReqIdParent */ i_iReqIdParent,
        /* iTimeout_ms  */ i_iTimeout_ms,
        /* bIsBlocking  */ i_bWait );

    if( !QObject::connect(
        /* pObjSender   */ pReqDbCltUpdateData,
        /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_requestQueue.addRequestInProgress(pReqDbCltUpdateData);

    CMsgReqUpdateData* pMsgReq = new CMsgReqUpdateData(
        /* pObjSender       */ this,
        /* pObjReceiver     */ i_pDbCltSocketDscr->m_pDbCltOrGtw,
        /* bMustBeConfirmed */ true,
        /* iReqId           */ pReqDbCltUpdateData->getId(),
        /* iMsgId           */ -1 );
    pMsgReq->setReceiverId(i_pDbCltSocketDscr->m_idxDbCltOrSocketId);
    POST_OR_DELETE_MESSAGE(pMsgReq);

    if( pReqDbCltUpdateData->isBlockingRequest() )
    {
        if( !pReqDbCltUpdateData->wait() )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultTimeout);
            errResultInfo.setAddErrInfoDscr( "Waiting for DbCltUpdateData timed out." );

            if( CErrLog::GetInstance() != nullptr )
            {
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }

        // DbCltUpdateData is an internal sequencer request. No one else may have
        // signal/slot connections to this request. So there is no need to lock
        // the queue on updating this request (but its necessary to update the
        // request to indicate the progress in the request execution tree).
        m_requestQueue.removeRequestInProgress( pReqDbCltUpdateData->getId() );
        //m_requestQueue.lock();
        pReqDbCltUpdateData->setErrResultInfo(errResultInfo);
        pReqDbCltUpdateData->setProgressInPerCent(100);
        pReqDbCltUpdateData->update();
        //m_requestQueue.unlock();
        deleteResultConfirmationMessage(pReqDbCltUpdateData, this);

        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ pReqDbCltUpdateData,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(pReqDbCltUpdateData);

    } // if( pReqDbCltUpdateData->isBlockingRequest() )

    else // if( !pReqDbCltUpdateData->isBlockingRequest() )
    {
        if( i_iTimeout_ms > 0 )
        {
            m_reqDscrTimeout = pReqDbCltUpdateData->getDescriptor();
            m_pTmrReqTimeout->start(i_iTimeout_ms);
        }
    }

    if( !isAsynchronousRequest(pReqDbCltUpdateData) )
    {
        // Don't delete the request right now but return the request to the caller
        // as the caller may want to evaluate some details of the executed request.
        // E.g. if the request has been started by a request sequencer the sequencer
        // wants to update the request id.
        // The request will be set at the request queue to be deleted later
        // (on destroying the queue or if the next sync request will be set).

        // The request has been finished and will be deleted sometimes later.
        // This doesn't matter me ...
        QObject::disconnect(
            /* pObjSender   */ pReqDbCltUpdateData,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        m_requestQueue.setSyncRequestToBeDeletedLater(pReqDbCltUpdateData);
    }

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "Request: " + pReqDbCltUpdateData->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    delete i_pDbCltSocketDscr;
    i_pDbCltSocketDscr = nullptr;

    return pReqDbCltUpdateData;

} // updateDbCltData

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
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRequestTimeout",
        /* strAddInfo   */ strAddTrcInfo );

    m_pTmrReqTimeout->stop();

    CRequest* pReq = m_requestQueue.getRequestInProgress(m_reqDscrTimeout.m_iId);

    if( pReq != nullptr )
    {
        SErrResultInfo errResultInfo = ErrResultInfoError("onRequestTimeout", EResultTimeout, pReq->getRequestName());
        m_requestQueue.lock();
        pReq->lock();
        pReq->setErrResultInfo(errResultInfo);
        pReq->setProgressInPerCent(100);
        pReq->update();
        pReq->unlock();
        m_requestQueue.unlock();

        // The one who calls update for the finished request must delete the request.
        deleteRequest(pReq); // The "onRequestChanged" slot will be called again. The request has already been removed from the queue.
        pReq = nullptr;
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
        strAddTrcInfo  = "ReqDscr: " + i_reqDscr.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", ReqSimInProgress: " + Request2Str(m_reqSimInProgress);
        strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onRequestChanged",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_requestQueue.isRequestInProgress(i_reqDscr.m_iId) )
    {
        CRequest* pReq = m_requestQueue.getRequestInProgress(i_reqDscr.m_iId);

        CRequest* pReqMainInProgress = m_requestQueue.getRequestInProgress();
        bool      bIsMainRequest = false;

        if( pReq == pReqMainInProgress )
        {
            bIsMainRequest = true;
            pReqMainInProgress = nullptr;
        }

        m_requestQueue.lock();

        if( i_reqDscr.m_errResultInfo.isErrorResult() )
        {
            if( !i_reqDscr.m_errResultInfo.isAddedToErrLogModel() )
            {
                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(i_reqDscr.m_errResultInfo);
                }
            }
        }

        SErrResultInfo errResultInfo = i_reqDscr.m_errResultInfo;

        qint64 iReqIdParent = i_reqDscr.m_iParentId;

        if( pReqMainInProgress != nullptr )
        {
            iReqIdParent = pReqMainInProgress->getParentId();
        }

        bool bIsWaitingRequest = false;

        if( iReqIdParent >= 0 )
        {
            bIsWaitingRequest = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
        }

        // If the request has been finished ...
        if( i_reqDscr.m_iProgress_perCent >= 100 )
        {
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }
            m_requestQueue.removeRequestInProgress(i_reqDscr.m_iId);

            if( m_pTmrReqTimeout->isActive() )
            {
                m_pTmrReqTimeout->stop();
            }

        } // if( i_reqDscr.m_iProgress_perCent >= 100 )

        CMsgCon* pMsgConInitiator = nullptr;

        // If the request has not yet been finished ...
        if( i_reqDscr.m_iProgress_perCent < 100 )
        {
            if( bIsMainRequest && !bIsWaitingRequest )
            {
                pMsgConInitiator = pReq->getInitiatorConfirmationMessage();

                // Send intermediate results to the initiator of the request.
                if( pMsgConInitiator != nullptr )
                {
                    pMsgConInitiator = dynamic_cast<CMsgCon*>(pMsgConInitiator->clone());
                    pMsgConInitiator->setErrResultInfo(i_reqDscr.m_errResultInfo);
                    pMsgConInitiator->setProgress(i_reqDscr.m_iProgress_perCent);
                }
            } // if( bIsMainRequest && !bIsWaitingRequest )
        } // if( i_reqDscr.m_iProgress_perCent < 100 )

        // If the request has been finished ..
        else if( i_reqDscr.m_iProgress_perCent >= 100 )
        {
            if( bIsMainRequest && !bIsWaitingRequest )
            {
                pMsgConInitiator = pReq->takeInitiatorConfirmationMessage();

                // Send final results to the initiator of the request.
                if( pMsgConInitiator != nullptr )
                {
                    pMsgConInitiator->setErrResultInfo(errResultInfo);
                    pMsgConInitiator->setProgress(i_reqDscr.m_iProgress_perCent);
                }
            }
        } // if( i_reqDscr.m_iProgress_perCent >= 100 )

        switch( i_reqDscr.m_request )
        {
            case Database::ERequestStartup:
            case Database::ERequestShutdown:
            case Database::ERequestReadSchema:
            case Database::ERequestSaveSchema:
            case Database::ERequestReadData:
            case Database::ERequestSaveData:
            case Database::ERequestSet2Default:
            case Database::ERequestConnect:
            case Database::ERequestDisconnect:
            case Database::ERequestRegister:
            case Database::ERequestUnregister:
            case Database::ERequestSelectSchema:
            case Database::ERequestUpdateSchema:
            case Database::ERequestSelectData:
            case Database::ERequestUpdateData:
            default:
            {
                break;
            }
        }

        // If the main request has been finished ...
        if( i_reqDscr.m_iProgress_perCent >= 100 && bIsMainRequest )
        {
            m_reqSimInProgress = ERequestNone;

            // The main request has been finished. The main request has been created by me
            // on starting the sequence. Usually the one finishing and updating the request
            // should delete the request. This woud be the request sequencer which got the
            // main request as the parent request (becoming its root entry). The sequencer
            // does not delete its parent request as the sequencer is not the owner of the
            // request. The main request got to be deleted by me. But not now as after this
            // "onRequestChanged" slot further "onRequestChanged" slots of other classes may
            // be called. So we don't delete the request right now but the request will be
            // set at the request queue to be deleted later (on destroying the queue or if
            // the next sync request will be set).
            if( !pReq->isLocked() )
            {
                // The request has been finished and will be deleted sometimes later.
                // This doesn't matter me ...
                QObject::disconnect(
                    /* pObjSender   */ pReq,
                    /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

                m_requestQueue.setSyncRequestToBeDeletedLater(pReq);
            }

            if( bIsWaitingRequest )
            {
                CRequestExecTree::GetInstance()->wake(iReqIdParent);
            }
        } // if( i_reqDscr.m_iProgress_perCent >= 100 && bIsMainRequest )

        if( pMsgConInitiator != nullptr )
        {
            POST_OR_DELETE_MESSAGE(pMsgConInitiator);
            pMsgConInitiator = nullptr;
        }

        m_requestQueue.unlock();

        if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
        {
            executeNextPostponedRequest();
        }
    } // if( m_requestQueue.isRequestInProgress(i_reqDscr.m_iId) )

} // onRequestChanged

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CDb::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    CMsg* pMsg = dynamic_cast<CMsg*>(i_pEv);

    if( pMsg != nullptr )
    {
        CMsgReq* pMsgReqInitiator = nullptr;
        CMsgInd* pMsgIndInitiator = nullptr;
        CMsgCon* pMsgConExec = nullptr;

        if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )
        {
            pMsgReqInitiator = dynamic_cast<CMsgReq*>(pMsg);
        }
        else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeInd )
        {
            pMsgIndInitiator = dynamic_cast<CMsgInd*>(pMsg);
        }
        else if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeCon )
        {
            pMsgConExec = dynamic_cast<CMsgCon*>(pMsg);
        }

        QString strAddTrcInfo;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
        {
            int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
            strAddTrcInfo  = "Msg: " + pMsg->getAddTrcInfoStr(iAddTrcInfoDetailLevel);
            strAddTrcInfo += ", RequestQueue: " + m_requestQueue.getAddTrcInfoStr(iAddTrcInfoDetailLevel);
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        SErrResultInfo errResultInfo = ErrResultInfoSuccess(pMsg->msgTypeToStr());

        if( pMsgReqInitiator != nullptr )
        {
            CMsgCon*  pMsgConInitiator = nullptr;
            CRequest* pReq             = nullptr;
            qint64    iReqIdParent     = pMsgReqInitiator->getRequestId();

            switch( static_cast<int>(pMsg->type()) )
            {
                case EMsgTypeReqStartup:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqStartup*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                    }
                    pReq = startup(iReqIdParent);
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqStartup

                case EMsgTypeReqShutdown:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqShutdown*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                    }
                    pReq = shutdown(iReqIdParent);
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqShutdown:

                case EMsgTypeReqReadSchema:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqReadSchema*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                    }
                    pReq = readSchema(iReqIdParent);
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqReadSchema

                case EMsgTypeReqSaveSchema:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqSaveSchema*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                    }
                    pReq = saveSchema(iReqIdParent);
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqSaveSchema

                case EMsgTypeReqReadData:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqReadData*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                    }
                    pReq = readData(iReqIdParent);
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqReadData

                case EMsgTypeReqSaveData:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqSaveData*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                    }
                    pReq = saveData(iReqIdParent);
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqSaveData

                case EMsgTypeReqSet2Default:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqSet2Default*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                    }
                    pReq = set2Default(iReqIdParent);
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqSet2Default

                case EMsgTypeReqConnect:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqConnect*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                        pMsgConInitiator->setReceiverId( pMsgReqInitiator->getSenderId() ); // SocketId if sent by gateway
                    }
                    pReq = onReceivedMsgReqConnect( dynamic_cast<CMsgReqConnect*>(pMsgReqInitiator) );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqConnect

                case EMsgTypeReqDisconnect:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqDisconnect*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                        pMsgConInitiator->setReceiverId( pMsgReqInitiator->getSenderId() ); // SocketId if sent by gateway
                    }
                    pReq = onReceivedMsgReqDisconnect( dynamic_cast<CMsgReqDisconnect*>(pMsgReqInitiator) );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqDisconnect

                case EMsgTypeReqRegister:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqRegister*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                        pMsgConInitiator->setReceiverId( pMsgReqInitiator->getSenderId() ); // SocketId if sent by gateway
                    }
                    pReq = onReceivedMsgReqRegister( dynamic_cast<CMsgReqRegister*>(pMsgReqInitiator) );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqRegister

                case EMsgTypeReqUnregister:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqUnregister*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                        pMsgConInitiator->setReceiverId( pMsgReqInitiator->getSenderId() ); // SocketId if sent by gateway
                    }
                    pReq = onReceivedMsgReqUnregister( dynamic_cast<CMsgReqUnregister*>(pMsgReqInitiator) );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqUnregister

                case EMsgTypeReqSelectSchema:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqSelectSchema*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                        pMsgConInitiator->setReceiverId( pMsgReqInitiator->getSenderId() ); // SocketId if sent by gateway
                    }
                    pReq = onReceivedMsgReqSelectSchema( dynamic_cast<CMsgReqSelectSchema*>(pMsgReqInitiator) );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqSelectSchema

                case EMsgTypeReqUpdateSchema:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqUpdateSchema*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                        pMsgConInitiator->setReceiverId( pMsgReqInitiator->getSenderId() ); // SocketId if sent by gateway
                    }
                    pReq = onReceivedMsgReqUpdateSchema( dynamic_cast<CMsgReqUpdateSchema*>(pMsgReqInitiator) );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqUpdateSchema

                case EMsgTypeReqSelectData:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqSelectData*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                        pMsgConInitiator->setReceiverId( pMsgReqInitiator->getSenderId() ); // SocketId if sent by gateway
                    }
                    pReq = onReceivedMsgReqSelectData( dynamic_cast<CMsgReqSelectData*>(pMsgReqInitiator) );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqSelectData

                case EMsgTypeReqUpdateData:
                {
                    pMsgReqInitiator = dynamic_cast<CMsgReqUpdateData*>(pMsg);
                    if( pMsgReqInitiator == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    if( pMsgReqInitiator->mustBeConfirmed() )
                    {
                        pMsgConInitiator = pMsgReqInitiator->createConfirmationMessage();
                        pMsgConInitiator->setReceiverId( pMsgReqInitiator->getSenderId() ); // SocketId if sent by gateway
                    }
                    pReq = onReceivedMsgReqUpdateData( dynamic_cast<CMsgReqUpdateData*>(pMsgReqInitiator) );
                    bEventHandled = true;
                    break;

                } // case EMsgTypeReqUpdateData

                case EBaseMsgTypeReqContinue:
                {
                    m_bMsgReqContinuePending = false;

                    if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )
                    {
                        CRequest* pReqPostponed = m_requestQueue.takeFirstPostponedRequest();

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
                                case ERequestReadSchema:
                                {
                                    executeReadSchemaRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestSaveSchema:
                                {
                                    executeSaveSchemaRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestReadData:
                                {
                                    executeReadDataRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestSaveData:
                                {
                                    executeSaveDataRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestSet2Default:
                                {
                                    executeSet2DefaultRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestConnect:
                                {
                                    executeConnectRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestDisconnect:
                                {
                                    executeDisconnectRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestRegister:
                                {
                                    executeRegisterRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestUnregister:
                                {
                                    executeUnregisterRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestSelectSchema:
                                {
                                    executeSelectSchemaRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestUpdateSchema:
                                {
                                    executeUpdateSchemaRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestSelectData:
                                {
                                    executeSelectDataRequest(pReqPostponed);
                                    pReqPostponed = nullptr; // Has either become the request in progress or is set as SyncRequestToBeDeletedLater.
                                    break;
                                }
                                case ERequestUpdateData:
                                {
                                    executeUpdateDataRequest(pReqPostponed);
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

                    } // if( !m_requestQueue.isRequestInProgress() && m_requestQueue.hasPostponedRequest() )

                    bEventHandled = true;
                    break;

                } // case EBaseMsgTypeReqContinue

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
                }

            } // switch( i_pMsg->type() )

            if( isAsynchronousRequest(pReq) )
            {
                pReq->setInitiatorConfirmationMessage(pMsgConInitiator);
                pMsgConInitiator = nullptr;
            }
            else // if( !isAsynchronousRequest(pReq) )
            {
                bool bIsWaitingRequest = false;

                if( iReqIdParent >= 0 )
                {
                    bIsWaitingRequest = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
                }

                if( bIsWaitingRequest )
                {
                    CRequestExecTree::GetInstance()->wake(iReqIdParent);
                }
                else if( pMsgConInitiator != nullptr )
                {
                    if( pReq != nullptr )
                    {
                        errResultInfo = pReq->getErrResultInfo();
                    }
                    else
                    {
                        errResultInfo.setResult(EResultSuccess);
                    }
                    pMsgConInitiator->setErrResultInfo(errResultInfo);
                    pMsgConInitiator->setProgress(100);

                    POST_OR_DELETE_MESSAGE(pMsgConInitiator);
                    pMsgConInitiator = nullptr;
                }
            } // if( !isAsynchronousRequest(pReq) )

            delete pMsgConInitiator;
            pMsgConInitiator = nullptr;

        } // if( pMsgReqInitiator != nullptr )

        else if( pMsgIndInitiator != nullptr )
        {
            // Please note that messages may be sent by the gateway right before the gateway
            // is destroyed. In this case the message may be received right after the gateway
            // has been destroyed, we can't access the message sender (the gateway) anymore
            // and also a dynamic_cast from the message sender to the gateway object will
            // result in an access violation exception.

            Ipc::ESocketType socketType = Ipc::ESocketTypeUndefined;

            switch( static_cast<int>(pMsg->type()) )
            {
                case EMsgTypeIndConnected:
                {
                    CMsgIndConnected* pMsgInd = dynamic_cast<CMsgIndConnected*>(pMsg);
                    if( pMsgInd == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }

                    socketType = pMsgInd->getSocketType();

                    if( socketType < 0 || socketType >= Ipc::ESocketTypeCount )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultInvalidValue);
                        errResultInfo.setAddErrInfoDscr( "Socket type " + Ipc::socketType2Str(socketType) + " out of range." );
                    }
                    else if( pMsgInd->getSender() == nullptr || pMsgInd->getSender() != m_arpDbGtw[socketType] )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultInvalidSenderObjInMessage);
                    }
                    if( errResultInfo.isErrorResult() )
                    {
                        if( CErrLog::GetInstance() != nullptr )
                        {
                            CErrLog::GetInstance()->addEntry(errResultInfo);
                        }
                    }
                    else
                    {
                        onReceivedMsgIndConnected(pMsgInd);
                    }
                    bEventHandled = true;
                    break;

                } // case EMsgTypeIndConnected

                case EMsgTypeIndDisconnected:
                {
                    CMsgIndDisconnected* pMsgInd = dynamic_cast<CMsgIndDisconnected*>(pMsg);
                    if( pMsgInd == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                    }
                    socketType = pMsgInd->getSocketType();

                    if( socketType < 0 || socketType >= Ipc::ESocketTypeCount )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultInvalidValue);
                        errResultInfo.setAddErrInfoDscr( "Socket type " + Ipc::socketType2Str(socketType) + " out of range." );
                    }
                    else if( pMsgInd->getSender() == nullptr || pMsgInd->getSender() != m_arpDbGtw[socketType] )
                    {
                        errResultInfo.setSeverity(EResultSeverityError);
                        errResultInfo.setResult(EResultInvalidSenderObjInMessage);
                    }
                    if( errResultInfo.isErrorResult() )
                    {
                        if( CErrLog::GetInstance() != nullptr )
                        {
                            CErrLog::GetInstance()->addEntry(errResultInfo);
                        }
                    }
                    else
                    {
                        onReceivedMsgIndDisconnected(pMsgInd);
                    }
                    bEventHandled = true;
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
            } // switch( pMsg->type() )
        } // if( pMsgIndInitiator != nullptr )

        else if( pMsgConExec != nullptr )
        {
            // Please note that messages may be sent by the gateway right before the gateway
            // is destroyed. In this case the message may be received right after the gateway
            // has been destroyed, we can't access the message sender (the gateway) anymore
            // and also a dynamic_cast from the message sender to the gateway object will
            // result in an access violation exception.

            if( pMsgConExec->getMsgType() == EMsgTypeConStartup )
            {
                CMsgConStartup* pMsgConStartup = dynamic_cast<CMsgConStartup*>(pMsgConExec);
                if( pMsgConStartup == nullptr )
                {
                    throw CException(__FILE__,__LINE__,EResultMessageTypeMismatch);
                }

                Ipc::ESocketType socketType = pMsgConStartup->getSocketType();

                if( socketType >= 0 && socketType < Ipc::ESocketTypeCount )
                {
                    if( m_arpDbGtwThread[socketType] != nullptr )
                    {
                        if( m_arpDbGtw[socketType] == nullptr )
                        {
                            try
                            {
                                m_arpDbGtw[socketType] = dynamic_cast<CDbGateway*>(pMsgConExec->getSender());
                            }
                            catch(...)
                            {
                                m_arpDbGtw[socketType] = nullptr;
                            }
                        }
                    }
                } // if( socketType >= 0 && socketType < Ipc::ESocketTypeCount )
            } // if( pMsgConExec->getMsgType() == EMsgTypeConStartup )

            CRequest* pReq = m_requestQueue.getRequestInProgress(pMsgConExec->getRequestId());

            if( pReq != nullptr )
            {
                int iProgress_perCent = pMsgConExec->getProgress();

                errResultInfo = pMsgConExec->getErrResultInfo();

                if( errResultInfo.getErrSource().isObjectPathEmpty() )
                {
                    errResultInfo.setErrSource(nameSpace(), className(), objectName(), pMsg->msgTypeToStr());
                }

                m_requestQueue.lock();

                pReq->setErrResultInfo(errResultInfo);
                pReq->setProgressInPerCent(iProgress_perCent);
                pReq->lock();
                pReq->update(); // "onRequestChanged" is called as a reentry removing the request from the queue.
                pReq->unlock();

                if( iProgress_perCent >= 100 )
                {
                    // The one who calls update for the finished request must delete the request.
                    deleteRequest(pReq); // The "onRequestChanged" slot will be called again. The request has already been removed from the queue.
                    pReq = nullptr;
                }
                m_requestQueue.unlock();

            } // if( pReq != nullptr )

            bEventHandled = true;

        } // if( pMsgConExec != nullptr )
    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event
