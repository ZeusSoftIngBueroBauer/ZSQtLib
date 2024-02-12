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

#include "ZSIpcLog/ZSIpcLogServer.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysLogFile.h"
#include "ZSSys/ZSSysLogger.h"
#include "ZSSys/ZSSysLoggerIdxTree.h"
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysRequest.h"

#include <QtCore/qcoreapplication.h>
#include <QtCore/qmutex.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Log;


/*******************************************************************************
class CIpcLogServer : public ZS::System::CLogServer
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the address of the log server.

    This method does neither create an instance of the class nor increments the
    reference counter. If no instance has been created yet the method returns nullptr.

    If you just need to access the already existing instance and you can be sure
    that the instance is already existing this method should be preferred to the
    createInstance call as this method does not affect the reference counter and
    there is no need to call releaseInstance later on.

    @note After a getInstance call a releaseInstance MUST NOT be called.

    @return Pointer to log server or nullptr, if the instance has not been created yet.
*/
CIpcLogServer* CIpcLogServer::GetInstance()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CIpcLogServer* pIpcLogServer = nullptr;

    CLogServer* pLogServer = CLogServer::GetInstance();

    if( pLogServer != nullptr )
    {
        pIpcLogServer = dynamic_cast<CIpcLogServer*>(pLogServer);
    }
    return pIpcLogServer;
}

//------------------------------------------------------------------------------
/*! Creates the log server if the server is not already existing.

    If the server is already existing the reference to the existing
    server is returned and a reference counter is incremented.

    @return Pointer to log server instance.
*/
CIpcLogServer* CIpcLogServer::CreateInstance()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CLogServer* pLogServer = s_pTheInst;

    CIpcLogServer* pIpcLogServer = nullptr;

    if( pLogServer == nullptr )
    {
        pIpcLogServer = new CIpcLogServer();
    }
    else
    {
        pIpcLogServer = dynamic_cast<CIpcLogServer*>(pLogServer);

        if( pIpcLogServer == nullptr )
        {
            throw CException(__FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated, "ZS::Log::CLogServer");
        }
    }

    if( pIpcLogServer != nullptr )
    {
        pIpcLogServer->incrementRefCount();
    }

    // The ctor adds the reference to the instance to the hash.
    // If the ctor itself calls methods of other classes which again recursively
    // call "GetInstance" this way "GetInstance" does not return null but the
    // pointer to the server instance currently beeing created.
    // But of course this requires special caution as within the ctor it must
    // be assured that recursively accessed instance members are already valid.
    //s_hshpInstances[i_strName] = pIpcLogServer;

    return pIpcLogServer;
}

//------------------------------------------------------------------------------
/*! @brief Releases the log server.

    Before invoking this method a reference to the instance must have been
    retrieved with a createInstance call.

    This method decrements the reference counter of the instance.
    If the reference counter reaches 0 the instance will be destroyed.

    @note A reference returned by getInstance MUST NOT be freed.
*/
void CIpcLogServer::ReleaseInstance()
//------------------------------------------------------------------------------
{
    CLogServer::ReleaseInstance();
}

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of the Ipc Log Server.

    To avoid multiple inheritance from QObject the server is not derived from
    CIpcServer but aggregates an instance of the Ipc server.

    The IpcServer of the log server uses the block type "<Len>[Data]".
*/
CIpcLogServer::CIpcLogServer() :
//------------------------------------------------------------------------------
    CLogServer(),
    m_pIpcServer(nullptr),
    m_bIsBeingDestroyed(false),
    m_ariSocketIdsConnectedClients(),
    m_bOnReceivedDataUpdateInProcess(false),
    m_pMtxListLogDataCached(nullptr),
    m_iLogDataCachedCount(0),
    m_arpLogDataCached()
{
    // The log server must aggregate the IpcServer to avoid multiple inheritance of QObject.
    m_pIpcServer = new CServer(objectName(), true);
    SServerHostSettings hostSettings = m_pIpcServer->getHostSettings();
    hostSettings.m_uLocalPort = 24762; // Default listen port for IpcLogServer
    m_pIpcServer->setHostSettings(hostSettings);

    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    m_pMtxListLogDataCached = new QRecursiveMutex();
    #else
    m_pMtxListLogDataCached = new QMutex(QMutex::Recursive);
    #endif

    // Need direct connections to signals of index tree.
    // If in another thread a trace admin object is created, removed or modified
    // the corresponding message must be send by the server to the connected clients
    // before sending log data. If the signals would be queued the client may receive
    // log data for logger objects which may not yet exist.
    QObject::connect(
        m_pLoggersIdxTree, &CIdxTree::treeEntryAdded,
        this, &CIpcLogServer::onLoggersIdxTreeEntryAdded,
        Qt::DirectConnection);
    QObject::connect(
        m_pLoggersIdxTree, &CIdxTree::treeEntryAboutToBeRemoved,
        this, &CIpcLogServer::onLoggersIdxTreeEntryAboutToBeRemoved,
        Qt::DirectConnection);
    QObject::connect(
        m_pLoggersIdxTree, &CIdxTreeLoggers::treeEntryChanged,
        this, &CIpcLogServer::onLoggersIdxTreeEntryChanged,
        Qt::DirectConnection);

    QObject::connect(
        m_pIpcServer, &CServer::connected,
        this, &CIpcLogServer::onIpcServerConnected);
    QObject::connect(
        m_pIpcServer, &CServer::disconnected,
        this, &CIpcLogServer::onIpcServerDisconnected);
    QObject::connect(
        m_pIpcServer, &CServer::receivedData,
        this, &CIpcLogServer::onIpcServerReceivedData);

} // ctor

//------------------------------------------------------------------------------
CIpcLogServer::~CIpcLogServer()
//------------------------------------------------------------------------------
{
    m_bIsBeingDestroyed = true;

    QObject::disconnect(
        m_pLoggersIdxTree, &CIdxTree::treeEntryAdded,
        this, &CIpcLogServer::onLoggersIdxTreeEntryAdded);
    QObject::disconnect(
        m_pLoggersIdxTree, &CIdxTree::treeEntryAboutToBeRemoved,
        this, &CIpcLogServer::onLoggersIdxTreeEntryAboutToBeRemoved);
    QObject::disconnect(
        m_pLoggersIdxTree, &CIdxTreeLoggers::treeEntryChanged,
        this, &CIpcLogServer::onLoggersIdxTreeEntryChanged);

    for( int idx = 0; idx < m_iLogDataCachedCount; idx++ )
    {
        try
        {
            delete m_arpLogDataCached[idx];
        }
        catch(...)
        {
        }
        m_arpLogDataCached[idx] = nullptr;
    }

    try
    {
        delete m_pIpcServer;
    }
    catch(...)
    {
    }

    try
    {
        delete m_pMtxListLogDataCached;
    }
    catch(...)
    {
    }

    // I am the parent of the IpcServer. The IpcServer will be destroyed by Qt.
    m_pIpcServer = nullptr;
    m_bIsBeingDestroyed = false;
    //m_ariSocketIdsConnectedClients.clear();
    m_bOnReceivedDataUpdateInProcess = false;
    m_pMtxListLogDataCached = nullptr;
    m_iLogDataCachedCount = 0;
    m_arpLogDataCached.clear();

} // dtor

/*==============================================================================
public: // overridables of base class CLogServer
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogServer::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( i_bEnabled != m_logSettings.m_bEnabled )
    {
        CLogServer::setEnabled(i_bEnabled);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings Enabled=\"" + bool2Str(m_logSettings.m_bEnabled) + "\"/>";

            sendData(ESocketIdAllSockets, str2ByteArr(strMsg));
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Checks whether logging is active.

    Logging is active if logging is enabled at all (flag enabled of the
    log settings) and either

    - if the local log file is used or
    - if output to remote client is activated.
      Output to the remote client is activated if
      - caching data is enabled or
      - if the flag to use the IpcServer is enabled and if a client is
        connected to the IpcServer.

    @return true if logging is active, false otherwise.
*/
bool CIpcLogServer::isActive() const
//------------------------------------------------------------------------------
{
    return isEnabled() && (isLocalLogFileActive() || getCacheLogDataIfNotConnected() || (isIpcServerUsed() && isConnected()));
}

