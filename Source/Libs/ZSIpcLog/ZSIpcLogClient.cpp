/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSIpcLog/ZSIpcLogClient.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysRequest.h"
#include "ZSSys/ZSSysRequestExecTree.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysLogger.h"
#include "ZSSys/ZSSysLoggerIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Log;


/*******************************************************************************
class CIpcLogClient : public CClient
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the log client.

    @param i_strName [in] Name of the client.
*/
CIpcLogClient::CIpcLogClient(const QString& i_strName) :
//------------------------------------------------------------------------------
    CClient(i_strName, false),
    m_strRemoteApplicationName(),
    m_strRemoteServerName(),
    m_logServerSettings(),
    m_pLoggersIdxTree(nullptr),
    m_bOnReceivedDataUpdateInProcess(false)
{
    m_watchDogTimerSettings.m_bEnabled = false;

    m_pLoggersIdxTree = new CIdxTreeLoggers(i_strName, this);

    m_hostSettings.m_uRemotePort = 24762;

    // Watch dog timer not used for the log client.
    m_bWatchDogTimerUsed = false;

    // Connect to signals of the Ipc client
    //-------------------------------------

    QObject::connect(
        this, &CIpcLogClient::connected,
        this, &CIpcLogClient::onIpcClientConnected);
    QObject::connect(
        this, &CIpcLogClient::disconnected,
        this, &CIpcLogClient::onIpcClientDisconnected);

    // Connect to the signals of the index tree
    //-----------------------------------------

    QObject::connect(
        m_pLoggersIdxTree, &CIdxTreeLoggers::treeEntryChanged,
        this, &CIpcLogClient::onLoggersIdxTreeEntryChanged );

} // ctor

//------------------------------------------------------------------------------
CIpcLogClient::~CIpcLogClient()
//------------------------------------------------------------------------------
{
    QObject::disconnect(
        m_pLoggersIdxTree, &CIdxTreeLoggers::treeEntryChanged,
        this, &CIpcLogClient::onLoggersIdxTreeEntryChanged );

    abortAllRequests(); // Deletes or at least invalidates the current request in progress.

    // Disconnect client before destroying the index tree.
    stopGatewayThread();

    try
    {
        delete m_pLoggersIdxTree;
    }
    catch(...)
    {
    }

    //m_strRemoteApplicationName;
    //m_strRemoteServerName;
    //m_logServerSettings;
    m_pLoggersIdxTree = nullptr;
    m_bOnReceivedDataUpdateInProcess = false;

} // dtor

/*==============================================================================
public: // overridables of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CIpcLogClient::connect_( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    m_strRemoteApplicationName = "";
    m_strRemoteServerName = "";

    m_pLoggersIdxTree->clear();

    return CClient::connect_(i_iTimeout_ms, i_bWait, i_iReqIdParent);
}

/*==============================================================================
public: // instance methods to read remote application settings
==============================================================================*/

//------------------------------------------------------------------------------
QString CIpcLogClient::getRemoteApplicationName() const
//------------------------------------------------------------------------------
{
    return m_strRemoteApplicationName;
}

//------------------------------------------------------------------------------
QString CIpcLogClient::getRemoteServerName() const
//------------------------------------------------------------------------------
{
    return m_strRemoteServerName;
}

/*==============================================================================
public: // instance methods to enable and disable the client and server
==============================================================================*/

//------------------------------------------------------------------------------
SLogServerSettings CIpcLogClient::getLogSettings() const
//------------------------------------------------------------------------------
{
    return m_logServerSettings;
}

