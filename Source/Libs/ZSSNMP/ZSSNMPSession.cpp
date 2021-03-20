/*******************************************************************************

Copyright 2004 - 2010 by    ZeusSoft, Ing. Buero Bauer
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

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdatetime.h>
#define HAVE_SOCKLEN_T
#include "net-snmp/net-snmp-config.h"
#include "net-snmp/net-snmp-includes.h"
#undef index
#include "ZSSNMP/Include/ZSSNMPSession.h"
#include "ZSSNMP/Include/ZSSNMPMsg.h"
#include "ZSTrace/Include/ZSTrcAdminObj.h"
#include "ZSTrace/Include/ZSTrcMethod.h"
#include "ZSTrace/Include/ZSTrcServer.h"
#include "ZSSys/Include/ZSSysErrLogModel.h"
#include "ZSSys/Include/ZSSysErrResult.h"
#include "ZSSys/Include/ZSSysException.h"
#include "ZSSys/Include/ZSSysMemLeakDump.h"

using namespace ZS::SNMP;
using namespace ZS::Trace;


/*******************************************************************************
class CSessionThread : public QThread
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CSessionThread::CSessionThread(
    const QString&    i_strObjNameSession,
    CExplorer*        i_pExplorer,
    ZS::CModelErrLog* i_pModelErrLog ) :
//------------------------------------------------------------------------------
    QThread(),
    m_pModelErrLog(i_pModelErrLog),
    m_strObjName(),
    m_strObjNameSession(i_strObjNameSession),
    m_pTrcAdminObj(NULL),
    m_pExplorer(i_pExplorer),
    m_pSession(NULL)
{
    if( m_strObjNameSession.isEmpty() )
    {
        m_strObjNameSession = "SNMPSession";
    }

    setObjectName(m_strObjNameSession); // set name of thread

    m_strObjName = m_strObjNameSession + "Thread";

    m_pTrcAdminObj = CTrcServer::getTrcAdminObj(
        /* traceType         */ ETraceTypeMth,
        /* strClassNameSpace */ "ZS::SNMP",
        /* strClassName      */ "CSessionThread",
        /* strObjNameSpace   */ "",
        /* strObjName        */ m_strObjName );

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "ctor",
        /* strAddTrcInfo */ "" );

} // ctor

//------------------------------------------------------------------------------
CSessionThread::~CSessionThread()
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "dtor",
        /* strAddTrcInfo */ "" );

    m_pExplorer = NULL;
    m_pModelErrLog = NULL;

    try
    {
        delete m_pSession;
    }
    catch(...)
    {
    }
    m_pSession = NULL;

    CTrcServer::releaseTrcAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = NULL;

} // dtor

/*==============================================================================
public: // overridables of base class QThread
==============================================================================*/

//------------------------------------------------------------------------------
void CSessionThread::run()
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "run",
        /* strAddTrcInfo */ "" );

    m_pSession = new CSession(
        /* szObjName      */ m_strObjNameSession,
        /* pExplorer      */ m_pExplorer,
        /* pSessionThread */ this,
        /* pModelErrLog   */ m_pModelErrLog );

    emit sessionCreated(this,m_pSession);

    try
    {
        exec();
    }
    catch( ZS::CException* )
    {
        // This type of exception has already been logged on throwing (creating) the exception.
    }
    catch( ZS::CException& )
    {
        // This type of exception has already been logged on throwing (creating) the exception.
    }
    catch(...)
    {
        if( m_pModelErrLog != NULL )
        {
            m_pModelErrLog->addEntry(
                /* dateTime    */ QDateTime::currentDateTime(),
                /* severity    */ ZS::EResultSeverityCritical,
                /* result      */ ZS::EResultUnknownException,
                /* strResult   */ ZS::result2Str(ZS::EResultUnknownException),
                /* strSource   */ m_strObjName,
                /* strAddTrcInfo  */ "",
                /* strProposal */ "Quit Application" );
        }
    }

    try
    {
        delete m_pSession;
    }
    catch(...)
    {
    }
    m_pSession = NULL;

} // run


