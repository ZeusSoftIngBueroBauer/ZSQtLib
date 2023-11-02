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

#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>
#include <QtQml/qqmlengine.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;


/*******************************************************************************
struct STrcServerSettings
*******************************************************************************/

/*==============================================================================
public: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a server settings structure.

    Please note that the members m_strAdminObjFileAbsFilePath and
    m_strLocalTrcFileAbsFilePath only could be initialized automatically after
    the applications organizsation and application name habe been set.
    As thats not always the case they must be set "manually" at the right time.
    E.g. right after setting the applications organization and application name
    during startup as follows:

        QCoreApplication::setOrganizationName("MyOrganization");
        QCoreApplication::setOrganizationDomain("www.MyOrganization.com");
        QCoreApplication::setApplicationName("MyAppName");

        m_trcServerSettings.m_strAdminObjFileAbsFilePath = CTrcServer::GetAdminObjFileAbsoluteFilePath();
        m_trcServerSettings.m_strLocalTrcFileAbsFilePath = CTrcServer::GetLocalTrcFileAbsoluteFilePath();

    @param i_bEnabled [in] Default: true
    @param i_bNewTrcAdminObjsEnabledAsDefault [in] Default: true
    @param i_eNewTrcAdminObjsMethodCallsDefaultDetailLevel [in] Default: None
    @param i_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel [in] Default: None
    @param i_bCacheDataIfNotConnected [in] Default: false
    @param i_iCacheDataMaxArrLen [in] Default: 1000
    @param i_bUseLocalTrcFile [in] Default: true
    @param i_iLocalTrcFileAutoSaveInterval_ms [in] Default: 1000
        The default value should be the same as used by the LogFile ctor.
    @param i_iLocalTrcFileSubFileCountMax [in] Default: 5
        The default value should be the same as used by the LogFile ctor.
    @param i_iLocalTrcFileSubFileLineCountMax [in] Default: 2000
        The default value should be the same as used by the LogFile ctor.
    @param i_bLocalTrcFileCloseFileAfterEachWrite [in] Default: false
        The default value should be the same as used by the LogFile ctor.
*/
STrcServerSettings::STrcServerSettings(
    bool i_bEnabled,
    bool i_bNewTrcAdminObjsEnabledAsDefault,
    EMethodTraceDetailLevel i_eNewTrcAdminObjsDefaultMethodCallsDetailLevel,
    ELogDetailLevel i_eNewTrcAdminObjsDefaultRuntimeInfoDetailLevel,
    bool i_bUseIpcServer,
    bool i_bCacheDataIfNotConnected,
    int  i_iCacheDataMaxArrLen,
    bool i_bUseLocalTrcFile,
    int  i_iLocalTrcFileAutoSaveInterval_ms,
    int  i_iLocalTrcFileSubFileCountMax,
    int  i_iLocalTrcFileSubFileLineCountMax,
    bool i_bLocalTrcFileCloseFileAfterEachWrite ) :
//------------------------------------------------------------------------------
    m_bEnabled(i_bEnabled),
    m_strAdminObjFileAbsFilePath(),
    m_bNewTrcAdminObjsEnabledAsDefault(i_bNewTrcAdminObjsEnabledAsDefault),
    m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel(i_eNewTrcAdminObjsDefaultMethodCallsDetailLevel),
    m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel(i_eNewTrcAdminObjsDefaultRuntimeInfoDetailLevel),
    m_bUseIpcServer(i_bUseIpcServer),
    m_bCacheDataIfNotConnected(i_bCacheDataIfNotConnected),
    m_iCacheDataMaxArrLen(i_iCacheDataMaxArrLen),
    m_bUseLocalTrcFile(i_bUseLocalTrcFile),
    m_strLocalTrcFileAbsFilePath(),
    m_iLocalTrcFileAutoSaveInterval_ms(i_iLocalTrcFileAutoSaveInterval_ms),
    m_iLocalTrcFileSubFileCountMax(i_iLocalTrcFileSubFileCountMax),
    m_iLocalTrcFileSubFileLineCountMax(i_iLocalTrcFileSubFileLineCountMax),
    m_bLocalTrcFileCloseFileAfterEachWrite(i_bLocalTrcFileCloseFileAfterEachWrite)
{
} // ctor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Equal operator.

    @param i_settingsOther [in]
*/
bool STrcServerSettings::operator == ( const STrcServerSettings& i_settingsOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_bEnabled != i_settingsOther.m_bEnabled )
    {
        bEqual = false;
    }
    else if( m_bNewTrcAdminObjsEnabledAsDefault != i_settingsOther.m_bNewTrcAdminObjsEnabledAsDefault )
    {
        bEqual = false;
    }
    else if( m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel != i_settingsOther.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel )
    {
        bEqual = false;
    }
    else if( m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel != i_settingsOther.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel )
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
    else if( m_bUseLocalTrcFile != i_settingsOther.m_bUseLocalTrcFile )
    {
        bEqual = false;
    }
    else if( m_iLocalTrcFileAutoSaveInterval_ms != i_settingsOther.m_iLocalTrcFileAutoSaveInterval_ms )
    {
        bEqual = false;
    }
    else if( m_iLocalTrcFileSubFileCountMax != i_settingsOther.m_iLocalTrcFileSubFileCountMax )
    {
        bEqual = false;
    }
    else if( m_iLocalTrcFileSubFileLineCountMax != i_settingsOther.m_iLocalTrcFileSubFileLineCountMax )
    {
        bEqual = false;
    }
    else if( m_bLocalTrcFileCloseFileAfterEachWrite != i_settingsOther.m_bLocalTrcFileCloseFileAfterEachWrite )
    {
        bEqual = false;
    }
    else if( m_strAdminObjFileAbsFilePath != i_settingsOther.m_strAdminObjFileAbsFilePath )
    {
        bEqual = false;
    }
    else if( m_strLocalTrcFileAbsFilePath != i_settingsOther.m_strLocalTrcFileAbsFilePath )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
/*! @brief Unequal operator.

    @param i_settingsOther [in]
*/
bool STrcServerSettings::operator != ( const STrcServerSettings& i_settingsOther ) const
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
QString STrcServerSettings::toString() const
//------------------------------------------------------------------------------
{
    QString str;
    str += "Enabled: " + bool2Str(m_bEnabled);
    str += ", AdmObjFile: " + m_strAdminObjFileAbsFilePath;
    str += ", AdmObjDefEnabled: " + bool2Str(m_bNewTrcAdminObjsEnabledAsDefault);
    str += ", AdmObjMthCallsDefLevel: " + CEnum<EMethodTraceDetailLevel>(m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel).toString();
    str += ", AdmObjRunInfoDefLevel: " + CEnum<ELogDetailLevel>(m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel).toString();
    str += ", UseIpcServer: " + bool2Str(m_bUseIpcServer);
    str += ", CacheData: " + bool2Str(m_bCacheDataIfNotConnected);
    str += ", CacheArrLen: " + QString::number(m_iCacheDataMaxArrLen);
    str += ", UseLocalFile: " + bool2Str(m_bUseLocalTrcFile);
    str += ", TrcFile: " + m_strLocalTrcFileAbsFilePath;
    str += ", TrcFileSave: " + QString::number(m_iLocalTrcFileAutoSaveInterval_ms) + " ms";
    str += ", TrcFileSubFiles: " + QString::number(m_iLocalTrcFileSubFileCountMax);
    str += ", TrcFileLines: " + QString::number(m_iLocalTrcFileSubFileLineCountMax);
    str += ", TrcFileClose: " + bool2Str(m_bLocalTrcFileCloseFileAfterEachWrite);
    return str;
}


