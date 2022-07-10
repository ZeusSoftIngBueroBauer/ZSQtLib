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
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qthread.h>

#include "ZSSys/ZSSysLogServer.h"
#include "ZSSys/ZSSysLogFile.h"
#include "ZSSys/ZSSysLogger.h"
#include "ZSSys/ZSSysLoggerIdxTree.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;


/*******************************************************************************
struct SLogServerSettings
*******************************************************************************/

/*==============================================================================
public: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a log server settings structure.

    Please note that the members m_strLoggerFileAbsFilePath and
    m_strLocalLogFileAbsFilePath only could be initialized automatically after
    the applications organizsation and application name habe been set.
    As thats not always the case they must be set "manually" at the right time.
    E.g. right after setting the applications organization and application name
    during startup as follows:

        QCoreApplication::setOrganizationName("MyOrganization");
        QCoreApplication::setOrganizationDomain("www.MyOrganization.com");
        QCoreApplication::setApplicationName("MyAppName");

        m_logServerSettings.m_strLoggerFileAbsFilePath = CLogServer::GetLoggerFileAbsoluteFilePath();
        m_logServerSettings.m_strLocalLogFileAbsFilePath = CLogServer::GetLocalLogFileAbsoluteFilePath();

    @param i_bEnabled [in] Default: true
    @param i_bNewLoggersEnabledAsDefault [in] Default: true
    @param i_eNewTLoggersDefaultDetailLevel [in] Default: None
    @param i_bCacheDataIfNotConnected [in] Default: false
    @param i_iCacheDataMaxArrLen [in] Default: 1000
    @param i_bUseLocalLogFile [in] Default: true
    @param i_iLocalLogFileAutoSaveInterval_ms [in] Default: 1000
        The default value should be the same as used by the LogFile ctor.
    @param i_iLocalLogFileSubFileCountMax [in] Default: 5
        The default value should be the same as used by the LogFile ctor.
    @param i_iLocalLogFileSubFileLineCountMax [in] Default: 2000
        The default value should be the same as used by the LogFile ctor.
    @param i_bLocalLogFileCloseFileAfterEachWrite [in] Default: false
        The default value should be the same as used by the LogFile ctor.
*/
SLogServerSettings::SLogServerSettings(
    bool i_bEnabled,
    bool i_bNewLoggersEnabledAsDefault,
    ELogDetailLevel i_eNewLoggersDefaultDetailLevel,
    bool i_bUseIpcServer,
    bool i_bCacheDataIfNotConnected,
    int  i_iCacheDataMaxArrLen,
    bool i_bUseLocalLogFile,
    int  i_iLocalLogFileAutoSaveInterval_ms,
    int  i_iLocalLogFileSubFileCountMax,
    int  i_iLocalLogFileSubFileLineCountMax,
    bool i_bLocalLogFileCloseFileAfterEachWrite ) :
//------------------------------------------------------------------------------
    m_bEnabled(i_bEnabled),
    m_strLoggerFileAbsFilePath(),
    m_bNewLoggersEnabledAsDefault(i_bNewLoggersEnabledAsDefault),
    m_eNewLoggersDefaultDetailLevel(i_eNewLoggersDefaultDetailLevel),
    m_bUseIpcServer(i_bUseIpcServer),
    m_bCacheDataIfNotConnected(i_bCacheDataIfNotConnected),
    m_iCacheDataMaxArrLen(i_iCacheDataMaxArrLen),
    m_bUseLocalLogFile(i_bUseLocalLogFile),
    m_strLocalLogFileAbsFilePath(),
    m_iLocalLogFileAutoSaveInterval_ms(i_iLocalLogFileAutoSaveInterval_ms),
    m_iLocalLogFileSubFileCountMax(i_iLocalLogFileSubFileCountMax),
    m_iLocalLogFileSubFileLineCountMax(i_iLocalLogFileSubFileLineCountMax),
    m_bLocalLogFileCloseFileAfterEachWrite(i_bLocalLogFileCloseFileAfterEachWrite)
{
} // ctor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Equal operator.

    @param i_settingsOther [in]
*/
bool SLogServerSettings::operator == ( const SLogServerSettings& i_settingsOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_bEnabled != i_settingsOther.m_bEnabled )
    {
        bEqual = false;
    }
    else if( m_eNewLoggersDefaultDetailLevel != i_settingsOther.m_eNewLoggersDefaultDetailLevel )
    {
        bEqual = false;
    }
    else if( m_eNewLoggersDefaultDetailLevel != i_settingsOther.m_eNewLoggersDefaultDetailLevel )
    {
        bEqual = false;
    }
    else if( m_bUseIpcServer != i_settingsOther.m_bUseIpcServer )
    {
        bEqual = false;
    }
    else if( m_bCacheDataIfNotConnected != i_settingsOther.m_bCacheDataIfNotConnected )
    {
        bEqual = false;
    }
    else if( m_iCacheDataMaxArrLen != i_settingsOther.m_iCacheDataMaxArrLen )
    {
        bEqual = false;
    }
    else if( m_bUseLocalLogFile != i_settingsOther.m_bUseLocalLogFile )
    {
        bEqual = false;
    }
    else if( m_iLocalLogFileAutoSaveInterval_ms != i_settingsOther.m_iLocalLogFileAutoSaveInterval_ms )
    {
        bEqual = false;
    }
    else if( m_iLocalLogFileSubFileCountMax != i_settingsOther.m_iLocalLogFileSubFileCountMax )
    {
        bEqual = false;
    }
    else if( m_iLocalLogFileSubFileLineCountMax != i_settingsOther.m_iLocalLogFileSubFileLineCountMax )
    {
        bEqual = false;
    }
    else if( m_bLocalLogFileCloseFileAfterEachWrite != i_settingsOther.m_bLocalLogFileCloseFileAfterEachWrite )
    {
        bEqual = false;
    }
    else if( m_strLoggerFileAbsFilePath != i_settingsOther.m_strLoggerFileAbsFilePath )
    {
        bEqual = false;
    }
    else if( m_strLocalLogFileAbsFilePath != i_settingsOther.m_strLocalLogFileAbsFilePath )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