/*==============================================================================
public: // overridables of base class CLogServer
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogServer::setNewLoggersEnabledAsDefault( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_bNewLoggersEnabledAsDefault != i_bEnabled )
    {
        CLogServer::setNewLoggersEnabledAsDefault(i_bEnabled);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings NewLoggersEnabledAsDefault=\""
                   + bool2Str(m_logSettings.m_bNewLoggersEnabledAsDefault) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }
}

//------------------------------------------------------------------------------
void CIpcLogServer::setNewLoggersDefaultDetailLevel( ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_eNewLoggersDefaultDetailLevel != i_eDetailLevel )
    {
        CLogServer::setNewLoggersDefaultDetailLevel(i_eDetailLevel);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings NewLoggersDefaultDetailLevel=\""
                   + CEnumLogDetailLevel(m_logSettings.m_eNewLoggersDefaultDetailLevel).toString() + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }
}

/*==============================================================================
public: // overridables of base class CLogServer
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogServer::setLoggerFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_strLoggerFileAbsFilePath != i_strAbsFilePath )
    {
        CLogServer::setLoggerFileAbsoluteFilePath(i_strAbsFilePath);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings LoggerFileAbsFilePath=\""
                   + m_logSettings.m_strLoggerFileAbsFilePath + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }
}

/*==============================================================================
public: // overridables of base class CLogServer
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogServer::setUseLocalLogFile( bool i_bUse )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_bUseLocalLogFile != i_bUse )
    {
        CLogServer::setUseLocalLogFile(i_bUse);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings UseLocalLogFile=\"" + bool2Str(m_logSettings.m_bUseLocalLogFile) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }
}

//------------------------------------------------------------------------------
void CIpcLogServer::setLocalLogFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( (m_logSettings.m_strLocalLogFileAbsFilePath != i_strAbsFilePath)
     || (m_pLogFile == nullptr && !m_logSettings.m_strLocalLogFileAbsFilePath.isEmpty()) )
    {
        CLogServer::setLocalLogFileAbsoluteFilePath(i_strAbsFilePath);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings LocalLogFileAbsFilePath=\""
                   + m_logSettings.m_strLocalLogFileAbsFilePath + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }
}

//------------------------------------------------------------------------------
void CIpcLogServer::setLocalLogFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_iLocalLogFileAutoSaveInterval_ms != i_iAutoSaveInterval_ms )
    {
        CLogServer::setLocalLogFileAutoSaveIntervalInMs(i_iAutoSaveInterval_ms);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings LocalLogFileAutoSaveInterval_ms=\""
                   + QString::number(m_logSettings.m_iLocalLogFileAutoSaveInterval_ms) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }
}

//------------------------------------------------------------------------------
void CIpcLogServer::setLocalLogFileSubFileCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_iLocalLogFileSubFileCountMax != i_iCountMax )
    {
        CLogServer::setLocalLogFileSubFileCountMax(i_iCountMax);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings LocalLogFileSubFileCountMax=\""
                   + QString::number(m_logSettings.m_iLocalLogFileSubFileCountMax) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }
}

//------------------------------------------------------------------------------
void CIpcLogServer::setLocalLogFileSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_iLocalLogFileSubFileLineCountMax != i_iCountMax )
    {
        CLogServer::setLocalLogFileSubFileLineCountMax(i_iCountMax);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings LocalLogFileSubFileLineCountMax=\""
                   + QString::number(m_logSettings.m_iLocalLogFileSubFileLineCountMax) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }
}

//------------------------------------------------------------------------------
void CIpcLogServer::setLocalLogFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_bLocalLogFileCloseFileAfterEachWrite != i_bCloseFile )
    {
        CLogServer::setLocalLogFileCloseFileAfterEachWrite(i_bCloseFile);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings LocalLogFileCloseAfterEachWrite=\""
                   + bool2Str(m_logSettings.m_bLocalLogFileCloseFileAfterEachWrite) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogServer::setUseIpcServer( bool i_bUse )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_bUseIpcServer != i_bUse )
    {
        CLogServer::setUseIpcServer(i_bUse);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings UseIpcServer=\"" + bool2Str(m_logSettings.m_bUseIpcServer) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }
}

//------------------------------------------------------------------------------
void CIpcLogServer::setCacheLogDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_bCacheDataIfNotConnected != i_bCacheData )
    {
        CLogServer::setCacheLogDataIfNotConnected(i_bCacheData);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings CacheDataIfNotConnected=\""
                   + bool2Str(m_logSettings.m_bCacheDataIfNotConnected) + "\"/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }
}

//------------------------------------------------------------------------------
void CIpcLogServer::setCacheLogDataMaxArrLen( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_iCacheDataMaxArrLen != i_iMaxArrLen )
    {
        CLogServer::setCacheLogDataMaxArrLen(i_iMaxArrLen);

        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings CacheDataMaxArrLen="
                   + QString::number(m_logSettings.m_iCacheDataMaxArrLen) + "/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }
    }
}

/*==============================================================================
public: // instance methods to enable and disable the client and server
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogServer::setLogSettings( const SLogServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings != i_settings )
    {
        // If not called on receiving settings from the server and if the client is connected ..
        if( !m_bOnReceivedDataUpdateInProcess && isConnected() )
        {
            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandUpdate) + " ";
            strMsg += "<ServerSettings";

            if( m_logSettings.m_bEnabled != i_settings.m_bEnabled )
            {
                strMsg += " Enabled=\"" + bool2Str(i_settings.m_bEnabled) + "\"";
            }
            if( m_logSettings.m_strLoggerFileAbsFilePath != i_settings.m_strLoggerFileAbsFilePath )
            {
                strMsg += " LoggerFileAbsFilePath=\"" + i_settings.m_strLoggerFileAbsFilePath + "\"";
            }
            if( m_logSettings.m_bNewLoggersEnabledAsDefault != i_settings.m_bNewLoggersEnabledAsDefault )
            {
                strMsg += " NewLoggersEnabledAsDefault=\"" + bool2Str(i_settings.m_bNewLoggersEnabledAsDefault) + "\"";
            }
            if( m_logSettings.m_eNewLoggersDefaultDetailLevel != i_settings.m_eNewLoggersDefaultDetailLevel )
            {
                strMsg += " NewLoggersDefaultDetailLevel=\""
                       + CEnumLogDetailLevel(i_settings.m_eNewLoggersDefaultDetailLevel).toString() + "\"";
            }
            if( m_logSettings.m_bUseIpcServer != i_settings.m_bUseIpcServer )
            {
                strMsg += " UseIpcServer=\"" + bool2Str(i_settings.m_bUseIpcServer) + "\"";
            }
            if( m_logSettings.m_bCacheDataIfNotConnected != i_settings.m_bCacheDataIfNotConnected )
            {
                strMsg += " CacheDataIfNotConnected=\"" + bool2Str(i_settings.m_bCacheDataIfNotConnected) + "\"";
            }
            if( m_logSettings.m_iCacheDataMaxArrLen != i_settings.m_iCacheDataMaxArrLen )
            {
                strMsg += " CacheDataMaxArrLen =\"" + QString::number(i_settings.m_iCacheDataMaxArrLen) + "\"";
            }
            if( m_logSettings.m_bUseLocalLogFile != i_settings.m_bUseLocalLogFile )
            {
                strMsg += " UseLocalLogFile=\"" + bool2Str(i_settings.m_bUseLocalLogFile) + "\"";
            }
            if( m_logSettings.m_strLocalLogFileAbsFilePath != i_settings.m_strLocalLogFileAbsFilePath )
            {
                strMsg += " LocalLogFileAbsFilePath=\"" + i_settings.m_strLocalLogFileAbsFilePath + "\"";
            }
            if( m_logSettings.m_iLocalLogFileAutoSaveInterval_ms != i_settings.m_iLocalLogFileAutoSaveInterval_ms )
            {
                strMsg += " LocalLogFileAutoSaveInterval_ms=\"" + QString::number(i_settings.m_iLocalLogFileAutoSaveInterval_ms) + "\"";
            }
            if( m_logSettings.m_iLocalLogFileSubFileCountMax != i_settings.m_iLocalLogFileSubFileCountMax )
            {
                strMsg += " LocalLogFileSubFileCountMax=\"" + QString::number(i_settings.m_iLocalLogFileSubFileCountMax) + "\"";
            }
            if( m_logSettings.m_iLocalLogFileSubFileLineCountMax != i_settings.m_iLocalLogFileSubFileLineCountMax )
            {
                strMsg += " LocalLogFileSubFileLineCountMax=\"" + QString::number(i_settings.m_iLocalLogFileSubFileLineCountMax) + "\"";
            }
            if( m_logSettings.m_bLocalLogFileCloseFileAfterEachWrite != i_settings.m_bLocalLogFileCloseFileAfterEachWrite )
            {
                strMsg += " LocalLogFileCloseAfterEachWrite=\"" + bool2Str(i_settings.m_bLocalLogFileCloseFileAfterEachWrite) + "\"";
            }
            strMsg += "/>";

            sendData( ESocketIdAllSockets, str2ByteArr(strMsg) );
        }

        CLogServer::setLogSettings(i_settings);
    }
} // setLogSettings

/*==============================================================================
public: // overridables of base class CLogServer
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a log entry for the given log level.

    This method overrides the base implementation of class ZS::System::CLogServer.
    In addition to write to a local log file the log entry will also be sent to
    a connected Log client.

    @see ZS::System::CLogServer::log

    @param i_eFilterDetailLevel [in]
        If the given filter detail level is not None and is equal or greater
        than the current detail level of the log server the log entry will
        be added the log file.
    @param i_strLogEntry [in] String to be logged.
*/
void CIpcLogServer::log( ELogDetailLevel i_eFilterDetailLevel, const QString& i_strLogEntry )
//------------------------------------------------------------------------------
{
    CIpcLogServer::log(m_pLogger, i_eFilterDetailLevel, i_strLogEntry);
}

