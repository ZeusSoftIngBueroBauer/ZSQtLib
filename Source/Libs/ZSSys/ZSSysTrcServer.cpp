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

#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;


/*******************************************************************************
struct STrcServerSettings
*******************************************************************************/

/*==============================================================================
public: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
STrcServerSettings::STrcServerSettings(
    bool i_bEnabled,
    bool i_bNewTrcAdminObjsActivatedAsDefault,
    int  i_iNewTrcAdminObjsDefaultDetailLevel,
    bool i_bCacheDataIfNotConnected,
    int  i_iCacheDataMaxArrLen,
    bool i_bUseLocalTrcFile,
    int  i_iLocalTrcFileAutoSaveInterval_ms,
    int  i_iLocalTrcFileSubFileCountMax,
    int  i_iLocalTrcFileSubFileLineCountMax,
    bool i_bLocalTrcFileCloseFileAfterEachWrite ) :
//------------------------------------------------------------------------------
    m_bEnabled(i_bEnabled),
    m_bNewTrcAdminObjsEnabledAsDefault(i_bNewTrcAdminObjsActivatedAsDefault),
    m_iNewTrcAdminObjsDefaultDetailLevel(i_iNewTrcAdminObjsDefaultDetailLevel),
    m_bCacheDataIfNotConnected(i_bCacheDataIfNotConnected),
    m_iCacheDataMaxArrLen(i_iCacheDataMaxArrLen),
    m_strAdminObjFileAbsFilePath(),
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
    else if( m_iNewTrcAdminObjsDefaultDetailLevel != i_settingsOther.m_iNewTrcAdminObjsDefaultDetailLevel )
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
    else if( m_strAdminObjFileAbsFilePath != i_settingsOther.m_strAdminObjFileAbsFilePath )
    {
        bEqual = false;
    }
    else if( m_bUseLocalTrcFile != i_settingsOther.m_bUseLocalTrcFile )
    {
        bEqual = false;
    }
    else if( m_strLocalTrcFileAbsFilePath != i_settingsOther.m_strLocalTrcFileAbsFilePath )
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
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool STrcServerSettings::operator != ( const STrcServerSettings& i_settingsOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_settingsOther);
}


/*******************************************************************************
class CTrcServer : public QObject
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

QMutex CTrcServer::s_mtx(QMutex::Recursive);
QHash<QString, CTrcServer*> CTrcServer::s_hshpInstances;
QHash<Qt::HANDLE, QString> CTrcServer::s_hshThreadNames;
QHash<QString, Qt::HANDLE> CTrcServer::s_hshThreadIds;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CTrcServer* CTrcServer::GetInstance( const QString& i_strName )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return s_hshpInstances.value(i_strName, nullptr);
}

//------------------------------------------------------------------------------
/*! Creates a trace server with the given name if a trace server with the given
    name is not already existing.

    If a trace server with the given name is already existing the reference to
    the existing trace server is returned and a reference counter is incremented.

    \param i_strName [in] Name of the trace server (default "ZSTrcServer")
    \param i_iTrcDetailLevel [in]
        If the methods of the trace server itself should be logged a value
        greater than 0 (ETraceDetailLevelNone) could be passed here.

    \return Pointer to trace server instance.
*/
CTrcServer* CTrcServer::CreateInstance(
    const QString& i_strName,
    int i_iTrcDetailLevel )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CTrcServer* pTrcServer = s_hshpInstances.value(i_strName, nullptr);

    if( pTrcServer == NULL )
    {
        pTrcServer = new CTrcServer(i_strName, i_iTrcDetailLevel);
    }

    pTrcServer->incrementRefCount();

    // The ctor adds the reference to the instance to the hash.
    // If the ctor itself calls methods of other classes which again recursively
    // call "GetInstance" this way "GetInstance" does not return null but the
    // pointer to the server instance currently beeing created.
    // But of course this requires special caution as within the ctor it must
    // be assured that recursively accessed instance members are already valid.
    //s_hshpInstances[i_strName] = pTrcServer;

    return pTrcServer;

} // CreateInstance