/*! @brief Unequal operator.

    @param i_settingsOther [in]
*/
bool SLogServerSettings::operator != ( const SLogServerSettings& i_settingsOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_settingsOther);
}

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns a human readable string describing the settings.

    @return String describing the settings which can be used for logging.
*/
QString SLogServerSettings::toString() const
//------------------------------------------------------------------------------
{
    QString str;
    str += "Enabled: " + bool2Str(m_bEnabled);
    str += ", LoggerFile: " + m_strLoggerFileAbsFilePath;
    str += ", LoggerDefEnabled: " + bool2Str(m_bNewLoggersEnabledAsDefault);
    str += ", LoggerDefLevel: " + CEnum<ELogDetailLevel>(m_eNewLoggersDefaultDetailLevel).toString();
    str += ", UseIpcServer: " + bool2Str(m_bUseIpcServer);
    str += ", CacheData: " + bool2Str(m_bCacheDataIfNotConnected);
    str += ", CacheArrLen: " + QString::number(m_iCacheDataMaxArrLen);
    str += ", UseLocalFile: " + bool2Str(m_bUseLocalLogFile);
    str += ", TrcFile: " + m_strLocalLogFileAbsFilePath;
    str += ", TrcFileSave: " + QString::number(m_iLocalLogFileAutoSaveInterval_ms) + " ms";
    str += ", TrcFileSubFiles: " + QString::number(m_iLocalLogFileSubFileCountMax);
    str += ", TrcFileLines: " + QString::number(m_iLocalLogFileSubFileLineCountMax);
    str += ", TrcFileClose: " + bool2Str(m_bLocalLogFileCloseFileAfterEachWrite);
    return str;
}


/*******************************************************************************
class CLogServer : public QObject
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QMutex CLogServer::s_mtx(QMutex::Recursive);
CLogServer* CLogServer::s_pTheInst;
QHash<Qt::HANDLE, QString> CLogServer::s_hshThreadNames;
QHash<QString, Qt::HANDLE> CLogServer::s_hshThreadIds;
QString CLogServer::s_strLoggerFileAbsFilePath;
QString CLogServer::s_strLocalLogFileAbsFilePath;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the address of the log server.

    This method does neither create an instance of the class nor increments
    the reference counter.
    If no instance has been created yet the method returns nullptr.

    If you just need to access the already existing instance and you can be sure
    that the instance is already existing this method should be preferred to the
    createInstance call as this method does not affect the reference counter and
    there is no need to call releaseInstance later on.

    @note After a getInstance call a releaseInstance MUST NOT be called.

    @return Pointer to log server or nullptr, if the instance has not been created yet.
*/
CLogServer* CLogServer::GetInstance()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return s_pTheInst;
}

//------------------------------------------------------------------------------
/*! @brief Creates the log server if the log server is not already existing.

    If the log server is already existing the reference to the existing
    log server is returned and a reference counter is incremented.

    @return Pointer to log server instance.
*/
CLogServer* CLogServer::CreateInstance()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( s_pTheInst == nullptr )
    {
        // The ctor sets s_pTheInst to this.
        // If the ctor itself calls methods of other classes which again recursively
        // call "GetInstance" this way "GetInstance" does not return null but the
        // pointer to the server instance currently beeing created.
        // But of course this requires special caution as within the ctor it must
        // be assured that recursively accessed instance members are already valid.
        new CLogServer();
    }

    s_pTheInst->incrementRefCount();

    return s_pTheInst;

} // CreateInstance

//------------------------------------------------------------------------------
/*! @brief Releases the log server instance.

    Before invoking this method a reference to the instance must have been retrieved
    with a createInstance call.

    This method decrements the reference counter of the instance.
    If the reference counter reaches 0 the instance will be destroyed.

    @note A reference returned by getInstance MUST NOT be freed.
*/
void CLogServer::ReleaseInstance()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( s_pTheInst == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultSingletonClassNotInstantiated, "ZS::System::CLogServer");
    }

    int iRefCount = s_pTheInst->decrementRefCount();

    if( iRefCount == 0 )
    {
        try
        {
            delete s_pTheInst;
        }
        catch(...)
        {
        }
        s_pTheInst = nullptr;
    }
} // ReleaseInstance