//------------------------------------------------------------------------------
/*! @brief Creates a log entry for the given log level.

    This method overrides the base implementation of class ZS::System::CLogServer.
    In addition to write to a local log file the log entry will also be sent to
    a connected Log client.

    @see ZS::System::CLogServer::log


    @param i_pLogger [in]
        Logger to be used for formatting the log entry.
    @param i_eFilterDetailLevel [in]
        If the given filter detail level is not None and is equal or greater
        than the current detail level of the log server the log entry will
        be added the log file.
    @param i_strLogEntry [in] String to be logged.
*/
void CIpcLogServer::log(
    CLogger*        i_pLogger,
    ELogDetailLevel i_eFilterDetailLevel,
    const QString&  i_strLogEntry )
//------------------------------------------------------------------------------
{
    if( m_bIsBeingDestroyed )
    {
        return;
    }

    QMutexLocker mtxLocker(&s_mtx);

    if( isActive() && i_pLogger->isActive(i_eFilterDetailLevel)
     && !i_pLogger->isSuppressedByDataFilter(i_strLogEntry) )
    {
        SLogData* pLogData = new SLogData(
            /* strThreadName */ i_pLogger->addThreadName() ? GetCurrentThreadName() : "",
            /* strDateTime   */ i_pLogger->addDateTime() ? QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz") : "",
            /* fSysTimeInSec */ i_pLogger->addSystemTime() ? Time::getProcTimeInSec() : -1.0,
            /* strNameSpace  */ i_pLogger->getNameSpace(),
            /* strClassName  */ i_pLogger->getClassName(),
            /* strObjName    */ i_pLogger->getObjectName(),
            /* strEntry      */ i_strLogEntry );

        if( m_logSettings.m_bUseLocalLogFile && m_pLogFile != nullptr )
        {
            m_pLogFile->addEntry(pLogData->toPlainString());
        }

        bool bAdd2Cache = false;

        if( m_logSettings.m_bCacheDataIfNotConnected && m_logSettings.m_iCacheDataMaxArrLen > 0 )
        {
            // Please note that only the first connected client will receive the cached data.

            // If no client is connected ..
            if( m_ariSocketIdsConnectedClients.length() == 0 )
            {
                // .. the data will be cached.
                bAdd2Cache = true;
            }
        }

        if( bAdd2Cache )
        {
            // ... temporarily store the log message until a client connects to the log server.
            QMutexLocker mutexLocker(m_pMtxListLogDataCached);

            int idx;

            if( m_iLogDataCachedCount == m_arpLogDataCached.size() )
            {
                const int ciMaxArrLen   = m_logSettings.m_iCacheDataMaxArrLen;
                const int ciRemoveCount = (ciMaxArrLen / 5) + 1; // 20 % (but at least one)

                // To save memory not more than MaxArrLen entries will be temporarily stored:
                if( m_iLogDataCachedCount < ciMaxArrLen )
                {
                    m_arpLogDataCached.reserve(ciMaxArrLen);

                    for( idx = m_iLogDataCachedCount; idx < m_arpLogDataCached.size(); idx++ )
                    {
                        m_arpLogDataCached[idx] = nullptr;
                    }
                }
                // If MaxArrLen or more entries have already been stored ...
                else
                {
                    // .. the oldest entries will be removed.
                    for( idx = 0; idx < ciRemoveCount; idx++ )
                    {
                        delete m_arpLogDataCached[idx];
                        m_arpLogDataCached[idx] = nullptr;
                    }
                    for( idx = 0; idx < m_iLogDataCachedCount-ciRemoveCount; idx++ )
                    {
                        m_arpLogDataCached[idx] = m_arpLogDataCached[idx+ciRemoveCount];
                    }
                    for( ; idx < m_arpLogDataCached.size(); idx++ )
                    {
                        m_arpLogDataCached[idx] = nullptr;
                    }
                    m_iLogDataCachedCount -= ciRemoveCount;
                }
            }

            // Add the entry to the list:
            m_arpLogDataCached[m_iLogDataCachedCount] = pLogData;
            pLogData = nullptr;
            m_iLogDataCachedCount++;

        } // if( bAdd2Cache )

        else if( m_ariSocketIdsConnectedClients.length() > 0 && isIpcServerUsed() )
        {
            // Please note that the cached data should have already been sent to the first
            // conencted client and cleared afterwards. So the cache must be empty here.

            QString strMsg;

            strMsg += systemMsgType2Str(MsgProtocol::ESystemMsgTypeInd) + " ";
            strMsg += command2Str(MsgProtocol::ECommandInsert) + " ";
            strMsg += pLogData->toXmlString();

            QByteArray byteArrMsg = str2ByteArr(strMsg);
            for( auto& iSocketId : m_ariSocketIdsConnectedClients )
            {
                sendData(iSocketId, byteArrMsg);
            }

            delete pLogData;
            pLogData = nullptr;
        }
    }
} // log

