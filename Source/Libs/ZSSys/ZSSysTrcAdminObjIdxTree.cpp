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

#include <QtCore/qmutex.h>
#include <QtCore/qdir.h>
#include <QtCore/qthread.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMutex.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;


/*******************************************************************************
class CIdxTreeTrcAdminObjs : public ZS::System::CIdxTree
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CIdxTreeTrcAdminObjs::CIdxTreeTrcAdminObjs(
    const QString&               i_strObjName,
    QObject*                     i_pObjParent,
    ETraceDetailLevelMethodCalls i_eTrcDetailLevel,
    ETraceDetailLevelMethodCalls i_eTrcDetailLevelMutex ) :
//------------------------------------------------------------------------------
    CIdxTree(
        /* strIdxTreeName       */ i_strObjName,
        /* pRootTreeEntry       */ nullptr,
        /* strNodeSeparator     */ "::",
        /* bCreateMutex         */ true,
        /* pObjParent           */ i_pObjParent,
        /* eTrcDetailLevel      */ i_eTrcDetailLevel,
        /* eTrcDetailLevelMutex */ i_eTrcDetailLevelMutex )
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strAddTrcInfo );

} // ctor

//------------------------------------------------------------------------------
CIdxTreeTrcAdminObjs::~CIdxTreeTrcAdminObjs()
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ strAddTrcInfo );

} // dtor

