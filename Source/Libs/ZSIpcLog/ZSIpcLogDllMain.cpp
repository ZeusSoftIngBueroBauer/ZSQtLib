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
#include <QtCore/qthread.h>

#include "ZSIpcLog/ZSIpcLogDllMain.h"
#include "ZSIpcLog/ZSIpcLogDllIf.h"
#include "ZSIpcLog/ZSIpcLogDllIfServerThread.h"
#include "ZSIpcLog/ZSIpcLogServer.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysLogger.h"
#include "ZSSys/ZSSysLoggerIdxTree.h"
#include "ZSSys/ZSSysRequest.h"
#include "ZSSys/ZSSysVersion.h"

#ifdef _WIN32
// As "min" will be defined as a macro with two arguments and qdatetime uses "min"
// as a function with no arguments "windows.h" must be included after qdatetime
// (which is included by ZSSysTime (which again is included by ZSSysAux)).
#include <windows.h>
#endif
#ifdef __linux__
#include <unistd.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Log;

using namespace std;

namespace ZS
{
namespace Log
{
namespace DllIf
{
class CIpcLogServerThread;

} // namespace DllIf
} // namespace Log
} // namespace ZS


/*******************************************************************************
public Dll interface methods
*******************************************************************************/

#ifdef _WINDOWS
static int s_iDLL_PROCESS_ATTACH = 0;
static int s_iDLL_THREAD_ATTACH = 0;
#endif

static QMutex DllIf_s_mtx;

static QCoreApplication* DllIf_s_pQtAppCreatedByDllIf = nullptr;

static DllIf::CIpcLogServer* DllIf_IpcLogServer_s_pTheInst = nullptr;
static int DllIf_IpcLogServer_s_iLogServerRefCount = 0;
static DllIf::CIpcLogServerThread* DllIf_s_pIpcLogServerThread = nullptr;

const QString c_strNameSpace = "ZS::Log::DllIf";
const QString c_strClassName = "DllMain";

/*==============================================================================
DllMain::CLogger
==============================================================================*/

namespace ZS
{
namespace Log
{
namespace DllIf
{
namespace DllMain
{
class CLogger : public DllIf::CLogger
{
public: // ctors and dtor
    CLogger( const char* i_szKeyInTree ) :
        DllIf::CLogger(),
        m_szKeyInTree(nullptr)
    {
        m_pMtx = new QMutex(QMutex::Recursive);

        size_t iStrLen = strlen(i_szKeyInTree);
        m_szKeyInTree = new char[iStrLen+1];
        memset(m_szKeyInTree, 0x00, iStrLen+1);
        memcpy(m_szKeyInTree, i_szKeyInTree, iStrLen);
    }
    virtual ~CLogger()
    {
        delete m_pMtx;
        m_pMtx = nullptr;
        delete m_szKeyInTree;
        m_szKeyInTree = nullptr;
    }
public: // instance methods
    virtual const char* keyInTree() const override
    {
        QMutexLocker mtxLocker(m_pMtx);
        return m_szKeyInTree;
    }
private: // instance members
    QMutex* m_pMtx;
    char*   m_szKeyInTree;
};
} // namespace DllMain
} // namespace DllIf
} // namespace Log
} // namespace ZS


/*==============================================================================
DllIf::CLogger
==============================================================================*/