//------------------------------------------------------------------------------
void CIpcLogClient::setLogSettings( const SLogServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    if( m_logServerSettings != i_settings )
    {
        // If not called on receiving log settings from the server and if the client is connected ..
        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings";

            if( m_logServerSettings.m_bEnabled != i_settings.m_bEnabled )
            {
                strMsg += " Enabled=\"" + bool2Str(i_settings.m_bEnabled) + "\"";
            }
            if( m_logServerSettings.m_bNewLoggersEnabledAsDefault != i_settings.m_bNewLoggersEnabledAsDefault )
            {
                strMsg += " NewLoggersEnabledAsDefault=\"" + bool2Str(i_settings.m_bNewLoggersEnabledAsDefault) + "\"";
            }
            if( m_logServerSettings.m_eNewLoggersDefaultDetailLevel != i_settings.m_eNewLoggersDefaultDetailLevel )
            {
                strMsg += " NewLoggersDefaultDetailLevel=\""
                       + CEnumLogDetailLevel(i_settings.m_eNewLoggersDefaultDetailLevel).toString() + "\"";
            }
            if( m_logServerSettings.m_bUseIpcServer != i_settings.m_bUseIpcServer )
            {
                strMsg += " UseIpcServer=\"" + bool2Str(i_settings.m_bUseIpcServer) + "\"";
            }
            if( m_logServerSettings.m_bCacheDataIfNotConnected != i_settings.m_bCacheDataIfNotConnected )
            {
                strMsg += " CacheDataIfNotConnected=\"" + bool2Str(i_settings.m_bCacheDataIfNotConnected) + "\"";
            }
            if( m_logServerSettings.m_iCacheDataMaxArrLen != i_settings.m_iCacheDataMaxArrLen )
            {
                strMsg += " CacheDataMaxArrLen =\"" + QString::number(i_settings.m_iCacheDataMaxArrLen) + "\"";
            }
            if( m_logServerSettings.m_bUseLocalLogFile != i_settings.m_bUseLocalLogFile )
            {
                strMsg += " UseLocalLogFile=\"" + bool2Str(i_settings.m_bUseLocalLogFile) + "\"";
            }
            if( m_logServerSettings.m_iLocalLogFileAutoSaveInterval_ms != i_settings.m_iLocalLogFileAutoSaveInterval_ms )
            {
                strMsg += " LocalLogFileAutoSaveInterval_ms=\"" + QString::number(i_settings.m_iLocalLogFileAutoSaveInterval_ms) + "\"";
            }
            if( m_logServerSettings.m_iLocalLogFileSubFileCountMax != i_settings.m_iLocalLogFileSubFileCountMax )
            {
                strMsg += " LocalLogFileSubFileCountMax=\"" + QString::number(i_settings.m_iLocalLogFileSubFileCountMax) + "\"";
            }
            if( m_logServerSettings.m_iLocalLogFileSubFileLineCountMax != i_settings.m_iLocalLogFileSubFileLineCountMax )
            {
                strMsg += " LocalLogFileSubFileLineCountMax=\"" + QString::number(i_settings.m_iLocalLogFileSubFileLineCountMax) + "\"";
            }
            if( m_logServerSettings.m_bLocalLogFileCloseFileAfterEachWrite != i_settings.m_bLocalLogFileCloseFileAfterEachWrite )
            {
                strMsg += " LocalLogFileCloseAfterEachWrite=\"" + bool2Str(i_settings.m_bLocalLogFileCloseFileAfterEachWrite) + "\"";
            }

            strMsg += "/>";

            CRequest* pReq = sendData( str2ByteArr(strMsg) );

            if( !isAsynchronousRequest(pReq) )
            {
                pReq = nullptr; // deleted later by request queue
            }
        } // if( !m_bOnReceivedDataUpdateInProcess && isConnected() )

        m_logServerSettings = i_settings;

        // If not called on receiving log settings from the server ..
        if( !m_bOnReceivedDataUpdateInProcess )
        {
            emit logSettingsChanged(this);
        }
    }
} // setLogSettings