/*******************************************************************************
class CTrcServer : public QObject
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QMutex CTrcServer::s_mtx(QMutex::Recursive);
CTrcServer* CTrcServer::s_pTheInst = nullptr;
QHash<Qt::HANDLE, QString> CTrcServer::s_hshThreadNames;
QHash<QString, Qt::HANDLE> CTrcServer::s_hshThreadIds;
QString CTrcServer::s_strAdminObjFileAbsFilePath;
QString CTrcServer::s_strLocalTrcFileAbsFilePath;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the address of the trace server.

    This method does neither create an instance of the class nor increments
    the reference counter.
    If no instance has been created yet the method returns nullptr.

    If you just need to access the already existing instance and you can be sure
    that the instance is already existing this method should be preferred to the
    createInstance call as this method does not affect the reference counter and
    there is no need to call releaseInstance later on.

    @note After a getInstance call a releaseInstance MUST NOT be called.

    @return Pointer to trace server or nullptr, if the instance has not been created yet.
*/
CTrcServer* CTrcServer::GetInstance()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return s_pTheInst;
}

//------------------------------------------------------------------------------
/*! @brief Creates the trace server if the trace server is not already existing.

    If the trace server is already existing the reference to the existing
    trace server is returned and a reference counter is incremented.

    @param i_eTrcDetailLevel [in]
        If the methods of the trace server itself should be logged a value
        greater than 0 (EMethodTraceDetailLevel::None) could be passed here.
        But of course trace output may only be written to the local trace
        method file.
    @param i_eTrcDetailLevelMutex [in]
        If the locking and unlocking of the mutex of trace server
        should be logged a value greater than 0 (EMethodTraceDetailLevel::None)
        could be passed here. But the value will be ignored if the detail
        level for tracing the server is None.
    @param i_eTrcDetailLevelAdminObjIdxTree [in]
        If the methods of the admin object index tree should be logged a value
        greater than 0 (EMethodTraceDetailLevel::None) could be passed here.
    @param i_eTrcDetailLevelAdminObjIdxTreeMutex [in]
        If the locking and unlocking of the mutex of admin object index tree
        should be logged a value greater than 0 (EMethodTraceDetailLevel::None)
        could be passed here. But the value will be ignored if the detail
        level for the admin object index tree is None.

    @return Pointer to trace server instance.
*/
CTrcServer* CTrcServer::CreateInstance(
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel i_eTrcDetailLevelMutex,
    EMethodTraceDetailLevel i_eTrcDetailLevelAdminObjIdxTree,
    EMethodTraceDetailLevel i_eTrcDetailLevelAdminObjIdxTreeMutex )
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
        new CTrcServer(
            i_eTrcDetailLevel,
            i_eTrcDetailLevelMutex,
            i_eTrcDetailLevelAdminObjIdxTree,
            i_eTrcDetailLevelAdminObjIdxTreeMutex);
    }

    s_pTheInst->incrementRefCount();

    return s_pTheInst;

} // CreateInstance

//------------------------------------------------------------------------------
/*! @brief Releases the trace server instance.

    Before invoking this method a reference to the instance must have been retrieved
    with a createInstance call.

    This method decrements the reference counter of the instance.
    If the reference counter reaches 0 the instance will be destroyed.

    @note A reference returned by getInstance MUST NOT be freed.
*/
void CTrcServer::ReleaseInstance()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( s_pTheInst == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultSingletonClassNotInstantiated, "ZS::System::CTrcServer");
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
void CTrcServer::RegisterThread( const QString& i_strThreadName, void* i_pvThreadHandle )
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

} // RegisterThread

//------------------------------------------------------------------------------
/*! @brief Removes the current thread from the hash of known threads.

    This method may be used in none Qt applications if it is not possible to
    assign a human readable descriptive object name to the thread instance.
*/
void CTrcServer::UnregisterThread( void* i_pvThreadHandle )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    Qt::HANDLE threadId = QThread::currentThreadId();

    QString strThreadName = s_hshThreadNames.value(threadId, "");

    if( strThreadName.isEmpty() )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, "CTrcServer::ThreadIds::" + threadId2Str(threadId));
    }
    s_hshThreadNames.remove(threadId);

    if( s_hshThreadIds.contains(strThreadName) )
    {
        s_hshThreadIds.remove(strThreadName);
    }
} // UnregisterThread

//------------------------------------------------------------------------------
/*! @brief Returns the name assigned to the current thread. If no name is assigned
           the thread id will be used.

    @return Name of the thread which may be the thread id starting with "Thread".
*/
QString CTrcServer::GetThreadName( void* i_pvThreadHandle )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return currentThreadName();
}

//------------------------------------------------------------------------------
/*! @brief Returns the name assigned to the current thread. If no name is assigned
           the thread id will be used.

    @return Name of the thread which may be the thread id starting with "Thread".
*/
QString CTrcServer::GetCurrentThreadName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return currentThreadName();
}

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the pointer to the index tree with trace admin objects.

    @return Pointer to index tree with trace admin objects.
*/
CIdxTreeTrcAdminObjs* CTrcServer::GetTraceAdminObjIdxTree()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CIdxTreeTrcAdminObjs* pTrcAdminObjIdxTree = nullptr;

    CTrcServer* pTrcServer = GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcAdminObjIdxTree = pTrcServer->getTraceAdminObjIdxTree();
    }
    return pTrcAdminObjIdxTree;
}

//------------------------------------------------------------------------------
CTrcAdminObj* CTrcServer::GetTraceAdminObj( int i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    CTrcServer* pTrcServer = GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcAdminObj = pTrcServer->getTraceAdminObj(i_idxInTree);
    }
    return pTrcAdminObj;
}

//------------------------------------------------------------------------------
CTrcAdminObj* CTrcServer::GetTraceAdminObj(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    CTrcServer* pTrcServer = GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcAdminObj = pTrcServer->getTraceAdminObj(
            /* strNameSpace             */ i_strNameSpace,
            /* strClassName             */ i_strClassName,
            /* strObjName               */ i_strObjName,
            /* bEnabledAsDefault        */ EEnabled::Undefined,
            /* eMethodCallsDefaultLevel */ EMethodTraceDetailLevel::Undefined,
            /* eRuntimeInfoDefaultLevel */ ELogDetailLevel::Undefined );
    }
    return pTrcAdminObj;
}

//------------------------------------------------------------------------------
CTrcAdminObj* CTrcServer::GetTraceAdminObj(
    const QString&          i_strNameSpace,
    const QString&          i_strClassName,
    const QString&          i_strObjName,
    ZS::System::EEnabled    i_bEnabledAsDefault,
    EMethodTraceDetailLevel i_eMethodCallsDefaultDetailLevel,
    ELogDetailLevel         i_eRuntimeInfoDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    CTrcServer* pTrcServer = GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcAdminObj = pTrcServer->getTraceAdminObj(
            /* strNameSpace             */ i_strNameSpace,
            /* strClassName             */ i_strClassName,
            /* strObjName               */ i_strObjName,
            /* bEnabledAsDefault        */ i_bEnabledAsDefault,
            /* eMethodCallsDefaultLevel */ i_eMethodCallsDefaultDetailLevel,
            /* eRuntimeInfoDefaultLevel */ i_eRuntimeInfoDefaultDetailLevel);
    }
    return pTrcAdminObj;
}

//------------------------------------------------------------------------------
/*! @brief Renames the given trace admin object by replacing the given input
           pointer with the address of the newly referenced trace admin object.

    If the given trace admin object will no longer be referenced it will be destroyed.
    If at the new position already a trace admin object is existing the reference
    to this admin object will be returned.
    If at the new position no trace admin object is existing a new object is
    created and the address of the newly created object is returned.

    @param io_ppTrcAdminObj [in, out]
        In:  Pointer to admin object which should be renamed. The reference counter
             of this object is decremented. If 0 the object will be destroyed.
        Out: Pointer to trace admin object at the new position. This might either
             be an already existing trace admin object whose reference counter is
             increased or a newly created object.
    @param i_strNewObjName [in] New object name.
*/
CTrcAdminObj* CTrcServer::RenameTraceAdminObj(
    CTrcAdminObj*  i_pTrcAdminObj,
    const QString& i_strNewObjName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CTrcAdminObj* pTrcAdminObj = i_pTrcAdminObj;

    CTrcServer* pTrcServer = GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcAdminObj = pTrcServer->renameTraceAdminObj(i_pTrcAdminObj, i_strNewObjName);
    }
    return pTrcAdminObj;
}