/*==============================================================================
public: // overridables of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CIpcLogServer::startup( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    return m_pIpcServer->startup(i_iTimeout_ms, i_bWait, i_iReqIdParent);
}

//------------------------------------------------------------------------------
CRequest* CIpcLogServer::shutdown( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    return m_pIpcServer->shutdown(i_iTimeout_ms, i_bWait, i_iReqIdParent);
}

/*==============================================================================
public: // overridables of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CIpcLogServer::changeSettings( int i_iTimeout_ms, bool i_bWait, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    return m_pIpcServer->changeSettings(i_iTimeout_ms, i_bWait, i_iReqIdParent);
}

/*==============================================================================
public: // instance methods changing and reading the host settings
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Ipc::SServerHostSettings CIpcLogServer::getHostSettings() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getHostSettings();
}

//------------------------------------------------------------------------------
void CIpcLogServer::setHostSettings( const ZS::Ipc::SServerHostSettings& i_hostSettings )
//------------------------------------------------------------------------------
{
    m_pIpcServer->setHostSettings(i_hostSettings);
}

/*==============================================================================
public: // instance methods of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogServer::setBlkType( ZS::Ipc::CBlkType* i_pBlkType )
//------------------------------------------------------------------------------
{
    m_pIpcServer->setBlkType(i_pBlkType);
}

//------------------------------------------------------------------------------
ZS::Ipc::CBlkType* CIpcLogServer::getBlkType()
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getBlkType();
}

/*==============================================================================
public: // instance methods of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
int CIpcLogServer::getSocketId( int i_idxSocket ) const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getSocketId(i_idxSocket);
}

//------------------------------------------------------------------------------
ZS::Ipc::SSocketDscr CIpcLogServer::getSocketDscr( int i_iSocketId ) const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getSocketDscr(i_iSocketId);
}

//------------------------------------------------------------------------------
int CIpcLogServer::getArrLenConnections() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getArrLenConnections();
}

//------------------------------------------------------------------------------
int CIpcLogServer::getActiveConnections() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getActiveConnections();
}

/*==============================================================================
public: // instance methods (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
ZS::Ipc::CServer::EState CIpcLogServer::getState() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getState();
}

//------------------------------------------------------------------------------
bool CIpcLogServer::isListening() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->isListening();
}

//------------------------------------------------------------------------------
bool CIpcLogServer::isStartingUp() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->isStartingUp();
}

//------------------------------------------------------------------------------
bool CIpcLogServer::isConnected( int /*i_iSocketId*/ ) const
//------------------------------------------------------------------------------
{
    bool bConnected = false;
    if( m_ariSocketIdsConnectedClients.length() > 0 )
    {
        bConnected = true;
    }
    return bConnected;
}

/*==============================================================================
public: // instance methods (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CIpcLogServer::isBusy() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->isBusy();
}

//------------------------------------------------------------------------------
ZS::Ipc::CServer::ERequest CIpcLogServer::requestInProgress() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->requestInProgress();
}

//------------------------------------------------------------------------------
QString CIpcLogServer::requestInProgress2Str( bool i_bShort ) const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->requestInProgress2Str(i_bShort);
}

//------------------------------------------------------------------------------
ZS::System::CRequest* CIpcLogServer::getRequestInProgress() const
//------------------------------------------------------------------------------
{
    return m_pIpcServer->getRequestInProgress();
}

/*==============================================================================
public: // instance methods (aborting requests)
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogServer::abortRequest( qint64 i_iRequestId )
//------------------------------------------------------------------------------
{
    m_pIpcServer->abortRequest(i_iRequestId);
}

//------------------------------------------------------------------------------
void CIpcLogServer::abortRequestInProgress()
//------------------------------------------------------------------------------
{
    m_pIpcServer->abortRequestInProgress();
}

//------------------------------------------------------------------------------
void CIpcLogServer::abortAllRequests()
//------------------------------------------------------------------------------
{
    m_pIpcServer->abortAllRequests();
}

/*==============================================================================
protected: // instance methods to send index tree entries to the connected clients
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogServer::sendBranch(
    int                         i_iSocketId,
    MsgProtocol::TSystemMsgType i_systemMsgType,
    MsgProtocol::TCommand       i_cmd,
    CIdxTreeEntry*              i_pBranch )
//------------------------------------------------------------------------------
{
    if( isConnected() && i_pBranch != nullptr )
    {
        if( !i_pBranch->isRoot() )
        {
            QString strMsg;
            QString strBranchName = i_pBranch->name();

            strBranchName = encodeForXml(strBranchName);

            strMsg += systemMsgType2Str(i_systemMsgType) + " ";
            strMsg += command2Str(i_cmd) + " ";

            strMsg += "<Branch ";
            if( i_pBranch->parentBranch() != nullptr )
            {
                strMsg += " ParentBranchIdxInTree=\"" + QString::number(i_pBranch->parentBranch()->indexInTree()) + "\"";
            }
            strMsg += " Name=\"" + strBranchName + "\"";
            strMsg += " IdxInTree=\"" + QString::number(i_pBranch->indexInTree()) + "\"";
            strMsg += "/>";

            sendData( i_iSocketId, str2ByteArr(strMsg) );

        } // if( !i_pBranch->isRoot() )

        CIdxTreeEntry* pTreeEntry;
        int            idxEntry;

        for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = i_pBranch->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->isRoot() || pTreeEntry->isBranch() )
                {
                    sendBranch(
                        /* iSocketId     */ i_iSocketId,
                        /* systemMsgType */ i_systemMsgType,
                        /* cmd           */ i_cmd,
                        /* pBranch       */ pTreeEntry );
                }
                else if( pTreeEntry->isLeave() )
                {
                    sendLeave(
                        /* iSocketId     */ i_iSocketId,
                        /* systemMsgType */ i_systemMsgType,
                        /* cmd           */ i_cmd,
                        /* strKeyInTree  */ pTreeEntry->keyInTree(),
                        /* idxInTree     */ pTreeEntry->indexInTree() );
                }
            }
        } // for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
    } // if( isConnected() && i_pBranch != nullptr )
} // sendBranch

//------------------------------------------------------------------------------
void CIpcLogServer::sendLeave(
    int                         i_iSocketId,
    MsgProtocol::TSystemMsgType i_systemMsgType,
    MsgProtocol::TCommand       i_cmd,
    const QString&              i_strKeyInTree,
    int                         i_idxInTree )
//------------------------------------------------------------------------------
{
    if( isConnected() )
    {
        QString strMsg;

        strMsg += systemMsgType2Str(i_systemMsgType) + " ";
        strMsg += command2Str(i_cmd) + " ";
        strMsg += "<Logger";
        strMsg += " IdxInTree=\"" + QString::number(i_idxInTree) + "\"";

        CLogger* pLogger = dynamic_cast<CLogger*>(m_pLoggersIdxTree->findLeave(i_strKeyInTree));

        // If removed the command is deleted. It is sufficient to send the IdxInTree which was deleted.
        if( pLogger != nullptr && !pLogger->isAboutToBeDestroyed() )
        {
            QString strName = encodeForXml(pLogger->name());
            QString strNameSpace = encodeForXml(pLogger->getNameSpace());
            QString strClassName = encodeForXml(pLogger->getClassName());
            QString strObjName = encodeForXml(pLogger->getObjectName());

            strMsg += " Name=\"" + strName + "\"";

            if( pLogger->parentBranch() != nullptr )
            {
                strMsg += " ParentBranchIdxInTree=\"" + QString::number(pLogger->parentBranch()->indexInTree()) + "\"";
            }
            strMsg += " Enabled=\"" + CEnumEnabled::toString(pLogger->getEnabled()) + "\"";
            strMsg += " LogLevel=\"" + CEnumLogDetailLevel(pLogger->getLogLevel()).toString() + "\"";
            strMsg += " DataFilter=\"" + pLogger->getDataFilter() + "\"";
            strMsg += " AddThreadName=\"" + bool2Str(pLogger->addThreadName()) + "\"";
            strMsg += " AddDateTime=\"" + bool2Str(pLogger->addDateTime()) + "\"";
            strMsg += " AddSystemTime=\"" + bool2Str(pLogger->addSystemTime()) + "\"";
            strMsg += " NameSpace=\"" + strNameSpace + "\"";
            strMsg += " ClassName=\"" + strClassName + "\"";
            strMsg += " ObjName=\"" + strObjName + "\"";
        }
        strMsg += "/>";

        sendData( i_iSocketId, str2ByteArr(strMsg) );
    }
}