/*==============================================================================
public: // instance methods to get and release admin objects
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the trace admin object with the given name space, class name and
    object name.

    If an admin object with the given name space, class name and object name
    is not yet existing in the index tree a new object is created and added to
    the index tree.
    If an admin object with the given name space, class name and object name
    is already existing a reference counter will be incremented and the already
    existing admin object is returned.

    @param i_strNameSpace [in] Name space (may be empty).
        The namespace will used to build the path to the tree entry.
        If both the object and class names are empty the namespace
        becomes the name of the leave entry.
    @param i_strClassName [in] Class name (may be empty).
        The class name will used to build the path to the tree entry.
        If the object name is empty the class name becomes the name of the leave entry.
    @param i_strObjName [in] Object name (may be empty).
        If not empty the object name becomes the name of the leave entry.
    @param i_bEnabledAsDefault [in] Range [Yes, No, Undefined]
        If not Undefined the enabled flag will be set as the trace admin object.
    @param i_eDefaultDetailLevelMethodCalls [in]
        If not Undefined the detail level will be set at the trace admin object.
    @param i_eDefaultDetailLevelRuntimeInfo [in]
        If not Undefined the detail level will be set at the trace admin object.
    @param i_strDefaultDataFilter [in]
        If not null (QString()) the data filter will be set at the trace admin object.
    @param i_bIncrementRefCount [in]
        true to increment the reference counter of the trace admin object.
        false is only used if the admin objects are recalled from file as
        at that point only the tree entries are existing as place holders
        but the admin objects are not yet referenced.

    @return Pointer to allocated trace admin objecct or nullptr on error.
*/
CTrcAdminObj* CIdxTreeTrcAdminObjs::getTraceAdminObj(
    const QString&               i_strNameSpace,
    const QString&               i_strClassName,
    const QString&               i_strObjName,
    ZS::System::EEnabled         i_bEnabledAsDefault,
    ETraceDetailLevelMethodCalls i_eDefaultDetailLevelMethodCalls,
    ETraceDetailLevelRuntimeInfo i_eDefaultDetailLevelRuntimeInfo,
    const QString&               i_strDefaultDataFilter,
    bool                         i_bIncrementRefCount )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo  = "NameSpace: " + i_strNameSpace;
        strAddTrcInfo += ", ClassName: " + i_strClassName;
        strAddTrcInfo += ", ObjName: " + i_strObjName;
        strAddTrcInfo += ", EnabledAsDefault: " + CEnumEnabled::toString(i_bEnabledAsDefault);
        strAddTrcInfo += ", DefaultDetailLevelMethodCalls: " + CEnumTraceDetailLevelMethodCalls(i_eDefaultDetailLevelMethodCalls).toString();
        strAddTrcInfo += ", DefaultDetailLevelRuntimeInfo: " + CEnumTraceDetailLevelRuntimeInfo(i_eDefaultDetailLevelRuntimeInfo).toString();
        strAddTrcInfo += ", DefaultDataFilter: " + i_strDefaultDataFilter;
        strAddTrcInfo += ", IncrementRefCount: " + bool2Str(i_bIncrementRefCount);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "getTraceAdminObj",
        /* strMethodInArgs    */ strAddTrcInfo );

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
        QString strParentBranchPath;
        QString strLeaveName;

        // ClassName and ObjName may contain node separators.
        // For ClassName to define group of methods which can separately controlled.
        // For ObjName to define logically grouped objects.
        // The name of the leave got to be determined which may be either the last
        // section of the class name or - if the object name is not empty - the last
        // section of the object name.
        // All three passed names will be sent to the branch names list.
        // The last branch name will be removed afterwards and taken as the leave name.

        QString strPath = buildPathStr(i_strNameSpace, i_strClassName, i_strObjName);
        splitPathStr(strPath, &strParentBranchPath, &strLeaveName);

        CIdxTreeEntry* pLeave = findLeave(strParentBranchPath, strLeaveName);

        if( pLeave != nullptr )
        {
            pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pLeave);
        }
        else // if( pLeave == nullptr )
        {
            QStringList strlstBranchNames = strParentBranchPath.split(m_strNodeSeparator);
            QString strParentPathTmp;
            QString strPathTmp;

            CIdxTreeEntry* pBranch;

            for( auto& strBranchName : strlstBranchNames )
            {
                strPathTmp = buildPathStr(strPathTmp, strBranchName);

                pBranch = findBranch(strPathTmp);

                if( pBranch == nullptr )
                {
                    pBranch = createBranch(strBranchName);
                    add(pBranch, strParentPathTmp);
                }
                strParentPathTmp = buildPathStr(strParentPathTmp, strBranchName);
            }

            pTrcAdminObj = new CTrcAdminObj(i_strNameSpace, i_strClassName, i_strObjName, strLeaveName);

            EEnabled bEnabled = EEnabled::Yes;
            ETraceDetailLevelMethodCalls eDetailLevelMethodCalls = ETraceDetailLevelMethodCalls::None;
            ETraceDetailLevelRuntimeInfo eDetailLevelRuntimeInfo = ETraceDetailLevelRuntimeInfo::None;
            QString strDataFilter = "";

            if( i_bEnabledAsDefault != EEnabled::Undefined )
            {
                bEnabled = i_bEnabledAsDefault;
            }
            if( i_eDefaultDetailLevelMethodCalls != ETraceDetailLevelMethodCalls::Undefined )
            {
                eDetailLevelMethodCalls = i_eDefaultDetailLevelMethodCalls;
            }
            if( i_eDefaultDetailLevelRuntimeInfo != ETraceDetailLevelRuntimeInfo::Undefined )
            {
                eDetailLevelRuntimeInfo = i_eDefaultDetailLevelRuntimeInfo;
            }
            if( !i_strDefaultDataFilter.isNull() )
            {
                strDataFilter = i_strDefaultDataFilter;
            }

            pTrcAdminObj->setEnabled(bEnabled);
            pTrcAdminObj->setMethodCallsTraceDetailLevel(eDetailLevelMethodCalls);
            pTrcAdminObj->setRuntimeInfoTraceDetailLevel(eDetailLevelRuntimeInfo);
            pTrcAdminObj->setTraceDataFilter(strDataFilter);

            add(pTrcAdminObj, strParentBranchPath);

        } // if( pLeave == nullptr )

        if( i_bIncrementRefCount )
        {
            pTrcAdminObj->incrementRefCount();
        }
    } // if( !i_strObjName.isEmpty() || !i_strClassName.isEmpty() || !i_strNameSpace.isEmpty() )

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCalls::ArgsNormal) )
    {
        mthTracer.setMethodReturn(QString(pTrcAdminObj == nullptr ? "nullptr" : pTrcAdminObj->keyInTree()));
    }

    return pTrcAdminObj;

} // getTraceAdminObj

//------------------------------------------------------------------------------
/*! @brief Returns the trace admin object at the given tree index.

    The reference counter of the trace admin object will be incremented.

    @param i_idxInTree [in] Index of the trace admin object.
    @param i_bIncrementRefCount [in]
        true to increment the reference counter of the trace admin object.
        false is used on clients side on receiving method trace data.

    @return Pointer to allocated trace admin objecct or nullptr on error.
*/
CTrcAdminObj* CIdxTreeTrcAdminObjs::getTraceAdminObj( int i_idxInTree, bool i_bIncrementRefCount )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo = "IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "getTraceAdminObj",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(getEntry(i_idxInTree));

    if( pTrcAdminObj != nullptr && i_bIncrementRefCount )
    {
        pTrcAdminObj->incrementRefCount();
    }

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCalls::ArgsNormal) )
    {
        mthTracer.setMethodReturn(QString(pTrcAdminObj == nullptr ? "nullptr" : pTrcAdminObj->keyInTree()));
    }

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

    @param io_ppTrcAdminObj [in, out]
        In:  Pointer to admin object which should be renamed. The reference counter
             of this object is decremented. If 0 the object will be destroyed.
        Out: Pointer to trace admin object at the new position. This might either
             be an already existing trace admin object whose reference counter is
             increased or a newly created object.
    @param i_strNewObjName [in] New object name.