//------------------------------------------------------------------------------
void CTrcServer::ReleaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CTrcServer* pTrcServer = GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->releaseTraceAdminObj(i_pTrcAdminObj);
    }
}

/*==============================================================================
public: // class methods to get default file paths
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief

    Class method as setting the path got to be called before creating
    the trace server instance.
*/
void CTrcServer::SetAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    s_strAdminObjFileAbsFilePath = i_strAbsFilePath;
    CTrcServer* pTrcServer = GetInstance();
    if( pTrcServer != nullptr )
    {
        pTrcServer->setAdminObjFileAbsoluteFilePath(s_strAdminObjFileAbsFilePath);
    }
}

//------------------------------------------------------------------------------
/*! @brief

    Class method as setting the path got to be called before creating
    the trace server instance.
*/
QString CTrcServer::GetAdminObjFileAbsoluteFilePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( s_strAdminObjFileAbsFilePath.isEmpty() )
    {
        QString strAppConfigDir = ZS::System::getAppConfigDir("System");
        QString strTrcAdminObjFileSuffix = "xml";
        QString strTrcAdminObjFileBaseName = "ZSTrcServer-TrcMthAdmObj";
        SetAdminObjFileAbsoluteFilePath(
            strAppConfigDir + QDir::separator() + strTrcAdminObjFileBaseName + "." + strTrcAdminObjFileSuffix);
    }
    return s_strAdminObjFileAbsFilePath;
}

//------------------------------------------------------------------------------
QString CTrcServer::GetAdminObjFileCompleteBaseName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QFileInfo fileInfoFile(s_strAdminObjFileAbsFilePath);
    return fileInfoFile.completeBaseName();
}

//------------------------------------------------------------------------------
QString CTrcServer::GetAdminObjFileAbsolutePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QFileInfo fileInfoFile(s_strAdminObjFileAbsFilePath);
    return fileInfoFile.absolutePath();
}

//------------------------------------------------------------------------------
/*! @brief

    Class method as setting the path got to be called before creating
    the trace server instance.
*/
//------------------------------------------------------------------------------
void CTrcServer::SetLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath )
{
    QMutexLocker mtxLocker(&s_mtx);
    s_strLocalTrcFileAbsFilePath = i_strAbsFilePath;
    CTrcServer* pTrcServer = GetInstance();
    if( pTrcServer != nullptr )
    {
        pTrcServer->setLocalTrcFileAbsoluteFilePath(s_strLocalTrcFileAbsFilePath);
    }
}

//------------------------------------------------------------------------------
/*! @brief

    Class method as setting the path got to be called before creating
    the trace server instance.
*/
QString CTrcServer::GetLocalTrcFileAbsoluteFilePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( s_strLocalTrcFileAbsFilePath.isEmpty() )
    {
        QString strAppLogDir = ZS::System::getAppLogDir("System");
        QString strTrcLogFileSuffix = "log";
        QString strTrcLogFileBaseName = "ZSTrcServer-TrcMth";
        SetLocalTrcFileAbsoluteFilePath(
            strAppLogDir + QDir::separator() + strTrcLogFileBaseName + "." + strTrcLogFileSuffix);
    }
    return s_strLocalTrcFileAbsFilePath;
}

//------------------------------------------------------------------------------
QString CTrcServer::GetLocalTrcFileCompleteBaseName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QFileInfo fileInfoFile(s_strLocalTrcFileAbsFilePath);
    return fileInfoFile.completeBaseName();
}

//------------------------------------------------------------------------------
QString CTrcServer::GetLocalTrcFileAbsolutePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QFileInfo fileInfoFile(s_strLocalTrcFileAbsFilePath);
    return fileInfoFile.absolutePath();
}

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates the trace server.

    The constructor is protected. The singleton class must be created via the
    static method createInstance.

    @param i_eTrcDetailLevel [in]
        To trace the methods of the trace server itself a value greater than
        None may be passed here.
    @param i_eTrcDetailLevelMutex [in]
        If the locking and unlocking of the mutex of trace server
        should be logged a value greater than 0 (EMethodTraceDetailLevel::None)
        could be passed here. But the value will be ignored if the detail
        level for tracing the server is None.
    @param i_eTrcDetailLevelAdminObjIdxTree [in]
        If the methods of the admin object index tree should be traced a value
        greater than 0 (EMethodTraceDetailLevel::None) could be passed here.
    @param i_eTrcDetailLevelAdminObjIdxTreeMutex [in]
        If the locking and unlocking of the mutex of admin object index tree
        should be traced a value greater than 0 (EMethodTraceDetailLevel::None)
        could be passed here. But the value will be ignored if the detail
        level for the admin object index tree is None.
*/
CTrcServer::CTrcServer(
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel /*i_eTrcDetailLevelMutex*/,
    EMethodTraceDetailLevel i_eTrcDetailLevelAdminObjIdxTree,
    EMethodTraceDetailLevel i_eTrcDetailLevelAdminObjIdxTreeMutex ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTrcAdminObjIdxTree(nullptr),
    m_trcSettings(),
    m_pTrcMthFile(nullptr),
    m_eTrcDetailLevel(i_eTrcDetailLevel),
    m_iRefCount(0)
{
    setObjectName("ZSTrcServer");

    m_trcSettings.m_strAdminObjFileAbsFilePath = GetAdminObjFileAbsoluteFilePath();
    m_trcSettings.m_strLocalTrcFileAbsFilePath = GetLocalTrcFileAbsoluteFilePath();

    m_pTrcMthFile = CTrcMthFile::Alloc(m_trcSettings.m_strLocalTrcFileAbsFilePath);

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );

    // Create index tree of trace admin objects. Pass the server as the parent object.
    // If the parent object is the trace server the index tree will not create a trace
    // admin object to trace the method calls.
    m_pTrcAdminObjIdxTree = new CIdxTreeTrcAdminObjs(
        "ZSTrcServer", this,
        i_eTrcDetailLevelAdminObjIdxTree, i_eTrcDetailLevelAdminObjIdxTreeMutex);

    // See comment in "CreateInstance" above.
    s_pTheInst = this;

} // ctor

//------------------------------------------------------------------------------
/*! @brief Destroys the trace server.

    The destructor is protected. The singleton class must be destroyed via the
    static method releaseInstance.
*/
CTrcServer::~CTrcServer()
//------------------------------------------------------------------------------
{
    // The method tracer to trace method enter and method leave cannot be used here.
    // The trace method file will be destroyed before method leave is traced.
    // As a workaround the method tracers scope is left before the trace method
    // file is closed and freed.

    {   CMethodTracer mthTracer(
            /* pTrcMthFile        */ m_pTrcMthFile,
            /* eTrcDetailLevel    */ m_eTrcDetailLevel,
            /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ NameSpace(),
            /* strClassName       */ ClassName(),
            /* strObjName         */ objectName(),
            /* strMethod          */ "dtor",
            /* strMthInArgs       */ "" );

        if( !m_trcSettings.m_strAdminObjFileAbsFilePath.isEmpty() )
        {
            //m_pTrcAdminObjIdxTree->save(m_trcSettings.m_strAdminObjFileAbsFilePath);
        }

        try
        {
            delete m_pTrcAdminObjIdxTree;
        }
        catch(...)
        {
        }
    } // CMethodTracer mthTracer(

    // Delete the trace method file after the index tree and after the mutex
    // as both may use the trace method file for method tracing.
    if( m_pTrcMthFile != nullptr )
    {
        m_pTrcMthFile->close();
        CTrcMthFile::Free(m_pTrcMthFile);
    }

    m_pTrcAdminObjIdxTree = nullptr;
    //m_trcSettings;
    m_pTrcMthFile = nullptr;
    m_eTrcDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_iRefCount = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the index tree containing the trace admin objects.

    @return Pointer to index tree with trace admin objects.
*/
CIdxTreeTrcAdminObjs* CTrcServer::getTraceAdminObjIdxTree()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_pTrcAdminObjIdxTree;
}

