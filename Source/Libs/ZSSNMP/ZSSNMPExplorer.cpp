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

$Revision: 10 $

$Author: Christian Bauer $

$History: ZSSNMPExplorer.cpp $
 * 
 * *****************  Version 10  *****************
 * User: Christian Bauer Date: 22.01.10   Time: 22:11
 * Updated in $/ZSSNMP/Source
 * 
 * *****************  Version 9  *****************
 * User: Christian Bauer Date: 10.01.10   Time: 19:19
 * Updated in $/ZSSNMP/Source

*******************************************************************************/

#include <QtCore/qcoreapplication.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtimer.h>
#include <QtCore/qthread.h>
#ifndef NO_GUI
#include <QtGui/qbitmap.h>
#endif
#define HAVE_SOCKLEN_T
#include "net-snmp/net-snmp-config.h"
#include "net-snmp/net-snmp-includes.h"
#undef index
#include "ZSSNMP/Include/ZSSNMPExplorer.h"
#include "ZSSNMP/Include/ZSSNMPOidNode.h"
#include "ZSSNMP/Include/ZSSNMPOidNodesModel.h"
#include "ZSSNMP/Include/ZSSNMPInstanceNode.h"
#include "ZSSNMP/Include/ZSSNMPInstanceNodesModel.h"
#include "ZSSNMP/Include/ZSSNMPMsg.h"
#include "ZSSNMP/Include/ZSSNMPSession.h"
#include "ZSTrace/Include/ZSTrcAdminObj.h"
#include "ZSTrace/Include/ZSTrcMethod.h"
#include "ZSTrace/Include/ZSTrcServer.h"
#include "ZSSys/Include/ZSSysErrLogModel.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysTime.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;
using namespace ZS::Trace;


/*******************************************************************************
struct SHostSettings
*******************************************************************************/

/*==============================================================================
public: // default ctor
==============================================================================*/

//------------------------------------------------------------------------------
SHostSettings::SHostSettings() :
//------------------------------------------------------------------------------
    m_strName("127.0.0.1"),
    m_uPort(161),
    m_strCommunity("public")
{
} // default ctor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool SHostSettings::operator == ( const SHostSettings& i_settingsOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if( m_strName != i_settingsOther.m_strName )
    {
        bEqual = false;
    }
    else if( m_uPort != i_settingsOther.m_uPort )
    {
        bEqual = false;
    }
    else if( m_strCommunity != i_settingsOther.m_strCommunity )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool SHostSettings::operator != ( const SHostSettings& i_settingsOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_settingsOther);
}


/*******************************************************************************
class CExplorer : public QObject
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
const ZS::SEnumEntry s_arEnumStrStates[CExplorer::EStateCount] =
//------------------------------------------------------------------------------
{
    ZS::SEnumEntry( CExplorer::EStateInit,                     "Init"                     ),
    ZS::SEnumEntry( CExplorer::EStateSessionClosed,            "SessionClosed"            ),
    ZS::SEnumEntry( CExplorer::EStateSessionOpened,            "SessionOpened"            ),
    ZS::SEnumEntry( CExplorer::EStateShutdown,                 "Shutdown"                 ),
    ZS::SEnumEntry( CExplorer::EStateWaitingForSessionCreated, "WaitingForSessionCreated" ),
    ZS::SEnumEntry( CExplorer::EStateWaitingForSessionOpened,  "WaitingForSessionOpened"  ),
    ZS::SEnumEntry( CExplorer::EStateWaitingForSessionClosed,  "WaitingForSessionClosed"  ),
    ZS::SEnumEntry( CExplorer::EStateWaitingForSessionDeleted, "WaitingForSessionDeleted" )
};

//------------------------------------------------------------------------------
QString CExplorer::state2Str( int i_iState )
//------------------------------------------------------------------------------
{
    return ZS::SEnumEntry::val2Str(s_arEnumStrStates,EStateCount,i_iState);
}

//------------------------------------------------------------------------------
const ZS::SEnumEntry s_arEnumStrRequests[CExplorer::ERequestCount] =
//------------------------------------------------------------------------------
{
    ZS::SEnumEntry( CExplorer::ERequestNone,         "None"         ),
    ZS::SEnumEntry( CExplorer::ERequestOpenSession,  "OpenSession"  ),
    ZS::SEnumEntry( CExplorer::ERequestCloseSession, "CloseSession" ),
    ZS::SEnumEntry( CExplorer::ERequestShutdown,     "Shutdown"     ),
    ZS::SEnumEntry( CExplorer::ERequestGetValue,     "GET"          ),
    ZS::SEnumEntry( CExplorer::ERequestGetNextValue, "GETNEXT"      ),
    ZS::SEnumEntry( CExplorer::ERequestGetBulk,      "GETBULK"      ),
    ZS::SEnumEntry( CExplorer::ERequestWalk,         "WALK"         )
};

//------------------------------------------------------------------------------
QString CExplorer::request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return ZS::SEnumEntry::val2Str(s_arEnumStrRequests,ERequestCount,i_iRequest);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CExplorer::CExplorer(
    const QString& i_strObjName,
    const QString& i_strObjNameSession,
    QSettings*     i_pSettingsFile,
    const QString& i_strSettingsKey,
    bool           i_bSettingsFileAutoReadEnabled ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strObjName(i_strObjName),
    m_strObjNameSession(i_strObjNameSession),
    m_state(EStateInit),
    m_requestInProgress(ERequestNone),
    m_iIdRequestInProgress(-1),
    m_iIdRequestWaitingFor(-1),
    m_fRequestStartTime_ms(0.0),
    //m_ariRequestTimeouts_ms[ERequestCount]
    //m_arfRequestDurationLastRecent_ms[ERequestCount]
    m_pOidRootWalkRequest(NULL),
    m_ptrListMsgReqPending(),
    m_pModelErrLog(NULL),
    m_strSettingsKey(i_strSettingsKey),
    m_pSettingsFile(i_pSettingsFile),
    m_bSettingsFileAutoReadEnabled(i_bSettingsFileAutoReadEnabled),
    m_iReadSettingsInterval_ms(5000),
    m_pTimerReadSettings(NULL),
    m_hostSettings(),
    m_bErrLogModelOutputEnabled(true),
    m_pModelOidNodes(NULL),
    m_pModelInstanceNodes(NULL),
    m_pTimerSession(NULL),
    m_pSessionThread(NULL),
    m_pSession(NULL),
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
        m_strObjName = "SNMPExplorer";
    }
    if( m_strObjNameSession.isEmpty() )
    {
        m_strObjNameSession = "SNMPSession";
    }

    if( m_strSettingsKey.isEmpty() )
    {
        m_strSettingsKey = m_strObjName;
    }

    m_pTrcAdminObj = CTrcServer::getTrcAdminObj(
        /* traceType         */ ETraceTypeMth,
        /* strClassNameSpace */ "ZS::SNMP",
        /* strClassName      */ "CExplorer",
        /* strObjNameSpace   */ "",
        /* strObjName        */ m_strObjName );

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "ctor",
        /* strAddTrcInfo */ strAddTrcInfo );

    if( m_pSettingsFile == NULL )
    {
        m_pModelErrLog = CModelErrLog::getInstance();
    }
    else
    {
        m_pModelErrLog = CModelErrLog::getInstance(m_pSettingsFile);

        readHostSettings();
        saveHostSettings(); // to ensure that the settings file exists with at least default entries

        m_pTimerReadSettings = new QTimer(this);

        if( !connect(
            /* pObjSender   */ m_pTimerReadSettings,
            /* szSignal     */ SIGNAL(timeout()),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onTimeoutReadSettings()) ) )
        {
            throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
        }

        if( m_bSettingsFileAutoReadEnabled && m_iReadSettingsInterval_ms > 0 )
        {
            m_pTimerReadSettings->start(m_iReadSettingsInterval_ms);
        }
    }

    // Initializes net-snmp MIB modules
    //---------------------------------

    // "add_mibdir" is used to add the specified directory to the path of locations
    // which are searched for files containing MIB modules. Note that this does not
    // actually load the MIB modules located in that directory, but is simply an
    // initialisation step to make them available to "netsnmp_read_module". This function
    // returns a count of files found in the directory, or a -1 if there is an error.

    QString strMibDir = QCoreApplication::applicationDirPath();

    if( !strMibDir.endsWith("/") )
    {
        strMibDir += "/Mibs";
    }
    else
    {
        strMibDir += "Mibs";
    }

    if( m_pSettingsFile != NULL )
    {
        strMibDir = m_pSettingsFile->value(m_strSettingsKey+"/MibDir",strMibDir).toString();
        m_pSettingsFile->setValue(m_strSettingsKey+"/MibDir",strMibDir);
        m_pSettingsFile->sync();
    }

    netsnmp_set_mib_directory(strMibDir.toAscii());

    //int iRes = add_mibdir(strMibDir.toAscii());

    //if( m_pModelErrLog != NULL && iRes < 0 )
    //{
    //    m_pModelErrLog->addEntry(
    //        /* dateTime    */ QDateTime::currentDateTime(),
    //        /* severity    */ ZS::EResultSeverityInfo,
    //        /* iErrNr      */ ZS::EResultFileOpenError,
    //        /* strErrNr    */ ZS::result2Str(ZS::EResultFileOpenError),
    //        /* strSource   */ m_strObjName,
    //        /* strAddTrcInfo  */ "add_mibdir(" + strMibDir + ")",
    //        /* strProposal */ "Check directory" );
    //}

    // "init_mib" is a convenience function that configures the MIB directory search path
    // (using add_mibdir ), set up the internal MIB framework (using init_mib_internals ),
    // and then loads the appropriate MIB modules (using netsnmp_read_module and read_mib).
    // It should be called before any other routine that manipulates or accesses the MIB tree.
    init_mib();

    // Initialize net-snmp
    //--------------------

    init_snmp("snmpapp");

    // windows32 specific initialization (is a noop on unix)
    SOCK_STARTUP;

    // Key nodes model
    //----------------

    m_pModelOidNodes = new CModelOidNodes(this);
    m_pModelInstanceNodes = new CModelInstanceNodes(this);

    // State machine
    //--------------

    m_pTimerSession = new QTimer(this);
    m_pTimerSession->setSingleShot(true);

    if( !connect(
        /* pObjSender   */ m_pTimerSession,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onSessionTimeout()) ) )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
    }

    m_state = EStateSessionClosed;

} // ctor

