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

//#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <QtCore/qcoreapplication.h>
#include <QtCore/qthread.h>

#include "ZSIpcTracePy/ZSIpcTrcPyDllMain.h"
#include "ZSIpcTrace/ZSIpcTrcDllIf.h"
#include "ZSIpcTrace/ZSIpcTrcDllIfServerThread.h"
#include "ZSIpcTrace/ZSIpcTrcServer.h"
#include "ZSIpc/ZSIpcSrvCltMsg.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysRequest.h"
#include "ZSSys/ZSSysVersion.h"

//#include <mutex>

#ifdef _WIN32
// As "min" will be defined as a macro with two arguments and qdatetime uses "min"
// as a function with no arguments "windows.h" must be included after qdatetime
// (which is included by ZSSysTime (which again is included by ZSSysAux)).
//#include <windows.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Ipc;
using namespace ZS::Trace;


/*******************************************************************************
Exported Dll methods
*******************************************************************************/

static PyObject* TrcAdminObj_setObjectThreadName( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* TrcAdminObj_getObjectThreadName( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* TrcAdminObj_setEnabled( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* TrcAdminObj_isEnabled( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* TrcAdminObj_setTraceDetailLevel( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* TrcAdminObj_getTraceDetailLevel( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* TrcAdminObj_isActive( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* TrcAdminObj_traceMethodEnter( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* TrcAdminObj_traceMethodLeave( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* TrcAdminObj_traceMethod( PyObject* i_pModule, PyObject* i_pArgs );

static PyObject* IpcTrcServer_registerThreadName( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_unregisterThreadName( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setOrganizationName( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getOrganizationName( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setApplicationName( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getApplicationName( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getDefaultFilePaths( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getInstance( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_createInstance( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_destroyInstance( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getTraceAdminObj( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_releaseTraceAdminObj( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_isActive( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setEnabled( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_isEnabled( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setNewTrcAdminObjsEnabledAsDefault( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_areNewTrcAdminObjsEnabledAsDefault( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setNewTrcAdminObjsDefaultDetailLevel( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getNewTrcAdminObjsDefaultDetailLevel( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setAdminObjFileAbsoluteFilePath( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getAdminObjFileAbsoluteFilePath( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_recallAdminObjs( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_saveAdminObjs( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setUseLocalTrcFile( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_isLocalTrcFileUsed( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setLocalTrcFileAbsoluteFilePath( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getLocalTrcFileAbsoluteFilePath( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getLocalTrcFileCompleteBaseName( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getLocalTrcFileAbsolutePath( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_isLocalTrcFileActive( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setLocalTrcFileAutoSaveInterval( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getLocalTrcFileAutoSaveInterval( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setLocalTrcFileCloseFileAfterEachWrite( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getLocalTrcFileCloseFileAfterEachWrite( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setLocalTrcFileSubFileCountMax( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getLocalTrcFileSubFileCountMax( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setLocalTrcFileSubFileLineCountMax( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getLocalTrcFileSubFileLineCountMax( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setCacheTrcDataIfNotConnected( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getCacheTrcDataIfNotConnected( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setCacheTrcDataMaxArrLen( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getCacheTrcDataMaxArrLen( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_clearLocalTrcFile( PyObject* i_pModule, PyObject* i_pArgs );

static PyObject* IpcTrcServer_startup( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_shutdown( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_isListening( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_isConnected( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_setPort( PyObject* i_pModule, PyObject* i_pArgs );
static PyObject* IpcTrcServer_getPort( PyObject* i_pModule, PyObject* i_pArgs );


/*******************************************************************************
Module's method table and Initialization Function

@see https://docs.python.org/3/extending/newtypes.html

*******************************************************************************/

//------------------------------------------------------------------------------
static PyMethodDef s_ipcTraceMethods[] = {
//------------------------------------------------------------------------------
 // { name,     meth,        flags,         doc

    { "TrcAdminObj_setObjectThreadName", TrcAdminObj_setObjectThreadName, METH_VARARGS, "__TODO__" },
    { "TrcAdminObj_getObjectThreadName", TrcAdminObj_getObjectThreadName, METH_VARARGS, "__TODO__" },
    { "TrcAdminObj_setEnabled", TrcAdminObj_setEnabled, METH_VARARGS, "__TODO__" },
    { "TrcAdminObj_isEnabled", TrcAdminObj_isEnabled, METH_VARARGS, "__TODO__" },
    { "TrcAdminObj_setTraceDetailLevel", TrcAdminObj_setTraceDetailLevel, METH_VARARGS, "__TODO__" },
    { "TrcAdminObj_getTraceDetailLevel", TrcAdminObj_getTraceDetailLevel, METH_VARARGS, "__TODO__" },
    { "TrcAdminObj_isActive", TrcAdminObj_isActive, METH_VARARGS, "__TODO__" },
    { "TrcAdminObj_traceMethodEnter", TrcAdminObj_traceMethodEnter, METH_VARARGS, "__TODO__" },
    { "TrcAdminObj_traceMethodLeave", TrcAdminObj_traceMethodLeave, METH_VARARGS, "__TODO__" },
    { "TrcAdminObj_traceMethod", TrcAdminObj_traceMethod, METH_VARARGS, "__TODO__" },

    { "IpcTrcServer_registerThreadName", IpcTrcServer_registerThreadName, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_unregisterThreadName", IpcTrcServer_unregisterThreadName, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setOrganizationName", IpcTrcServer_setOrganizationName, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getOrganizationName", IpcTrcServer_getOrganizationName, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setApplicationName", IpcTrcServer_setApplicationName, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getApplicationName", IpcTrcServer_getApplicationName, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getDefaultFilePaths", IpcTrcServer_getDefaultFilePaths, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getInstance", IpcTrcServer_getInstance, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_createInstance", IpcTrcServer_createInstance, METH_VARARGS, "Creates the Ipc Tracer Server Instance." },
    { "IpcTrcServer_destroyInstance", IpcTrcServer_destroyInstance, METH_VARARGS, "Destroys the Ipc Tracer Server Instance." },
    { "IpcTrcServer_getTraceAdminObj", IpcTrcServer_getTraceAdminObj, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_releaseTraceAdminObj", IpcTrcServer_releaseTraceAdminObj, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_isActive", IpcTrcServer_isActive, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setEnabled", IpcTrcServer_setEnabled, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_isEnabled", IpcTrcServer_isEnabled, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setNewTrcAdminObjsEnabledAsDefault", IpcTrcServer_setNewTrcAdminObjsEnabledAsDefault, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_areNewTrcAdminObjsEnabledAsDefault", IpcTrcServer_areNewTrcAdminObjsEnabledAsDefault, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setNewTrcAdminObjsDefaultDetailLevel", IpcTrcServer_setNewTrcAdminObjsDefaultDetailLevel, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getNewTrcAdminObjsDefaultDetailLevel", IpcTrcServer_getNewTrcAdminObjsDefaultDetailLevel, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setAdminObjFileAbsoluteFilePath", IpcTrcServer_setAdminObjFileAbsoluteFilePath, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getAdminObjFileAbsoluteFilePath", IpcTrcServer_getAdminObjFileAbsoluteFilePath, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_recallAdminObjs", IpcTrcServer_recallAdminObjs, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_saveAdminObjs", IpcTrcServer_saveAdminObjs, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setUseLocalTrcFile", IpcTrcServer_setUseLocalTrcFile, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_isLocalTrcFileUsed", IpcTrcServer_isLocalTrcFileUsed, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setLocalTrcFileAbsoluteFilePath", IpcTrcServer_setLocalTrcFileAbsoluteFilePath, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getLocalTrcFileAbsoluteFilePath", IpcTrcServer_getLocalTrcFileAbsoluteFilePath, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getLocalTrcFileCompleteBaseName", IpcTrcServer_getLocalTrcFileCompleteBaseName, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getLocalTrcFileAbsolutePath", IpcTrcServer_getLocalTrcFileAbsolutePath, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_isLocalTrcFileActive", IpcTrcServer_isLocalTrcFileActive, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setLocalTrcFileAutoSaveInterval", IpcTrcServer_setLocalTrcFileAutoSaveInterval, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getLocalTrcFileAutoSaveInterval", IpcTrcServer_getLocalTrcFileAutoSaveInterval, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setLocalTrcFileCloseFileAfterEachWrite", IpcTrcServer_setLocalTrcFileCloseFileAfterEachWrite, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getLocalTrcFileCloseFileAfterEachWrite", IpcTrcServer_getLocalTrcFileCloseFileAfterEachWrite, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setLocalTrcFileSubFileCountMax", IpcTrcServer_setLocalTrcFileSubFileCountMax, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getLocalTrcFileSubFileCountMax", IpcTrcServer_getLocalTrcFileSubFileCountMax, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setLocalTrcFileSubFileLineCountMax", IpcTrcServer_setLocalTrcFileSubFileLineCountMax, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getLocalTrcFileSubFileLineCountMax", IpcTrcServer_getLocalTrcFileSubFileLineCountMax, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setCacheTrcDataIfNotConnected", IpcTrcServer_setCacheTrcDataIfNotConnected, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getCacheTrcDataIfNotConnected", IpcTrcServer_getCacheTrcDataIfNotConnected, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setCacheTrcDataMaxArrLen", IpcTrcServer_setCacheTrcDataMaxArrLen, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getCacheTrcDataMaxArrLen", IpcTrcServer_getCacheTrcDataMaxArrLen, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_clearLocalTrcFile", IpcTrcServer_clearLocalTrcFile, METH_VARARGS, "__TODO__" },

    { "IpcTrcServer_startup", IpcTrcServer_startup, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_shutdown", IpcTrcServer_shutdown, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_isListening", IpcTrcServer_isListening, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_isConnected", IpcTrcServer_isConnected, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_setPort", IpcTrcServer_setPort, METH_VARARGS, "__TODO__" },
    { "IpcTrcServer_getPort", IpcTrcServer_getPort, METH_VARARGS, "__TODO__" },

    // Terminate the array with an object containing nulls.
    { nullptr, nullptr, 0, nullptr }
};

//------------------------------------------------------------------------------
static PyModuleDef s_ipcTraceModule = {
//------------------------------------------------------------------------------
    /* base     */ PyModuleDef_HEAD_INIT,
    /* name     */ "ZSIpcTrace",
    /* doc      */ "Method Tracer",
    /* size     */ 0,
    /* methods  */ s_ipcTraceMethods,
    /* slots    */ NULL,
    /* traverse */ NULL,
    /* clear    */ NULL,
    /* free     */ NULL
};

//------------------------------------------------------------------------------
/*! 

    @note In order to import this extension when running the debug version of the
          python executable the compiled ZSIpcTracePy dll must end with "_d"
          ("ZSIpcTracePy_d.pyd").
*/
PyMODINIT_FUNC PyInit_ZSIpcTracePy()
//------------------------------------------------------------------------------
{
    return PyModule_Create(&s_ipcTraceModule);
}


/*******************************************************************************
public Dll interface methods
*******************************************************************************/

static QMutex DllIf_s_mtx(QMutex::Recursive);

static QCoreApplication* DllIf_s_pQtAppCreatedByDllIf = nullptr;

#ifdef _TRACE_IPCTRACEPYDLL_METHODs
static QHash<QString, CTrcMthFile*> DllIf_IpcTrcServer_s_hshpTrcMthFiles;
static QHash<QString, int>          DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels;
#endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs
static QHash<QString, bool>         DllIf_IpcTrcServer_s_hshbTrcServerCreated;

static ZS::Trace::DllIf::CIpcTrcServerThread* DllIf_s_pThreadIpcTrcServer = nullptr;


const QString c_strNameSpace  = "ZS::Trace::DllIf";
const QString c_strModuleName = "ZSIpcTrcPyDllMain";

/*==============================================================================
Auxiliary methods
==============================================================================*/

//------------------------------------------------------------------------------
static CTrcAdminObj* getTraceAdminObj( int i_idxInTree, const QString& i_strServerName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    CTrcServer* pTrcServer = CTrcServer::GetInstance(i_strServerName);

    if( pTrcServer != nullptr )
    {
        CIdxTreeTrcAdminObjs*  pIdxTree = pTrcServer->getTraceAdminObjIdxTree();
        CIdxTreeEntry* pTreeEntry = pIdxTree->getEntry(i_idxInTree);

        pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);
    }

    return pTrcAdminObj;

} // getTraceAdminObj

//------------------------------------------------------------------------------
static CTrcAdminObj* getTraceAdminObj( const QString& i_strKeyInTree, const QString& i_strServerName )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    CTrcServer* pTrcServer = CTrcServer::GetInstance(i_strServerName);

    if( pTrcServer != nullptr )
    {
        CIdxTreeTrcAdminObjs*  pIdxTree = pTrcServer->getTraceAdminObjIdxTree();
        CIdxTreeEntry* pTreeEntry = pIdxTree->findEntry(i_strKeyInTree);

        pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);
    }

    return pTrcAdminObj;

} // getTraceAdminObj

/*==============================================================================
DllIf::CTrcAdminObj
==============================================================================*/

//------------------------------------------------------------------------------
static PyObject* TrcAdminObj_setObjectThreadName( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    //int       i_idxInTree;
    const char* i_szKeyInTree;
    const char* i_szThreadName;
    const char* i_szServerName;

    //if( PyArg_ParseTuple(i_pArgs, "iss", &i_idxInTree, &i_szThreadName, &i_szServerName) )
    if( PyArg_ParseTuple(i_pArgs, "sss", &i_szKeyInTree, &i_szThreadName, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strKeyInTree  = i_szKeyInTree;
        QString strThreadName = i_szThreadName;
        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "IdxInTree: " + QString::number(i_idxInTree);
            strMthInArgs += ", ThreadName: " + strThreadName;
            strMthInArgs += ", ServerName: " + strServerName;
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strModuleName,
            /* strObjName         */ "",
            /* strMethod          */ "TrcAdminObj_setObjectThreadName",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        //CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(i_idxInTree, strServerName);
        CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(strKeyInTree, strServerName);

        if( pTrcAdminObj != nullptr )
        {
            #ifdef _TRACE_IPCTRACEPYDLL_METHODs
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("-> " + pTrcAdminObj->keyInTree() + ".setObjectThreadName(" + strThreadName + ")");
            }
            #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

            pTrcAdminObj->setObjectThreadName(strThreadName);

            #ifdef _TRACE_IPCTRACEPYDLL_METHODs
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("<- " + pTrcAdminObj->keyInTree() + ".setObjectThreadName()");
            }
            #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        } // if( pTrcAdminObj != nullptr )
    } // if( PyArg_ParseTuple(i_pArgs, "iss", &i_idxInTree, &i_szThreadName, &i_szServerName) )

    return Py_BuildValue("");

} // TrcAdminObj_setObjectThreadName

//------------------------------------------------------------------------------
static PyObject* TrcAdminObj_getObjectThreadName( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    QString strThreadName;

    //int       i_idxInTree;
    const char* i_szKeyInTree;
    const char* i_szServerName;

    //if( PyArg_ParseTuple(i_pArgs, "is", &i_idxInTree, &i_szServerName) )
    if( PyArg_ParseTuple(i_pArgs, "ss", &i_szKeyInTree, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strKeyInTree  = i_szKeyInTree;
        QString strServerName = i_szServerName;

        //CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(i_idxInTree, strServerName);
        CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(strKeyInTree, strServerName);

        if( pTrcAdminObj != nullptr )
        {
            strThreadName = pTrcAdminObj->getObjectThreadName();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "is", &i_idxInTree, &i_szServerName) )

    return Py_BuildValue("s", strThreadName.toStdString().c_str());

} // TrcAdminObj_getObjectThreadName

//------------------------------------------------------------------------------
static PyObject* TrcAdminObj_setEnabled( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    //int       i_idxInTree;
    const char* i_szKeyInTree;
    bool        i_bEnabled;
    const char* i_szServerName;

    //if( PyArg_ParseTuple(i_pArgs, "ibs", &i_idxInTree, &i_bEnabled, &i_szServerName) )
    if( PyArg_ParseTuple(i_pArgs, "sbs", &i_szKeyInTree, &i_bEnabled, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strKeyInTree  = i_szKeyInTree;
        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "IdxInTree: " + QString::number(i_idxInTree);
            strMthInArgs += ", Enabled: " + bool2Str(i_bEnabled);
            strMthInArgs += ", ServerName: " + strServerName;
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strModuleName,
            /* strObjName         */ "",
            /* strMethod          */ "TrcAdminObj_setEnabled",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        //CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(i_idxInTree, strServerName);
        CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(strKeyInTree, strServerName);

        if( pTrcAdminObj != nullptr )
        {
            #ifdef _TRACE_IPCTRACEPYDLL_METHODs
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("-> " + pTrcAdminObj->keyInTree() + ".setEnabled(" + bool2Str(i_bEnabled) + ")");
            }
            #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

            pTrcAdminObj->setEnabled(i_bEnabled ? EEnabled::Yes : EEnabled::No);

            #ifdef _TRACE_IPCTRACEPYDLL_METHODs
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("<- " + pTrcAdminObj->keyInTree() + ".setEnabled()");
            }
            #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        } // if( pTrcAdminObj != nullptr )
    } // if( PyArg_ParseTuple(i_pArgs, "ibs", &i_idxInTree, &i_bEnabled, &i_szServerName) )

    return Py_BuildValue("");

} // TrcAdminObj_setEnabled

//------------------------------------------------------------------------------
static PyObject* TrcAdminObj_isEnabled( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bEnabled = false;

    //int       i_idxInTree;
    const char* i_szKeyInTree;
    const char* i_szServerName;

    //if( PyArg_ParseTuple(i_pArgs, "is", &i_idxInTree, &i_szServerName) )
    if( PyArg_ParseTuple(i_pArgs, "ss", &i_szKeyInTree, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strKeyInTree  = i_szKeyInTree;
        QString strServerName = i_szServerName;

        //CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(i_idxInTree, strServerName);
        CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(strKeyInTree, strServerName);

        if( pTrcAdminObj != nullptr )
        {
            bEnabled = pTrcAdminObj->isEnabled();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "is", &i_idxInTree, &i_szServerName) )

    return Py_BuildValue("O", bEnabled ? Py_True : Py_False);

} // TrcAdminObj_isEnabled

//------------------------------------------------------------------------------
static PyObject* TrcAdminObj_setTraceDetailLevel( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    //int       i_idxInTree;
    const char* i_szKeyInTree;
    int         i_iDetailLevel;
    const char* i_szServerName;

    //if( PyArg_ParseTuple(i_pArgs, "iis", &i_idxInTree, &i_iDetailLevel, &i_szServerName) )
    if( PyArg_ParseTuple(i_pArgs, "sis", &i_szKeyInTree, &i_iDetailLevel, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strKeyInTree  = i_szKeyInTree;
        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "IdxInTree: " + QString::number(i_idxInTree);
            strMthInArgs += ", DetailLevel: " + traceDetailLevel2Str(i_iDetailLevel);
            strMthInArgs += ", ServerName: " + strServerName;
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strModuleName,
            /* strObjName         */ "",
            /* strMethod          */ "TrcAdminObj_setTraceDetailLevel",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        //CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(i_idxInTree, strServerName);
        CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(strKeyInTree, strServerName);

        if( pTrcAdminObj != nullptr )
        {
            #ifdef _TRACE_IPCTRACEPYDLL_METHODs
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("-> " + pTrcAdminObj->keyInTree() + ".setTraceDetailLevel(" + traceDetailLevel2Str(i_iDetailLevel) + ")");
            }
            #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

            pTrcAdminObj->setTraceDetailLevel(i_iDetailLevel);

            #ifdef _TRACE_IPCTRACEPYDLL_METHODs
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("<- " + pTrcAdminObj->keyInTree() + ".setTraceDetailLevel()");
            }
            #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        } // if( pTrcAdminObj != nullptr )
    } // if( PyArg_ParseTuple(i_pArgs, "ibs", &i_idxInTree, &i_bEnabled, &i_szServerName) )

    return Py_BuildValue("");

} // TrcAdminObj_setTraceDetailLevel

//------------------------------------------------------------------------------
static PyObject* TrcAdminObj_getTraceDetailLevel( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int iDetailLevel = ETraceDetailLevelNone;

    //int       i_idxInTree;
    const char* i_szKeyInTree;
    const char* i_szServerName;

    //if( PyArg_ParseTuple(i_pArgs, "is", &i_idxInTree, &i_szServerName) )
    if( PyArg_ParseTuple(i_pArgs, "ss", &i_szKeyInTree, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strKeyInTree  = i_szKeyInTree;
        QString strServerName = i_szServerName;

        //CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(i_idxInTree, strServerName);
        CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(strKeyInTree, strServerName);

        if( pTrcAdminObj != nullptr )
        {
            iDetailLevel = pTrcAdminObj->getTraceDetailLevel();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "ibs", &i_idxInTree, &i_szServerName) )

    return Py_BuildValue("i", iDetailLevel);

} // TrcAdminObj_getTraceDetailLevel

//------------------------------------------------------------------------------
static PyObject* TrcAdminObj_isActive( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    //int       i_idxInTree;
    const char* i_szKeyInTree;
    int         i_iDetailLevel;
    const char* i_szServerName;

    //if( PyArg_ParseTuple(i_pArgs, "iis", &i_idxInTree, &i_iDetailLevel, &i_szServerName) )
    if( PyArg_ParseTuple(i_pArgs, "sis", &i_szKeyInTree, &i_iDetailLevel, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strKeyInTree  = i_szKeyInTree;
        QString strServerName = i_szServerName;

        //CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(i_idxInTree, strServerName);
        CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(strKeyInTree, strServerName);

        if( pTrcAdminObj != nullptr )
        {
            bIsActive = pTrcAdminObj->isActive(i_iDetailLevel);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "ibs", &i_idxInTree, &i_bEnabled, &i_szServerName) )

    return Py_BuildValue("O", bIsActive ? Py_True : Py_False);

} // TrcAdminObj_isActive

//------------------------------------------------------------------------------
static PyObject* TrcAdminObj_traceMethodEnter( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    //int       i_idxInTree;
    const char* i_szKeyInTree;
    const char* i_szMethodName;
    const char* i_szMethodInArgs;
    const char* i_szObjectName;
    const char* i_szServerName;

    //if( PyArg_ParseTuple(i_pArgs, "issss", &i_idxInTree, &i_szMethodName, &i_szMethodInArgs, &i_szObjectName, &i_szServerName) )
    if( PyArg_ParseTuple(i_pArgs, "sssss", &i_szKeyInTree, &i_szMethodName, &i_szMethodInArgs, &i_szObjectName, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strKeyInTree    = i_szKeyInTree;
        QString strMethodName   = i_szMethodName;
        QString strMethodInArgs = i_szMethodInArgs;
        QString strObjectName   = i_szObjectName;
        QString strServerName   = i_szServerName;

        // !! Please note that this extension method cannot be traced.
        // !! Otherwise the method trace stack counter gets mixed up.
        // !! If instantiating a local method tracer here to trace entering and leaving the extension method
        // !! the stack counter for the current thread would incrementing before invoking "traceMethodEnter".
        // !! Afterwards "traceMethodEnter" is called to trace entering the python script method also
        // !! incrementing the stack counter.
        // !! Before leaving this extension method the local method tracer instance is destroyed
        // !! invoking "traceMethodLeave" for this "TrcAdminObj_traceMethodEnter" extension method
        // !! what messes up the counter.
        // !! This will end up with the following trace output in the trace method file.
        // !!
        // !!   -> TrcAdminObj_traceMethodEnter(..)
        // !!      -> PyObj.methodCall(..)
        // !!      <- TrcAdminObj_traceMethodEnter(..)
        // !!   <- PyObj.methodCall(..)

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            //CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(i_idxInTree, strServerName);
            CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(strKeyInTree, strServerName);

            if( pTrcAdminObj != nullptr )
            {
                if( strObjectName.isEmpty() )
                {
                    pTrcServer->traceMethodEnter(
                        /* pAdminObj    */ pTrcAdminObj,
                        /* strMethod    */ strMethodName,
                        /* strMthInArgs */ strMethodInArgs );
                }
                else // if( !strObjectName.isEmpty() )
                {
                    pTrcServer->traceMethodEnter(
                        /* pAdminObj    */ pTrcAdminObj,
                        /* strObjName   */ strObjectName,
                        /* strMethod    */ strMethodName,
                        /* strMthInArgs */ strMethodInArgs );
                }
            } // if( pTrcAdminObj != nullptr )
        } // if( pTrcServer != nullptr )
    } // if( PyArg_ParseTuple(i_pArgs, "issss", &i_idxInTree, &i_szMethodName, &i_szMethodInArgs, &i_szObjectName, &i_szServerName) )

    return Py_BuildValue("");

} // TrcAdminObj_traceMethodEnter

//------------------------------------------------------------------------------
static PyObject* TrcAdminObj_traceMethodLeave( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    //int       i_idxInTree;
    const char* i_szKeyInTree;
    const char* i_szMethodName;
    const char* i_szMethodReturn;
    const char* i_szMethodOutArgs;
    const char* i_szObjectName;
    const char* i_szServerName;

    //if( PyArg_ParseTuple(i_pArgs, "isssss", &i_idxInTree, &i_szMethodName, &i_szMethodReturn, &i_szMethodOutArgs, &i_szObjectName, &i_szServerName) )
    if( PyArg_ParseTuple(i_pArgs, "ssssss", &i_szKeyInTree, &i_szMethodName, &i_szMethodReturn, &i_szMethodOutArgs, &i_szObjectName, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strKeyInTree     = i_szKeyInTree;
        QString strMethodName    = i_szMethodName;
        QString strMethodReturn  = i_szMethodReturn;
        QString strMethodOutArgs = i_szMethodOutArgs;
        QString strObjectName    = i_szObjectName;
        QString strServerName    = i_szServerName;

        // !! Please note that this extension method cannot be traced (see comments above in "TrcAdminObj_traceMethodEnter").

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            //CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(i_idxInTree, strServerName);
            CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(strKeyInTree, strServerName);

            if( pTrcAdminObj != nullptr )
            {
                if( strObjectName.isEmpty() )
                {
                    pTrcServer->traceMethodLeave(
                        /* pAdminObj    */ pTrcAdminObj,
                        /* strMethod    */ strMethodName,
                        /* strMthRet    */ strMethodReturn,
                        /* strMthInArgs */ strMethodOutArgs );
                }
                else // if( !strObjectName.isEmpty() )
                {
                    pTrcServer->traceMethodLeave(
                        /* pAdminObj    */ pTrcAdminObj,
                        /* strObjName   */ strObjectName,
                        /* strMethod    */ strMethodName,
                        /* strMthRet    */ strMethodReturn,
                        /* strMthInArgs */ strMethodOutArgs );
                }
            } // if( pTrcAdminObj != nullptr )
        } // if( pTrcServer != nullptr )
    } // if( PyArg_ParseTuple(i_pArgs, "isssss", &i_idxInTree, &i_szMethodName, &i_szMethodReturn, &i_szMethodOutArgs, &i_szObjectName, &i_szServerName) )

    return Py_BuildValue("");

} // TrcAdminObj_traceMethodLeave

//------------------------------------------------------------------------------
static PyObject* TrcAdminObj_traceMethod( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    //int       i_idxInTree;
    const char* i_szKeyInTree;
    const char* i_szMethodName;
    const char* i_szMethodAddInfo;
    const char* i_szObjectName;
    const char* i_szServerName;

    //if( PyArg_ParseTuple(i_pArgs, "issss", &i_idxInTree, &i_szMethodName, &i_szMethodAddInfo, &i_szObjectName, &i_szServerName) )
    if( PyArg_ParseTuple(i_pArgs, "sssss", &i_szKeyInTree, &i_szMethodName, &i_szMethodAddInfo, &i_szObjectName, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strKeyInTree     = i_szKeyInTree;
        QString strMethodName    = i_szMethodName;
        QString strMethodAddInfo = i_szMethodAddInfo;
        QString strObjectName    = i_szObjectName;
        QString strServerName    = i_szServerName;

        // !! Please note that this extension method cannot be traced (see comments above in "TrcAdminObj_traceMethodEnter").

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            //CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(i_idxInTree, strServerName);
            CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(strKeyInTree, strServerName);

            if( pTrcAdminObj != nullptr )
            {
                if( strObjectName.isEmpty() )
                {
                    pTrcServer->traceMethod(
                        /* pAdminObj    */ pTrcAdminObj,
                        /* strMethod    */ strMethodName,
                        /* strMthInArgs */ strMethodAddInfo );
                }
                else // if( !strObjectName.isEmpty() )
                {
                    pTrcServer->traceMethod(
                        /* pAdminObj    */ pTrcAdminObj,
                        /* strObjName   */ strObjectName,
                        /* strMethod    */ strMethodName,
                        /* strMthInArgs */ strMethodAddInfo );
                }
            } // if( pTrcAdminObj != nullptr )
        } // if( pTrcServer != nullptr )
    } // if( PyArg_ParseTuple(i_pArgs, "issss", &i_idxInTree, &i_szMethodName, &i_szMethodAddInfo, &i_szObjectName, &i_szServerName) )

    return Py_BuildValue("");

} // TrcAdminObj_traceMethod


/*==============================================================================
DllIf::CIpcTrcServer
==============================================================================*/

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_registerThreadName( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int i_iThreadId;
    const char* i_szThreadName;

    if( PyArg_ParseTuple(i_pArgs, "is", &i_iThreadId, &i_szThreadName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        CTrcServer::RegisterThreadName(i_iThreadId, i_szThreadName);

    } // if( PyArg_ParseTuple(i_pArgs, "is", &i_iThreadId, &i_szThreadName) )

    return Py_BuildValue("");

} // IpcTrcServer_registerThreadName

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_unregisterThreadName( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int i_iThreadId;

    if( PyArg_ParseTuple(i_pArgs, "i", &i_iThreadId) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        CTrcServer::UnregisterThreadName(i_iThreadId);

    } // if( PyArg_ParseTuple(i_pArgs, "i", &i_iThreadId) )

    return Py_BuildValue("");

} // IpcTrcServer_unregisterThreadName

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setOrganizationName( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    const char* i_szName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szName) )
    {
        QCoreApplication::setOrganizationName(i_szName);
    }

    return Py_BuildValue("");

} // IpcTrcServer_setOrganizationName

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getOrganizationName( PyObject* i_pModule, PyObject* /*i_pArgs*/ )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    QString strName = QCoreApplication::organizationName();

    return Py_BuildValue("s", strName.toStdString().c_str());

} // IpcTrcServer_getOrganizationName

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setApplicationName( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    const char* i_szName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szName) )
    {
        QCoreApplication::setApplicationName(i_szName);
    }

    return Py_BuildValue("");

} // IpcTrcServer_setApplicationName

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getApplicationName( PyObject* i_pModule, PyObject* /*i_pArgs*/ )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    QString strName = QCoreApplication::applicationName();

    return Py_BuildValue("s", strName.toStdString().c_str());

} // IpcTrcServer_getApplicationName

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getDefaultFilePaths( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(&DllIf_s_mtx);

    const char* i_szIniFileScope;

    QString strAdminObjFileAbsFilePath;
    QString strLocalTrcFileAbsFilePath;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szIniFileScope) )
    {
        QString strIniFileScope = i_szIniFileScope;

        CTrcServer::GetDefaultFilePaths(strAdminObjFileAbsFilePath, strLocalTrcFileAbsFilePath, strIniFileScope);

    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szIniFileScope) )

    return Py_BuildValue("ss", strAdminObjFileAbsFilePath.toStdString().c_str(), strLocalTrcFileAbsFilePath.toStdString().c_str());

} // IpcTrcServer_getDefaultFilePaths

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getInstance( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    QMutexLocker mtxLocker(&DllIf_s_mtx);

    const char* i_szName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szName) )
    {
        QString strServerName = i_szName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs = strServerName;
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strModuleName,
            /* strObjName         */ strServerName,
            /* strMethod          */ "IpcTrcServer_getInstance",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        if( CIpcTrcServer::GetInstance(strServerName) != nullptr )
        {
            bOk = true;
        }

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs
        mthTracer.setMethodReturn(bOk);
        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szName) )

    return Py_BuildValue("O", bOk ? Py_True : Py_False);

} // IpcTrcServer_getInstance

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_createInstance( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    const char* i_szName;
    int         i_iTrcDetailLevel = 0; // ETraceDetailLevelNone;

    if( PyArg_ParseTuple(i_pArgs, "si", &i_szName, &i_iTrcDetailLevel) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szName;

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

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        if( !DllIf_IpcTrcServer_s_hshpTrcMthFiles.contains(strServerName) )
        {
            CTrcServer::GetDefaultFilePaths(strAdminObjFileAbsFilePath, strLocalTrcFileAbsFilePath);

            DllIf_IpcTrcServer_s_hshpTrcMthFiles[strServerName] = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);
            DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels[strServerName] = i_iTrcDetailLevel;
        }

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "Name: " + strServerName;
            strMthInArgs += ", TrcLevel: " + QString::number(i_iTrcDetailLevel);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strModuleName,
            /* strObjName         */ strServerName,
            /* strMethod          */ "IpcTrcServer_createInstance",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            DllIf_IpcTrcServer_s_hshbTrcServerCreated[strServerName] = false;
        }
        else // if( pTrcServer == nullptr )
        {
            // If the application creating and starting the trace server is a Qt application ...
            if( DllIf_s_pQtAppCreatedByDllIf == nullptr )
            {
                CIpcTrcServer::CreateInstance(strServerName, i_iTrcDetailLevel);
            }

            // If the application creating and starting the trace server is not a Qt application ...
            else // if( DllIf_s_pQtAppCreatedByDllIf != nullptr )
            {
                // ... the trace server must be started from within a different thread context.
                if( DllIf_s_pThreadIpcTrcServer == nullptr )
                {
                    DllIf_s_pThreadIpcTrcServer = new DllIf::CIpcTrcServerThread(strServerName, i_iTrcDetailLevel);
                }

                const int c_iMaxWaitCount = 25;
                int iWaitCount = 0;

                // Start driver thread and wait until the driver has been created.
                if( !DllIf_s_pThreadIpcTrcServer->isRunning() )
                {
                    DllIf_s_pThreadIpcTrcServer->start();

                    iWaitCount = 0;
                    while( !DllIf_s_pThreadIpcTrcServer->isServerCreated() )
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
            } // if( s_pAppCreatedByDriver != nullptr )

            pTrcServer = CIpcTrcServer::GetInstance(strServerName);

            if( pTrcServer != nullptr )
            {
                bOk = true;
                DllIf_IpcTrcServer_s_hshbTrcServerCreated[strServerName] = true;
            }
        } // if( pTrcServer == nullptr )

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs
        mthTracer.setMethodReturn(bOk);
        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    } // if( PyArg_ParseTuple(i_pArgs, "si", &i_szName, &i_iTrcDetailLevel) )

    return Py_BuildValue("O", bOk ? Py_True : Py_False);

} // IpcTrcServer_createInstance

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_destroyInstance( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    const char* i_szName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        QString strMthInArgs;

        // The method tracer must be destroyed before the trace method file will be freed.
        {
            CMethodTracer mthTracer(
                /* pTrcMthFile        */ pTrcMthFile,
                /* iTrcDetailLevel    */ iTrcDetailLevel,
                /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
                /* strNameSpace       */ c_strNameSpace,
                /* strClassName       */ c_strModuleName,
                /* strObjName         */ strServerName,
                /* strMethod          */ "IpcTrcServer_destroyInstance",
                /* strMthInArgs       */ strMthInArgs );

            #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

            // If the application creating and starting the trace server is a Qt application ...
            if( DllIf_s_pThreadIpcTrcServer == nullptr )
            {
                if( DllIf_IpcTrcServer_s_hshbTrcServerCreated.value(strServerName, false) )
                {
                    CIpcTrcServer::DestroyInstance(strServerName);
                }
            }

            // If the application creating and starting the trace server is not a Qt application ...
            else // if( DllIf_s_pThreadIpcTrcServer != nullptr )
            {
                // ... the trace server was created and started from within a different thread context.
                DllIf_s_pThreadIpcTrcServer->quit();
                DllIf_s_pThreadIpcTrcServer->wait(30000);

                if( !DllIf_s_pThreadIpcTrcServer->isRunning() )
                {
                    try
                    {
                        delete DllIf_s_pThreadIpcTrcServer;
                    }
                    catch(...)
                    {
                    }
                }
                DllIf_s_pThreadIpcTrcServer = nullptr;

            } // if( DllIf_s_pThreadIpcTrcServer != nullptr )

            if( DllIf_s_pQtAppCreatedByDllIf != NULL )
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

            if( DllIf_IpcTrcServer_s_hshbTrcServerCreated.contains(strServerName) )
            {
                DllIf_IpcTrcServer_s_hshbTrcServerCreated.remove(strServerName);
            }

            bOk = true;

            #ifdef _TRACE_IPCTRACEPYDLL_METHODs

            mthTracer.setMethodReturn(bOk);

        } // The method tracer must be destroyed before the trace method file will be freed.

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs
        if( DllIf_IpcTrcServer_s_hshpTrcMthFiles.contains(strServerName) )
        {
            CTrcMthFile::Free(DllIf_IpcTrcServer_s_hshpTrcMthFiles[strServerName]);

            DllIf_IpcTrcServer_s_hshpTrcMthFiles.remove(strServerName);
            DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.remove(strServerName);
        }
        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szName) )

    return Py_BuildValue("O", bOk ? Py_True : Py_False);

} // IpcTrcServer_destroyInstance

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getTraceAdminObj( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int     idxInTree = -1;
    QString strKeyInTree;

    const char* i_szNameSpace;
    const char* i_szClassName;
    const char* i_szObjName;
    bool        i_bEnabledAsDefault;
    int         i_iDefaultDetailLevel;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "sssbis", &i_szNameSpace, &i_szClassName, &i_szObjName, &i_bEnabledAsDefault, &i_iDefaultDetailLevel, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strNameSpace  = i_szNameSpace;
        QString strClassName  = i_szClassName;
        QString strObjName    = i_szObjName;
        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        QString strMthInArgs;
        QString strMthRet;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "NameSpace: " + QString(i_szNameSpace);
            strMthInArgs += ", ClassName: " + QString(i_szClassName);
            strMthInArgs += ", ObjName: " + QString(i_szObjName);
            strMthInArgs += ", DefEnabled: " + bool2Str(i_bEnabledAsDefault);
            strMthInArgs += ", DefLevel: " + QString::number(i_iDefaultDetailLevel);
            strMthInArgs += ", ServerName: " + strServerName;
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strModuleName,
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_getTraceAdminObj",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcAdminObj* pTrcAdminObj = CTrcServer::GetTraceAdminObj(
            /* strNameSpace        */ strNameSpace,
            /* strClassName        */ strClassName,
            /* strObjName          */ strObjName,
            /* bEnabledAsDefault   */ i_bEnabledAsDefault ? EEnabled::Yes : EEnabled::No,
            /* iDefaultDetailLevel */ i_iDefaultDetailLevel,
            /* strServerName       */ strServerName );

        if( pTrcAdminObj != nullptr )
        {
            idxInTree    = pTrcAdminObj->indexInTree();
            strKeyInTree = pTrcAdminObj->keyInTree();
        }

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs
        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthRet  = "IdxInTree: " + QString::number(idxInTree);
            strMthRet += ", KeyInTree: " + strKeyInTree;
            mthTracer.setMethodReturn(strMthRet);
        }
        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    } // if( PyArg_ParseTuple(i_pArgs, "sssbis", &i_szNameSpace, &i_szClassName, &i_szObjName, &i_bEnabledAsDefault, &i_iDefaultDetailLevel, &i_szServerName) )

    //return Py_BuildValue("is", idxInTree, strKeyInTree.toStdString().c_str());
    //return Py_BuildValue("i", (const int)(idxInTree));
    return Py_BuildValue("s", strKeyInTree.toStdString().c_str());

} // IpcTrcServer_getTraceAdminObj

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_releaseTraceAdminObj( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    //int         i_idxInTree;
    const char* i_szKeyInTree;
    const char* i_szServerName;

    //if( PyArg_ParseTuple(i_pArgs, "is", &i_idxInTree, &i_szServerName) )
    if( PyArg_ParseTuple(i_pArgs, "ss", &i_szKeyInTree, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strKeyInTree  = i_szKeyInTree;
        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "IdxInTree: " + QString::number(i_idxInTree);
            strMthInArgs += ", ServerName: " + strServerName;
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strModuleName,
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_releaseTraceAdminObj",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        //CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(i_idxInTree, strServerName);
        CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(strKeyInTree, strServerName);

        if( pTrcAdminObj != nullptr )
        {
            #ifdef _TRACE_IPCTRACEPYDLL_METHODs
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("-> CTrcServer::ReleaseTraceAdminObj(" + pTrcAdminObj->keyInTree() + ")");
            }
            #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

            CTrcServer::ReleaseTraceAdminObj(pTrcAdminObj, strServerName);

            #ifdef _TRACE_IPCTRACEPYDLL_METHODs
            if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
            {
                mthTracer.trace("<- CTrcServer::ReleaseTraceAdminObj()");
            }
            #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs
        }
    } // if( PyArg_ParseTuple(i_pArgs, "is", &i_idxInTree, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_releaseTraceAdminObj

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_isActive( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bIsActive = false;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            bIsActive = pTrcServer->isActive();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("O", bIsActive ? Py_True : Py_False);

} // IpcTrcServer_isActive

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setEnabled( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool        i_bEnabled;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "bs", &i_bEnabled, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_setEnabled",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        if( pTrcServer != nullptr )
        {
            pTrcServer->setEnabled(i_bEnabled);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "bs", &i_bEnabled, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_setEnabled

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_isEnabled( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bEnabled = false;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            bEnabled = pTrcServer->isEnabled();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("O", bEnabled ? Py_True : Py_False);

} // IpcTrcServer_isEnabled

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setNewTrcAdminObjsEnabledAsDefault( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool        i_bEnabled;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "bs", &i_bEnabled, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_setNewTrcAdminObjsEnabledAsDefault",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            pTrcServer->setNewTrcAdminObjsEnabledAsDefault(i_bEnabled);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "bs", &i_bEnabled, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_setNewTrcAdminObjsEnabledAsDefault

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_areNewTrcAdminObjsEnabledAsDefault( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bEnabled = false;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            bEnabled = pTrcServer->areNewTrcAdminObjsEnabledAsDefault();
        }
    } // if( i_pTrcServer != nullptr )

    return Py_BuildValue("O", bEnabled ? Py_True : Py_False);

} // IpcTrcServer_areNewTrcAdminObjsEnabledAsDefault

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setNewTrcAdminObjsDefaultDetailLevel( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int         i_iDetailLevel;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "is", &i_iDetailLevel, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_setNewTrcAdminObjsDefaultDetailLevel",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            pTrcServer->setNewTrcAdminObjsDefaultDetailLevel(i_iDetailLevel);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "is", &i_iDetailLevel, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_setNewTrcAdminObjsDefaultDetailLevel

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getNewTrcAdminObjsDefaultDetailLevel( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int iDetailLevel = ETraceDetailLevelNone;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            iDetailLevel = pTrcServer->getNewTrcAdminObjsDefaultDetailLevel();
        }
    } // if( i_pTrcServer != nullptr )

    return Py_BuildValue("i", iDetailLevel);

} // IpcTrcServer_getNewTrcAdminObjsDefaultDetailLevel

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setAdminObjFileAbsoluteFilePath( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    const char* i_szAbsFilePath;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "ss", &i_szAbsFilePath, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strAbsFilePath = i_szAbsFilePath;
        QString strServerName  = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_setAdminObjFileAbsoluteFilePath",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            pTrcServer->setAdminObjFileAbsoluteFilePath(strAbsFilePath);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "ss", &i_szAbsFilePath, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_setAdminObjFileAbsoluteFilePath

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getAdminObjFileAbsoluteFilePath( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    QString strAbsFilePath;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            strAbsFilePath = pTrcServer->getAdminObjFileAbsoluteFilePath();
        }
    } // if( i_pTrcServer != nullptr )

    return Py_BuildValue("s", strAbsFilePath.toStdString().c_str());

} // IpcTrcServer_getAdminObjFileAbsoluteFilePath

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_recallAdminObjs( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    const char* i_szName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strModuleName,
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_recallAdminObjs",
            /* strMthInArgs       */ "" );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

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

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs
        mthTracer.setMethodReturn(bOk);
        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szName) )

    return Py_BuildValue("O", bOk ? Py_True : Py_False);

} // IpcTrcServer_recallAdminObjs

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_saveAdminObjs( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    const char* i_szName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strModuleName,
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_saveAdminObjs",
            /* strMthInArgs       */ "" );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

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

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs
        mthTracer.setMethodReturn(bOk);
        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szName) )

    return Py_BuildValue("O", bOk ? Py_True : Py_False);

} // IpcTrcServer_saveAdminObjs

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setUseLocalTrcFile( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool        i_bUse;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "bs", &i_bUse, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_setUseLocalTrcFile",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            pTrcServer->setUseLocalTrcFile(i_bUse);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "bs", &i_bUse, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_setUseLocalTrcFile

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_isLocalTrcFileUsed( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bUsed = false;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            bUsed = pTrcServer->isLocalTrcFileUsed();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("O", bUsed ? Py_True : Py_False);

} // IpcTrcServer_isLocalTrcFileUsed

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setLocalTrcFileAbsoluteFilePath( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    const char* i_szAbsFilePath;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "ss", &i_szAbsFilePath, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_setLocalTrcFileAbsoluteFilePath",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            pTrcServer->setLocalTrcFileAbsoluteFilePath(i_szAbsFilePath);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "ss", &i_szAbsFilePath, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_setLocalTrcFileAbsoluteFilePath

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getLocalTrcFileAbsoluteFilePath( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    QString strAbsFilePath;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            strAbsFilePath = pTrcServer->getLocalTrcFileAbsoluteFilePath();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("s", strAbsFilePath.toStdString().c_str());

} // IpcTrcServer_getLocalTrcFileAbsoluteFilePath

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getLocalTrcFileCompleteBaseName( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    QString strBaseName;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            strBaseName = pTrcServer->getLocalTrcFileCompleteBaseName();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("s", strBaseName.toStdString().c_str());

} // IpcTrcServer_getLocalTrcFileCompleteBaseName

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getLocalTrcFileAbsolutePath( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    QString strAbsPath;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            strAbsPath = pTrcServer->getLocalTrcFileAbsolutePath();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("s", strAbsPath.toStdString().c_str());

} // IpcTrcServer_getLocalTrcFileAbsolutePath

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_isLocalTrcFileActive( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bActive = false;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            bActive = pTrcServer->isLocalTrcFileActive();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("O", bActive ? Py_True : Py_False);

} // IpcTrcServer_isLocalTrcFileActive

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setLocalTrcFileAutoSaveInterval( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int         i_iAutoSaveInterval_ms;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "is", &i_iAutoSaveInterval_ms, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_setLocalTrcFileAutoSaveInterval",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            pTrcServer->setLocalTrcFileAutoSaveInterval(i_iAutoSaveInterval_ms);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "is", &i_iAutoSaveInterval_ms, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_setLocalTrcFileAutoSaveInterval

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getLocalTrcFileAutoSaveInterval( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int iAutoSaveInterval_ms = 0;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            iAutoSaveInterval_ms = pTrcServer->getLocalTrcFileAutoSaveInterval();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("i", iAutoSaveInterval_ms);

} // IpcTrcServer_getLocalTrcFileAutoSaveInterval

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setLocalTrcFileCloseFileAfterEachWrite( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool        i_bCloseFile;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "bs", &i_bCloseFile, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "CloseFile: " + bool2Str(i_bCloseFile);
            strMthInArgs += ", ServerName: " + strServerName;
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strModuleName,
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_setLocalTrcFileCloseFileAfterEachWrite",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            pTrcServer->setLocalTrcFileCloseFileAfterEachWrite(i_bCloseFile);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "bs", &i_bCloseFile, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_setLocalTrcFileCloseFileAfterEachWrite

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getLocalTrcFileCloseFileAfterEachWrite( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bCloseFile = false;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            bCloseFile = pTrcServer->getLocalTrcFileCloseFileAfterEachWrite();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("O", bCloseFile ? Py_True : Py_False);

} // IpcTrcServer_getLocalTrcFileCloseFileAfterEachWrite

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setLocalTrcFileSubFileCountMax( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int         i_iCountMax;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "is", &i_iCountMax, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_setLocalTrcFileSubFileCountMax",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            pTrcServer->setLocalTrcFileSubFileCountMax(i_iCountMax);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "is", &i_iCountMax, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_setLocalTrcFileSubFileCountMax

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getLocalTrcFileSubFileCountMax( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int iCountMax = 0;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            iCountMax = pTrcServer->getLocalTrcFileSubFileCountMax();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("i", iCountMax);

} // IpcTrcServer_getLocalTrcFileSubFileCountMax

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setLocalTrcFileSubFileLineCountMax( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int         i_iCountMax;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "is", &i_iCountMax, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_setLocalTrcFileSubFileLineCountMax",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            pTrcServer->setLocalTrcFileSubFileLineCountMax(i_iCountMax);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "is", &i_iCountMax, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_setLocalTrcFileSubFileLineCountMax

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getLocalTrcFileSubFileLineCountMax( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int iCountMax = 0;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            iCountMax = pTrcServer->getLocalTrcFileSubFileLineCountMax();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("i", iCountMax);

} // IpcTrcServer_getLocalTrcFileSubFileLineCountMax

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setCacheTrcDataIfNotConnected( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool        i_bCacheData;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "bs", &i_bCacheData, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_setCacheTrcDataIfNotConnected",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            pTrcServer->setCacheTrcDataIfNotConnected(i_bCacheData);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "bs", &i_bCacheData, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_setCacheTrcDataIfNotConnected

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getCacheTrcDataIfNotConnected( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bCacheData = false;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            bCacheData = pTrcServer->getCacheTrcDataIfNotConnected();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("O", bCacheData ? Py_True : Py_False);

} // IpcTrcServer_getCacheTrcDataIfNotConnected

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setCacheTrcDataMaxArrLen( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int         i_iMaxArrLen;
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "is", &i_iMaxArrLen, &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_setCacheTrcDataMaxArrLen",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            pTrcServer->setCacheTrcDataMaxArrLen(i_iMaxArrLen);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "is", &i_iMaxArrLen, &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_setCacheTrcDataMaxArrLen

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getCacheTrcDataMaxArrLen( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int iMaxArrLen = 0;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            iMaxArrLen = pTrcServer->getCacheTrcDataMaxArrLen();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("i", iMaxArrLen);

} // IpcTrcServer_getCacheTrcDataMaxArrLen

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_clearLocalTrcFile( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        QString strMthInArgs;

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strServerName,
            /* strMethod          */ "TrcServer_clearLocalTrcFile",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcServer* pTrcServer = CTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            pTrcServer->clearLocalTrcFile();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("");

} // IpcTrcServer_clearLocalTrcFile

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_startup( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    const char* i_szServerName;
    int         i_iTimeout_ms;
    bool        i_bWait;

    if( PyArg_ParseTuple(i_pArgs, "sib", &i_szServerName, &i_iTimeout_ms, &i_bWait) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strClassName       */ c_strModuleName,
            /* strObjName         */ strServerName,
            /* strMethod          */ "IpcTrcServer_startup",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            // If the application creating and starting the trace server is a Qt application ...
            if( DllIf_s_pQtAppCreatedByDllIf == nullptr && QCoreApplication::instance() != nullptr )
            {
                // The thread in which the trace server is created must start its event loop.
                // To ensure this the thread affinity of the server will be set to Qt's main
                // thread as the main thread always has an event loop.
                if( pTrcServer->thread() != QCoreApplication::instance()->thread() )
                {
                    pTrcServer->moveToThread( QCoreApplication::instance()->thread() );
                }
            }

            // If the application creating and starting the trace server is a Qt application ...
            if( DllIf_s_pThreadIpcTrcServer == nullptr )
            {
                // The trace server is created in the QtAppThread and can be directly started.
                CRequest* pReq = pTrcServer->startup(i_iTimeout_ms, i_bWait);

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
            } // if( DllIf_s_pThreadIpcTrcServer == nullptr )

            // If the application creating and starting the trace server is not a Qt application ...
            else // if( DllIf_s_pThreadIpcTrcServer != nullptr )
            {
                // ... the trace server is created in a different thread context and the
                // server must be started in this thread context.
                CMsgReqStartup* pMsg = new CMsgReqStartup(
                    /* pObjSender       */ nullptr,
                    /* pObjReceiver     */ pTrcServer,
                    /* hostSettings     */ pTrcServer->getHostSettings(),
                    /* pBlkType         */ pTrcServer->getBlkType(),  // The block will be cloned.
                    /* bMustBeConfirmed */ false,
                    /* iReqId           */ -1,
                    /* iMsgId           */ -1 );
                #ifdef _TRACE_IPCTRACEPYDLL_METHODs
                POST_OR_DELETE_MESSAGE(pMsg, &mthTracer, ETraceDetailLevelRuntimeInfo);
                #else
                POST_OR_DELETE_MESSAGE(pMsg);
                #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs
                pMsg = NULL;

                const int c_iMaxWaitCount = 25;
                int iWaitCount = 0;

                if( DllIf_s_pThreadIpcTrcServer != NULL )
                {
                    while( !DllIf_s_pThreadIpcTrcServer->isServerStarted() )
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

                bOk = DllIf_s_pThreadIpcTrcServer->isServerStarted();

            } // if( DllIf_s_pThreadIpcTrcServer != nullptr )
        } // if( pTrcServer != nullptr )

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs
        mthTracer.setMethodReturn(bOk);
        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    } // if( PyArg_ParseTuple(i_pArgs, "sib", &i_szServerName, &i_iTimeout_ms, &i_bWait) )

    return Py_BuildValue("O", bOk ? Py_True : Py_False);

} // IpcTrcServer_startup

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_shutdown( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    const char* i_szName;
    int         i_iTimeout_ms;
    bool        i_bWait;

    if( PyArg_ParseTuple(i_pArgs, "sib", &i_szName, &i_iTimeout_ms, &i_bWait) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

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
            /* strClassName       */ c_strModuleName,
            /* strObjName         */ strServerName,
            /* strMethod          */ "IpcTrcServer_shutdown",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            // If the application creating and starting the trace server is a Qt application ...
            if( DllIf_s_pThreadIpcTrcServer == nullptr )
            {
                // The trace server is created in the QtAppThread and can be directly shutdown.
                CRequest* pReq = pTrcServer->shutdown(i_iTimeout_ms, i_bWait);

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
            } // if( DllIf_s_pThreadIpcTrcServer == nullptr )

            // If the application creating and starting the trace server is not a Qt application ...
            else // if( DllIf_s_pThreadIpcTrcServer != nullptr )
            {
                // ... the trace server is created in a different thread context and the
                // server must be shutdown in this thread context.
                CMsgReqShutdown* pMsg = new CMsgReqShutdown(
                    /* pObjSender       */ nullptr,
                    /* pObjReceiver     */ pTrcServer,
                    /* bMustBeConfirmed */ false,
                    /* iReqId           */ -1,
                    /* iMsgId           */ -1 );
                #ifdef _TRACE_IPCTRACEPYDLL_METHODs
                POST_OR_DELETE_MESSAGE(pMsg, &mthTracer, ETraceDetailLevelRuntimeInfo);
                #else
                POST_OR_DELETE_MESSAGE(pMsg);
                #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs
                pMsg = NULL;

                const int c_iMaxWaitCount = 25;
                int iWaitCount = 0;

                if( DllIf_s_pThreadIpcTrcServer != NULL )
                {
                    while( !DllIf_s_pThreadIpcTrcServer->isServerShutdown() )
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

                bOk = DllIf_s_pThreadIpcTrcServer->isServerShutdown();

            } // if( DllIf_s_pThreadIpcTrcServer != nullptr )
        } // if( pTrcServer != nullptr )

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs
        mthTracer.setMethodReturn(bOk);
        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

    } // if( PyArg_ParseTuple(i_pArgs, "sib", &i_szName, &i_iTimeout_ms, &i_bWait) )

    return Py_BuildValue("O", bOk ? Py_True : Py_False);

} // IpcTrcServer_shutdown

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_isListening( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bIsListening = false;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            bIsListening = pTrcServer->isListening();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("O", bIsListening ? Py_True : Py_False);

} // IpcTrcServer_isListening

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_isConnected( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bIsConnected = false;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            bIsConnected = pTrcServer->isConnected();
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("O", bIsConnected ? Py_True : Py_False);

} // IpcTrcServer_isConnected

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_setPort( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    bool bOk = false;

    int         i_iPort;
    const char* i_szServerName;
    int         i_iTimeout_ms;
    bool        i_bWait;

    if( PyArg_ParseTuple(i_pArgs, "isib", &i_iPort, &i_szServerName, &i_iTimeout_ms, &i_bWait) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CTrcMthFile* pTrcMthFile = DllIf_IpcTrcServer_s_hshpTrcMthFiles.value(strServerName, nullptr);
        int          iTrcDetailLevel = DllIf_IpcTrcServer_s_hshiTrcMthDetailLevels.value(strServerName, ETraceDetailLevelNone);

        QString strMthInArgs;

        if( iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
        {
            strMthInArgs  = "Port: " + QString::number(i_iPort);
            strMthInArgs += ", Timeout: " + QString::number(i_iTimeout_ms) + " ms";
            strMthInArgs += ", Wait: " + bool2Str(i_bWait);
        }

        CMethodTracer mthTracer(
            /* pTrcMthFile        */ pTrcMthFile,
            /* iTrcDetailLevel    */ iTrcDetailLevel,
            /* iFilterDetailLavel */ ETraceDetailLevelMethodCalls,
            /* strNameSpace       */ c_strNameSpace,
            /* strClassName       */ c_strClassName,
            /* strObjName         */ strServerName,
            /* strMethod          */ "IpcTrcServer_setPort",
            /* strMthInArgs       */ strMthInArgs );

        #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs

        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            ZS::Ipc::SServerHostSettings hostSettings = pTrcServer->getHostSettings();

            //hostSettings.m_socketType = ZS::Ipc::ESocketTypeTcp;
            //hostSettings.m_pObjLocal = nullptr;
            //hostSettings.m_strLocalHostName;
            //hostSettings.m_hostAddrLocal;
            hostSettings.m_uLocalPort = static_cast<unsigned short>(i_iPort);
            //hostSettings.m_uMaxPendingConnections = 30;
            //hostSettings.m_uBufferSize;

            // If the application creating and starting the trace server is a Qt application ...
            if( DllIf_s_pThreadIpcTrcServer == nullptr )
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
            } // if( DllIf_s_pThreadIpcTrcServer == nullptr )

            // If the application creating and starting the trace server is not a Qt application ...
            else // if( DllIf_s_pThreadIpcTrcServer != nullptr )
            {
                // ... the trace server is created in a different thread context and the
                // server must be shutdown in this thread context.
                CMsgReqChangeSettings* pMsg = new CMsgReqChangeSettings(
                    /* pObjSender       */ nullptr,
                    /* pObjReceiver     */ pTrcServer,
                    /* hostSettings     */ hostSettings,
                    /* bMustBeConfirmed */ false,
                    /* iReqId           */ -1,
                    /* iMsgId           */ -1 );
                #ifdef _TRACE_IPCTRACEPYDLL_METHODs
                POST_OR_DELETE_MESSAGE(pMsg, &mthTracer, ETraceDetailLevelRuntimeInfo);
                #else
                POST_OR_DELETE_MESSAGE(pMsg);
                #endif // #ifdef _TRACE_IPCTRACEPYDLL_METHODs
                pMsg = NULL;

                bOk = true;

            } // if( DllIf_s_pThreadIpcTrcServer != nullptr )
        } // if( pTrcServer != nullptr )
    } // if( PyArg_ParseTuple(i_pArgs, "isib", &i_iPort, &i_szServerName, &i_iTimeout_ms, &i_bWait) )

    return Py_BuildValue("O", bOk ? Py_True : Py_False);

} // IpcTrcServer_setPort

//------------------------------------------------------------------------------
static PyObject* IpcTrcServer_getPort( PyObject* i_pModule, PyObject* i_pArgs )
//------------------------------------------------------------------------------
{
    int iPort = 0;

    const char* i_szServerName;

    if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )
    {
        QMutexLocker mtxLocker(&DllIf_s_mtx);

        QString strServerName = i_szServerName;

        CIpcTrcServer* pTrcServer = CIpcTrcServer::GetInstance(strServerName);

        if( pTrcServer != nullptr )
        {
            ZS::Ipc::SServerHostSettings hostSettings = pTrcServer->getHostSettings();

            iPort = static_cast<int>(hostSettings.m_uLocalPort);
        }
    } // if( PyArg_ParseTuple(i_pArgs, "s", &i_szServerName) )

    return Py_BuildValue("i", iPort);

} // IpcTrcServer_getPort

/*******************************************************************************
Libraries depending on build configuration and used Qt version
*******************************************************************************/

#ifdef _WINDOWS

#ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#pragma message(__FILE__ ": Linking against = " QTCORELIB)
#pragma comment( lib, QTCORELIB )
#pragma message(__FILE__ ": Linking against = " QTNETWORKLIB)
#pragma comment( lib, QTNETWORKLIB )
#pragma message(__FILE__ ": Linking against = " QTXMLLIB)
#pragma comment( lib, QTXMLLIB )

#pragma message(__FILE__ ": Linking against = " ZSSYSLIB)
#pragma comment(lib, ZSSYSLIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCLIB)
#pragma comment(lib, ZSIPCLIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCTRACELIB)
#pragma comment(lib, ZSIPCTRACELIB)

#endif // #ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#endif // #ifdef _WINDOWS


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
    static int s_iProcessDetachCount = 0;
    static int s_iProcessAttachCount = 0;
    static int s_iThreadAttachCount  = 0;
    static int s_iThreadDetachCount  = 0;

    switch( i_ul_reason_for_call )
    {
        case DLL_PROCESS_DETACH: // 0
        {
            s_iProcessDetachCount++;
            break;
        }
        case DLL_PROCESS_ATTACH: // 1
        {
            s_iProcessAttachCount++;
            break;
        }
        case DLL_THREAD_ATTACH: // 2
        {
            s_iThreadAttachCount++;
            break;
        }
        case DLL_THREAD_DETACH: // 3
        {
            s_iThreadDetachCount++;
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