*/
void CIdxTreeTrcAdminObjs::renameTraceAdminObj(
    CTrcAdminObj** io_ppTrcAdminObj,
    const QString& i_strNewObjName )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo = QString(*io_ppTrcAdminObj == nullptr ? "nullptr" : (*io_ppTrcAdminObj)->keyInTree());
        strAddTrcInfo = ", NewObjName: " + i_strNewObjName;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "renameTraceAdminObj",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    CTrcAdminObj* pTrcAdminObj = *io_ppTrcAdminObj;

    if( pTrcAdminObj == nullptr)
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "renameTraceAdminObj",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "No TraceAdminObj defined");

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else if( i_strNewObjName.isEmpty() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "renameTraceAdminObj",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "New ObjectName is empty");

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else if( pTrcAdminObj->getObjectName().isEmpty() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "renameTraceAdminObj",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "Cannot rename an object with an empty object name");

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else if( pTrcAdminObj->getRefCount() < 1 )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "renameTraceAdminObj",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "Only instances referencing the trace admin object may rename the object");

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else if( pTrcAdminObj->getObjectName() != i_strNewObjName )
    {
        QString strNameSpace  = pTrcAdminObj->getNameSpace();
        QString strClassName  = pTrcAdminObj->getClassName();
        QString strOldObjName = pTrcAdminObj->getObjectName();
        EEnabled bEnabled     = pTrcAdminObj->getEnabled();
        ETraceDetailLevelMethodCalls eDetailLevelMethodCalls = pTrcAdminObj->getMethodCallsTraceDetailLevel();
        ETraceDetailLevelRuntimeInfo eDetailLevelRuntimeInfo = pTrcAdminObj->getRuntimeInfoTraceDetailLevel();
        QString strDataFilter = pTrcAdminObj->getTraceDataFilter();

        // The reference counter of the previously referenced trace admin object is decremented.
        pTrcAdminObj->decrementRefCount();

        // If the trace admin objects reference counter is greater than 0
        // all other modules must still refer to the unchanged trace admin object.

        // If no longer referenced ...
        if( pTrcAdminObj->getRefCount() == 0 )
        {
            // .. the object will be deleted and removed from the tree.
            if( pTrcAdminObj->isLocked() )
            {
                pTrcAdminObj->setDeleteOnUnlock(true);
            }
            else
            {
                delete pTrcAdminObj;
                pTrcAdminObj = nullptr;
            }

            QString strOldParentBranchPath;

            QString strOldPath = buildPathStr(strNameSpace, strClassName, strOldObjName);
            splitPathStr(strOldPath, &strOldParentBranchPath, nullptr);

            removeEmptyBranches(strOldParentBranchPath);
        }

        QString strNewParentBranchPath;
        QString strNewLeaveName;

        QString strNewPath = buildPathStr(strNameSpace, strClassName, i_strNewObjName);
        splitPathStr(strNewPath, &strNewParentBranchPath, &strNewLeaveName);

        CIdxTreeEntry* pLeaveNew = findLeave(strNewParentBranchPath, strNewLeaveName);

        // If already a trace admin object with the name space, class name and
        // new object name is existing ..
        if( pLeaveNew != nullptr )
        {
            pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pLeaveNew);
            pTrcAdminObj->incrementRefCount();
        }
        // If no trace admin object with the name space, class name and new object name is existing ..
        else // if( pLeaveNew == nullptr )
        {
            // A new trace admin object has to be created and returned.
            pTrcAdminObj = getTraceAdminObj(
                strNameSpace, strClassName, i_strNewObjName, bEnabled,
                eDetailLevelMethodCalls, eDetailLevelRuntimeInfo, strDataFilter);
        }

        pTrcAdminObj->setObjectName(i_strNewObjName);

    } // if( pTrcAdminObj != nullptr && !i_strNewObjName.isEmpty() )

    if( mthTracer.areMethodCallsActive(ETraceDetailLevelMethodCalls::ArgsNormal) )
    {
        mthTracer.setMethodOutArgs(QString(pTrcAdminObj == nullptr ? "nullptr" : pTrcAdminObj->keyInTree()));
    }

    *io_ppTrcAdminObj = pTrcAdminObj;

} // renameTraceAdminObj

