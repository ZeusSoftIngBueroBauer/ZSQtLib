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
    @param i_iTrcMthFileDetailLevel [in]
        If the methods of the trace client itself should be logged a value
        greater than 0 (ETraceDetailLevelNone) could be passed here.
    @param i_iTrcMthFileDetailLevelGateway [in]
        If the methods of the clients gateway should be logged a value greater
        than 0 (ETraceDetailLevelNone) could be passed here.
*/
CIpcTrcClient::CIpcTrcClient(
    const QString& i_strName,
    int            i_iTrcMthFileDetailLevel,
    int            i_iTrcMthFileDetailLevelGateway ) :
//------------------------------------------------------------------------------
    CClient(
        /* strObjName                    */ i_strName,
        /* bMultiThreadedAccess          */ false,
        /* iTrcMthFileDetailLevel        */ i_iTrcMthFileDetailLevel,
        /* iTrcMthFileDetailLevelGateway */ i_iTrcMthFileDetailLevelGateway ),
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
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );

    m_watchDogTimerSettings.m_bEnabled = false;

    m_pTrcAdminObjIdxTree = new CIdxTreeTrcAdminObjs(i_strName, this);

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

    // On disconnecting the trace admin object pool got to be cleared.
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

    if( !QObject::connect(
        /* pObjSender   */ m_pTrcAdminObjIdxTree,
        /* szSignal     */ SIGNAL( treeEntryChanged(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTrcAdminObjIdxTreeEntryChanged(ZS::System::CIdxTree*, ZS::System::CIdxTreeEntry*) ),
        /* cnctType     */ Qt::DirectConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

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
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMthInArgs       */ "" );

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

    if( m_iTrcMthFileDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_settings.toString();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
            if( m_trcServerSettings.m_bCacheDataIfNotConnected != i_settings.m_bCacheDataIfNotConnected )
            {
                strMsg += " CacheDataIfNotConnected=\"" + bool2Str(i_settings.m_bCacheDataIfNotConnected) + "\"";
            }
            if( m_trcServerSettings.m_iCacheDataMaxArrLen != i_settings.m_iCacheDataMaxArrLen )
            {
                strMsg += " CacheDataMaxArrLen =\"" + QString::number(i_settings.m_iCacheDataMaxArrLen) + "\"";
            }
            if( m_trcServerSettings.m_bNewTrcAdminObjsEnabledAsDefault != i_settings.m_bNewTrcAdminObjsEnabledAsDefault )
            {
                strMsg += " NewTrcAdminObjsEnabledAsDefault=\"" + bool2Str(i_settings.m_bNewTrcAdminObjsEnabledAsDefault) + "\"";
            }
            if( m_trcServerSettings.m_iNewTrcAdminObjsDefaultDetailLevel != i_settings.m_iNewTrcAdminObjsDefaultDetailLevel )
            {
                strMsg += " NewTrcAdminObjsDefaultDetailLevel=\"" + QString::number(i_settings.m_iNewTrcAdminObjsDefaultDetailLevel) + "\"";
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
        }

        m_trcServerSettings = i_settings;

        // If not called on receiving trace settings from the server ..
        if( !m_bOnReceivedDataUpdateInProcess )
        {
            emit traceSettingsChanged(this);
        }
    }
}

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

    if( m_iTrcMthFileDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "MsgType: " + systemMsgType2Str(i_systemMsgType);
        strMthInArgs += ", Cmd: " + command2Str(i_cmd);
        strMthInArgs += ", AdmObj: " + i_pTrcAdminObj->keyInTree();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
        strMsg += " DetailLevel=\"" + QString::number(i_pTrcAdminObj->getTraceDetailLevel()) + "\"";
        strMsg += "/>";

        sendData( str2ByteArr(strMsg) );

    } // if( i_pTrcAdminObj != nullptr && isConnected() )

} // sendAdminObj