static QHash<QString, DllIf::DllMain::CLogger*> DllIf_LogServer_s_hshpLoggers;

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void Logger_log(
    DllIf::CLogger* i_pLogger,
    ELogDetailLevel i_eFilterDetailLevel,
    const char*     i_szLogEntry )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                pLogger->log(i_eFilterDetailLevel, i_szLogEntry);
            }
        }
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void Logger_setEnabled(
    DllIf::CLogger* i_pLogger,
    bool            i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                pLogger->setEnabled(i_bEnabled ? EEnabled::Yes : EEnabled::No);
            }
        }
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool Logger_isEnabled( const DllIf::CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bEnabled = false;
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                bEnabled = pLogger->isEnabled();
            }
        }
    }
    return bEnabled;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void Logger_setLogLevel(
    DllIf::CLogger*        i_pLogger,
    DllIf::ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                pLogger->setLogLevel(static_cast<ELogDetailLevel>(i_eDetailLevel));
            }
        }
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API DllIf::ELogDetailLevel Logger_getLogLevel( const DllIf::CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    DllIf::ELogDetailLevel eDetailLevel = DllIf::ELogDetailLevelNone;
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                eDetailLevel = static_cast<DllIf::ELogDetailLevel>(pLogger->getLogLevel());
            }
        }
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool Logger_isActive(
    const DllIf::CLogger*  i_pLogger,
    DllIf::ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bIsActive = false;
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                bIsActive = pLogger->isActive(static_cast<ELogDetailLevel>(i_eDetailLevel));
            }
        }
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void Logger_setDataFilter(
    DllIf::CLogger* i_pLogger,
    const char*     i_szFilter )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                pLogger->setDataFilter(i_szFilter);
            }
        }
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* Logger_getDataFilter( const DllIf::CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    char* szFilter = nullptr;
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                std::string stdstrFilter = pLogger->getDataFilter().toStdString();
                szFilter = new char[stdstrFilter.size() + 1];
                memcpy(szFilter, stdstrFilter.c_str(), stdstrFilter.size());
                szFilter[stdstrFilter.size()] = 0x00;
            }
        }
    }
    return szFilter;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool Logger_isSuppressedByDataFilter(
    DllIf::CLogger* i_pLogger,
    const char*     i_szLogEntry )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bIsSuppressed = false;
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                bIsSuppressed = pLogger->isSuppressedByDataFilter(i_szLogEntry);
            }
        }
    }
    return bIsSuppressed;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void Logger_setAddThreadName( const DllIf::CLogger* i_pLogger, bool i_bAdd )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                pLogger->setAddThreadName(i_bAdd);
            }
        }
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool Logger_addThreadName( const DllIf::CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bAdd = false;
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                bAdd = pLogger->addThreadName();
            }
        }
    }
    return bAdd;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void Logger_setAddDateTime( const DllIf::CLogger* i_pLogger, bool i_bAdd )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                pLogger->setAddDateTime(i_bAdd);
            }
        }
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool Logger_addDateTime( const DllIf::CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bAdd = false;
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                bAdd = pLogger->addDateTime();
            }
        }
    }
    return bAdd;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void Logger_setAddSystemTime( const DllIf::CLogger* i_pLogger, bool i_bAdd )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                pLogger->setAddSystemTime(i_bAdd);
            }
        }
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool Logger_addSystemTime( const DllIf::CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bAdd = false;
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                bAdd = pLogger->addSystemTime();
            }
        }
    }
    return bAdd;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void Logger_setNameSpace(
    const DllIf::CLogger* i_pLogger,
    const char*           i_szName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                pLogger->setNameSpace(i_szName);
            }
        }
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* Logger_getNameSpace( const DllIf::CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    char* szName = nullptr;
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                std::string stdstrName = pLogger->getNameSpace().toStdString();
                szName = new char[stdstrName.size() + 1];
                memcpy(szName, stdstrName.c_str(), stdstrName.size());
                szName[stdstrName.size()] = 0x00;
            }
        }
    }
    return szName;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void Logger_setClassName(
    const DllIf::CLogger* i_pLogger,
    const char*           i_szName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                pLogger->setClassName(i_szName);
            }
        }
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* Logger_getClassName( const DllIf::CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    char* szName = nullptr;
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                std::string stdstrName = pLogger->getClassName().toStdString();
                szName = new char[stdstrName.size() + 1];
                memcpy(szName, stdstrName.c_str(), stdstrName.size());
                szName[stdstrName.size()] = 0x00;
            }
        }
    }
    return szName;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void Logger_setObjectName(
    const DllIf::CLogger* i_pLogger,
    const char*           i_szName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                pLogger->setObjectName(i_szName);
            }
        }
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* Logger_getObjectName( const DllIf::CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    char* szName = nullptr;
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                std::string stdstrName = pLogger->getObjectName().toStdString();
                szName = new char[stdstrName.size() + 1];
                memcpy(szName, stdstrName.c_str(), stdstrName.size());
                szName[stdstrName.size()] = 0x00;
            }
        }
    }
    return szName;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* Logger_name( const DllIf::CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    char* szName = nullptr;
    if( i_pLogger != nullptr )
    {
        QString strKeyInTree = i_pLogger->keyInTree();
        CLogServer* pLogServer = CLogServer::GetInstance();
        if( pLogServer != nullptr )
        {
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);
            if( pLogger != nullptr )
            {
                std::string stdstrName = pLogger->path().toStdString();
                szName = new char[stdstrName.size() + 1];
                memcpy(szName, stdstrName.c_str(), stdstrName.size());
                szName[stdstrName.size()] = 0x00;
            }
        }
    }
    return szName;
}