//------------------------------------------------------------------------------
CExplorer::~CExplorer()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "dtor",
        /* strAddTrcInfo */ strAddTrcInfo );

    // net-snmp windows32 specific cleanup (is a noop on unix)
    SOCK_CLEANUP;

    try
    {
        delete m_pOidRootWalkRequest;
    }
    catch(...)
    {
    }
    m_pOidRootWalkRequest = NULL;

    if( m_pSessionThread != NULL && m_pSessionThread->isRunning() )
    {
        shutdown();
    }
    m_pSessionThread = NULL;

    try
    {
        delete m_pModelInstanceNodes;
    }
    catch(...)
    {
    }
    m_pModelInstanceNodes = NULL;

    try
    {
        delete m_pModelOidNodes;
    }
    catch(...)
    {
    }
    m_pModelOidNodes = NULL;

    m_pTimerReadSettings = NULL;
    m_pSettingsFile = NULL;
    m_pTimerSession = NULL;
    m_pSession = NULL;
    m_pModelErrLog = NULL;

    // "shutdown_mib" will clear the information that was gathered by "netsnmp_read_module",
    // "add_mibdir" and "add_module_replacement". It is strongly recommended that one does
    // not invoke "shutdown_mib" while there are SNMP sessions being actively managed.
    shutdown_mib();

    CTrcServer::releaseTrcAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = NULL;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CExplorer::getName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
QString CExplorer::getSessionName() const
//------------------------------------------------------------------------------
{
    return m_strObjNameSession;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CExplorer::setErrLogModelOutputEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    m_bErrLogModelOutputEnabled = i_bEnabled;
}

//------------------------------------------------------------------------------
bool CExplorer::isErrLogModelOutputEnabled() const
//------------------------------------------------------------------------------
{
    return m_bErrLogModelOutputEnabled;
}

/*==============================================================================
public: // instance methods to trigger system startup and shutdown
==============================================================================*/

//------------------------------------------------------------------------------
ZS::SRequestResult CExplorer::openSession()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "openSession",
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
        CMsgReqOpenSession* pMsgReq = new CMsgReqOpenSession(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* hostSettings */ m_hostSettings );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        m_ptrListMsgReqPending.append(pMsgReq);
        pMsgReq = NULL;
    }
    else
    {
        switch( m_state )
        {
            case EStateSessionClosed:
            {
                m_requestInProgress = ERequestOpenSession; // set it here and not before switch because of addInfo in state machine exception
                emit stateChanged(this,m_state,m_requestInProgress);

                CMsgReqOpenSession* pMsgReq = new CMsgReqOpenSession(
                    /* pObjSender   */ this,
                    /* pObjReceiver */ this,
                    /* hostSettings */ m_hostSettings );
                requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
                requestResult.m_result = EResultUndefined;
                QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
                pMsgReq = NULL;
                break;
            }
            case EStateSessionOpened:
            {
                break;
            }
            case EStateInit:
            case EStateShutdown:
            case EStateWaitingForSessionCreated:
            case EStateWaitingForSessionOpened:
            case EStateWaitingForSessionClosed:
            case EStateWaitingForSessionDeleted:
            default:
            {
                throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
            }
        } // switch( m_state )
    } // if( m_requestInProgress == ERequestNone )

    return requestResult;

} // openSession

//------------------------------------------------------------------------------
ZS::SRequestResult CExplorer::closeSession()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "closeSession",
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
    else
    {
        switch( m_state )
        {
            case EStateSessionClosed:
            {
                break;
            }
            case EStateSessionOpened:
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
                break;
            }
            case EStateInit:
            case EStateShutdown:
            case EStateWaitingForSessionCreated:
            case EStateWaitingForSessionOpened:
            case EStateWaitingForSessionClosed:
            case EStateWaitingForSessionDeleted:
            default:
            {
                throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
            }
        } // switch( m_state )
    } // if( m_requestInProgress == ERequestNone )

    return requestResult;

} // closeSession

//------------------------------------------------------------------------------
void CExplorer::shutdown()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "shutdown",
        /* strAddTrcInfo */ strAddTrcInfo );

    CMsgReq*       pMsgReq;
    SErrResultInfo errResultInfo;

    errResultInfo.setSeverity(ZS::EResultSeverityError);
    errResultInfo.setResult(ZS::EResultRequestAborted);
    errResultInfo.setResultStr(ZS::result2Str(ZS::EResultRequestAborted));
    errResultInfo.setErrSource(m_strObjName);

    m_state = EStateShutdown;
    m_requestInProgress = ERequestShutdown;
    emit stateChanged(this,m_state,m_requestInProgress);

    // Delete all pending requests ..
    while( !m_ptrListMsgReqPending.isEmpty() )
    {
        pMsgReq = m_ptrListMsgReqPending.takeFirst();

        errResultInfo.setAddErrInfoDscr(pMsgReq->msgTypeToStr());
        emit requestExecuted(this,pMsgReq->getMsgId(),errResultInfo);

        try
        {
            delete pMsgReq;
        }
        catch(...)
        {
        }
        pMsgReq = NULL;
    }

    if( m_pSessionThread != NULL )
    {
        if( m_pSessionThread->isRunning() )
        {
            CMsgReqCloseSession* pMsgReqSession = new CMsgReqCloseSession(
                /* pObjSender       */ this,
                /* pObjReceiver     */ m_pSession,
                /* bMustBeConfirmed */ false );
            emit sendingMessage(pMsgReqSession->getReceiver(),pMsgReqSession);
            QCoreApplication::postEvent(pMsgReqSession->getReceiver(),pMsgReqSession);
            pMsgReqSession = NULL;

            m_pSessionThread->quit();
            m_pSessionThread->wait(30000);
        }
        // Still running (wait timed out) ..
        if( m_pSessionThread->isRunning() )
        {
            m_pSessionThread->deleteLater();
        }
        else
        {
            try
            {
                delete m_pSessionThread;
            }
            catch(...)
            {
            }
        }
        m_pSessionThread = NULL;
    }

    m_state = EStateSessionClosed;
    m_requestInProgress = ERequestNone;
    emit stateChanged(this,m_state,m_requestInProgress);

} // shutdown

/*==============================================================================
public: // instance methods changing and reading the host settings
==============================================================================*/

//------------------------------------------------------------------------------
ZS::SRequestResult CExplorer::setHostSettings( const SHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", " + i_hostSettings.m_strName + ":" + QString::number(i_hostSettings.m_uPort) + "/" + i_hostSettings.m_strCommunity;
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "setHostSettings",
        /* strAddTrcInfo */ strAddTrcInfo );

    ZS::SRequestResult requestResult;

    if( m_state != EStateInit )
    {
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
            CMsgReqOpenSession* pMsgReq = new CMsgReqOpenSession(
                /* pObjSender   */ this,
                /* pObjReceiver */ this,
                /* hostSettings */ i_hostSettings );
            requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
            requestResult.m_result = EResultUndefined;
            m_ptrListMsgReqPending.append(pMsgReq);
            pMsgReq = NULL;
        }
        else
        {
            switch( m_state )
            {
                case EStateSessionClosed:
                case EStateSessionOpened:
                {
                    break;
                }
                case EStateInit:
                case EStateShutdown:
                case EStateWaitingForSessionCreated:
                case EStateWaitingForSessionOpened:
                case EStateWaitingForSessionClosed:
                case EStateWaitingForSessionDeleted:
                default:
                {
                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
                }
            } // switch( m_state )

            // On changing the host settings we try to automatically open a new SNMP session
            //------------------------------------------------------------------------------

            // For the new host the SNMP nodes must be newly requested.
            // The current node models will be cleared.
            if( m_pModelInstanceNodes != NULL )
            {
                m_pModelInstanceNodes->clear();
            }
            if( m_pModelOidNodes != NULL )
            {
                m_pModelOidNodes->clear();
            }

            m_requestInProgress = ERequestOpenSession; // set it here and not before switch because of addInfo in state machine exception
            emit stateChanged(this,m_state,m_requestInProgress);

            CMsgReqOpenSession* pMsgReq = new CMsgReqOpenSession(
                /* pObjSender   */ this,
                /* pObjReceiver */ this,
                /* hostSettings */ i_hostSettings );
            requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
            requestResult.m_result = EResultUndefined;
            QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
            pMsgReq = NULL;

        } // if( m_requestInProgress == ERequestNone )
    } // if( m_state != EStateInit )

    return requestResult;

} // setHostSettings

//------------------------------------------------------------------------------
SHostSettings CExplorer::getHostSettings() const
//------------------------------------------------------------------------------
{
    return m_hostSettings;
}

