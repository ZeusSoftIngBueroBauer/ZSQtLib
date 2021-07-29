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
CIpcTrcClient::CIpcTrcClient( const QString& i_strName ) :
//------------------------------------------------------------------------------
    CClient(
        /* strObjName             */ i_strName,
        /* bMultiThreadedAccess   */ false,
        /* pTrcMthFile            */ nullptr,
        /* iTrcMthFileDetailLevel */ ETraceDetailLevelNone ),
    m_trcServerSettings(),
    m_pTrcAdminObjIdxTree(nullptr),
    m_bOnReceivedDataUpdateInProcess(false)
{
    m_watchDogTimerSettings.m_bEnabled = false;

    m_pTrcAdminObjIdxTree = new CIdxTreeTrcAdminObjs(i_strName, this);

    // Set default ports depending on trace types
    //-------------------------------------------

    m_hostSettings.m_uRemotePort = 24763;

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
        /* szSignal     */ SIGNAL( treeEntryChanged(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTrcAdminObjIdxTreeEntryChanged(ZS::System::CIdxTree*, ZS::System::CAbstractIdxTreeEntry*) ),
        /* cnctType     */ Qt::DirectConnection ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CIpcTrcClient::~CIpcTrcClient()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pTrcAdminObjIdxTree;
    }
    catch(...)
    {
    }
    m_pTrcAdminObjIdxTree = nullptr;

} // dtor