/*==============================================================================
public: // class methods to register thread names
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Assigns the given thread name to the thread id of the current thread and
           adds the thread name to the hash of known threads with the thread id as key.

    This method may be used in none Qt applications if it is not possible to
    assign a human readable descriptive object name to the thread instance.

    @param i_strThreadName [in] Name of the thread
*/
void CLogServer::RegisterCurrentThread( const QString& i_strThreadName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    // Thread Ids may change during runtime for the same thread name.
    // To avoid that the thread maps are increasing endlessly it will be
    // checked whether the thread name was already registered. If the
    // thread name (and its previous id) will be removed from the maps.

    Qt::HANDLE threadIdCurr = QThread::currentThreadId();

    // If the thread name is already registered for another thread id ..
    Qt::HANDLE threadIdPrev = s_hshThreadIds.value(i_strThreadName, nullptr);
    if( threadIdPrev != nullptr && threadIdPrev != threadIdCurr )
    {
        s_hshThreadIds.remove(i_strThreadName);
        if( s_hshThreadNames.contains(threadIdPrev) )
        {
            s_hshThreadNames.remove(threadIdPrev);
        }
    }

    s_hshThreadNames[threadIdCurr] = i_strThreadName;
    s_hshThreadIds[i_strThreadName] = threadIdCurr;

} // RegisterCurrentThread

//------------------------------------------------------------------------------
/*! @brief Removes the current thread from the hash of known threads.

    This method may be used in none Qt applications if it is not possible to
    assign a human readable descriptive object name to the thread instance.
*/
void CLogServer::UnregisterCurrentThread()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    Qt::HANDLE threadId = QThread::currentThreadId();

    QString strThreadName = s_hshThreadNames.value(threadId, "");

    if( strThreadName.isEmpty() )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, "CLogServer::ThreadIds::" + threadId2Str(threadId));
    }
    s_hshThreadNames.remove(threadId);

    if( s_hshThreadIds.contains(strThreadName) )
    {
        s_hshThreadIds.remove(strThreadName);
    }
} // UnregisterCurrentThread

//------------------------------------------------------------------------------
/*! @brief Returns the name assigned to the current thread. If no name is assigned
           the thread id will be used.

    @return Name of the thread which may be the thread id starting with "Thread".
*/
QString CLogServer::GetCurrentThreadName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return currentThreadName();
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the pointer to the index tree with logger objects.

    @return Pointer to index tree with logger objects.
*/
CIdxTreeLoggers* CLogServer::GetLoggersIdxTree()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CIdxTreeLoggers* pLoggerIdxTree = nullptr;

    CLogServer* pLogServer = GetInstance();

    if( pLogServer != nullptr )
    {
        pLoggerIdxTree = pLogServer->getLoggersIdxTree();
    }
    return pLoggerIdxTree;
}

//------------------------------------------------------------------------------
CLogger* CLogServer::GetLogger( int i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CLogger* pLogger = nullptr;

    CLogServer* pLogServer = GetInstance();

    if( pLogServer != nullptr )
    {
        pLogger = pLogServer->getLogger(i_idxInTree);
    }
    return pLogger;
}

//------------------------------------------------------------------------------
CLogger* CLogServer::GetLogger(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CLogger* pLogger = nullptr;

    CLogServer* pLogServer = GetInstance();

    if( pLogServer != nullptr )
    {
        pLogger = pLogServer->getLogger(
            /* strNameSpace      */ i_strNameSpace,
            /* strClassName      */ i_strClassName,
            /* strObjName        */ i_strObjName,
            /* bEnabledAsDefault */ EEnabled::Undefined,
            /* eDefaultLevel     */ ELogDetailLevel::Undefined );
    }
    return pLogger;
}

//------------------------------------------------------------------------------
CLogger* CLogServer::GetLogger(
    const QString&               i_strNameSpace,
    const QString&               i_strClassName,
    const QString&               i_strObjName,
    ZS::System::EEnabled         i_bEnabledAsDefault,
    ELogDetailLevel i_eDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CLogger* pLogger = nullptr;

    CLogServer* pLogServer = GetInstance();

    if( pLogServer != nullptr )
    {
        pLogger = pLogServer->getLogger(
            /* strNameSpace      */ i_strNameSpace,
            /* strClassName      */ i_strClassName,
            /* strObjName        */ i_strObjName,
            /* bEnabledAsDefault */ i_bEnabledAsDefault,
            /* eDefaultLevel     */ i_eDefaultDetailLevel);
    }
    return pLogger;
}

//------------------------------------------------------------------------------
/*! @brief Renames the given logger by replacing the given input pointer with
           the address of the newly referenced logger.

    If the given logger will no longer be referenced it will be destroyed.
    If at the new position already a logger is existing the reference
    to this logger will be returned.
    If at the new position no logger is existing a new object is
    created and the address of the newly created object is returned.

    @param io_ppLogger [in, out]
        In:  Pointer to logger object which should be renamed. The reference counter
             of this object is decremented. If 0 the object will be destroyed.
        Out: Pointer to logger object at the new position. This might either
             be an already existing logger object whose reference counter is
             increased or a newly created object.
    @param i_strNewObjName [in] New object name.
*/
void CLogServer::RenameLogger(
    CLogger**      io_ppLogger,
    const QString& i_strNewObjName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CLogServer* pLogServer = GetInstance();

    if( pLogServer != nullptr )
    {
        pLogServer->renameLogger(io_ppLogger, i_strNewObjName);
    }
}

//------------------------------------------------------------------------------
void CLogServer::ReleaseLogger( CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CLogServer* pLogServer = GetInstance();

    if( pLogServer != nullptr )
    {
        pLogServer->releaseLogger(i_pLogger);
    }
}

