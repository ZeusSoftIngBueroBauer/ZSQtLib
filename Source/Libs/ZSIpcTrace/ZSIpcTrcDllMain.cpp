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
static int          DllIf_IpcTrcServer_s_iTrcMthDetailLevel = ETraceDetailLevelNone;

static QCoreApplication* DllIf_s_pQtAppCreatedByDllIf = nullptr;

static DllIf::CIpcTrcServer* DllIf_IpcTrcServer_s_pTheInst = nullptr;
static int DllIf_IpcTrcServer_s_iTrcServerRefCount = 0;
static DllIf::CIpcTrcServerThread* DllIf_s_pIpcTrcServerThread = nullptr;

const QString c_strNameSpace = "ZS::Trace::DllIf";
const QString c_strClassName = "DllMain";

/*==============================================================================
DllIf::CTrcAdminObj
==============================================================================*/

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
                char* szName = new char[stdstrName.size() + 1];
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
                char* szName = new char[stdstrName.size() + 1];
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
                char* szName = new char[stdstrName.size() + 1];
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
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = i_szThreadName;
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
                char* szName = new char[stdstrName.size() + 1];
                memcpy(szName, stdstrName.c_str(), stdstrName.size());
                szName[stdstrName.size()] = 0x00;
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return szName;

} // TrcAdminObj_getObjectThreadName

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
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = bool2Str(i_bEnabled);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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

} // TrcAdminObj_getTraceDetailLevel

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcAdminObj_setTraceDetailLevel(
    DllIf::CTrcAdminObj* i_pTrcAdminObj,
    int                  i_iDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcAdminObj != nullptr )
    {
        QString strKeyInTree = i_pTrcAdminObj->keyInTree();

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = QString::number(i_iDetailLevel);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strKeyInTree,
            /* strMethod          */ "TrcAdminObj_setTraceDetailLevel",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            CIdxTreeTrcAdminObjs* pIdxTree = pTrcServer->getTraceAdminObjIdxTree();

            CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(strKeyInTree);

            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcAdminObj->setTraceDetailLevel(i_iDetailLevel);
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

} // TrcAdminObj_setTraceDetailLevel

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcAdminObj_getTraceDetailLevel( const DllIf::CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    int iDetailLevel = ETraceDetailLevelNone;

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
                iDetailLevel = pTrcAdminObj->getTraceDetailLevel();
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return iDetailLevel;

} // TrcAdminObj_getTraceDetailLevel

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcAdminObj_isActive(
    const DllIf::CTrcAdminObj* i_pTrcAdminObj,
    int                        i_iDetailLevel )
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
                bIsActive = pTrcAdminObj->isActive(i_iDetailLevel);
            }
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcAdminObj != nullptr )

    return bIsActive;

} // TrcAdminObj_isActive

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
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "ObjName: " + QString(i_szObjName);
            strMthInArgs += ", Method: " + QString(i_szMethod);
            strMthInArgs += ", InArgs: " + QString(i_szMethodInArgs);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "ObjName: " + QString(i_szObjName);
            strMthInArgs += ", Method: " + QString(i_szMethod);
            strMthInArgs += ", Return: " + QString(i_szMethodReturn);
            strMthInArgs += ", OutArgs: " + QString(i_szMethodOutArgs);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "ObjName: " + QString(i_szObjName);
            strMthInArgs += ", Method: " + QString(i_szMethod);
            strMthInArgs += ", AddInfo: " + QString(i_szMethodAddInfo);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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

static QHash<QString, DllIf::CTrcAdminObj*> DllIf_TrcServer_s_hshpTrcAdminObjs;
static QHash<QString, int> DllIf_TrcServer_s_hshiTrcAdminObjsRefCounts;

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::CTrcAdminObj* TrcServer_GetTraceAdminObj(
    const char* i_szNameSpace,
    const char* i_szClassName,
    const char* i_szObjName,
    EEnabled    i_bEnabledAsDefault,
    int         i_iDefaultDetailLevel )