/*==============================================================================
DllIf::CLogServer
==============================================================================*/

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_SetOrganizationName( const char* i_szName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    QCoreApplication::setOrganizationName(i_szName);
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* LogServer_GetOrganizationName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrName = QCoreApplication::organizationName().toStdString();
    char* szName = new char[stdstrName.size() + 1];
    memcpy(szName, stdstrName.c_str(), stdstrName.size());
    szName[stdstrName.size()] = 0x00;
    return szName;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_SetApplicationName( const char* i_szName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    QCoreApplication::setApplicationName(i_szName);
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* LogServer_GetApplicationName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrName = QCoreApplication::applicationName().toStdString();
    char* szName = new char[stdstrName.size() + 1];
    memcpy(szName, stdstrName.c_str(), stdstrName.size());
    szName[stdstrName.size()] = 0x00;
    return szName;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_SetLoggerFileAbsoluteFilePath( const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer::SetLoggerFileAbsoluteFilePath(i_szAbsFilePath);
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* LogServer_GetLoggerFileAbsoluteFilePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrAbsFilePath = CLogServer::GetLoggerFileAbsoluteFilePath().toStdString();
    char* szAbsFilePath = new char[stdstrAbsFilePath.size() + 1];
    memcpy(szAbsFilePath, stdstrAbsFilePath.c_str(), stdstrAbsFilePath.size());
    szAbsFilePath[stdstrAbsFilePath.size()] = 0x00;
    return szAbsFilePath;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* LogServer_GetLoggerFileCompleteBaseName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrBaseName = CLogServer::GetLoggerFileCompleteBaseName().toStdString();
    char* szBaseName = new char[stdstrBaseName.size() + 1];
    memcpy(szBaseName, stdstrBaseName.c_str(), stdstrBaseName.size());
    szBaseName[stdstrBaseName.size()] = 0x00;
    return szBaseName;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* LogServer_GetLoggerFileAbsolutePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrAbsPath = CLogServer::GetLoggerFileAbsolutePath().toStdString();
    char* szAbsPath = new char[stdstrAbsPath.size() + 1];
    memcpy(szAbsPath, stdstrAbsPath.c_str(), stdstrAbsPath.size());
    szAbsPath[stdstrAbsPath.size()] = 0x00;
    return szAbsPath;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_SetLocalLogFileAbsoluteFilePath( const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer::SetLocalLogFileAbsoluteFilePath(i_szAbsFilePath);
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* LogServer_GetLocalLogFileAbsoluteFilePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrAbsFilePath = CLogServer::GetLocalLogFileAbsoluteFilePath().toStdString();
    char* szAbsFilePath = new char[stdstrAbsFilePath.size() + 1];
    memcpy(szAbsFilePath, stdstrAbsFilePath.c_str(), stdstrAbsFilePath.size());
    szAbsFilePath[stdstrAbsFilePath.size()] = 0x00;
    return szAbsFilePath;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* LogServer_GetLocalLogFileCompleteBaseName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrBaseName = CLogServer::GetLocalLogFileCompleteBaseName().toStdString();
    char* szBaseName = new char[stdstrBaseName.size() + 1];
    memcpy(szBaseName, stdstrBaseName.c_str(), stdstrBaseName.size());
    szBaseName[stdstrBaseName.size()] = 0x00;
    return szBaseName;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* LogServer_GetLocalLogFileAbsolutePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrAbsPath = CLogServer::GetLocalLogFileAbsolutePath().toStdString();
    char* szAbsPath = new char[stdstrAbsPath.size() + 1];
    memcpy(szAbsPath, stdstrAbsPath.c_str(), stdstrAbsPath.size());
    szAbsPath[stdstrAbsPath.size()] = 0x00;
    return szAbsPath;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_RegisterCurrentThread( const char* i_szThreadName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer::RegisterCurrentThread(i_szThreadName);
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_UnregisterCurrentThread()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer::UnregisterCurrentThread();
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API char* LogServer_GetCurrentThreadName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    std::string stdstrName = CLogServer::GetCurrentThreadName().toStdString();
    char* szName = new char[stdstrName.size() + 1];
    memcpy(szName, stdstrName.c_str(), stdstrName.size());
    szName[stdstrName.size()] = 0x00;
    return szName;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API DllIf::CLogger* LogServer_GetLogger(
    const char*            i_szName,
    EEnabled               i_bEnabledAsDefault,
    DllIf::ELogDetailLevel i_eDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    if( s_iDLL_PROCESS_ATTACH <= 0 ) // Dll already unloaded
    {
        return NULL;
    }
    #endif

    QMutexLocker mtxLocker(&DllIf_s_mtx);

    DllIf::DllMain::CLogger* pDllIfLogger = nullptr;

    QString strName = i_szName;

    CLogServer* pLogServer = CLogServer::GetInstance();

    if( pLogServer != nullptr )
    {
        CLogger* pLogger = nullptr;

        if( strName.isEmpty() )
        {
            pLogger = CLogServer::GetLogger();
        }
        else
        {
            pLogger = CLogServer::GetLogger(
                strName, i_bEnabledAsDefault,
                static_cast<ELogDetailLevel>(i_eDefaultDetailLevel) );
        }
        if( pLogger != nullptr )
        {
            QString strKeyInTree = pLogger->keyInTree();

            pDllIfLogger = DllIf_LogServer_s_hshpLoggers.value(strKeyInTree, nullptr);

            if( pDllIfLogger == nullptr )
            {
                pDllIfLogger = new DllIf::DllMain::CLogger(strKeyInTree.toUtf8());
                DllIf_LogServer_s_hshpLoggers[strKeyInTree] = pDllIfLogger;
            }
        }
    }
    return pDllIfLogger;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_log(
    const DllIf::CLogger* i_pLogger,
    ELogDetailLevel       i_eFilterDetailLevel,
    const char*           i_szLogEntry )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        CLogger* pLogger = nullptr;
        if( i_pLogger != nullptr )
        {
            QString strKeyInTree = i_pLogger->keyInTree();
            CIdxTreeLoggers* pIdxTree = pLogServer->getLoggersIdxTree();
            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);
            pLogger = dynamic_cast<CLogger*>(pTreeEntry);
        }
        if( pLogger != nullptr )
        {
            pLogServer->log(pLogger, i_eFilterDetailLevel, i_szLogEntry);
        }
        else
        {
            pLogServer->log(i_eFilterDetailLevel, i_szLogEntry);
        }
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool LogServer_isActive()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bIsActive = false;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        bIsActive = pLogServer->isActive();
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->setEnabled(i_bEnabled);
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool LogServer_isEnabled()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bEnabled = false;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        bEnabled = pLogServer->isEnabled();
    }
    return bEnabled;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_setNewLoggersEnabledAsDefault( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->setNewLoggersEnabledAsDefault(i_bEnabled);
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool LogServer_areNewLoggersEnabledAsDefault()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bEnabled = false;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        bEnabled = pLogServer->areNewLoggersEnabledAsDefault();
    }
    return bEnabled;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_setNewLoggersDefaultDetailLevel( DllIf::ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->setNewLoggersDefaultDetailLevel(static_cast<ELogDetailLevel>(i_eDetailLevel));
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API DllIf::ELogDetailLevel LogServer_getNewLoggersDefaultDetailLevel()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    DllIf::ELogDetailLevel eDetailLevel = DllIf::ELogDetailLevelNone;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        eDetailLevel = static_cast<DllIf::ELogDetailLevel>(pLogServer->getNewLoggersDefaultDetailLevel());
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool LogServer_recallLoggers( const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bOk = false;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        SErrResultInfo errResultInfo = pLogServer->recallLoggers(i_szAbsFilePath);
        if( errResultInfo.isErrorResult() )
        {
            bOk = false;
        }
        else
        {
            bOk = true;
        }
    }
    return bOk;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool LogServer_saveLoggers( const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bOk = false;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        SErrResultInfo errResultInfo = pLogServer->saveLoggers(i_szAbsFilePath);

        if( errResultInfo.isErrorResult() )
        {
            bOk = false;
        }
        else
        {
            bOk = true;
        }
    }
    return bOk;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_setUseLocalLogFile( bool i_bUse )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->setUseLocalLogFile(i_bUse);
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool LogServer_isLocalLogFileUsed()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bUsed = false;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        bUsed = pLogServer->isLocalLogFileUsed();
    }
    return bUsed;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API int LogServer_isLocalLogFileActive()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bActive = false;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        bActive = pLogServer->isLocalLogFileActive();
    }
    return bActive;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_setLocalLogFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->setLocalLogFileAutoSaveIntervalInMs(i_iAutoSaveInterval_ms);
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API int LogServer_getLocalLogFileAutoSaveIntervalInMs()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    int iAutoSaveInterval_ms = 0;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        iAutoSaveInterval_ms = pLogServer->getLocalLogFileAutoSaveIntervalInMs();
    }
    return iAutoSaveInterval_ms;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_setLocalLogFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->setLocalLogFileCloseFileAfterEachWrite(i_bCloseFile);
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool LogServer_getLocalLogFileCloseFileAfterEachWrite()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bCloseFile = false;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        bCloseFile = pLogServer->getLocalLogFileCloseFileAfterEachWrite();
    }
    return bCloseFile;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_setLocalLogFileSubFileCountMax( bool i_iCountMax )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->setLocalLogFileSubFileCountMax(i_iCountMax);
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API int LogServer_getLocalLogFileSubFileCountMax()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    int iCountMax = 0;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        iCountMax = pLogServer->getLocalLogFileSubFileCountMax();
    }
    return iCountMax;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_setLocalLogFileSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->setLocalLogFileSubFileLineCountMax(i_iCountMax);
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API int LogServer_getLocalLogFileSubFileLineCountMax()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    int iCountMax = 0;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        iCountMax = pLogServer->getLocalLogFileSubFileLineCountMax();
    }
    return iCountMax;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_setUseIpcServer( bool i_bUseIpcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->setUseIpcServer(i_bUseIpcServer);
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool LogServer_isIpcServerUsed()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bUseIpcServer = false;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        bUseIpcServer = pLogServer->isIpcServerUsed();
    }
    return bUseIpcServer;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_setCacheLogDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->setCacheLogDataIfNotConnected(i_bCacheData);
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool LogServer_getCacheLogDataIfNotConnected()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bCacheData = false;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        bCacheData = pLogServer->getCacheLogDataIfNotConnected();
    }
    return bCacheData;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_setCacheLogDataMaxArrLen( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->setCacheLogDataMaxArrLen(i_iMaxArrLen);
    }
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API int LogServer_getCacheLogDataMaxArrLen()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    int iMaxArrLen = 0;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        iMaxArrLen = pLogServer->getCacheLogDataMaxArrLen();
    }
    return iMaxArrLen;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool LogServer_setLogSettings( const DllIf::SLogServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bOk = false;
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        ZS::System::SLogServerSettings logSettings;

        logSettings.m_bEnabled                              = i_settings.m_bEnabled;
        logSettings.m_strLoggerFileAbsFilePath              = i_settings.m_szLoggerFileAbsFilePath;
        logSettings.m_bNewLoggersEnabledAsDefault           = i_settings.m_bNewLoggersEnabledAsDefault;
        logSettings.m_eNewLoggersDefaultDetailLevel =
            static_cast<ELogDetailLevel>(i_settings.m_eNewLoggersDefaultDetailLevel);
        logSettings.m_bUseIpcServer                         = i_settings.m_bUseIpcServer;
        logSettings.m_bCacheDataIfNotConnected              = i_settings.m_bCacheDataIfNotConnected;
        logSettings.m_iCacheDataMaxArrLen                   = i_settings.m_iCacheDataMaxArrLen ;
        logSettings.m_bUseLocalLogFile                      = i_settings.m_bUseLocalLogFile;
        logSettings.m_strLocalLogFileAbsFilePath            = i_settings.m_szLocalLogFileAbsFilePath;
        logSettings.m_iLocalLogFileAutoSaveInterval_ms      = i_settings.m_iLocalLogFileAutoSaveInterval_ms;
        logSettings.m_iLocalLogFileSubFileCountMax          = i_settings.m_iLocalLogFileSubFileCountMax;
        logSettings.m_iLocalLogFileSubFileLineCountMax      = i_settings.m_iLocalLogFileSubFileLineCountMax;
        logSettings.m_bLocalLogFileCloseFileAfterEachWrite  = i_settings.m_bLocalLogFileCloseFileAfterEachWrite;

        pLogServer->setLogSettings(logSettings);

        bOk = true;
    }
    return bOk;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API DllIf::SLogServerSettings LogServer_getLogSettings()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    DllIf::SLogServerSettings dllIfTrcSettings;
    DllIf::SLogServerSettings_init(dllIfTrcSettings);
    CLogServer* pLogServer = CLogServer::GetInstance();

    if( pLogServer != nullptr )
    {
        int iStrLen;

        ZS::System::SLogServerSettings logSettings = pLogServer->getLogSettings();

        dllIfTrcSettings.m_bEnabled                              = logSettings.m_bEnabled;
        dllIfTrcSettings.m_bNewLoggersEnabledAsDefault           = logSettings.m_bNewLoggersEnabledAsDefault;
        dllIfTrcSettings.m_eNewLoggersDefaultDetailLevel         = static_cast<DllIf::ELogDetailLevel>(logSettings.m_eNewLoggersDefaultDetailLevel);
        dllIfTrcSettings.m_bUseIpcServer                         = logSettings.m_bUseIpcServer;
        dllIfTrcSettings.m_bCacheDataIfNotConnected              = logSettings.m_bCacheDataIfNotConnected;
        dllIfTrcSettings.m_iCacheDataMaxArrLen                   = logSettings.m_iCacheDataMaxArrLen;
        dllIfTrcSettings.m_bUseLocalLogFile                      = logSettings.m_bUseLocalLogFile;
        dllIfTrcSettings.m_iLocalLogFileAutoSaveInterval_ms      = logSettings.m_iLocalLogFileAutoSaveInterval_ms;
        dllIfTrcSettings.m_iLocalLogFileSubFileCountMax          = logSettings.m_iLocalLogFileSubFileCountMax;
        dllIfTrcSettings.m_iLocalLogFileSubFileLineCountMax      = logSettings.m_iLocalLogFileSubFileLineCountMax;
        dllIfTrcSettings.m_bLocalLogFileCloseFileAfterEachWrite  = logSettings.m_bLocalLogFileCloseFileAfterEachWrite;

        iStrLen = logSettings.m_strLoggerFileAbsFilePath.length();
        dllIfTrcSettings.m_szLoggerFileAbsFilePath = new char[iStrLen+1];
        memset(dllIfTrcSettings.m_szLoggerFileAbsFilePath, 0x00, iStrLen+1);
        memcpy(dllIfTrcSettings.m_szLoggerFileAbsFilePath, logSettings.m_strLoggerFileAbsFilePath.toUtf8().data(), iStrLen);

        iStrLen = logSettings.m_strLocalLogFileAbsFilePath.length();
        dllIfTrcSettings.m_szLocalLogFileAbsFilePath = new char[iStrLen+1];
        memset(dllIfTrcSettings.m_szLocalLogFileAbsFilePath, 0x00, iStrLen+1);
        memcpy(dllIfTrcSettings.m_szLocalLogFileAbsFilePath, logSettings.m_strLocalLogFileAbsFilePath.toUtf8().data(), iStrLen);
    }
    return dllIfTrcSettings;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void LogServer_clearLocalLogFile()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CLogServer* pLogServer = CLogServer::GetInstance();
    if( pLogServer != nullptr )
    {
        pLogServer->clearLocalLogFile();
    }
}