//------------------------------------------------------------------------------
void CTrcServer::ReleaseInstance( const QString& i_strName )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CTrcServer* pTrcServer = s_hshpInstances.value(i_strName, nullptr);

    if( pTrcServer == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, "CTrcServer::" + i_strName);
    }

    int iRefCount = pTrcServer->decrementRefCount();

    if( iRefCount == 0 )
    {
        s_hshpInstances.remove(i_strName);

        try
        {
            delete pTrcServer;
        }
        catch(...)
        {
        }
        pTrcServer = nullptr;
    }
} // ReleaseInstance

//------------------------------------------------------------------------------
void CTrcServer::ReleaseInstance( CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strName = i_pTrcServer->objectName();

    if( !s_hshpInstances.contains(strName) )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, "CTrcServer::" + strName);
    }

    int iRefCount = i_pTrcServer->decrementRefCount();

    if( iRefCount == 0 )
    {
        s_hshpInstances.remove(strName);

        delete i_pTrcServer;
        i_pTrcServer = nullptr;
    }
} // ReleaseInstance

//------------------------------------------------------------------------------
void CTrcServer::DestroyAllInstances()
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CTrcServer* pTrcServer;
    QString     strName;

    QHash<QString, CTrcServer*>::iterator itTrcServer;

    for( itTrcServer = s_hshpInstances.begin(); itTrcServer != s_hshpInstances.end(); itTrcServer++ )
    {
        strName = itTrcServer.key();
        pTrcServer = itTrcServer.value();

        s_hshpInstances[strName] = nullptr;

        delete pTrcServer;
        pTrcServer = nullptr;
    }

    s_hshpInstances.clear();

} // DestroyAllInstances

/*==============================================================================
public: // class methods to register thread names
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::RegisterCurrentThread( const QString& i_strThreadName )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
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
void CTrcServer::UnregisterCurrentThread()
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
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

} // UnregisterCurrentThread

//------------------------------------------------------------------------------
QString CTrcServer::GetCurrentThreadName()
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    return currentThreadName();

} // UnregisterCurrentThread

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
CIdxTreeTrcAdminObjs* CTrcServer::GetTraceAdminObjIdxTree( const QString& i_strServerName )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CIdxTreeTrcAdminObjs* pTrcAdminObjIdxTree = nullptr;

    CTrcServer* pTrcServer = GetInstance(i_strServerName);

    if( pTrcServer != nullptr )
    {
        pTrcAdminObjIdxTree = pTrcServer->getTraceAdminObjIdxTree();
    }

    return pTrcAdminObjIdxTree;

} // GetTraceAdminObjIdxTree

//------------------------------------------------------------------------------
CTrcAdminObj* CTrcServer::GetTraceAdminObj(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strServerName )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    CTrcServer* pTrcServer = GetInstance(i_strServerName);

    if( pTrcServer != nullptr )
    {
        pTrcAdminObj = pTrcServer->getTraceAdminObj(
            /* strNameSpace        */ i_strNameSpace,
            /* strClassName        */ i_strClassName,
            /* strObjName          */ i_strObjName,
            /* bEnabledAsDefault   */ EEnabled::Undefined,
            /* iDefaultDetailLevel */ -1 );
    }

    return pTrcAdminObj;

} // GetTraceAdminObj

//------------------------------------------------------------------------------
CTrcAdminObj* CTrcServer::GetTraceAdminObj(
    const QString&       i_strNameSpace,
    const QString&       i_strClassName,
    const QString&       i_strObjName,
    ZS::System::EEnabled i_bEnabledAsDefault,
    int                  i_iDefaultDetailLevel,
    const QString&       i_strServerName )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    CTrcServer* pTrcServer = GetInstance(i_strServerName);

    if( pTrcServer != nullptr )
    {
        pTrcAdminObj = pTrcServer->getTraceAdminObj(
            /* strNameSpace        */ i_strNameSpace,
            /* strClassName        */ i_strClassName,
            /* strObjName          */ i_strObjName,
            /* bEnabledAsDefault   */ i_bEnabledAsDefault,
            /* iDefaultDetailLevel */ i_iDefaultDetailLevel );
    }

    return pTrcAdminObj;

} // GetTraceAdminObj