/*==============================================================================
public: // class methods to get default file paths
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    Class method as setting the path got to be called before creating
    the log server instance.
*/
//------------------------------------------------------------------------------
void CLogServer::SetLoggerFileAbsoluteFilePath( const QString& i_strAbsFilePath )
{
    QMutexLocker mtxLocker(&s_mtx);

    s_strLoggerFileAbsFilePath = i_strAbsFilePath;

    CLogServer* pLogServer = GetInstance();

    if( pLogServer != nullptr )
    {
        pLogServer->setLoggerFileAbsoluteFilePath(s_strLoggerFileAbsFilePath);
    }
}

//------------------------------------------------------------------------------
/*! @brief

    Class method as setting the path got to be called before creating
    the log server instance.
*/
QString CLogServer::GetLoggerFileAbsoluteFilePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( s_strLoggerFileAbsFilePath.isEmpty() )
    {
        QString strAppConfigDir = ZS::System::getAppConfigDir("System");
        QString strLoggerFileSuffix = "xml";
        QString strLoggerFileBaseName = "ZSLogServer-TrcMthAdmObj";
        s_strLoggerFileAbsFilePath = strAppConfigDir + "/" + strLoggerFileBaseName + "." + strLoggerFileSuffix;
    }
    return s_strLoggerFileAbsFilePath;
}

//------------------------------------------------------------------------------
QString CLogServer::GetLoggerFileCompleteBaseName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QString strBaseName;
    CLogServer* pLogServer = GetInstance();
    if( pLogServer != nullptr )
    {
        strBaseName = pLogServer->getLoggerFileCompleteBaseName();
    }
    return strBaseName;
}

//------------------------------------------------------------------------------
QString CLogServer::GetLoggerFileAbsolutePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QString strAbsPath;
    CLogServer* pLogServer = GetInstance();
    if( pLogServer != nullptr )
    {
        strAbsPath = pLogServer->getLoggerFileAbsolutePath();
    }
    return strAbsPath;
}

//------------------------------------------------------------------------------
/*! @brief

    Class method as setting the path got to be called before creating
    the log server instance.
*/
//------------------------------------------------------------------------------
void CLogServer::SetLocalLogFileAbsoluteFilePath( const QString& i_strAbsFilePath )
{
    QMutexLocker mtxLocker(&s_mtx);
    s_strLocalLogFileAbsFilePath = i_strAbsFilePath;
    CLogServer* pLogServer = GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->setLocalLogFileAbsoluteFilePath(s_strLocalLogFileAbsFilePath);
    }
}

//------------------------------------------------------------------------------
/*! @brief

    Class method as setting the path got to be called before creating
    the log server instance.
*/
QString CLogServer::GetLocalLogFileAbsoluteFilePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( s_strLocalLogFileAbsFilePath.isEmpty() )
    {
        QString strAppLogDir = ZS::System::getAppLogDir("System");
        QString strTrcLogFileSuffix = "log";
        QString strTrcLogFileBaseName = "ZSLogServer-TrcMth";
        s_strLocalLogFileAbsFilePath = strAppLogDir + "/" + strTrcLogFileBaseName + "." + strTrcLogFileSuffix;
    }
    return s_strLocalLogFileAbsFilePath;
}

//------------------------------------------------------------------------------
QString CLogServer::GetLocalLogFileCompleteBaseName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QString strBaseName;
    CLogServer* pLogServer = GetInstance();
    if( pLogServer != nullptr )
    {
        strBaseName = pLogServer->getLocalLogFileCompleteBaseName();
    }
    return strBaseName;
}

//------------------------------------------------------------------------------
QString CLogServer::GetLocalLogFileAbsolutePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QString strAbsPath;
    CLogServer* pLogServer = GetInstance();
    if( pLogServer != nullptr )
    {
        strAbsPath = pLogServer->getLocalLogFileAbsolutePath();
    }
    return strAbsPath;
}

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the log server.

    The constructor is protected. The singleton class must be created via the
    static method createInstance.
*/
CLogServer::CLogServer() :
//------------------------------------------------------------------------------
    QObject(),
    m_pLoggersIdxTree(nullptr),
    m_logSettings(),
    m_pLogFile(nullptr),
    m_iRefCount(0)
{
    setObjectName("ZSLogServer");

    m_logSettings.m_strLoggerFileAbsFilePath = GetLoggerFileAbsoluteFilePath();
    m_logSettings.m_strLocalLogFileAbsFilePath = GetLocalLogFileAbsoluteFilePath();

    m_pLogFile = CLogFile::Alloc(m_logSettings.m_strLocalLogFileAbsFilePath);

    m_pLoggersIdxTree = new CIdxTreeLoggers("ZSLogServer", this);

    // See comment in "CreateInstance" above.
    s_pTheInst = this;

} // ctor

//------------------------------------------------------------------------------
/*! @brief Destroys the log server.

    The destructor is protected. The singleton class must be destroyed via the
    static method releaseInstance.
*/
CLogServer::~CLogServer()
//------------------------------------------------------------------------------
{
    if( !m_logSettings.m_strLoggerFileAbsFilePath.isEmpty() )
    {
        //m_pLoggersIdxTree->save(m_logSettings.m_strLoggerFileAbsFilePath);
    }

    try
    {
        delete m_pLoggersIdxTree;
    }
    catch(...)
    {
    }

    if( m_pLogFile != nullptr )
    {
        m_pLogFile->close();
        CLogFile::Free(m_pLogFile);
    }

    m_pLoggersIdxTree = nullptr;
    //m_logSettings;
    m_pLogFile = nullptr;
    m_iRefCount = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the index tree containing the loggers.

    @return Pointer to index tree with loggers.
*/
CIdxTreeLoggers* CLogServer::getLoggersIdxTree()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_pLoggersIdxTree;
}