/*==============================================================================
public: // instance methods to add, remove and modify admin objects
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the trace admin object at the given tree index.

    @param i_idxInTree [in]
        Index in tree of trace admin objects.

    @return Pointer trace admin object or nullptr, if no trace admin object
            is existing at the given tree index.
*/
CTrcAdminObj* CTrcServer::getTraceAdminObj( int i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "getTraceAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    if( i_idxInTree < 0 )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ NameSpace(), ClassName(), objectName(), "getTraceAdminObj",
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
        pTrcAdminObj = m_pTrcAdminObjIdxTree->getTraceAdminObj(i_idxInTree);

        if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            if( pTrcAdminObj != nullptr )
            {
                strMthRet = pTrcAdminObj->getCalculatedKeyInTree();
            }
            else
            {
                strMthRet = "nullptr";
            }
            mthTracer.setMethodReturn(strMthRet);
        }
    }

    return pTrcAdminObj;

} // getTraceAdminObj

//------------------------------------------------------------------------------
/*! @brief Returns a trace admin object with the given name space, class and
           object name.

    If a trace admin object is not yet existing with the given name space, class
    and object name a new trace admin object is created.
    If already existing a reference counter is incremented and the pointer to
    the already existing object is returned.

    @param i_strNameSpace [in] Name space of the objects class (e.g. "ZS::Diagram")
    @param i_strClassName [in] Class name of the object (e.g. "CWdgtDiagram")
    @param i_strObjName [in] "Real" object name (e.g. "PvT" (Power versus Time))
    @param i_bEnabledAsDefault [in] Undefined means use "trcServerSettings".
    @param i_eMethodCallsDefaultDetailLevel [in] Undefined means use "trcServerSettings".
    @param i_eRuntimeInfoDefaultDetailLevel [in] Undefined means use "trcServerSettings".

    @return Pointer to trace admin object.
*/
CTrcAdminObj* CTrcServer::getTraceAdminObj(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strEnabledAsDefault,
    const QString& i_strMethodCallsDefaultDetailLevel,
    const QString& i_strRuntimeInfoDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "NameSpace: " + i_strNameSpace;
        strMthInArgs += ", ClassName: " + i_strClassName;
        strMthInArgs += ", ObjName: " + i_strObjName;
        strMthInArgs += ", EnabledAsDefault: " + i_strEnabledAsDefault;
        strMthInArgs += ", MethodCallsDefault: " + i_strMethodCallsDefaultDetailLevel;
        strMthInArgs += ", RuntimeInfoDefault: " + i_strRuntimeInfoDefaultDetailLevel;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "getTraceAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    CEnumEnabled eEnabledAsDefault(i_strEnabledAsDefault);
    CEnumMethodTraceDetailLevel eMethodCallsDefaultDetailLevel(i_strMethodCallsDefaultDetailLevel);
    CEnumLogDetailLevel eRuntimeInfoDefaultDetailLevel(i_strRuntimeInfoDefaultDetailLevel);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    if( !eEnabledAsDefault.isValid() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ NameSpace(), ClassName(), objectName(), "getTraceAdminObj",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "EnabledAsDefault " + i_strEnabledAsDefault + " is out of range");
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else if( !eMethodCallsDefaultDetailLevel.isValid() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ NameSpace(), ClassName(), objectName(), "getTraceAdminObj",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "MethodCallsDefaultDetailLevel " + i_strMethodCallsDefaultDetailLevel + " is out of range");
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else if( !eRuntimeInfoDefaultDetailLevel.isValid() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ NameSpace(), ClassName(), objectName(), "getTraceAdminObj",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "RuntimeInfoDefaultDetailLevel " + i_strRuntimeInfoDefaultDetailLevel + " is out of range");
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else if( i_strObjName.isEmpty() && i_strClassName.isEmpty() && i_strNameSpace.isEmpty() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ NameSpace(), ClassName(), objectName(), "getTraceAdminObj",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "Neither NameSpace nor ClassName nor ObjectName defined");
        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else
    {
        EEnabled eEnabled = m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault ? EEnabled::Yes : EEnabled::No;
        EMethodTraceDetailLevel eDetailLevelMethodCalls = m_trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel;
        ELogDetailLevel eDetailLevelRuntimeInfo = m_trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel;

        if( eEnabledAsDefault != EEnabled::Undefined )
        {
            eEnabled = eEnabledAsDefault.enumerator();
        }
        if( eMethodCallsDefaultDetailLevel != EMethodTraceDetailLevel::Undefined )
        {
            eDetailLevelMethodCalls = eMethodCallsDefaultDetailLevel.enumerator();
        }
        if( eRuntimeInfoDefaultDetailLevel != ELogDetailLevel::Undefined )
        {
            eDetailLevelRuntimeInfo = eRuntimeInfoDefaultDetailLevel.enumerator();
        }

        pTrcAdminObj = m_pTrcAdminObjIdxTree->getTraceAdminObj(
            /* strNameSpace                   */ i_strNameSpace,
            /* strClassName                   */ i_strClassName,
            /* strObjName                     */ i_strObjName,
            /* bEnabledAsDefault              */ eEnabled,
            /* eDefaultDetailLevelMethodCalls */ eDetailLevelMethodCalls,
            /* eDefaultDetailLevelRuntimeInfo */ eDetailLevelRuntimeInfo,
            /* strDefaultDataFilter           */ QString(),
            /* bIncrementRefCount             */ true );
        QQmlEngine::setObjectOwnership(pTrcAdminObj, QQmlEngine::CppOwnership);

        if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            if( pTrcAdminObj != nullptr )
            {
                strMthRet = pTrcAdminObj->getCalculatedKeyInTree();
            }
            else
            {
                strMthRet = "nullptr";
            }
            mthTracer.setMethodReturn(strMthRet);
        }
    } // if( !i_strObjName.isEmpty() || !i_strClassName.isEmpty() || !i_strNameSpace.isEmpty() )

    return pTrcAdminObj;

} // getTraceAdminObj

