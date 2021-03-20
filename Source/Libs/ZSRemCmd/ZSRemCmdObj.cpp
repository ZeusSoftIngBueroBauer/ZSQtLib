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
#include <QtCore/qthread.h>

#include "ZSRemCmd/ZSRemCmdObj.h"
#include "ZSRemCmd/ZSRemCmdObjPool.h"
#include "ZSRemCmd/ZSRemCmdErrNumbers.h"
#include "ZSRemCmd/ZSRemCmdMsg.h"
#include "ZSRemCmd/ZSRemCmdServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysRequestQueue.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::RemCmd;
using namespace ZS::Trace;


/*******************************************************************************
class CRemCmdObj : public QObject
*******************************************************************************/

/*==============================================================================
public: // ctors at servers side only
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdObj::CRemCmdObj(
    CRemCmdServer* i_pServer,
    const QString& i_strCmd,
    int            i_iObjId ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pServer(i_pServer),
    m_pObjPool(nullptr),
    // Command specification
    m_strCmd(i_strCmd),
    m_strSyntax(),
    m_bIsQueryOnly(false),
    m_bIsEventOnly(false),
    m_strDscr(),
    m_pObjFctExecute(nullptr),
    m_fctExecute(nullptr),
    m_iObjId(-1),
    m_pListEntry(nullptr),
    // State Machine
    m_hshpReqsInProgress(),
    m_pReqSyncToBeDeletedLater(nullptr),
    // Method Trace
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strCmd);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::RemCmd", "CRemCmdObj", i_strCmd);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pServer == nullptr )
        {
            strAddTrcInfo = "Server: nullptr";
        }
        else
        {
            strAddTrcInfo = "Server: " + i_pServer->objectName();
        }
        strAddTrcInfo += ", Cmd: " + i_strCmd;
        strAddTrcInfo += ", ObjId: " + QString::number(i_iObjId);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pServer == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pRemCmdServer == nullptr");
    }
    if( m_pServer->getObjPool() == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pRemCmdServer->getObjPool() == nullptr");
    }

    m_pObjPool = m_pServer->getObjPool();

    if( m_iObjId < 0 )
    {
        m_iObjId = m_pObjPool->addObjNode(i_strCmd,EObjState::Created);
    }
    else
    {
        m_pObjPool->addObjNode(i_strCmd,m_iObjId,EObjState::Created);
    }

    m_pListEntry = m_pObjPool->getListEntry(m_iObjId);
    m_pListEntry->setObj(this);
    m_pListEntry->getTreeEntry()->setObj(this);

} // ctor

//------------------------------------------------------------------------------
CRemCmdObj::CRemCmdObj(
    CRemCmdServer* i_pServer,
    const QString& i_strCmd,
    QObject*       i_pObjFctExecute,
    TFctExecute    i_fctExecute,
    int            i_iObjId ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pServer(i_pServer),
    m_pObjPool(nullptr),
    // Command specification
    m_strCmd(i_strCmd),
    m_strSyntax(),
    m_bIsQueryOnly(false),
    m_bIsEventOnly(false),
    m_strDscr(),
    m_pObjFctExecute(i_pObjFctExecute),
    m_fctExecute(i_fctExecute),
    m_iObjId(-1),
    m_pListEntry(nullptr),
    // State Machine
    m_hshpReqsInProgress(),
    m_pReqSyncToBeDeletedLater(nullptr),
    // Method Trace
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::RemCmd", "CRemCmdObj", i_strCmd);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pServer == nullptr )
        {
            strAddTrcInfo = "Server: nullptr";
        }
        else
        {
            strAddTrcInfo = "Server: " + i_pServer->objectName();
        }
        strAddTrcInfo += ", Cmd: " + i_strCmd;

        if( i_pObjFctExecute == nullptr )
        {
            strAddTrcInfo += ", ObjFctExecute: nullptr";
        }
        else
        {
            strAddTrcInfo += ", ObjFctExecute: " + i_pObjFctExecute->objectName();
        }
        if( i_fctExecute == nullptr )
        {
            strAddTrcInfo += ", fctExecute: nullptr";
        }
        else
        {
            strAddTrcInfo += ", fctExecute: 0x" + QString::number(reinterpret_cast<qint64>(i_fctExecute),16);
        }
        strAddTrcInfo += ", ObjId: " + QString::number(i_iObjId);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_pServer == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pRemCmdServer == nullptr");
    }
    if( m_pServer->getObjPool() == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pRemCmdServer->getObjPool() == nullptr");
    }

    m_pObjPool = m_pServer->getObjPool();

    if( m_iObjId < 0 )
    {
        m_iObjId = m_pObjPool->addObjNode(i_strCmd,EObjState::Created);
    }
    else
    {
        m_pObjPool->addObjNode(i_strCmd,m_iObjId,EObjState::Created);
    }

    m_pListEntry = m_pObjPool->getListEntry(m_iObjId);
    m_pListEntry->setObj(this);
    m_pListEntry->getTreeEntry()->setObj(this);

} // ctor

/*==============================================================================
public: // ctors at clients side
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdObj::CRemCmdObj(
    CRemCmdObjPool* i_pObjPool,
    const QString&  i_strCmd,
    int             i_iObjId ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pServer(nullptr),
    m_pObjPool(i_pObjPool),
    // Command specification
    m_strCmd(i_strCmd),
    m_strSyntax(),
    m_bIsQueryOnly(false),
    m_bIsEventOnly(false),
    m_strDscr(),
    m_pObjFctExecute(nullptr),
    m_fctExecute(nullptr),
    m_iObjId(i_iObjId),
    m_pListEntry(nullptr),
    // State Machine
    m_hshpReqsInProgress(),
    m_pReqSyncToBeDeletedLater(nullptr),
    // Method Trace
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::RemCmd", "CRemCmdObj", i_strCmd);

    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pObjPool == nullptr )
        {
            strAddTrcInfo = "ObjPool: nullptr";
        }
        else
        {
            strAddTrcInfo = "ObjPool: " + i_pObjPool->objectName();
        }
        strAddTrcInfo += ", Cmd: " + i_strCmd;
        strAddTrcInfo += ", ObjId: " + QString::number(i_iObjId);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( m_pObjPool == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultArgOutOfRange,"i_pObjPool == nullptr");
    }

    if( m_iObjId < 0 )
    {
        m_iObjId = m_pObjPool->addObjNode(i_strCmd,EObjState::Created);
    }
    else
    {
        m_pObjPool->addObjNode(i_strCmd,m_iObjId,EObjState::Created);
    }

    m_pListEntry = m_pObjPool->getListEntry(m_iObjId);
    m_pListEntry->setObj(this);
    m_pListEntry->getTreeEntry()->setObj(this);

} // ctor

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdObj::~CRemCmdObj()
//------------------------------------------------------------------------------
{
    m_pObjPool->invalidateObjNode(m_iObjId);

    try
    {
        delete m_pReqSyncToBeDeletedLater;
    }
    catch(...)
    {
    }

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pServer = nullptr;
    m_pObjPool = nullptr;
    // Command specification
    //m_strCmd;
    //m_strSyntax;
    m_bIsQueryOnly = false;
    m_bIsEventOnly = false;
    //m_strDscr;
    m_pObjFctExecute = nullptr;
    m_fctExecute = nullptr;
    m_pListEntry = nullptr;
    m_iObjId = 0;
    // State Machine
    //m_hshpReqsInProgress.clear();
    m_pReqSyncToBeDeletedLater = nullptr;
    // Method Trace
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdObj::setSyntax( const QString& i_strSyntax )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strSyntax;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setSyntax",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_strSyntax != i_strSyntax )
    {
        m_strSyntax = i_strSyntax;
        m_pObjPool->dataChanged( m_pListEntry->getTreeEntry() );
        emit changed(this);
    }

} // setSyntax

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdObj::setIsQueryOnly( bool i_bIs )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = bool2Str(i_bIs);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setIsQueryOnly",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_bIsQueryOnly != i_bIs )
    {
        m_bIsQueryOnly = i_bIs;
        m_pObjPool->dataChanged( m_pListEntry->getTreeEntry() );
        emit changed(this);
    }

} // setIsQueryOnly

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdObj::setIsEventOnly( bool i_bIs )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = bool2Str(i_bIs);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setIsEventOnly",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_bIsEventOnly != i_bIs )
    {
        m_bIsEventOnly = i_bIs;
        m_pObjPool->dataChanged( m_pListEntry->getTreeEntry() );
        emit changed(this);
    }

} // setIsEventOnly

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdObj::setDescription( const QString& i_strDscr )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_strDscr;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setDescription",
        /* strAddInfo   */ strAddTrcInfo );

    if( m_strDscr != i_strDscr )
    {
        m_strDscr = i_strDscr;
        m_pObjPool->dataChanged( m_pListEntry->getTreeEntry() );
        emit changed(this);
    }

} // setDescription

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CRemCmdObj::toXmlString() const
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "toXmlString",
        /* strAddInfo   */ "" );

    QString strXml;
    QString strCmd    = m_strCmd;
    QString strSyntax = m_strSyntax;
    QString strDscr   = m_strDscr;

    if( strCmd.contains("\n") )
    {
        strCmd.replace("\n","&lt;br&gt;");
    }
    if( strCmd.contains('<') )
    {
        strCmd.replace("<","&lt;");
    }
    if( strCmd.contains('>') )
    {
        strCmd.replace(">","&gt;");
    }

    if( strSyntax.contains("\n") )
    {
        strSyntax.replace("\n","&lt;br&gt;");
    }
    if( strSyntax.contains('<') )
    {
        strSyntax.replace("<","&lt;");
    }
    if( strSyntax.contains('>') )
    {
        strSyntax.replace(">","&gt;");
    }

    if( strDscr.contains("\n") )
    {
        strDscr.replace("\n","&lt;br&gt;");
    }
    if( strDscr.contains('<') )
    {
        strDscr.replace("<","&lt;");
    }
    if( strDscr.contains('>') )
    {
        strDscr.replace(">","&gt;");
    }

    strXml += "<RemCmdObj>";
    strXml += "<Cmd>" + strCmd + "</Cmd>";
    strXml += "<ObjId>" + QString::number(m_iObjId) + "</ObjId>";

    if( !strSyntax.isEmpty() )
    {
        strXml += "<Syntax>" + strSyntax + "</Syntax>";
    }
    if( m_bIsQueryOnly )
    {
        strXml += "<QueryOnly>" + bool2Str(m_bIsQueryOnly) + "</QueryOnly>";
    }
    if( m_bIsEventOnly )
    {
        strXml += "<EventOnly>" + bool2Str(m_bIsEventOnly) + "</EventOnly>";
    }
    if( !strDscr.isEmpty() )
    {
        strXml += "<Dscr>" + strDscr + "</Dscr>";
    }
    strXml += "</RemCmdObj>";

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        mthTracer.setMethodReturn(strXml);
    }

    return strXml;

} // toXmlString

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdObj::setObjFctExecute( QObject* i_pObjFctExecute )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        if( i_pObjFctExecute == nullptr )
        {
            strAddTrcInfo = "Obj: nullptr";
        }
        else
        {
            strAddTrcInfo = "Obj: " + i_pObjFctExecute->objectName();
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setObjFctExecute",
        /* strAddInfo   */ strAddTrcInfo );

    m_pObjFctExecute = i_pObjFctExecute;

    m_pObjPool->dataChanged( m_pListEntry->getTreeEntry() );

    emit changed(this);

} // setObjFctExecute