//------------------------------------------------------------------------------
{
    #ifdef _WINDOWS
    if( s_iDLL_PROCESS_ATTACH <= 0 ) // Dll already unloaded
    {
        return NULL;
    }
    #endif

    QMutexLocker mtxLocker(&DllIf_s_mtx);

    DllIf::CTrcAdminObj* pDllIfTrcAdminObj = nullptr;

    QString strNameSpace  = i_szNameSpace;
    QString strClassName  = i_szClassName;
    QString strObjName    = i_szObjName;

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

    QString strMthInArgs;

    if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "NameSpace: " + QString(i_szNameSpace);
        strMthInArgs += ", ClassName: " + QString(i_szClassName);
        strMthInArgs += ", ObjName: " + QString(i_szObjName);
        strMthInArgs += ", DefEnabled: " + CEnumEnabled::toString(i_bEnabledAsDefault);
        strMthInArgs += ", DefLevel: " + QString::number(i_iDefaultDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* iTrcDetailLevel    */ iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_GetTraceAdminObj",
        /* strMthInArgs       */ strMthInArgs );

    CTrcServer* pTrcServer = CTrcServer::GetInstance();

    if( pTrcServer != nullptr )
    {
        CTrcAdminObj* pTrcAdminObj = CTrcServer::GetTraceAdminObj(
            /* strNameSpace        */ strNameSpace,
            /* strClassName        */ strClassName,
            /* strObjName          */ strObjName,
            /* bEnabledAsDefault   */ i_bEnabledAsDefault,
            /* iDefaultDetailLevel */ i_iDefaultDetailLevel );

        if( pTrcAdminObj != nullptr )
        {
            QString strKeyInTree = pTrcAdminObj->keyInTree();

            pDllIfTrcAdminObj = DllIf_TrcServer_s_hshpTrcAdminObjs.value(strKeyInTree, nullptr);

            int iRefCount = DllIf_TrcServer_s_hshiTrcAdminObjsRefCounts.value(strKeyInTree, 0);

            if( pDllIfTrcAdminObj == nullptr )
            {
                pDllIfTrcAdminObj = new DllIf::CTrcAdminObj(strKeyInTree.toUtf8());
                DllIf_TrcServer_s_hshpTrcAdminObjs[strKeyInTree] = pDllIfTrcAdminObj;
            }

            ++iRefCount;

            DllIf_TrcServer_s_hshiTrcAdminObjsRefCounts[strKeyInTree] = iRefCount;
        }
    } // if( pTrcServer != nullptr )

    return pDllIfTrcAdminObj;

} // TrcServer_GetTraceAdminObj

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

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

    QString strMthInArgs;

    if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_pTrcAdminObj == nullptr ? "null" : i_pTrcAdminObj->keyInTree();
    }

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* iTrcDetailLevel    */ iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "TrcServer_ReleaseTraceAdminObj",
        /* strMthInArgs       */ strMthInArgs );

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
                CTrcServer::ReleaseTraceAdminObj(pTrcAdminObj);
            }
        } // if( pTrcServer != nullptr )

        int iRefCount = DllIf_TrcServer_s_hshiTrcAdminObjsRefCounts.value(strKeyInTree, 0);

        --iRefCount;

        DllIf_TrcServer_s_hshiTrcAdminObjsRefCounts[strKeyInTree] = iRefCount;

        if( iRefCount <= 0 )
        {
            if( DllIf_TrcServer_s_hshpTrcAdminObjs.contains(strKeyInTree) )
            {
                DllIf_TrcServer_s_hshpTrcAdminObjs.remove(strKeyInTree);
            }

            delete i_pTrcAdminObj;
            i_pTrcAdminObj = nullptr;

            DllIf_TrcServer_s_hshiTrcAdminObjsRefCounts.remove(strKeyInTree);
        }
    } // if( i_pTrcAdminObj != nullptr )

} // TrcServer_ReleaseTraceAdminObj

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_SetOrganizationName( const char* i_szName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    QCoreApplication::setOrganizationName(i_szName);

} // TrcServer_SetOrganizationName

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

} // TrcServer_GetOrganizationName

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_SetApplicationName( const char* i_szName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);
    QCoreApplication::setApplicationName(i_szName);

} // TrcServer_SetApplicationName

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

} // TrcServer_GetApplicationName

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_GetDefaultAdminObjFileAbsoluteFilePath( const char* i_szIniFileScope )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    QString strIniFileScope = i_szIniFileScope;
    std::string stdstrAbsFilePath = CTrcServer::GetDefaultAdminObjFileAbsoluteFilePath(strIniFileScope).toStdString();
    char* szAbsFilePath = new char[stdstrAbsFilePath.size() + 1];
    memcpy(szAbsFilePath, stdstrAbsFilePath.c_str(), stdstrAbsFilePath.size());
    szAbsFilePath[stdstrAbsFilePath.size()] = 0x00;
    return szAbsFilePath;

} // TrcServer_GetDefaultAdminObjFileAbsoluteFilePath

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_GetDefaultLocalTrcFileAbsoluteFilePath( const char* i_szIniFileScope )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    QString strIniFileScope = i_szIniFileScope;
    std::string stdstrAbsFilePath = CTrcServer::GetDefaultLocalTrcFileAbsoluteFilePath(strIniFileScope).toStdString();
    char* szAbsFilePath = new char[stdstrAbsFilePath.size() + 1];
    memcpy(szAbsFilePath, stdstrAbsFilePath.c_str(), stdstrAbsFilePath.size());
    szAbsFilePath[stdstrAbsFilePath.size()] = 0x00;
    return szAbsFilePath;

} // TrcServer_GetDefaultLocalTrcFileAbsoluteFilePath

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_RegisterCurrentThread( const char* i_szThreadName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcServer::RegisterCurrentThread(i_szThreadName);

} // TrcServer_RegisterCurrentThread

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_UnregisterCurrentThread()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcServer::UnregisterCurrentThread();

} // TrcServer_UnregisterCurrentThread

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

} // TrcServer_GetCurrentThreadName

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_isActive( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bIsActive = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            bIsActive = pTrcServer->isActive();
        }
    } // if( i_pTrcServer != nullptr )

    return bIsActive;

} // TrcServer_isActive

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setEnabled( const DllIf::CTrcServer* i_pTrcServer, bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = bool2Str(i_bEnabled);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setEnabled

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_isEnabled( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bEnabled = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            bEnabled = pTrcServer->isEnabled();
        }
    } // if( i_pTrcServer != nullptr )

    return bEnabled;

} // TrcServer_isEnabled

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setNewTrcAdminObjsEnabledAsDefault( const DllIf::CTrcServer* i_pTrcServer, bool i_bEnabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = bool2Str(i_bEnabled);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setNewTrcAdminObjsEnabledAsDefault

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_areNewTrcAdminObjsEnabledAsDefault( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bEnabled = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            bEnabled = pTrcServer->areNewTrcAdminObjsEnabledAsDefault();
        }
    } // if( i_pTrcServer != nullptr )

    return bEnabled;

} // TrcServer_areNewTrcAdminObjsEnabledAsDefault

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setNewTrcAdminObjsDefaultDetailLevel( const DllIf::CTrcServer* i_pTrcServer, int i_iDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = QString::number(i_iDetailLevel);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ "ZSTrcServer",
            /* strMethod          */ "TrcServer_setNewTrcAdminObjsDefaultDetailLevel",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            pTrcServer->setNewTrcAdminObjsDefaultDetailLevel(i_iDetailLevel);
        }
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setNewTrcAdminObjsDefaultDetailLevel

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcServer_getNewTrcAdminObjsDefaultDetailLevel( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    int iDetailLevel = ETraceDetailLevelNone;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            iDetailLevel = pTrcServer->getNewTrcAdminObjsDefaultDetailLevel();
        }
    } // if( i_pTrcServer != nullptr )

    return iDetailLevel;

} // TrcServer_getNewTrcAdminObjsDefaultDetailLevel

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setAdminObjFileAbsoluteFilePath( const DllIf::CTrcServer* i_pTrcServer, const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = i_szAbsFilePath;
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ "ZSTrcServer",
            /* strMethod          */ "TrcServer_setAdminObjFileAbsoluteFilePath",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            pTrcServer->setAdminObjFileAbsoluteFilePath(i_szAbsFilePath);
        }
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setAdminObjFileAbsoluteFilePath

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_getAdminObjFileAbsoluteFilePath( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    char* szAbsFilePath = nullptr;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            std::string stdstrAbsFilePath = pTrcServer->getAdminObjFileAbsoluteFilePath().toStdString();
            szAbsFilePath = new char[stdstrAbsFilePath.size() + 1];
            memcpy(szAbsFilePath, stdstrAbsFilePath.c_str(), stdstrAbsFilePath.size());
            szAbsFilePath[stdstrAbsFilePath.size()] = 0x00;
        }
    } // if( i_pTrcServer != nullptr )

    return szAbsFilePath;

} // TrcServer_getAdminObjFileAbsoluteFilePath

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_recallAdminObjs( DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bOk = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcServer != nullptr )

    return bOk;

} // TrcServer_recallAdminObjs

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_saveAdminObjs( DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bOk = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
        } // if( pTrcServer != nullptr )
    } // if( i_pTrcServer != nullptr )

    return bOk;

} // TrcServer_saveAdminObjs

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setUseLocalTrcFile( const DllIf::CTrcServer* i_pTrcServer, bool i_bUse )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = bool2Str(i_bUse);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setUseLocalTrcFile

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_isLocalTrcFileUsed( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bUsed = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            bUsed = pTrcServer->isLocalTrcFileUsed();
        }
    } // if( i_pTrcServer != nullptr )

    return bUsed;

} // TrcServer_isLocalTrcFileUsed

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setLocalTrcFileAbsoluteFilePath( const DllIf::CTrcServer* i_pTrcServer, const char* i_szAbsFilePath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = i_szAbsFilePath;
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ "ZSTrcServer",
            /* strMethod          */ "TrcServer_setLocalTrcFileAbsoluteFilePath",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            pTrcServer->setLocalTrcFileAbsoluteFilePath(i_szAbsFilePath);
        }
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setLocalTrcFileAbsoluteFilePath

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_getLocalTrcFileAbsoluteFilePath( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    char* szAbsFilePath = nullptr;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            std::string stdstrAbsFilePath = pTrcServer->getLocalTrcFileAbsoluteFilePath().toStdString();
            szAbsFilePath = new char[stdstrAbsFilePath.size() + 1];
            memcpy(szAbsFilePath, stdstrAbsFilePath.c_str(), stdstrAbsFilePath.size());
            szAbsFilePath[stdstrAbsFilePath.size()] = 0x00;
        }
    } // if( i_pTrcServer != nullptr )

    return szAbsFilePath;

} // TrcServer_getLocalTrcFileAbsoluteFilePath

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_getLocalTrcFileCompleteBaseName( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    char* szName = nullptr;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            std::string stdstrName = pTrcServer->getLocalTrcFileCompleteBaseName().toStdString();
            szName = new char[stdstrName.size() + 1];
            memcpy(szName, stdstrName.c_str(), stdstrName.size());
            szName[stdstrName.size()] = 0x00;
        }
    } // if( i_pTrcServer != nullptr )

    return szName;

} // TrcServer_getLocalTrcFileCompleteBaseName

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API char* TrcServer_getLocalTrcFileAbsolutePath( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    char* szAbsFilePath = nullptr;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            std::string stdstrAbsFilePath = pTrcServer->getLocalTrcFileAbsolutePath().toStdString();
            szAbsFilePath = new char[stdstrAbsFilePath.size() + 1];
            memcpy(szAbsFilePath, stdstrAbsFilePath.c_str(), stdstrAbsFilePath.size());
            szAbsFilePath[stdstrAbsFilePath.size()] = 0x00;
        }
    } // if( i_pTrcServer != nullptr )

    return szAbsFilePath;

} // TrcServer_getLocalTrcFileAbsolutePath

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcServer_isLocalTrcFileActive( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bActive = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            bActive = pTrcServer->isLocalTrcFileActive();
        }
    } // if( i_pTrcServer != nullptr )

    return bActive;

} // TrcServer_isLocalTrcFileActive

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setLocalTrcFileAutoSaveIntervalInMs( const DllIf::CTrcServer* i_pTrcServer, int i_iAutoSaveInterval_ms )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = QString::number(i_iAutoSaveInterval_ms);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setLocalTrcFileAutoSaveIntervalInMs

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcServer_getLocalTrcFileAutoSaveIntervalInMs( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    int iAutoSaveInterval_ms = 0;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            iAutoSaveInterval_ms = pTrcServer->getLocalTrcFileAutoSaveIntervalInMs();
        }
    } // if( i_pTrcServer != nullptr )

    return iAutoSaveInterval_ms;

} // TrcServer_getLocalTrcFileAutoSaveIntervalInMs

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setLocalTrcFileCloseFileAfterEachWrite( const DllIf::CTrcServer* i_pTrcServer, bool i_bCloseFile )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = bool2Str(i_bCloseFile);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setLocalTrcFileCloseFileAfterEachWrite

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_getLocalTrcFileCloseFileAfterEachWrite( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bCloseFile = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            bCloseFile = pTrcServer->getLocalTrcFileCloseFileAfterEachWrite();
        }
    } // if( i_pTrcServer != nullptr )

    return bCloseFile;

} // TrcServer_getLocalTrcFileCloseFileAfterEachWrite

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setLocalTrcFileSubFileCountMax( const DllIf::CTrcServer* i_pTrcServer, bool i_iCountMax )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = QString::number(i_iCountMax);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setLocalTrcFileSubFileCountMax

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcServer_getLocalTrcFileSubFileCountMax( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    int iCountMax = 0;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            iCountMax = pTrcServer->getLocalTrcFileSubFileCountMax();
        }
    } // if( i_pTrcServer != nullptr )

    return iCountMax;

} // TrcServer_getLocalTrcFileSubFileCountMax

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setLocalTrcFileSubFileLineCountMax( const DllIf::CTrcServer* i_pTrcServer, int i_iCountMax )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = QString::number(i_iCountMax);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setLocalTrcFileSubFileLineCountMax

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcServer_getLocalTrcFileSubFileLineCountMax( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    int iCountMax = 0;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            iCountMax = pTrcServer->getLocalTrcFileSubFileLineCountMax();
        }
    } // if( i_pTrcServer != nullptr )

    return iCountMax;

} // TrcServer_getLocalTrcFileSubFileLineCountMax

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setUseIpcServer( const DllIf::CTrcServer* i_pTrcServer, bool i_bUseIpcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = bool2Str(i_bUseIpcServer);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setUseIpcServer

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_isIpcServerUsed( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bUseIpcServer = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            bUseIpcServer = pTrcServer->isIpcServerUsed();
        }
    } // if( i_pTrcServer != nullptr )

    return bUseIpcServer;

} // TrcServer_isIpcServerUsed

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setCacheTrcDataIfNotConnected( const DllIf::CTrcServer* i_pTrcServer, bool i_bCacheData )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = bool2Str(i_bCacheData);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setCacheTrcDataIfNotConnected

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_getCacheTrcDataIfNotConnected( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bCacheData = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            bCacheData = pTrcServer->getCacheTrcDataIfNotConnected();
        }
    } // if( i_pTrcServer != nullptr )

    return bCacheData;

} // TrcServer_getCacheTrcDataIfNotConnected

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_setCacheTrcDataMaxArrLen( const DllIf::CTrcServer* i_pTrcServer, int i_iMaxArrLen )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = QString::number(i_iMaxArrLen);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    } // if( i_pTrcServer != nullptr )

} // TrcServer_setCacheTrcDataMaxArrLen

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API int TrcServer_getCacheTrcDataMaxArrLen( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    int iMaxArrLen = 0;

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            iMaxArrLen = pTrcServer->getCacheTrcDataMaxArrLen();
        }
    } // if( i_pTrcServer != nullptr )

    return iMaxArrLen;

} // TrcServer_getCacheTrcDataMaxArrLen

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API bool TrcServer_setTraceSettings(
    DllIf::CTrcServer*               i_pTrcServer,
    const DllIf::STrcServerSettings& i_settings )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    bool bOk = false;

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            //strMthInArgs = i_settings.toString();
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ "ZSTrcServer",
            /* strMethod          */ "TrcServer_setTraceSettings",
            /* strMthInArgs       */ strMthInArgs );

        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            ZS::Trace::STrcServerSettings trcSettings;

            trcSettings.m_bEnabled                              = i_settings.m_bEnabled;
            trcSettings.m_strAdminObjFileAbsFilePath            = i_settings.m_szAdminObjFileAbsFilePath;
            trcSettings.m_bNewTrcAdminObjsEnabledAsDefault      = i_settings.m_bNewTrcAdminObjsEnabledAsDefault;
            trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel    = i_settings.m_iNewTrcAdminObjsDefaultDetailLevel;
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

        } // if( pTrcServer != nullptr )
    } // if( i_pTrcServer != nullptr )

    return bOk;

} // TrcServer_setTraceSettings

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::STrcServerSettings TrcServer_getTraceSettings( const DllIf::CTrcServer* i_pTrcServer )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    DllIf::STrcServerSettings dllIfTrcSettings;

    DllIf::STrcServerSettings_init(dllIfTrcSettings);

    if( i_pTrcServer != nullptr )
    {
        CTrcServer* pTrcServer = CTrcServer::GetInstance();

        if( pTrcServer != nullptr )
        {
            int iStrLen;

            ZS::Trace::STrcServerSettings trcSettings = pTrcServer->getTraceSettings();

            dllIfTrcSettings.m_bEnabled                              = trcSettings.m_bEnabled;
            dllIfTrcSettings.m_bNewTrcAdminObjsEnabledAsDefault      = trcSettings.m_bNewTrcAdminObjsEnabledAsDefault;
            dllIfTrcSettings.m_iNewTrcAdminObjsDefaultDetailLevel    = trcSettings.m_iNewTrcAdminObjsDefaultDetailLevel;
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

        } // if( pTrcServer != nullptr )
    } // if( i_pTrcServer != nullptr )

    return dllIfTrcSettings;

} // TrcServer_getTraceSettings

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API void TrcServer_clearLocalTrcFile( const DllIf::CTrcServer* i_pTrcServer)
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    if( i_pTrcServer != nullptr )
    {
        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    } // if( i_pTrcServer != nullptr )

} // TrcServer_clearLocalTrcFile

