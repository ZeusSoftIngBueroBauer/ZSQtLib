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
#include "ZSSys/ZSSysMutex.h"
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
/*! @brief Creates a server settings structure.

    @param i_bEnabled [in] Default: true
    @param i_bNewTrcAdminObjsEnabledAsDefault [in] Default: true
    @param i_iNewTrcAdminObjsDefaultDetailLevel [in] Default: 0
    @param i_bCacheDataIfNotConnected [in] Default: false
    @param i_iCacheDataMaxArrLen [in] Default: 1000
    @param i_bUseLocalTrcFile [in] Default: true
    @param i_iLocalTrcFileAutoSaveInterval_ms [in] Default: 1000
    @param i_iLocalTrcFileSubFileCountMax [in] Default: 5
    @param i_iLocalTrcFileSubFileLineCountMax [in] Default: 2000
    @param i_bLocalTrcFileCloseFileAfterEachWrite [in] Default: false
*/
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
    m_strAdminObjFileAbsFilePath(),
    m_bNewTrcAdminObjsEnabledAsDefault(i_bNewTrcAdminObjsActivatedAsDefault),
    m_iNewTrcAdminObjsDefaultDetailLevel(i_iNewTrcAdminObjsDefaultDetailLevel),
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
    str += ", AdmObjDefLevel: " + QString::number(m_iNewTrcAdminObjsDefaultDetailLevel);
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
CTrcServer* CTrcServer::s_pTheInst;
QHash<Qt::HANDLE, QString> CTrcServer::s_hshThreadNames;
QHash<QString, Qt::HANDLE> CTrcServer::s_hshThreadIds;

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
/*! Creates the trace server if the trace server is not already existing.

    If the trace server is already existing the reference to the existing
    trace server is returned and a reference counter is incremented.

    \param i_iTrcDetailLevel [in]
        If the methods of the trace server itself should be logged a value
        greater than 0 (ETraceDetailLevelNone) could be passed here.

    \return Pointer to trace server instance.
*/
CTrcServer* CTrcServer::CreateInstance( int i_iTrcDetailLevel )
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
        new CTrcServer(i_iTrcDetailLevel);
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
        throw CException(__FILE__, __LINE__, EResultSingletonClassNotInstantiated, "ZS::Trace::CTrcServer");
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
void CTrcServer::RegisterCurrentThread( const QString& i_strThreadName )
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
void CTrcServer::UnregisterCurrentThread()
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

} // UnregisterCurrentThread

//------------------------------------------------------------------------------
QString CTrcServer::GetCurrentThreadName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    return currentThreadName();

} // UnregisterCurrentThread

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
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

} // GetTraceAdminObjIdxTree

//------------------------------------------------------------------------------
CTrcAdminObj* CTrcServer::GetTraceAdminObj( int i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    CTrcServer* pTrcServer = GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcAdminObj = pTrcServer->getTraceAdminObj(
            /* idxInTree           */ i_idxInTree,
            /* bEnabledAsDefault   */ EEnabled::Undefined,
            /* iDefaultDetailLevel */ -1 );
    }
    return pTrcAdminObj;

} // GetTraceAdminObj

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
    int                  i_iDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&s_mtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    CTrcServer* pTrcServer = GetInstance();

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
void CTrcServer::ReleaseTraceAdminObj( CTrcAdminObj*  i_pTrcAdminObj )
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
QString CTrcServer::GetDefaultAdminObjFileAbsoluteFilePath( const QString& i_strIniFileScope )
//------------------------------------------------------------------------------
{
    QString strAppConfigDir = ZS::System::getAppConfigDir(i_strIniFileScope);

    QString strTrcAdminObjFileSuffix = "xml";
    QString strTrcAdminObjFileBaseName = "ZSTrcServer-TrcMthAdmObj";

    return strAppConfigDir + "/" + strTrcAdminObjFileBaseName + "." + strTrcAdminObjFileSuffix;
}