//------------------------------------------------------------------------------
/*! @brief Releases the trace admin object.

    The trace admin object will not be destroyed. Only the reference counter
    will be decremented.

    @param i_pTrcAdminObj [in] Pointer to trace admin object to be released.
*/
void CTrcServer::releaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_pTrcAdminObj == nullptr ? "nullptr" : i_pTrcAdminObj->getCalculatedKeyInTree();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "releaseTraceAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( i_pTrcAdminObj != nullptr )
    {
        m_pTrcAdminObjIdxTree->releaseTraceAdminObj(i_pTrcAdminObj);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns a trace admin object with the given name space, class and
           object name.

    If a trace admin object is not yet existing with the given name space, class
    and object name a new trace admin object is created.
    If already existing a reference counter is incremented and the pointer to
    the already existing object is returned.

    @param i_strNameSpace [in] Name space of the objects class (e.g. "ZS::Diagram")
    @param i_strClassName [in] Class name of the object (e.g. "CWdgtDiagram")
    @param i_strObjName [in] "Real" object name (e.g. "PvT" (Power versus Time))
    @param i_eEnabledAsDefault [in] Undefined means use "trcServerSettings".
    @param i_eMethodCallsDefaultDetailLevel [in] Undefined means use "trcServerSettings".
    @param i_eRuntimeInfoDefaultDetailLevel [in] Undefined means use "trcServerSettings".

   @return Pointer to trace admin object.
*/
CTrcAdminObj* CTrcServer::getTraceAdminObj(
    const QString&          i_strNameSpace,
    const QString&          i_strClassName,
    const QString&          i_strObjName,
    EEnabled                i_eEnabledAsDefault,
    EMethodTraceDetailLevel i_eMethodCallsDefaultDetailLevel,
    ELogDetailLevel         i_eRuntimeInfoDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "NameSpace: " + i_strNameSpace;
        strMthInArgs += ", ClassName: " + i_strClassName;
        strMthInArgs += ", ObjName: " + i_strObjName;
        strMthInArgs += ", EnabledAsDefault: " + CEnumEnabled::toString(i_eEnabledAsDefault);
        strMthInArgs += ", MethodCallsDefault: " + CEnumMethodTraceDetailLevel(i_eMethodCallsDefaultDetailLevel).toString();
        strMthInArgs += ", RuntimeInfoDefault: " + CEnumLogDetailLevel(i_eRuntimeInfoDefaultDetailLevel).toString();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "getTraceAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    if( i_strObjName.isEmpty() && i_strClassName.isEmpty() && i_strNameSpace.isEmpty() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ NameSpace(), ClassName(), objectName(), "getTraceAdminObj",
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
        EEnabled eEnabled = m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault ? EEnabled::Yes : EEnabled::No;
        EMethodTraceDetailLevel eDetailLevelMethodCalls = m_trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel;
        ELogDetailLevel eDetailLevelRuntimeInfo = m_trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel;

        if( i_eEnabledAsDefault != EEnabled::Undefined )
        {
            eEnabled = i_eEnabledAsDefault;
        }
        if( i_eMethodCallsDefaultDetailLevel != EMethodTraceDetailLevel::Undefined )
        {
            eDetailLevelMethodCalls = i_eMethodCallsDefaultDetailLevel;
        }
        if( i_eRuntimeInfoDefaultDetailLevel != ELogDetailLevel::Undefined )
        {
            eDetailLevelRuntimeInfo = i_eRuntimeInfoDefaultDetailLevel;
        }

        pTrcAdminObj = m_pTrcAdminObjIdxTree->getTraceAdminObj(
            /* strNameSpace                   */ i_strNameSpace,
            /* strClassName                   */ i_strClassName,
            /* strObjName                     */ i_strObjName,
            /* eEnabledAsDefault              */ eEnabled,
            /* eDefaultDetailLevelMethodCalls */ eDetailLevelMethodCalls,
            /* eDefaultDetailLevelRuntimeInfo */ eDetailLevelRuntimeInfo,
            /* strDefaultDataFilter           */ QString(),
            /* bIncrementRefCount             */ true );
        QQmlEngine::setObjectOwnership(pTrcAdminObj, QQmlEngine::CppOwnership);

        if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            if( pTrcAdminObj != nullptr )
            {
                strMthRet = pTrcAdminObj->getCalculatedKeyInTree();
            }
            else
            {
                strMthRet = "nullptr";
            }
            mthTracer.setMethodReturn(strMthRet);
        }
    } // if( !i_strObjName.isEmpty() || !i_strClassName.isEmpty() || !i_strNameSpace.isEmpty() )

    return pTrcAdminObj;

} // getTraceAdminObj

//------------------------------------------------------------------------------
/*! @brief Renames the given trace admin object by replacing the given input
           pointer with the address of the newly referenced trace admin object.

    If the given trace admin object will no longer be referenced it will be destroyed.
    If at the new position already a trace admin object is existing the reference
    to this admin object will be returned.
    If at the new position no trace admin object is existing a new object is
    created and the address of the newly created object is returned.

    @param i_pTrcAdminObj [in]
        Pointer to admin object which should be renamed. The reference counter
        of this object is decremented. If 0 the object will be destroyed.
    @param i_strNewObjName [in] New object name.

    @return Pointer to trace admin object at the new position.
        This might either be an already existing trace admin object whose
        reference counter is increased or a newly created object.
*/
CTrcAdminObj* CTrcServer::renameTraceAdminObj(
    CTrcAdminObj*  i_pTrcAdminObj,
    const QString& i_strNewObjName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = QString(i_pTrcAdminObj == nullptr ? "nullptr" : (i_pTrcAdminObj)->getCalculatedKeyInTree());
        strMthInArgs += ", NewObjName: " + i_strNewObjName;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "renameTraceAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);
    CTrcAdminObj* pTrcAdminObj =
        m_pTrcAdminObjIdxTree->renameTraceAdminObj(i_pTrcAdminObj, i_strNewObjName);
    QQmlEngine::setObjectOwnership(pTrcAdminObj, QQmlEngine::CppOwnership);
    return pTrcAdminObj;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Traces entering a method.

    Trace output is only created

    - if the trace admin objects detail level is not set to None,
    - if the trace server outputs are enabled and
    - if output to local method trace file is activated.

    Output to local method trace file may be deactivated via the flag
    'UseLocalTrcFile' of the servers trace settings.

    The name of the current thread, the current date time and the time in seconds
    since the applications has been started is inserted at the beginning of the
    log entry.

    For each thread (defined by its name) the call depth is maintained.
    Spaces are inserted according to the current call depth of the thread.
    After output of trace string the call depth for the thread is incremented.
    For the indentation to work correctly each thread must have a unique name.
    For safety, the class name of the thread class can be prepended to the object
    name of the thread:

    @code
    class MyThread : public QThread {
    public:
        MyThread() : QThread() {
            setObjectName("MyThread-" + m_strMyClass2ObjName);
        }
    };
    @endcode

    After the thread and time information and the call stack indentation by
    thread the tag '->' is added to indicate that the method is entered.

    At next the name space and class name is output encapsulated in the tags '<' and '>'.

    After the class information the method is output.

    - For instance tracers the name of the instance for which the method is called
      is added followed by the method name. The object name is separted by a '.'
      from the method name.
    - For class tracers only the method name is output.

    At last the method input arguments are added encapsulated in the tags '(' and ')'.

    Typical log outputs will look like:

    @code
    <GUIMain > 2022-04-04 22:17:30:569 ( 27.720377): -> <ZS::Apps::Test::IpcTrace::CMyClass1> classMethod(Hello Class)
    <GUIMain > 2022-04-04 22:17:30:581 ( 27.741674):    -> <ZS::Apps::Test::IpcTrace::CMyClass1> classMethod(Hello Class)
    @endcode

    **Instance Tracer**

    @code
    <GUIMain > 2022-04-04 22:17:30:569 ( 27.720377): -> <ZS::Apps::Test::IpcTrace::CMyClass1> Inst1.instMethod(Hello Class)
    <GUIMain > 2022-04-04 22:17:30:581 ( 27.741674):    -> <ZS::Apps::Test::IpcTrace::CMyClass1> Inst1.instMethod(Hello Class)
    @endcode

    @param i_pTrcAdminObj [in] Pointer to trace admin object.
        In addition to the trace detail level the name space, the class name
        and - in case of an instance tracer - the instance name is taken from
        the trace admin object.

    @param i_strMethod [in] Name of the entered method.

    @param i_strMethodInArgs [in] String describing the input arguments.
        It is up to the caller what is included in this string. If only
        one argument is passed the name of the argument may be omitted.
        In some cases the argument name makes no real sense. E.g. for a method
        'setEnabled' it doesn't really make sense to add "Enabled: " before the
        arguments value. If there are several input arguments it is useful to
        add the name of the argument.

        The following rule for type of arguments may be used:

        - For values with units add the unit symbol at the end of the value.
          @code
          'Freq: 56.0 kHz'
          @endcode

        - For structures encapsulate the members in '{' and '}'.
          @code
          'Settings: {Freq: 45 Hz, Level: 6 dBm}'
          @endcode

        - For lists start with the length of the list encapsulated in '[' and ']'.
          If the list contains elements encapsulate the list in '(' and ')' and
          each element in '{' and '}'. For each element - if desired - start with
          the index (or key) followed by ':' and the value. If the value is a
          structure or a list again follow the rules for structures and lists.
          @code
          'Frequencies: [3]({0: 45 kHz}, {1: 12 Hz}, {2: 20.0 MHz})
          'Settings: [2]({Input: {Freq: 45 kHz, Level: 5 W}}, {Output: {Freq: 12 Hz, Level: 2 W}})
          'Arbitrary: [2]({Elem1: {Val1: uups}}, {Elem2: [2]({0: argh}, {1: aha})})
          @endcode
*/
void CTrcServer::traceMethodEnter(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strMethod,
    const QString&      i_strMethodInArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( areMethodCallsActive(i_pTrcAdminObj) )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::Enter,
            /* strNameSpace  */ i_pTrcAdminObj->getNameSpace(),
            /* strClassName  */ i_pTrcAdminObj->getClassName(),
            /* strObjName    */ i_pTrcAdminObj->getObjectName(),
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strMethodInArgs );
    }
}