//------------------------------------------------------------------------------
/*! @brief Releases the given trace admin object.

    Only the reference counter of the trace admin object will be decremented
    but the admin object will not neither be removed from the tree nor will
    be destroyed.

    @param i_pTrcAdminObj [in] Pointer to admin object to be released.
*/
void CIdxTreeTrcAdminObjs::releaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo = "TrcAdminObj: " + QString(i_pTrcAdminObj == nullptr ? "nullptr" : i_pTrcAdminObj->path());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "releaseTraceAdminObj",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( i_pTrcAdminObj != nullptr )
    {
        if( i_pTrcAdminObj->deleteOnUnlock() )
        {
            i_pTrcAdminObj->setDeleteOnUnlock(false); // to avoid an entry into the error log
            delete i_pTrcAdminObj;
            i_pTrcAdminObj = nullptr;
        }
        else
        {
            i_pTrcAdminObj->decrementRefCount();
        }
    }
} // releaseTraceAdminObj

/*==============================================================================
public: // instance methods to insert branch nodes and admin objects
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief This method is used by trace clients to insert a branch
           at the given index in the tree.

    The branch becomes a child of the branch given by parameter i_iParentBranchIdxInTree.

    @param i_iParentBranchIdxInTree [in] Tree entry index of the parent entry.
    @param i_strBranchName [in] Branch name to be created.
    @param i_idxInTree [in] Tree entry index at which the new branch will be inserted.

    @return Pointer to allocated trace admin objecct or nullptr on error.
*/
CIdxTreeEntry* CIdxTreeTrcAdminObjs::insertBranch(
    int            i_iParentBranchIdxInTree,
    const QString& i_strBranchName,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo  = "ParentBranchIdxInTree: " + QString::number(i_iParentBranchIdxInTree);
        strAddTrcInfo += ", Name: " + i_strBranchName;
        strAddTrcInfo += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insertBranch",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pBranch = nullptr;

    QString strMth = "insertBranch";
    QString strAddErrInfo;

    CIdxTreeEntry* pBranchParent = nullptr;

    if( i_iParentBranchIdxInTree >= 0 )
    {
        pBranchParent = getEntry(i_iParentBranchIdxInTree);

        if( pBranchParent == nullptr )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                strAddErrInfo = "Cannot add branch " + i_strBranchName + " with tree index " + QString::number(i_idxInTree);
                strAddErrInfo += " as there is no parent branch at the specified parent branch tree index " + QString::number(i_iParentBranchIdxInTree);
                SErrResultInfo errResultInfo = ErrResultInfoError(strMth, EResultObjNotInList, strAddErrInfo);
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
    }
    else // if( i_iParentBranchIdxInTree < 0 )
    {
        pBranchParent = m_pRoot;
    }

    if( i_idxInTree >= 0 )
    {
        CIdxTreeEntry* pTreeEntry = getEntry(i_idxInTree);

        // If there is already a tree entry at the given index ..
        if( pTreeEntry != nullptr )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                strAddErrInfo = "Cannot add branch " + i_strBranchName + " with tree index " + QString::number(i_idxInTree);
                strAddErrInfo += " as the tree index is already occupied by " + pTreeEntry->keyInTree();
                SErrResultInfo errResultInfo = ErrResultInfoError(strMth, EResultObjAlreadyInList, strAddErrInfo);
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
        // If there is no tree entry at the given index ..
        else // if( pTreeEntry == nullptr )
        {
             pBranch = createBranch(i_strBranchName);
             insert(pBranch, pBranchParent, -1, i_idxInTree);
        }
    } // if( i_idxInTree >= 0 )

    return pBranch;

} // insertBranch