/*******************************************************************************
class CSession : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CSession::CSession(
    const QString&    i_strObjName,
    CExplorer*        i_pExplorer,
    CSessionThread*   i_pSessionThread,
    ZS::CModelErrLog* i_pModelErrLog ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pModelErrLog(i_pModelErrLog),
    m_strObjName(i_strObjName),
    m_pTrcAdminObj(NULL),
    m_pExplorer(i_pExplorer),
    m_pSessionThread(i_pSessionThread),
    m_hostSettings(),
    m_pSession(NULL)
{
    if( m_strObjName.isEmpty() )
    {
        m_strObjName = "SNMPSession";
    }

    m_pTrcAdminObj = CTrcServer::getTrcAdminObj(
        /* traceType         */ ETraceTypeMth,
        /* strClassNameSpace */ "ZS::SNMP",
        /* strClassName      */ "CSession",
        /* strObjNameSpace   */ "",
        /* strObjName        */ m_strObjName );

    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "ctor",
        /* strAddTrcInfo */ "" );

} // ctor

//------------------------------------------------------------------------------
CSession::~CSession()
//------------------------------------------------------------------------------
{
    M_TRACE_INST_METHOD(
        /* pAdminObj     */ m_pTrcAdminObj,
        /* iDetailLevel  */ 0,
        /* strMethod     */ "dtor",
        /* strAddTrcInfo */ "" );

    if( m_pSession != NULL )
    {
        snmp_close(m_pSession);
    }
    m_pSession = NULL;

    m_pExplorer = NULL;
    m_pSessionThread = NULL;
    m_pModelErrLog = NULL;

    CTrcServer::releaseTrcAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = NULL;

} // dtor

