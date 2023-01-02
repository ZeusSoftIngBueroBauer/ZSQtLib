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

#include "ZSIpcTrace/ZSIpcTrcDllMain.h"
#include "ZSIpcTrace/ZSIpcTrcDllIf.h"
#include "ZSIpcTrace/ZSIpcTrcDllIfServerThread.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcMthFile.h"
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
using namespace ZS::Trace;

using namespace std;

namespace ZS
{
namespace Trace
{
namespace DllIf
{
class CIpcTrcServerThread;

} // namespace DllIf
} // namespace Trace
} // namespace ZS


/*******************************************************************************
public Dll interface methods
*******************************************************************************/

#ifdef _WINDOWS
static int s_iDLL_PROCESS_ATTACH = 0;
static int s_iDLL_THREAD_ATTACH = 0;
#endif

static QMutex DllIf_s_mtx;

static CTrcMthFile* DllIf_IpcTrcServer_s_pTrcMthFile = nullptr;
static EMethodTraceDetailLevel DllIf_IpcTrcServer_s_eTrcMthDetailLevel = EMethodTraceDetailLevel::None;

static QCoreApplication* DllIf_s_pQtAppCreatedByDllIf = nullptr;

static DllIf::CIpcTrcServer* DllIf_IpcTrcServer_s_pTheInst = nullptr;
static int DllIf_IpcTrcServer_s_iTrcServerRefCount = 0;
static DllIf::CIpcTrcServerThread* DllIf_s_pIpcTrcServerThread = nullptr;

const QString c_strNameSpace = "ZS::Trace::DllIf";
const QString c_strClassName = "DllMain";

/*==============================================================================
DllMain::CTrcAdminObj
==============================================================================*/

namespace ZS
{
namespace Trace
{
namespace DllIf
{
namespace DllMain
{
class CTrcAdminObj : public DllIf::CTrcAdminObj
{
public: // ctors and dtor
    CTrcAdminObj( const char* i_szKeyInTree ) :
        DllIf::CTrcAdminObj(),
        m_szKeyInTree(nullptr),
        m_iLockCount(0),
        m_bDeleteOnUnlock(false),
        m_iRefCount(0)
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        ZS::System::EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        ZS::System::CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ ZS::System::EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ "CTrcAdminObj",
            /* strObjName         */ i_szKeyInTree,
            /* strMethod          */ "ctor",
            /* strMthInArgs       */ "" );

        m_pMtx = new QMutex(QMutex::Recursive);

        size_t iStrLen = strlen(i_szKeyInTree);
        m_szKeyInTree = new char[iStrLen+1];
        memset(m_szKeyInTree, 0x00, iStrLen+1);
        memcpy(m_szKeyInTree, i_szKeyInTree, iStrLen);
    }
    virtual ~CTrcAdminObj()
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        ZS::System::EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        ZS::System::CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ ZS::System::EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ "CTrcAdminObj",
            /* strObjName         */ m_szKeyInTree,
            /* strMethod          */ "dtor",
            /* strMthInArgs       */ "" );

        delete m_pMtx;
        m_pMtx = nullptr;
        delete m_szKeyInTree;
        m_szKeyInTree = nullptr;
        m_iLockCount = 0;
        m_bDeleteOnUnlock = false;
        m_iRefCount = 0;
    }
public: // instance methods
    virtual const char* keyInTree() const override
    {
        QMutexLocker mtxLocker(m_pMtx);
        return m_szKeyInTree;
    }
public: // instance methods
    virtual int lock() override
    {
        QMutexLocker mtxLocker(m_pMtx);
        ++m_iLockCount;
        return m_iLockCount;
    }
    virtual int unlock() override
    {
        QMutexLocker mtxLocker(m_pMtx);
        --m_iLockCount;
        return m_iLockCount;
    }
    virtual bool isLocked() const override
    {
        QMutexLocker mtxLocker(m_pMtx);
        return (m_iLockCount > 0);
    }
    virtual int getLockCount() const override
    {
        QMutexLocker mtxLocker(m_pMtx);
        return m_iLockCount;
    }
    virtual void setDeleteOnUnlock( bool i_bDelete ) override
    {
        QMutexLocker mtxLocker(m_pMtx);
        m_bDeleteOnUnlock = i_bDelete;
    }
    virtual bool deleteOnUnlock() const override
    {
        QMutexLocker mtxLocker(m_pMtx);
        return (m_iLockCount <= 0) && (m_iRefCount <= 0) && m_bDeleteOnUnlock;
    }
public: // instance methods
    virtual int incrementRefCount() override
    {
        QMutexLocker mtxLocker(m_pMtx);
        ++m_iRefCount;
        return m_iRefCount;
    }
    virtual int decrementRefCount() override
    {
        QMutexLocker mtxLocker(m_pMtx);
        --m_iRefCount;
        return m_iRefCount;
    }
    virtual void setRefCount( int i_iRefCount ) override
    {
        QMutexLocker mtxLocker(m_pMtx);
        m_iRefCount = i_iRefCount;
    }
    virtual int getRefCount() const override
    {
        QMutexLocker mtxLocker(m_pMtx);
        return m_iRefCount;
    }
private: // instance members
    QMutex* m_pMtx;
    char*   m_szKeyInTree;
    int     m_iLockCount;
    bool    m_bDeleteOnUnlock;
    int     m_iRefCount;
};
} // namespace DllMain
} // namespace DllIf
} // namespace Trace
} // namespace ZS


/*==============================================================================
DllIf::CTrcAdminObj
==============================================================================*/