/*==============================================================================
public: // instance methods to add, remove and modify admin objects
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the logger object at the given tree index.

    @param i_idxInTree [in]
        Index in tree of loggers.

    @return Pointer logger or nullptr, if no logger
            is existing at the given tree index.
*/
CLogger* CLogServer::getLogger( int i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CLogger* pLogger = nullptr;

    if( i_idxInTree < 0 )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "getTraceLogger",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "Idx In Tree (=" + QString::number(i_idxInTree) + ") is out of range");

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else
    {
        pLogger = m_pLoggersIdxTree->getLogger(i_idxInTree);
    }

    return pLogger;
}

//------------------------------------------------------------------------------
/*! @brief Returns a logger with the given name space, class and object name.

    If a logger is not yet existing with the given name space, class
    and object name a new logger is created.
    If already existing a reference counter is incremented and the pointer to
    the already existing object is returned.

    @param i_strNameSpace [in] Name space of the objects class (e.g. "ZS::Diagram")
    @param i_strClassName [in] Class name of the object (e.g. "CWdgtDiagram")
    @param i_strObjName [in] "Real" object name (e.g. "PvT" (Power versus Time))
    @param i_bEnabledAsDefault [in] Undefined means use "logServerSettings".
    @param i_eDefaultDetailLevel [in] Undefined means use "logServerSettings".

   @return Pointer to logger.
*/
CLogger* CLogServer::getLogger(
    const QString&               i_strNameSpace,
    const QString&               i_strClassName,
    const QString&               i_strObjName,
    EEnabled                     i_bEnabledAsDefault,
    ELogDetailLevel i_eDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CLogger* pLogger = nullptr;

    if( i_strObjName.isEmpty() && i_strClassName.isEmpty() && i_strNameSpace.isEmpty() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "getTraceLogger",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "Neither NameSpace nor ClassName nor ObjectName defined");

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else // if( !i_strObjName.isEmpty() || !i_strClassName.isEmpty() || !i_strNameSpace.isEmpty() )
    {
        EEnabled bEnabled = m_logSettings.m_bNewLoggersEnabledAsDefault ? EEnabled::Yes : EEnabled::No;
        ELogDetailLevel eDetailLevel = m_logSettings.m_eNewLoggersDefaultDetailLevel;

        if( i_bEnabledAsDefault != EEnabled::Undefined )
        {
            bEnabled = i_bEnabledAsDefault;
        }
        if( i_eDefaultDetailLevel != ELogDetailLevel::Undefined )
        {
            eDetailLevel = i_eDefaultDetailLevel;
        }

        pLogger = m_pLoggersIdxTree->getLogger(
            /* strNameSpace         */ i_strNameSpace,
            /* strClassName         */ i_strClassName,
            /* strObjName           */ i_strObjName,
            /* bEnabledAsDefault    */ bEnabled,
            /* eDefaultDetailLevel  */ eDetailLevel,
            /* strDefaultDataFilter */ QString(),
            /* bIncrementRefCount   */ true );
    }
    return pLogger;
}

//------------------------------------------------------------------------------
/*! @brief Renames the given loggerby replacing the given input
           pointer with the address of the newly referenced logger.

    If the given logger will no longer be referenced it will be destroyed.
    If at the new position already a logger is existing the reference
    to this logger will be returned.
    If at the new position no logger is existing a new object is
    created and the address of the newly created object is returned.

    @param io_ppLogger [in, out]
        In:  Pointer to admin object which should be renamed. The reference counter
             of this object is decremented. If 0 the object will be destroyed.
        Out: Pointer to logger at the new position. This might either
             be an already existing logger whose reference counter is
             increased or a newly created object.
    @param i_strNewObjName [in] New object name.
*/
void CLogServer::renameLogger(
    CLogger**      io_ppLogger,
    const QString& i_strNewObjName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    m_pLoggersIdxTree->renameLogger(io_ppLogger, i_strNewObjName);
}

//------------------------------------------------------------------------------
/*! @brief Releases the logger.

    The logger will not be destroyed. Only the reference counter
    will be decremented.

    @param i_pLogger [in] Pointer to logger to be released.
*/
void CLogServer::releaseLogger( CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pLogger != nullptr )
    {
        m_pLoggersIdxTree->releaseLogger(i_pLogger);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CLogServer::log(
    const CLogger* i_pLogger,
    const QString& i_strMethod,
    const QString& i_strAddInfo )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( isActive(i_pLogger) )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* strNameSpace  */ i_pLogger->getNameSpace(),
            /* strClassName  */ i_pLogger->getClassName(),
            /* strObjName    */ i_pLogger->getObjectName(),
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strAddInfo );
    }
}

//------------------------------------------------------------------------------
void CLogServer::log(
    const CLogger* i_pLogger,
    const QString& i_strObjName,
    const QString& i_strMethod,
    const QString& i_strAddInfo )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( isActive(i_pLogger) )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* strNameSpace  */ i_pLogger->getNameSpace(),
            /* strClassName  */ i_pLogger->getClassName(),
            /* strObjName    */ i_strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strAddInfo );
    }
}