//------------------------------------------------------------------------------
/*! @brief This method is used by trace clients to insert a trace admin object
           at the given index in the tree.

    If there is no admin object at the given index a new object is created
    with the given name space, class name and object name and inserted in the
    index tree at the index given by the object id.

    If there is already an object at the given index with a different name space,
    class name or object name nullptr is returned and an err log entry is created.

    As this method is used by trace clients the reference counter of the trace
    admin object is !! NOT !! incremented.

    The trace admin object becomes a child of the branch given by parameter
    i_iParentBranchIdxInTree.

    @param i_iParentBranchIdxInTree [in] Tree entry index of the parent entry.
    @param i_strNameSpace [in] Name space of the trace amdin object.
    @param i_strClassName [in] Class name of the trace amdin object.
    @param i_strObjName [in] Object name of the trace amdin object.
    @param i_idxInTree [in] Tree entry index at which the new object will be inserted.

    @return Pointer to allocated trace admin objecct or nullptr on error.
*/
CTrcAdminObj* CIdxTreeTrcAdminObjs::insertTraceAdminObj(
    int            i_iParentBranchIdxInTree,
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo  = "ParentBranchIdxInTree: " + QString::number(i_iParentBranchIdxInTree);
        strAddTrcInfo += ", NameSpace: " + i_strNameSpace;
        strAddTrcInfo += ", ClassName: " + i_strClassName;
        strAddTrcInfo += ", ObjName: " + i_strObjName;
        strAddTrcInfo += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insertTraceAdminObj",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    QString strMth = "insertTraceAdminObj";
    QString strAddErrInfo;

    CIdxTreeEntry* pBranchParent = nullptr;

    if( i_iParentBranchIdxInTree >= 0 )
    {
        pBranchParent = getEntry(i_iParentBranchIdxInTree);

        if( pBranchParent == nullptr )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                strAddErrInfo = "Cannot add trace admin object " + i_strObjName + " with tree index " + QString::number(i_idxInTree);
                strAddErrInfo += " as there is no parent branch at the specified parent branch tree index " + QString::number(i_iParentBranchIdxInTree);
                SErrResultInfo errResultInfo = ErrResultInfoError(strMth, EResultObjNotInList, strAddErrInfo);
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
    }
    else // if( i_iParentBranchIdxInTree < 0 )
    {
        pBranchParent = m_pRoot;
    }

    if( i_idxInTree >= 0 )
    {
        CIdxTreeEntry* pTreeEntry = getEntry(i_idxInTree);

        // If there is already a tree entry at the given index ..
        if( pTreeEntry != nullptr )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                strAddErrInfo = "Cannot add trace admin object " + i_strObjName + " with tree index " + QString::number(i_idxInTree);
                strAddErrInfo += " as the tree index is already occupied by " + pTreeEntry->keyInTree();
                SErrResultInfo errResultInfo = ErrResultInfoError(strMth, EResultObjAlreadyInList, strAddErrInfo);
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
        // If there is no tree entry at the given index ..
        else // if( pTreeEntry == nullptr )
        {
            QString strLeaveName;
            QString strPath = buildPathStr(m_strNodeSeparator, i_strNameSpace, i_strClassName, i_strObjName);
            splitPathStr(strPath, nullptr, &strLeaveName);
            pTrcAdminObj = new CTrcAdminObj(i_strNameSpace, i_strClassName, i_strObjName, strLeaveName);
            insert(pTrcAdminObj, pBranchParent, -1, i_idxInTree);
        }
    } // if( i_idxInTree >= 0 )

    return pTrcAdminObj;

} // insertTraceAdminObj

/*==============================================================================
public: // instance methods to recursively modify admin objects via object index of node entries
==============================================================================*/

//------------------------------------------------------------------------------
void CIdxTreeTrcAdminObjs::setEnabled( int i_iObjId, EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo  = "ObjId: " + QString::number(i_iObjId);
        strAddTrcInfo += ", Enabled: " + CEnumEnabled::toString(i_enabled);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setEnabled",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = getEntry(i_iObjId);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            setEnabled(pTreeEntry, i_enabled);
        }
        else if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcAdminObj->setEnabled(i_enabled);
            }
        }
    }

} // setEnabled

//------------------------------------------------------------------------------
void CIdxTreeTrcAdminObjs::setMethodCallsTraceDetailLevel(
    int i_iObjId, ETraceDetailLevelMethodCalls i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo  = "ObjId: " + QString::number(i_iObjId);
        strAddTrcInfo += ", DetailLevel: " + CEnumTraceDetailLevelMethodCalls(i_eDetailLevel).toString();
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setMethodCallsTraceDetailLevel",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = getEntry(i_iObjId);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            setMethodCallsTraceDetailLevel(pTreeEntry, i_eDetailLevel);
        }
        else if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcAdminObj->setMethodCallsTraceDetailLevel(i_eDetailLevel);
            }
        }
    }

} // setMethodCallsTraceDetailLevel

//------------------------------------------------------------------------------
void CIdxTreeTrcAdminObjs::setRuntimeInfoTraceDetailLevel(
    int i_iObjId, ETraceDetailLevelRuntimeInfo i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo  = "ObjId: " + QString::number(i_iObjId);
        strAddTrcInfo += ", DetailLevel: " + CEnumTraceDetailLevelRuntimeInfo(i_eDetailLevel).toString();
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setRuntimeInfoTraceDetailLevel",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = getEntry(i_iObjId);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            setRuntimeInfoTraceDetailLevel(pTreeEntry, i_eDetailLevel);
        }
        else if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcAdminObj->setRuntimeInfoTraceDetailLevel(i_eDetailLevel);
            }
        }
    }

} // setRuntimeInfoTraceDetailLevel

/*==============================================================================
public: // instance methods to recursively modify admin objects via namespace node entries
==============================================================================*/