//------------------------------------------------------------------------------
void CIpcTrcClient::sendNameSpace(
    MsgProtocol::TSystemMsgType i_systemMsgType,
    MsgProtocol::TCommand       i_cmd,
    CIdxTreeEntry*              i_pBranch,
    EEnabled                    i_enabled,
    int                         i_iDetailLevel )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcMthFileDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "MsgType: " + systemMsgType2Str(i_systemMsgType);
        strMthInArgs += ", Cmd: " + command2Str(i_cmd);
        strMthInArgs += ", Branch: " + i_pBranch->keyInTree();
        strMthInArgs += ", Enabled: " + CEnum<EEnabled>(i_enabled).toString();
        strMthInArgs += ", DetailLevel: " + QString::number(i_iDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
        if( i_iDetailLevel >= 0 )
        {
            strMsg += " DetailLevel=\"" + QString::number(i_iDetailLevel) + "\"";
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

    if( m_iTrcMthFileDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs += "[" + QString::number(i_byteArr.size()) + "]";
        if( m_iTrcMthFileDetailLevel < ETraceDetailLevelVerbose ) {
            strMthInArgs += "(" + truncateStringWithEllipsisInTheMiddle(byteArr2Str(i_byteArr), 30) + ")";
        } else {
            strMthInArgs += "(" + truncateStringWithEllipsisInTheMiddle(byteArr2Str(i_byteArr), 100) + ")";
        }
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onReceivedData",
        /* strMthInArgs       */ strMthInArgs );

    m_bOnReceivedDataUpdateInProcess = true;

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
        QString                     strElemName;
        QString                     strDataSnd;
        QString                     strAttr;
        bool                        bVal;
        int                         iVal;
        bool                        bOk;

        xmlStreamTokenType = xmlStreamReader.readNext();

        if( xmlStreamTokenType != QXmlStreamReader::StartDocument )
        {
            xmlStreamReader.raiseError("Invalid XML command");
        }
        else // if( xmlStreamTokenType == QXmlStreamReader::StartDocument )
        {
            while( xmlStreamTokenType != QXmlStreamReader::EndElement && xmlStreamTokenType != QXmlStreamReader::Invalid )
            {
                xmlStreamTokenType = xmlStreamReader.readNext();

                if( xmlStreamReader.isStartElement() )
                {
                    strElemName = xmlStreamReader.name().toString();

                    if( strElemName == "ServerSettings" )
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
                        if( xmlStreamReader.attributes().hasAttribute("NewTrcAdminObjsDefaultDetailLevel") )
                        {
                            strAttr = xmlStreamReader.attributes().value("NewTrcAdminObjsDefaultDetailLevel").toString();
                            iVal = strAttr.toInt(&bOk);
                            if( bOk ) trcServerSettings.m_iNewTrcAdminObjsDefaultDetailLevel = iVal;
                            else xmlStreamReader.raiseError("Attribute \"NewTrcAdminObjsDefaultDetailLevel\" (" + strAttr + ") is out of range");
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
                            setTraceSettings(trcServerSettings);
                            emit traceSettingsChanged(this);
                        }
                    } // if( strElemName == "ServerSettings" )

                    else if( strElemName == "Branch" )
                    {
                        int      iParentPranchIdxInTree = -1;
                        QString  strBranchName;
                        int      idxInTree = -1;
                        EEnabled enabled = EEnabled::Undefined;
                        int      iDetailLevel = -1;

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
                            if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("DetailLevel") )
                            {
                                strAttr = xmlStreamReader.attributes().value("DetailLevel").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk && iVal >= 0 ) iDetailLevel = iVal;
                                else xmlStreamReader.raiseError("Attribute \"DetailLevel\" (" + strAttr + ") is out of range");
                            }

                            if( !xmlStreamReader.hasError() )
                            {
                                CIdxTreeEntry* pBranch = m_pTrcAdminObjIdxTree->getEntry(idxInTree);

                                if( strBranchName.isEmpty() && pBranch == nullptr )
                                {
                                    xmlStreamReader.raiseError("There is no branch at tree index " + QString::number(idxInTree));
                                }
                                else if( !strBranchName.isEmpty() && pBranch == nullptr )
                                {
                                    pBranch = m_pTrcAdminObjIdxTree->insertBranch(iParentPranchIdxInTree, strBranchName, idxInTree);
                                }
                                if( pBranch != nullptr )
                                {
                                    if( enabled != EEnabled::Undefined ) m_pTrcAdminObjIdxTree->setEnabled(pBranch, enabled);
                                    if( iDetailLevel >= 0 ) m_pTrcAdminObjIdxTree->setTraceDetailLevel(pBranch, iDetailLevel);
                                }
                            } // if( !xmlStreamReader.hasError() )
                        } // if( idxInTree >= 0 )
                    } // if( strElemName == "Branch" )

                    else if( strElemName == "TrcAdminObj" )
                    {
                        int      iParentPranchIdxInTree = -1;
                        QString  strNameSpace;
                        QString  strClassName;
                        QString  strObjName;
                        int      idxInTree = -1;
                        QString  strThreadName;
                        EEnabled enabled = EEnabled::Undefined;
                        int      iDetailLevel = -1;
                        int      iRefCount = -1;

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
                            if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("DetailLevel") )
                            {
                                strAttr = xmlStreamReader.attributes().value("DetailLevel").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk && iVal >= 0 ) iDetailLevel = iVal;
                                else xmlStreamReader.raiseError("Attribute \"DetailLevel\" (" + strAttr + ") is out of range");
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
                                CTrcAdminObj* pTrcAdminObj = m_pTrcAdminObjIdxTree->getTraceAdminObj(idxInTree);

                                if( pTrcAdminObj != nullptr )
                                {
                                    bool bSignalsBlocked = pTrcAdminObj->blockTreeEntryChangedSignal(true);

                                    if( !strThreadName.isEmpty() ) pTrcAdminObj->setObjectThreadName(strThreadName);
                                    if( enabled != EEnabled::Undefined ) pTrcAdminObj->setEnabled(enabled);
                                    if( iDetailLevel >= 0 ) pTrcAdminObj->setTraceDetailLevel(iDetailLevel);
                                    if( iRefCount >= 0 ) pTrcAdminObj->setRefCount(iRefCount);

                                    pTrcAdminObj->blockTreeEntryChangedSignal(bSignalsBlocked);
                                }
                                else // if( pTrcAdminObj == nullptr )
                                {
                                    if( strNameSpace.isEmpty() && strClassName.isEmpty() && strObjName.isEmpty() )
                                    {
                                        xmlStreamReader.raiseError("Neither NameSpace nor ClassName nor ObjName provided and there is no trace admin object at tree index " + QString::number(idxInTree));
                                    }
                                    else
                                    {
                                        pTrcAdminObj = m_pTrcAdminObjIdxTree->insertTraceAdminObj(iParentPranchIdxInTree, strNameSpace, strClassName, strObjName, idxInTree);

                                        if( pTrcAdminObj != nullptr )
                                        {
                                            bool bSignalsBlocked = pTrcAdminObj->blockTreeEntryChangedSignal(true);

                                            if( !strThreadName.isEmpty() ) pTrcAdminObj->setObjectThreadName(strThreadName);
                                            if( enabled != EEnabled::Undefined ) pTrcAdminObj->setEnabled(enabled);
                                            if( iDetailLevel >= 0 ) pTrcAdminObj->setTraceDetailLevel(iDetailLevel);
                                            if( iRefCount >= 0 ) pTrcAdminObj->setRefCount(iRefCount);

                                            pTrcAdminObj->blockTreeEntryChangedSignal(bSignalsBlocked);

                                            emit traceAdminObjInserted(this, pTrcAdminObj->keyInTree());
                                        }
                                    }
                                }
                            } // if( !xmlStreamReader.hasError() )
                        } // if( idxInTree >= 0 )
                    } // if( strTblName == "TrcAdminObj" )

                    else if( strElemName == "TrcData" )
                    {
                        emit traceDataReceived(this, strData);
                    }

                    else
                    {
                        xmlStreamReader.raiseError("Invalid element name \"" + strElemName + "\" in received XML data");
                    }
                } // if( xmlStreamReader.isStartElement() )
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

    m_bOnReceivedDataUpdateInProcess = false;

} // onReceivedData