//------------------------------------------------------------------------------
void CLogServer::log(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strMethod,
    const QString& i_strAddInfo )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( isActive() )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* strNameSpace  */ i_strNameSpace,
            /* strClassName  */ i_strClassName,
            /* strObjName    */ i_strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strAddInfo );
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether logging is active.

    Logging is active if logging is enabled at all (flag enabled of the
    log settings) and if the local log file is used.

    This method may be overridden to add additional checks.

    The Ipc Log Server overrides this method and also checks whether
    remote logging (output to remote client) is enabled.

    @return true if logging is active, false otherwise.
*/
bool CLogServer::isActive() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return isEnabled() && isLocalLogFileActive();
}

//------------------------------------------------------------------------------
/*! @brief Checks whether logging is active.

    Logging is active if logging is enabled at all (flag enabled of the
    log settings) and if the local log file is used.

    This method may be overridden to add additional checks.

    The Ipc Log Server overrides this method and also checks whether
    remote logging (output to remote client) is enabled.

    @return true if logging is active, false otherwise.
*/
bool CLogServer::isActive( const CLogger* i_pLogger ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    bool bIsActive = false;
    if( i_pLogger != nullptr && i_pLogger->isActive(ELogDetailLevel::CriticalError) )
    {
        bIsActive = isActive();
    }
    return bIsActive;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CLogServer::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    if( i_bEnabled != m_logSettings.m_bEnabled )
    {
        m_logSettings.m_bEnabled = i_bEnabled;

        // Close (flush buffer) of log file so it can be read by editors.
        if( m_pLogFile != nullptr )
        {
            m_pLogFile->close();
        }
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
bool CLogServer::isEnabled() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_logSettings.m_bEnabled;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CLogServer::setNewLoggersEnabledAsDefault( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_bNewLoggersEnabledAsDefault != i_bEnabled )
    {
        m_logSettings.m_bNewLoggersEnabledAsDefault = i_bEnabled;
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
bool CLogServer::areNewLoggersEnabledAsDefault() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_logSettings.m_bNewLoggersEnabledAsDefault;
}

//------------------------------------------------------------------------------
void CLogServer::setNewLoggersDefaultDetailLevel( ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_eNewLoggersDefaultDetailLevel != i_eDetailLevel )
    {
        m_logSettings.m_eNewLoggersDefaultDetailLevel = i_eDetailLevel;
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
ELogDetailLevel CLogServer::getNewLoggersDefaultDetailLevel() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_logSettings.m_eNewLoggersDefaultDetailLevel;
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CLogServer::setLoggerFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_strLoggerFileAbsFilePath != i_strAbsFilePath )
    {
        m_logSettings.m_strLoggerFileAbsFilePath = i_strAbsFilePath;
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
QString CLogServer::getLoggerFileCompleteBaseName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QFileInfo fileInfoFile(s_strLoggerFileAbsFilePath);
    return fileInfoFile.completeBaseName();
}

//------------------------------------------------------------------------------
QString CLogServer::getLoggerFileAbsolutePath() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QFileInfo fileInfoFile(s_strLoggerFileAbsFilePath);
    return fileInfoFile.absolutePath();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CLogServer::recallLoggers( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( !i_strAbsFilePath.isEmpty() && m_logSettings.m_strLoggerFileAbsFilePath != i_strAbsFilePath )
    {
        m_logSettings.m_strLoggerFileAbsFilePath = i_strAbsFilePath;

        emit logSettingsChanged(this);
    }

    return m_pLoggersIdxTree->recall(m_logSettings.m_strLoggerFileAbsFilePath);
}

//------------------------------------------------------------------------------
SErrResultInfo CLogServer::saveLoggers( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( !i_strAbsFilePath.isEmpty() && m_logSettings.m_strLoggerFileAbsFilePath != i_strAbsFilePath )
    {
        m_logSettings.m_strLoggerFileAbsFilePath = i_strAbsFilePath;
        emit logSettingsChanged(this);
    }

    return m_pLoggersIdxTree->save(m_logSettings.m_strLoggerFileAbsFilePath);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CLogServer::setLocalLogFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_strLocalLogFileAbsFilePath != i_strAbsFilePath )
    {
        m_logSettings.m_strLocalLogFileAbsFilePath = i_strAbsFilePath;

        if( m_pLogFile != nullptr )
        {
            m_pLogFile->setAbsoluteFilePath(i_strAbsFilePath);
        }
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
QString CLogServer::getLocalLogFileCompleteBaseName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    QString strBaseName;

    if( m_pLogFile != nullptr )
    {
        strBaseName = m_pLogFile->completeBaseName();
    }
    return strBaseName;
}

//------------------------------------------------------------------------------
QString CLogServer::getLocalLogFileAbsolutePath() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    QString strAbsPath;

    if( m_pLogFile != nullptr )
    {
        strAbsPath = m_pLogFile->absolutePath();
    }
    return strAbsPath;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CLogServer::setUseLocalLogFile( bool i_bUse )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_bUseLocalLogFile != i_bUse )
    {
        m_logSettings.m_bUseLocalLogFile = i_bUse;

        if( !m_logSettings.m_bUseLocalLogFile && m_pLogFile != nullptr )
        {
            m_pLogFile->close();
        }
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
bool CLogServer::isLocalLogFileUsed() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_logSettings.m_bUseLocalLogFile;
}

//------------------------------------------------------------------------------
bool CLogServer::isLocalLogFileActive() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return (m_logSettings.m_bUseLocalLogFile && m_pLogFile != nullptr);
}