//------------------------------------------------------------------------------
void CIdxTreeTrcAdminObjs::setEnabled( CIdxTreeEntry* i_pBranch, EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->path());
        strAddTrcInfo += ", Enabled: " + CEnumEnabled::toString(i_enabled);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setEnabled",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( i_pBranch != nullptr )
    {
        CIdxTreeEntry* pTreeEntry;
        CTrcAdminObj*  pTrcAdminObj;
        int            idxEntry;

        for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = i_pBranch->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                {
                    pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

                    if( pTrcAdminObj != nullptr )
                    {
                        if( pTrcAdminObj->getEnabled() != i_enabled )
                        {
                            pTrcAdminObj->setEnabled(i_enabled);
                        }
                    }
                }
                else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
                {
                    setEnabled(pTreeEntry, i_enabled );
                }
            }
        } // for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
    } // if( i_pBranch != nullptr )

} // setEnabled

//------------------------------------------------------------------------------
void CIdxTreeTrcAdminObjs::setMethodCallsTraceDetailLevel(
    CIdxTreeEntry* i_pBranch, ETraceDetailLevelMethodCalls i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->path());
        strAddTrcInfo += ", DetailLevel: " + CEnumTraceDetailLevelMethodCalls(i_eDetailLevel).toString();
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setMethodCallsTraceDetailLevel",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( i_pBranch != nullptr )
    {
        CIdxTreeEntry* pTreeEntry;
        CTrcAdminObj*  pTrcAdminObj;
        int            idxEntry;

        for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = i_pBranch->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                {
                    pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

                    if( pTrcAdminObj != nullptr )
                    {
                        if( pTrcAdminObj->getMethodCallsTraceDetailLevel() != i_eDetailLevel )
                        {
                            pTrcAdminObj->setMethodCallsTraceDetailLevel(i_eDetailLevel);
                        }
                    }
                }
                else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
                {
                    setMethodCallsTraceDetailLevel(pTreeEntry, i_eDetailLevel);
                }
            }
        } // for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
    } // if( i_pBranch != nullptr )

} // setMethodCallsTraceDetailLevel

//------------------------------------------------------------------------------
void CIdxTreeTrcAdminObjs::setRuntimeInfoTraceDetailLevel(
    CIdxTreeEntry* i_pBranch, ETraceDetailLevelRuntimeInfo i_eDetailLevel )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->path());
        strAddTrcInfo += ", DetailLevel: " + CEnumTraceDetailLevelRuntimeInfo(i_eDetailLevel).toString();
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setRuntimeInfoTraceDetailLevel",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    if( i_pBranch != nullptr )
    {
        CIdxTreeEntry* pTreeEntry;
        CTrcAdminObj*  pTrcAdminObj;
        int            idxEntry;

        for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = i_pBranch->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                {
                    pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

                    if( pTrcAdminObj != nullptr )
                    {
                        if( pTrcAdminObj->getRuntimeInfoTraceDetailLevel() != i_eDetailLevel )
                        {
                            pTrcAdminObj->setRuntimeInfoTraceDetailLevel(i_eDetailLevel);
                        }
                    }
                }
                else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
                {
                    setRuntimeInfoTraceDetailLevel(pTreeEntry, i_eDetailLevel);
                }
            }
        } // for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
    } // if( i_pBranch != nullptr )

} // setRuntimeInfoTraceDetailLevel

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CIdxTreeTrcAdminObjs::save( const QString& i_strAbsFilePath ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo = i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "save",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    QString strMth = "save";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QFile file;

    if( i_strAbsFilePath.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidFileName);
    }
    else
    {
        QFileInfo fileInfo(i_strAbsFilePath);
        QDir      dir = fileInfo.absoluteDir();

        if( !dir.exists() )
        {
            dir.mkpath(dir.absolutePath());
        }

        file.setFileName(i_strAbsFilePath);

        if( !file.open(QIODevice::WriteOnly) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileOpenForWrite);
            errResultInfo.setAddErrInfoDscr(i_strAbsFilePath);
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        QByteArray byteArrSchema;

        QXmlStreamWriter xmlStreamWriter(&byteArrSchema);

        xmlStreamWriter.setAutoFormatting(true);

        xmlStreamWriter.writeStartDocument();
        xmlStreamWriter.writeStartElement("TrcAdminObjs");

        save(xmlStreamWriter, m_pRoot);

        xmlStreamWriter.writeEndElement();  // Method Trace Admin Objects
        xmlStreamWriter.writeEndDocument();

        if( !errResultInfo.isErrorResult() )
        {
            file.write(byteArrSchema);
        }
    } // if( !errResultInfo.isErrorResult() )

    return ErrResultSuccess;

} // save