/*==============================================================================
protected slots: // connected to the signals of the IPC client
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcClient::onIpcClientConnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcMthFileDetailLevel >= ETraceDetailLevelMethodArgs )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcClientConnected",
        /* strMthInArgs       */ strMthInArgs );

    // Request the trace admin objects from the server:
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

} // onIpcClientConnected

//------------------------------------------------------------------------------
void CIpcTrcClient::onIpcClientDisconnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcMthFileDetailLevel >= ETraceDetailLevelMethodArgs )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onIpcClientDisconnected",
        /* strMthInArgs       */ strMthInArgs );

    // Before the index tree can be cleared the ref counters of the trace admin objects
    // got to be reset to avoid an err log entry (object ref counter is not 0 in dtor).

    m_strRemoteApplicationName = "";
    m_strRemoteServerName = "";

    // To avoid err log entry: The dtor is called even if the objects reference counter is not 0.
    resetTrcAdminRefCounters(m_pTrcAdminObjIdxTree->root());

    m_pTrcAdminObjIdxTree->clear();

} // onIpcClientDisconnected

/*==============================================================================
protected slots: // connected to the slots of the trace admin object pool model
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcClient::onTrcAdminObjIdxTreeEntryChanged(
    CIdxTree*      /*i_pIdxTree*/,
    CIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    if( m_bOnReceivedDataUpdateInProcess )
    {
        return;
    }

    QString strMthInArgs;

    if( m_iTrcMthFileDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTrcAdminObjIdxTreeEntryChanged",
        /* strMthInArgs       */ strMthInArgs );

    if( i_pTreeEntry != nullptr )
    {
        if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            //if( m_pTrcAdminObjIdxTree->getUpdateNameSpaceCallDepth() == 0 )
            {
                sendAdminObj(
                    /* systemMsgType */ MsgProtocol::ESystemMsgTypeReq,
                    /* cmd           */ MsgProtocol::ECommandUpdate,
                    /* pTrcAdminObj  */ dynamic_cast<CTrcAdminObj*>(i_pTreeEntry) );
            }
        }
        else // if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
        {
            //sendNameSpace(
            //    /* systemMsgType */ MsgProtocol::ESystemMsgTypeReq,
            //    /* cmd           */ ECommandUpdate,
            //    /* pTreeEntry    */ i_pTreeEntry,
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

    if( m_iTrcMthFileDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_pBranch == nullptr ? "null" : i_pBranch->keyInTree();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcMthFileDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