static QHash<QString, DllIf::DllMain::CTrcAdminObj*> DllIf_TrcServer_s_hshpTrcAdminObjs;

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcAdminObj_getNameSpace( const DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    char* szName = nullptr;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                std::string stdstrName = pTrcAdminObj->getNameSpace().toStdString();
                szName = new char[stdstrName.size() + 1];
                memcpy(szName, stdstrName.c_str(), stdstrName.size());
                szName[stdstrName.size()] = 0x00;
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return szName;

} // TrcAdminObj_getNameSpace

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcAdminObj_getClassName( const DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    char* szName = nullptr;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                std::string stdstrName = pTrcAdminObj->getClassName().toStdString();
                szName = new char[stdstrName.size() + 1];
                memcpy(szName, stdstrName.c_str(), stdstrName.size());
                szName[stdstrName.size()] = 0x00;
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return szName;

} // TrcAdminObj_getClassName

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcAdminObj_getObjectName( const DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    char* szName = nullptr;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                std::string stdstrName = pTrcAdminObj->getObjectName().toStdString();
                szName = new char[stdstrName.size() + 1];
                memcpy(szName, stdstrName.c_str(), stdstrName.size());
                szName[stdstrName.size()] = 0x00;
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return szName;

} // TrcAdminObj_getObjectName

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcAdminObj_setObjectThreadName(
    DllIf::CTrcAdminObj* i_pTrcAdminObj,
    const char*          i_szThreadName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs = i_szThreadName;
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_setObjectThreadName",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcAdminObj->setObjectThreadName(i_szThreadName);
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

} // TrcAdminObj_setObjectThreadName

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcAdminObj_getObjectThreadName( const DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    char* szName = nullptr;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                std::string stdstrName = pTrcAdminObj->getObjectThreadName().toStdString();
                szName = new char[stdstrName.size() + 1];
                memcpy(szName, stdstrName.c_str(), stdstrName.size());
                szName[stdstrName.size()] = 0x00;
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return szName;

} // TrcAdminObj_getObjectThreadName

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcAdminObj_lock( DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    int iLockCount = 0;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_lock",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                iLockCount = pTrcAdminObj->lock();
            }
        } // if( pTrcServer != nullptr )

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            mthTracer.setMethodReturn(iLockCount);
        }
    } // if( i_pTrcAdminObj != nullptr )

    return iLockCount;

} // TrcAdminObj_lock

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcAdminObj_unlock( DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    int iLockCount = 0;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_unlock",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                iLockCount = pTrcAdminObj->unlock();
            }
        } // if( pTrcServer != nullptr )

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            mthTracer.setMethodReturn(iLockCount);
        }
    } // if( i_pTrcAdminObj != nullptr )

    return iLockCount;

} // TrcAdminObj_unlock

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcAdminObj_isLocked( DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bIsLocked = false;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_isLocked",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                bIsLocked = pTrcAdminObj->isLocked();
            }
        } // if( pTrcServer != nullptr )

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            mthTracer.setMethodReturn(bIsLocked);
        }
    } // if( i_pTrcAdminObj != nullptr )

    return bIsLocked;

} // TrcAdminObj_isLocked

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcAdminObj_getLockCount( DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    int iLockCount = 0;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_getLockCount",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                iLockCount = pTrcAdminObj->getLockCount();
            }
        } // if( pTrcServer != nullptr )

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            mthTracer.setMethodReturn(iLockCount);
        }
    } // if( i_pTrcAdminObj != nullptr )

    return iLockCount;

} // TrcAdminObj_getLockCount

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcAdminObj_setDeleteOnUnlock(
    DllIf::CTrcAdminObj* i_pTrcAdminObj,
    bool                 i_bDelete )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs = bool2Str(i_bDelete);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_setDeleteOnUnlock",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcAdminObj->setDeleteOnUnlock(i_bDelete);
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

} // TrcAdminObj_setDeleteOnUnlock

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcAdminObj_deleteOnUnlock(
    DllIf::CTrcAdminObj* i_pTrcAdminObj,
    bool                 i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bDelete = false;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs = bool2Str(i_bEnabled);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_deleteOnUnlock",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                bDelete = pTrcAdminObj->deleteOnUnlock();
            }
        } // if( pTrcServer != nullptr )

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            mthTracer.setMethodReturn(bDelete);
        }
    } // if( i_pTrcAdminObj != nullptr )

    return bDelete;

} // TrcAdminObj_deleteOnUnlock

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcAdminObj_setEnabled(
    DllIf::CTrcAdminObj* i_pTrcAdminObj,
    bool                 i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs = bool2Str(i_bEnabled);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_setEnabled",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcAdminObj->setEnabled(i_bEnabled ? EEnabled::Yes : EEnabled::No);
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

} // TrcAdminObj_setEnabled

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcAdminObj_isEnabled( const DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bEnabled = false;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                bEnabled = pTrcAdminObj->isEnabled();
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return bEnabled;

} // TrcAdminObj_isEnabled

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcAdminObj_setMethodCallsTraceDetailLevel(
    DllIf::CTrcAdminObj*                i_pTrcAdminObj,
    DllIf::EMethodTraceDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs = CEnumMethodTraceDetailLevel(i_eDetailLevel).toString();
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_setMethodCallsTraceDetailLevel",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcAdminObj->setMethodCallsTraceDetailLevel(static_cast<EMethodTraceDetailLevel>(i_eDetailLevel));
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

} // TrcAdminObj_setMethodCallsTraceDetailLevel

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::EMethodTraceDetailLevel TrcAdminObj_getMethodCallsTraceDetailLevel( const DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    DllIf::EMethodTraceDetailLevel eDetailLevel = DllIf::EMethodTraceDetailLevelNone;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                eDetailLevel = static_cast<DllIf::EMethodTraceDetailLevel>(pTrcAdminObj->getMethodCallsTraceDetailLevel());
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return eDetailLevel;

} // TrcAdminObj_getMethodCallsTraceDetailLevel

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcAdminObj_areMethodCallsActive(
    const DllIf::CTrcAdminObj*     i_pTrcAdminObj,
    DllIf::EMethodTraceDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bIsActive = false;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                bIsActive = pTrcAdminObj->areMethodCallsActive(static_cast<EMethodTraceDetailLevel>(i_eDetailLevel));
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return bIsActive;

} // TrcAdminObj_areMethodCallsActive

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcAdminObj_setRuntimeInfoTraceDetailLevel(
    DllIf::CTrcAdminObj*   i_pTrcAdminObj,
    DllIf::ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs = CEnumLogDetailLevel(i_eDetailLevel).toString();
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_setRuntimeInfoTraceDetailLevel",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcAdminObj->setRuntimeInfoTraceDetailLevel(static_cast<ELogDetailLevel>(i_eDetailLevel));
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

} // TrcAdminObj_setRuntimeInfoTraceDetailLevel

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::ELogDetailLevel TrcAdminObj_getRuntimeInfoTraceDetailLevel( const DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    DllIf::ELogDetailLevel eDetailLevel = DllIf::ELogDetailLevelNone;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                eDetailLevel = static_cast<DllIf::ELogDetailLevel>(pTrcAdminObj->getRuntimeInfoTraceDetailLevel());
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return eDetailLevel;

} // TrcAdminObj_getRuntimeInfoTraceDetailLevel

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcAdminObj_isRuntimeInfoActive(
    const DllIf::CTrcAdminObj* i_pTrcAdminObj,
    DllIf::ELogDetailLevel     i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bIsActive = false;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                bIsActive = pTrcAdminObj->isRuntimeInfoActive(static_cast<ELogDetailLevel>(i_eDetailLevel));
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return bIsActive;

} // TrcAdminObj_isRuntimeInfoActive

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcAdminObj_setTraceDataFilter(
    DllIf::CTrcAdminObj* i_pTrcAdminObj,
    const char*          i_szFilter )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs = i_szFilter;
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_setTraceDataFilter",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcAdminObj->setTraceDataFilter(i_szFilter);
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

} // TrcAdminObj_setTraceDataFilter

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcAdminObj_getTraceDataFilter( const DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    char* szFilter = nullptr;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                std::string stdstrFilter = pTrcAdminObj->getTraceDataFilter().toStdString();
                szFilter = new char[stdstrFilter.size() + 1];
                memcpy(szFilter, stdstrFilter.c_str(), stdstrFilter.size());
                szFilter[stdstrFilter.size()] = 0x00;
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return szFilter;

} // TrcAdminObj_getTraceDataFilter

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcAdminObj_isTraceDataSuppressedByFilter(
    DllIf::CTrcAdminObj* i_pTrcAdminObj,
    const char*          i_szTraceData )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bIsSuppressed = false;

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                bIsSuppressed = pTrcAdminObj->isTraceDataSuppressedByFilter(i_szTraceData);
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return bIsSuppressed;

} // TrcAdminObj

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcAdminObj_traceMethodEnter(
    DllIf::CTrcAdminObj* i_pTrcAdminObj,
    const char*          i_szObjName,
    const char*          i_szMethod,
    const char*          i_szMethodInArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs  = "ObjName: " + QString(i_szObjName);
            strMthInArgs += ", Method: " + QString(i_szMethod);
            strMthInArgs += ", InArgs: " + QString(i_szMethodInArgs);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_traceMethodEnter",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcServer->traceMethodEnter(
                    /* pAdminObj    */ pTrcAdminObj,
                    /* strObjName   */ i_szObjName,
                    /* strMethod    */ i_szMethod,
                    /* strMthInArgs */ i_szMethodInArgs );
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

} // TrcAdminObj_traceMethodEnter

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcAdminObj_traceMethodLeave(
    DllIf::CTrcAdminObj* i_pTrcAdminObj,
    const char*          i_szObjName,
    const char*          i_szMethod,
    const char*          i_szMethodReturn,
    const char*          i_szMethodOutArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs  = "ObjName: " + QString(i_szObjName);
            strMthInArgs += ", Method: " + QString(i_szMethod);
            strMthInArgs += ", Return: " + QString(i_szMethodReturn);
            strMthInArgs += ", OutArgs: " + QString(i_szMethodOutArgs);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_traceMethodLeave",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcServer->traceMethodLeave(
                    /* pAdminObj     */ pTrcAdminObj,
                    /* strObjName    */ i_szObjName,
                    /* strMethod     */ i_szMethod,
                    /* strMthReturn  */ i_szMethodReturn,
                    /* strMthOutArgs */ i_szMethodOutArgs );
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

} // TrcAdminObj_traceMethodLeave

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcAdminObj_traceMethod(
    DllIf::CTrcAdminObj* i_pTrcAdminObj,
    const char*          i_szObjName,
    const char*          i_szMethod,
    const char*          i_szMethodAddInfo )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs  = "ObjName: " + QString(i_szObjName);
            strMthInArgs += ", Method: " + QString(i_szMethod);
            strMthInArgs += ", AddInfo: " + QString(i_szMethodAddInfo);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_traceMethod",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcServer->traceMethod(
                    /* pAdminObj   */ pTrcAdminObj,
                    /* strObjName  */ i_szObjName,
                    /* strMethod   */ i_szMethod,
                    /* strdAddInfo */ i_szMethodAddInfo );
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

} // TrcAdminObj_traceMethod