/*==============================================================================
DllIf::CIpcTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::CIpcTrcServer* IpcTrcServer_GetInstance()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* iTrcDetailLevel    */ iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ c_strClassName,
        /* strObjName         */ "ZSTrcServer",
        /* strMethod          */ "IpcTrcServer_GetInstance",
        /* strMthInArgs       */ strMthInArgs );

    return DllIf_IpcTrcServer_s_pTheInst;

} // IpcTrcServer_GetInstance

//------------------------------------------------------------------------------
ZSIPCTRACEDLL_EXTERN_API DllIf::CIpcTrcServer* IpcTrcServer_CreateInstance( int i_iTrcDetailLevel )
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
            QThread::currentThread()->setObjectName("QtAppThread");
        }
    }

    QString strAdminObjFileAbsFilePath;
    QString strLocalTrcFileAbsFilePath;

    if( DllIf_IpcTrcServer_s_pTrcMthFile == nullptr )
    {
        strAdminObjFileAbsFilePath = CTrcServer::GetDefaultAdminObjFileAbsoluteFilePath();
        strLocalTrcFileAbsFilePath = CTrcServer::GetDefaultLocalTrcFileAbsoluteFilePath();

        DllIf_IpcTrcServer_s_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);
        DllIf_IpcTrcServer_s_iTrcMthDetailLevel = i_iTrcDetailLevel;
    }

    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* iTrcDetailLevel    */ iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
        // with an event loop. If the application does not use Qt it is anyway necessar
        // to create the tracer server in a thread with an event loop.
        DllIf::CIpcTrcServerThread* pTrcServerThread = DllIf_s_pIpcTrcServerThread;
        if( pTrcServerThread == nullptr )
        {
            DllIf_s_pIpcTrcServerThread = new DllIf::CIpcTrcServerThread(i_iTrcDetailLevel);
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

        pDllIfTrcServer = new DllIf::CIpcTrcServer(i_iTrcDetailLevel);
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
        int iRefCount = 0;

        {
            CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
            int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

            QString strMthInArgs;

            CMethodTracer mthTracer(
                /* pTrcMthFile        */ pTrcMthFile,
                /* iTrcDetailLevel    */ iTrcDetailLevel,
                /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
                /* strNameSpace       */ c_strNameSpace,
                /* strClassName       */ c_strClassName,
                /* strObjName         */ "ZSTrcServer",
                /* strMethod          */ "IpcTrcServer_ReleaseInstance",
                /* strMthInArgs       */ strMthInArgs );

            iRefCount = DllIf_IpcTrcServer_s_iTrcServerRefCount - 1;

            if( iRefCount > 0 /* && pDllIfTrcServer != nullptr */ ) // pDllIfTrcServer must not be nullptr. Otherwise ... crash.
            {
                CIpcTrcServer::ReleaseInstance();

                DllIf_IpcTrcServer_s_iTrcServerRefCount = iRefCount;
            }
            // Trace server is no longer referenced ...
            else // if( iRefCount == 0 )
            {
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

            } // if( iRefCount == 0 )

            // dtor of mthTracer here which access the trace method file.
            // The trace method file got to be removed after this block.
        }

        if( iRefCount == 0 )
        {
            if( DllIf_IpcTrcServer_s_pTrcMthFile != nullptr )
            {
                CTrcMthFile::Free(DllIf_IpcTrcServer_s_pTrcMthFile);

                DllIf_IpcTrcServer_s_pTrcMthFile = nullptr;
                DllIf_IpcTrcServer_s_iTrcMthDetailLevel = 0;
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
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
            strMthInArgs += ", Wait: " + bool2Str(i_bWait);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "Timeout: " + QString::number(i_iTimeout_ms) + " ms";
            strMthInArgs += ", Wait: " + bool2Str(i_bWait);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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

} // IpcTrcServer_isListening

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

} // IpcTrcServer_isConnected

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
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "Port: " + QString::number(i_uPort);
            strMthInArgs += ", Timeout: " + QString::number(i_iTimeout_ms) + " ms";
            strMthInArgs += ", Wait: " + bool2Str(i_bWait);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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

} // IpcTrcServer_getPort


