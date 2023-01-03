/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSIpcTrace/ZSIpcTrcClient.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequest.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;


/*******************************************************************************
class CIpcTrcClient : public CClient
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the trace client.

    @param i_strName [in] Name of the client.
    @param i_eTrcMthFileDetailLevel [in]
        If the methods of the trace client itself should be logged a value
        greater than 0 (EMethodTraceDetailLevel::None) could be passed here.
    @param i_eTrcMthFileDetailLevelMutex [in]
        If the locking and unlocking of the mutex of trace client
        should be logged a value greater than 0 (EMethodTraceDetailLevel::None)
        could be passed here. But the value will be ignored if the detail
        level for the client tracer is None.
    @param i_eTrcMthFileDetailLevelGateway [in]
        If the methods of the clients gateway should be logged a value greater
        than 0 (EMethodTraceDetailLevel::None) could be passed here.
*/
CIpcTrcClient::CIpcTrcClient(
    const QString& i_strName,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevel,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevelMutex,
    EMethodTraceDetailLevel i_eTrcMthFileDetailLevelGateway ) :
//------------------------------------------------------------------------------
    CClient(
        /* strObjName                    */ i_strName,
        /* bMultiThreadedAccess          */ false,
        /* iTrcMthFileDetailLevel        */ i_eTrcMthFileDetailLevel,
        /* iTrcMthFileDetailLevelGateway */ i_eTrcMthFileDetailLevelGateway ),
    m_strRemoteApplicationName(),
    m_strRemoteServerName(),
    m_trcServerSettings(),
    m_pTrcAdminObjIdxTree(nullptr),
    m_bOnReceivedDataUpdateInProcess(false)
{
    if( !i_strName.endsWith("TrcClient") )
    {
        // The object name is used within the Ipc Subsystem to decide whether tracing
        // through the trace server can be used to avoid deadlocks and endless recursions
        // (e.g. tracing a method call within the trace client would send a message
        // through the trace server to the trace client which again would lead to tracing
        // a method and sending a message to the client and so on ...
        throw ZS::System::CException(__FILE__, __LINE__, EResultArgOutOfRange, "Name of trace client must end with TrcClient");
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );

    m_watchDogTimerSettings.m_bEnabled = false;

    m_pTrcAdminObjIdxTree = new CIdxTreeTrcAdminObjs("TrcAdminObjs-" + i_strName, this);

    // Set default ports depending on trace types
    //-------------------------------------------

    m_hostSettings.m_uRemotePort = 24763;

    // Watch dog timer not used for the trace method client.
    m_bWatchDogTimerUsed = false;

    // Connect to signals of the Ipc client
    //-------------------------------------

    // On connecting the trace admin objects got to be selected.
    if( !QObject::connect(
        /* pObjSender   */ this,
        /* szSignal     */ SIGNAL( connected(QObject*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onIpcClientConnected(QObject*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ this,
        /* szSignal     */ SIGNAL( disconnected(QObject*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onIpcClientDisconnected(QObject*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Connect to the admin object signals of the object pool
    //-------------------------------------------------------

    QObject::connect(
        m_pTrcAdminObjIdxTree, &CIdxTreeTrcAdminObjs::treeEntryChanged,
        this, &CIpcTrcClient::onTrcAdminObjIdxTreeEntryChanged );

} // ctor

//------------------------------------------------------------------------------
CIpcTrcClient::~CIpcTrcClient()
//------------------------------------------------------------------------------
{
    // The method tracer to trace method enter and method leave cannot be used here.
    // The trace method file will be destroyed before method leave is traced.
    // As a workaround the method tracers scope is left before the trace method
    // file is closed and freed.

    {   CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMthInArgs       */ "" );

        QObject::disconnect(
            m_pTrcAdminObjIdxTree, &CIdxTreeTrcAdminObjs::treeEntryChanged,
            this, &CIpcTrcClient::onTrcAdminObjIdxTreeEntryChanged );

        abortAllRequests(); // Deletes or at least invalidates the current request in progress.

        // Disconnect client before destroying the trace admin object tree.
        stopGatewayThread();

        // To avoid err log entry: The dtor is called even if the objects reference counter is not 0.
        resetTrcAdminRefCounters(m_pTrcAdminObjIdxTree->root());

        try
        {
            delete m_pTrcAdminObjIdxTree;
        }
        catch(...)
        {
        }
    } // CMethodTracer mthTracer(

    //m_strRemoteApplicationName;
    //m_strRemoteServerName;
    //m_trcServerSettings;
    m_pTrcAdminObjIdxTree = nullptr;
    m_bOnReceivedDataUpdateInProcess = false;

} // dtor

/*==============================================================================
public: // overridables of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CIpcTrcClient::connect_( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
        strAddTrcInfo += ", Wait: " + bool2Str(i_bWait);
        strAddTrcInfo += ", ParentRequest {" + CRequestExecTree::GetAddTrcInfoStr(i_iReqIdParent) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj          */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "connect_",
        /* strAddInfo         */ strAddTrcInfo );

    // Before the index tree can be cleared the ref counters of the trace admin objects
    // got to be reset to avoid an err log entry (object ref counter is not 0 in dtor).

    m_strRemoteApplicationName = "";
    m_strRemoteServerName = "";

    // To avoid err log entry: The dtor is called even if the objects reference counter is not 0.
    resetTrcAdminRefCounters(m_pTrcAdminObjIdxTree->root());

    m_pTrcAdminObjIdxTree->clear();

    CRequest* pReq = CClient::connect_(i_iTimeout_ms, i_bWait, i_iReqIdParent);

    if( areTraceMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
        strAddTrcInfo = QString( pReq == nullptr ? "SUCCESS" : pReq->getResultStr() );
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return pReq;
}

/*==============================================================================
public: // instance methods to read remote application settings
==============================================================================*/

//------------------------------------------------------------------------------
QString CIpcTrcClient::getRemoteApplicationName() const
//------------------------------------------------------------------------------
{
    return m_strRemoteApplicationName;
}

//------------------------------------------------------------------------------
QString CIpcTrcClient::getRemoteServerName() const
//------------------------------------------------------------------------------
{
    return m_strRemoteServerName;
}

/*==============================================================================
public: // instance methods to enable and disable the client and server
==============================================================================*/

//------------------------------------------------------------------------------
STrcServerSettings CIpcTrcClient::getTraceSettings() const
//------------------------------------------------------------------------------
{
    return m_trcServerSettings;
}

//------------------------------------------------------------------------------
void CIpcTrcClient::setTraceSettings( const STrcServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_settings.toString();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setTraceSettings",
        /* strMthInArgs       */ strMthInArgs );

    if( m_trcServerSettings != i_settings )
    {
        // If not called on receiving trace settings from the server and if the client is connected ..
        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings";

            if( m_trcServerSettings.m_bEnabled != i_settings.m_bEnabled )
            {
                strMsg += " Enabled=\"" + bool2Str(i_settings.m_bEnabled) + "\"";
            }
            if( m_trcServerSettings.m_bNewTrcAdminObjsEnabledAsDefault != i_settings.m_bNewTrcAdminObjsEnabledAsDefault )
            {
                strMsg += " NewTrcAdminObjsEnabledAsDefault=\"" + bool2Str(i_settings.m_bNewTrcAdminObjsEnabledAsDefault) + "\"";
            }
            if( m_trcServerSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel != i_settings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel )
            {
                strMsg += " NewTrcAdminObjsMethodCallsDefaultDetailLevel=\""
                       + CEnumMethodTraceDetailLevel(i_settings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel).toString() + "\"";
            }
            if( m_trcServerSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel != i_settings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel )
            {
                strMsg += " NewTrcAdminObjsRuntimeInfoDefaultDetailLevel=\""
                       + CEnumLogDetailLevel(i_settings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel).toString() + "\"";
            }
            if( m_trcServerSettings.m_bUseIpcServer != i_settings.m_bUseIpcServer )
            {
                strMsg += " UseIpcServer=\"" + bool2Str(i_settings.m_bUseIpcServer) + "\"";
            }
            if( m_trcServerSettings.m_bCacheDataIfNotConnected != i_settings.m_bCacheDataIfNotConnected )
            {
                strMsg += " CacheDataIfNotConnected=\"" + bool2Str(i_settings.m_bCacheDataIfNotConnected) + "\"";
            }
            if( m_trcServerSettings.m_iCacheDataMaxArrLen != i_settings.m_iCacheDataMaxArrLen )
            {
                strMsg += " CacheDataMaxArrLen =\"" + QString::number(i_settings.m_iCacheDataMaxArrLen) + "\"";
            }
            if( m_trcServerSettings.m_bUseLocalTrcFile != i_settings.m_bUseLocalTrcFile )
            {
                strMsg += " UseLocalTrcFile=\"" + bool2Str(i_settings.m_bUseLocalTrcFile) + "\"";
            }
            if( m_trcServerSettings.m_iLocalTrcFileAutoSaveInterval_ms != i_settings.m_iLocalTrcFileAutoSaveInterval_ms )
            {
                strMsg += " LocalTrcFileAutoSaveInterval_ms=\"" + QString::number(i_settings.m_iLocalTrcFileAutoSaveInterval_ms) + "\"";
            }
            if( m_trcServerSettings.m_iLocalTrcFileSubFileCountMax != i_settings.m_iLocalTrcFileSubFileCountMax )
            {
                strMsg += " LocalTrcFileSubFileCountMax=\"" + QString::number(i_settings.m_iLocalTrcFileSubFileCountMax) + "\"";
            }
            if( m_trcServerSettings.m_iLocalTrcFileSubFileLineCountMax != i_settings.m_iLocalTrcFileSubFileLineCountMax )
            {
                strMsg += " LocalTrcFileSubFileLineCountMax=\"" + QString::number(i_settings.m_iLocalTrcFileSubFileLineCountMax) + "\"";
            }
            if( m_trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite != i_settings.m_bLocalTrcFileCloseFileAfterEachWrite )
            {
                strMsg += " LocalTrcFileCloseAfterEachWrite=\"" + bool2Str(i_settings.m_bLocalTrcFileCloseFileAfterEachWrite) + "\"";
            }

            strMsg += "/>";

            CRequest* pReq = sendData( str2ByteArr(strMsg) );

            if( !isAsynchronousRequest(pReq) )
            {
                pReq = nullptr; // deleted later by request queue
            }
        } // if( !m_bOnReceivedDataUpdateInProcess && isConnected() )

        m_trcServerSettings = i_settings;

        // If not called on receiving trace settings from the server ..
        if( !m_bOnReceivedDataUpdateInProcess )
        {
            emit traceSettingsChanged(this);
        }
    }
} // setTraceSettings

/*==============================================================================
protected: // instance methods to send admin objects to the connected server
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcClient::sendAdminObj(
    MsgProtocol::TSystemMsgType i_systemMsgType,
    MsgProtocol::TCommand       i_cmd,
    CTrcAdminObj*               i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "MsgType: " + systemMsgType2Str(i_systemMsgType);
        strMthInArgs += ", Cmd: " + command2Str(i_cmd);
        strMthInArgs += ", AdmObj: " + i_pTrcAdminObj->keyInTree();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sendAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    if( i_pTrcAdminObj != nullptr && isConnected() )
    {
        QString strMsg;

        strMsg += systemMsgType2Str(i_systemMsgType) + " ";
        strMsg += command2Str(i_cmd) + " ";
        strMsg += "<TrcAdminObj ";
        strMsg += " ObjId=\"" + QString::number(i_pTrcAdminObj->indexInTree()) + "\"";
        strMsg += " Enabled=\"" + CEnumEnabled::toString(i_pTrcAdminObj->getEnabled()) + "\"";
        strMsg += " MethodCallsDetailLevel=\"" + CEnumMethodTraceDetailLevel(i_pTrcAdminObj->getMethodCallsTraceDetailLevel()).toString() + "\"";
        strMsg += " RuntimeInfoDetailLevel=\"" + CEnumLogDetailLevel(i_pTrcAdminObj->getRuntimeInfoTraceDetailLevel()).toString() + "\"";
        strMsg += " DataFilter=\"" + i_pTrcAdminObj->getTraceDataFilter() + "\"";
        strMsg += "/>";

        sendData( str2ByteArr(strMsg) );

    } // if( i_pTrcAdminObj != nullptr && isConnected() )

} // sendAdminObj

//------------------------------------------------------------------------------
void CIpcTrcClient::sendNameSpace(
    MsgProtocol::TSystemMsgType  i_systemMsgType,
    MsgProtocol::TCommand        i_cmd,
    CIdxTreeEntry*               i_pBranch,
    EEnabled                     i_enabled,
    EMethodTraceDetailLevel i_eDetailLevelMethodCalls,
    ELogDetailLevel i_eDetailLevelRuntimeInfo,
    const QString&               i_strDataFilter )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "MsgType: " + systemMsgType2Str(i_systemMsgType);
        strMthInArgs += ", Cmd: " + command2Str(i_cmd);
        strMthInArgs += ", Branch: " + i_pBranch->keyInTree();
        strMthInArgs += ", Enabled: " + CEnumEnabled(i_enabled).toString();
        strMthInArgs += ", DetailLevelMethodCalls: " + CEnumMethodTraceDetailLevel(i_eDetailLevelMethodCalls).toString();
        strMthInArgs += ", DetailLevelRuntimeInfo: " + CEnumLogDetailLevel(i_eDetailLevelRuntimeInfo).toString();
        strMthInArgs += ", DataFilter: " + i_strDataFilter;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "sendNameSpace",
        /* strMthInArgs       */ strMthInArgs );

    if( i_pBranch != nullptr && isConnected() )
    {
        QString strMsg;

        strMsg += systemMsgType2Str(i_systemMsgType) + " ";
        strMsg += command2Str(i_cmd) + " ";

        strMsg += "<NameSpace ";
        strMsg += " ObjId=" + QString::number(i_pBranch->indexInTree());

        if( static_cast<int>(i_enabled) >= 0 && static_cast<int>(i_enabled) < CEnumEnabled::count() )
        {
            strMsg += " Enabled=\"" + CEnumEnabled::toString(i_enabled) + "\"";
        }
        if( i_eDetailLevelMethodCalls != EMethodTraceDetailLevel::Undefined )
        {
            strMsg += " MethodCallsDetailLevel=\"" + CEnumMethodTraceDetailLevel(i_eDetailLevelMethodCalls).toString() + "\"";
        }
        if( i_eDetailLevelRuntimeInfo != ELogDetailLevel::Undefined )
        {
            strMsg += " MethodCallsDetailLevel=\"" + CEnumLogDetailLevel(i_eDetailLevelRuntimeInfo).toString() + "\"";
        }
        if( !i_strDataFilter.isNull() )
        {
            strMsg += " DataFilter=\"" + i_strDataFilter + "\"";
        }
        strMsg += "/>";

        sendData( str2ByteArr(strMsg) );

    } // if( i_pBranch != nullptr && isConnected() )

} // sendNameSpace

//------------------------------------------------------------------------------
void CIpcTrcClient::onReceivedData( const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs += "[" + QString::number(i_byteArr.size()) + "]";
        if( m_eTrcMthFileDetailLevel < EMethodTraceDetailLevel::ArgsVerbose ) {
            strMthInArgs += "(" + truncateStringWithEllipsisInTheMiddle(byteArr2Str(i_byteArr), 30) + ")";
        } else {
            strMthInArgs += "(" + truncateStringWithEllipsisInTheMiddle(byteArr2Str(i_byteArr), 100) + ")";
        }
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onReceivedData",
        /* strMthInArgs       */ strMthInArgs );

    QString                     str = byteArr2Str(i_byteArr);
    const QChar*                pcData = str.data();
    int                         iDataLen = str.size();
    int                         iSection = 0;
    QString                     strSystemMsgTypeOfData;
    MsgProtocol::TSystemMsgType systemMsgTypeOfData;
    QString                     strCmd;
    MsgProtocol::TCommand       cmd;

    strSystemMsgTypeOfData = str.section(" ",iSection,iSection);
    systemMsgTypeOfData = str2SystemMsgType(strSystemMsgTypeOfData);
    if( systemMsgTypeOfData != MsgProtocol::ESystemMsgTypeUndefined )
    {
        pcData += (strSystemMsgTypeOfData.length() + 1);
        iDataLen -= (strSystemMsgTypeOfData.length() + 1);
        iSection++;
    }

    strCmd = str.section(" ",iSection,iSection);
    cmd = str2Command(strCmd);
    if( cmd != MsgProtocol::ECommandUndefined )
    {
        pcData += (strCmd.length() + 1);
        iDataLen -= (strCmd.length() + 1);
        iSection++;
    }

    QString strData(pcData,iDataLen);

    if( systemMsgTypeOfData == MsgProtocol::ESystemMsgTypeCon || systemMsgTypeOfData == MsgProtocol::ESystemMsgTypeInd )
    {
        QXmlStreamReader xmlStreamReader(strData);

        QXmlStreamReader::TokenType xmlStreamTokenType;

        QString strElemName;
        QString strDataSnd;
        QString strAttr;
        bool    bVal;
        int     iVal;
        bool    bOk;

        xmlStreamTokenType = xmlStreamReader.readNext();

        if( xmlStreamTokenType != QXmlStreamReader::StartDocument )
        {
            xmlStreamReader.raiseError("Invalid XML command");
        }
        else // if( xmlStreamTokenType == QXmlStreamReader::StartDocument )
        {
            bool bInTrcDataBlock = false;

            while( xmlStreamTokenType != QXmlStreamReader::EndElement && xmlStreamTokenType != QXmlStreamReader::Invalid )
            {
                xmlStreamTokenType = xmlStreamReader.readNext();

                if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
                {
                    strElemName = xmlStreamReader.name().toString();

                    if( strElemName == "TrcData" )
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            bInTrcDataBlock = true;
                            emit traceDataReceived(this, strData);
                        }
                        else if( xmlStreamReader.isEndElement() )
                        {
                            bInTrcDataBlock = false;
                        }
                    }

                    else if( strElemName == "Branch" )
                    {
                        if( !bInTrcDataBlock && xmlStreamReader.isStartElement() )
                        {
                            int      iParentPranchIdxInTree = -1;
                            QString  strBranchName;
                            int      idxInTree = -1;
                            EEnabled enabled = EEnabled::Undefined;
                            bool     bSetDataFilter = false;
                            QString  strDataFilter;
                            EMethodTraceDetailLevel eDetailLevelMethodCalls = EMethodTraceDetailLevel::Undefined;
                            ELogDetailLevel eDetailLevelRuntimeInfo = ELogDetailLevel::Undefined;

                            if( !xmlStreamReader.attributes().hasAttribute("IdxInTree") )
                            {
                                xmlStreamReader.raiseError("Attribute \"IdxInTree\" is missing");
                            }
                            else // if( xmlStreamReader.attributes().hasAttribute("IdxInTree") )
                            {
                                strAttr = xmlStreamReader.attributes().value("IdxInTree").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk && iVal >= 0 ) idxInTree = iVal;
                                else xmlStreamReader.raiseError("Attribute \"IdxInTree\" (" + strAttr + ") is out of range");
                            }

                            if( idxInTree >= 0 )
                            {
                                if( xmlStreamReader.attributes().hasAttribute("ParentBranchIdxInTree") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("ParentBranchIdxInTree").toString();
                                    iVal = strAttr.toInt(&bOk);
                                    if( bOk ) iParentPranchIdxInTree = iVal;
                                    else xmlStreamReader.raiseError("Attribute \"ParentBranchIdxInTree\" (" + strAttr + ") is out of range");
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Name") )
                                {
                                    strBranchName = xmlStreamReader.attributes().value("Name").toString();
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Enabled") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                                    enabled = CEnumEnabled::toEnumerator(strAttr);
                                    if( enabled == EEnabled::Undefined ) xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") is out of range");
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("MethodCallsDetailLevel") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("MethodCallsDetailLevel").toString();
                                    eDetailLevelMethodCalls = CEnumMethodTraceDetailLevel::toEnumerator(strAttr);
                                    if( eDetailLevelMethodCalls == EMethodTraceDetailLevel::Undefined )
                                    {
                                        xmlStreamReader.raiseError("Attribute \"MethodCallsDetailLevel\" (" + strAttr + ") is out of range");
                                    }
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("RuntimeInfoDetailLevel") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("RuntimeInfoDetailLevel").toString();
                                    eDetailLevelRuntimeInfo = CEnumLogDetailLevel::toEnumerator(strAttr);
                                    if( eDetailLevelRuntimeInfo == ELogDetailLevel::Undefined )
                                    {
                                        xmlStreamReader.raiseError("Attribute \"RuntimeInfoDetailLevel\" (" + strAttr + ") is out of range");
                                    }
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("DataFilter") )
                                {
                                    strDataFilter = xmlStreamReader.attributes().value("DataFilter").toString();
                                    bSetDataFilter = true;
                                }

                                if( !xmlStreamReader.hasError() )
                                {
                                    m_bOnReceivedDataUpdateInProcess = true;

                                    CIdxTreeEntry* pBranch = m_pTrcAdminObjIdxTree->getEntry(idxInTree);

                                    // Confirmation to select command.
                                    if( cmd == MsgProtocol::ECommandSelect )
                                    {
                                        if( pBranch == nullptr )
                                        {
                                            if( strBranchName.isEmpty() )
                                            {
                                                xmlStreamReader.raiseError("No branch at tree index " + QString::number(idxInTree) + " and no branch name provided.");
                                            }
                                            else
                                            {
                                                pBranch = m_pTrcAdminObjIdxTree->insertBranch(iParentPranchIdxInTree, strBranchName, idxInTree);
                                            }
                                        }
                                        else // if( pBranch != nullptr )
                                        {
                                            if( !strBranchName.isEmpty() && strBranchName.compare(pBranch->name(), Qt::CaseInsensitive) != 0 )
                                            {
                                                xmlStreamReader.raiseError("Update branch command received for tree index " + QString::number(idxInTree) +
                                                    " but provided branch name " + strBranchName + " is different from existing name of branch " + pBranch->name());
                                            }
                                        }
                                    }
                                    // Indication that object has been changed.
                                    else if( cmd == MsgProtocol::ECommandUpdate )
                                    {
                                        if( pBranch == nullptr )
                                        {
                                            xmlStreamReader.raiseError("Update branch command received but there is no branch at tree index " + QString::number(idxInTree));
                                        }
                                    }
                                    // Indication that object has been inserted.
                                    else if( cmd == MsgProtocol::ECommandInsert )
                                    {
                                        if( pBranch != nullptr )
                                        {
                                            xmlStreamReader.raiseError("Insert branch command received but there is already a branch at tree index " + QString::number(idxInTree));
                                        }
                                        else if( strBranchName.isEmpty() )
                                        {
                                            xmlStreamReader.raiseError("Insert branch command received but no branch name provided");
                                        }
                                        else // if( pBranch == nullptr && !strBranchName.isEmpty() )
                                        {
                                            pBranch = m_pTrcAdminObjIdxTree->insertBranch(iParentPranchIdxInTree, strBranchName, idxInTree);
                                        }
                                    }
                                    // Indication that object has been deleted.
                                    else if( cmd == MsgProtocol::ECommandDelete )
                                    {
                                        if( pBranch == nullptr )
                                        {
                                            xmlStreamReader.raiseError("Delete branch command received but there is no branch at tree index " + QString::number(idxInTree));
                                        }
                                        else
                                        {
                                            delete pBranch;
                                            pBranch = nullptr;
                                        }
                                    }

                                    if( !xmlStreamReader.hasError() && pBranch != nullptr )
                                    {
                                        if( enabled != EEnabled::Undefined ) {
                                            m_pTrcAdminObjIdxTree->setEnabled(pBranch, enabled);
                                        }
                                        if( eDetailLevelMethodCalls != EMethodTraceDetailLevel::Undefined ) {
                                            m_pTrcAdminObjIdxTree->setMethodCallsTraceDetailLevel(pBranch, eDetailLevelMethodCalls);
                                        }
                                        if( eDetailLevelRuntimeInfo != ELogDetailLevel::Undefined ) {
                                            m_pTrcAdminObjIdxTree->setRuntimeInfoTraceDetailLevel(pBranch, eDetailLevelRuntimeInfo);
                                        }
                                        if( bSetDataFilter ) {
                                            m_pTrcAdminObjIdxTree->setTraceDataFilter(pBranch, strDataFilter);
                                        }
                                    }
                                    m_bOnReceivedDataUpdateInProcess = false;
                                }
                            } // if( idxInTree >= 0 )
                        } // if( !bInTrcDataBlock && xmlStreamReader.isStartElement() )
                    } // if( strElemName == "Branch" )

                    else if( strElemName == "TrcAdminObj" )
                    {
                        if( !bInTrcDataBlock && xmlStreamReader.isStartElement() )
                        {
                            int      iParentPranchIdxInTree = -1;
                            QString  strNameSpace;
                            QString  strClassName;
                            QString  strObjName;
                            int      idxInTree = -1;
                            QString  strThreadName;
                            EEnabled enabled = EEnabled::Undefined;
                            int      iRefCount = -1;
                            bool     bSetDataFilter = false;
                            QString  strDataFilter;

                            EMethodTraceDetailLevel eDetailLevelMethodCalls = EMethodTraceDetailLevel::Undefined;
                            ELogDetailLevel eDetailLevelRuntimeInfo = ELogDetailLevel::Undefined;

                            if( !xmlStreamReader.attributes().hasAttribute("IdxInTree") )
                            {
                                xmlStreamReader.raiseError("Attribute \"IdxInTree\" is missing");
                            }
                            else // if( xmlStreamReader.attributes().hasAttribute("IdxInTree") )
                            {
                                strAttr = xmlStreamReader.attributes().value("IdxInTree").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk && iVal >= 0 ) idxInTree = iVal;
                                else xmlStreamReader.raiseError("Attribute \"IdxInTree\" (" + strAttr + ") is out of range");
                            }

                            if( idxInTree >= 0 )
                            {
                                if( xmlStreamReader.attributes().hasAttribute("ParentBranchIdxInTree") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("ParentBranchIdxInTree").toString();
                                    iVal = strAttr.toInt(&bOk);
                                    if( bOk || iVal < 0 ) iParentPranchIdxInTree = iVal;
                                    else xmlStreamReader.raiseError("Attribute \"ParentBranchIdxInTree\" (" + strAttr + ") is out of range");
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("NameSpace") )
                                {
                                    strNameSpace = xmlStreamReader.attributes().value("NameSpace").toString();
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("ClassName") )
                                {
                                    strClassName = xmlStreamReader.attributes().value("ClassName").toString();
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("ObjName") )
                                {
                                    strObjName = xmlStreamReader.attributes().value("ObjName").toString();
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Thread") )
                                {
                                    strThreadName = xmlStreamReader.attributes().value("Thread").toString();
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Enabled") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                                    enabled = CEnumEnabled::toEnumerator(strAttr);
                                    if( enabled == EEnabled::Undefined ) xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") is out of range");
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("MethodCallsDetailLevel") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("MethodCallsDetailLevel").toString();
                                    eDetailLevelMethodCalls = CEnumMethodTraceDetailLevel::toEnumerator(strAttr);
                                    if( eDetailLevelMethodCalls == EMethodTraceDetailLevel::Undefined )
                                    {
                                        xmlStreamReader.raiseError("Attribute \"MethodCallsDetailLevel\" (" + strAttr + ") is out of range");
                                    }
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("RuntimeInfoDetailLevel") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("RuntimeInfoDetailLevel").toString();
                                    eDetailLevelRuntimeInfo = CEnumLogDetailLevel::toEnumerator(strAttr);
                                    if( eDetailLevelRuntimeInfo == ELogDetailLevel::Undefined )
                                    {
                                        xmlStreamReader.raiseError("Attribute \"RuntimeInfoDetailLevel\" (" + strAttr + ") is out of range");
                                    }
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("DataFilter") )
                                {
                                    strDataFilter = xmlStreamReader.attributes().value("DataFilter").toString();
                                    bSetDataFilter = true;
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("RefCount") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("RefCount").toString();
                                    iVal = strAttr.toInt(&bOk);
                                    if( bOk && iVal >= 0 ) iRefCount = iVal;
                                    else xmlStreamReader.raiseError("Attribute \"RefCount\" (" + strAttr + ") is out of range");
                                }

                                if( !xmlStreamReader.hasError() )
                                {
                                    m_bOnReceivedDataUpdateInProcess = true;

                                    CTrcAdminObj* pTrcAdminObj = m_pTrcAdminObjIdxTree->getTraceAdminObj(idxInTree, false);

                                    bool bTrcAdminObjAlreadyExisting = (pTrcAdminObj != nullptr);

                                    // Confirmation to select command.
                                    if( cmd == MsgProtocol::ECommandSelect )
                                    {
                                        if( pTrcAdminObj == nullptr )
                                        {
                                            if( strNameSpace.isEmpty() && strClassName.isEmpty() && strObjName.isEmpty() )
                                            {
                                                xmlStreamReader.raiseError("No trace admin object at tree index " + QString::number(idxInTree) + " but neither NameSpace nor ClassName nor ObjName provided.");
                                            }
                                            else
                                            {
                                                pTrcAdminObj = m_pTrcAdminObjIdxTree->insertTraceAdminObj(iParentPranchIdxInTree, strNameSpace, strClassName, strObjName, idxInTree);
                                            }
                                        }
                                        else // if( pBranch != nullptr )
                                        {
                                            if( !strNameSpace.isEmpty() && strNameSpace.compare(pTrcAdminObj->getNameSpace(), Qt::CaseInsensitive) != 0 )
                                            {
                                                xmlStreamReader.raiseError("Update trace admin object command received for tree index " + QString::number(idxInTree) +
                                                    " but provided name space " + strNameSpace + " is different from existing name space " + pTrcAdminObj->getNameSpace());
                                            }
                                            else if( !strClassName.isEmpty() && strClassName.compare(pTrcAdminObj->getClassName(), Qt::CaseInsensitive) != 0 )
                                            {
                                                xmlStreamReader.raiseError("Update trace admin object command received for tree index " + QString::number(idxInTree) +
                                                    " but provided class name " + strClassName + " is different from existing class name " + pTrcAdminObj->getClassName());
                                            }
                                            else if( !strObjName.isEmpty() && strObjName.compare(pTrcAdminObj->getObjectName(), Qt::CaseInsensitive) != 0 )
                                            {
                                                xmlStreamReader.raiseError("Update trace admin object command received for tree index " + QString::number(idxInTree) +
                                                    " but provided object name " + strObjName + " is different from existing object name " + pTrcAdminObj->getObjectName());
                                            }
                                        }
                                    }
                                    else if( cmd == MsgProtocol::ECommandUpdate )
                                    {
                                        if( pTrcAdminObj == nullptr )
                                        {
                                            xmlStreamReader.raiseError("Update trace admin object command received but there is no object entry at tree index " + QString::number(idxInTree));
                                        }
                                    }
                                    // Indication that object has been inserted.
                                    else if( cmd == MsgProtocol::ECommandInsert )
                                    {
                                        if( pTrcAdminObj != nullptr )
                                        {
                                            xmlStreamReader.raiseError("Insert trace admin object command received but there is already an object entry at tree index " + QString::number(idxInTree));
                                        }
                                        else if( strNameSpace.isEmpty() && strClassName.isEmpty() && strObjName.isEmpty() )
                                        {
                                            xmlStreamReader.raiseError("Insert trace admin object command received but neither NameSpace nor ClassName nor ObjName provided");
                                        }
                                        else // if( pTrcAdminObj == nullptr && !(strNameSpace.isEmpty() || .. )
                                        {
                                            pTrcAdminObj = m_pTrcAdminObjIdxTree->insertTraceAdminObj(iParentPranchIdxInTree, strNameSpace, strClassName, strObjName, idxInTree);
                                        }
                                    }
                                    // Indication that object has been deleted.
                                    else if( cmd == MsgProtocol::ECommandDelete )
                                    {
                                        if( pTrcAdminObj == nullptr )
                                        {
                                            xmlStreamReader.raiseError("Delete trace admin object command received but there is no object entry at tree index " + QString::number(idxInTree));
                                        }
                                        else
                                        {
                                            delete dynamic_cast<CIdxTreeEntry*>(pTrcAdminObj);
                                            pTrcAdminObj = nullptr;
                                        }
                                    }

                                    if( !xmlStreamReader.hasError() && pTrcAdminObj != nullptr )
                                    {
                                        bool bSignalsBlocked = pTrcAdminObj->blockTreeEntryChangedSignal(true);

                                        if( !strThreadName.isEmpty() ) {
                                            pTrcAdminObj->setObjectThreadName(strThreadName);
                                        }
                                        if( enabled != EEnabled::Undefined ) {
                                            pTrcAdminObj->setEnabled(enabled);
                                        }
                                        if( eDetailLevelMethodCalls != EMethodTraceDetailLevel::Undefined ) {
                                            pTrcAdminObj->setMethodCallsTraceDetailLevel(eDetailLevelMethodCalls);
                                        }
                                        if( eDetailLevelRuntimeInfo != ELogDetailLevel::Undefined ) {
                                            pTrcAdminObj->setRuntimeInfoTraceDetailLevel(eDetailLevelRuntimeInfo);
                                        }
                                        if( bSetDataFilter ) {
                                            pTrcAdminObj->setTraceDataFilter(strDataFilter);
                                        }
                                        if( iRefCount >= 0 ) {
                                            pTrcAdminObj->setRefCount(iRefCount);
                                        }

                                        pTrcAdminObj->blockTreeEntryChangedSignal(bSignalsBlocked);

                                        if( !bTrcAdminObjAlreadyExisting )
                                        {
                                            emit traceAdminObjInserted(this, pTrcAdminObj->keyInTree());
                                        }
                                    }
                                    m_bOnReceivedDataUpdateInProcess = false;

                                } // if( !xmlStreamReader.hasError() )
                            } // if( idxInTree >= 0 )
                        } // if( !bInTrcDataBlock && xmlStreamReader.isStartElement() )
                    } // if( strElemName == "TrcAdminObj" )

                    else if( strElemName == "ServerSettings" )
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            bool bRemoteNameChanged = false;

                            if( xmlStreamReader.attributes().hasAttribute("ApplicationName") )
                            {
                                strAttr = xmlStreamReader.attributes().value("ApplicationName").toString();
                                if( m_strRemoteApplicationName != strAttr )
                                {
                                    m_strRemoteApplicationName = strAttr;
                                    bRemoteNameChanged = true;
                                }
                            }
                            if( xmlStreamReader.attributes().hasAttribute("ServerName") )
                            {
                                strAttr = xmlStreamReader.attributes().value("ServerName").toString();
                                if( m_strRemoteServerName != strAttr )
                                {
                                    m_strRemoteServerName = strAttr;
                                    bRemoteNameChanged = true;
                                }
                            }

                            STrcServerSettings trcServerSettings = m_trcServerSettings;

                            if( xmlStreamReader.attributes().hasAttribute("Enabled") )
                            {
                                strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                                bVal = str2Bool(strAttr, &bOk);
                                if( bOk ) trcServerSettings.m_bEnabled = bVal;
                                else xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("AdminObjFileAbsFilePath") )
                            {
                                strAttr = xmlStreamReader.attributes().value("AdminObjFileAbsFilePath").toString();
                                trcServerSettings.m_strAdminObjFileAbsFilePath = strAttr;
                            }
                            if( xmlStreamReader.attributes().hasAttribute("NewTrcAdminObjsEnabledAsDefault") )
                            {
                                strAttr = xmlStreamReader.attributes().value("NewTrcAdminObjsEnabledAsDefault").toString();
                                bVal = str2Bool(strAttr, &bOk);
                                if( bOk ) trcServerSettings.m_bNewTrcAdminObjsEnabledAsDefault = bVal;
                                else xmlStreamReader.raiseError("Attribute \"NewTrcAdminObjsEnabledAsDefault\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("NewTrcAdminObjsMethodCallsDefaultDetailLevel") )
                            {
                                strAttr = xmlStreamReader.attributes().value("NewTrcAdminObjsMethodCallsDefaultDetailLevel").toString();
                                CEnumMethodTraceDetailLevel eDetailLevel = CEnumMethodTraceDetailLevel::fromString(strAttr);
                                if( eDetailLevel.enumerator() != EMethodTraceDetailLevel::Undefined ) trcServerSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel = eDetailLevel.enumerator();
                                else xmlStreamReader.raiseError("Attribute \"NewTrcAdminObjsMethodCallsDefaultDetailLevel\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("NewTrcAdminObjsRuntimeInfoDefaultDetailLevel") )
                            {
                                strAttr = xmlStreamReader.attributes().value("NewTrcAdminObjsRuntimeInfoDefaultDetailLevel").toString();
                                CEnumLogDetailLevel eDetailLevel = CEnumLogDetailLevel::fromString(strAttr);
                                if( eDetailLevel.enumerator() != ELogDetailLevel::Undefined ) trcServerSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = eDetailLevel.enumerator();
                                else xmlStreamReader.raiseError("Attribute \"NewTrcAdminObjsRuntimeInfoDefaultDetailLevel\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("UseIpcServer") )
                            {
                                strAttr = xmlStreamReader.attributes().value("UseIpcServer").toString();
                                bVal = str2Bool(strAttr, &bOk);
                                if( bOk ) trcServerSettings.m_bUseIpcServer = bVal;
                                else xmlStreamReader.raiseError("Attribute \"UseIpcServer\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("CacheDataIfNotConnected") )
                            {
                                strAttr = xmlStreamReader.attributes().value("CacheDataIfNotConnected").toString();
                                bVal = str2Bool(strAttr, &bOk);
                                if( bOk ) trcServerSettings.m_bCacheDataIfNotConnected = bVal;
                                else xmlStreamReader.raiseError("Attribute \"CacheDataIfNotConnected\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("CacheDataMaxArrLen") )
                            {
                                strAttr = xmlStreamReader.attributes().value("CacheDataMaxArrLen").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk ) trcServerSettings.m_iCacheDataMaxArrLen = iVal;
                                else xmlStreamReader.raiseError("Attribute \"CacheDataMaxArrLen\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("UseLocalTrcFile") )
                            {
                                strAttr = xmlStreamReader.attributes().value("UseLocalTrcFile").toString();
                                bVal = str2Bool(strAttr, &bOk);
                                if( bOk ) trcServerSettings.m_bUseLocalTrcFile = bVal;
                                else xmlStreamReader.raiseError("Attribute \"UseLocalTrcFile\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("LocalTrcFileAbsFilePath") )
                            {
                                strAttr = xmlStreamReader.attributes().value("LocalTrcFileAbsFilePath").toString();
                                trcServerSettings.m_strLocalTrcFileAbsFilePath = strAttr;
                            }
                            if( xmlStreamReader.attributes().hasAttribute("LocalTrcFileAutoSaveInterval_ms") )
                            {
                                strAttr = xmlStreamReader.attributes().value("LocalTrcFileAutoSaveInterval_ms").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk ) trcServerSettings.m_iLocalTrcFileAutoSaveInterval_ms = iVal;
                                else xmlStreamReader.raiseError("Attribute \"LocalTrcFileAutoSaveInterval_ms\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("LocalTrcFileSubFileCountMax") )
                            {
                                strAttr = xmlStreamReader.attributes().value("LocalTrcFileSubFileCountMax").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk ) trcServerSettings.m_iLocalTrcFileSubFileCountMax = iVal;
                                else xmlStreamReader.raiseError("Attribute \"LocalTrcFileSubFileCountMax\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("LocalTrcFileSubFileLineCountMax") )
                            {
                                strAttr = xmlStreamReader.attributes().value("LocalTrcFileSubFileLineCountMax").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk ) trcServerSettings.m_iLocalTrcFileSubFileLineCountMax = iVal;
                                else xmlStreamReader.raiseError("Attribute \"LocalTrcFileSubFileLineCountMax\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("LocalTrcFileCloseAfterEachWrite") )
                            {
                                strAttr = xmlStreamReader.attributes().value("LocalTrcFileCloseAfterEachWrite").toString();
                                bVal = str2Bool(strAttr, &bOk);
                                if( bOk ) trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite = bVal;
                                else xmlStreamReader.raiseError("Attribute \"LocalTrcFileCloseAfterEachWrite\" (" + strAttr + ") is out of range");
                            }

                            // While receiving the trace settings emitting the traceSettingsChanged signal is blocked
                            // by setting the flag m_bOnReceivedDataUpdateInProcess to true to accumulate all changes
                            // and emit the signal just once. setTraceSettings will check if the settings have been
                            // changed before taken them over. But setTraceSettings will not emit the settings changed
                            // signal as the flag m_bOnReceivedDataUpdateInProcess is set. So we need to do this check
                            // here also and emit the signal.
                            if( bRemoteNameChanged || trcServerSettings != m_trcServerSettings )
                            {
                                m_bOnReceivedDataUpdateInProcess = true;
                                setTraceSettings(trcServerSettings);
                                m_bOnReceivedDataUpdateInProcess = false;
                                emit traceSettingsChanged(this);
                            }
                        } // if( xmlStreamReader.isStartElement() )
                    } // if( strElemName == "ServerSettings" )

                    else
                    {
                        xmlStreamReader.raiseError("Invalid element name \"" + strElemName + "\" in received XML data");
                    }
                } // if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
            } // while( xmlStreamTokenType != QXmlStreamReader::EndElement && xmlStreamTokenType != QXmlStreamReader::Invalid )
        } // if( xmlStreamTokenType == QXmlStreamReader::StartDocument )

        if( xmlStreamReader.hasError() )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                SErrResultInfo errResultInfo = ErrResultInfoError("onReceivedData", EResultFileReadContent, xmlStreamReader.errorString());
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
    } // if( systemMsgTypeOfData == MsgProtocol::ESystemMsgTypeCon || systemMsgTypeOfData == MsgProtocol::ESystemMsgTypeInd )

} // onReceivedData

/*==============================================================================
protected slots: // connected to the signals of the IPC client
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcClient::onIpcClientConnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcClientConnected",
        /* strMthInArgs       */ strMthInArgs );

    // Request the trace admin objects from the server:
    // Not necessary as the trace server must send the settings,
    // the trace admin object tree and the cached trace data when
    // the client connects.

    #if 0
    QString strMsg;

    // Select (query) the settings of the trace server.
    strMsg  = systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
    strMsg += command2Str(MsgProtocol::ECommandSelect) + " ";
    strMsg += "<ServerSettings/>";

    sendData( str2ByteArr(strMsg) );

    // Select (query) the trace admin objects.
    strMsg  = systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
    strMsg += command2Str(MsgProtocol::ECommandSelect) + " ";
    strMsg += "<TrcAdminObjs/>";

    sendData( str2ByteArr(strMsg) );

    // Select (query) cached trace data. This query registers the client at
    // the server for online trace data.
    strMsg  = systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
    strMsg += command2Str(MsgProtocol::ECommandSelect) + " ";
    strMsg += "<TrcData/>";

    sendData( str2ByteArr(strMsg) );
    #endif

} // onIpcClientConnected

//------------------------------------------------------------------------------
void CIpcTrcClient::onIpcClientDisconnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcClientDisconnected",
        /* strMthInArgs       */ strMthInArgs );

} // onIpcClientDisconnected

/*==============================================================================
protected slots: // connected to the slots of the trace admin object pool model
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcClient::onTrcAdminObjIdxTreeEntryChanged( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_bOnReceivedDataUpdateInProcess )
    {
        return;
    }

    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strKeyInTree;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTrcAdminObjIdxTreeEntryChanged",
        /* strMthInArgs       */ strMthInArgs );

    CIdxTreeLocker idxTreeLocker(m_pTrcAdminObjIdxTree);

    CIdxTreeEntry* pTreeEntry = m_pTrcAdminObjIdxTree->findEntry(i_strKeyInTree);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->isLeave() )
        {
            //if( m_pTrcAdminObjIdxTree->getUpdateNameSpaceCallDepth() == 0 )
            {
                sendAdminObj(
                    /* systemMsgType */ MsgProtocol::ESystemMsgTypeReq,
                    /* cmd           */ MsgProtocol::ECommandUpdate,
                    /* pTrcAdminObj  */ dynamic_cast<CTrcAdminObj*>(pTreeEntry) );
            }
        }
        else // if( i_pTreeEntry->isRoot() || i_pTreeEntry->isBranch() )
        {
            //sendNameSpace(
            //    /* systemMsgType */ MsgProtocol::ESystemMsgTypeReq,
            //    /* cmd           */ ECommandUpdate,
            //    /* pTreeEntry    */ pTreeEntry,
            //    /* enabled       */ enabled,
            //    /* iDetailLevel  */ i_iDetailLevel );
        }
    } // if( i_pTreeEntry != nullptr )

} // onTrcAdminObjIdxTreeEntryChanged

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcClient::resetTrcAdminRefCounters( ZS::System::CIdxTreeEntry* i_pBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_pBranch == nullptr ? "null" : i_pBranch->keyInTree();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "resetTrcAdminRefCounters",
        /* strMthInArgs       */ strMthInArgs );

    CIdxTreeEntry* pTreeEntry;
    int            idxEntry;

    for( idxEntry = i_pBranch->count()-1; idxEntry >= 0; --idxEntry )
    {
        pTreeEntry = i_pBranch->at(idxEntry);

        if( pTreeEntry != nullptr )
        {
            if( pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
            {
                resetTrcAdminRefCounters(pTreeEntry);
            }
            else if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
            {
                CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

                if( pTrcAdminObj != nullptr )
                {
                    pTrcAdminObj->setRefCount(0);
                }
            }
        }
    }
} // resetTrcAdminRefCounters