/*==============================================================================
protected: // auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sends the server settings including application and server name to
           the connected client.

    @param i_iSocketId [in] Socket of the connected client.
*/
void CIpcLogServer::sendServerSettings(int i_iSocketId)
//------------------------------------------------------------------------------
{
    QString strDataSnd;
    strDataSnd  = systemMsgType2Str(MsgProtocol::ESystemMsgTypeCon) + " ";
    strDataSnd += command2Str(MsgProtocol::ECommandSelect) + " ";
    strDataSnd += "<ServerSettings";
    strDataSnd += " ApplicationName=\"" + QCoreApplication::applicationName() + "\"";
    strDataSnd += " ServerName=\"" + objectName() + "\"";
    strDataSnd += " Enabled=\"" + bool2Str(m_logSettings.m_bEnabled) + "\"";
    strDataSnd += " LoggerFileAbsFilePath=\"" + m_logSettings.m_strLoggerFileAbsFilePath + "\"";
    strDataSnd += " NewLoggersEnabledAsDefault=\"" + bool2Str(m_logSettings.m_bNewLoggersEnabledAsDefault) + "\"";
    strDataSnd += " NewLoggersDefaultDetailLevel=\""
               + CEnumLogDetailLevel(m_logSettings.m_eNewLoggersDefaultDetailLevel).toString() + "\"";
    strDataSnd += " UseIpcServer=\"" + bool2Str(m_logSettings.m_bUseIpcServer) + "\"";
    strDataSnd += " CacheDataIfNotConnected=\"" + bool2Str(m_logSettings.m_bCacheDataIfNotConnected) + "\"";
    strDataSnd += " CacheDataMaxArrLen=\"" + QString::number(m_logSettings.m_iCacheDataMaxArrLen) + "\"";
    strDataSnd += " UseLocalLogFile=\"" + bool2Str(m_logSettings.m_bUseLocalLogFile) + "\"";
    strDataSnd += " LocalLogFileAbsFilePath=\"" + m_logSettings.m_strLocalLogFileAbsFilePath + "\"";
    strDataSnd += " LocalLogFileAutoSaveInterval_ms=\"" + QString::number(m_logSettings.m_iLocalLogFileAutoSaveInterval_ms) + "\"";
    strDataSnd += " LocalLogFileSubFileCountMax=\"" + QString::number(m_logSettings.m_iLocalLogFileSubFileCountMax) + "\"";
    strDataSnd += " LocalLogFileSubFileLineCountMax=\"" + QString::number(m_logSettings.m_iLocalLogFileSubFileLineCountMax) + "\"";
    strDataSnd += " LocalLogFileCloseAfterEachWrite=\"" + bool2Str(m_logSettings.m_bLocalLogFileCloseFileAfterEachWrite) + "\"";
    strDataSnd += "/>";

    sendData(i_iSocketId, str2ByteArr(strDataSnd));
}

//------------------------------------------------------------------------------
/*! @brief Sends the cached data to the connected client.

    @param i_iSocketId [in] Socket of the connected client.
*/
void CIpcLogServer::sendCachedLogData(int i_iSocketId)
//------------------------------------------------------------------------------
{
    // If data has been temporarily stored ...
    if( m_iLogDataCachedCount > 0 )
    {
        QString   strDataSnd;
        SLogData* pLogData;
        int       idx;

        QMutexLocker mutexLocker(m_pMtxListLogDataCached);

        for( idx = 0; idx < m_iLogDataCachedCount; idx++ )
        {
            pLogData = m_arpLogDataCached[idx];

            if( pLogData != nullptr )
            {
                strDataSnd  = systemMsgType2Str(MsgProtocol::ESystemMsgTypeCon) + " ";
                strDataSnd += command2Str(MsgProtocol::ECommandInsert) + " ";
                strDataSnd += pLogData->toXmlString();

                sendData(i_iSocketId, str2ByteArr(strDataSnd));

                delete pLogData;
                pLogData = nullptr;
            }
            m_arpLogDataCached[idx] = nullptr;
        }
        m_arpLogDataCached.clear();
        m_iLogDataCachedCount = 0;
    }
} // sendCachedLogData

/*==============================================================================
protected: // instance methods of the remote connection
==============================================================================*/

//------------------------------------------------------------------------------
CRequest* CIpcLogServer::sendData( int i_iSocketId, const QByteArray& i_byteArr, qint64 i_iReqIdParent )
//------------------------------------------------------------------------------
{
    // The sendData method of the log server may be called from any thread in the application.
    return m_pIpcServer->sendData(
        /* iSocketId    */ i_iSocketId,
        /* byteArr      */ i_byteArr,
        /* iTimeout_ms  */ 0,
        /* bWait        */ false,
        /* iReqIdParent */ i_iReqIdParent );
}

/*==============================================================================
protected slots: // connected to the signals of the Ipc Server
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogServer::onIpcServerConnected( QObject* /*i_pServer*/, const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( !m_ariSocketIdsConnectedClients.contains(i_socketDscr.m_iSocketId) )
    {
        m_ariSocketIdsConnectedClients.append(i_socketDscr.m_iSocketId);

        sendServerSettings(i_socketDscr.m_iSocketId);

        // Send the whole logger tree to the client ...
        sendBranch(
            /* iSocketId     */ i_socketDscr.m_iSocketId,
            /* systemMsgType */ MsgProtocol::ESystemMsgTypeCon,
            /* cmd           */ MsgProtocol::ECommandSelect,
            /* pBranch       */ m_pLoggersIdxTree->root() );

        // If log data has been temporarily stored ...
        if( m_iLogDataCachedCount > 0 )
        {
            sendCachedLogData(i_socketDscr.m_iSocketId);
        }
    }
    else
    {
        QString strAddErrInfo = "Received connected signal for already connected socket " + i_socketDscr.getConnectionString();
        SErrResultInfo errResultInfo = ErrResultInfoError("onIpcServerConnected", EResultSocketIdOutOfRange, strAddErrInfo);
    }
}

//------------------------------------------------------------------------------
void CIpcLogServer::onIpcServerDisconnected( QObject* /*i_pServer*/, const SSocketDscr& i_socketDscr )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_ariSocketIdsConnectedClients.contains(i_socketDscr.m_iSocketId) )
    {
        m_ariSocketIdsConnectedClients.removeOne(i_socketDscr.m_iSocketId);
    }
    else
    {
        QString strAddErrInfo = "Received disconnected signal for not connected socket " + i_socketDscr.getConnectionString();
        SErrResultInfo errResultInfo = ErrResultInfoError("onIpcServerDisconnected", EResultSocketIdOutOfRange, strAddErrInfo);
    }
}

//------------------------------------------------------------------------------
void CIpcLogServer::onIpcServerReceivedData(
    QObject*          /*i_pServer*/,
    int               i_iSocketId,
    const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

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
        pcData   += (strSystemMsgTypeOfData.length() + 1);
        iDataLen -= (strSystemMsgTypeOfData.length() + 1);
        iSection++;
    }

    strCmd = str.section(" ",iSection,iSection);
    cmd = str2Command(strCmd);
    if( cmd != MsgProtocol::ECommandUndefined )
    {
        pcData   += (strCmd.length() + 1);
        iDataLen -= (strCmd.length() + 1);
        iSection++;
    }

    QString strData(pcData, iDataLen);

    switch( systemMsgTypeOfData )
    {
        case MsgProtocol::ESystemMsgTypeReq:
        {
            switch( cmd )
            {
                case MsgProtocol::ECommandSelect:
                {
                    onIpcServerReceivedReqSelect(i_iSocketId, strData);
                    break;
                }
                case MsgProtocol::ECommandUpdate:
                {
                    onIpcServerReceivedReqUpdate(i_iSocketId, strData);
                    break;
                }
                case MsgProtocol::ECommandInsert:
                case MsgProtocol::ECommandDelete:
                default:
                {
                    break;
                }
            }
            break;
        }
        case MsgProtocol::ESystemMsgTypeCon:
        case MsgProtocol::ESystemMsgTypeInd:
        case MsgProtocol::ESystemMsgTypeAck:
        default:
        {
            break;
        }
    } // switch( systemMsgTypeOfData )
} // onIpcServerReceivedData