/*==============================================================================
protected: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CSession::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    ZS::CMsg* pMsg = dynamic_cast<ZS::CMsg*>(i_pEv);

    if( pMsg != NULL )
    {
        QString strAddTrcInfo;

        if( m_pTrcAdminObj != NULL && m_pTrcAdminObj->isActive(0) )
        {
            if( m_pSession == NULL )
            {
                strAddTrcInfo = "SessionClosed, ";
            }
            else
            {
                strAddTrcInfo = "SessionOpened, ";
            }
            strAddTrcInfo = "Msg: " + pMsg->msgTypeToStr();
        }

        M_TRACE_INST_METHOD(
            /* pAdminObj     */ m_pTrcAdminObj,
            /* iDetailLevel  */ 0,
            /* strMethod     */ "event",
            /* strAddTrcInfo */ strAddTrcInfo );

        switch( pMsg->type() )
        {
            case EMsgTypeReqOpenSession:
            {
                CMsgReqOpenSession* pMsgReq = dynamic_cast<CMsgReqOpenSession*>(pMsg);

                SErrResultInfo errResultInfo;
                QString        strAddErrInfo;

                if( m_pSession != NULL )
                {
                    snmp_close(m_pSession);
                    m_pSession = NULL;
                }

                m_hostSettings = pMsgReq->getHostSettings();

                errResultInfo.setErrSource( m_strObjName + " (" + m_hostSettings.m_strName + "/" + m_hostSettings.m_strCommunity + ")" );

                strAddErrInfo = pMsgReq->msgTypeToStr() + ": ";

                struct snmp_session session;

                // Init session setting up defaults for the session struct
                snmp_sess_init( &session );

                char* szPeerName = new char[m_hostSettings.m_strName.length()+1];
                #ifdef _WINDOWS
                #pragma warning( disable: 4996 )
                #endif
                strcpy( szPeerName, m_hostSettings.m_strName.toAscii().data() );
                #ifdef _WINDOWS
                #pragma warning( default: 4996 )
                #endif

                size_t  iStrLenCommunity = m_hostSettings.m_strCommunity.length();
                u_char* pucCommunity = new u_char[iStrLenCommunity+1];
                memcpy( pucCommunity, m_hostSettings.m_strCommunity.toAscii().data(), iStrLenCommunity );
                pucCommunity[iStrLenCommunity] = 0x00;

                session.version = SNMP_VERSION_2c;
                session.peername = szPeerName;
                session.remote_port = m_hostSettings.m_uPort;
                session.community = pucCommunity;
                session.community_len = iStrLenCommunity;

                if( methodTracerInst.isActive(1) )
                {
                    strAddTrcInfo  = "-> snmp_open( ";
                    strAddTrcInfo += m_hostSettings.m_strName + ":";
                    strAddTrcInfo += QString::number(m_hostSettings.m_uPort) + "/";
                    strAddTrcInfo += m_hostSettings.m_strCommunity + " )";
                    methodTracerInst.trace(strAddTrcInfo);
                }

                m_pSession = snmp_open(&session);

                delete [] szPeerName;
                szPeerName = NULL;
                delete [] pucCommunity;
                pucCommunity = NULL;

                if( m_pSession == NULL)
                {
                    strAddErrInfo += snmp_api_errstring(session.s_snmp_errno);

                    errResultInfo.setSeverity(ZS::EResultSeverityError);
                    errResultInfo.setResult(ZS::EResultSessionOpenError);
                    errResultInfo.setResultStr(ZS::result2Str(ZS::EResultSessionOpenError));
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                }

                if( methodTracerInst.isActive(1) )
                {
                    strAddTrcInfo  = "<- snmp_open( Result: ";
                    strAddTrcInfo += errResultInfo.getResultStr();
                    if( errResultInfo.getResult() != EResultSuccess )
                    {
                        strAddTrcInfo += errResultInfo.getAddErrInfoDscr();
                    }
                    strAddTrcInfo += " )";
                    methodTracerInst.trace(strAddTrcInfo);
                }

                CMsgConOpenSession* pMsgCon = new CMsgConOpenSession(
                    /* pObjSender    */ this,
                    /* pObjReceiver  */ pMsgReq->getSender(),
                    /* iMsgId        */ pMsgReq->getMsgId(),
                    /* errResultInfo */ errResultInfo );
                QCoreApplication::postEvent( pMsgReq->getSender(), pMsgCon );
                pMsgCon = NULL;
                break;
            } // case EMsgTypeReqOpenSession

            case EMsgTypeReqCloseSession:
            {
                CMsgReqCloseSession* pMsgReq = dynamic_cast<CMsgReqCloseSession*>(pMsg);

                if( m_pSession != NULL )
                {
                    if( methodTracerInst.isActive(1) )
                    {
                        strAddTrcInfo = "-> snmp_close()";
                        methodTracerInst.trace(strAddTrcInfo);
                    }

                    snmp_close(m_pSession);

                    if( methodTracerInst.isActive(1) )
                    {
                        strAddTrcInfo = "<- snmp_close()";
                        methodTracerInst.trace(strAddTrcInfo);
                    }
                }
                m_pSession = NULL;

                if( pMsgReq->mustBeConfirmed() )
                {
                    CMsgConCloseSession* pMsgCon = new CMsgConCloseSession(
                        /* pObjSender    */ this,
                        /* pObjReceiver  */ pMsgReq->getSender(),
                        /* iMsgId        */ pMsgReq->getMsgId() );
                    QCoreApplication::postEvent( pMsgCon->getReceiver(), pMsgCon );
                    pMsgCon = NULL;
                }
                break;
            } // case EMsgTypeReqCloseSession

            case EMsgTypeReqGetValue:
            case EMsgTypeReqGetNextValue:
            case EMsgTypeReqGetBulk:
            {
                CMsgReqGet* pMsgReq = dynamic_cast<CMsgReqGet*>(pMsg);

                SErrResultInfo errResultInfo;
                QString        strAddErrInfo;
                QString        strAddErrInfoOids;
                TValuePtrList  valuePtrListCon;
                COid*          pOid;
                int            idxOid;

                for( idxOid = 0; idxOid < pMsgReq->getOidCount(); idxOid++ )
                {
                    pOid = pMsgReq->getOid(idxOid,false);

                    if( pOid != NULL )
                    {
                        if( !strAddErrInfoOids.isEmpty() )
                        {
                            strAddErrInfoOids += "; ";
                        }
                        strAddErrInfoOids += pOid->toString(EOidFormatFull);
                    }
                }

                errResultInfo.setErrSource( m_strObjName + " (" + m_hostSettings.m_strName + "/" + m_hostSettings.m_strCommunity + ")" );

                if( m_pSession == NULL )
                {
                    strAddErrInfo = pMsgReq->msgTypeToStr() + " " + strAddErrInfoOids + ": No SNMP Session";
                    errResultInfo.setSeverity(ZS::EResultSeverityError);
                    errResultInfo.setResult(ZS::EResultRequestRefused);
                    errResultInfo.setResultStr(ZS::result2Str(ZS::EResultRequestRefused));
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                }
                else
                {
                    EValueType valueType;
                    CValue*    pValue;
                    int        iRes;

                    struct snmp_pdu*      pPdu;
                    struct snmp_pdu*      pPduResponse;
                    struct variable_list* pVarList;

                    if( pMsg->type() == static_cast<QEvent::Type>(EMsgTypeReqGetValue) )
                    {
                        if( methodTracerInst.isActive(1) )
                        {
                            strAddTrcInfo = "-> snmp_pdu_create(SNMP_MSG_GET)";
                            methodTracerInst.trace(strAddTrcInfo);
                        }

                        pPdu = snmp_pdu_create(SNMP_MSG_GET);

                        if( methodTracerInst.isActive(1) )
                        {
                            strAddTrcInfo = "<- snmp_pdu_create()";
                            methodTracerInst.trace(strAddTrcInfo);
                        }
                    }
                    else if( pMsg->type() == static_cast<QEvent::Type>(EMsgTypeReqGetNextValue) )
                    {
                        if( methodTracerInst.isActive(1) )
                        {
                            strAddTrcInfo = "-> snmp_pdu_create(SNMP_MSG_GETNEXT)";
                            methodTracerInst.trace(strAddTrcInfo);
                        }

                        pPdu = snmp_pdu_create(SNMP_MSG_GETNEXT);

                        if( methodTracerInst.isActive(1) )
                        {
                            strAddTrcInfo = "<- snmp_pdu_create()";
                            methodTracerInst.trace(strAddTrcInfo);
                        }
                    }
                    else // if( pMsg->type() == EMsgTypeReqGetBulk )
                    {
                        if( methodTracerInst.isActive(1) )
                        {
                            strAddTrcInfo  = "-> snmp_pdu_create(SNMP_MSG_GETBULK)";
                            methodTracerInst.trace(strAddTrcInfo);
                        }

                        pPdu = snmp_pdu_create(SNMP_MSG_GETBULK);
                        #ifdef _WINDOWS
                        #pragma message(__TODO__"max_repetitions should be set via message")
                        #endif
                        pPdu->max_repetitions = 10;
                        pPdu->non_repeaters = 0;

                        if( methodTracerInst.isActive(1) )
                        {
                            strAddTrcInfo = "<- snmp_pdu_create( max_repetitions:" + QString::number(pPdu->max_repetitions) + " )";
                            methodTracerInst.trace(strAddTrcInfo);
                        }
                    }

                    for( idxOid = 0; idxOid < pMsgReq->getOidCount(); idxOid++ )
                    {
                        pOid = pMsgReq->getOid(idxOid,false);

                        if( pOid != NULL )
                        {
                            if( methodTracerInst.isActive(1) )
                            {
                                strAddTrcInfo = "-> snmp_add_null_var( " + pOid->toString() + " )";
                                methodTracerInst.trace(strAddTrcInfo);
                            }

                            snmp_add_null_var( pPdu, pOid->getSubIdsList(), pOid->getSubIdsCount() );

                            if( methodTracerInst.isActive(1) )
                            {
                                strAddTrcInfo = "<- snmp_add_null_var()";
                                methodTracerInst.trace(strAddTrcInfo);
                            }
                        }
                    }

                    if( methodTracerInst.isActive(1) )
                    {
                        strAddTrcInfo = "-> snmp_synch_response()";
                        methodTracerInst.trace(strAddTrcInfo);
                    }

                    iRes = snmp_synch_response( m_pSession, pPdu, &pPduResponse );

                    if( iRes != STAT_SUCCESS )
                    {
                        strAddErrInfo = pMsgReq->msgTypeToStr() + " " + strAddErrInfoOids;
                        errResultInfo.setSeverity(ZS::EResultSeverityError);
                        errResultInfo.setResult(ZS::EResultRequestRefused);
                        errResultInfo.setResultStr(snmp_api_errstring(m_pSession->s_snmp_errno));
                        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                    }
                    else if( pPduResponse == NULL )
                    {
                        strAddErrInfo = pMsgReq->msgTypeToStr() + " " + strAddErrInfoOids + ": " + "No Response";
                        errResultInfo.setSeverity(ZS::EResultSeverityError);
                        errResultInfo.setResult(ZS::EResultRequestRefused);
                        errResultInfo.setResultStr(ZS::result2Str(ZS::EResultRequestRefused));
                        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                    }
                    else if( pPduResponse->errstat != SNMP_ERR_NOERROR )
                    {
                        errResultInfo.setSeverity(ZS::EResultSeverityWarning);

                        if( pPduResponse->variables != NULL && pPduResponse->errstat == SNMP_ERR_NOSUCHNAME )
                        {
                            //COid oid( pPduResponse->variables->name, pPduResponse->variables->name_length );
                            //strAddErrInfo = pMsgReq->msgTypeToStr() + " " + oid.toString(EOidFormatFull);
                            strAddErrInfo = pMsgReq->msgTypeToStr() + " " + strAddErrInfoOids;
                            errResultInfo.setResult(ZS::EResultRequestRefused);
                            errResultInfo.setResultStr(snmp_errstring(pPduResponse->errstat));
                            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                        }
                        else
                        {
                            strAddErrInfo = pMsgReq->msgTypeToStr() + " " + strAddErrInfoOids;
                            errResultInfo.setResult(ZS::EResultRequestRefused);
                            errResultInfo.setResultStr(snmp_errstring(pPduResponse->errstat));
                            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                        }
                    }
                    else
                    {
                        for( pVarList = pPduResponse->variables; pVarList != NULL; pVarList = pVarList->next_variable )
                        {
                            valueType = pduDataType2ValueType(pVarList->type);

                            switch( valueType )
                            {
                                case EValueTypeUndefined:
                                {
                                    COid oid( pVarList->name, pVarList->name_length );
                                    strAddErrInfo  = pMsgReq->msgTypeToStr() + " " + oid.toString(EOidFormatFull) + ": ";
                                    strAddErrInfo += "PDU Data Type " + QString::number(pVarList->type) + "(" + QString::number(pVarList->type,16) + ") not yet supported";
                                    errResultInfo.setSeverity(ZS::EResultSeverityWarning);
                                    errResultInfo.setResult(ZS::EResultUnknownDataType);
                                    errResultInfo.setResultStr(ZS::result2Str(ZS::EResultUnknownDataType));
                                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                                    break;
                                }
                                case EValueTypeNull:
                                case EValueTypeSequence:
                                case EValueTypeOpaque:
                                {
                                    COid oid( pVarList->name, pVarList->name_length );
                                    strAddErrInfo  = pMsgReq->msgTypeToStr() + " " + oid.toString(EOidFormatFull) + ": ";
                                    strAddErrInfo += "PDU Data Type " + QString::number(pVarList->type) + "(" + QString::number(pVarList->type,16) + ") not yet supported";
                                    errResultInfo.setSeverity(ZS::EResultSeverityWarning);
                                    errResultInfo.setResult(ZS::EResultUnsupportedDataType);
                                    errResultInfo.setResultStr(ZS::result2Str(ZS::EResultUnsupportedDataType));
                                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                                    break;
                                }
                                case EValueTypeNoSuchObject:
                                case EValueTypeNoSuchInstance:
                                {
                                    COid oid( pVarList->name, pVarList->name_length );
                                    strAddErrInfo  = pMsgReq->msgTypeToStr() + " " + oid.toString(EOidFormatFull) + ": ";
                                    strAddErrInfo += pduDataType2Str(pVarList->type);
                                    errResultInfo.setSeverity(ZS::EResultSeverityWarning);
                                    errResultInfo.setResult(ZS::EResultObjNotInList);
                                    errResultInfo.setResultStr(ZS::result2Str(ZS::EResultObjNotInList));
                                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                                    break;
                                }
                                case EValueTypeEndOfMibView:
                                {
                                    COid oid( pVarList->name, pVarList->name_length );
                                    strAddErrInfo  = pMsgReq->msgTypeToStr() + " " + oid.toString(EOidFormatFull) + ": ";
                                    strAddErrInfo += pduDataType2Str(pVarList->type);
                                    errResultInfo.setSeverity(ZS::EResultSeverityWarning);
                                    errResultInfo.setResult(ZS::EResultEndOfList);
                                    errResultInfo.setResultStr(ZS::result2Str(ZS::EResultEndOfList));
                                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                                    break;
                                }
                                case EValueTypeString:
                                case EValueTypeBitString:
                                case EValueTypeOctetString:
                                {
                                    char arcValue[1024] = { 0 };

                                    // Convert the unsigned int sub id array into desired format.
                                    // "snprint_objid" returns the length of the string or -1 on error.
                                    int iOutputFormat = netsnmp_ds_get_boolean( NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_QUICK_PRINT );
                                    netsnmp_ds_set_boolean( NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_QUICK_PRINT, true );
                                    snprint_value( arcValue, sizeof(arcValue), pVarList->name, pVarList->name_length, pVarList );
                                    netsnmp_ds_set_boolean( NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_QUICK_PRINT, iOutputFormat );

                                    pValue = new CValue(
                                        /* puSubIds      */ pVarList->name,
                                        /* uSubIdsCount  */ pVarList->name_length,
                                        /* valueType     */ valueType,
                                        /* pcValue       */ arcValue,
                                        /* uValLen       */ strlen(arcValue),
                                        /* ucPDUDataType */ pVarList->type );
                                    valuePtrListCon.append(pValue);
                                    break;
                                }
                                case EValueTypeBoolean:
                                case EValueTypeInt:
                                case EValueTypeUInt:
                                case EValueTypeTimeTicks:
                                case EValueTypeFloat:
                                case EValueTypeDouble:
                                case EValueTypeIPAddress:
                                case EValueTypeCounter32:
                                case EValueTypeCounter64:
                                case EValueTypeOid:
                                default:
                                {
                                    pValue = new CValue(
                                        /* puSubIds      */ pVarList->name,
                                        /* uSubIdsCount  */ pVarList->name_length,
                                        /* valueType     */ valueType,
                                        /* pcValue       */ reinterpret_cast<char*>(pVarList->val.string),
                                        /* uValLen       */ pVarList->val_len,
                                        /* ucPDUDataType */ pVarList->type );
                                    valuePtrListCon.append(pValue);
                                    break;
                                }
                            } // switch( valueType )
                        } // for( pVarList != NULL; pVarList = pVarList->next_variable )
                    } // if( iRes == STAT_SUCCESS && pPduResponse->errstat == SNMP_ERR_NOERROR )

                    if( methodTracerInst.isActive(1) )
                    {
                        strAddTrcInfo  = "<- snmp_synch_response( Result: ";
                        strAddTrcInfo += errResultInfo.getResultStr();
                        if( errResultInfo.getResult() != EResultSuccess )
                        {
                            strAddTrcInfo += " (" + errResultInfo.getAddErrInfoDscr() + ")";
                        }
                        for( int idxVal = 0; idxVal < valuePtrListCon.size(); idxVal++ )
                        {
                            pValue = valuePtrListCon[idxVal];

                            if( pValue == NULL )
                            {
                                strAddTrcInfo += ", Val[" + QString::number(idxVal) + "]: NULL";
                            }
                            else
                            {
                                strAddTrcInfo += ", Val[" + QString::number(idxVal) + "]: ";
                                if( pValue->getOid() != NULL )
                                {
                                    strAddTrcInfo += pValue->getOid()->toString();
                                    strAddTrcInfo += ", " + pValue->getValueString();
                                }
                                else
                                {
                                    strAddTrcInfo += ", " + pValue->getValueString();
                                }
                            }
                        }
                        strAddTrcInfo += " )";
                        methodTracerInst.trace(strAddTrcInfo);
                    }

                    if( pPduResponse != NULL )
                    {
                        if( methodTracerInst.isActive(1) )
                        {
                            strAddTrcInfo = "-> snmp_free_pdu()";
                            methodTracerInst.trace(strAddTrcInfo);
                        }

                        snmp_free_pdu(pPduResponse);

                        if( methodTracerInst.isActive(1) )
                        {
                            strAddTrcInfo = "<- snmp_free_pdu()";
                            methodTracerInst.trace(strAddTrcInfo);
                        }
                    }
                }

                CMsgConResponse* pMsgCon = new CMsgConResponse(
                    /* pObjSender    */ this,
                    /* pObjReceiver  */ pMsgReq->getSender(),
                    /* valuePtrList  */ valuePtrListCon,
                    /* iMsgId        */ pMsgReq->getMsgId(),
                    /* errResultInfo */ errResultInfo );
                QCoreApplication::postEvent( pMsgCon->getReceiver(), pMsgCon );
                pMsgCon = NULL;
                valuePtrListCon.clear();
                break;
            } // case EMsgTypeReqGetValue|EMsgTypeReqGetNextValue|EMsgTypeReqGetBulk:

            default:
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
