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

#include "ZSData/ZSDatabaseGateway.h"
#include "ZSData/ZSDatabase.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Data;


/*******************************************************************************
class CDbGateway : public QObject
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/*------------------------------------------------------------------------------
enum ERequest
------------------------------------------------------------------------------*/

const SEnumEntry s_arstrConnectionGatewayRequests[CDbGateway::ERequestCount] = {
    /* 0: */ SEnumEntry( CDbGateway::ERequestNone,               "None"               ),
    /* 1: */ SEnumEntry( CDbGateway::ERequestStartup,            "Startup"            ),
    /* 2: */ SEnumEntry( CDbGateway::ERequestShutdown,           "Shutdown"           ),
    /* 3: */ SEnumEntry( CDbGateway::ERequestUpdateHostSettings, "UpdateHostSettings" )
};

//------------------------------------------------------------------------------
QString CDbGateway::Request2Str( int i_iRequest )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arstrConnectionGatewayRequests, _ZSArrLen(s_arstrConnectionGatewayRequests), i_iRequest );
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDbGateway::CDbGateway(
    EProtocolType        i_protocolType,
    ZS::Ipc::ESocketType i_socketType,
    CDb*                 i_pDb,
    CDbGatewayThread*    i_pGatewayThread ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pDb(i_pDb),
    m_pGatewayThread(i_pGatewayThread),
    m_protocolType(i_protocolType),
    m_hostSettings(i_socketType),
    m_requestInProgress(ERequestNone),
    m_iReqIdInProgress(-1),
    m_iMsgIdReqInProgress(-1),
    m_pMsgCon(nullptr),
    m_arpTrcMsgLogObjects(),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( m_pDb->objectName() + protocolType2Str(i_protocolType) + socketType2Str(i_socketType) );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(nameSpace(), className(), objectName());

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CDbGateway::~CDbGateway()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    try
    {
        delete m_pMsgCon;
    }
    catch(...)
    {
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDb = nullptr;
    m_pGatewayThread = nullptr;
    m_protocolType = static_cast<EProtocolType>(0);
    //m_hostSettings;
    m_requestInProgress = static_cast<ERequest>(0);
    m_iReqIdInProgress = 0;
    m_iMsgIdReqInProgress = 0;
    m_pMsgCon = nullptr;
    //m_arpTrcMsgLogObjects.clear();
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbGateway::abortRequestInProgress()
//------------------------------------------------------------------------------
{
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
        strAddTrcInfo  = "ReqInProgress: " + Request2Str(m_requestInProgress);
        strAddTrcInfo += ", ReqIdInProgress: " + QString::number(m_iReqIdInProgress);
        strAddTrcInfo += ", MsgReqIdProgress: " + QString::number(m_iMsgIdReqInProgress);
        mthTracer.trace(strAddTrcInfo);
    }

    //switch( m_requestInProgress )
    //{
    //    case ERequestNone:
    //    default:
    //    {
    //        break;
    //    }
    //}

} // abortRequestInProgress

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDbGateway::addTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
    if( i_pObj == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObj == nullptr");
    }

    QObject* pObj;
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

} // addTrcMsgLogObject

//------------------------------------------------------------------------------
void CDbGateway::removeTrcMsgLogObject( QObject* i_pObj )
//------------------------------------------------------------------------------
{
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

} // removeTrcMsgLogObject

/*==============================================================================
public: // must overridables (overridables of base class QObject)
==============================================================================*/

//------------------------------------------------------------------------------
bool CDbGateway::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    //--------------------------------------------------------------------------
    // Method body which can be used as a template.
    // -------------------------------------------------------------------------
    // This method makes not really much. The event method must be overwritten
    // by derived classes. This method may be used as a template for copying
    // the code extending the code for handling the received messages (see
    // TODO_BEGIN and TODO_END blocks).
    //--------------------------------------------------------------------------

    bool bEventHandled = false;

    CMsg* pMsg = dynamic_cast<CMsg*>(i_pMsg);

    if( pMsg != nullptr )
    {
        bEventHandled = true;

        //SErrResultInfo errResultInfo( objectName() + ".event" );

        QString strAddTrcInfo;

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
        {
            int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
            strAddTrcInfo = "Msg { " + pMsg->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + " }";
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strAddTrcInfo  = "ReqInProgress: " + Request2Str(m_requestInProgress);
            strAddTrcInfo += ", ReqIdInProgress: " + QString::number(m_iReqIdInProgress);
            strAddTrcInfo += ", MsgReqIdProgress: " + QString::number(m_iMsgIdReqInProgress);
            mthTracer.trace(strAddTrcInfo);
        }
    } // if( pMsg != nullptr )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;

} // event