//------------------------------------------------------------------------------
CLogFile* CLogServer::getLocalLogFile()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_pLogFile;
}

//------------------------------------------------------------------------------
void CLogServer::setLocalLogFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_iLocalLogFileAutoSaveInterval_ms != i_iAutoSaveInterval_ms )
    {
        m_logSettings.m_iLocalLogFileAutoSaveInterval_ms = i_iAutoSaveInterval_ms;

        if( m_pLogFile != nullptr )
        {
            m_pLogFile->setAutoSaveInterval(m_logSettings.m_iLocalLogFileAutoSaveInterval_ms);
        }
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
int CLogServer::getLocalLogFileAutoSaveIntervalInMs() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_logSettings.m_iLocalLogFileAutoSaveInterval_ms;
}

//------------------------------------------------------------------------------
void CLogServer::setLocalLogFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_bLocalLogFileCloseFileAfterEachWrite != i_bCloseFile )
    {
        m_logSettings.m_bLocalLogFileCloseFileAfterEachWrite = i_bCloseFile;

        if( m_pLogFile != nullptr )
        {
            if( m_pLogFile->getCloseFileAfterEachWrite() != i_bCloseFile )
            {
                m_pLogFile->setCloseFileAfterEachWrite(i_bCloseFile);
            }
        }
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
bool CLogServer::getLocalLogFileCloseFileAfterEachWrite() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_logSettings.m_bLocalLogFileCloseFileAfterEachWrite;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CLogServer::setLocalLogFileSubFileCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_iLocalLogFileSubFileCountMax != i_iCountMax )
    {
        m_logSettings.m_iLocalLogFileSubFileCountMax = i_iCountMax;

        if( m_pLogFile != nullptr )
        {
            m_pLogFile->setSubFileCountMax(m_logSettings.m_iLocalLogFileSubFileCountMax);
        }
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
int CLogServer::getLocalLogFileSubFileCountMax() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_logSettings.m_iLocalLogFileSubFileCountMax;
}

//------------------------------------------------------------------------------
void CLogServer::setLocalLogFileSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_iLocalLogFileSubFileLineCountMax != i_iCountMax )
    {
        m_logSettings.m_iLocalLogFileSubFileLineCountMax = i_iCountMax;

        if( m_pLogFile != nullptr )
        {
            m_pLogFile->setSubFileLineCountMax(m_logSettings.m_iLocalLogFileSubFileLineCountMax);
        }
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
int CLogServer::getLocalLogFileSubFileLineCountMax() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_logSettings.m_iLocalLogFileSubFileLineCountMax;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CLogServer::setUseIpcServer( bool i_bUse )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_bUseIpcServer != i_bUse )
    {
        m_logSettings.m_bUseIpcServer = i_bUse;
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
bool CLogServer::isIpcServerUsed() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_logSettings.m_bUseIpcServer;
}

//------------------------------------------------------------------------------
void CLogServer::setCacheLogDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_bCacheDataIfNotConnected != i_bCacheData )
    {
        m_logSettings.m_bCacheDataIfNotConnected = i_bCacheData;
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
bool CLogServer::getCacheLogDataIfNotConnected() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_logSettings.m_bCacheDataIfNotConnected;
}

//------------------------------------------------------------------------------
void CLogServer::setCacheLogDataMaxArrLen( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_iCacheDataMaxArrLen != i_iMaxArrLen )
    {
        m_logSettings.m_iCacheDataMaxArrLen = i_iMaxArrLen;
        emit logSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
int CLogServer::getCacheLogDataMaxArrLen() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_logSettings.m_iCacheDataMaxArrLen;
}

/*==============================================================================
public: // instance methods (log settings)
==============================================================================*/