//------------------------------------------------------------------------------
void CTrcServer::ReleaseTraceAdminObj(
    CTrcAdminObj*  i_pTrcAdminObj,
    const QString& i_strServerName )
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    CTrcServer* pTrcServer = GetInstance(i_strServerName);

    if( pTrcServer != nullptr )
    {
        pTrcServer->releaseTraceAdminObj(i_pTrcAdminObj);
    }

} // ReleaseTraceAdminObj

/*==============================================================================
public: // class methods to get default file paths
==============================================================================*/

//------------------------------------------------------------------------------
QString CTrcServer::GetDefaultAdminObjFileAbsoluteFilePath( const QString& i_strIniFileScope )
//------------------------------------------------------------------------------
{
    QString strAppConfigDir = ZS::System::getAppConfigDir(i_strIniFileScope);

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strTrcAdminObjFileSuffix = "xml";
    QString strTrcAdminObjFileBaseName = strAppNameNormalized + "-TrcMthAdmObj";

    return strAppConfigDir + "/" + strTrcAdminObjFileBaseName + "." + strTrcAdminObjFileSuffix;

} // GetDefaultAdminObjFileAbsoluteFilePath

//------------------------------------------------------------------------------
QString CTrcServer::GetDefaultLocalTrcFileAbsoluteFilePath( const QString& i_strIniFileScope )
//------------------------------------------------------------------------------
{
    QString strAppLogDir = ZS::System::getAppLogDir(i_strIniFileScope);

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strTrcLogFileSuffix = "log";
    QString strTrcLogFileBaseName = strAppNameNormalized + "-TrcMth";

    return strAppLogDir + "/" + strTrcLogFileBaseName + "." + strTrcLogFileSuffix;

} // GetDefaultLocalTrcFileAbsoluteFilePath

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTrcServer::CTrcServer( const QString& i_strName, int i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pTrcAdminObjIdxTree(nullptr),
    m_trcSettings(),
    m_pTrcMthFile(nullptr),
    m_iTrcDetailLevel(i_iTrcDetailLevel),
    m_iRefCount(0)
{
    setObjectName(i_strName);

    m_trcSettings.m_strAdminObjFileAbsFilePath = GetDefaultAdminObjFileAbsoluteFilePath();
    m_trcSettings.m_strLocalTrcFileAbsFilePath = GetDefaultLocalTrcFileAbsoluteFilePath();

    m_pTrcMthFile = CTrcMthFile::Alloc(m_trcSettings.m_strLocalTrcFileAbsFilePath);

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );

    // Create index tree of trace admin objects. Pass the server as the parent object.
    // If the parent object is the trace server the index tree will not create a trace
    // admin object to trace the method calls.
    m_pTrcAdminObjIdxTree = new CIdxTreeTrcAdminObjs(i_strName, this, i_iTrcDetailLevel);

    // See comment in "CreateInstance" above.
    s_hshpInstances[i_strName] = this;

} // ctor