/*==============================================================================
DllIf::CTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::CTrcAdminObj* TrcServer_GetTraceAdminObj(
    const char* i_szNameSpace,
    const char* i_szClassName,
    const char* i_szObjName,
    EEnabled    i_bEnabledAsDefault,
    DllIf::EMethodTraceDetailLevel i_eMethodCallsDefaultDetailLevel,
    DllIf::ELogDetailLevel i_eRuntimeInfoDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    if( s_iDLL_PROCESS_ATTACH <= 0 ) // Dll already unloaded
    {
        return NULL;
    }
    #endif

    QMutexLocker mtxLocker(&DllIf_s_mtx);

    DllIf::DllMain::CTrcAdminObj* pDllIfTrcAdminObj = nullptr;

    QString strNameSpace  = i_szNameSpace;
    QString strClassName  = i_szClassName;
    QString strObjName    = i_szObjName;

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "NameSpace: " + QString(i_szNameSpace);
        strMthInArgs += ", ClassName: " + QString(i_szClassName);
        strMthInArgs += ", ObjName: " + QString(i_szObjName);
        strMthInArgs += ", DefEnabled: " + CEnumEnabled::toString(i_bEnabledAsDefault);
        strMthInArgs += ", MthCallsDefLevel: " + CEnumMethodTraceDetailLevel(i_eMethodCallsDefaultDetailLevel).toString();
        strMthInArgs += ", RunInfoDefLevel: " + CEnumLogDetailLevel(i_eRuntimeInfoDefaultDetailLevel).toString();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_GetTraceAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        CTrcAdminObj* pTrcAdminObj = CTrcServer::GetTraceAdminObj(
            /* strNameSpace          */ strNameSpace,
            /* strClassName          */ strClassName,
            /* strObjName            */ strObjName,
            /* bEnabledAsDefault     */ i_bEnabledAsDefault,
            /* iMthCallsDefaultLevel */ static_cast<EMethodTraceDetailLevel>(i_eMethodCallsDefaultDetailLevel),
            /* iRunInfoDefaultLevel  */ static_cast<ELogDetailLevel>(i_eRuntimeInfoDefaultDetailLevel) );

        if( pTrcAdminObj != nullptr )
        {
            QString strKeyInTree = pTrcAdminObj->keyInTree();

            pDllIfTrcAdminObj = DllIf_TrcServer_s_hshpTrcAdminObjs.value(strKeyInTree, nullptr);

            if( pDllIfTrcAdminObj == nullptr )
            {
                pDllIfTrcAdminObj = new DllIf::DllMain::CTrcAdminObj(strKeyInTree.toUtf8());
                DllIf_TrcServer_s_hshpTrcAdminObjs[strKeyInTree] = pDllIfTrcAdminObj;
            }

            pDllIfTrcAdminObj->incrementRefCount();
        }
    } // if( pTrcServer != nullptr )

    return pDllIfTrcAdminObj;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::CTrcAdminObj* TrcServer_RenameTraceAdminObj(
    DllIf::CTrcAdminObj* i_pTrcAdminObj,
    const char*          i_szNewObjName )
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    if( s_iDLL_PROCESS_ATTACH <= 0 ) // Dll already unloaded
    {
        return nullptr;
    }
    #endif

    QMutexLocker mtxLocker(&DllIf_s_mtx);

    DllIf::DllMain::CTrcAdminObj* pDllIfTrcAdminObj =
        dynamic_cast<DllIf::DllMain::CTrcAdminObj*>(i_pTrcAdminObj);

    QString strNewObjName = i_szNewObjName;

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = QString(pDllIfTrcAdminObj == nullptr ? "null" : pDllIfTrcAdminObj->keyInTree());
        strMthInArgs += ", NewObjName: " + strNewObjName;
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_RenameTraceAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    if( pDllIfTrcAdminObj != nullptr )
    {
        QString strOldKeyInTree = i_pTrcAdminObj->keyInTree();
        QString strNewKeyInTree = strOldKeyInTree;

        int iRefCount = pDllIfTrcAdminObj->getRefCount();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strOldKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcAdminObj = CTrcServer::RenameTraceAdminObj(pTrcAdminObj, strNewObjName);
                strNewKeyInTree = pTrcAdminObj->keyInTree();
            }
        } // if( pTrcServer != nullptr )

        if( strNewKeyInTree != strOldKeyInTree )
        {
            int iRefCount = pDllIfTrcAdminObj->decrementRefCount();

            if( iRefCount == 0 )
            {
                if( pDllIfTrcAdminObj->isLocked() )
                {
                    pDllIfTrcAdminObj->setDeleteOnUnlock(true);
                }
                else
                {
                    DllIf_TrcServer_s_hshpTrcAdminObjs.remove(strOldKeyInTree);
                    delete pDllIfTrcAdminObj;
                    pDllIfTrcAdminObj = nullptr;
                }
            }

            pDllIfTrcAdminObj = DllIf_TrcServer_s_hshpTrcAdminObjs.value(strNewKeyInTree, nullptr);

            if( pDllIfTrcAdminObj == nullptr )
            {
                pDllIfTrcAdminObj = new DllIf::DllMain::CTrcAdminObj(strNewKeyInTree.toUtf8());
                DllIf_TrcServer_s_hshpTrcAdminObjs[strNewKeyInTree] = pDllIfTrcAdminObj;
            }

            pDllIfTrcAdminObj->incrementRefCount();
        }
    } // if( pDllIfTrcAdminObj != nullptr )

    return pDllIfTrcAdminObj;

} // TrcServer_RenameTraceAdminObj

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_ReleaseTraceAdminObj( DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    if( s_iDLL_PROCESS_ATTACH <= 0 ) // Dll already unloaded
    {
        return;
    }
    #endif

    QMutexLocker mtxLocker(&DllIf_s_mtx);

    DllIf::DllMain::CTrcAdminObj* pDllIfTrcAdminObj =
        dynamic_cast<DllIf::DllMain::CTrcAdminObj*>(i_pTrcAdminObj);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_pTrcAdminObj == nullptr ? "null" : i_pTrcAdminObj->keyInTree();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_ReleaseTraceAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    if( pDllIfTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                CTrcServer::ReleaseTraceAdminObj(pTrcAdminObj);
            }
        } // if( pTrcServer != nullptr )

        if( pDllIfTrcAdminObj->deleteOnUnlock() )
        {
            pDllIfTrcAdminObj->setDeleteOnUnlock(false);
            DllIf_TrcServer_s_hshpTrcAdminObjs.remove(strKeyInTree);
            delete pDllIfTrcAdminObj;
            pDllIfTrcAdminObj = nullptr;
        }
        else
        {
            pDllIfTrcAdminObj->decrementRefCount();
        }

        // The trace admin object of the Dll interface will be kept as long as the trace server is alive.
        // If the trace server is released and its reference counter reaches 0 the admin objects are deleted.

    } // if( pDllIfTrcAdminObj != nullptr )

} // TrcServer_ReleaseTraceAdminObj

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_SetOrganizationName( const char* i_szName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    QCoreApplication::setOrganizationName(i_szName);
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_GetOrganizationName()
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
ZSIPCTRACEDLL_EXTERN_API void TrcServer_SetApplicationName( const char* i_szName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    QCoreApplication::setApplicationName(i_szName);
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_GetApplicationName()
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
ZSIPCTRACEDLL_EXTERN_API void TrcServer_SetAdminObjFileAbsoluteFilePath( const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CTrcServer::SetAdminObjFileAbsoluteFilePath(i_szAbsFilePath);
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_GetAdminObjFileAbsoluteFilePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrAbsFilePath = CTrcServer::GetAdminObjFileAbsoluteFilePath().toStdString();
    char* szAbsFilePath = new char[stdstrAbsFilePath.size() + 1];
    memcpy(szAbsFilePath, stdstrAbsFilePath.c_str(), stdstrAbsFilePath.size());
    szAbsFilePath[stdstrAbsFilePath.size()] = 0x00;
    return szAbsFilePath;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_GetAdminObjFileCompleteBaseName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrBaseName = CTrcServer::GetAdminObjFileCompleteBaseName().toStdString();
    char* szBaseName = new char[stdstrBaseName.size() + 1];
    memcpy(szBaseName, stdstrBaseName.c_str(), stdstrBaseName.size());
    szBaseName[stdstrBaseName.size()] = 0x00;
    return szBaseName;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_GetAdminObjFileAbsolutePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrAbsPath = CTrcServer::GetAdminObjFileAbsolutePath().toStdString();
    char* szAbsPath = new char[stdstrAbsPath.size() + 1];
    memcpy(szAbsPath, stdstrAbsPath.c_str(), stdstrAbsPath.size());
    szAbsPath[stdstrAbsPath.size()] = 0x00;
    return szAbsPath;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_SetLocalTrcFileAbsoluteFilePath( const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CTrcServer::SetLocalTrcFileAbsoluteFilePath(i_szAbsFilePath);
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_GetLocalTrcFileAbsoluteFilePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrAbsFilePath = CTrcServer::GetLocalTrcFileAbsoluteFilePath().toStdString();
    char* szAbsFilePath = new char[stdstrAbsFilePath.size() + 1];
    memcpy(szAbsFilePath, stdstrAbsFilePath.c_str(), stdstrAbsFilePath.size());
    szAbsFilePath[stdstrAbsFilePath.size()] = 0x00;
    return szAbsFilePath;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_GetLocalTrcFileCompleteBaseName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrBaseName = CTrcServer::GetLocalTrcFileCompleteBaseName().toStdString();
    char* szBaseName = new char[stdstrBaseName.size() + 1];
    memcpy(szBaseName, stdstrBaseName.c_str(), stdstrBaseName.size());
    szBaseName[stdstrBaseName.size()] = 0x00;
    return szBaseName;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_GetLocalTrcFileAbsolutePath()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    std::string stdstrAbsPath = CTrcServer::GetLocalTrcFileAbsolutePath().toStdString();
    char* szAbsPath = new char[stdstrAbsPath.size() + 1];
    memcpy(szAbsPath, stdstrAbsPath.c_str(), stdstrAbsPath.size());
    szAbsPath[stdstrAbsPath.size()] = 0x00;
    return szAbsPath;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_RegisterCurrentThread( const char* i_szThreadName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CTrcServer::RegisterCurrentThread(i_szThreadName);
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_UnregisterCurrentThread()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    CTrcServer::UnregisterCurrentThread();
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_GetCurrentThreadName()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    std::string stdstrName = CTrcServer::GetCurrentThreadName().toStdString();
    char* szName = new char[stdstrName.size() + 1];
    memcpy(szName, stdstrName.c_str(), stdstrName.size());
    szName[stdstrName.size()] = 0x00;
    return szName;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_isActive()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bIsActive = false;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        bIsActive = pTrcServer->isActive();
    }
    return bIsActive;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setEnabled( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bEnabled);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setEnabled",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->setEnabled(i_bEnabled);
    }
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_isEnabled()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bEnabled = false;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        bEnabled = pTrcServer->isEnabled();
    }
    return bEnabled;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setNewTrcAdminObjsEnabledAsDefault( bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bEnabled);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setNewTrcAdminObjsEnabledAsDefault",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->setNewTrcAdminObjsEnabledAsDefault(i_bEnabled);
    }
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_areNewTrcAdminObjsEnabledAsDefault()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bEnabled = false;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        bEnabled = pTrcServer->areNewTrcAdminObjsEnabledAsDefault();
    }
    return bEnabled;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setNewTrcAdminObjsMethodCallsDefaultDetailLevel( const DllIf::EMethodTraceDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = CEnumMethodTraceDetailLevel(i_eDetailLevel).toString();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setNewTrcAdminObjsMethodCallsDefaultDetailLevel",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->setNewTrcAdminObjsMethodCallsDefaultDetailLevel(static_cast<EMethodTraceDetailLevel>(i_eDetailLevel));
    }
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::EMethodTraceDetailLevel TrcServer_getNewTrcAdminObjsMethodCallsDefaultDetailLevel()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    DllIf::EMethodTraceDetailLevel eDetailLevel = DllIf::EMethodTraceDetailLevelNone;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        eDetailLevel = static_cast<DllIf::EMethodTraceDetailLevel>(pTrcServer->getNewTrcAdminObjsMethodCallsDefaultDetailLevel());
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel( DllIf::ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = CEnumLogDetailLevel(i_eDetailLevel).toString();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->setNewTrcAdminObjsRuntimeInfoDefaultDetailLevel(static_cast<ELogDetailLevel>(i_eDetailLevel));
    }
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::ELogDetailLevel TrcServer_getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    DllIf::ELogDetailLevel eDetailLevel = DllIf::ELogDetailLevelNone;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        eDetailLevel = static_cast<DllIf::ELogDetailLevel>(pTrcServer->getNewTrcAdminObjsRuntimeInfoDefaultDetailLevel());
    }
    return eDetailLevel;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_recallAdminObjs()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bOk = false;

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_recallAdminObjs",
        /* strMthInArgs       */ "" );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        SErrResultInfo errResultInfo = pTrcServer->recallAdminObjs();

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
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_saveAdminObjs()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bOk = false;

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_saveAdminObjs",
        /* strMthInArgs       */ "" );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        SErrResultInfo errResultInfo = pTrcServer->saveAdminObjs();

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
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setUseLocalTrcFile( bool i_bUse )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bUse);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setUseLocalTrcFile",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->setUseLocalTrcFile(i_bUse);
    }
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_isLocalTrcFileUsed()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bUsed = false;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        bUsed = pTrcServer->isLocalTrcFileUsed();
    }
    return bUsed;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcServer_isLocalTrcFileActive()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bActive = false;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        bActive = pTrcServer->isLocalTrcFileActive();
    }
    return bActive;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setLocalTrcFileAutoSaveIntervalInMs( int i_iAutoSaveInterval_ms )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = QString::number(i_iAutoSaveInterval_ms);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setLocalTrcFileAutoSaveInterval",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->setLocalTrcFileAutoSaveIntervalInMs(i_iAutoSaveInterval_ms);
    }
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcServer_getLocalTrcFileAutoSaveIntervalInMs()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    int iAutoSaveInterval_ms = 0;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        iAutoSaveInterval_ms = pTrcServer->getLocalTrcFileAutoSaveIntervalInMs();
    }
    return iAutoSaveInterval_ms;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setLocalTrcFileCloseFileAfterEachWrite( bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bCloseFile);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setLocalTrcFileCloseFileAfterEachWrite",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->setLocalTrcFileCloseFileAfterEachWrite(i_bCloseFile);
    }
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_getLocalTrcFileCloseFileAfterEachWrite()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bCloseFile = false;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        bCloseFile = pTrcServer->getLocalTrcFileCloseFileAfterEachWrite();
    }
    return bCloseFile;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setLocalTrcFileSubFileCountMax( bool i_iCountMax )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = QString::number(i_iCountMax);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setLocalTrcFileSubFileCountMax",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->setLocalTrcFileSubFileCountMax(i_iCountMax);
    }
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcServer_getLocalTrcFileSubFileCountMax()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    int iCountMax = 0;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        iCountMax = pTrcServer->getLocalTrcFileSubFileCountMax();
    }
    return iCountMax;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setLocalTrcFileSubFileLineCountMax( int i_iCountMax )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = QString::number(i_iCountMax);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setLocalTrcFileSubFileLineCountMax",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->setLocalTrcFileSubFileLineCountMax(i_iCountMax);
    }
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcServer_getLocalTrcFileSubFileLineCountMax()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    int iCountMax = 0;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        iCountMax = pTrcServer->getLocalTrcFileSubFileLineCountMax();
    }
    return iCountMax;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setUseIpcServer( bool i_bUseIpcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bUseIpcServer);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setUseIpcServer",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->setUseIpcServer(i_bUseIpcServer);
    }
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_isIpcServerUsed()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bUseIpcServer = false;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        bUseIpcServer = pTrcServer->isIpcServerUsed();
    }
   return bUseIpcServer;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setCacheTrcDataIfNotConnected( bool i_bCacheData )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = bool2Str(i_bCacheData);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setCacheTrcDataIfNotConnected",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->setCacheTrcDataIfNotConnected(i_bCacheData);
    }
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_getCacheTrcDataIfNotConnected()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    bool bCacheData = false;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        bCacheData = pTrcServer->getCacheTrcDataIfNotConnected();
    }
    return bCacheData;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setCacheTrcDataMaxArrLen( int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = QString::number(i_iMaxArrLen);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setCacheTrcDataMaxArrLen",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->setCacheTrcDataMaxArrLen(i_iMaxArrLen);
    }
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcServer_getCacheTrcDataMaxArrLen()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    int iMaxArrLen = 0;
    CTrcServer* pTrcServer = CTrcServer::GetInstance();
    if( pTrcServer != nullptr )
    {
        iMaxArrLen = pTrcServer->getCacheTrcDataMaxArrLen();
    }
    return iMaxArrLen;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_setTraceSettings( const DllIf::STrcServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bOk = false;

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        //strMthInArgs = i_settings.toString();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_setTraceSettings",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        ZS::System::STrcServerSettings trcSettings;

        trcSettings.m_bEnabled                              = i_settings.m_bEnabled;
        trcSettings.m_strAdminObjFileAbsFilePath            = i_settings.m_szAdminObjFileAbsFilePath;
        trcSettings.m_bNewTrcAdminObjsEnabledAsDefault      = i_settings.m_bNewTrcAdminObjsEnabledAsDefault;
        trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel =
            static_cast<EMethodTraceDetailLevel>(i_settings.m_iNewTrcAdminObjsMethodCallsDefaultDetailLevel);
        trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel =
            static_cast<ELogDetailLevel>(i_settings.m_iNewTrcAdminObjsRuntimeInfoDefaultDetailLevel);
        trcSettings.m_bUseIpcServer                         = i_settings.m_bUseIpcServer;
        trcSettings.m_bCacheDataIfNotConnected              = i_settings.m_bCacheDataIfNotConnected;
        trcSettings.m_iCacheDataMaxArrLen                   = i_settings.m_iCacheDataMaxArrLen ;
        trcSettings.m_bUseLocalTrcFile                      = i_settings.m_bUseLocalTrcFile;
        trcSettings.m_strLocalTrcFileAbsFilePath            = i_settings.m_szLocalTrcFileAbsFilePath;
        trcSettings.m_iLocalTrcFileAutoSaveInterval_ms      = i_settings.m_iLocalTrcFileAutoSaveInterval_ms;
        trcSettings.m_iLocalTrcFileSubFileCountMax          = i_settings.m_iLocalTrcFileSubFileCountMax;
        trcSettings.m_iLocalTrcFileSubFileLineCountMax      = i_settings.m_iLocalTrcFileSubFileLineCountMax;
        trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite  = i_settings.m_bLocalTrcFileCloseFileAfterEachWrite;

        pTrcServer->setTraceSettings(trcSettings);

        bOk = true;
    }

    return bOk;

} // TrcServer_setTraceSettings

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::STrcServerSettings TrcServer_getTraceSettings()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    DllIf::STrcServerSettings dllIfTrcSettings;

    DllIf::STrcServerSettings_init(dllIfTrcSettings);

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        int iStrLen;

        ZS::System::STrcServerSettings trcSettings = pTrcServer->getTraceSettings();

        dllIfTrcSettings.m_bEnabled                              = trcSettings.m_bEnabled;
        dllIfTrcSettings.m_bNewTrcAdminObjsEnabledAsDefault      = trcSettings.m_bNewTrcAdminObjsEnabledAsDefault;
        dllIfTrcSettings.m_iNewTrcAdminObjsMethodCallsDefaultDetailLevel =
            static_cast<int>(trcSettings.m_eNewTrcAdminObjsMethodCallsDefaultDetailLevel);
        dllIfTrcSettings.m_iNewTrcAdminObjsRuntimeInfoDefaultDetailLevel =
            static_cast<int>(trcSettings.m_eNewTrcAdminObjsRuntimeInfoDefaultDetailLevel);
        dllIfTrcSettings.m_bUseIpcServer                         = trcSettings.m_bUseIpcServer;
        dllIfTrcSettings.m_bCacheDataIfNotConnected              = trcSettings.m_bCacheDataIfNotConnected;
        dllIfTrcSettings.m_iCacheDataMaxArrLen                   = trcSettings.m_iCacheDataMaxArrLen ;
        dllIfTrcSettings.m_bUseLocalTrcFile                      = trcSettings.m_bUseLocalTrcFile;
        dllIfTrcSettings.m_iLocalTrcFileAutoSaveInterval_ms      = trcSettings.m_iLocalTrcFileAutoSaveInterval_ms;
        dllIfTrcSettings.m_iLocalTrcFileSubFileCountMax          = trcSettings.m_iLocalTrcFileSubFileCountMax;
        dllIfTrcSettings.m_iLocalTrcFileSubFileLineCountMax      = trcSettings.m_iLocalTrcFileSubFileLineCountMax;
        dllIfTrcSettings.m_bLocalTrcFileCloseFileAfterEachWrite  = trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite;

        iStrLen = trcSettings.m_strAdminObjFileAbsFilePath.length();
        dllIfTrcSettings.m_szAdminObjFileAbsFilePath = new char[iStrLen+1];
        memset(dllIfTrcSettings.m_szAdminObjFileAbsFilePath, 0x00, iStrLen+1);
        memcpy(dllIfTrcSettings.m_szAdminObjFileAbsFilePath, trcSettings.m_strAdminObjFileAbsFilePath.toUtf8().data(), iStrLen);

        iStrLen = trcSettings.m_strLocalTrcFileAbsFilePath.length();
        dllIfTrcSettings.m_szLocalTrcFileAbsFilePath = new char[iStrLen+1];
        memset(dllIfTrcSettings.m_szLocalTrcFileAbsFilePath, 0x00, iStrLen+1);
        memcpy(dllIfTrcSettings.m_szLocalTrcFileAbsFilePath, trcSettings.m_strLocalTrcFileAbsFilePath.toUtf8().data(), iStrLen);
    }

    return dllIfTrcSettings;

} // TrcServer_getTraceSettings

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_clearLocalTrcFile()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_clearLocalTrcFile",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        pTrcServer->clearLocalTrcFile();
    }
}