/*==============================================================================
public: // instance methods to enable and disable the client and server
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcClient::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( m_trcServerSettings.m_bEnabled != i_bEnabled )
    {
        m_trcServerSettings.m_bEnabled = i_bEnabled;
        emit traceSettingsChanged(this);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<Server>";
            strMsg += "<Enabled>" + bool2Str(m_trcServerSettings.m_bEnabled) + "</Enabled>";
            strMsg += "</Server>";

            CRequest* pReq = sendData( str2ByteArr(strMsg) );

            if( !isAsynchronousRequest(pReq) )
            {
                pReq = nullptr; // deleted later by request queue
            }
        }
    }

} // setEnabled

//------------------------------------------------------------------------------
bool CIpcTrcClient::isEnabled() const
//------------------------------------------------------------------------------
{
    return m_trcServerSettings.m_bEnabled;
}

//------------------------------------------------------------------------------
void CIpcTrcClient::setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( m_trcServerSettings.m_bNewTrcAdminObjsEnabledAsDefault != i_bEnabled )
    {
        m_trcServerSettings.m_bNewTrcAdminObjsEnabledAsDefault = i_bEnabled;
        emit traceSettingsChanged(this);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<Server>";
            strMsg += "<NewTrcAdminObjsEnabledAsDefault>" + bool2Str(m_trcServerSettings.m_bNewTrcAdminObjsEnabledAsDefault) + "</NewTrcAdminObjsEnabledAsDefault>";
            strMsg += "</Server>";

            CRequest* pReq = sendData( str2ByteArr(strMsg) );

            if( !isAsynchronousRequest(pReq) )
            {
                pReq = nullptr; // deleted later by request queue
            }
        }
    }

} // setNewTrcAdminObjsEnabledAsDefault

//------------------------------------------------------------------------------
bool CIpcTrcClient::areNewTrcAdminObjsEnabledAsDefault() const
//------------------------------------------------------------------------------
{
    return m_trcServerSettings.m_bNewTrcAdminObjsEnabledAsDefault;
}

//------------------------------------------------------------------------------
void CIpcTrcClient::setNewTrcAdminObjsDefaultDetailLevel( int i_iDetailLevel )
//------------------------------------------------------------------------------
{
    if( m_trcServerSettings.m_iNewTrcAdminObjsDefaultDetailLevel != i_iDetailLevel )
    {
        m_trcServerSettings.m_iNewTrcAdminObjsDefaultDetailLevel = i_iDetailLevel;
        emit traceSettingsChanged(this);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<Server>";
            strMsg += "<NewTrcAdminObjsDefaultDetailLevel>" + QString::number(m_trcServerSettings.m_iNewTrcAdminObjsDefaultDetailLevel) + "</NewTrcAdminObjsDefaultDetailLevel>";
            strMsg += "</Server>";

            CRequest* pReq = sendData( str2ByteArr(strMsg) );

            if( !isAsynchronousRequest(pReq) )
            {
                pReq = nullptr; // deleted later by request queue
            }
        }
    }

} // setNewTrcAdminObjsDefaultDetailLevel

//------------------------------------------------------------------------------
int CIpcTrcClient::getNewTrcAdminObjsDefaultDetailLevel() const
//------------------------------------------------------------------------------
{
    return m_trcServerSettings.m_iNewTrcAdminObjsDefaultDetailLevel;
}

//------------------------------------------------------------------------------
void CIpcTrcClient::setCacheTrcDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    if( m_trcServerSettings.m_bCacheDataIfNotConnected != i_bCacheData )
    {
        m_trcServerSettings.m_bCacheDataIfNotConnected = i_bCacheData;
        emit traceSettingsChanged(this);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<Server>";
            strMsg += "<CacheDataIfNotConnected>" + bool2Str(m_trcServerSettings.m_bCacheDataIfNotConnected) + "</CacheDataIfNotConnected>";
            strMsg += "</Server>";

            CRequest* pReq = sendData( str2ByteArr(strMsg) );

            if( !isAsynchronousRequest(pReq) )
            {
                pReq = nullptr; // deleted later by request queue
            }
        }
    }

} // setCacheTrcDataIfNotConnected

//------------------------------------------------------------------------------
bool CIpcTrcClient::getCacheTrcDataIfNotConnected() const
//------------------------------------------------------------------------------
{
    return m_trcServerSettings.m_bCacheDataIfNotConnected;
}

//------------------------------------------------------------------------------
void CIpcTrcClient::setCacheTrcDataMaxArrLen( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    if( i_iMaxArrLen > 0 && m_trcServerSettings.m_iCacheDataMaxArrLen != i_iMaxArrLen )
    {
        m_trcServerSettings.m_iCacheDataMaxArrLen = i_iMaxArrLen;
        emit traceSettingsChanged(this);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<Server>";
            strMsg += "<CacheDataMaxArrLen>" + QString::number(m_trcServerSettings.m_iCacheDataMaxArrLen) + "</CacheDataMaxArrLen>";
            strMsg += "</Server>";

            CRequest* pReq = sendData( str2ByteArr(strMsg) );

            if( !isAsynchronousRequest(pReq) )
            {
                pReq = nullptr; // deleted later by request queue
            }
        }
    }

} // setCacheTrcDataMaxArrLen

//------------------------------------------------------------------------------
int CIpcTrcClient::getCacheTrcDataMaxArrLen() const
//------------------------------------------------------------------------------
{
    return m_trcServerSettings.m_iCacheDataMaxArrLen;
}

//------------------------------------------------------------------------------
void CIpcTrcClient::setAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    if( m_trcServerSettings.m_strAdminObjFileAbsFilePath != i_strAbsFilePath )
    {
        m_trcServerSettings.m_strAdminObjFileAbsFilePath = i_strAbsFilePath;
        emit traceSettingsChanged(this);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            //QString strMsg;

            //strMsg += systemMsgType2Str(ESystemMsgTypeReq) + " ";
            //strMsg += command2Str(ECommandUpdate) + " ";
            //strMsg += "<Server>";
            //strMsg += "<AdminObjFileAbsFilePath>" + m_strAdminObjFileAbsFilePath + "</AdminObjFileAbsFilePath>";
            //strMsg += "</Server>";

            //CRequest* pReq = sendData( str2ByteArr(strMsg) );

            //if( !isAsynchronousRequest(pReq) )
            //{
            //    pReq = nullptr; // deleted later by request queue
            //}
        }
    }

} // setAdminObjFileAbsoluteFilePath

//------------------------------------------------------------------------------
QString CIpcTrcClient::getAdminObjFileAbsoluteFilePath() const
//------------------------------------------------------------------------------
{
    return m_trcServerSettings.m_strAdminObjFileAbsFilePath;
}

//------------------------------------------------------------------------------
void CIpcTrcClient::setUseLocalTrcFile( bool i_bUse )
//------------------------------------------------------------------------------
{
    if( m_trcServerSettings.m_bUseLocalTrcFile != i_bUse )
    {
        m_trcServerSettings.m_bUseLocalTrcFile = i_bUse;
        emit traceSettingsChanged(this);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<Server>";
            strMsg += "<UseLocalTrcFile>" + bool2Str(m_trcServerSettings.m_bUseLocalTrcFile) + "</UseLocalTrcFile>";
            strMsg += "</Server>";

            CRequest* pReq = sendData( str2ByteArr(strMsg) );

            if( !isAsynchronousRequest(pReq) )
            {
                pReq = nullptr; // deleted later by request queue
            }
        }
    }

} // setUseLocalTrcFile

//------------------------------------------------------------------------------
bool CIpcTrcClient::isLocalTrcFileUsed() const
//------------------------------------------------------------------------------
{
    return m_trcServerSettings.m_bUseLocalTrcFile;
}

//------------------------------------------------------------------------------
void CIpcTrcClient::setLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    if( m_trcServerSettings.m_strLocalTrcFileAbsFilePath != i_strAbsFilePath )
    {
        m_trcServerSettings.m_strLocalTrcFileAbsFilePath = i_strAbsFilePath;
        emit traceSettingsChanged(this);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            //QString strMsg;

            //strMsg += systemMsgType2Str(ESystemMsgTypeReq) + " ";
            //strMsg += command2Str(ECommandUpdate) + " ";
            //strMsg += "<Server>";
            //strMsg += "<LocalTrcFileAbsFilePath>" + m_strLocalTrcFileAbsFilePath + "</LocalTrcFileAbsFilePath>";
            //strMsg += "</Server>";

            //CRequest* pReq = sendData( str2ByteArr(strMsg) );

            //if( !isAsynchronousRequest(pReq) )
            //{
            //    pReq = nullptr; // deleted later by request queue
            //}
        }
    }

} // setLocalTrcFileAbsFilePath

//------------------------------------------------------------------------------
QString CIpcTrcClient::getLocalTrcFileAbsoluteFilePath() const
//------------------------------------------------------------------------------
{
    return m_trcServerSettings.m_strLocalTrcFileAbsFilePath;
}

//------------------------------------------------------------------------------
void CIpcTrcClient::setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    if( m_trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite != i_bCloseFile )
    {
        m_trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite = i_bCloseFile;
        emit traceSettingsChanged(this);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeReq) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<Server>";
            strMsg += "<LocalTrcFileCloseAfterEachWrite>" + bool2Str(m_trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite) + "</LocalTrcFileCloseAfterEachWrite>";
            strMsg += "</Server>";

            CRequest* pReq = sendData( str2ByteArr(strMsg) );

            if( !isAsynchronousRequest(pReq) )
            {
                pReq = nullptr; // deleted later by request queue
            }
        }
    }

} // setLocalTrcFileCloseFileAfterEachWrite

//------------------------------------------------------------------------------
bool CIpcTrcClient::getLocalTrcFileCloseFileAfterEachWrite() const
//------------------------------------------------------------------------------
{
    return m_trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite;
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
    if( i_pTrcAdminObj != nullptr && isConnected() )
    {
        QString strMsg;
        //QString strNameSpace = i_pTrcAdminObj->getNameSpace();
        //QString strClassName = i_pTrcAdminObj->getClassName();
        //QString strObjName = i_pTrcAdminObj->getObjectName();

        //if( strNameSpace.contains('<') )
        //{
        //    strNameSpace.replace("<","&lt;");
        //}
        //if( strNameSpace.contains('>') )
        //{
        //    strNameSpace.replace(">","&gt;");
        //}
        //if( strClassName.contains('<') )
        //{
        //    strClassName.replace("<","&lt;");
        //}
        //if( strClassName.contains('>') )
        //{
        //    strClassName.replace(">","&gt;");
        //}
        //if( strObjName.contains('<') )
        //{
        //    strObjName.replace("<","&lt;");
        //}
        //if( strObjName.contains('>') )
        //{
        //    strObjName.replace(">","&gt;");
        //}

        strMsg += systemMsgType2Str(i_systemMsgType) + " ";
        strMsg += command2Str(i_cmd) + " ";
        strMsg += "<TrcAdminObj ";
        //strMsg += " NameSpace=\"" + strNameSpace + "\"";
        //strMsg += " ClassName=\"" + strClassName + "\"";
        //strMsg += " ObjName=\"" + strObjName + "\"";
        //strMsg += " ThreadName=\"" + strThreadName + "\"";
        strMsg += " ObjId=\"" + QString::number(i_pTrcAdminObj->indexInTree()) + "\"";
        strMsg += " Enabled=\"" + CEnumEnabled::toString(i_pTrcAdminObj->getEnabled()) + "\"";
        strMsg += " DetailLevel=\"" + QString::number(i_pTrcAdminObj->getTraceDetailLevel()) + "\"";
        //strMsg += " RefCount=" + QString::number(i_pTrcAdminObj->getRefCount());
        strMsg += "/>";

        sendData( str2ByteArr(strMsg) );

    } // if( i_pTrcAdminObj != nullptr && isConnected() )

} // sendAdminObj

//------------------------------------------------------------------------------
void CIpcTrcClient::sendNameSpace(
    MsgProtocol::TSystemMsgType i_systemMsgType,
    MsgProtocol::TCommand       i_cmd,
    CBranchIdxTreeEntry*        i_pBranch,
    EEnabled                    i_enabled,
    int                         i_iDetailLevel )
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
        if( i_iDetailLevel >= 0 )
        {
            strMsg += " DetailLevel=\"" + QString::number(i_iDetailLevel) + "\"";
        }
        strMsg += "/>";

        sendData( str2ByteArr(strMsg) );

    } // if( i_pBranch != nullptr && isConnected() )

} // sendNameSpace

/*==============================================================================
public: // overridables of base class Client
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcClient::onReceivedData( const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
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

        if( xmlStreamTokenType == QXmlStreamReader::StartDocument )
        {
            xmlStreamTokenType = xmlStreamReader.readNext();
        }

        if( xmlStreamTokenType == QXmlStreamReader::StartElement )
        {
            strElemName = xmlStreamReader.name().toString();

            if( strElemName == "ServerSettings" )
            {
                if( xmlStreamReader.attributes().hasAttribute("Enabled") )
                {
                    strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                    bVal = str2Bool(strAttr, &bOk);
                    if( bOk ) setEnabled(bVal);
                    else xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") is out of range");
                }
                else if( xmlStreamReader.attributes().hasAttribute("NewTrcAdminObjsEnabledAsDefault") )
                {
                    strAttr = xmlStreamReader.attributes().value("NewTrcAdminObjsEnabledAsDefault").toString();
                    bVal = str2Bool(strAttr, &bOk);
                    if( bOk ) setNewTrcAdminObjsEnabledAsDefault(bVal);
                    else xmlStreamReader.raiseError("Attribute \"NewTrcAdminObjsEnabledAsDefault\" (" + strAttr + ") is out of range");
                }
                if( xmlStreamReader.attributes().hasAttribute("NewTrcAdminObjsDefaultDetailLevel") )
                {
                    strAttr = xmlStreamReader.attributes().value("NewTrcAdminObjsDefaultDetailLevel").toString();
                    iVal = strAttr.toInt(&bOk);
                    if( bOk ) setNewTrcAdminObjsDefaultDetailLevel(iVal);
                    else xmlStreamReader.raiseError("Attribute \"NewTrcAdminObjsDefaultDetailLevel\" (" + strAttr + ") is out of range");
                }
                if( xmlStreamReader.attributes().hasAttribute("CacheDataIfNotConnected") )
                {
                    strAttr = xmlStreamReader.attributes().value("CacheDataIfNotConnected").toString();
                    bVal = str2Bool(strAttr, &bOk);
                    if( bOk ) setCacheTrcDataIfNotConnected(bVal);
                    else xmlStreamReader.raiseError("Attribute \"CacheDataIfNotConnected\" (" + strAttr + ") is out of range");
                }
                if( xmlStreamReader.attributes().hasAttribute("CacheDataMaxArrLen") )
                {
                    strAttr = xmlStreamReader.attributes().value("CacheDataMaxArrLen").toString();
                    iVal = strAttr.toInt(&bOk);
                    if( bOk ) setCacheTrcDataMaxArrLen(iVal);
                    else xmlStreamReader.raiseError("Attribute \"CacheDataMaxArrLen\" (" + strAttr + ") is out of range");
                }
                if( xmlStreamReader.attributes().hasAttribute("AdminObjFileAbsFilePath") )
                {
                    strAttr = xmlStreamReader.attributes().value("AdminObjFileAbsFilePath").toString();
                    setAdminObjFileAbsoluteFilePath(strAttr);
                }
                if( xmlStreamReader.attributes().hasAttribute("UseLocalTrcFile") )
                {
                    strAttr = xmlStreamReader.attributes().value("UseLocalTrcFile").toString();
                    bVal = str2Bool(strAttr, &bOk);
                    if( bOk ) setUseLocalTrcFile(bVal);
                    else xmlStreamReader.raiseError("Attribute \"UseLocalTrcFile\" (" + strAttr + ") is out of range");
                }
                if( xmlStreamReader.attributes().hasAttribute("LocalTrcFileAbsFilePath") )
                {
                    strAttr = xmlStreamReader.attributes().value("LocalTrcFileAbsFilePath").toString();
                    setLocalTrcFileAbsoluteFilePath(strAttr);
                }
                if( xmlStreamReader.attributes().hasAttribute("LocalTrcFileCloseAfterEachWrite") )
                {
                    strAttr = xmlStreamReader.attributes().value("LocalTrcFileCloseAfterEachWrite").toString();
                    bVal = str2Bool(strAttr, &bOk);
                    if( bOk ) setLocalTrcFileCloseFileAfterEachWrite(bVal);
                    else xmlStreamReader.raiseError("Attribute \"LocalTrcFileCloseAfterEachWrite\" (" + strAttr + ") is out of range");
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
                        CBranchIdxTreeEntry* pBranch = m_pTrcAdminObjIdxTree->getBranch(idxInTree);

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
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Name") )
                    {
                        strObjName = xmlStreamReader.attributes().value("Name").toString();
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

                        if( strObjName.isEmpty() && pTrcAdminObj == nullptr )
                        {
                            xmlStreamReader.raiseError("There is no trace admin object at tree index " + QString::number(idxInTree));
                        }
                        else if( !strObjName.isEmpty() && pTrcAdminObj == nullptr )
                        {
                            pTrcAdminObj = m_pTrcAdminObjIdxTree->insertTraceAdminObj(iParentPranchIdxInTree, strObjName, idxInTree);

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
                        else if( pTrcAdminObj != nullptr )
                        {
                            bool bSignalsBlocked = pTrcAdminObj->blockTreeEntryChangedSignal(true);

                            if( !strThreadName.isEmpty() ) pTrcAdminObj->setObjectThreadName(strThreadName);
                            if( enabled != EEnabled::Undefined ) pTrcAdminObj->setEnabled(enabled);
                            if( iDetailLevel >= 0 ) pTrcAdminObj->setTraceDetailLevel(iDetailLevel);
                            if( iRefCount >= 0 ) pTrcAdminObj->setRefCount(iRefCount);

                            pTrcAdminObj->blockTreeEntryChangedSignal(bSignalsBlocked);
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
        } // if( xmlStreamTokenType == QXmlStreamReader::StartElement )

        else
        {
            xmlStreamReader.raiseError("Invalid XML command");
        }

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
    // Before the index tree can be cleared the ref counters of the trace admin objects
    // got to be reset to avoid an err log entry (object ref counter is not 0 in dtor).

    resetTrcAdminRefCounters(m_pTrcAdminObjIdxTree->root());

    m_pTrcAdminObjIdxTree->clear();

} // onIpcClientDisconnected

/*==============================================================================
protected slots: // connected to the slots of the trace admin object pool model
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcTrcClient::onTrcAdminObjIdxTreeEntryChanged(
    CIdxTree*              /*i_pIdxTree*/,
    CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    if( m_bOnReceivedDataUpdateInProcess )
    {
        return;
    }

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
void CIpcTrcClient::resetTrcAdminRefCounters( ZS::System::CBranchIdxTreeEntry* i_pBranch )
//------------------------------------------------------------------------------
{
    CAbstractIdxTreeEntry* pTreeEntry;
    int                    idxEntry;

    for( idxEntry = i_pBranch->count()-1; idxEntry >= 0; --idxEntry )
    {
        pTreeEntry = i_pBranch->at(idxEntry);

        if( pTreeEntry != nullptr )
        {
            if( pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
            {
                resetTrcAdminRefCounters(dynamic_cast<CBranchIdxTreeEntry*>(pTreeEntry));
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