//------------------------------------------------------------------------------
void CRemCmdObj::setFctExecute( TFctExecute i_fctExecute )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setFctExecute",
        /* strAddInfo   */ "" );

    m_fctExecute = i_fctExecute;

    m_pObjPool->dataChanged( m_pListEntry->getTreeEntry() );

    emit changed(this);

} // setFctExecute

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CRemCmdObj::executeCommand(
    bool              i_bIsQuery,
    const QByteArray& i_byteArrArgs,
    QByteArray&       o_byteArrResults,
    int               i_iTimeout_ms,
    bool              i_bWait,
    qint64            i_iReqIdParent )
//------------------------------------------------------------------------------
{
    SSocketDscr socketDscr;
    return executeCommand(
        /* socketDscr     */ socketDscr,
        /* bIsQuery       */ i_bIsQuery,
        /* byteArrArgs    */ i_byteArrArgs,
        /* byteArrResults */ o_byteArrResults,
        /* iTimeout_ms    */ i_iTimeout_ms,
        /* bWait          */ i_bWait,
        /* iReqIdParent   */ i_iReqIdParent );
}

//------------------------------------------------------------------------------
CRequest* CRemCmdObj::executeCommand(
    SSocketDscr&      i_socketDscr,
    bool              i_bIsQuery,
    const QByteArray& i_byteArrArgs,
    QByteArray&       o_byteArrResults,
    int               i_iTimeout_ms,
    bool              i_bWait,
    qint64            i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo  = "SocketDscr: " + i_socketDscr.getConnectionString(iAddTrcInfoDetailLevel);
        strAddTrcInfo += ", IsQuery: " + bool2Str(i_bIsQuery);
        strAddTrcInfo += ", Args [" + QString::number(i_byteArrArgs.size()) + "]";
        if( m_pTrcAdminObj->isActive(ETraceDetailLevelInternalStates) )
        {
            strAddTrcInfo += "(" + i_byteArrArgs + ")";
        }
        else if( i_byteArrArgs.size() <= 50 )
        {
            strAddTrcInfo += "(" + i_byteArrArgs + ")";
        }
        else
        {
            strAddTrcInfo += "(" + i_byteArrArgs.left(5) + ".." + i_byteArrArgs.right(5) + ")";
        }
        strAddTrcInfo += ", Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "executeCommand",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("executeCommand");

    QString strAddErrInfo;

    CRequest* pReq = nullptr;

    if( m_fctExecute != nullptr )
    {
        pReq = m_fctExecute(
            /* pObj           */ m_pObjFctExecute,
            /* socketDscr     */ i_socketDscr,
            /* bIsQuery       */ i_bIsQuery,
            /* byteArrArgs    */ i_byteArrArgs,
            /* byteArrResults */ o_byteArrResults,
            /* iReqIdParent   */ i_iReqIdParent );
    }
    else // if( m_fctExecute != nullptr )
    {
        pReq = new CRequest(
            /* strNameSpace */ "ZS::RemCmd",
            /* strClassName */ "CRemCmdObj",
            /* pObjCreator  */ this,
            /* iRequest     */ ERequestCmdExecute,
            /* strRequest   */ request2Str(ERequestCmdExecute),
            /* iReqIdParent */ i_iReqIdParent,
            /* bKeepInTree  */ false,
            /* iTimeout_ms  */ i_iTimeout_ms,
            /* bIsBlocking  */ i_bWait );

        pReq->setDescription(m_strCmd);

        // There must have been a slot method connected to the execute signal of the
        // command object. This slot method cannnot return a request instance and
        // therefore the command is considered to be executed synchronously.
        emit execute(
            /* socketDscr     */ i_socketDscr,
            /* bIsQuery       */ i_bIsQuery,
            /* byteArrArgs    */ i_byteArrArgs,
            /* byteArrResults */ o_byteArrResults,
            /* pReq           */ pReq );
    }

    if( !isAsynchronousRequest(pReq) )
    {
        // Please note that the object creating the request must delete the request.
        if( m_fctExecute != nullptr )
        {
            pReq = nullptr; // deleted later by request queue of command executing object
        }
        else
        {
            // Don't delete the request right now but return the request to the caller
            // as the caller may want to evaluate some details of the executed request.
            delete m_pReqSyncToBeDeletedLater;
            m_pReqSyncToBeDeletedLater = pReq;
        }
    }
    else // if( isAsynchronousRequest(pReq) )
    {
        m_hshpReqsInProgress[pReq->getId()] = pReq;

        if( !QObject::connect(
            /* pObjSender   */ pReq,
            /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( isAsynchronousRequest(pReq) )

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        int iAddTrcInfoDetailLevel = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo ? 1 : 0;
        strAddTrcInfo = "Req {" + pReq->getAddTrcInfoStr(iAddTrcInfoDetailLevel) + "}";
        mthTracer.trace(strAddTrcInfo);
    }

    return pReq;

} // executeCommand

/*==============================================================================
public: // overridables of inherited class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CRemCmdObj::event( QEvent* i_pMsg )
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
            bEventHandled = true;

            SErrResultInfo errResultInfo = ErrResultInfoSuccess(pMsg->msgTypeToStr());

            switch( static_cast<int>(pMsg->type()) )
            {
                case EMsgTypeReqCmdExecute:
                {
                    CMsgReqCmdExecute* pMsgReq = dynamic_cast<CMsgReqCmdExecute*>(pMsg);
                    if( pMsgReq == nullptr )
                    {
                        throw CException(__FILE__,__LINE__,EResultInvalidDynamicTypeCast);
                    }

                    bool        bIsQuery = pMsgReq->isQuery();
                    QByteArray  byteArrArgs = pMsgReq->getArgs();
                    QByteArray  byteArrResults;
                    SSocketDscr socketDscr = pMsgReq->getSocketDscr();

                    CRequest* pReq = executeCommand(
                        /* socketDscr     */ socketDscr,
                        /* bIsQuery       */ bIsQuery,
                        /* byteArrArgs    */ byteArrArgs,
                        /* byteArrResults */ byteArrResults,
                        /* iTimeout_ms    */ 0,
                        /* bWait          */ false,
                        /* iReqIdParent   */ pMsgReq->getRequestId() );

                    if( isAsynchronousRequest(pReq) )
                    {
                        pReq->setInitiatorRequestMessage(dynamic_cast<CMsgReqCmdExecute*>(pMsgReq->clone()));
                    }
                    else
                    {
                        if( pReq != nullptr )
                        {
                            errResultInfo = pReq->getErrResultInfo();
                        }

                        if( pMsgReq->mustBeConfirmed() )
                        {
                            CMsgConCmdExecute* pMsgCon = dynamic_cast<CMsgConCmdExecute*>(pMsgReq->createConfirmationMessage());
                            pMsgCon->setErrResultInfo(errResultInfo);
                            pMsgCon->setResults(byteArrResults);
                            POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
                            pMsgCon = nullptr;
                        }
                    }
                    break;
                } // case EMsgTypeReqCmdExecute:

                default:
                {
                    QString strAddErrInfo = pMsg->getAddTrcInfoStr();

                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInvalidMessage);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);

                    if( CErrLog::GetInstance() != nullptr )
                    {
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                    else
                    {
                        throw CException(__FILE__, __LINE__, errResultInfo);
                    }
                    break;
                }
            } // switch( i_pMsg->type() )
        } // if( pMsg != nullptr )
    } // if( i_pMsg->type() >= QEvent::User )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;

} // event

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdObj::onRequestChanged( ZS::System::SRequestDscr i_reqDscr )
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

    CRequest* pReq = m_hshpReqsInProgress.value(i_reqDscr.m_iId, nullptr);

    if( pReq == nullptr )
    {
        QString strAddErrInfo;
        strAddErrInfo = "Slot method called for request \"" + i_reqDscr.getAddTrcInfoStr(0) + "\" but request is not in request queue.";
        SErrResultInfo errResultInfo = ErrResultInfoError("onRequestChanged", EResultObjNotInList, strAddErrInfo);
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else // if( pReq != nullptr )
    {
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

        // Assign after adding result to error log so that the "AddedTo" flag is taken over.
        SErrResultInfo errResultInfo = i_reqDscr.m_errResultInfo;

        // If the request has been finished ...
        if( i_reqDscr.m_iProgress_perCent >= 100 )
        {
            if( errResultInfo.getResult() == EResultUndefined )
            {
                errResultInfo.setErrResult(ErrResultSuccess);
            }
            m_hshpReqsInProgress.remove(i_reqDscr.m_iId);

            // The request has been finished and will be deleted sometimes later.
            // Further changes (e.g. deleting the request) don't matter me ...
            QObject::disconnect(
                /* pObjSender   */ pReq,
                /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT(onRequestChanged(ZS::System::SRequestDscr)) );

        } // if( i_reqDscr.m_iProgress_perCent >= 100 )

        switch( i_reqDscr.m_request )
        {
            case ERequestCmdExecute:
            {
                break;
            }
            default:
            {
                QString strAddErrInfo = "Don't expect signal for request " + request2Str(i_reqDscr.m_request);
                throw ZS::System::CException( __FILE__, __LINE__, EResultInternalStateMachineError, strAddErrInfo );
                break;
            }
        } // switch( i_reqDscr.m_request )

        // If the request has been finished ...
        if( i_reqDscr.m_iProgress_perCent >= 100 )
        {
            CMsgConCmdExecute* pMsgCon = nullptr;

            // If the initiator expects a confirmation message ...
            if( pReq->getInitiatorConfirmationMessage() != nullptr )
            {
                pMsgCon = dynamic_cast<CMsgConCmdExecute*>(pReq->takeInitiatorConfirmationMessage());
            }
            else if( pReq->getInitiatorRequestMessage() != nullptr )
            {
                CMsgReqCmdExecute* pMsgReq = dynamic_cast<CMsgReqCmdExecute*>(pReq->takeInitiatorRequestMessage());

                if( pMsgReq->mustBeConfirmed() )
                {
                    pMsgCon = dynamic_cast<CMsgConCmdExecute*>(pMsgReq->createConfirmationMessage());
                }
                delete pMsgReq;
                pMsgReq = nullptr;
            }

            if( pMsgCon != nullptr )
            {
                QByteArray byteArrResults = pReq->getResultValue().toByteArray();
                pMsgCon->setProgress(pReq->getProgressInPerCent());
                pMsgCon->setErrResultInfo(errResultInfo);
                pMsgCon->setResults(byteArrResults);
                POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer, ETraceDetailLevelRuntimeInfo);
                pMsgCon = nullptr;
            }

            // The request created by this command object has been finished.
            // The owner of the request instance got to delete the request.
            // But not immediately now as after this "onRequestChanged" slot
            // further "onRequestChanged" slots of other classes may be called.
            // Because of this the request will be set at the request queue
            // to be deleted later on destroying the command object or if the
            // next request is set to be deleted later.
            if( !pReq->isLocked() )
            {
                delete m_pReqSyncToBeDeletedLater;
                m_pReqSyncToBeDeletedLater = pReq;
            }
        } // if( i_reqDscr.m_iProgress_perCent >= 100 && bIsMainRequest )
    } // if( m_hshpReqsInProgress->isRequestInProgress(i_reqDscr.m_iId) )

} // onRequestChanged