//------------------------------------------------------------------------------
SErrResultInfo CIdxTreeTrcAdminObjs::recall( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_eTrcDetailLevel >= ETraceDetailLevelMethodCalls::ArgsNormal )
    {
        strAddTrcInfo = i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ ETraceDetailLevelMethodCalls::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "recall",
        /* strMethodInArgs    */ strAddTrcInfo );

    CMutexLocker mtxLocker(m_pMtx);

    QString strMth = "recall";

    QString strAddErrInfo;

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QFile file;

    if( i_strAbsFilePath.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidFileName);
    }
    else
    {
        file.setFileName(i_strAbsFilePath);

        if( !file.open(QIODevice::ReadOnly) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileOpenForRead);
            errResultInfo.setAddErrInfoDscr(i_strAbsFilePath);
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        QXmlStreamReader xmlStreamReader(&file);

        QXmlStreamReader::TokenType xmlStreamTokenType;

        QString  strElemName;
        QString  strAttr;
        QString  strPath;
        QString  strNameSpace;
        QString  strClassName;
        QString  strObjName;
        QString  strThread;
        EEnabled enabled;
        QString  strDataFilter;

        ETraceDetailLevelMethodCalls eDetailLevelMethodCalls;
        ETraceDetailLevelRuntimeInfo eDetailLevelRuntimeInfo;

        xmlStreamTokenType = xmlStreamReader.readNext();

        if( xmlStreamTokenType != QXmlStreamReader::StartDocument )
        {
            xmlStreamReader.raiseError("Invalid XML document");
        }

        xmlStreamTokenType = xmlStreamReader.readNext();
        strElemName = xmlStreamReader.name().toString();

        if( xmlStreamTokenType != QXmlStreamReader::StartElement || strElemName != "TrcAdminObjs" )
        {
            xmlStreamReader.raiseError("Invalid XML method trace admin objects file (missing root node \"TrcAdminObjs\")");
        }

        if( !xmlStreamReader.hasError() )
        {
            while( !xmlStreamReader.atEnd() )
            {
                xmlStreamTokenType = xmlStreamReader.readNext();

                if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
                {
                    strElemName = xmlStreamReader.name().toString();

                    // For best performance start with the most frequently used element names ..

                    if( strElemName == "TrcAdminObj" )
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strNameSpace = "";
                            strClassName = "";
                            strObjName = "";
                            strThread = "";
                            enabled = EEnabled::Yes;
                            eDetailLevelMethodCalls = ETraceDetailLevelMethodCalls::None;
                            eDetailLevelRuntimeInfo = ETraceDetailLevelRuntimeInfo::None;
                            strDataFilter = "";

                            if( xmlStreamReader.attributes().hasAttribute("NameSpace") )
                            {
                                strNameSpace = xmlStreamReader.attributes().value("NameSpace").toString();
                            }
                            if( xmlStreamReader.attributes().hasAttribute("ClassName") )
                            {
                                strClassName = xmlStreamReader.attributes().value("ClassName").toString();
                            }
                            if( xmlStreamReader.attributes().hasAttribute("ObjName") )
                            {
                                strObjName = xmlStreamReader.attributes().value("ObjName").toString();
                            }
                            if( strNameSpace.isEmpty() && strClassName.isEmpty() && strObjName.isEmpty() )
                            {
                                xmlStreamReader.raiseError("Neither NameSpace nor ClassName nor ObjName defined");
                            }
                            else // if( !strNameSpace.isEmpty() || !strClassName.isEmpty() || !strObjName.isEmpty() )
                            {
                                strPath = buildPathStr(strNameSpace, strClassName, strObjName);

                                if( xmlStreamReader.attributes().hasAttribute("Thread") )
                                {
                                    strThread = xmlStreamReader.attributes().value("Thread").toString();
                                }
                                if( xmlStreamReader.attributes().hasAttribute("Enabled") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("Enabled").toString();
                                    enabled = CEnumEnabled::toEnumerator(strAttr);
                                    if( enabled == EEnabled::Undefined )
                                    {
                                        xmlStreamReader.raiseError("Attribute \"Enabled\" (" + strAttr + ") for \"" + strPath + "\" is out of range");
                                    }
                                }
                                if( xmlStreamReader.attributes().hasAttribute("MethodCallsDetailLevel") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("MethodCallsDetailLevel").toString();
                                    eDetailLevelMethodCalls = CEnumTraceDetailLevelMethodCalls::toEnumerator(strAttr);
                                    if( eDetailLevelMethodCalls == ETraceDetailLevelMethodCalls::Undefined )
                                    {
                                        xmlStreamReader.raiseError("Attribute \"MethodCallsDetailLevel\" (" + strAttr + ") for \"" + strPath + "\" is out of range");
                                    }
                                }
                                if( xmlStreamReader.attributes().hasAttribute("RuntimeInfoDetailLevel") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("RuntimeInfoDetailLevel").toString();
                                    eDetailLevelRuntimeInfo = CEnumTraceDetailLevelRuntimeInfo::toEnumerator(strAttr);
                                    if( eDetailLevelRuntimeInfo == ETraceDetailLevelRuntimeInfo::Undefined )
                                    {
                                        xmlStreamReader.raiseError("Attribute \"RuntimeInfoDetailLevel\" (" + strAttr + ") for \"" + strPath + "\" is out of range");
                                    }
                                }
                                if( xmlStreamReader.attributes().hasAttribute("DataFilter") )
                                {
                                    strDataFilter = xmlStreamReader.attributes().value("DataFilter").toString();
                                }

                                CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(
                                    strNameSpace, strClassName, strObjName, enabled,
                                    eDetailLevelMethodCalls, eDetailLevelRuntimeInfo, strDataFilter, false);

                                pTrcAdminObj->setObjectThreadName(strThread);

                            } // else // if( !strNameSpace.isEmpty() || !strClassName.isEmpty() || !strObjName.isEmpty() )
                        } // if( xmlStreamReader.isStartElement() )
                    } // if( strElemName == "TrcAdminObj" )
                } // if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
            } // while( !xmlStreamReader.atEnd() )
        } // if( !xmlStreamReader.hasError() )

        if( xmlStreamReader.hasError() )
        {
            strAddErrInfo  = xmlStreamReader.errorString() + " on reading " + i_strAbsFilePath + " ";
            strAddErrInfo += "Line:" + QString::number(xmlStreamReader.lineNumber()) + ", ";
            strAddErrInfo += "Column:" + QString::number(xmlStreamReader.columnNumber());

            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileReadContent);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( !errResultInfo.isErrorResult() )

    return errResultInfo;

} // recall

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CIdxTreeTrcAdminObjs::save(
    QXmlStreamWriter& i_xmlStreamWriter,
    CIdxTreeEntry*    i_pTreeEntry ) const