/*==============================================================================
protected: // instance methods to send admin objects to the connected server
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogClient::sendBranch(
    MsgProtocol::TSystemMsgType i_systemMsgType,
    MsgProtocol::TCommand       i_cmd,
    CIdxTreeEntry*              i_pBranch,
    EEnabled                    i_enabled,
    ELogDetailLevel             i_eDetailLevel,
    const QString&              i_strDataFilter )
//------------------------------------------------------------------------------
{
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
        if( i_eDetailLevel != ELogDetailLevel::Undefined )
        {
            strMsg += " DetailLevel=\"" + CEnumLogDetailLevel(i_eDetailLevel).toString() + "\"";
        }
        if( !i_strDataFilter.isNull() )
        {
            strMsg += " DataFilter=\"" + i_strDataFilter + "\"";
        }
        strMsg += "/>";

        sendData( str2ByteArr(strMsg) );
    }
}

//------------------------------------------------------------------------------
void CIpcLogClient::sendLeave(
    MsgProtocol::TSystemMsgType i_systemMsgType,
    MsgProtocol::TCommand       i_cmd,
    CLogger*                    i_pLogger )
//------------------------------------------------------------------------------
{
    if( i_pLogger != nullptr && isConnected() )
    {
        QString strMsg;

        strMsg += systemMsgType2Str(i_systemMsgType) + " ";
        strMsg += command2Str(i_cmd) + " ";
        strMsg += "<Logger ";
        strMsg += " ObjId=\"" + QString::number(i_pLogger->indexInTree()) + "\"";
        strMsg += " Enabled=\"" + CEnumEnabled::toString(i_pLogger->getEnabled()) + "\"";
        strMsg += " LogLevel=\"" + CEnumLogDetailLevel(i_pLogger->getLogLevel()).toString() + "\"";
        strMsg += " DataFilter=\"" + i_pLogger->getDataFilter() + "\"";
        strMsg += " AddThreadName=\"" + bool2Str(i_pLogger->addThreadName()) + "\"";
        strMsg += " AddDateTime=\"" + bool2Str(i_pLogger->addDateTime()) + "\"";
        strMsg += " AddSystemTime=\"" + bool2Str(i_pLogger->addSystemTime()) + "\"";
        strMsg += "/>";

        sendData( str2ByteArr(strMsg) );
    }
}

//------------------------------------------------------------------------------
void CIpcLogClient::onReceivedData( const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
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
            bool bInLogDataBlock = false;

            while( xmlStreamTokenType != QXmlStreamReader::EndElement && xmlStreamTokenType != QXmlStreamReader::Invalid )
            {
                xmlStreamTokenType = xmlStreamReader.readNext();

                if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
                {
                    strElemName = xmlStreamReader.name().toString();

                    if( strElemName == "LogData" )
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            bInLogDataBlock = true;
                            emit logDataReceived(this, strData);
                        }
                        else if( xmlStreamReader.isEndElement() )
                        {
                            bInLogDataBlock = false;
                        }
                    }

                    else if( strElemName == "Branch" )
                    {
                        if( !bInLogDataBlock && xmlStreamReader.isStartElement() )
                        {
                            int      iParentPranchIdxInTree = -1;
                            QString  strBranchName;
                            int      idxInTree = -1;
                            EEnabled enabled = EEnabled::Undefined;
                            bool     bSetDataFilter = false;
                            QString  strDataFilter;
                            ELogDetailLevel eDetailLevel = ELogDetailLevel::Undefined;

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
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LogLevel") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("LogLevel").toString();
                                    eDetailLevel = CEnumLogDetailLevel::toEnumerator(strAttr);
                                    if( eDetailLevel == ELogDetailLevel::Undefined )
                                    {
                                        xmlStreamReader.raiseError("Attribute \"LogLevel\" (" + strAttr + ") is out of range");
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

                                    CIdxTreeEntry* pBranch = m_pLoggersIdxTree->getEntry(idxInTree);

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
                                                pBranch = m_pLoggersIdxTree->insertBranch(iParentPranchIdxInTree, strBranchName, idxInTree);
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
                                            pBranch = m_pLoggersIdxTree->insertBranch(iParentPranchIdxInTree, strBranchName, idxInTree);
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
                                            m_pLoggersIdxTree->setEnabled(pBranch, enabled);
                                        }
                                        if( eDetailLevel != ELogDetailLevel::Undefined ) {
                                            m_pLoggersIdxTree->setDetailLevel(pBranch, eDetailLevel);
                                        }
                                        if( bSetDataFilter ) {
                                            m_pLoggersIdxTree->setDataFilter(pBranch, strDataFilter);
                                        }
                                    }
                                    m_bOnReceivedDataUpdateInProcess = false;
                                }
                            } // if( idxInTree >= 0 )
                        } // if( !bInLogDataBlock && xmlStreamReader.isStartElement() )
                    } // if( strElemName == "Branch" )

                    else if( strElemName == "Logger" )
                    {
                        if( !bInLogDataBlock && xmlStreamReader.isStartElement() )
                        {
                            int      iParentPranchIdxInTree = -1;
                            QString  strName;
                            QString  strNameSpace;
                            QString  strClassName;
                            QString  strObjName;
                            int      idxInTree = -1;
                            EEnabled enabled = EEnabled::Undefined;
                            bool     bSetDataFilter = false;
                            QString  strDataFilter;
                            bool     bSetAddThreadName = false;
                            bool     bAddThreadName = false;
                            bool     bSetAddDateTime = false;
                            bool     bAddDateTime = false;
                            bool     bSetAddSystemTime = false;
                            bool     bAddSystemTime = false;

                            ELogDetailLevel eDetailLevel = ELogDetailLevel::Undefined;

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
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Name") )
                                {
                                    strName = xmlStreamReader.attributes().value("Name").toString();
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Enabled") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                                    enabled = CEnumEnabled::toEnumerator(strAttr);
                                    if( enabled == EEnabled::Undefined ) xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") is out of range");
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LogLevel") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("LogLevel").toString();
                                    eDetailLevel = CEnumLogDetailLevel::toEnumerator(strAttr);
                                    if( eDetailLevel == ELogDetailLevel::Undefined )
                                    {
                                        xmlStreamReader.raiseError("Attribute \"LogLevel\" (" + strAttr + ") is out of range");
                                    }
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("DataFilter") )
                                {
                                    strDataFilter = xmlStreamReader.attributes().value("DataFilter").toString();
                                    bSetDataFilter = true;
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("AddThreadName") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("AddThreadName").toString();
                                    bVal = str2Bool(strAttr, &bOk);
                                    if( !bOk ) {
                                        xmlStreamReader.raiseError("Attribute \"AddThreadName\" (" + strAttr + ") is out of range");
                                    }
                                    else {
                                        bAddThreadName = bVal;
                                        bSetAddThreadName = true;
                                    }
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("AddDateTime") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("AddDateTime").toString();
                                    bVal = str2Bool(strAttr, &bOk);
                                    if( !bOk ) {
                                        xmlStreamReader.raiseError("Attribute \"AddDateTime\" (" + strAttr + ") is out of range");
                                    }
                                    else {
                                        bAddDateTime = bVal;
                                        bSetAddDateTime = true;
                                    }
                                }
                                if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("AddSystemTime") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("AddSystemTime").toString();
                                    bVal = str2Bool(strAttr, &bOk);
                                    if( !bOk ) {
                                        xmlStreamReader.raiseError("Attribute \"AddSystemTime\" (" + strAttr + ") is out of range");
                                    }
                                    else {
                                        bAddSystemTime = bVal;
                                        bSetAddSystemTime = true;
                                    }
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

                                if( !xmlStreamReader.hasError() )
                                {
                                    m_bOnReceivedDataUpdateInProcess = true;

                                    CLogger* pLogger = m_pLoggersIdxTree->getLogger(idxInTree);

                                    bool bLoggerAlreadyExisting = (pLogger != nullptr);

                                    // Confirmation to select command.
                                    if( cmd == MsgProtocol::ECommandSelect )
                                    {
                                        if( pLogger == nullptr )
                                        {
                                            if( strName.isEmpty() )
                                            {
                                                xmlStreamReader.raiseError("No logger at tree index " + QString::number(idxInTree) + " but no logger name provided.");
                                            }
                                            else
                                            {
                                                pLogger = m_pLoggersIdxTree->insertLogger(iParentPranchIdxInTree, strName, idxInTree);
                                            }
                                        }
                                        else // if( pBranch != nullptr )
                                        {
                                            if( !strNameSpace.isEmpty() && strNameSpace.compare(pLogger->getNameSpace(), Qt::CaseInsensitive) != 0 )
                                            {
                                                xmlStreamReader.raiseError("Update logger command received for tree index " + QString::number(idxInTree) +
                                                    " but provided name space " + strNameSpace + " is different from existing name space " + pLogger->getNameSpace());
                                            }
                                            else if( !strClassName.isEmpty() && strClassName.compare(pLogger->getClassName(), Qt::CaseInsensitive) != 0 )
                                            {
                                                xmlStreamReader.raiseError("Update logger command received for tree index " + QString::number(idxInTree) +
                                                    " but provided class name " + strClassName + " is different from existing class name " + pLogger->getClassName());
                                            }
                                            else if( !strObjName.isEmpty() && strObjName.compare(pLogger->getObjectName(), Qt::CaseInsensitive) != 0 )
                                            {
                                                xmlStreamReader.raiseError("Update logger received for tree index " + QString::number(idxInTree) +
                                                    " but provided object name " + strObjName + " is different from existing object name " + pLogger->getObjectName());
                                            }
                                        }
                                    }
                                    else if( cmd == MsgProtocol::ECommandUpdate )
                                    {
                                        if( pLogger == nullptr )
                                        {
                                            xmlStreamReader.raiseError("Update logger command received but there is no object entry at tree index " + QString::number(idxInTree));
                                        }
                                    }
                                    // Indication that object has been inserted.
                                    else if( cmd == MsgProtocol::ECommandInsert )
                                    {
                                        if( pLogger != nullptr )
                                        {
                                            xmlStreamReader.raiseError("Insert logger command received but there is already an object entry at tree index " + QString::number(idxInTree));
                                        }
                                        else if( strName.isEmpty() )
                                        {
                                            xmlStreamReader.raiseError("Insert logger command received but no logger name provided");
                                        }
                                        else // if( pLogger == nullptr && !(strNameSpace.isEmpty() || .. )
                                        {
                                            pLogger = m_pLoggersIdxTree->insertLogger(iParentPranchIdxInTree, strName, idxInTree);
                                        }
                                    }
                                    // Indication that object has been deleted.
                                    else if( cmd == MsgProtocol::ECommandDelete )
                                    {
                                        if( pLogger == nullptr )
                                        {
                                            xmlStreamReader.raiseError("Delete logger command received but there is no object entry at tree index " + QString::number(idxInTree));
                                        }
                                        else
                                        {
                                            delete dynamic_cast<CIdxTreeEntry*>(pLogger);
                                            pLogger = nullptr;
                                        }
                                    }

                                    if( !xmlStreamReader.hasError() && pLogger != nullptr )
                                    {
                                        bool bSignalsBlocked = pLogger->blockTreeEntryChangedSignal(true);

                                        if( enabled != EEnabled::Undefined ) {
                                            pLogger->setEnabled(enabled);
                                        }
                                        if( eDetailLevel != ELogDetailLevel::Undefined ) {
                                            pLogger->setLogLevel(eDetailLevel);
                                        }
                                        if( bSetDataFilter ) {
                                            pLogger->setDataFilter(strDataFilter);
                                        }
                                        if( bSetAddThreadName ) {
                                            pLogger->setAddThreadName(bAddThreadName);
                                        }
                                        if( bSetAddDateTime ) {
                                            pLogger->setAddDateTime(bAddDateTime);
                                        }
                                        if( bSetAddSystemTime ) {
                                            pLogger->setAddSystemTime(bAddSystemTime);
                                        }
                                        if( !strNameSpace.isNull() ) {
                                            pLogger->setNameSpace(strNameSpace);
                                        }
                                        if( !strClassName.isNull() ) {
                                            pLogger->setClassName(strClassName);
                                        }
                                        if( !strObjName.isNull() ) {
                                            pLogger->setObjectName(strObjName);
                                        }

                                        pLogger->blockTreeEntryChangedSignal(bSignalsBlocked);

                                        if( !bLoggerAlreadyExisting )
                                        {
                                            emit loggerInserted(this, pLogger->keyInTree());
                                        }
                                        else
                                        {
                                            emit loggerChanged(this, pLogger->keyInTree());
                                        }
                                    }
                                    m_bOnReceivedDataUpdateInProcess = false;

                                } // if( !xmlStreamReader.hasError() )
                            } // if( idxInTree >= 0 )
                        } // if( !bInLogDataBlock && xmlStreamReader.isStartElement() )
                    } // if( strElemName == "Logger" )

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

                            SLogServerSettings logServerSettings = m_logServerSettings;

                            if( xmlStreamReader.attributes().hasAttribute("Enabled") )
                            {
                                strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                                bVal = str2Bool(strAttr, &bOk);
                                if( bOk ) logServerSettings.m_bEnabled = bVal;
                                else xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("LoggerFileAbsFilePath") )
                            {
                                strAttr = xmlStreamReader.attributes().value("LoggerFileAbsFilePath").toString();
                                logServerSettings.m_strLoggerFileAbsFilePath = strAttr;
                            }
                            if( xmlStreamReader.attributes().hasAttribute("NewLoggersEnabledAsDefault") )
                            {
                                strAttr = xmlStreamReader.attributes().value("NewLoggersEnabledAsDefault").toString();
                                bVal = str2Bool(strAttr, &bOk);
                                if( bOk ) logServerSettings.m_bNewLoggersEnabledAsDefault = bVal;
                                else xmlStreamReader.raiseError("Attribute \"NewLoggersEnabledAsDefault\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("NewLoggersDefaultDetailLevel") )
                            {
                                strAttr = xmlStreamReader.attributes().value("NewLoggersDefaultDetailLevel").toString();
                                CEnumLogDetailLevel eDetailLevel = CEnumLogDetailLevel::fromString(strAttr);
                                if( eDetailLevel.enumerator() != ELogDetailLevel::Undefined ) logServerSettings.m_eNewLoggersDefaultDetailLevel = eDetailLevel.enumerator();
                                else xmlStreamReader.raiseError("Attribute \"NewLoggersDefaultDetailLevel\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("UseIpcServer") )
                            {
                                strAttr = xmlStreamReader.attributes().value("UseIpcServer").toString();
                                bVal = str2Bool(strAttr, &bOk);
                                if( bOk ) logServerSettings.m_bUseIpcServer = bVal;
                                else xmlStreamReader.raiseError("Attribute \"UseIpcServer\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("CacheDataIfNotConnected") )
                            {
                                strAttr = xmlStreamReader.attributes().value("CacheDataIfNotConnected").toString();
                                bVal = str2Bool(strAttr, &bOk);
                                if( bOk ) logServerSettings.m_bCacheDataIfNotConnected = bVal;
                                else xmlStreamReader.raiseError("Attribute \"CacheDataIfNotConnected\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("CacheDataMaxArrLen") )
                            {
                                strAttr = xmlStreamReader.attributes().value("CacheDataMaxArrLen").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk ) logServerSettings.m_iCacheDataMaxArrLen = iVal;
                                else xmlStreamReader.raiseError("Attribute \"CacheDataMaxArrLen\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("UseLocalLogFile") )
                            {
                                strAttr = xmlStreamReader.attributes().value("UseLocalLogFile").toString();
                                bVal = str2Bool(strAttr, &bOk);
                                if( bOk ) logServerSettings.m_bUseLocalLogFile = bVal;
                                else xmlStreamReader.raiseError("Attribute \"UseLocalLogFile\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("LocalLogFileAbsFilePath") )
                            {
                                strAttr = xmlStreamReader.attributes().value("LocalLogFileAbsFilePath").toString();
                                logServerSettings.m_strLocalLogFileAbsFilePath = strAttr;
                            }
                            if( xmlStreamReader.attributes().hasAttribute("LocalLogFileAutoSaveInterval_ms") )
                            {
                                strAttr = xmlStreamReader.attributes().value("LocalLogFileAutoSaveInterval_ms").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk ) logServerSettings.m_iLocalLogFileAutoSaveInterval_ms = iVal;
                                else xmlStreamReader.raiseError("Attribute \"LocalLogFileAutoSaveInterval_ms\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("LocalLogFileSubFileCountMax") )
                            {
                                strAttr = xmlStreamReader.attributes().value("LocalLogFileSubFileCountMax").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk ) logServerSettings.m_iLocalLogFileSubFileCountMax = iVal;
                                else xmlStreamReader.raiseError("Attribute \"LocalLogFileSubFileCountMax\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("LocalLogFileSubFileLineCountMax") )
                            {
                                strAttr = xmlStreamReader.attributes().value("LocalLogFileSubFileLineCountMax").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk ) logServerSettings.m_iLocalLogFileSubFileLineCountMax = iVal;
                                else xmlStreamReader.raiseError("Attribute \"LocalLogFileSubFileLineCountMax\" (" + strAttr + ") is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("LocalLogFileCloseAfterEachWrite") )
                            {
                                strAttr = xmlStreamReader.attributes().value("LocalLogFileCloseAfterEachWrite").toString();
                                bVal = str2Bool(strAttr, &bOk);
                                if( bOk ) logServerSettings.m_bLocalLogFileCloseFileAfterEachWrite = bVal;
                                else xmlStreamReader.raiseError("Attribute \"LocalLogFileCloseAfterEachWrite\" (" + strAttr + ") is out of range");
                            }

                            // While receiving the log settings emitting the logSettingsChanged signal is blocked
                            // by setting the flag m_bOnReceivedDataUpdateInProcess to true to accumulate all changes
                            // and emit the signal just once. setLogSettings will check if the settings have been
                            // changed before taken them over. But setLogSettings will not emit the settings changed
                            // signal as the flag m_bOnReceivedDataUpdateInProcess is set. So we need to do this check
                            // here also and emit the signal.
                            if( bRemoteNameChanged || logServerSettings != m_logServerSettings )
                            {
                                m_bOnReceivedDataUpdateInProcess = true;
                                setLogSettings(logServerSettings);
                                m_bOnReceivedDataUpdateInProcess = false;
                                emit logSettingsChanged(this);
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
void CIpcLogClient::onIpcClientConnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    // Request the trace admin objects from the server:
    // Not necessary as the log server must send the settings,
    // the index tree and the cached data when the client connects.

    #if 0
    QString strMsg;

    // Select (query) the settings of the log server.
    strMsg  = systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
    strMsg += command2Str(MsgProtocol::ECommandSelect) + " ";
    strMsg += "<ServerSettings/>";

    sendData( str2ByteArr(strMsg) );

    // Select (query) the loggers.
    strMsg  = systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
    strMsg += command2Str(MsgProtocol::ECommandSelect) + " ";
    strMsg += "<Loggers/>";

    sendData( str2ByteArr(strMsg) );

    // Select (query) cached data. This query registers the client at
    // the server for online log data.
    strMsg  = systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
    strMsg += command2Str(MsgProtocol::ECommandSelect) + " ";
    strMsg += "<LogData/>";

    sendData( str2ByteArr(strMsg) );
    #endif

} // onIpcClientConnected

//------------------------------------------------------------------------------
void CIpcLogClient::onIpcClientDisconnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
}

/*==============================================================================
protected slots: // connected to the slots of the index tree
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogClient::onLoggersIdxTreeEntryChanged( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    if( m_bOnReceivedDataUpdateInProcess )
    {
        return;
    }

    CIdxTreeLocker idxTreeLocker(m_pLoggersIdxTree);

    CIdxTreeEntry* pTreeEntry = m_pLoggersIdxTree->findEntry(i_strKeyInTree);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->isLeave() )
        {
            sendLeave(
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeReq,
                /* cmd           */ MsgProtocol::ECommandUpdate,
                /* pLogger       */ dynamic_cast<CLogger*>(pTreeEntry) );
        }
    }
}