/*==============================================================================
DllIf::CIpcTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::CIpcTrcServer* IpcTrcServer_GetInstance()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "IpcTrcServer_GetInstance",
        /* strMthInArgs       */ strMthInArgs );

    return DllIf_IpcTrcServer_s_pTheInst;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::CIpcTrcServer* IpcTrcServer_CreateInstance(
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelDllIf,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServer,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerMutex,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServer,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServerMutex,
    DllIf::EMethodTraceDetailLevel i_eTrcDetailLevelTrcServerIpcServerGateway )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    QCoreApplication* pApp = dynamic_cast<QCoreApplication*>(QCoreApplication::instance());

    // If the application creating and starting the trace server is not a Qt application
    // and if the Qt application has not yet been created by this module ...
    if( pApp == nullptr && DllIf_s_pQtAppCreatedByDllIf == nullptr )
    {
        int   argc = 1;
        const char* szAppName = "ZSIpcTrcDllIf";
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

    QString strAdminObjFileAbsFilePath;
    QString strLocalTrcFileAbsFilePath;

    if( DllIf_IpcTrcServer_s_pTrcMthFile == nullptr )
    {
        strAdminObjFileAbsFilePath = CTrcServer::GetAdminObjFileAbsoluteFilePath();
        strLocalTrcFileAbsFilePath = CTrcServer::GetLocalTrcFileAbsoluteFilePath();

        DllIf_IpcTrcServer_s_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);
        DllIf_IpcTrcServer_s_eTrcMthDetailLevel = static_cast<EMethodTraceDetailLevel>(i_eTrcDetailLevelDllIf);
    }

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "IpcTrcServer_CreateInstance",
        /* strMthInArgs       */ strMthInArgs );

    DllIf::CIpcTrcServer* pDllIfTrcServer = DllIf_IpcTrcServer_s_pTheInst;

    int iRefCount = DllIf_IpcTrcServer_s_iTrcServerRefCount + 1;

    if( pDllIfTrcServer != nullptr /* && iRefCount > 0 */ ) // RefCount mus be greater than 0. Otherwise ... crash.
    {
        CIpcTrcServer::GetInstance();

        DllIf_IpcTrcServer_s_iTrcServerRefCount = iRefCount;
    }
    else // if( pDllIfTrcServer == nullptr )
    {
        // No matter whether the application creating the trace server is a Qt application
        // or not the trace servers created via the Dll interface (via this main dll module)
        // will be created and later on started from a separate thread.
        // If the application uses Qt this ensures that the server is hosted in a QThread
        // with an event loop. If the application does not use Qt it is anyway necessary
        // to create the tracer server in a thread with an event loop.
        DllIf::CIpcTrcServerThread* pTrcServerThread = DllIf_s_pIpcTrcServerThread;
        if( pTrcServerThread == nullptr )
        {
            DllIf_s_pIpcTrcServerThread = new DllIf::CIpcTrcServerThread(
                eTrcDetailLevel,
                static_cast<EMethodTraceDetailLevel>(i_eTrcDetailLevelTrcServer),
                static_cast<EMethodTraceDetailLevel>(i_eTrcDetailLevelTrcServerMutex),
                static_cast<EMethodTraceDetailLevel>(i_eTrcDetailLevelTrcServerIpcServer),
                static_cast<EMethodTraceDetailLevel>(i_eTrcDetailLevelTrcServerIpcServerMutex),
                static_cast<EMethodTraceDetailLevel>(i_eTrcDetailLevelTrcServerIpcServerGateway) );
        }
        pTrcServerThread = DllIf_s_pIpcTrcServerThread;

        const int c_iMaxWaitCount = 25;
        int iWaitCount = 0;

        // Start driver thread and wait until the driver has been created.
        if( !pTrcServerThread->isRunning() )
        {
            pTrcServerThread->start();

            iWaitCount = 0;
            while( !pTrcServerThread->isServerCreated() )
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

        // Please note that the trace server has been created (or at least the reference
        // counter for an existing trace server has been incremented) in the thread.
        // Invoking "GetInstance" again here would increment the reference counter twice.
        pDllIfTrcServer = new DllIf::CIpcTrcServer();
        DllIf_IpcTrcServer_s_pTheInst = pDllIfTrcServer;
        DllIf_IpcTrcServer_s_iTrcServerRefCount = iRefCount;

    } // if( pDllIfTrcServer == nullptr )

    return pDllIfTrcServer;

} // IpcTrcServer_CreateInstance

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void IpcTrcServer_ReleaseInstance( DllIf::CIpcTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        int iTrcServerRefCount = 0;

        {
            CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
            EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

            QString strMthInArgs;

            CMethodTracer mthTracer(
                /* pTrcMthFile        */ pTrcMthFile,
                /* eTrcDetailLevel    */ eTrcDetailLevel,
                /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
                /* strNameSpace       */ c_strNameSpace,
                /* strClassName       */ c_strClassName,
                /* strObjName         */ "ZSTrcServer",
                /* strMethod          */ "IpcTrcServer_ReleaseInstance",
                /* strMthInArgs       */ strMthInArgs );

            iTrcServerRefCount = DllIf_IpcTrcServer_s_iTrcServerRefCount - 1;

            if( iTrcServerRefCount > 0 /* && pDllIfTrcServer != nullptr */ ) // pDllIfTrcServer must not be nullptr. Otherwise ... crash.
            {
                CIpcTrcServer::ReleaseInstance();

                DllIf_IpcTrcServer_s_iTrcServerRefCount = iTrcServerRefCount;
            }
            // Trace server is no longer referenced ...
            else // if( iTrcServerRefCount == 0 )
            {
                QStringList strlstKeysInTree = DllIf_TrcServer_s_hshpTrcAdminObjs.keys();

                for( const QString& strKeyInTree : strlstKeysInTree )
                {
                    DllIf::DllMain::CTrcAdminObj* pDllIfTrcAdminObj = DllIf_TrcServer_s_hshpTrcAdminObjs[strKeyInTree];
                    int iTrcAdminObjRefCount = pDllIfTrcAdminObj->getRefCount();

                    if( iTrcAdminObjRefCount > 0 )
                    {
                        SErrResultInfo errResultInfo(
                            /* errSource     */ "ZS::Trace::DllIf", "IpcTrcServer", strKeyInTree, "ReleaseInstance",
                            /* result        */ EResultObjRefCounterIsNotZero,
                            /* severity      */ EResultSeverityError,
                            /* strAddErrInfo */ "The dtor is called even if the objects reference counter is not 0 but " + QString::number(iTrcAdminObjRefCount));

                        if( CErrLog::GetInstance() != nullptr )
                        {
                            CErrLog::GetInstance()->addEntry(errResultInfo);
                        }
                    }

                    DllIf_TrcServer_s_hshpTrcAdminObjs.remove(strKeyInTree);

                    try
                    {
                        delete pDllIfTrcAdminObj;
                    }
                    catch(...)
                    {
                    }
                    pDllIfTrcAdminObj = nullptr;
                }

                // The trace server was created and started from within a different thread context.
                DllIf::CIpcTrcServerThread* pTrcServerThread = DllIf_s_pIpcTrcServerThread;

                pTrcServerThread->quit();
                pTrcServerThread->wait(30000);

                if( !pTrcServerThread->isRunning() )
                {
                    try
                    {
                        delete pTrcServerThread;
                    }
                    catch(...)
                    {
                    }
                }
                pTrcServerThread = nullptr;
                DllIf_s_pIpcTrcServerThread = nullptr;

                // Please note that the trace server has been released in the thread.
                // Invoking "ReleaseInstance" again here would deincrement the reference counter twice.

                DllIf_IpcTrcServer_s_pTheInst = nullptr;
                DllIf_IpcTrcServer_s_iTrcServerRefCount = 0;

                delete i_pTrcServer;
                i_pTrcServer = nullptr;

            } // if( iTrcServerRefCount == 0 )

            // dtor of mthTracer here which access the trace method file.
            // The trace method file got to be removed after this block.
        }

        if( iTrcServerRefCount == 0 )
        {
            if( DllIf_IpcTrcServer_s_pTrcMthFile != nullptr )
            {
                CTrcMthFile::Free(DllIf_IpcTrcServer_s_pTrcMthFile);

                DllIf_IpcTrcServer_s_pTrcMthFile = nullptr;
                DllIf_IpcTrcServer_s_eTrcMthDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
            }
        }
    } // if( i_pTrcServer != nullptr )

    // If the application creating and starting the trace server is not a Qt application ...
    if( DllIf_s_pQtAppCreatedByDllIf != NULL )
    {
        // Free Qt Application if all trace servers referenced by the Dll interface have been released.
        if( DllIf_IpcTrcServer_s_pTheInst == nullptr )
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

} // IpcTrcServer_ReleaseInstance

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool IpcTrcServer_startup( DllIf::CIpcTrcServer* i_pTrcServer, int i_iTimeout_ms, bool i_bWait )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bOk = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
            strMthInArgs += ", Wait: " + bool2Str(i_bWait);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ "ZSTrcServer",
            /* strMethod          */ "IpcTrcServer_startup",
            /* strMthInArgs       */ strMthInArgs );

        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            // No matter whether the application creating the trace server is a Qt application
            // or not the trace servers created via the Dll interface (via this main dll module)
            // will be created and started from a separate thread.
            // The thread hosting the trace server must start an event loop.

            CMsgReqStartup* pMsg = new CMsgReqStartup(
                /* pObjSender       */ nullptr,
                /* pObjReceiver     */ pTrcServer,
                /* hostSettings     */ pTrcServer->getHostSettings(),
                /* pBlkType         */ pTrcServer->getBlkType(),  // The block will be cloned.
                /* bMustBeConfirmed */ false,
                /* iReqId           */ -1,
                /* iMsgId           */ -1 );
            QCoreApplication::postEvent(pTrcServer, pMsg);
            pMsg = NULL;

            const int c_iMaxWaitCount = 25;
            int iWaitCount = 0;

            DllIf::CIpcTrcServerThread* pTrcServerThread = DllIf_s_pIpcTrcServerThread;

            while( !pTrcServerThread->isServerStarted() )
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

            bOk = pTrcServerThread->isServerStarted();

        } // if( pTrcServer != nullptr )
    } // if( i_pTrcServer != nullptr )

    return bOk;

} // IpcTrcServer_startup

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool IpcTrcServer_shutdown( DllIf::CIpcTrcServer* i_pTrcServer, int i_iTimeout_ms, bool i_bWait )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bOk = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
            strMthInArgs += ", Wait: " + bool2Str(i_bWait);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ "ZSTrcServer",
            /* strMethod          */ "IpcTrcServer_shutdown",
            /* strMthInArgs       */ strMthInArgs );

        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            // No matter whether the application creating the trace server is a Qt application
            // the trace server is hosted in a different thread context and the server must be
            // shutdown in this thread context.
            CMsgReqShutdown* pMsg = new CMsgReqShutdown(
                /* pObjSender       */ nullptr,
                /* pObjReceiver     */ pTrcServer,
                /* bMustBeConfirmed */ false,
                /* iReqId           */ -1,
                /* iMsgId           */ -1 );
            QCoreApplication::postEvent(pTrcServer, pMsg);
            pMsg = NULL;

            const int c_iMaxWaitCount = 25;
            int iWaitCount = 0;

            DllIf::CIpcTrcServerThread* pTrcServerThread = DllIf_s_pIpcTrcServerThread;

            if( pTrcServerThread != NULL )
            {
                while( !pTrcServerThread->isServerShutdown() )
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
            bOk = pTrcServerThread->isServerShutdown();

        } // if( pTrcServer != nullptr )
    } // if( i_pTrcServer != nullptr )

    return bOk;

} // IpcTrcServer_shutdown

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool IpcTrcServer_isListening( DllIf::CIpcTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bIsListening = false;

    if( i_pTrcServer != nullptr )
    {
        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            bIsListening = pTrcServer->isListening();
        }
    } // if( i_pTrcServer != nullptr )

    return bIsListening;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool IpcTrcServer_isConnected( DllIf::CIpcTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bIsConnected = false;

    if( i_pTrcServer != nullptr )
    {
        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            bIsConnected = pTrcServer->isConnected();
        }
    } // if( i_pTrcServer != nullptr )

    return bIsConnected;
}

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool IpcTrcServer_setPort(
    DllIf::CIpcTrcServer* i_pTrcServer,
    unsigned short        i_uPort,
    int                   i_iTimeout_ms,
    bool                  i_bWait )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bOk = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

        QString strMthInArgs;

        if( eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
        {
            strMthInArgs  = "Port: " + QString::number(i_uPort);
            strMthInArgs += ", Timeout: " + QString::number(i_iTimeout_ms) + " ms";
            strMthInArgs += ", Wait: " + bool2Str(i_bWait);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* eTrcDetailLevel    */ eTrcDetailLevel,
            /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ "ZSTrcServer",
            /* strMethod          */ "IpcTrcServer_setPort",
            /* strMthInArgs       */ strMthInArgs );

        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            ZS::Ipc::SServerHostSettings hostSettings;

            hostSettings.m_socketType = ZS::Ipc::ESocketTypeTcp;
            //hostSettings.m_pObjLocal = nullptr;
            //hostSettings.m_strLocalHostName;
            //hostSettings.m_hostAddrLocal;
            hostSettings.m_uLocalPort = i_uPort;
            hostSettings.m_uMaxPendingConnections = 30;
            //hostSettings.m_uBufferSize;

            // If the application creating and starting the trace server is a Qt application ...
            if( DllIf_s_pQtAppCreatedByDllIf == nullptr )
            {
                pTrcServer->setHostSettings(hostSettings);

                CRequest* pReq = pTrcServer->changeSettings();

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

            // If the application creating and starting the trace server is not a Qt application ...
            else // if( DllIf_s_pQtAppCreatedByDllIf != nullptr )
            {
                // ... the trace server is created in a different thread context and the
                // server must be shutdown in this thread context.
                CMsgReqChangeSettings* pMsg = new CMsgReqChangeSettings(
                    /* pObjSender       */ nullptr,
                    /* pObjReceiver     */ pTrcServer,
                    /* hostSettings     */ pTrcServer->getHostSettings(),
                    /* bMustBeConfirmed */ false,
                    /* iReqId           */ -1,
                    /* iMsgId           */ -1 );
                QCoreApplication::postEvent(pTrcServer, pMsg);
                pMsg = NULL;

            } // if( DllIf_s_pQtAppCreatedByDllIf != nullptr )
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcServer != nullptr )

    return bOk;

} // IpcTrcServer_setPort

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API unsigned short IpcTrcServer_getPort( const DllIf::CIpcTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    unsigned short uPort = 0;

    if( i_pTrcServer != nullptr )
    {
        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            ZS::Ipc::SServerHostSettings hostSettings = pTrcServer->getHostSettings();

            uPort = hostSettings.m_uLocalPort;
        }
    } // if( i_pTrcServer != nullptr )

    return uPort;
}