/*==============================================================================
DllIf::CIpcLogServer
==============================================================================*/

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API DllIf::CIpcLogServer* IpcLogServer_GetInstance()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    return DllIf_IpcLogServer_s_pTheInst;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API DllIf::CIpcLogServer* IpcLogServer_CreateInstance()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    QCoreApplication* pApp = dynamic_cast<QCoreApplication*>(QCoreApplication::instance());

    // If the application creating and starting the log server is not a Qt application
    // and if the Qt application has not yet been created by this module ...
    if( pApp == nullptr && DllIf_s_pQtAppCreatedByDllIf == nullptr )
    {
        int   argc = 1;
        const char* szAppName = "ZSIpcLogDllIf";
        char* argv[1] = { const_cast<char*>(szAppName) };

        pApp = DllIf_s_pQtAppCreatedByDllIf = new QCoreApplication(argc, argv);
    }

    if( QThread::currentThread() != nullptr && QThread::currentThread()->objectName().isEmpty() )
    {
        if( pApp != nullptr && pApp->thread() == QThread::currentThread() )
        {
            QThread::currentThread()->setObjectName("GuiMain");
        }
    }

    QString strLoggerFileAbsFilePath = CLogServer::GetLoggerFileAbsoluteFilePath();
    QString strLocalLogFileAbsFilePath = CLogServer::GetLocalLogFileAbsoluteFilePath();

    DllIf::CIpcLogServer* pDllIfLogServer = DllIf_IpcLogServer_s_pTheInst;

    int iRefCount = DllIf_IpcLogServer_s_iLogServerRefCount + 1;

    if( pDllIfLogServer != nullptr /* && iRefCount > 0 */ ) // RefCount mus be greater than 0. Otherwise ... crash.
    {
        CIpcLogServer::GetInstance();

        DllIf_IpcLogServer_s_iLogServerRefCount = iRefCount;
    }
    else // if( pDllIfLogServer == nullptr )
    {
        // No matter whether the application creating the log server is a Qt application
        // or not the log server created via the Dll interface (via this main dll module)
        // will be created and later on started from a separate thread.
        // If the application uses Qt this ensures that the server is hosted in a QThread
        // with an event loop. If the application does not use Qt it is anyway necessary
        // to create the log server in a thread with an event loop.
        DllIf::CIpcLogServerThread* pLogServerThread = DllIf_s_pIpcLogServerThread;
        if( pLogServerThread == nullptr )
        {
            DllIf_s_pIpcLogServerThread = new DllIf::CIpcLogServerThread();
        }
        pLogServerThread = DllIf_s_pIpcLogServerThread;

        const int c_iMaxWaitCount = 25;
        int iWaitCount = 0;

        // Start driver thread and wait until the driver has been created.
        if( !pLogServerThread->isRunning() )
        {
            pLogServerThread->start();

            iWaitCount = 0;
            while( !pLogServerThread->isServerCreated() )
            {
                #ifdef _WINDOWS
                Sleep(200);
                #endif
                #ifdef __linux__
                usleep(200000);
                #endif

                iWaitCount++;
                if( iWaitCount > c_iMaxWaitCount )
                {
                    break;
                }
            }
        }

        // Please note that the log server has been created (or at least the reference
        // counter for an existing log server has been incremented) in the thread.
        // Invoking "GetInstance" again here would increment the reference counter twice.
        pDllIfLogServer = new DllIf::CIpcLogServer();
        DllIf_IpcLogServer_s_pTheInst = pDllIfLogServer;
        DllIf_IpcLogServer_s_iLogServerRefCount = iRefCount;
    }
    return pDllIfLogServer;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API void IpcLogServer_ReleaseInstance( DllIf::CIpcLogServer* i_pLogServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pLogServer != nullptr )
    {
        int iLogServerRefCount = DllIf_IpcLogServer_s_iLogServerRefCount - 1;

        if( iLogServerRefCount > 0 /* && pDllIfLogServer != nullptr */ ) // pDllIfLogServer must not be nullptr. Otherwise ... crash.
        {
            CIpcLogServer::ReleaseInstance();

            DllIf_IpcLogServer_s_iLogServerRefCount = iLogServerRefCount;
        }
        // Log server is no longer referenced ...
        else // if( iLogServerRefCount == 0 )
        {
            QStringList strlstKeysInTree = DllIf_LogServer_s_hshpLoggers.keys();

            for( const QString& strKeyInTree : strlstKeysInTree )
            {
                DllIf::DllMain::CLogger* pDllIfLogger = DllIf_LogServer_s_hshpLoggers[strKeyInTree];
                DllIf_LogServer_s_hshpLoggers.remove(strKeyInTree);
                try
                {
                    delete pDllIfLogger;
                }
                catch(...)
                {
                }
                pDllIfLogger = nullptr;
            }

            // The log server was created and started from within a different thread context.
            DllIf::CIpcLogServerThread* pLogServerThread = DllIf_s_pIpcLogServerThread;

            pLogServerThread->quit();
            pLogServerThread->wait(30000);

            if( !pLogServerThread->isRunning() )
            {
                try
                {
                    delete pLogServerThread;
                }
                catch(...)
                {
                }
            }
            pLogServerThread = nullptr;
            DllIf_s_pIpcLogServerThread = nullptr;

            // Please note that the log server has been released in the thread.
            // Invoking "ReleaseInstance" again here would deincrement the reference counter twice.

            DllIf_IpcLogServer_s_pTheInst = nullptr;
            DllIf_IpcLogServer_s_iLogServerRefCount = 0;

            delete i_pLogServer;
            i_pLogServer = nullptr;

        } // if( iLogServerRefCount == 0 )

        if( iLogServerRefCount == 0 )
        {
        }
    } // if( i_pLogServer != nullptr )

    // If the application creating and starting the log server is not a Qt application ...
    if( DllIf_s_pQtAppCreatedByDllIf != NULL )
    {
        // Free Qt Application if all log servers referenced by the Dll interface have been released.
        if( DllIf_IpcLogServer_s_pTheInst == nullptr )
        {
            try
            {
                delete DllIf_s_pQtAppCreatedByDllIf;
            }
            catch(...)
            {
            }
            DllIf_s_pQtAppCreatedByDllIf = NULL;
        }
    }

} // IpcLogServer_ReleaseInstance

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool IpcLogServer_startup( DllIf::CIpcLogServer* i_pLogServer, int i_iTimeout_ms, bool i_bWait )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bOk = false;

    if( i_pLogServer != nullptr )
    {
        CIpcLogServer* pLogServer = CIpcLogServer::GetInstance();

        if( pLogServer != nullptr )
        {
            // No matter whether the application creating the log server is a Qt application
            // or not the log server created via the Dll interface (via this main dll module)
            // will be created and started from a separate thread.
            // The thread hosting the log server must start an event loop.

            CMsgReqStartup* pMsg = new CMsgReqStartup(
                /* pObjSender       */ nullptr,
                /* pObjReceiver     */ pLogServer,
                /* hostSettings     */ pLogServer->getHostSettings(),
                /* pBlkType         */ pLogServer->getBlkType(),  // The block will be cloned.
                /* bMustBeConfirmed */ false,
                /* iReqId           */ -1,
                /* iMsgId           */ -1 );
            QCoreApplication::postEvent(pLogServer, pMsg);
            pMsg = NULL;

            const int c_iMaxWaitCount = 25;
            int iWaitCount = 0;

            DllIf::CIpcLogServerThread* pLogServerThread = DllIf_s_pIpcLogServerThread;

            while( !pLogServerThread->isServerStarted() )
            {
                #ifdef _WIN32
                Sleep(200);
                #else
                usleep(200000);
                #endif

                iWaitCount++;
                if( iWaitCount > c_iMaxWaitCount )
                {
                    break;
                }
            }

            bOk = pLogServerThread->isServerStarted();

        } // if( pLogServer != nullptr )
    } // if( i_pLogServer != nullptr )

    return bOk;

} // IpcLogServer_startup

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool IpcLogServer_shutdown( DllIf::CIpcLogServer* i_pLogServer, int i_iTimeout_ms, bool i_bWait )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bOk = false;

    if( i_pLogServer != nullptr )
    {
        CIpcLogServer* pLogServer = CIpcLogServer::GetInstance();

        if( pLogServer != nullptr )
        {
            // No matter whether the application creating the log server is a Qt application
            // the log server is hosted in a different thread context and the server must be
            // shutdown in this thread context.
            CMsgReqShutdown* pMsg = new CMsgReqShutdown(
                /* pObjSender       */ nullptr,
                /* pObjReceiver     */ pLogServer,
                /* bMustBeConfirmed */ false,
                /* iReqId           */ -1,
                /* iMsgId           */ -1 );
            QCoreApplication::postEvent(pLogServer, pMsg);
            pMsg = NULL;

            const int c_iMaxWaitCount = 25;
            int iWaitCount = 0;

            DllIf::CIpcLogServerThread* pLogServerThread = DllIf_s_pIpcLogServerThread;

            if( pLogServerThread != NULL )
            {
                while( !pLogServerThread->isServerShutdown() )
                {
                    #ifdef _WIN32
                    Sleep(200);
                    #else
                    usleep(200000);
                    #endif

                    iWaitCount++;
                    if( iWaitCount > c_iMaxWaitCount )
                    {
                        break;
                    }
                }
            }
            bOk = pLogServerThread->isServerShutdown();

        } // if( pLogServer != nullptr )
    } // if( i_pLogServer != nullptr )

    return bOk;

} // IpcLogServer_shutdown

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool IpcLogServer_isListening( DllIf::CIpcLogServer* i_pLogServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bIsListening = false;

    if( i_pLogServer != nullptr )
    {
        CIpcLogServer* pLogServer = CIpcLogServer::GetInstance();

        if( pLogServer != nullptr )
        {
            bIsListening = pLogServer->isListening();
        }
    }
    return bIsListening;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool IpcLogServer_isConnected( DllIf::CIpcLogServer* i_pLogServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bIsConnected = false;

    if( i_pLogServer != nullptr )
    {
        CIpcLogServer* pLogServer = CIpcLogServer::GetInstance();

        if( pLogServer != nullptr )
        {
            bIsConnected = pLogServer->isConnected();
        }
    }
    return bIsConnected;
}

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API bool IpcLogServer_setPort(
    DllIf::CIpcLogServer* i_pLogServer,
    unsigned short        i_uPort,
    int                   i_iTimeout_ms,
    bool                  i_bWait )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bOk = false;

    if( i_pLogServer != nullptr )
    {
        CIpcLogServer* pLogServer = CIpcLogServer::GetInstance();

        if( pLogServer != nullptr )
        {
            ZS::Ipc::SServerHostSettings hostSettings;

            hostSettings.m_socketType = ZS::Ipc::ESocketTypeTcp;
            //hostSettings.m_pObjLocal = nullptr;
            //hostSettings.m_strLocalHostName;
            //hostSettings.m_hostAddrLocal;
            hostSettings.m_uLocalPort = i_uPort;
            hostSettings.m_uMaxPendingConnections = 30;
            //hostSettings.m_uBufferSize;

            // If the application creating and starting the log server is a Qt application ...
            if( DllIf_s_pQtAppCreatedByDllIf == nullptr )
            {
                pLogServer->setHostSettings(hostSettings);

                CRequest* pReq = pLogServer->changeSettings();

                if( isErrorResult(pReq) )
                {
                    bOk = false;
                }
                else if( isAsynchronousRequest(pReq) )
                {
                    bOk = false;
                }
                else
                {
                    bOk = true;
                }
            } // if( DllIf_s_pQtAppCreatedByDllIf == nullptr )

            // If the application creating and starting the log server is not a Qt application ...
            else // if( DllIf_s_pQtAppCreatedByDllIf != nullptr )
            {
                // ... the log server is created in a different thread context and the
                // server must be shutdown in this thread context.
                CMsgReqChangeSettings* pMsg = new CMsgReqChangeSettings(
                    /* pObjSender       */ nullptr,
                    /* pObjReceiver     */ pLogServer,
                    /* hostSettings     */ pLogServer->getHostSettings(),
                    /* bMustBeConfirmed */ false,
                    /* iReqId           */ -1,
                    /* iMsgId           */ -1 );
                QCoreApplication::postEvent(pLogServer, pMsg);
                pMsg = NULL;
            }
        } // if( pLogServer != nullptr )
    } // if( i_pLogServer != nullptr )

    return bOk;

} // IpcLogServer_setPort