/*==============================================================================
protected: // overridables to parse and execute the incoming data stream
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogServer::onIpcServerReceivedReqSelect( int i_iSocketId, const QString& i_strData )
//------------------------------------------------------------------------------
{
    // Already locked by onIpcServerReceivedData
    //QMutexLocker mtxLocker(&s_mtx);

    // The index tree will be locked so it will not be changed
    // when sending the whole content of the index tree to the client.
    QMutexLocker mutexLocker(m_pLoggersIdxTree->mutex());

    QString strMth = "onIpcServerReceivedReqSelect";

    QXmlStreamReader xmlStreamReader(i_strData);

    QXmlStreamReader::TokenType xmlStreamTokenType;
    QString                     strElemName;
    QString                     strDataSnd;

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
                    sendServerSettings(i_iSocketId);
                }
                else if( strElemName == "Loggers" )
                {
                    // Send the whole admin object tree to the client ...
                    sendBranch(
                        /* iSocketId     */ i_iSocketId,
                        /* systemMsgType */ MsgProtocol::ESystemMsgTypeCon,
                        /* cmd           */ MsgProtocol::ECommandSelect,
                        /* pBranch       */ m_pLoggersIdxTree->root() );
                }
                else if( strElemName == "LogData" )
                {
                    // If data has been temporarily stored ...
                    if( m_iLogDataCachedCount > 0 )
                    {
                        sendCachedLogData(i_iSocketId);
                    }
                } // if( strElemName == "LogData" )

                else
                {
                    xmlStreamReader.raiseError("Invalid element \"" + strElemName + "\" in select request");
                }
            } // if( xmlStreamTokenType == QXmlStreamReader::StartElement )
        } // while( xmlStreamTokenType != QXmlStreamReader::EndElement && xmlStreamTokenType != QXmlStreamReader::Invalid )
    } // if( xmlStreamTokenType == QXmlStreamReader::StartDocument )

    if( xmlStreamReader.hasError() )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileReadContent);
            errResultInfo.setAddErrInfoDscr(xmlStreamReader.errorString());

            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
} // onIpcServerReceivedReqSelect