//------------------------------------------------------------------------------
void CLogServer::setLogSettings( const SLogServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings != i_settings )
    {
        m_logSettings.m_bEnabled = i_settings.m_bEnabled;

        if( m_logSettings.m_strLoggerFileAbsFilePath != i_settings.m_strLoggerFileAbsFilePath )
        {
            setLoggerFileAbsoluteFilePath(i_settings.m_strLoggerFileAbsFilePath);
        }

        m_logSettings.m_bNewLoggersEnabledAsDefault = i_settings.m_bNewLoggersEnabledAsDefault;
        m_logSettings.m_eNewLoggersDefaultDetailLevel = i_settings.m_eNewLoggersDefaultDetailLevel;

        m_logSettings.m_bUseIpcServer = i_settings.m_bUseIpcServer;
        m_logSettings.m_bCacheDataIfNotConnected = i_settings.m_bCacheDataIfNotConnected;
        m_logSettings.m_iCacheDataMaxArrLen = i_settings.m_iCacheDataMaxArrLen;

        m_logSettings.m_bUseLocalLogFile = i_settings.m_bUseLocalLogFile;

        if( m_logSettings.m_strLocalLogFileAbsFilePath != i_settings.m_strLocalLogFileAbsFilePath || m_pLogFile == nullptr )
        {
            // Will take over the file name and the interval into the settings struct and saves the current settings:
            setLocalLogFileAbsoluteFilePath( i_settings.m_strLocalLogFileAbsFilePath );
        }

        if( m_logSettings.m_iLocalLogFileAutoSaveInterval_ms != i_settings.m_iLocalLogFileAutoSaveInterval_ms )
        {
            m_logSettings.m_iLocalLogFileAutoSaveInterval_ms = i_settings.m_iLocalLogFileAutoSaveInterval_ms;

            if( m_pLogFile != nullptr )
            {
                m_pLogFile->setAutoSaveInterval(m_logSettings.m_iLocalLogFileAutoSaveInterval_ms);
            }
        }

        if( m_logSettings.m_iLocalLogFileSubFileCountMax != i_settings.m_iLocalLogFileSubFileCountMax )
        {
            m_logSettings.m_iLocalLogFileSubFileCountMax = i_settings.m_iLocalLogFileSubFileCountMax;

            if( m_pLogFile != nullptr )
            {
                m_pLogFile->setSubFileCountMax(m_logSettings.m_iLocalLogFileSubFileCountMax);
            }
        }

        if( m_logSettings.m_iLocalLogFileSubFileLineCountMax != i_settings.m_iLocalLogFileSubFileLineCountMax )
        {
            m_logSettings.m_iLocalLogFileSubFileLineCountMax = i_settings.m_iLocalLogFileSubFileLineCountMax;

            if( m_pLogFile != nullptr )
            {
                m_pLogFile->setSubFileLineCountMax(m_logSettings.m_iLocalLogFileSubFileLineCountMax);
            }
        }

        if( m_logSettings.m_bLocalLogFileCloseFileAfterEachWrite != i_settings.m_bLocalLogFileCloseFileAfterEachWrite )
        {
            m_logSettings.m_bLocalLogFileCloseFileAfterEachWrite = i_settings.m_bLocalLogFileCloseFileAfterEachWrite;

            if( m_pLogFile != nullptr )
            {
                if( m_pLogFile->getCloseFileAfterEachWrite() != m_logSettings.m_bLocalLogFileCloseFileAfterEachWrite )
                {
                    m_pLogFile->setCloseFileAfterEachWrite(m_logSettings.m_bLocalLogFileCloseFileAfterEachWrite);
                }
            }
        }

        emit logSettingsChanged(this);
    }
} // setTraceSettings

//------------------------------------------------------------------------------
SLogServerSettings CLogServer::getLogSettings() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_logSettings;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CLogServer::clearLocalLogFile()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_pLogFile != nullptr )
    {
        m_pLogFile->clear();
    }
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CLogServer::addEntry(
    const QString&   i_strThreadName,
    const QDateTime& i_dt,
    double           i_fSysTimeInSec,
    const QString&   i_strNameSpace,
    const QString&   i_strClassName,
    const QString&   i_strObjName,
    const QString&   i_strMethod,
    const QString&   i_strAddInfo )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_logSettings.m_bUseLocalLogFile && m_pLogFile != nullptr )
    {
        QString strLogEntry;

        strLogEntry += i_dt.toString("yyyy-MM-dd hh:mm:ss:zzz");
        strLogEntry += " (" + QString::number(i_fSysTimeInSec, 'f', 6) + ")";

        if( m_pLoggersIdxTree != nullptr )
        {
            strLogEntry = "<" + m_pLoggersIdxTree->buildPathStr(i_strNameSpace, i_strClassName) + "> ";
        }
        else
        {
            strLogEntry = "<" + buildPathStr("::", i_strNameSpace, i_strClassName) + "> ";
        }
        if( i_strObjName.isEmpty() )
        {
            strLogEntry += i_strMethod;
        }
        else
        {
            strLogEntry += i_strObjName + "." + i_strMethod;
        }
        if( !i_strAddInfo.isEmpty() )
        {
            strLogEntry += "(" + i_strAddInfo + ")";
        }

        m_pLogFile->addEntry(strLogEntry);
    }
} // addEntry

//------------------------------------------------------------------------------
/*! Evaluates the name of the current thread.

    /return Name of the current thread.
*/
//------------------------------------------------------------------------------
QString CLogServer::currentThreadName()
{
    QString strThreadName = "Undefined";

    try
    {
        QThread* pThread = QThread::currentThread();

        if( pThread != nullptr )
        {
            strThreadName = pThread->objectName();

            if( strThreadName.length() == 0 )
            {
                Qt::HANDLE threadId = QThread::currentThreadId();

                if( s_hshThreadNames.contains(threadId) )
                {
                    strThreadName = s_hshThreadNames[threadId];
                }
                else
                {
                    strThreadName = QString("Thread") + threadId2Str(threadId);
                }
            }
        }
    }
    catch(...)
    {
    }

    return strThreadName;

} // currentThreadName

//------------------------------------------------------------------------------
/*! Returns the number of active references to this instance.
    If the count reaches 0 the instance has to be deleted.

    /return Number of active references.
*/
//------------------------------------------------------------------------------
int CLogServer::getRefCount() const
{
    return m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Increments the number of active reference to this instance.

    /return Number of active references after increment.
*/
//------------------------------------------------------------------------------
int CLogServer::incrementRefCount()
{
    return ++m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Decrements the number of active reference to this instance.
    If the count reaches 0 the instance has to be deleted.

    /return Number of active references after decrement.
*/
//------------------------------------------------------------------------------
int CLogServer::decrementRefCount()
{
    if( m_iRefCount <= 0)
    {
        throw CException(__FILE__, __LINE__, EResultObjRefCounterIsZero, "CLogServer::" + objectName());
    }
    return --m_iRefCount;
}