//------------------------------------------------------------------------------
ZSIPCLOGDLL_EXTERN_API unsigned short IpcLogServer_getPort( const DllIf::CIpcLogServer* i_pLogServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    unsigned short uPort = 0;

    if( i_pLogServer != nullptr )
    {
        CIpcLogServer* pLogServer = CIpcLogServer::GetInstance();

        if( pLogServer != nullptr )
        {
            ZS::Ipc::SServerHostSettings hostSettings = pLogServer->getHostSettings();

            uPort = hostSettings.m_uLocalPort;
        }
    }
    return uPort;
}


/*******************************************************************************
class DllIf::CLogger
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor (declared public but for internal use only, implemented in ZSIpcLog::ZSIpcLogDllMain)
==============================================================================*/

//------------------------------------------------------------------------------
DllIf::CLogger::CLogger()
//------------------------------------------------------------------------------
{
} // ctor

//------------------------------------------------------------------------------
DllIf::CLogger::~CLogger()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class DllIf::CLogServer
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
DllIf::CLogServer::CLogServer()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
DllIf::CLogServer::~CLogServer()
//------------------------------------------------------------------------------
{
}


/*******************************************************************************
class DllIf::CIpcLogServer
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
DllIf::CIpcLogServer::CIpcLogServer() :
//------------------------------------------------------------------------------
    CLogServer()
{
}

//------------------------------------------------------------------------------
DllIf::CIpcLogServer::~CIpcLogServer()
//------------------------------------------------------------------------------
{
}


/*******************************************************************************
Entry point for the DLL application.
*******************************************************************************/

#ifdef _WINDOWS
//------------------------------------------------------------------------------
BOOL APIENTRY DllMain(
    HANDLE /*i_hModule*/,
    DWORD  i_ul_reason_for_call,
    LPVOID /*i_lpReserved*/ )
//------------------------------------------------------------------------------
{
    // To allow setting breakpoints:
    switch( i_ul_reason_for_call )
    {
        case DLL_PROCESS_ATTACH:
        {
            ++s_iDLL_PROCESS_ATTACH;
            break;
        }
        case DLL_THREAD_ATTACH:
        {
            ++s_iDLL_THREAD_ATTACH;
            break;
        }
        case DLL_THREAD_DETACH:
        {
            --s_iDLL_THREAD_ATTACH;
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            --s_iDLL_PROCESS_ATTACH;
            break;
        }
        default:
        {
            break;
        }
    }
    return TRUE;

} // DllMain
#endif