//------------------------------------------------------------------------------
void CTrcServer::traceMethodEnter(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strObjName,
    const QString&      i_strMethod,
    const QString&      i_strMethodInArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( areMethodCallsActive(i_pTrcAdminObj) )
    {
        QString strObjName = i_strObjName;
        if (i_strObjName.isEmpty()) {
            strObjName = i_pTrcAdminObj->getObjectName();
        }
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::Enter,
            /* strNameSpace  */ i_pTrcAdminObj->getNameSpace(),
            /* strClassName  */ i_pTrcAdminObj->getClassName(),
            /* strObjName    */ strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strMethodInArgs );
    }
}

//------------------------------------------------------------------------------
void CTrcServer::traceMethod(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strMethod,
    const QString&      i_strAddInfo )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( isRuntimeInfoActive(i_pTrcAdminObj) )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::None,
            /* strNameSpace  */ i_pTrcAdminObj->getNameSpace(),
            /* strClassName  */ i_pTrcAdminObj->getClassName(),
            /* strObjName    */ i_pTrcAdminObj->getObjectName(),
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strAddInfo );
    }
}

//------------------------------------------------------------------------------
void CTrcServer::traceMethod(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strObjName,
    const QString&      i_strMethod,
    const QString&      i_strAddInfo )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( isRuntimeInfoActive(i_pTrcAdminObj) )
    {
        QString strObjName = i_strObjName;
        if (i_strObjName.isEmpty()) {
            strObjName = i_pTrcAdminObj->getObjectName();
        }
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::None,
            /* strNameSpace  */ i_pTrcAdminObj->getNameSpace(),
            /* strClassName  */ i_pTrcAdminObj->getClassName(),
            /* strObjName    */ strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strAddInfo );
    }
}

//------------------------------------------------------------------------------
/*! @brief

    **Class Tracer**

    @code
    <GUIMain > 2022-04-04 22:17:30:570 ( 27.720972): <- <ZS::Apps::Test::IpcTrace::CMyClass1> classMethod(): Hello World
    @endcode

    **Instance Tracer**

    @code
    <GUIMain > 2022-04-04 22:17:30:570 ( 27.720972): <- <ZS::Apps::Test::IpcTrace::CMyClass1> Inst1.instMethod(): Hello World
    @endcode
*/
void CTrcServer::traceMethodLeave(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strMethod,
    const QString&      i_strMethodReturn,
    const QString&      i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( areMethodCallsActive(i_pTrcAdminObj) )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::Leave,
            /* strNameSpace  */ i_pTrcAdminObj->getNameSpace(),
            /* strClassName  */ i_pTrcAdminObj->getClassName(),
            /* strObjName    */ i_pTrcAdminObj->getObjectName(),
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strMethodReturn,
            /* strMthOutArgs */ i_strMethodOutArgs );
    }
}

//------------------------------------------------------------------------------
void CTrcServer::traceMethodLeave(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strObjName,
    const QString&      i_strMethod,
    const QString&      i_strMethodReturn,
    const QString&      i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( areMethodCallsActive(i_pTrcAdminObj) )
    {
        QString strObjName = i_strObjName;
        if (i_strObjName.isEmpty()) {
            strObjName = i_pTrcAdminObj->getObjectName();
        }
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::Leave,
            /* strNameSpace  */ i_pTrcAdminObj->getNameSpace(),
            /* strClassName  */ i_pTrcAdminObj->getClassName(),
            /* strObjName    */ strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strMethodReturn,
            /* strMthOutArgs */ i_strMethodOutArgs );
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::traceMethodEnter(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strMethod,
    const QString& i_strMethodInArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( isActive() )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::Enter,
            /* strNameSpace  */ i_strNameSpace,
            /* strClassName  */ i_strClassName,
            /* strObjName    */ i_strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strMethodInArgs );
    }
}

//------------------------------------------------------------------------------
void CTrcServer::traceMethod(
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
            /* mthDir        */ EMethodDir::None,
            /* strNameSpace  */ i_strNameSpace,
            /* strClassName  */ i_strClassName,
            /* strObjName    */ i_strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strAddInfo );
    }
}

//------------------------------------------------------------------------------
void CTrcServer::traceMethodLeave(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strMethod,
    const QString& i_strMethodReturn,
    const QString& i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( isActive() )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::Leave,
            /* strNameSpace  */ i_strNameSpace,
            /* strClassName  */ i_strClassName,
            /* strObjName    */ i_strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strMethodReturn,
            /* strMthOutArgs */ i_strMethodOutArgs );
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing is active.

    Tracing is active if tracing is enabled at all (flag enabled of the
    trace settings) and if the local trace file is used.

    This method may be overridden to add additional checks.

    The Ipc Trace Server overrides this method and also checks whether
    remote tracing (output to remote client) is enabled.

    @return true if tracing is active, false otherwise.
*/
bool CTrcServer::isActive() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return isEnabled() && isLocalTrcFileActive();
}

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing is active.

    Tracing is active if tracing is enabled at all (flag enabled of the
    trace settings) and if the local trace file is used.

    This method may be overridden to add additional checks.

    The Ipc Trace Server overrides this method and also checks whether
    remote tracing (output to remote client) is enabled.

    @return true if tracing is active, false otherwise.
*/
bool CTrcServer::areMethodCallsActive( const CTrcAdminObj* i_pTrcAdminObj ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    bool bIsActive = false;
    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->areMethodCallsActive(EMethodTraceDetailLevel::EnterLeave) )
    {
        bIsActive = isActive();
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
/*! @brief Checks whether tracing is active.

    Tracing is active if tracing is enabled at all (flag enabled of the
    trace settings) and if the local trace file is used.

    This method may be overridden to add additional checks.

    The Ipc Trace Server overrides this method and also checks whether
    remote tracing (output to remote client) is enabled.

    @return true if tracing is active, false otherwise.
*/
bool CTrcServer::isRuntimeInfoActive( const CTrcAdminObj* i_pTrcAdminObj ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    bool bIsActive = false;
    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isRuntimeInfoActive(ELogDetailLevel::Fatal) )
    {
        bIsActive = isActive();
    }
    return bIsActive;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setEnabled",
        /* strMthInArgs       */ "" );

    QMutexLocker mtxLocker(&s_mtx);

    if( i_bEnabled != m_trcSettings.m_bEnabled )
    {
        m_trcSettings.m_bEnabled = i_bEnabled;

        // Close (flush buffer) of trace file so it can be read by editors.
        if( m_pTrcMthFile != nullptr )
        {
            m_pTrcMthFile->close();
        }
        emit traceSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
bool CTrcServer::isEnabled() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_bEnabled;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bEnabled);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setNewTrcAdminObjsEnabledAsDefault",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault != i_bEnabled )
    {
        m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault = i_bEnabled;

        emit traceSettingsChanged(this);
    }
} // setNewTrcAdminObjsEnabledAsDefault

//------------------------------------------------------------------------------
bool CTrcServer::areNewTrcAdminObjsEnabledAsDefault() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault;
}