//------------------------------------------------------------------------------
void CExplorer::saveHostSettings()
//------------------------------------------------------------------------------
{
    if( m_pSettingsFile != NULL )
    {
        m_pSettingsFile->setValue(m_strSettingsKey+"/HostName",m_hostSettings.m_strName);
        m_pSettingsFile->setValue(m_strSettingsKey+"/HostPort",m_hostSettings.m_uPort);
        m_pSettingsFile->setValue(m_strSettingsKey+"/CommunityString",m_hostSettings.m_strCommunity);
        m_pSettingsFile->sync();
    }

} // saveHostSettings

//------------------------------------------------------------------------------
void CExplorer::readHostSettings()
//------------------------------------------------------------------------------
{
    if( m_pSettingsFile != NULL )
    {
        m_pSettingsFile->sync();

        SHostSettings hostSettingsCurr = m_hostSettings;

        hostSettingsCurr.m_strName = m_pSettingsFile->value(m_strSettingsKey+"/HostName",hostSettingsCurr.m_strName).toString();
        hostSettingsCurr.m_uPort = m_pSettingsFile->value(m_strSettingsKey+"/HostPort",hostSettingsCurr.m_uPort).toUInt();
        hostSettingsCurr.m_strCommunity = m_pSettingsFile->value(m_strSettingsKey+"/CommunityString",hostSettingsCurr.m_strCommunity).toString();

        if( hostSettingsCurr != m_hostSettings )
        {
            if( m_state == EStateInit )
            {
                m_hostSettings = hostSettingsCurr;
            }
            else
            {
                setHostSettings(hostSettingsCurr);
            }
        }
    }

} // readHostSettings

/*==============================================================================
public: // instance methods sending SNMP data packages to the SNMP host
==============================================================================*/

//------------------------------------------------------------------------------
ZS::SRequestResult CExplorer::requestValue( const QString& i_strOid )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", Oid:" + i_strOid;
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "requestValue",
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
        CMsgReqGetValue* pMsgReq = new CMsgReqGetValue(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oid          */ i_strOid );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        m_ptrListMsgReqPending.append(pMsgReq);
        pMsgReq = NULL;
    }
    else
    {
        switch( m_state )
        {
            case EStateSessionClosed:
            {
                requestResult.m_result = EResultRequestRefused;
                break;
            }
            case EStateSessionOpened:
            {
                m_requestInProgress = ERequestGetValue; // set it here and not before switch because of addInfo in state machine exception
                emit stateChanged(this,m_state,m_requestInProgress);

                CMsgReqGetValue* pMsgReq = new CMsgReqGetValue(
                    /* pObjSender   */ this,
                    /* pObjReceiver */ this,
                    /* oid          */ i_strOid );
                requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
                requestResult.m_result = EResultUndefined;
                QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
                pMsgReq = NULL;
                break;
            }
            case EStateInit:
            case EStateShutdown:
            case EStateWaitingForSessionCreated:
            case EStateWaitingForSessionOpened:
            case EStateWaitingForSessionClosed:
            case EStateWaitingForSessionDeleted:
            default:
            {
                throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
            }
        } // switch( m_state )
    } // if( m_requestInProgress == ERequestNone )

    return requestResult;

} // requestValue

//------------------------------------------------------------------------------
ZS::SRequestResult CExplorer::requestValues( const TOidPtrList& i_oidPtrList )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);

        for( int idxVal = 0; idxVal < i_oidPtrList.size(); idxVal++ )
        {
            COid* pOid = i_oidPtrList[idxVal];

            if( pOid == NULL )
            {
                strAddTrcInfo += ", Oid[" + QString::number(idxVal) + "]:NULL";
            }
            else
            {
                strAddTrcInfo += ", Oid[" + QString::number(idxVal) + "]:" + pOid->toString();
            }
        }
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "requestValues",
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
        CMsgReqGetValue* pMsgReq = new CMsgReqGetValue(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oidPtrList   */ i_oidPtrList );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        m_ptrListMsgReqPending.append(pMsgReq);
        pMsgReq = NULL;
    }
    else
    {
        switch( m_state )
        {
            case EStateSessionClosed:
            {
                requestResult.m_result = EResultRequestRefused;
                break;
            }
            case EStateSessionOpened:
            {
                m_requestInProgress = ERequestGetValue; // set it here and not before switch because of addInfo in state machine exception
                emit stateChanged(this,m_state,m_requestInProgress);

                CMsgReqGetValue* pMsgReq = new CMsgReqGetValue(
                    /* pObjSender   */ this,
                    /* pObjReceiver */ this,
                    /* oidPtrList   */ i_oidPtrList );
                requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
                requestResult.m_result = EResultUndefined;
                QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
                pMsgReq = NULL;
                break;
            }
            case EStateInit:
            case EStateShutdown:
            case EStateWaitingForSessionCreated:
            case EStateWaitingForSessionOpened:
            case EStateWaitingForSessionClosed:
            case EStateWaitingForSessionDeleted:
            default:
            {
                throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
            }
        } // switch( m_state )
    } // if( m_requestInProgress == ERequestNone )

    return requestResult;

} // requestValues

//------------------------------------------------------------------------------
ZS::SRequestResult CExplorer::requestNextValue( const QString& i_strOid )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", Oid:" + i_strOid;
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "requestNextValue",
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
        CMsgReqGetNextValue* pMsgReq = new CMsgReqGetNextValue(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oid          */ i_strOid );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        m_ptrListMsgReqPending.append(pMsgReq);
        pMsgReq = NULL;
    }
    else
    {
        switch( m_state )
        {
            case EStateSessionClosed:
            {
                requestResult.m_result = EResultRequestRefused;
                break;
            }
            case EStateSessionOpened:
            {
                m_requestInProgress = ERequestGetNextValue; // set it here and not before switch because of addInfo in state machine exception
                emit stateChanged(this,m_state,m_requestInProgress);

                CMsgReqGetNextValue* pMsgReq = new CMsgReqGetNextValue(
                    /* pObjSender   */ this,
                    /* pObjReceiver */ this,
                    /* oid          */ i_strOid );
                requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
                requestResult.m_result = EResultUndefined;
                QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
                pMsgReq = NULL;
                break;
            }
            case EStateInit:
            case EStateShutdown:
            case EStateWaitingForSessionCreated:
            case EStateWaitingForSessionOpened:
            case EStateWaitingForSessionClosed:
            case EStateWaitingForSessionDeleted:
            default:
            {
                throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
            }
        } // switch( m_state )
    } // if( m_requestInProgress == ERequestNone )

    return requestResult;

} // requestNextValue

//------------------------------------------------------------------------------
ZS::SRequestResult CExplorer::requestBulk( const QString& i_strOid )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", Oid:" + i_strOid;
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "requestBulk",
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
        CMsgReqGetBulk* pMsgReq = new CMsgReqGetBulk(
            /* pObjSender   */ this,
            /* pObjReceiver */ this,
            /* oid          */ i_strOid );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        m_ptrListMsgReqPending.append(pMsgReq);
        pMsgReq = NULL;
    }
    else
    {
        switch( m_state )
        {
            case EStateSessionClosed:
            {
                requestResult.m_result = EResultRequestRefused;
                break;
            }
            case EStateSessionOpened:
            {
                m_requestInProgress = ERequestGetBulk; // set it here and not before switch because of addInfo in state machine exception
                emit stateChanged(this,m_state,m_requestInProgress);

                CMsgReqGetBulk* pMsgReq = new CMsgReqGetBulk(
                    /* pObjSender   */ this,
                    /* pObjReceiver */ this,
                    /* oid          */ i_strOid );
                requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
                requestResult.m_result = EResultUndefined;
                QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
                pMsgReq = NULL;
                break;
            }
            case EStateInit:
            case EStateShutdown:
            case EStateWaitingForSessionCreated:
            case EStateWaitingForSessionOpened:
            case EStateWaitingForSessionClosed:
            case EStateWaitingForSessionDeleted:
            default:
            {
                throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
            }
        } // switch( m_state )
    } // if( m_requestInProgress == ERequestNone )

    return requestResult;

} // requestBulk

//------------------------------------------------------------------------------
ZS::SRequestResult CExplorer::walk( const QString& i_strOid )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", Oid:" + i_strOid;
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "walk",
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
            /* oid          */ i_strOid );
        requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
        requestResult.m_result = EResultUndefined;
        m_ptrListMsgReqPending.append(pMsgReq);
        pMsgReq = NULL;
    }
    else
    {
        switch( m_state )
        {
            case EStateSessionClosed:
            {
                requestResult.m_result = EResultRequestRefused;
                break;
            }
            case EStateSessionOpened:
            {
                // The request message will be sent also if the session is closed.
                // On receiving the request message the request executed signal will
                // be emitted with result = RequestRefused.

                m_requestInProgress = ERequestWalk; // set it here and not before switch because of addInfo in state machine exception
                emit stateChanged(this,m_state,m_requestInProgress);

                CMsgReqWalk* pMsgReq = new CMsgReqWalk(
                    /* pObjSender   */ this,
                    /* pObjReceiver */ this,
                    /* oid          */ i_strOid );
                requestResult.m_iPendingRequestId = pMsgReq->getMsgId();
                requestResult.m_result = EResultUndefined;
                QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
                pMsgReq = NULL;
                break;
            }
            case EStateInit:
            case EStateShutdown:
            case EStateWaitingForSessionCreated:
            case EStateWaitingForSessionOpened:
            case EStateWaitingForSessionClosed:
            case EStateWaitingForSessionDeleted:
            default:
            {
                throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
            }
        } // switch( m_state )
    } // if( m_requestInProgress == ERequestNone )

    return requestResult;

} // walk