//------------------------------------------------------------------------------
{
    // This is a protected method which may only be called through the public
    // methods. The mutex to protect the list and tree has already been locked.

    CIdxTreeEntry* pTreeEntry;
    CTrcAdminObj*  pTrcAdminObj;
    int            idxEntry;

    if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
    {
        pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(i_pTreeEntry);

        i_xmlStreamWriter.writeStartElement("TrcAdminObj");
        i_xmlStreamWriter.writeAttribute( "NameSpace", pTrcAdminObj->getNameSpace() );
        i_xmlStreamWriter.writeAttribute( "ClassName", pTrcAdminObj->getClassName() );
        i_xmlStreamWriter.writeAttribute( "ObjName", pTrcAdminObj->getObjectName() );
        i_xmlStreamWriter.writeAttribute( "Thread", pTrcAdminObj->getObjectThreadName() );
        i_xmlStreamWriter.writeAttribute( "Enabled", CEnumEnabled::toString(pTrcAdminObj->getEnabled()) );
        i_xmlStreamWriter.writeAttribute( "MethodCallsDetailLevel", CEnumTraceDetailLevelMethodCalls::toString(pTrcAdminObj->getMethodCallsTraceDetailLevel()) );
        i_xmlStreamWriter.writeAttribute( "RuntimeInfoDetailLevel", CEnumTraceDetailLevelRuntimeInfo::toString(pTrcAdminObj->getRuntimeInfoTraceDetailLevel()) );
        i_xmlStreamWriter.writeAttribute( "DataFilter", pTrcAdminObj->getTraceDataFilter() );
        i_xmlStreamWriter.writeEndElement(/*"TrcAdminObj"*/);
    }
    else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
    {
        for( idxEntry = 0; idxEntry < i_pTreeEntry->count(); ++idxEntry )
        {
            pTreeEntry = i_pTreeEntry->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                save(i_xmlStreamWriter, pTreeEntry);
            }
        }
    }

} // save

//------------------------------------------------------------------------------
/*! @brief Removes all empty branches beginning with the passed branch name
           walking the tree upwards in direction to root node.

    A branch will be removed if the branch has no child anymore.

    @param i_strBranchPath [in] Path of the branch to be checked whether it
        or its parent branches can be removed.
*/
void CIdxTreeTrcAdminObjs::removeEmptyBranches( const QString& i_strBranchPath )
//------------------------------------------------------------------------------
{
    CIdxTreeEntry* pBranch = findBranch(i_strBranchPath);

    while( pBranch != nullptr && pBranch->count() == 0 )
    {
        CIdxTreeEntry* pBranchParent = pBranch->parentBranch();
        remove(pBranch);
        delete pBranch;
        pBranch = pBranchParent;
    }
}