//------------------------------------------------------------------------------
void CTrcServer::setNewTrcAdminObjsMethodCallsDefaultDetailLevel( EMethodTraceDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = CEnumMethodTraceDetailLevel(i_eDetailLevel).toString();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setNewTrcAdminObjsMethodCallsDefaultDetailLevel",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel != i_eDetailLevel )
    {
        m_trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel = i_eDetailLevel;

        emit traceSettingsChanged(this);
    }
} // setNewTrcAdminObjsMethodCallsDefaultDetailLevel

//------------------------------------------------------------------------------
EMethodTraceDetailLevel CTrcServer::getNewTrcAdminObjsMethodCallsDefaultDetailLevel() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel;
}

//------------------------------------------------------------------------------
void CTrcServer::setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel( ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = CEnumLogDetailLevel(i_eDetailLevel).toString();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel != i_eDetailLevel )
    {
        m_trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = i_eDetailLevel;

        emit traceSettingsChanged(this);
    }
} // setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel

//------------------------------------------------------------------------------
ELogDetailLevel CTrcServer::getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel;
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "AbsFilePath: " + i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setAdminObjFileAbsoluteFilePath",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_strAdminObjFileAbsFilePath != i_strAbsFilePath )
    {
        m_trcSettings.m_strAdminObjFileAbsFilePath = i_strAbsFilePath;
        s_strAdminObjFileAbsFilePath = i_strAbsFilePath;
        emit traceSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
QString CTrcServer::getAdminObjFileAbsoluteFilePath() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return s_strAdminObjFileAbsFilePath;
}

//------------------------------------------------------------------------------
QString CTrcServer::getAdminObjFileCompleteBaseName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QFileInfo fileInfoFile(s_strAdminObjFileAbsFilePath);
    return fileInfoFile.completeBaseName();
}

//------------------------------------------------------------------------------
QString CTrcServer::getAdminObjFileAbsolutePath() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QFileInfo fileInfoFile(s_strAdminObjFileAbsFilePath);
    return fileInfoFile.absolutePath();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CTrcServer::recallAdminObjs( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "AbsFilePath: " + i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "recallAdminObjs",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( !i_strAbsFilePath.isEmpty() && m_trcSettings.m_strAdminObjFileAbsFilePath != i_strAbsFilePath )
    {
        setAdminObjFileAbsoluteFilePath(i_strAbsFilePath);
    }

    SErrResultInfo errResultInfo = m_pTrcAdminObjIdxTree->recall(m_trcSettings.m_strAdminObjFileAbsFilePath);

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }
    return errResultInfo;
}

//------------------------------------------------------------------------------
SErrResultInfo CTrcServer::saveAdminObjs( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "AbsFilePath: " + i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "saveAdminObjs",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( !i_strAbsFilePath.isEmpty() && m_trcSettings.m_strAdminObjFileAbsFilePath != i_strAbsFilePath )
    {
        setAdminObjFileAbsoluteFilePath(i_strAbsFilePath);
    }

    SErrResultInfo errResultInfo = m_pTrcAdminObjIdxTree->save(m_trcSettings.m_strAdminObjFileAbsFilePath);

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }
    return errResultInfo;
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "AbsFilePath: " + i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalTrcFileAbsoluteFilePath",
        /* strMthInArgs       */ strMthInArgs );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_strLocalTrcFileAbsFilePath != i_strAbsFilePath )
    {
        m_trcSettings.m_strLocalTrcFileAbsFilePath = i_strAbsFilePath;
        s_strLocalTrcFileAbsFilePath = i_strAbsFilePath;
        if( m_pTrcMthFile != nullptr )
        {
            m_pTrcMthFile->setAbsoluteFilePath(i_strAbsFilePath);
        }
        emit traceSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
QString CTrcServer::getLocalTrcFileAbsoluteFilePath() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return s_strLocalTrcFileAbsFilePath;
}

//------------------------------------------------------------------------------
QString CTrcServer::getLocalTrcFileCompleteBaseName() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QFileInfo fileInfoFile(s_strLocalTrcFileAbsFilePath);
    return fileInfoFile.completeBaseName();
}

//------------------------------------------------------------------------------
QString CTrcServer::getLocalTrcFileAbsolutePath() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    QFileInfo fileInfoFile(s_strLocalTrcFileAbsFilePath);
    return fileInfoFile.absolutePath();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setUseLocalTrcFile( bool i_bUse )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setUseLocalTrcFile",
        /* strMthInArgs       */ "" );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_bUseLocalTrcFile != i_bUse )
    {
        m_trcSettings.m_bUseLocalTrcFile = i_bUse;

        if( !m_trcSettings.m_bUseLocalTrcFile && m_pTrcMthFile != nullptr )
        {
            m_pTrcMthFile->close();
        }

        emit traceSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
bool CTrcServer::isLocalTrcFileUsed() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_bUseLocalTrcFile;
}

//------------------------------------------------------------------------------
bool CTrcServer::isLocalTrcFileActive() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return (m_trcSettings.m_bUseLocalTrcFile && m_pTrcMthFile != nullptr);
}

//------------------------------------------------------------------------------
CTrcMthFile* CTrcServer::getLocalTrcFile()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_pTrcMthFile;
}

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalTrcFileAutoSaveIntervalInMs",
        /* strMthInArgs       */ "" );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms != i_iAutoSaveInterval_ms )
    {
        m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms = i_iAutoSaveInterval_ms;

        if( m_pTrcMthFile != nullptr )
        {
            m_pTrcMthFile->setAutoSaveInterval(m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms);
        }
        emit traceSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
int CTrcServer::getLocalTrcFileAutoSaveIntervalInMs() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms;
}

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalTrcFileCloseFileAfterEachWrite",
        /* strMthInArgs       */ "" );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite != i_bCloseFile )
    {
        m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite = i_bCloseFile;

        if( m_pTrcMthFile != nullptr )
        {
            if( m_pTrcMthFile->getCloseFileAfterEachWrite() != i_bCloseFile )
            {
                m_pTrcMthFile->setCloseFileAfterEachWrite(i_bCloseFile);
            }
        }

        emit traceSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
bool CTrcServer::getLocalTrcFileCloseFileAfterEachWrite() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileSubFileCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalTrcFileSubFileCountMax",
        /* strMthInArgs       */ "" );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_iLocalTrcFileSubFileCountMax != i_iCountMax )
    {
        m_trcSettings.m_iLocalTrcFileSubFileCountMax = i_iCountMax;

        if( m_pTrcMthFile != nullptr )
        {
            m_pTrcMthFile->setSubFileCountMax(m_trcSettings.m_iLocalTrcFileSubFileCountMax);
        }
        emit traceSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
int CTrcServer::getLocalTrcFileSubFileCountMax() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_iLocalTrcFileSubFileCountMax;
}

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setLocalTrcFileSubFileLineCountMax",
        /* strMthInArgs       */ "" );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_iLocalTrcFileSubFileLineCountMax != i_iCountMax )
    {
        m_trcSettings.m_iLocalTrcFileSubFileLineCountMax = i_iCountMax;

        if( m_pTrcMthFile != nullptr )
        {
            m_pTrcMthFile->setSubFileLineCountMax(m_trcSettings.m_iLocalTrcFileSubFileLineCountMax);
        }
        emit traceSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
int CTrcServer::getLocalTrcFileSubFileLineCountMax() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_iLocalTrcFileSubFileLineCountMax;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setUseIpcServer( bool i_bUse )
//------------------------------------------------------------------------------
{
    // When the mutex creates trace output also this method should be traced.
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setUseIpcServer",
        /* strMthInArgs       */ "" );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_bUseIpcServer != i_bUse )
    {
        m_trcSettings.m_bUseIpcServer = i_bUse;
        emit traceSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
bool CTrcServer::isIpcServerUsed() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_bUseIpcServer;
}