//------------------------------------------------------------------------------
void CExplorer::abortRequest( int i_iRequestId )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
        strAddTrcInfo += ", RequestIdInProgress:" + QString::number(m_iIdRequestInProgress);
        strAddTrcInfo += ", RequestIdToBeAborted:" + QString::number(i_iRequestId);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "abortRequest",
        /* strAddTrcInfo */ strAddTrcInfo );

    // If the request to be aborted is currently in progress ...
    if( m_iIdRequestInProgress == i_iRequestId )
    {
        switch( m_requestInProgress )
        {
            case ERequestShutdown:
            {
                break;
            }
            case ERequestOpenSession:
            case ERequestCloseSession:
            {
                shutdown();
                break;
            }
            case ERequestGetValue:
            case ERequestGetNextValue:
            case ERequestGetBulk:
            case ERequestWalk:
            default:
            {
                m_state = EStateSessionOpened;
                m_requestInProgress = ERequestNone;
                emit stateChanged(this,m_state,m_requestInProgress);

                // The request has been executed. Check if there are pending requests ...
                if( m_ptrListMsgReqPending.count() > 0 )
                {
                    CMsgReq* pMsgReqPending = m_ptrListMsgReqPending.takeFirst();

                    if( pMsgReqPending != NULL )
                    {
                        m_requestInProgress = pendingMsgType2Request(pMsgReqPending);
                        m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                        emit stateChanged(this,m_state,m_requestInProgress);
                        QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                        pMsgReqPending = NULL;
                    }
                }
                break;
            }
        } // switch( m_requestInProgress )
    } // if( m_iIdRequestInProgress == i_iRequestId )

    // If the request to be aborted is not currently in progress ..
    else
    {
        CMsgReq* pMsgReq;
        bool     bRequestFound = false;

        // .. search the request in the list of pending requests.
        QLinkedList<CMsgReq*>::iterator it = m_ptrListMsgReqPending.begin();

        while( it != m_ptrListMsgReqPending.end() )
        {
            pMsgReq = *it;

            if( pMsgReq->getMsgId() == i_iRequestId )
            {
                it = m_ptrListMsgReqPending.erase(it);
                bRequestFound = true;
                break;
            }
            ++it;
        }
        if( !bRequestFound )
        {
            throw ZS::CException(__FILE__,__LINE__,ZS::EResultObjNotInList,strAddTrcInfo);
        }
    } // if( m_iIdRequestInProgress != i_iRequestId )

} // abortRequest

/*==============================================================================
public: // instance methods (state machine timeouts)
==============================================================================*/

//------------------------------------------------------------------------------
void CExplorer::setRequestTimeoutInMs( ERequest i_request, int i_iTimeout_ms )
//------------------------------------------------------------------------------
{
    if( i_request >= 0 && i_request < ERequestCount )
    {
        m_ariRequestTimeouts_ms[i_request] = i_iTimeout_ms;
    }
}

//------------------------------------------------------------------------------
int CExplorer::getRequestTimeoutInMs( ERequest i_request ) const
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
double CExplorer::getLastRecentRequestDurationInMs( ERequest i_request ) const
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
QString CExplorer::getSettingsKey() const
//------------------------------------------------------------------------------
{
    return m_strSettingsKey;
}

//------------------------------------------------------------------------------
int CExplorer::getReadSettingsIntervalInMs() const
//------------------------------------------------------------------------------
{
    return m_iReadSettingsInterval_ms;
}

//------------------------------------------------------------------------------
void CExplorer::setReadSettingsIntervalInMs( int i_iInterval_ms, bool i_bSettingsFileAutoReadEnabled )
//------------------------------------------------------------------------------
{
    if( m_iReadSettingsInterval_ms != i_iInterval_ms || m_bSettingsFileAutoReadEnabled != i_bSettingsFileAutoReadEnabled )
    {
        if( m_pTimerReadSettings != NULL && m_pTimerReadSettings->isActive() )
        {
            m_pTimerReadSettings->stop();
        }

        m_iReadSettingsInterval_ms = i_iInterval_ms;
        m_bSettingsFileAutoReadEnabled = i_bSettingsFileAutoReadEnabled;

        if( m_pTimerReadSettings != NULL && m_bSettingsFileAutoReadEnabled && m_iReadSettingsInterval_ms > 0 )
        {
            m_pTimerReadSettings->start(m_iReadSettingsInterval_ms);
        }
    }

} // setReadSettingsIntervalInMs

/*==============================================================================
protected slots: // continuously read the settings file
==============================================================================*/

//------------------------------------------------------------------------------
void CExplorer::onTimeoutReadSettings()
//------------------------------------------------------------------------------
{
    readHostSettings();
}

/*==============================================================================
public: // instance methods exploring the SNMP host
==============================================================================*/

//------------------------------------------------------------------------------
CExplorer::EState CExplorer::getCurrentState() const
//------------------------------------------------------------------------------
{
    return m_state;
}

//------------------------------------------------------------------------------
CExplorer::ERequest CExplorer::getRequestInProgress() const
//------------------------------------------------------------------------------
{
    return m_requestInProgress;
}

//------------------------------------------------------------------------------
bool CExplorer::isRequestInProgressOrPending() const
//------------------------------------------------------------------------------
{
    return ( (m_requestInProgress != ERequestNone) || (m_ptrListMsgReqPending.count() > 0) );
}

//------------------------------------------------------------------------------
COidNode* CExplorer::findOidNode( COid& i_oid )
//------------------------------------------------------------------------------
{
    return m_pModelOidNodes->findOidNode(i_oid);
}

//------------------------------------------------------------------------------
COidNode* CExplorer::findOidNode( const QString& i_strOid )
//------------------------------------------------------------------------------
{
    return m_pModelOidNodes->findOidNode(i_strOid);
}

//------------------------------------------------------------------------------
CInstanceNode* CExplorer::findInstanceNode( const QString& i_strOid )
//------------------------------------------------------------------------------
{
    return m_pModelOidNodes->findInstanceNode(i_strOid);
}

//------------------------------------------------------------------------------
CModelOidNodes* CExplorer::getModelOidNodes()
//------------------------------------------------------------------------------
{
    return m_pModelOidNodes;
}

//------------------------------------------------------------------------------
CModelInstanceNodes* CExplorer::getModelInstanceNodes()
//------------------------------------------------------------------------------
{
    return m_pModelInstanceNodes;
}

/*==============================================================================
protected slots: // instance methods of internal state machine
==============================================================================*/

//------------------------------------------------------------------------------
void CExplorer::onSessionTimeout()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onSessionTimeout",
        /* strAddTrcInfo */ strAddTrcInfo );

    CMsgIndTimeout* pMsgInd = new CMsgIndTimeout(
        /* pObjSender   */ this,
        /* pObjReceiver */ this );
    QCoreApplication::postEvent(pMsgInd->getReceiver(),pMsgInd);
    pMsgInd = NULL;

} // onSessionTimeout

//------------------------------------------------------------------------------
void CExplorer::onSessionCreated( QObject* /*i_pSessionThread*/, QObject* i_pSession )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
    {
        strAddTrcInfo  = "State:" + state2Str(m_state);
        strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
    }

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "onSessionCreated",
        /* strAddTrcInfo */ strAddTrcInfo );

    disconnect(
        /* pObjSender   */ m_pSessionThread,
        /* szSignal     */ SIGNAL(sessionCreated(QObject*,QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onSessionCreated(QObject*,QObject*)) );

    // If the request has not yet been aborted in the meantime ...
    if( m_state == EStateWaitingForSessionCreated && m_requestInProgress == ERequestOpenSession )
    {
        m_pSession = dynamic_cast<CSession*>(i_pSession);

        if( m_pSession == NULL )
        {
            throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"m_pSession = NULL");
        }

        m_state = EStateWaitingForSessionOpened;
        emit stateChanged(this,m_state,m_requestInProgress);

        CMsgReqOpenSession* pMsgReq = new CMsgReqOpenSession(
            /* pObjSender   */ this,
            /* pObjReceiver */ m_pSession,
            /* hostSettings */ m_hostSettings );
        m_iIdRequestWaitingFor = pMsgReq->getMsgId();
        emit sendingMessage(pMsgReq->getReceiver(),pMsgReq);
        QCoreApplication::postEvent(pMsgReq->getReceiver(),pMsgReq);
        pMsgReq = NULL;
    }

} // onSessionCreated