//------------------------------------------------------------------------------
CTrcServer::~CTrcServer()
//------------------------------------------------------------------------------
{
    // The method tracer to trace method enter and method leave cannot be used here.
    // The trace method file will be destroyed before method leave is traced.
    // As a workaround the method tracers scope is left before the trace method
    // file is closed and freed.

    {   CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
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

    // Delete the trace method file after the index tree as the
    // index tree may use the trace method file for method tracing.
    if( m_pTrcMthFile != nullptr )
    {
        m_pTrcMthFile->close();
    }

    CTrcMthFile::Free(m_pTrcMthFile);

    m_pTrcAdminObjIdxTree = nullptr;
    //m_trcSettings;
    m_pTrcMthFile = nullptr;
    m_iTrcDetailLevel = 0;
    m_iRefCount = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CIdxTreeTrcAdminObjs* CTrcServer::getTraceAdminObjIdxTree()
//------------------------------------------------------------------------------
{
    // The class may be accessed from within different thread contexts and
    // therefore accessing the class must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_pTrcAdminObjIdxTree;
}

/*==============================================================================
public: // instance methods to add, remove and modify admin objects
==============================================================================*/

//------------------------------------------------------------------------------
CTrcAdminObj* CTrcServer::getTraceAdminObj(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    EEnabled       i_bEnabledAsDefault,
    int            i_iDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;
    QString strMthRet;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "NameSpace: " + i_strNameSpace;
        strMthInArgs += ", ClassName: " + i_strClassName;
        strMthInArgs += ", ObjName: " + i_strObjName;
        strMthInArgs += ", EnabledAsDefault: " + CEnumEnabled::toString(i_bEnabledAsDefault);
        strMthInArgs += ", DefaultDetailLevel: " + QString::number(i_iDefaultDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "getTraceAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    QString strParentBranchPath = buildPathStr(m_pTrcAdminObjIdxTree->nodeSeparator(), i_strNameSpace, i_strClassName);

    CIdxTreeEntry* pLeave = m_pTrcAdminObjIdxTree->findLeave(strParentBranchPath, i_strObjName);

    bool bInitiallyCreated = pLeave == nullptr;

    CTrcAdminObj* pTrcAdminObj = m_pTrcAdminObjIdxTree->getTraceAdminObj(i_strNameSpace, i_strClassName, i_strObjName);

    if( pTrcAdminObj != nullptr )
    {
        if( bInitiallyCreated )
        {
            EEnabled bEnabled     = m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault ? EEnabled::Yes : EEnabled::No;
            int      iDetailLevel = m_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel;

            if( i_bEnabledAsDefault != EEnabled::Undefined )
            {
                bEnabled = i_bEnabledAsDefault;
            }
            if( i_iDefaultDetailLevel >= 0 )
            {
                iDetailLevel = i_iDefaultDetailLevel;
            }

            pTrcAdminObj->setEnabled(bEnabled);
            pTrcAdminObj->setTraceDetailLevel(iDetailLevel);
        }

        if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthRet = pTrcAdminObj->getCalculatedKeyInTree();
            mthTracer.setMethodReturn(strMthRet);
        }
    }

    return pTrcAdminObj;

} // getTraceAdminObj

//------------------------------------------------------------------------------
void CTrcServer::releaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_pTrcAdminObj == nullptr ? "nullptr" : i_pTrcAdminObj->getCalculatedKeyInTree();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "releaseTraceAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    if( i_pTrcAdminObj != nullptr )
    {
        m_pTrcAdminObjIdxTree->releaseTraceAdminObj(i_pTrcAdminObj);
    }

} // releaseTraceAdminObj

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::traceMethodEnter(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strMethod,
    const QString&      i_strMethodInArgs )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isActive(ETraceDetailLevelMethodCalls) && isEnabled() && isActive() )
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

} // traceMethodEnter

//------------------------------------------------------------------------------
void CTrcServer::traceMethodEnter(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strObjName,
    const QString&      i_strMethod,
    const QString&      i_strMethodInArgs )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isActive(ETraceDetailLevelMethodCalls) && isEnabled() && isActive() )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::Enter,
            /* strNameSpace  */ i_pTrcAdminObj->getNameSpace(),
            /* strClassName  */ i_pTrcAdminObj->getClassName(),
            /* strObjName    */ i_strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strMethodInArgs );
    }

} // traceMethodEnter

//------------------------------------------------------------------------------
void CTrcServer::traceMethod(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strMethod,
    const QString&      i_strAddInfo )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // Send trace message to the trace server.
    // Using messages avoids using a mutex to protect the list of traced method
    // calls as "traceMethodEnter" is usually called from within different
    // thread contexts.

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isActive(ETraceDetailLevelRuntimeInfo) && isEnabled() && isActive() )
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

} // traceMethod