//------------------------------------------------------------------------------
void CTrcServer::setCacheTrcDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setCacheTrcDataIfNotConnected",
        /* strMthInArgs       */ "" );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_bCacheDataIfNotConnected != i_bCacheData )
    {
        m_trcSettings.m_bCacheDataIfNotConnected = i_bCacheData;

        emit traceSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
bool CTrcServer::getCacheTrcDataIfNotConnected() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_bCacheDataIfNotConnected;
}

//------------------------------------------------------------------------------
void CTrcServer::setCacheTrcDataMaxArrLen( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setCacheTrcDataMaxArrLen",
        /* strMthInArgs       */ "" );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_iCacheDataMaxArrLen != i_iMaxArrLen )
    {
        m_trcSettings.m_iCacheDataMaxArrLen = i_iMaxArrLen;

        emit traceSettingsChanged(this);
    }
}

//------------------------------------------------------------------------------
int CTrcServer::getCacheTrcDataMaxArrLen() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_iCacheDataMaxArrLen;
}

/*==============================================================================
public: // instance methods (trace settings)
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setTraceSettings( const STrcServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setTraceSettings",
        /* strMthInArgs       */ "" );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings != i_settings )
    {
        m_trcSettings.m_bEnabled = i_settings.m_bEnabled;

        if( m_trcSettings.m_strAdminObjFileAbsFilePath != i_settings.m_strAdminObjFileAbsFilePath )
        {
            setAdminObjFileAbsoluteFilePath(i_settings.m_strAdminObjFileAbsFilePath);
        }

        m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault = i_settings.m_bNewTrcAdminObjsEnabledAsDefault;
        m_trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel = i_settings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel;
        m_trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel = i_settings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel;

        m_trcSettings.m_bUseIpcServer = i_settings.m_bUseIpcServer;
        m_trcSettings.m_bCacheDataIfNotConnected = i_settings.m_bCacheDataIfNotConnected;
        m_trcSettings.m_iCacheDataMaxArrLen = i_settings.m_iCacheDataMaxArrLen;

        m_trcSettings.m_bUseLocalTrcFile = i_settings.m_bUseLocalTrcFile;

        if( m_trcSettings.m_strLocalTrcFileAbsFilePath != i_settings.m_strLocalTrcFileAbsFilePath || m_pTrcMthFile == nullptr )
        {
            // Will take over the file name and the interval into the settings struct and saves the current settings:
            setLocalTrcFileAbsoluteFilePath( i_settings.m_strLocalTrcFileAbsFilePath );
        }

        if( m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms != i_settings.m_iLocalTrcFileAutoSaveInterval_ms )
        {
            m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms = i_settings.m_iLocalTrcFileAutoSaveInterval_ms;

            if( m_pTrcMthFile != nullptr )
            {
                m_pTrcMthFile->setAutoSaveInterval(m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms);
            }
        }

        if( m_trcSettings.m_iLocalTrcFileSubFileCountMax != i_settings.m_iLocalTrcFileSubFileCountMax )
        {
            m_trcSettings.m_iLocalTrcFileSubFileCountMax = i_settings.m_iLocalTrcFileSubFileCountMax;

            if( m_pTrcMthFile != nullptr )
            {
                m_pTrcMthFile->setSubFileCountMax(m_trcSettings.m_iLocalTrcFileSubFileCountMax);
            }
        }

        if( m_trcSettings.m_iLocalTrcFileSubFileLineCountMax != i_settings.m_iLocalTrcFileSubFileLineCountMax )
        {
            m_trcSettings.m_iLocalTrcFileSubFileLineCountMax = i_settings.m_iLocalTrcFileSubFileLineCountMax;

            if( m_pTrcMthFile != nullptr )
            {
                m_pTrcMthFile->setSubFileLineCountMax(m_trcSettings.m_iLocalTrcFileSubFileLineCountMax);
            }
        }

        if( m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite != i_settings.m_bLocalTrcFileCloseFileAfterEachWrite )
        {
            m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite = i_settings.m_bLocalTrcFileCloseFileAfterEachWrite;

            if( m_pTrcMthFile != nullptr )
            {
                if( m_pTrcMthFile->getCloseFileAfterEachWrite() != m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite )
                {
                    m_pTrcMthFile->setCloseFileAfterEachWrite(m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite);
                }
            }
        }

        emit traceSettingsChanged(this);
    }
} // setTraceSettings

//------------------------------------------------------------------------------
STrcServerSettings CTrcServer::getTraceSettings() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::clearLocalTrcFile()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLavel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "clearLocalTrcFile",
        /* strMthInArgs       */ "" );

    QMutexLocker mtxLocker(&s_mtx);

    if( m_pTrcMthFile != nullptr )
    {
        m_pTrcMthFile->clear();
    }
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::addEntry(
    const QString&         i_strThreadName,
    const QDateTime&       i_dt,
    double                 i_fSysTimeInSec,
    ZS::System::EMethodDir i_mthDir,
    const QString&         i_strNameSpace,
    const QString&         i_strClassName,
    const QString&         i_strObjName,
    const QString&         i_strMethod,
    const QString&         i_strAddInfo,
    const QString&         i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_bUseLocalTrcFile && m_pTrcMthFile != nullptr )
    {
        QString strMth;

        if( m_pTrcAdminObjIdxTree != nullptr )
        {
            strMth = "<" + m_pTrcAdminObjIdxTree->buildPathStr(i_strNameSpace, i_strClassName) + "> ";
        }
        else
        {
            strMth = "<" + buildPathStr("::", i_strNameSpace, i_strClassName) + "> ";
        }
        if( i_strObjName.isEmpty() )
        {
            strMth += i_strMethod;
        }
        else
        {
            strMth += i_strObjName + "." + i_strMethod;
        }

        if( i_mthDir == EMethodDir::Enter )
        {
            m_pTrcMthFile->traceMethodEnter(
                /* strMthThreadName */ i_strThreadName,
                /* dt               */ i_dt,
                /* fSysTimeInSec    */ i_fSysTimeInSec,
                /* strMethod        */ strMth,
                /* strMthInArgs     */ i_strAddInfo );
        }
        else if( i_mthDir == EMethodDir::Leave )
        {
            m_pTrcMthFile->traceMethodLeave(
                /* strMthThreadName */ i_strThreadName,
                /* dt               */ i_dt,
                /* fSysTimeInSec    */ i_fSysTimeInSec,
                /* strMethod        */ strMth,
                /* strMthRet        */ i_strAddInfo,
                /* strMthOutArgs    */ i_strMethodOutArgs );
        }
        else if( i_mthDir == EMethodDir::None )
        {
            m_pTrcMthFile->traceMethod(
                /* strMthThreadName */ i_strThreadName,
                /* dt               */ i_dt,
                /* fSysTimeInSec    */ i_fSysTimeInSec,
                /* strMethod        */ strMth,
                /* strAddInfo       */ i_strAddInfo );
        }
    } // if( m_trcSettings.m_bUseLocalTrcFile && m_pTrcMthFile != nullptr )

} // addEntry

//------------------------------------------------------------------------------
/*! Evaluates the name of the current thread.

    /return Name of the current thread.
*/
//------------------------------------------------------------------------------
QString CTrcServer::currentThreadName()
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
int CTrcServer::getRefCount() const
{
    return m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Increments the number of active reference to this instance.

    /return Number of active references after increment.
*/
//------------------------------------------------------------------------------
int CTrcServer::incrementRefCount()
{
    return ++m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Decrements the number of active reference to this instance.
    If the count reaches 0 the instance has to be deleted.

    /return Number of active references after decrement.
*/
//------------------------------------------------------------------------------
int CTrcServer::decrementRefCount()
{
    if( m_iRefCount <= 0)
    {
        throw CException(__FILE__, __LINE__, EResultObjRefCounterIsZero, "CTrcServer::" + objectName());
    }
    return --m_iRefCount;
}