//------------------------------------------------------------------------------
CExplorer::ERequest CExplorer::pendingMsgType2Request( ZS::CMsgReq* i_pMsgReq )
//------------------------------------------------------------------------------
{
    if( i_pMsgReq == NULL )
    {
        throw ZS::CException(__FILE__,__LINE__,ZS::EResultArgOutOfRange,"i_pMsgReq == NULL");
    }

    ERequest request = ERequestUndefined;

    switch( i_pMsgReq->getMsgType() )
    {
        case EMsgTypeReqOpenSession:
        {
            request = ERequestOpenSession;
            break;
        }
        case EMsgTypeReqCloseSession:
        {
            request = ERequestCloseSession;
            break;
        }
        case EMsgTypeReqGetValue:
        {
            request = ERequestGetValue;
            break;
        }
        case EMsgTypeReqGetNextValue:
        {
            request = ERequestGetNextValue;
            break;
        }
        case EMsgTypeReqGetBulk:
        {
            request = ERequestGetBulk;
            break;
        }
        case EMsgTypeReqWalk:
        {
            request = ERequestWalk;
            break;
        }
        case EMsgTypeConOpenSession:
        case EMsgTypeConCloseSession:
        case EMsgTypeConResponse:
        case EMsgTypeIndTimeout:
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
bool CExplorer::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    ZS::CMsg* pMsg = dynamic_cast<ZS::CMsg*>(i_pEv);

    if( pMsg != NULL )
    {
        bEventHandled = true;

        QString strAddTrcInfo;

        if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
        {
            strAddTrcInfo  = "State:" + state2Str(m_state);
            strAddTrcInfo += ", RequestInProgress:" + request2Str(m_requestInProgress);
            strAddTrcInfo += ", Event:" + pMsg->msgTypeToStr();
        }

        M_TRACE_INST_METHOD(
            /* pAdminObj     */ m_pTrcAdminObj,
            /* iDetailLevel  */ 0,
            /* strMethod     */ "event",
            /* strAddTrcInfo */ strAddTrcInfo );

        switch( pMsg->type() )
        {
            //------------------------------------------------------------------
            case EMsgTypeReqOpenSession:
            //------------------------------------------------------------------
            {
                CMsgReqOpenSession* pMsgReq = dynamic_cast<CMsgReqOpenSession*>(pMsg);
                if( pMsgReq == NULL )
                {
                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                }

                switch( m_state )
                {
                    case EStateSessionClosed:
                    {
                        if( m_hostSettings != pMsgReq->getHostSettings() )
                        {
                            m_hostSettings = pMsgReq->getHostSettings();
                            saveHostSettings();
                            emit hostSettingsChanged(this);
                        }

                        m_state = EStateWaitingForSessionCreated;
                        m_requestInProgress = ERequestOpenSession;
                        m_iIdRequestInProgress = pMsgReq->getMsgId();
                        m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                        emit stateChanged(this,m_state,m_requestInProgress);

                        m_pSessionThread = new CSessionThread(
                            /* szObjNameSession */ m_strObjNameSession,
                            /* pExplorer        */ this,
                            /* pModelErrLog     */ m_pModelErrLog );

                        if( !connect(
                            /* pObjSender   */ m_pSessionThread,
                            /* szSignal     */ SIGNAL(sessionCreated(QObject*,QObject*)),
                            /* pObjReceiver */ this,
                            /* szSlot       */ SLOT(onSessionCreated(QObject*,QObject*)) ) )
                        {
                            throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
                        }
                        if( m_ariRequestTimeouts_ms[m_requestInProgress] > 0 )
                        {
                            m_pTimerSession->start(m_ariRequestTimeouts_ms[m_requestInProgress]);
                        }
                        m_pSessionThread->start();
                        break;
                    }
                    case EStateSessionOpened:
                    {
                        if( m_hostSettings != pMsgReq->getHostSettings() )
                        {
                            m_hostSettings = pMsgReq->getHostSettings();
                            saveHostSettings();
                            emit hostSettingsChanged(this);

                            m_state = EStateWaitingForSessionClosed;
                            m_requestInProgress = ERequestOpenSession;
                            m_iIdRequestInProgress = pMsgReq->getMsgId();
                            m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                            emit stateChanged(this,m_state,m_requestInProgress);

                            if( m_ariRequestTimeouts_ms[m_requestInProgress] > 0 )
                            {
                                m_pTimerSession->start(m_ariRequestTimeouts_ms[m_requestInProgress]);
                            }

                            CMsgReqCloseSession* pMsgReqSession = new CMsgReqCloseSession(
                                /* pObjSender   */ this,
                                /* pObjReceiver */ m_pSession );
                            m_iIdRequestWaitingFor = pMsgReqSession->getMsgId();
                            emit sendingMessage(pMsgReqSession->getReceiver(),pMsgReqSession);
                            QCoreApplication::postEvent(pMsgReqSession->getReceiver(),pMsgReqSession);
                            pMsgReqSession = NULL;
                        }
                        else
                        {
                            m_state = EStateSessionOpened;
                            m_requestInProgress = ERequestNone;
                            emit stateChanged(this,m_state,m_requestInProgress);
                            emit requestExecuted(this,pMsgReq->getMsgId(),SErrResultInfo());

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
                                    m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                                    emit stateChanged(this,m_state,m_requestInProgress);
                                    QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                                    pMsgReqPending = NULL;
                                }
                            }
                        }
                        break;
                    }
                    case EStateInit:
                    case EStateWaitingForSessionCreated:
                    case EStateWaitingForSessionOpened:
                    case EStateWaitingForSessionClosed:
                    case EStateWaitingForSessionDeleted:
                    default:
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
                    }
                } // switch( m_state )
                break;
            } // case EMsgTypeReqOpenSession

            //------------------------------------------------------------------
            case EMsgTypeConOpenSession:
            //------------------------------------------------------------------
            {
                CMsgConOpenSession* pMsgCon = dynamic_cast<CMsgConOpenSession*>(pMsg);
                if( pMsgCon == NULL )
                {
                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                }
                emit receivedMessage(this,pMsgCon);

                // Did we receive the confirmation message to our request message
                // (and if the request has not been aborted in the meantime) ..
                if( m_iIdRequestWaitingFor == pMsgCon->getMsgId() )
                {
                    EResult result = pMsgCon->getResult();

                    switch( m_state )
                    {
                        case EStateWaitingForSessionOpened:
                        {
                            if( m_pTimerSession->isActive() )
                            {
                                m_pTimerSession->stop();
                            }

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

                                if( m_pModelErrLog != NULL && m_bErrLogModelOutputEnabled && !pMsgCon->getErrResultInfo().isAddedToErrLogModel() )
                                {
                                    m_pModelErrLog->addEntry(
                                        /* dateTime    */ QDateTime::currentDateTime(),
                                        /* severity    */ pMsgCon->getErrResult().getSeverity(),
                                        /* iErrNr      */ pMsgCon->getErrResult().getResult(),
                                        /* strErrNr    */ pMsgCon->getErrResult().getResultStr(),
                                        /* strSource   */ pMsgCon->getErrSource(),
                                        /* strAddTrcInfo  */ pMsgCon->getAddErrInfoDscr(),
                                        /* strProposal */ strProposal );
                                }

                                m_pSessionThread->quit();
                                m_pSessionThread->wait(30000);

                                try
                                {
                                    delete m_pSessionThread;
                                }
                                catch(...)
                                {
                                }
                                m_pSessionThread = NULL;
                                m_pSession = NULL;

                                m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                                m_state = EStateSessionClosed;
                                m_requestInProgress = ERequestNone;
                                emit stateChanged(this,m_state,m_requestInProgress);
                                emit requestExecuted(this,m_iIdRequestInProgress,pMsgCon->getErrResultInfo());
                            }
                            else
                            {
                                m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                                m_state = EStateSessionOpened;
                                m_requestInProgress = ERequestNone;
                                emit stateChanged(this,m_state,m_requestInProgress);
                                emit requestExecuted(this,m_iIdRequestInProgress,pMsgCon->getErrResultInfo());
                            }

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
                                    m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                                    emit stateChanged(this,m_state,m_requestInProgress);
                                    QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                                    pMsgReqPending = NULL;
                                }
                            }
                            break;
                        }
                        case EStateInit:
                        case EStateShutdown:
                        case EStateSessionClosed:
                        case EStateSessionOpened:
                        case EStateWaitingForSessionCreated:
                        case EStateWaitingForSessionClosed:
                        case EStateWaitingForSessionDeleted:
                        default:
                        {
                            throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
                        }
                    } // switch( m_state )
                }
                break;
            } // case EMsgTypeConOpenSession

            //------------------------------------------------------------------
            case EMsgTypeReqCloseSession:
            //------------------------------------------------------------------
            {
                CMsgReqCloseSession* pMsgReq = dynamic_cast<CMsgReqCloseSession*>(pMsg);
                if( pMsgReq == NULL )
                {
                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                }

                switch( m_state )
                {
                    case EStateSessionClosed:
                    {
                        if( m_pTimerSession->isActive() )
                        {
                            m_pTimerSession->stop();
                        }
                        m_requestInProgress = ERequestNone;
                        emit requestExecuted(this,pMsgReq->getMsgId(),SErrResultInfo());

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
                                m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                                emit stateChanged(this,m_state,m_requestInProgress);
                                QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                                pMsgReqPending = NULL;
                            }
                        }
                        break;
                    }
                    case EStateSessionOpened:
                    {
                        m_state = EStateWaitingForSessionClosed;
                        m_requestInProgress = ERequestCloseSession;
                        m_iIdRequestInProgress = pMsgReq->getMsgId();
                        m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                        emit stateChanged(this,m_state,m_requestInProgress);

                        if( m_ariRequestTimeouts_ms[m_requestInProgress] > 0 )
                        {
                            m_pTimerSession->start(m_ariRequestTimeouts_ms[m_requestInProgress]);
                        }

                        CMsgReqCloseSession* pMsgReqSession = new CMsgReqCloseSession(
                            /* pObjSender   */ this,
                            /* pObjReceiver */ m_pSession );
                        m_iIdRequestWaitingFor = pMsgReqSession->getMsgId();
                        emit sendingMessage(pMsgReqSession->getReceiver(),pMsgReqSession);
                        QCoreApplication::postEvent(pMsgReqSession->getReceiver(),pMsgReqSession);
                        pMsgReqSession = NULL;
                        break;
                    }
                    case EStateInit:
                    case EStateShutdown:
                    case EStateWaitingForSessionCreated:
                    case EStateWaitingForSessionOpened:
                    case EStateWaitingForSessionClosed:
                    case EStateWaitingForSessionDeleted:
                    default:
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
                    }
                } // switch( m_state )
                break;
            } // case EMsgTypeReqCloseSession

            //------------------------------------------------------------------
            case EMsgTypeConCloseSession:
            //------------------------------------------------------------------
            {
                CMsgConCloseSession* pMsgCon = dynamic_cast<CMsgConCloseSession*>(pMsg);
                if( pMsgCon == NULL )
                {
                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                }
                emit receivedMessage(this,pMsgCon);

                // If we received the confirmation message to our request message
                // (and if the request has not been aborted in the meantime) ..
                if( m_iIdRequestWaitingFor == pMsgCon->getMsgId() )
                {
                    switch( m_state )
                    {
                        case EStateWaitingForSessionClosed:
                        {
                            if( m_pTimerSession->isActive() )
                            {
                                m_pTimerSession->stop();
                            }

                            if( pMsgCon->getResult() != EResultSuccess )
                            {
                                if( m_pModelErrLog != NULL && m_bErrLogModelOutputEnabled && !pMsgCon->getErrResultInfo().isAddedToErrLogModel() )
                                {
                                    m_pModelErrLog->addEntry(
                                        /* dateTime    */ QDateTime::currentDateTime(),
                                        /* severity    */ pMsgCon->getErrResult().getSeverity(),
                                        /* iErrNr      */ pMsgCon->getErrResult().getResult(),
                                        /* strErrNr    */ pMsgCon->getErrResult().getResultStr(),
                                        /* strSource   */ pMsgCon->getErrSource(),
                                        /* strAddTrcInfo  */ pMsgCon->getAddErrInfoDscr(),
                                        /* strProposal */ "" );
                                }
                            }

                            m_pSessionThread->quit();
                            m_pSessionThread->wait(30000);

                            try
                            {
                                delete m_pSessionThread;
                            }
                            catch(...)
                            {
                            }
                            m_pSessionThread = NULL;
                            m_pSession = NULL;

                            if( m_requestInProgress == ERequestOpenSession )
                            {
                                m_state = EStateWaitingForSessionCreated;
                                emit stateChanged(this,m_state,m_requestInProgress);

                                m_pSessionThread = new CSessionThread(
                                    /* szObjNameSession */ m_strObjNameSession,
                                    /* pExplorer        */ this,
                                    /* pModelErrLog     */ m_pModelErrLog );

                                if( !connect(
                                    /* pObjSender   */ m_pSessionThread,
                                    /* szSignal     */ SIGNAL(sessionCreated(QObject*,QObject*)),
                                    /* pObjReceiver */ this,
                                    /* szSlot       */ SLOT(onSessionCreated(QObject*,QObject*)) ) )
                                {
                                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultSignalSlotConnectionFailed);
                                }
                                if( m_ariRequestTimeouts_ms[m_requestInProgress] > 0 )
                                {
                                    m_pTimerSession->start(m_ariRequestTimeouts_ms[m_requestInProgress]);
                                }
                                m_pSessionThread->start();
                            }
                            else
                            {
                                m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                                m_state = EStateSessionClosed;
                                m_requestInProgress = ERequestNone;
                                emit stateChanged(this,m_state,m_requestInProgress);
                                emit requestExecuted(this,m_iIdRequestInProgress,pMsgCon->getErrResultInfo());

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
                                        m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                                        emit stateChanged(this,m_state,m_requestInProgress);
                                        QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                                        pMsgReqPending = NULL;
                                    }
                                }
                            }
                            break;
                        }
                        case EStateInit:
                        case EStateSessionClosed:
                        case EStateSessionOpened:
                        case EStateShutdown:
                        case EStateWaitingForSessionCreated:
                        case EStateWaitingForSessionOpened:
                        case EStateWaitingForSessionDeleted:
                        default:
                        {
                            throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
                        }
                    } // switch( m_state )
                }
                break;
            } // case EMsgTypeConCloseSession

            //------------------------------------------------------------------
            case EMsgTypeReqGetValue:
            //------------------------------------------------------------------
            {
                CMsgReqGetValue* pMsgReq = dynamic_cast<CMsgReqGetValue*>(pMsg);
                if( pMsgReq == NULL )
                {
                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                }

                switch( m_state )
                {
                    case EStateSessionClosed:
                    {
                        if( m_pTimerSession->isActive() )
                        {
                            m_pTimerSession->stop();
                        }

                        SErrResultInfo errResultInfo;
                        QString        strAddTrcInfo;
                        COid*          pOid = pMsgReq->getOid(0,false);

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
                        emit stateChanged(this,m_state,m_requestInProgress);
                        emit requestExecuted(this,pMsgReq->getMsgId(),errResultInfo);

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
                                m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                                emit stateChanged(this,m_state,m_requestInProgress);
                                QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                                pMsgReqPending = NULL;
                            }
                        }
                        break;
                    }
                    case EStateSessionOpened:
                    {
                        m_requestInProgress = ERequestGetValue;
                        m_iIdRequestInProgress = pMsgReq->getMsgId();
                        m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                        emit stateChanged(this,m_state,m_requestInProgress);

                        if( m_ariRequestTimeouts_ms[m_requestInProgress] > 0 )
                        {
                            m_pTimerSession->start(m_ariRequestTimeouts_ms[m_requestInProgress]);
                        }

                        CMsgReqGetValue* pMsgReqSession = new CMsgReqGetValue(
                            /* pObjSender   */ pMsgReq->getSender(),
                            /* pObjReceiver */ m_pSession,
                            /* oidPtrList   */ pMsgReq->getOidPtrList() );
                        m_iIdRequestWaitingFor = pMsgReqSession->getMsgId();
                        emit sendingMessage(pMsgReqSession->getReceiver(),pMsgReqSession);
                        QCoreApplication::postEvent(pMsgReqSession->getReceiver(),pMsgReqSession);
                        pMsgReqSession = NULL;
                        break;
                    }
                    case EStateInit:
                    case EStateShutdown:
                    case EStateWaitingForSessionCreated:
                    case EStateWaitingForSessionOpened:
                    case EStateWaitingForSessionClosed:
                    case EStateWaitingForSessionDeleted:
                    default:
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
                    }
                } // switch( m_state )
                break;
            } // case EMsgTypeReqGetValue

            //------------------------------------------------------------------
            case EMsgTypeReqGetNextValue:
            //------------------------------------------------------------------
            {
                CMsgReqGetNextValue* pMsgReq = dynamic_cast<CMsgReqGetNextValue*>(pMsg);
                if( pMsgReq == NULL )
                {
                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                }

                switch( m_state )
                {
                    case EStateSessionClosed:
                    {
                        if( m_pTimerSession->isActive() )
                        {
                            m_pTimerSession->stop();
                        }

                        SErrResultInfo errResultInfo;
                        QString        strAddTrcInfo;
                        COid*          pOid = pMsgReq->getOid(0,false);

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
                        emit stateChanged(this,m_state,m_requestInProgress);
                        emit requestExecuted(this,pMsgReq->getMsgId(),errResultInfo);

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
                                m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                                emit stateChanged(this,m_state,m_requestInProgress);
                                QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                                pMsgReqPending = NULL;
                            }
                        }
                        break;
                    }
                    case EStateSessionOpened:
                    {
                        m_requestInProgress = ERequestGetNextValue;
                        m_iIdRequestInProgress = pMsgReq->getMsgId();
                        m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                        emit stateChanged(this,m_state,m_requestInProgress);

                        if( m_ariRequestTimeouts_ms[m_requestInProgress] > 0 )
                        {
                            m_pTimerSession->start(m_ariRequestTimeouts_ms[m_requestInProgress]);
                        }

                        CMsgReqGetNextValue* pMsgReqSession = new CMsgReqGetNextValue(
                            /* pObjSender   */ pMsgReq->getSender(),
                            /* pObjReceiver */ m_pSession,
                            /* oidPtrList   */ pMsgReq->getOidPtrList() );
                        m_iIdRequestWaitingFor = pMsgReqSession->getMsgId();
                        emit sendingMessage(pMsgReqSession->getReceiver(),pMsgReqSession);
                        QCoreApplication::postEvent(pMsgReqSession->getReceiver(),pMsgReqSession);
                        pMsgReqSession = NULL;
                        break;
                    }
                    case EStateInit:
                    case EStateShutdown:
                    case EStateWaitingForSessionCreated:
                    case EStateWaitingForSessionOpened:
                    case EStateWaitingForSessionClosed:
                    case EStateWaitingForSessionDeleted:
                    default:
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
                    }
                } // switch( m_state )
                break;
            } // case EMsgTypeReqGetNextValue

            //------------------------------------------------------------------
            case EMsgTypeReqGetBulk:
            //------------------------------------------------------------------
            {
                CMsgReqGetBulk* pMsgReq = dynamic_cast<CMsgReqGetBulk*>(pMsg);
                if( pMsgReq == NULL )
                {
                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                }

                switch( m_state )
                {
                    case EStateSessionClosed:
                    {
                        if( m_pTimerSession->isActive() )
                        {
                            m_pTimerSession->stop();
                        }

                        SErrResultInfo errResultInfo;
                        QString        strAddTrcInfo;
                        COid*          pOid = pMsgReq->getOid(0,false);

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
                        emit stateChanged(this,m_state,m_requestInProgress);
                        emit requestExecuted(this,pMsgReq->getMsgId(),errResultInfo);

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
                                m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                                emit stateChanged(this,m_state,m_requestInProgress);
                                QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                                pMsgReqPending = NULL;
                            }
                        }
                        break;
                    }
                    case EStateSessionOpened:
                    {
                        m_requestInProgress = ERequestGetBulk;
                        m_iIdRequestInProgress = pMsgReq->getMsgId();
                        m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                        emit stateChanged(this,m_state,m_requestInProgress);

                        if( m_ariRequestTimeouts_ms[m_requestInProgress] > 0 )
                        {
                            m_pTimerSession->start(m_ariRequestTimeouts_ms[m_requestInProgress]);
                        }

                        // The nodes below the requested oid must be removed as we don't know
                        // whether the instances are still exist ..

                        COid*     pOid;
                        COidNode* pOidNode;
                        int       idxOid;

                        for( idxOid = 0; idxOid < pMsgReq->getOidPtrList(false).count(); idxOid++ )
                        {
                            pOid = pMsgReq->getOid(idxOid,false);

                            if( pOid != NULL )
                            {
                                pOidNode = m_pModelOidNodes->findOidNode(*pOid);

                                if( pOidNode != NULL )
                                {
                                    m_pModelOidNodes->clear(pOidNode);
                                }
                            }
                        }

                        CMsgReqGetBulk* pMsgReqSession = new CMsgReqGetBulk(
                            /* pObjSender   */ pMsgReq->getSender(),
                            /* pObjReceiver */ m_pSession,
                            /* oidPtrList   */ pMsgReq->getOidPtrList() );
                        m_iIdRequestWaitingFor = pMsgReqSession->getMsgId();
                        emit sendingMessage(pMsgReqSession->getReceiver(),pMsgReqSession);
                        QCoreApplication::postEvent(pMsgReqSession->getReceiver(),pMsgReqSession);
                        pMsgReqSession = NULL;
                        break;
                    }
                    case EStateInit:
                    case EStateShutdown:
                    case EStateWaitingForSessionCreated:
                    case EStateWaitingForSessionOpened:
                    case EStateWaitingForSessionClosed:
                    case EStateWaitingForSessionDeleted:
                    default:
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
                    }
                } // switch( m_state )
                break;
            } // case EMsgTypeReqGetBulk

            //------------------------------------------------------------------
            case EMsgTypeReqWalk:
            //------------------------------------------------------------------
            {
                CMsgReqWalk* pMsgReq = dynamic_cast<CMsgReqWalk*>(pMsg);
                if( pMsgReq == NULL )
                {
                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                }

                switch( m_state )
                {
                    case EStateSessionClosed:
                    {
                        if( m_pTimerSession->isActive() )
                        {
                            m_pTimerSession->stop();
                        }

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
                        emit stateChanged(this,m_state,m_requestInProgress);
                        emit requestExecuted(this,pMsgReq->getMsgId(),errResultInfo);

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
                                m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                                emit stateChanged(this,m_state,m_requestInProgress);
                                QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                                pMsgReqPending = NULL;
                            }
                        }
                        break;
                    }
                    case EStateSessionOpened:
                    {
                        if( pMsgReq->getOid(false) != NULL )
                        {
                            m_requestInProgress = ERequestWalk;
                            m_iIdRequestInProgress = pMsgReq->getMsgId();
                            m_fRequestStartTime_ms = ZS::System::Time::getProcTimeInMilliSec();
                            if( pMsgReq->getOid(false) != NULL )
                            {
                                delete m_pOidRootWalkRequest;
                                m_pOidRootWalkRequest = new COid(*pMsgReq->getOid(false));
                            }
                            emit stateChanged(this,m_state,m_requestInProgress);

                            if( m_ariRequestTimeouts_ms[m_requestInProgress] > 0 )
                            {
                                m_pTimerSession->start(m_ariRequestTimeouts_ms[m_requestInProgress]);
                            }

                            // The nodes below the requested oid must be removed as we don't know
                            // whether the instances are still exist ..
                            COidNode* pOidNode = m_pModelOidNodes->findOidNode(*m_pOidRootWalkRequest);
                            if( pOidNode != NULL )
                            {
                                m_pModelOidNodes->clear(pOidNode);
                            }

                            CMsgReqGetNextValue* pMsgReqSession = new CMsgReqGetNextValue(
                                /* pObjSender   */ this,
                                /* pObjReceiver */ m_pSession,
                                /* oid          */ *pMsgReq->getOid(false) );
                            m_iIdRequestWaitingFor = pMsgReqSession->getMsgId();
                            emit sendingMessage(pMsgReqSession->getReceiver(),pMsgReqSession);
                            QCoreApplication::postEvent(pMsgReqSession->getReceiver(),pMsgReqSession);
                            pMsgReqSession = NULL;
                        }
                        else
                        {
                            if( m_pTimerSession->isActive() )
                            {
                                m_pTimerSession->stop();
                            }

                            m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                            m_requestInProgress = ERequestNone;
                            emit stateChanged(this,m_state,m_requestInProgress);
                            emit requestExecuted(this,pMsgReq->getMsgId(),SErrResultInfo());

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
                                    m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                                    emit stateChanged(this,m_state,m_requestInProgress);
                                    QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                                    pMsgReqPending = NULL;
                                }
                            }
                        }
                        break;
                    }
                    case EStateInit:
                    case EStateShutdown:
                    case EStateWaitingForSessionCreated:
                    case EStateWaitingForSessionOpened:
                    case EStateWaitingForSessionClosed:
                    case EStateWaitingForSessionDeleted:
                    default:
                    {
                        throw ZS::CException(__FILE__,__LINE__,ZS::EResultInternalStateMachineError,strAddTrcInfo);
                    }
                } // switch( m_state )
                break;
            } // case EMsgTypeReqWalk

            //------------------------------------------------------------------
            case EMsgTypeConResponse:
            //------------------------------------------------------------------
            {
                CMsgConResponse* pMsgCon = dynamic_cast<CMsgConResponse*>(pMsg);
                if( pMsgCon == NULL )
                {
                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                }
                emit receivedMessage(this,pMsgCon);

                // This confirmation message may be received even if the corresponding
                // get request has been aborted or timed out.

                // Did we receive the confirmation message to our request message
                // (and if the request has not been aborted in the meantime) ..
                if( m_iIdRequestWaitingFor == pMsgCon->getMsgId() )
                {
                    // First step: take over the received values into the models (on success)
                    //-----------------------------------------------------------------------

                    EResult        result = pMsgCon->getResult();
                    CValue*        pValueLast = NULL;
                    bool           bWalkRequestExecuted = false;
                    int            idxValue;
                    int            idxSubId;
                    CValue*        pValue = NULL;
                    COid*          pOid = NULL;
                    COidNode*      pOidNodeParent = NULL;
                    COidNode*      pOidNode = NULL;
                    CInstanceNode* pInstanceNode = NULL;

                    if( result != EResultSuccess )
                    {
                        // The result "EndOfList" is not an error in case of an walk request but
                        // indicates that the walk request has been successfully executed.
                        if( !(m_requestInProgress == ERequestWalk && result == EResultEndOfList) )
                        {
                            QString strProposal;

                            switch( result )
                            {
                                case EResultRequestRefused:
                                {
                                    strProposal = "Check Host Settings (IP address, community string, etc.)";
                                    break;
                                }
                                case EResultObjNotInList:
                                {
                                    strProposal = "Check OID";
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                            }

                            if( m_pModelErrLog != NULL && m_bErrLogModelOutputEnabled && !pMsgCon->getErrResultInfo().isAddedToErrLogModel() )
                            {
                                m_pModelErrLog->addEntry(
                                    /* dateTime    */ QDateTime::currentDateTime(),
                                    /* severity    */ pMsgCon->getErrResult().getSeverity(),
                                    /* iErrNr      */ pMsgCon->getErrResult().getResult(),
                                    /* strErrNr    */ pMsgCon->getErrResult().getResultStr(),
                                    /* strSource   */ pMsgCon->getErrSource(),
                                    /* strAddInfo  */ pMsgCon->getAddErrInfoDscr(),
                                    /* strProposal */ strProposal );
                            }

                            // We don't know for which OID our request has failed. We just know that
                            // one of the requested values is not valid. To ensure that the model is
                            // up to date we are going to remove each node for which values have
                            // be requested.
                            if( m_requestInProgress == ERequestGetValue )
                            {
                                for( idxValue = 0; idxValue < pMsgCon->getValueCount(); idxValue++ )
                                {
                                    if( pMsgCon->getValue(idxValue,false) != NULL )
                                    {
                                        pValue = pMsgCon->getValue(idxValue);
                                        pInstanceNode = NULL;

                                        if( pValue->isInstanceNodeValueType() )
                                        {
                                            pOid = pValue->getOid();

                                            if( pOid != NULL && pOid->getSubIdsCount() > 1 )
                                            {
                                                pOidNodeParent = NULL;

                                                for( idxSubId = 0; idxSubId < pOid->getSubIdsCount()-1; idxSubId ++ )
                                                {
                                                    pOidNode = m_pModelOidNodes->findOidNode( pOidNodeParent, pOid->getSubId(idxSubId) );

                                                    if( pOidNode == NULL )
                                                    {
                                                        pOidNode = m_pModelOidNodes->addOidNode( pOidNodeParent, pOid->getSubId(idxSubId) );
                                                    }
                                                    pOidNodeParent = pOidNode;
                                                }
                                                m_pModelOidNodes->clear(pOidNodeParent);
                                            }
                                        }
                                    } // if( pMsgCon->getValue(idxValue,false) != NULL )
                                } // for( idxValue < pMsgCon->getValueCount() )
                            } // if( m_requestInProgress == ERequestGetValue )
                        } // if( !(m_requestInProgress == ERequestWalk && result == EResultEndOfList) )
                    }
                    else // if( result == EResultSuccess )
                    {
                        // In case a WALK request is in progress ...
                        if( m_requestInProgress == ERequestWalk )
                        {
                            // .. we need to keep the last (available) value to determine later on
                            // whether the WALK request has been executed.
                            if( pMsgCon->getValueCount() > 0 )
                            {
                                for( idxValue = pMsgCon->getValueCount()-1; idxValue >= 0; idxValue-- )
                                {
                                    if( pMsgCon->getValue(idxValue,false) != NULL )
                                    {
                                        pValueLast = new CValue(*pMsgCon->getValue(idxValue,false));
                                        break;
                                    }
                                }
                            }
                        }

                        for( idxValue = 0; idxValue < pMsgCon->getValueCount(); idxValue++ )
                        {
                            if( pMsgCon->getValue(idxValue,false) != NULL )
                            {
                                pValue = pMsgCon->getValue(idxValue);
                                pInstanceNode = NULL;

                                if( pValue->isInstanceNodeValueType() )
                                {
                                    pOid = pValue->getOid();

                                    if( pOid != NULL && pOid->getSubIdsCount() > 1 )
                                    {
                                        pOidNodeParent = NULL;

                                        for( idxSubId = 0; idxSubId < pOid->getSubIdsCount()-1; idxSubId ++ )
                                        {
                                            pOidNode = m_pModelOidNodes->findOidNode( pOidNodeParent, pOid->getSubId(idxSubId) );

                                            if( pOidNode == NULL )
                                            {
                                                pOidNode = m_pModelOidNodes->addOidNode( pOidNodeParent, pOid->getSubId(idxSubId) );
                                            }
                                            pOidNodeParent = pOidNode;
                                        }
                                    }
                                    if( pOid->getSubIdsCount() > 0 )
                                    {
                                        idxSubId = pOid->getSubIdsCount()-1;
                                        pInstanceNode = m_pModelOidNodes->findInstanceNode( pOidNodeParent, pOid->getSubId(idxSubId) );

                                        if( pInstanceNode == NULL )
                                        {
                                            pInstanceNode = m_pModelOidNodes->addInstanceNode( pOidNodeParent, pOid->getSubId(idxSubId) );
                                        }
                                    }
                                }

                                if( pInstanceNode != NULL && pValue != NULL )
                                {
                                    pInstanceNode->setValue(pValue);
                                    pValue = NULL;
                                }

                                delete pValue;
                                pValue = NULL;

                            } // if( pMsgCon->getValue(udxValue,false) != NULL )

                        } // for( idxValue < pMsgCon->getValueCount() )
                    } // if( result == EResultSuccess )

                    // Second step: check if all requested values have been received or if further requests are necessary
                    //---------------------------------------------------------------------------------------------------

                    switch( m_requestInProgress )
                    {
                        case ERequestGetValue:
                        case ERequestGetNextValue:
                        case ERequestGetBulk:
                        {
                            if( m_pTimerSession->isActive() )
                            {
                                m_pTimerSession->stop();
                            }

                            m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                            m_requestInProgress = ERequestNone;
                            emit stateChanged(this,m_state,m_requestInProgress);
                            emit requestExecuted(this,m_iIdRequestInProgress,pMsgCon->getErrResultInfo());

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
                                    m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                                    emit stateChanged(this,m_state,m_requestInProgress);
                                    QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                                    pMsgReqPending = NULL;
                                }
                            }
                            break;
                        }
                        case ERequestWalk:
                        {
                            // First we need to check whether all requested values have been received.
                            // On starting the WALK request a parent oid has been defined for which
                            // all child values should be queried from the SNMP agent. As the bulk
                            // request allways returns "max_repetitions" values (unless EndOfMibView
                            // has been reached) also values for sibling or even parent oid nodes
                            // of the starting oid node may have been received. Even if not requested
                            // those siblings or parents have been stored within the models (which is
                            // not really bad or a failure but maybe just unsuspected by the user who
                            // initiated the walk request).

                            // To allow checking for the last required value the last received
                            // value has not yet been deleted (see comment above) ..
                            if( pValueLast == NULL )
                            {
                                // In this case no value at all has been received as the response to
                                // our BULK request and we terminate the WALK request as we would not
                                // receive further values on sending the next BULK request message
                                // using the same parent oid as used with our previous BULK request.
                                bWalkRequestExecuted = true;
                            }
                            else if( pValueLast->getOid() == NULL || pValueLast->getValueType() == EValueTypeEndOfMibView )
                            {
                                bWalkRequestExecuted = true;
                            }
                            else if( m_pOidRootWalkRequest != NULL && pValueLast != NULL && pValueLast->getOid() != NULL )
                            {
                                // The WALK request has been executed on receiving either a sibling or a parent oid
                                // of the oid that has been specified as the starting point ..
                                if( !pValueLast->getOid()->isChildOf(m_pOidRootWalkRequest) )
                                {
                                    bWalkRequestExecuted = true;
                                }
                                // If the WALK request has NOT been executed ..
                                else
                                {
                                    CMsgReqGetNextValue* pMsgReqSession = new CMsgReqGetNextValue(
                                        /* pObjSender   */ this,
                                        /* pObjReceiver */ m_pSession,
                                        /* oid          */ *pValueLast->getOid() );
                                    m_iIdRequestWaitingFor = pMsgReqSession->getMsgId();
                                    emit sendingMessage(pMsgReqSession->getReceiver(),pMsgReqSession);
                                    QCoreApplication::postEvent(pMsgReqSession->getReceiver(),pMsgReqSession);
                                    pMsgReqSession = NULL;
                                }
                            }

                            // The last received value has not been deleted (see comments above) ..
                            delete pValueLast;
                            pValueLast = NULL;

                            // If the WALK request has been completely executed ..
                            if( bWalkRequestExecuted )
                            {
                                if( m_pTimerSession->isActive() )
                                {
                                    m_pTimerSession->stop();
                                }

                                m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                                delete m_pOidRootWalkRequest;
                                m_pOidRootWalkRequest = NULL;
                                m_requestInProgress = ERequestNone;
                                emit stateChanged(this,m_state,m_requestInProgress);
                                emit requestExecuted(this,m_iIdRequestInProgress,pMsgCon->getErrResultInfo());

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
                                        m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                                        emit stateChanged(this,m_state,m_requestInProgress);
                                        QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                                        pMsgReqPending = NULL;
                                    }
                                }
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                }
                break;
            } // case EMsgTypeConResponse

            //------------------------------------------------------------------
            case EMsgTypeIndTimeout:
            //------------------------------------------------------------------
            {
                CMsgIndTimeout* pMsgInd = dynamic_cast<CMsgIndTimeout*>(pMsg);
                if( pMsgInd == NULL )
                {
                    throw ZS::CException(__FILE__,__LINE__,ZS::EResultInvalidMessageContent,strAddTrcInfo);
                }
                emit receivedMessage(this,pMsgInd);

                switch( m_state )
                {
                    case EStateWaitingForSessionCreated:
                    case EStateWaitingForSessionOpened:
                    case EStateWaitingForSessionClosed:
                    case EStateWaitingForSessionDeleted:
                    {
                        SErrResultInfo errResultInfo;

                        errResultInfo.setSeverity(ZS::EResultSeverityError);
                        errResultInfo.setResult(ZS::EResultTimeout);
                        errResultInfo.setResultStr(ZS::result2Str(ZS::EResultTimeout));
                        errResultInfo.setErrSource(m_strObjName);
                        errResultInfo.setAddErrInfoDscr(strAddTrcInfo);

                        if( m_pModelErrLog != NULL && m_bErrLogModelOutputEnabled )
                        {
                            m_pModelErrLog->addEntry(
                                /* dateTime    */ QDateTime::currentDateTime(),
                                /* severity    */ errResultInfo.getSeverity(),
                                /* iErrNr      */ errResultInfo.getResult(),
                                /* strErrNr    */ errResultInfo.getResultStr(),
                                /* strSource   */ errResultInfo.getErrSource(),
                                /* strAddInfo  */ errResultInfo.getAddErrInfoDscr(),
                                /* strProposal */ "" );
                        }

                        disconnect(
                            /* pObjSender   */ m_pSessionThread,
                            /* szSignal     */ SIGNAL(sessionCreated(QObject*,QObject*)),
                            /* pObjReceiver */ this,
                            /* szSlot       */ SLOT(onSessionCreated(QObject*,QObject*)) );

                        if( m_pSessionThread != NULL )
                        {
                            if( m_pSessionThread->isRunning() )
                            {
                                m_pSessionThread->quit();
                                m_pSessionThread->wait(10000);
                            }
                            if( m_pSessionThread->isRunning() )
                            {
                                m_pSessionThread->deleteLater();
                            }
                            else
                            {
                                try
                                {
                                    delete m_pSessionThread;
                                }
                                catch(...)
                                {
                                }
                            }
                        }
                        m_pSessionThread = NULL;
                        m_pSession = NULL;

                        m_arfRequestDurationLastRecent_ms[m_requestInProgress] = ZS::System::Time::getProcTimeInMilliSec() - m_fRequestStartTime_ms;
                        m_state = EStateSessionClosed;
                        m_requestInProgress = ERequestNone;
                        emit stateChanged(this,m_state,m_requestInProgress);
                        emit requestExecuted(this,m_iIdRequestInProgress,errResultInfo);

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
                                m_iIdRequestInProgress = pMsgReqPending->getMsgId();
                                emit stateChanged(this,m_state,m_requestInProgress);
                                QCoreApplication::postEvent(pMsgReqPending->getReceiver(),pMsgReqPending);
                                pMsgReqPending = NULL;
                            }
                        }
                        break;
                    }
                    case EStateInit:
                    case EStateShutdown:
                    case EStateSessionClosed:
                    case EStateSessionOpened:
                    default:
                    {
                        break;
                    }
                } // switch( m_state )
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

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pEv);
    }
    return bEventHandled;

} // event