//------------------------------------------------------------------------------
void CTrcServer::traceMethod(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strObjName,
    const QString&      i_strMethod,
    const QString&      i_strAddInfo )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // Send trace message to the trace server.
    // Using messages avoids using a mutex to protect the list of traced method
    // calls as "traceMethodEnter" is usually called from within different
    // thread contexts.

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isActive(ETraceDetailLevelRuntimeInfo) && isEnabled() && isActive() )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::None,
            /* strNameSpace  */ i_pTrcAdminObj->getNameSpace(),
            /* strClassName  */ i_pTrcAdminObj->getClassName(),
            /* strObjName    */ i_strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strAddInfo );
    }

} // traceMethod

//------------------------------------------------------------------------------
void CTrcServer::traceMethodLeave(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strMethod,
    const QString&      i_strMethodReturn,
    const QString&      i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // Send trace message to the trace server.
    // Using messages avoids using a mutex to protect the list of traced method
    // calls as "traceMethodEnter" is usually called from within different
    // thread contexts.

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isActive(ETraceDetailLevelMethodCalls) && isEnabled() && isActive() )
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

} // traceMethodLeave

//------------------------------------------------------------------------------
void CTrcServer::traceMethodLeave(
    const CTrcAdminObj* i_pTrcAdminObj,
    const QString&      i_strObjName,
    const QString&      i_strMethod,
    const QString&      i_strMethodReturn,
    const QString&      i_strMethodOutArgs )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // Send trace message to the trace server.
    // Using messages avoids using a mutex to protect the list of traced method
    // calls as "traceMethodEnter" is usually called from within different
    // thread contexts.

    if( i_pTrcAdminObj != nullptr && i_pTrcAdminObj->isActive(ETraceDetailLevelMethodCalls) && isEnabled() && isActive() )
    {
        addEntry(
            /* strThreadName */ currentThreadName(),
            /* dt            */ QDateTime::currentDateTime(),
            /* fSysTimeInSec */ Time::getProcTimeInSec(),
            /* mthDir        */ EMethodDir::Leave,
            /* strNameSpace  */ i_pTrcAdminObj->getNameSpace(),
            /* strClassName  */ i_pTrcAdminObj->getClassName(),
            /* strObjName    */ i_strObjName,
            /* strMethod     */ i_strMethod,
            /* strAddInfo    */ i_strMethodReturn,
            /* strMthOutArgs */ i_strMethodOutArgs );
    }

} // traceMethodLeave

/*==============================================================================
public: // overridables
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
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( isEnabled() && isActive() )
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

} // traceMethodEnter

//------------------------------------------------------------------------------
void CTrcServer::traceMethod(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strMethod,
    const QString& i_strAddInfo )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // Send trace message to the trace server.
    // Using messages avoids using a mutex to protect the list of traced method
    // calls as "traceMethodEnter" is usually called from within different
    // thread contexts.

    if( isEnabled() && isActive() )
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

} // traceMethod

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
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    // Send trace message to the trace server.
    // Using messages avoids using a mutex to protect the list of traced method
    // calls as "traceMethodEnter" is usually called from within different
    // thread contexts.

    if( isEnabled() && isActive() )
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

} // traceMethodLeave

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
bool CTrcServer::isActive() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    return isLocalTrcFileActive();
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( i_bEnabled != m_trcSettings.m_bEnabled )
    {
        m_trcSettings.m_bEnabled = i_bEnabled;

        emit traceSettingsChanged(this);
    }

} // setEnabled

//------------------------------------------------------------------------------
bool CTrcServer::isEnabled() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_bEnabled;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = bool2Str(i_bEnabled);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setNewTrcAdminObjsEnabledAsDefault",
        /* strMthInArgs       */ strMthInArgs );

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
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault;
}

//------------------------------------------------------------------------------
void CTrcServer::setNewTrcAdminObjsDefaultDetailLevel( int i_iDetailLevel )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = QString::number(i_iDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setNewTrcAdminObjsDefaultDetailLevel",
        /* strMthInArgs       */ strMthInArgs );

    if( m_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel != i_iDetailLevel )
    {
        m_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel = i_iDetailLevel;

        emit traceSettingsChanged(this);
    }

} // setNewTrcAdminObjsDefaultDetailLevel