//------------------------------------------------------------------------------
void CIpcLogServer::onIpcServerReceivedReqUpdate( int i_iSocketId, const QString& i_strData )
//------------------------------------------------------------------------------
{
    // The index tree will be locked so it will not be changed
    // when sending the whole content of the index tree to the client.
    QMutexLocker mutexLocker(m_pLoggersIdxTree->mutex());

    // Already locked by onIpcServerReceivedData
    //QMutexLocker mtxLocker(&s_mtx);

    QString strMth = "onIpcServerReceivedReqUpdate";

    m_bOnReceivedDataUpdateInProcess = true;

    QXmlStreamReader xmlStreamReader(i_strData);

    QXmlStreamReader::TokenType xmlStreamTokenType;

    QString  strElemName;
    QString  strAttr;
    bool     bVal;
    int      iVal;
    int      iObjId;
    EEnabled enabled;
    bool     bSetDataFilter;
    QString  strDataFilter;
    bool     bSetAddThreadName;
    bool     bAddThreadName;
    bool     bSetAddDateTime;
    bool     bAddDateTime;
    bool     bSetAddSystemTime;
    bool     bAddSystemTime;
    bool     bOk;

    ELogDetailLevel eDetailLevel;

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

                // For best performance start with the most frequently used element names ..

                if( strElemName == "ServerSettings" )
                {
                    SLogServerSettings logServerSettings = m_logSettings;

                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Enabled") )
                    {
                        strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                        bVal = str2Bool(strAttr, &bOk);
                        if( bOk ) logServerSettings.m_bEnabled = bVal;
                        else xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LoggerFileAbsFilePath") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LoggerFileAbsFilePath").toString();
                        logServerSettings.m_strLoggerFileAbsFilePath = strAttr;
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("NewLoggersEnabledAsDefault") )
                    {
                        strAttr = xmlStreamReader.attributes().value("NewLoggersEnabledAsDefault").toString();
                        bVal = str2Bool(strAttr, &bOk);
                        if( bOk ) logServerSettings.m_bNewLoggersEnabledAsDefault = bVal;
                        else xmlStreamReader.raiseError("Attribute \"NewLoggersEnabledAsDefault\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("NewLoggersDefaultDetailLevel") )
                    {
                        strAttr = xmlStreamReader.attributes().value("NewLoggersDefaultDetailLevel").toString();
                        CEnumLogDetailLevel eDetailLevel = CEnumLogDetailLevel::fromString(strAttr);
                        if( eDetailLevel != ELogDetailLevel::Undefined ) logServerSettings.m_eNewLoggersDefaultDetailLevel = eDetailLevel.enumerator();
                        else xmlStreamReader.raiseError("Attribute \"NewLoggersDefaultDetailLevel\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("UseIpcServer") )
                    {
                        strAttr = xmlStreamReader.attributes().value("UseIpcServer").toString();
                        bVal = str2Bool(strAttr, &bOk);
                        if( bOk ) logServerSettings.m_bUseIpcServer = bVal;
                        else xmlStreamReader.raiseError("Attribute \"UseIpcServer\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("CacheDataIfNotConnected") )
                    {
                        strAttr = xmlStreamReader.attributes().value("CacheDataIfNotConnected").toString();
                        bVal = str2Bool(strAttr, &bOk);
                        if( bOk ) logServerSettings.m_bCacheDataIfNotConnected = bVal;
                        else xmlStreamReader.raiseError("Attribute \"CacheDataIfNotConnected\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("CacheDataMaxArrLen") )
                    {
                        strAttr = xmlStreamReader.attributes().value("CacheDataMaxArrLen").toString();
                        iVal = strAttr.toInt(&bOk);
                        if( bOk ) logServerSettings.m_iCacheDataMaxArrLen = iVal;
                        else xmlStreamReader.raiseError("Attribute \"CacheDataMaxArrLen\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("UseLocalLogFile") )
                    {
                        strAttr = xmlStreamReader.attributes().value("UseLocalLogFile").toString();
                        bVal = str2Bool(strAttr, &bOk);
                        if( bOk ) logServerSettings.m_bUseLocalLogFile = bVal;
                        else xmlStreamReader.raiseError("Attribute \"UseLocalLogFile\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LocalLogFileAbsFilePath") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LocalLogFileAbsFilePath").toString();
                        logServerSettings.m_strLocalLogFileAbsFilePath = strAttr;
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LocalLogFileAutoSaveInterval_ms") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LocalLogFileAutoSaveInterval_ms").toString();
                        iVal = strAttr.toInt(&bOk);
                        if( bOk ) logServerSettings.m_iLocalLogFileAutoSaveInterval_ms = iVal;
                        else xmlStreamReader.raiseError("Attribute \"LocalLogFileAutoSaveInterval_ms\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LocalLogFileSubFileCountMax") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LocalLogFileSubFileCountMax").toString();
                        iVal = strAttr.toInt(&bOk);
                        if( bOk ) logServerSettings.m_iLocalLogFileSubFileCountMax = iVal;
                        else xmlStreamReader.raiseError("Attribute \"LocalLogFileSubFileCountMax\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LocalLogFileSubFileLineCountMax") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LocalLogFileSubFileLineCountMax").toString();
                        iVal = strAttr.toInt(&bOk);
                        if( bOk ) logServerSettings.m_iLocalLogFileSubFileLineCountMax = iVal;
                        else xmlStreamReader.raiseError("Attribute \"LocalLogFileSubFileLineCountMax\" (" + strAttr + ") is out of range");
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LocalLogFileCloseAfterEachWrite") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LocalLogFileCloseAfterEachWrite").toString();
                        bVal = str2Bool(strAttr, &bOk);
                        if( bOk ) logServerSettings.m_bLocalLogFileCloseFileAfterEachWrite = bVal;
                        else xmlStreamReader.raiseError("Attribute \"LocalLogFileCloseAfterEachWrite\" (" + strAttr + ") is out of range");
                    }

                    // While receiving the settings emitting the logSettingsChanged signal is blocked
                    // by setting the flag m_bOnReceivedDataUpdateInProcess to true to accumulate all changes
                    // and emit the signal just once. setLogSettings will check if the settings have been
                    // changed before taken them over. But setLogSettings will not emit the settings changed
                    // signal as the flag m_bOnReceivedDataUpdateInProcess is set. So we need to do this check
                    // here also and emit the signal.
                    if( logServerSettings != m_logSettings )
                    {
                        setLogSettings(logServerSettings);
                        emit logSettingsChanged(this);
                    }
                } // if( strElemName == "ServerSettings" )

                else if( strElemName == "Logger" )
                {
                    iObjId = -1;
                    enabled = EEnabled::Undefined;
                    bSetDataFilter = false;
                    strDataFilter = "";
                    eDetailLevel = ELogDetailLevel::None;
                    bAddThreadName = false;
                    bSetAddThreadName = false;
                    bAddDateTime = false;
                    bSetAddDateTime = false;
                    bAddSystemTime = false;
                    bSetAddSystemTime = false;

                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("ObjId") )
                    {
                        strAttr = xmlStreamReader.attributes().value("ObjId").toString();
                        iObjId = strAttr.toInt(&bOk);
                        if( !bOk )
                        {
                            xmlStreamReader.raiseError("Attribute \"ObjId\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Enabled") )
                    {
                        strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                        enabled = CEnumEnabled::toEnumerator(strAttr);
                        if( enabled == EEnabled::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LogLevel") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LogLevel").toString();
                        eDetailLevel = CEnumLogDetailLevel::toEnumerator(strAttr);
                        if( eDetailLevel == ELogDetailLevel::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"LogLevel\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
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

                    if( iObjId < 0 || iObjId >= m_pLoggersIdxTree->treeEntriesVectorSize() )
                    {
                        xmlStreamReader.raiseError("ObjectId " + QString::number(iObjId) + " for \"" + strElemName + "\" is out of range");
                    }
                    else // if( iObjId >= 0 && iObjId < m_pLoggersIdxTree->treeEntriesVectorSize() )
                    {
                        CLogger* pLogger = m_pLoggersIdxTree->getLogger(iObjId);

                        if( pLogger == nullptr )
                        {
                            xmlStreamReader.raiseError("An Object with Id " + QString::number(iObjId) + " for \"" + strElemName + "\" is not existing");
                        }
                        else
                        {
                            pLogger->setEnabled(enabled);
                            pLogger->setLogLevel(eDetailLevel);
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
                        }
                    } // if( iObjId >= 0 && iObjId < m_pLoggersIdxTree->treeEntriesVectorSize() )
                } // if( strElemName == "Logger" )

                else if( strElemName == "NameSpace" )
                {
                    iObjId = -1;
                    enabled = EEnabled::Undefined;
                    bSetDataFilter = false;
                    strDataFilter = "";
                    eDetailLevel = ELogDetailLevel::None;

                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("ObjId") )
                    {
                        strAttr = xmlStreamReader.attributes().value("ObjId").toString();
                        iObjId = strAttr.toInt(&bOk);
                        if( !bOk )
                        {
                            xmlStreamReader.raiseError("Attribute \"ObjId\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Enabled") )
                    {
                        strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                        enabled = CEnumEnabled::toEnumerator(strAttr);
                        if( enabled == EEnabled::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("LogLevel") )
                    {
                        strAttr = xmlStreamReader.attributes().value("LogLevel").toString();
                        eDetailLevel = CEnumLogDetailLevel::toEnumerator(strAttr);
                        if( eDetailLevel == ELogDetailLevel::Undefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"LogLevel\" (" + strAttr + ") for \"" + strElemName + "\" is out of range");
                        }
                    }
                    if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("DataFilter") )
                    {
                        strDataFilter = xmlStreamReader.attributes().value("DataFilter").toString();
                        bSetDataFilter = true;
                    }

                    if( iObjId < 0 || iObjId >= m_pLoggersIdxTree->treeEntriesVectorSize() )
                    {
                        xmlStreamReader.raiseError("ObjectId " + QString::number(iObjId) + " for \"" + strElemName + "\" is out of range");
                    }
                    else // if( iObjId >= 0 && iObjId < m_pLoggersIdxTree->treeEntriesVectorSize() )
                    {
                        CIdxTreeEntry* pTreeEntry = m_pLoggersIdxTree->getEntry(iObjId);

                        if( pTreeEntry == nullptr )
                        {
                            xmlStreamReader.raiseError("An Object with Id " + QString::number(iObjId) + " for \"" + strElemName + "\" is not existing");
                        }
                        else if( !pTreeEntry->isRoot() && !pTreeEntry->isBranch() )
                        {
                            xmlStreamReader.raiseError("The Object with Id " + QString::number(iObjId) + " for \"" + strElemName + "\" is not a name space node");
                        }
                        else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
                        {
                            m_pLoggersIdxTree->setEnabled(iObjId, enabled);
                            m_pLoggersIdxTree->setDetailLevel(iObjId, eDetailLevel);
                            if( bSetDataFilter ) {
                                m_pLoggersIdxTree->setDataFilter(iObjId, strDataFilter);
                            }
                        }
                    } // if( iObjId >= 0 && iObjId < m_pLoggersIdxTree->treeEntriesVectorSize() )
                } // if( strElemName == "NameSpace" )
            } // if( xmlStreamReader.isStartElement() )
        } // while( xmlStreamTokenType != QXmlStreamReader::EndElement && xmlStreamTokenType != QXmlStreamReader::Invalid )
    } // if( xmlStreamTokenType == QXmlStreamReader::StartDocument )

    if( xmlStreamReader.hasError() )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileReadContent);
            errResultInfo.setAddErrInfoDscr(xmlStreamReader.errorString());

            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    if( !m_logSettings.m_strLoggerFileAbsFilePath.isEmpty() )
    {
        saveLoggers();
    }

    m_bOnReceivedDataUpdateInProcess = false;

} // onIpcServerReceivedReqUpdate

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CIpcLogServer::onLoggersIdxTreeEntryAdded( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    // The index tree will be locked so it will not be changed when accessing it here.
    CMutexLocker mutexLocker(m_pLoggersIdxTree->mutex());

    QMutexLocker mtxLocker(&s_mtx);

    if( m_bOnReceivedDataUpdateInProcess )
    {
        return;
    }

    CIdxTreeLocker idxTreeLocker(m_pLoggersIdxTree);

    CIdxTreeEntry* pTreeEntry = m_pLoggersIdxTree->findEntry(i_strKeyInTree);

    if( pTreeEntry != nullptr && isConnected() )
    {
        if( pTreeEntry->isBranch() )
        {
            sendBranch(
                /* iSocketId     */ ESocketIdAllSockets,
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                /* cmd           */ MsgProtocol::ECommandInsert,
                /* pBranch       */ pTreeEntry );
        }
        else if( pTreeEntry->isLeave() )
        {
            sendLeave(
                /* iSocketId     */ ESocketIdAllSockets,
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                /* cmd           */ MsgProtocol::ECommandInsert,
                /* strKeyInTree  */ pTreeEntry->keyInTree(),
                /* idxInTree     */ pTreeEntry->indexInTree() );
        }
    }
}

//------------------------------------------------------------------------------
void CIpcLogServer::onLoggersIdxTreeEntryAboutToBeRemoved(
    const QString& i_strKeyInTree, int i_idxInTree )
//------------------------------------------------------------------------------
{
    // The index tree will be locked so it will not be changed when accessing it here.
    CMutexLocker mutexLocker(m_pLoggersIdxTree->mutex());

    QMutexLocker mtxLocker(&s_mtx);

    if( m_bOnReceivedDataUpdateInProcess )
    {
        return;
    }

    if( isConnected() )
    {
        // Could have already been removed and may no longer exist.
        CIdxTreeEntry* pTreeEntry = m_pLoggersIdxTree->findEntry(i_strKeyInTree);

        if( pTreeEntry != nullptr )
        {
            if( pTreeEntry->isBranch() )
            {
                sendBranch(
                    /* iSocketId     */ ESocketIdAllSockets,
                    /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                    /* cmd           */ MsgProtocol::ECommandDelete,
                    /* pBranch       */ pTreeEntry );
            }
            else if( pTreeEntry->isLeave() )
            {
                sendLeave(
                    /* iSocketId     */ ESocketIdAllSockets,
                    /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                    /* cmd           */ MsgProtocol::ECommandDelete,
                    /* strKeyInTree  */ i_strKeyInTree,
                    /* idxInTree     */ i_idxInTree );
            }
        }
    }
}

//------------------------------------------------------------------------------
void CIpcLogServer::onLoggersIdxTreeEntryChanged( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    // The index tree will be locked so it will not be changed when accessing it here.
    CMutexLocker mutexLocker(m_pLoggersIdxTree->mutex());

    if( m_bOnReceivedDataUpdateInProcess )
    {
        return;
    }

    CIdxTreeLocker idxTreeLocker(m_pLoggersIdxTree);

    CIdxTreeEntry* pTreeEntry = m_pLoggersIdxTree->findEntry(i_strKeyInTree);

    if( pTreeEntry != nullptr && isConnected() )
    {
        if( pTreeEntry->isRoot() || pTreeEntry->isBranch() )
        {
            sendBranch(
                /* iSocketId     */ ESocketIdAllSockets,
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                /* cmd           */ MsgProtocol::ECommandInsert,
                /* pBranch       */ pTreeEntry );
        }
        else if( pTreeEntry->isLeave() )
        {
            sendLeave(
                /* iSocketId     */ ESocketIdAllSockets,
                /* systemMsgType */ MsgProtocol::ESystemMsgTypeInd,
                /* cmd           */ MsgProtocol::ECommandUpdate,
                /* strKeyInTree  */ pTreeEntry->keyInTree(),
                /* idxInTree     */ pTreeEntry->indexInTree() );
        }
    }
}

/*==============================================================================
protected: // overridables of inherited class QObject (state machine)
==============================================================================*/

//------------------------------------------------------------------------------
bool CIpcLogServer::event( QEvent* i_pMsg )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pMsg->type() >= QEvent::User )
    {
        QString strAddTrcInfo;
        QString strAddErrInfo;

        CMsg* pMsg = nullptr;

        try
        {
            pMsg = dynamic_cast<CMsg*>(i_pMsg);
        }
        catch(...)
        {
            pMsg = nullptr;
        }

        if( pMsg != nullptr )
        {
            QMutexLocker mtxLocker(&s_mtx);

            bEventHandled = true;

            if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )
            {
                //--------------------------------------------------------------
                // Sending request messages to the server by proxies is not yet supported.
                // For this setting request and confirmation messages at the request instance
                // must be revised as for blocking requests the gateway also sets the
                // confirmation message to indicate the result of the executed request.
                //--------------------------------------------------------------

                CMsgReq* pMsgReq = dynamic_cast<CMsgReq*>(pMsg);
                if( pMsgReq == nullptr )
                {
                    throw CException(__FILE__, __LINE__, EResultMessageTypeMismatch);
                }

                //qint64   iReqIdParent = pMsgReq->getRequestId();
                //CMsgCon* pMsgCon = nullptr;

                //if( isAsynchronousRequest(pReq) )
                //{
                //    pReq->setConfirmationMessage(pMsgCon);
                //    pMsgCon = nullptr;
                //}
                //else // if( !isAsynchronousRequest(pReq) )
                //{
                //    bool bIsWaiting = false;

                //    if( iReqIdParent >= 0 )
                //    {
                //        bIsWaiting = CRequestExecTree::GetInstance()->isWaiting(iReqIdParent);
                //    }

                //    if( bIsWaiting != 0 )
                //    {
                //        CRequestExecTree::GetInstance()->wake(iReqIdParent);
                //    }
                //    else if( pMsgCon != nullptr )
                //    {
                //        if( pReq != nullptr )
                //        {
                //            errResultInfo = pReq->getErrResultInfo();
                //        }
                //        else if( errResultInfo.getResult() == EResultUndefined )
                //        {
                //            errResultInfo.setResult(EResultSuccess);
                //        }
                //        pMsgCon->setErrResultInfo(errResultInfo);
                //        pMsgCon->setProgress(100);

                //        POST_OR_DELETE_MESSAGE(pMsgCon, &mthTracer, ELogDetailLevel);
                //        pMsgCon = nullptr;
                //    }
                //} // if( !isAsynchronousRequest(pReq) )

                //delete pMsgCon;
                //pMsgCon = nullptr;

                switch( static_cast<int>(pMsgReq->type()) )
                {
                    case EMsgTypeReqStartup:
                    {
                        CMsgReqStartup* pMsgReq = dynamic_cast<CMsgReqStartup*>(pMsg);
                        if( pMsgReq == nullptr )
                        {
                            throw ZS::System::CException(__FILE__, __LINE__, EResultMessageTypeMismatch, "MsgReqStartup == nullptr! " + strAddTrcInfo);
                        }
                        setHostSettings(pMsgReq->getHostSettings());
                        setBlkType(pMsgReq->getBlkType(true)); // The server takes ownership of the block.
                        startup(10000, true);
                        break;
                    } // case EMsgTypeReqStartup

                    case EMsgTypeReqShutdown:
                    {
                        CMsgReqShutdown* pMsgReq = dynamic_cast<CMsgReqShutdown*>(pMsg);
                        if( pMsgReq == nullptr )
                        {
                            throw ZS::System::CException(__FILE__, __LINE__, EResultMessageTypeMismatch, "MsgReqShutdown == nullptr! " + strAddTrcInfo);
                        }
                        shutdown(10000, true);
                        break;
                    }

                    case EMsgTypeReqChangeSettings:
                    {
                        CMsgReqChangeSettings* pMsgReq = dynamic_cast<CMsgReqChangeSettings*>(pMsg);
                        if( pMsgReq == nullptr )
                        {
                            throw ZS::System::CException(__FILE__, __LINE__, EResultMessageTypeMismatch, "MsgReqChangeSettings == nullptr! " + strAddTrcInfo);
                        }
                        setHostSettings(pMsgReq->getServerHostSettings());
                        changeSettings(10000, true);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                } // switch( static_cast<int>(pMsg->type()) )
            } // if( pMsg->getSystemMsgType() == MsgProtocol::ESystemMsgTypeReq )
        } // if( pMsg != nullptr )
    } // if( pMsg->type() >= QEvent::User )

    if( !bEventHandled )
    {
        bEventHandled = QObject::event(i_pMsg);
    }
    return bEventHandled;

} // event
