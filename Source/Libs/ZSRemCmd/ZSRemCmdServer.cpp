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
#include <QtCore/qmutex.h>
#include <QtCore/qthread.h>

#include "ZSRemCmd/ZSRemCmdServer.h"
#include "ZSRemCmd/ZSRemCmdErrNumbers.h"
#include "ZSRemCmd/ZSRemCmdMsg.h"
#include "ZSRemCmd/ZSRemCmdObj.h"
#include "ZSRemCmd/ZSRemCmdObjPool.h"
#include "ZSRemCmd/ZSRemCmdParser.h"
#include "ZSIpc/ZSIpcBlkTypeL.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::RemCmd;


/*******************************************************************************
class CRemCmdServer : public ZS::Ipc::CServer
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdServer::CRemCmdServer( const QString& i_strObjName ) :
//------------------------------------------------------------------------------
    CServer(i_strObjName, false),
    m_pRemCmdParser(nullptr),
    m_pRemCmdObjPool(nullptr),
    // Method Trace
    m_pTrcAdminObj(nullptr)
{
    QString strObjName = i_strObjName;

    if( strObjName.isEmpty() )
    {
        strObjName = "RemCmdServer";
    }
    setObjectName(strObjName);

    // Create trace admin object
    //====================================

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::RemCmd", "CRemCmdServer", i_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    // Setup IPC Server
    //====================================

    CBlkType* pBlkType = new CBlkTypeL(EBlkLenDataTypeBinUINT32);
    setBlkType(pBlkType);
    pBlkType = nullptr;

    if( m_pErrLog == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotDefined, "m_pErrLog == nullptr" );
    }

    m_pErrLog->setEntriesCountMax(1000, EResultSeverityError);
    m_pErrLog->setEntriesCountMax(1000, EResultSeverityCritical);

    // Create Remote Command Parser
    //===========================================================================

    m_pRemCmdParser = new CRemCmdParser(i_strObjName + "Parser");

    // Create Command Object Pool
    //===========================================================================

    m_pRemCmdObjPool = new CRemCmdObjPool(i_strObjName + "ObjPool");

    // Connect to the signals of the object pool model
    //===========================================================================

    if( !QObject::connect(
        /* pObjSender   */ m_pRemCmdObjPool,
        /* szSignal     */ SIGNAL( objectNodeInserted(QObject*,ZS::System::CObjPoolListEntry*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onObjNodeInserted(QObject*,ZS::System::CObjPoolListEntry*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CRemCmdServer::~CRemCmdServer()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // Destroy instance members
    //-------------------------

    try
    {
        delete m_pRemCmdObjPool;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pRemCmdParser;
    }
    catch(...)
    {
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    // Invalidate instance members
    //----------------------------

    m_pRemCmdParser = nullptr;
    m_pRemCmdObjPool = nullptr;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdSubSysNode* CRemCmdServer::getRemCmdSubSysNode( const QString& i_strNode )
//------------------------------------------------------------------------------
{
    CRemCmdSubSysNode* pRemCmdSubSysNode = nullptr;

    CObjPoolTreeEntry* pTreeEntry = m_pRemCmdObjPool->getTreeEntry(i_strNode,EObjPoolEntryTypeNameSpace);

    if( pTreeEntry != nullptr )
    {
        pRemCmdSubSysNode = reinterpret_cast<CRemCmdSubSysNode*>(pTreeEntry->getObj());
    }
    return pRemCmdSubSysNode;

} // getRemCmdSubSysNode

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdObj* CRemCmdServer::getRemCmdObj( const QString& i_strCmd )
//------------------------------------------------------------------------------
{
    return reinterpret_cast<CRemCmdObj*>(m_pRemCmdObjPool->getObj(i_strCmd));
}

//------------------------------------------------------------------------------
CRemCmdObj* CRemCmdServer::getRemCmdObj( int i_iObjId )
//------------------------------------------------------------------------------
{
    return reinterpret_cast<CRemCmdObj*>(m_pRemCmdObjPool->getObj(i_iObjId));
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdServer::addErrResultInfo( const ZS::System::SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = i_errResultInfo;

    EResultSeverity severity = errResultInfo.getSeverity();

    int iCount    = m_pErrLog->getEntryCount(severity);
    int iCountMax = m_pErrLog->getEntriesCountMax(severity);

    if( iCount == iCountMax )
    {
        m_pErrLog->removeEntry(iCount-1, severity);

        QString strAddErrInfo;
        strAddErrInfo  = "MaxErrResultInfoCount " + QString::number(iCountMax);
        strAddErrInfo += " for severity " + resultSeverity2Str(severity) + " reached";
        errResultInfo = ErrResultInfoError("addErrResultInfo", EResultListIsFull, strAddErrInfo);
    }

    m_pErrLog->addEntry(errResultInfo);

} // addErrResultInfo

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CRemCmdServer::parseData(
    const QByteArray& i_byteArrData,
    QString*          o_pstrCmd,
    bool*             o_pbIsQuery,
    QByteArray*       o_pByteArrArgs ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_byteArrData;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "parseData",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo( objectName(), EResultSuccess );

    if( m_pRemCmdParser != nullptr )
    {
        errResultInfo = m_pRemCmdParser->parse(
            /* byteArrData */ i_byteArrData,
            /* pstrCmd     */ o_pstrCmd,
            /* pbIsQuery   */ o_pbIsQuery,
            /* pstrlstArgs */ o_pByteArrArgs );
    }

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        if( errResultInfo.isErrorResult() )
        {
            strAddTrcInfo = errResultInfo.getSeverityStr() + " " + errResultInfo.getResultStr() + " " + errResultInfo.getAddErrInfoDscr();
        }
        else
        {
            strAddTrcInfo = "Ok";
        }

        if( o_pstrCmd != nullptr )
        {
            strAddTrcInfo += ", Cmd: " + *o_pstrCmd;
        }
        if( o_pbIsQuery != nullptr )
        {
            strAddTrcInfo += ", IsQuery: " + bool2Str(*o_pbIsQuery);
        }
        if( o_pByteArrArgs != nullptr )
        {
            strAddTrcInfo += ", Args: " + *o_pByteArrArgs;
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return errResultInfo;

} // parseData

//------------------------------------------------------------------------------
SErrResultInfo CRemCmdServer::executeCommand(
    const ZS::Ipc::SSocketDscr& i_socketDscr,
    const QString&              i_strCmd,
    bool                        i_bIsQuery,
    const QByteArray&           i_byteArrArgs )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "SocketDscr: " + i_socketDscr.getConnectionString();
        strAddTrcInfo += ", Cmd: " + i_strCmd;
        strAddTrcInfo += ", IsQuery: " + bool2Str(i_bIsQuery);
        strAddTrcInfo += ", Args: " + i_byteArrArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeCommand",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo( objectName(), EResultSuccess );

    CRemCmdObj* pRemCmdObj = getRemCmdObj(i_strCmd);

    if( pRemCmdObj == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr( "Remote command object " + i_strCmd + " is not existing." );
        //errResultInfo.setUserErrCode(EErrNrUndefinedHeader);
        //errResultInfo.setUserErrCodeStr(errNr2Str(EErrNrUndefinedHeader));

        addErrResultInfo(errResultInfo);
    }
    else // if( pRemCmdObj != nullptr )
    {
        if( i_bIsQuery )
        {
            if( pRemCmdObj->isEventOnly() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Command type is event only." );
                //errResultInfo.setUserErrCode(EErrNrCommandError);
                //errResultInfo.setUserErrCodeStr(errNr2Str(EErrNrCommandError));

                addErrResultInfo(errResultInfo);
            }
        }
        else // if( !bIsQuery )
        {
            if( pRemCmdObj->isQueryOnly() )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultRequestRefused);
                errResultInfo.setAddErrInfoDscr( "Command type is query only." );
                //errResultInfo.setUserErrCode(EErrNrCommandError);
                //errResultInfo.setUserErrCodeStr(errNr2Str(EErrNrCommandError));

                addErrResultInfo(errResultInfo);
            }
        }

        if( !errResultInfo.isErrorResult() )
        {
            CMsgReqCmdExecute* pMsgReq = new CMsgReqCmdExecute(
                /* pObjSender       */ this,
                /* pObjReceiver     */ pRemCmdObj,
                /* socketDscr       */ i_socketDscr,
                /* strCmd           */ i_strCmd,
                /* bIsQuery         */ i_bIsQuery,
                /* byteArrArgs      */ i_byteArrArgs,
                /* bMustBeConfirmed */ true,
                /* iReqId           */ -1 );
            POST_OR_DELETE_MESSAGE(pMsgReq);
            pMsgReq = nullptr;

            errResultInfo.setResult(EResultUndefined);

        } // if( !errResultInfo.isErrorResult() )
    } // if( pRemCmdObj != nullptr )

    return errResultInfo;

} // executeCommand

/*==============================================================================
protected: // overridables of base class ZS::Ipc::CServer
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdServer::onReceivedData( int i_iSocketId, const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo  = "SocketId: " + QString::number(i_iSocketId);
        strAddTrcInfo += "Data: " + i_byteArr;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onReceivedData",
        /* strAddInfo   */ strAddTrcInfo );

    QString    strCmd;
    bool       bIsQuery;
    QByteArray byteArrArgs;

    SErrResultInfo errResultInfo = parseData(
        /* byteArrData */ i_byteArr,
        /* pstrCmd     */ &strCmd,
        /* pbIsQuery   */ &bIsQuery,
        /* pstrlstArgs */ &byteArrArgs);

    if( !errResultInfo.isErrorResult() )
    {
        SSocketDscr socketDscr = getSocketDscr(i_iSocketId);

        errResultInfo = executeCommand( socketDscr, strCmd, bIsQuery, byteArrArgs );
    }

} // onReceivedData

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CRemCmdServer::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pMsg->type() >= QEvent::User )
    {
        QString strAddTrcInfo;

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
            strAddTrcInfo = "Addr: 0x" + QString::number(reinterpret_cast<qint64>(i_pMsg),16);

            if( pMsg == nullptr )
            {
                strAddTrcInfo += ", Content {MsgType: " + QString::number(i_pMsg->type()) + "}";
            }
            else
            {
                strAddTrcInfo += ", Content {" + pMsg->getAddTrcInfoStr() + "}";
            }
        }

        CMethodTracer mthTracer(
            /* pAdminObj    */ m_pTrcAdminObj,
            /* iDetailLevel */ ETraceDetailLevelMethodCalls,
            /* strMethod    */ "event",
            /* strAddInfo   */ strAddTrcInfo );

        if( pMsg != nullptr )
        {
            switch( static_cast<int>(pMsg->type()) )
            {
                case EMsgTypeConCmdExecute:
                {
                    CMsgConCmdExecute* pMsgCon = dynamic_cast<CMsgConCmdExecute*>(pMsg);
                    if( pMsgCon == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultInvalidDynamicTypeCast);
                    }

                    QByteArray byteArrResults = pMsgCon->getResults();

                    if( byteArrResults.size() > 0 || pMsgCon->wasQuery() )
                    {
                        if( pMsgCon->getSocketId() >= 0 )
                        {
                            sendData( pMsgCon->getSocketId(), byteArrResults );
                        }

                        emit dataSent( this, pMsgCon->getSocketId(), byteArrResults );

                    } // if( strlstResults.count() > 0 )

                    SErrResultInfo errResultInfo = pMsgCon->getErrResultInfo();

                    if( errResultInfo.isErrorResult() )
                    {
                        addErrResultInfo(errResultInfo);
                    }

                    //emit commandConfirmed( this, pMsgCon->getSocketId(), pMsgCon->getCommand(), pMsgCon->getResults(), pMsgCon->getErrResultInfo() );

                    bEventHandled = true;
                    break;
                } // case EMsgTypeConCmdExecute:

                default:
                {
                    break;
                }
            } // switch( pMsg->type() )
        } // if( pMsg != nullptr )
    } // if( i_pMsg->type() >= QEvent::User )

    if( !bEventHandled )
    {
        bEventHandled = Ipc::CServer::event(i_pMsg);
    }
    return bEventHandled;

} // event

/*==============================================================================
protected slots: // connected to the slots of the object pool model
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdServer::onObjNodeInserted( QObject* /*i_pObjPool*/, ZS::System::CObjPoolListEntry* i_pListEntry )
//------------------------------------------------------------------------------
{
    COBJPOOL_MUTEXLOCKER(m_pRemCmdObjPool);

    if( i_pListEntry != nullptr && isConnected() )
    {
        CRemCmdObj* pRemCmdObj = reinterpret_cast<CRemCmdObj*>(i_pListEntry->getObj());

        if( pRemCmdObj != nullptr )
        {
            QByteArray byteArr = str2ByteArr(pRemCmdObj->toXmlString());
            sendData( ESocketIdAllSockets, byteArr );
            emit dataSent( this, ESocketIdAllSockets, byteArr );
        }
    }

} // onObjNodeInserted