//------------------------------------------------------------------------------
int CTrcServer::getNewTrcAdminObjsDefaultDetailLevel() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_strAdminObjFileAbsFilePath != i_strAbsFilePath )
    {
        m_trcSettings.m_strAdminObjFileAbsFilePath = i_strAbsFilePath;

        emit traceSettingsChanged(this);
    }

} // setAdminObjFileName

//------------------------------------------------------------------------------
QString CTrcServer::getAdminObjFileAbsoluteFilePath() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_strAdminObjFileAbsFilePath;
}

//------------------------------------------------------------------------------
SErrResultInfo CTrcServer::recallAdminObjs( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "AbsFilePath: " + i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "recallAdminObjs",
        /* strMthInArgs       */ strMthInArgs );

    if( !i_strAbsFilePath.isEmpty() && m_trcSettings.m_strAdminObjFileAbsFilePath != i_strAbsFilePath )
    {
        m_trcSettings.m_strAdminObjFileAbsFilePath = i_strAbsFilePath;

        emit traceSettingsChanged(this);
    }

    SErrResultInfo errResultInfo = m_pTrcAdminObjIdxTree->recall(m_trcSettings.m_strAdminObjFileAbsFilePath);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // recallAdminObjs

//------------------------------------------------------------------------------
SErrResultInfo CTrcServer::saveAdminObjs( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "AbsFilePath: " + i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ nameSpace(),
        /* strClassName       */ className(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "recallAdminObjs",
        /* strMthInArgs       */ strMthInArgs );

    if( !i_strAbsFilePath.isEmpty() && m_trcSettings.m_strAdminObjFileAbsFilePath != i_strAbsFilePath )
    {
        m_trcSettings.m_strAdminObjFileAbsFilePath = i_strAbsFilePath;
        emit traceSettingsChanged(this);
    }

    SErrResultInfo errResultInfo = m_pTrcAdminObjIdxTree->save(m_trcSettings.m_strAdminObjFileAbsFilePath);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(errResultInfo);
    }

    return errResultInfo;

} // saveAdminObjs

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setUseLocalTrcFile( bool i_bUse )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
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

} // setUseLocalTrcFile

//------------------------------------------------------------------------------
bool CTrcServer::isLocalTrcFileUsed() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_bUseLocalTrcFile;
}

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( (m_trcSettings.m_strLocalTrcFileAbsFilePath != i_strAbsFilePath)
     || (m_pTrcMthFile == nullptr && !m_trcSettings.m_strLocalTrcFileAbsFilePath.isEmpty()) )
    {
        m_trcSettings.m_strLocalTrcFileAbsFilePath = i_strAbsFilePath;

        if( m_trcSettings.m_strLocalTrcFileAbsFilePath.isEmpty() )
        {
            if( m_pTrcMthFile != nullptr )
            {
                CTrcMthFile::Free(m_pTrcMthFile);
                m_pTrcMthFile = nullptr;
            }
        }
        else // if( !m_trcSettings.m_strLocalTrcFileAbsFilePath.isEmpty() )
        {
            if( m_pTrcMthFile != nullptr && m_pTrcMthFile->absoluteFilePath() != m_trcSettings.m_strLocalTrcFileAbsFilePath )
            {
                CTrcMthFile::Free(m_pTrcMthFile);
                m_pTrcMthFile = nullptr;
            }
            if( m_pTrcMthFile == nullptr )
            {
                m_pTrcMthFile = CTrcMthFile::Alloc(m_trcSettings.m_strLocalTrcFileAbsFilePath);
            }
            m_pTrcMthFile->setAutoSaveInterval(m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms);
            m_pTrcMthFile->setSubFileCountMax(m_trcSettings.m_iLocalTrcFileSubFileCountMax);
            m_pTrcMthFile->setSubFileLineCountMax(m_trcSettings.m_iLocalTrcFileSubFileLineCountMax);
            m_pTrcMthFile->setCloseFileAfterEachWrite(m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite);

        } // if( !m_trcSettings.m_strLocalTrcFileAbsFilePath.isEmpty() )

        emit traceSettingsChanged(this);

    } // if( (m_trcSettings.m_strLocalTrcFileAbsFilePath != i_strAbsFilePath) ..

} // setLocalTrcFileAbsoluteFilePath