//------------------------------------------------------------------------------
QString CTrcServer::GetDefaultLocalTrcFileAbsoluteFilePath( const QString& i_strIniFileScope )
//------------------------------------------------------------------------------
{
    QString strAppLogDir = ZS::System::getAppLogDir(i_strIniFileScope);

    QString strTrcLogFileSuffix = "log";
    QString strTrcLogFileBaseName = "ZSTrcServer-TrcMth";

    return strAppLogDir + "/" + strTrcLogFileBaseName + "." + strTrcLogFileSuffix;
}

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTrcServer::CTrcServer( int i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    QObject(),
    m_pMtx(nullptr),
    m_pTrcAdminObjIdxTree(nullptr),
    m_trcSettings(),
    m_pTrcMthFile(nullptr),
    m_iTrcDetailLevel(i_iTrcDetailLevel),
    m_iRefCount(0)
{
    setObjectName("ZSTrcServer");

    m_trcSettings.m_strAdminObjFileAbsFilePath = GetDefaultAdminObjFileAbsoluteFilePath("System");
    m_trcSettings.m_strLocalTrcFileAbsFilePath = GetDefaultLocalTrcFileAbsoluteFilePath("System");

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

    m_pMtx = new CMutex(QMutex::Recursive, "ZS::Trace::CTrcServer", i_iTrcDetailLevel),

    // Create index tree of trace admin objects. Pass the server as the parent object.
    // If the parent object is the trace server the index tree will not create a trace
    // admin object to trace the method calls.
    m_pTrcAdminObjIdxTree = new CIdxTreeTrcAdminObjs("ZSTrcServer", this, i_iTrcDetailLevel);

    // See comment in "CreateInstance" above.
    s_pTheInst = this;

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

    try
    {
        delete m_pMtx;
    }
    catch(...)
    {
    }

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
    CMutexLocker mtxLocker(m_pMtx);
    return m_pTrcAdminObjIdxTree;
}

/*==============================================================================
public: // instance methods to add, remove and modify admin objects
==============================================================================*/

//------------------------------------------------------------------------------
CTrcAdminObj* CTrcServer::getTraceAdminObj(
    int      i_idxInTree,
    EEnabled i_bEnabledAsDefault,
    int      i_iDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthRet;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "IdxInTree: " + QString::number(i_idxInTree);
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

    CMutexLocker mtxLocker(m_pMtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    if( i_idxInTree < 0 )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "getTraceAdminObj",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "Idx In Tree (=" + QString::number(i_idxInTree) + ") is out of range");

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else // if( !i_strObjName.isEmpty() || !i_strClassName.isEmpty() || !i_strNameSpace.isEmpty() )
    {
        pTrcAdminObj = m_pTrcAdminObjIdxTree->getTraceAdminObj(i_idxInTree);

        if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
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
CTrcAdminObj* CTrcServer::getTraceAdminObj(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    EEnabled       i_bEnabledAsDefault,
    int            i_iDefaultDetailLevel )
//------------------------------------------------------------------------------
{
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

    CMutexLocker mtxLocker(m_pMtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    if( i_strObjName.isEmpty() && i_strClassName.isEmpty() && i_strNameSpace.isEmpty() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "getTraceAdminObj",
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

        pTrcAdminObj = m_pTrcAdminObjIdxTree->getTraceAdminObj(
            /* strNameSpace        */ i_strNameSpace,
            /* strClassName        */ i_strClassName,
            /* strObjName          */ i_strObjName,
            /* bEnabledAsDefault   */ bEnabled,
            /* iDefaultDetailLevel */ iDetailLevel,
            /* bIncrementRefCount  */ true );

        if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
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
void CTrcServer::releaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
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

    CMutexLocker mtxLocker(m_pMtx);

    if( i_pTrcAdminObj != nullptr )
    {
        m_pTrcAdminObjIdxTree->releaseTraceAdminObj(i_pTrcAdminObj);
    }

} // releaseTraceAdminObj

/*==============================================================================
public: // overridables
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
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pTrcAdminObj != nullptr && (i_pTrcAdminObj->getTraceDetailLevel() > ETraceDetailLevelNone) && isEnabled() && isActive() )
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
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pTrcAdminObj != nullptr && (i_pTrcAdminObj->getTraceDetailLevel() > ETraceDetailLevelNone) && isEnabled() && isActive() )
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
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pTrcAdminObj != nullptr && (i_pTrcAdminObj->getTraceDetailLevel() > ETraceDetailLevelNone) && isEnabled() && isActive() )
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
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pTrcAdminObj != nullptr && (i_pTrcAdminObj->getTraceDetailLevel() > ETraceDetailLevelNone) && isEnabled() && isActive() )
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
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pTrcAdminObj != nullptr && (i_pTrcAdminObj->getTraceDetailLevel() > ETraceDetailLevelNone) && isEnabled() && isActive() )
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
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pTrcAdminObj != nullptr && (i_pTrcAdminObj->getTraceDetailLevel() > ETraceDetailLevelNone) && isEnabled() && isActive() )
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
    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);
    return isLocalTrcFileActive();
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

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

} // setEnabled

//------------------------------------------------------------------------------
bool CTrcServer::isEnabled() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings.m_bEnabled;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled )
//------------------------------------------------------------------------------
{
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

    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings.m_bNewTrcAdminObjsEnabledAsDefault;
}

//------------------------------------------------------------------------------
void CTrcServer::setNewTrcAdminObjsDefaultDetailLevel( int i_iDetailLevel )
//------------------------------------------------------------------------------
{
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

    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setAdminObjFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings.m_strAdminObjFileAbsFilePath;
}

//------------------------------------------------------------------------------
SErrResultInfo CTrcServer::recallAdminObjs( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
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

    CMutexLocker mtxLocker(m_pMtx);

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

    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings.m_bUseLocalTrcFile;
}

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileAbsoluteFilePath( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    if( m_trcSettings.m_strLocalTrcFileAbsFilePath != i_strAbsFilePath )
    {
        m_trcSettings.m_strLocalTrcFileAbsFilePath = i_strAbsFilePath;

        if( m_pTrcMthFile != nullptr )
        {
            m_pTrcMthFile->setAbsoluteFilePath(i_strAbsFilePath);
        }

        emit traceSettingsChanged(this);
    }
} // setLocalTrcFileAbsoluteFilePath

//------------------------------------------------------------------------------
QString CTrcServer::getLocalTrcFileAbsoluteFilePath() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings.m_strLocalTrcFileAbsFilePath;
}

//------------------------------------------------------------------------------
QString CTrcServer::getLocalTrcFileCompleteBaseName() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);
    return (m_trcSettings.m_bUseLocalTrcFile && m_pTrcMthFile != nullptr);
}

//------------------------------------------------------------------------------
CTrcMthFile* CTrcServer::getLocalTrcFile()
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_pTrcMthFile;
}

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    if( m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms != i_iAutoSaveInterval_ms )
    {
        m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms = i_iAutoSaveInterval_ms;

        if( m_pTrcMthFile != nullptr )
        {
            m_pTrcMthFile->setAutoSaveInterval(m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms);
        }
        emit traceSettingsChanged(this);
    }

} // setLocalTrcFileAutoSaveIntervalInMs

//------------------------------------------------------------------------------
int CTrcServer::getLocalTrcFileAutoSaveIntervalInMs() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings.m_iLocalTrcFileAutoSaveInterval_ms;
}

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileSubFileCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings.m_iLocalTrcFileSubFileCountMax;
}

//------------------------------------------------------------------------------
void CTrcServer::setLocalTrcFileSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings.m_iLocalTrcFileSubFileLineCountMax;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setCacheTrcDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings.m_bCacheDataIfNotConnected;
}

//------------------------------------------------------------------------------
void CTrcServer::setCacheTrcDataMaxArrLen( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

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
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings.m_iCacheDataMaxArrLen;
}

/*==============================================================================
public: // instance methods (trace settings)
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::setTraceSettings( const STrcServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

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
            m_trcSettings.m_bEnabled = i_settings.m_bEnabled;
        }

        emit traceSettingsChanged(this);
    }

} // setTraceSettings

//------------------------------------------------------------------------------
STrcServerSettings CTrcServer::getTraceSettings() const
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);
    return m_trcSettings;
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTrcServer::clearLocalTrcFile()
//------------------------------------------------------------------------------
{
    if( m_pTrcMthFile != nullptr )
    {
        m_pTrcMthFile->clear();
    }
}

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
    CMutexLocker mtxLocker(m_pMtx);

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