/*******************************************************************************
class DllIf::CTrcAdminObj
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor (declared public but for internal use only, implemented in ZSIpcTrace::ZSIpcTrcDllMain)
==============================================================================*/

//------------------------------------------------------------------------------
DllIf::CTrcAdminObj::CTrcAdminObj( const char* i_szKeyInTree ) :
//------------------------------------------------------------------------------
    m_szKeyInTree(nullptr)
{
    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

    Trace::CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* iTrcDetailLevel    */ iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ "CTrcAdminObj",
        /* strObjName         */ i_szKeyInTree,
        /* strMethod          */ "ctor",
        /* strMthInArgs       */ "" );

    size_t iStrLen = strlen(i_szKeyInTree);
    m_szKeyInTree = new char[iStrLen+1];
    memset(m_szKeyInTree, 0x00, iStrLen+1);
    memcpy(m_szKeyInTree, i_szKeyInTree, iStrLen);

} // ctor

//------------------------------------------------------------------------------
DllIf::CTrcAdminObj::~CTrcAdminObj()
//------------------------------------------------------------------------------
{
    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

    Trace::CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* iTrcDetailLevel    */ iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ c_strNameSpace,
        /* strClassName       */ "CTrcAdminObj",
        /* strObjName         */ m_szKeyInTree,
        /* strMethod          */ "dtor",
        /* strMthInArgs       */ "" );

    delete m_szKeyInTree;
    m_szKeyInTree = nullptr;

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
    int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

    Trace::CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* iTrcDetailLevel    */ iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

    Trace::CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* iTrcDetailLevel    */ iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
DllIf::CIpcTrcServer::CIpcTrcServer(int /*i_iTrcDetailLevel*/ ) :
//------------------------------------------------------------------------------
    CTrcServer()
{
    CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_pTrcMthFile;
    int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

    Trace::CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* iTrcDetailLevel    */ iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
    int          iTrcDetailLevel = DllIf_IpcTrcServer_s_iTrcMthDetailLevel;

    Trace::CMethodTracer mthTracer(
        /* pTrcMthFile        */ pTrcMthFile,
        /* iTrcDetailLevel    */ iTrcDetailLevel,
        /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
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