//------------------------------------------------------------------------------
QString CTrcServer::getLocalTrcFileAbsoluteFilePath() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_strLocalTrcFileAbsFilePath;
}

//------------------------------------------------------------------------------
QString CTrcServer::getLocalTrcFileCompleteBaseName() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strBaseName;

    if( m_pTrcMthFile != nullptr )
    {
        strBaseName = m_pTrcMthFile->completeBaseName();
    }
    return strBaseName;

} // getLocalTrcFileCompleteBaseName

//------------------------------------------------------------------------------
QString CTrcServer::getLocalTrcFileAbsolutePath() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    QString strAbsPath;

    if( m_pTrcMthFile != nullptr )
    {
        strAbsPath = m_pTrcMthFile->absolutePath();
    }
    return strAbsPath;

} // getLocalTrcFileAbsPath

//------------------------------------------------------------------------------
bool CTrcServer::isLocalTrcFileActive() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return (m_trcSettings.m_bUseLocalTrcFile && m_pTrcMthFile != nullptr);
}

//------------------------------------------------------------------------------
CTrcMthFile* CTrcServer::getLocalTrcFile()
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_pTrcMthFile;
}

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileAutoSaveInterval( int i_iAutoSaveInterval_ms )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
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

} // setLocalTrcFileAutoSaveInterval

//------------------------------------------------------------------------------
int CTrcServer::getLocalTrcFileAutoSaveInterval() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms;
}

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
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

} // setLocalTrcFileCloseFileAfterEachWrite

//------------------------------------------------------------------------------
bool CTrcServer::getLocalTrcFileCloseFileAfterEachWrite() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileSubFileCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
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

} // setLocalTrcFileSubFileCountMax

//------------------------------------------------------------------------------
int CTrcServer::getLocalTrcFileSubFileCountMax() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_iLocalTrcFileSubFileCountMax;
}

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
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

} // setLocalTrcFileSubFileLineCountMax

//------------------------------------------------------------------------------
int CTrcServer::getLocalTrcFileSubFileLineCountMax() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_iLocalTrcFileSubFileLineCountMax;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setCacheTrcDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_bCacheDataIfNotConnected != i_bCacheData )
    {
        m_trcSettings.m_bCacheDataIfNotConnected = i_bCacheData;

        emit traceSettingsChanged(this);
    }

} // setCacheTrcDataIfNotConnected

//------------------------------------------------------------------------------
bool CTrcServer::getCacheTrcDataIfNotConnected() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings.m_bCacheDataIfNotConnected;
}

//------------------------------------------------------------------------------
void CTrcServer::setCacheTrcDataMaxArrLen( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings.m_iCacheDataMaxArrLen != i_iMaxArrLen )
    {
        m_trcSettings.m_iCacheDataMaxArrLen = i_iMaxArrLen;

        emit traceSettingsChanged(this);
    }

} // setCacheTrcDataIfNotConnected