/*******************************************************************************
class DllIf::CTrcAdminObj
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor (declared public but for internal use only, implemented in ZSIpcTrace::ZSIpcTrcDllMain)
==============================================================================*/

//------------------------------------------------------------------------------
DllIf::CTrcAdminObj::CTrcAdminObj()
//------------------------------------------------------------------------------
{
} // ctor

//------------------------------------------------------------------------------
DllIf::CTrcAdminObj::~CTrcAdminObj()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class DllIf::CTrcServer
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
DllIf::CTrcServer::CTrcServer()
//------------------------------------------------------------------------------
{
    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    ZS::System::EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    ZS::System::CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ ZS::System::EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ "CTrcServer",
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );

} // ctor

//------------------------------------------------------------------------------
DllIf::CTrcServer::~CTrcServer()
//------------------------------------------------------------------------------
{
    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    ZS::System::EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    ZS::System::CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ ZS::System::EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ "CTrcServer",
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "dtor",
        /* strMthInArgs       */ "" );

} // dtor


/*******************************************************************************
class DllIf::CIpcTrcServer
*******************************************************************************/

/*==============================================================================
protected: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
DllIf::CIpcTrcServer::CIpcTrcServer() :
//------------------------------------------------------------------------------
    CTrcServer()
{
    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    ZS::System::EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    ZS::System::CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ ZS::System::EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ "CIpcTrcServer",
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );

} // ctor

//------------------------------------------------------------------------------
DllIf::CIpcTrcServer::~CIpcTrcServer()
//------------------------------------------------------------------------------
{
    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    ZS::System::EMethodTraceDetailLevel eTrcDetailLevel = DllIf_IpcTrcServer_s_eTrcMthDetailLevel;

    ZS::System::CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ ZS::System::EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ "CIpcTrcServer",
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "dtor",
        /* strMthInArgs       */ "" );

} // dtor


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