//------------------------------------------------------------------------------
int CTrcServer::getCacheTrcDataMaxArrLen() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
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
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    if( m_trcSettings != i_settings )
    {
        if( m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault != i_settings.m_bNewTrcAdminObjsEnabledAsDefault )
        {
            m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault = i_settings.m_bNewTrcAdminObjsEnabledAsDefault;
        }
        if( m_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel != i_settings.m_iNewTrcAdminObjsDefaultDetailLevel )
        {
            m_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel = i_settings.m_iNewTrcAdminObjsDefaultDetailLevel;
        }

        if( m_trcSettings.m_bCacheDataIfNotConnected != i_settings.m_bCacheDataIfNotConnected )
        {
            m_trcSettings.m_bCacheDataIfNotConnected = i_settings.m_bCacheDataIfNotConnected;
        }
        if( m_trcSettings.m_iCacheDataMaxArrLen != i_settings.m_iCacheDataMaxArrLen )
        {
            m_trcSettings.m_iCacheDataMaxArrLen = i_settings.m_iCacheDataMaxArrLen;
        }

        if( m_trcSettings.m_strAdminObjFileAbsFilePath != i_settings.m_strAdminObjFileAbsFilePath )
        {
            setAdminObjFileAbsoluteFilePath(i_settings.m_strAdminObjFileAbsFilePath);
        }

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

        if( m_trcSettings.m_bUseLocalTrcFile != i_settings.m_bUseLocalTrcFile )
        {
            m_trcSettings.m_bUseLocalTrcFile = i_settings.m_bUseLocalTrcFile;
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

        if( m_trcSettings.m_bEnabled != i_settings.m_bEnabled )
        {
            // Will take over the enabled state into the settings struct and saves the current settings:
            setEnabled(i_settings.m_bEnabled);
        }

        emit traceSettingsChanged(this);
    }

} // setTraceSettings

//------------------------------------------------------------------------------
STrcServerSettings CTrcServer::getTraceSettings() const
//------------------------------------------------------------------------------
{
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);
    return m_trcSettings;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::clearLocalTrcFile()
//------------------------------------------------------------------------------
{
    CTrcMthFile::Free(m_pTrcMthFile);

    m_pTrcMthFile = CTrcMthFile::Alloc(m_trcSettings.m_strLocalTrcFileAbsFilePath);

    m_pTrcMthFile->setAutoSaveInterval(m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms);
    m_pTrcMthFile->setSubFileCountMax(m_trcSettings.m_iLocalTrcFileSubFileCountMax);
    m_pTrcMthFile->setSubFileLineCountMax(m_trcSettings.m_iLocalTrcFileSubFileLineCountMax);
    m_pTrcMthFile->setCloseFileAfterEachWrite(m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite);

} // clearLocalTrcFile

/*==============================================================================
protected: // overridables
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
    // The class (and all instances of the class) may be accessed from within
    // different thread contexts and therefore accessing the class and the
    // instances must be serialized using a mutex ..
    QMutexLocker mtxLocker(&s_mtx);

    //if( strMthArgs.contains('<') )
    //{
    //    strMthArgs.replace("<","&lt;");
    //}
    //if( strMthArgs.contains('>') )
    //{
    //    strMthArgs.replace(">","&gt;");
    //}
    //if( strMthArgs.contains('"') )
    //{
    //    strMthArgs.replace("\"","&quot;");
    //}
    //if( strMthArgs.contains("'") )
    //{
    //    strMthArgs.replace("'","&apos;");
    //}

    if( m_trcSettings.m_bUseLocalTrcFile && m_pTrcMthFile != nullptr )
    {
        QString strMth;

        if( m_pTrcAdminObjIdxTree != nullptr )
        {
            strMth = "<" + m_pTrcAdminObjIdxTree->buildPathStr(i_strNameSpace, i_strClassName) + "> " + i_strObjName + "." + i_strMethod;
        }
        else
        {
            strMth = "<" + buildPathStr("::", i_strNameSpace, i_strClassName) + "> " + i_strObjName + "." + i_strMethod;
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
/*! Returns the number of actove references to this trace server.
    If the count reaches 0 the instance has to be deleted.

    /return Number of active references.
*/
//------------------------------------------------------------------------------
int CTrcServer::getRefCount() const
{
    return m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Increments the number of active reference to this trace server.

    /return Number of active references after increment.
*/
//------------------------------------------------------------------------------
int CTrcServer::incrementRefCount()
{
    return ++m_iRefCount;
}

//------------------------------------------------------------------------------
/*! Decrements the number of active reference to this trace server.
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
