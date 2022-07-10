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

#include "ZSSys/ZSSysLoggerIdxTree.h"
#include "ZSSys/ZSSysLogger.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMutex.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
class CIdxTreeLoggers : public ZS::System::CIdxTree
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CIdxTreeLoggers::CIdxTreeLoggers( const QString& i_strObjName, QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    CIdxTree(
        /* strIdxTreeName   */ i_strObjName,
        /* pRootTreeEntry   */ nullptr,
        /* strNodeSeparator */ "::",
        /* bCreateMutex     */ true,
        /* pObjParent       */ i_pObjParent )
{
}

//------------------------------------------------------------------------------
CIdxTreeLoggers::~CIdxTreeLoggers()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // instance methods to get and release admin objects
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the logger with the given name space, class name and object name.

    If a logger with the given name space, class name and object name is not yet
    existing in the index tree a new object is created and added to the index tree.

    If a logger with the given name space, class name and object name is already
    existing a reference counter will be incremented and the already existing
    logger is returned.

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
        If not Undefined the enabled flag will be set as the logger.
    @param i_eDefaultDetailLevel [in]
        If not Undefined the detail level will be set at the logger.
    @param i_strDefaultDataFilter [in]
        If not null (QString()) the data filter will be set at the logger.
    @param i_bIncrementRefCount [in]
        true to increment the reference counter of the logger.
        false is only used if the loggers are recalled from file as
        at that point only the tree entries are existing as place holders
        but the loggers are not yet referenced.

    @return Pointer to allocated logger or nullptr on error.
*/
CLogger* CIdxTreeLoggers::getLogger(
    const QString&               i_strNameSpace,
    const QString&               i_strClassName,
    const QString&               i_strObjName,
    ZS::System::EEnabled         i_bEnabledAsDefault,
    ELogDetailLevel i_eDefaultDetailLevel,
    const QString&               i_strDefaultDataFilter,
    bool                         i_bIncrementRefCount )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CLogger* pLogger = nullptr;

    if( i_strObjName.isEmpty() && i_strClassName.isEmpty() && i_strNameSpace.isEmpty() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "getLogger",
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
            pLogger = dynamic_cast<CLogger*>(pLeave);
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

            pLogger = new CLogger(i_strNameSpace, i_strClassName, i_strObjName, strLeaveName);

            EEnabled bEnabled = EEnabled::Yes;
            ELogDetailLevel eDetailLevel = ELogDetailLevel::None;
            QString strDataFilter = "";

            if( i_bEnabledAsDefault != EEnabled::Undefined )
            {
                bEnabled = i_bEnabledAsDefault;
            }
            if( i_eDefaultDetailLevel != ELogDetailLevel::Undefined )
            {
                eDetailLevel = i_eDefaultDetailLevel;
            }
            if( !i_strDefaultDataFilter.isNull() )
            {
                strDataFilter = i_strDefaultDataFilter;
            }

            pLogger->setEnabled(bEnabled);
            pLogger->setDetailLevel(eDetailLevel);
            pLogger->setDataFilter(strDataFilter);

            add(pLogger, strParentBranchPath);

        } // if( pLeave == nullptr )

        if( i_bIncrementRefCount )
        {
            pLogger->incrementRefCount();
        }
    } // if( !i_strObjName.isEmpty() || !i_strClassName.isEmpty() || !i_strNameSpace.isEmpty() )

    return pLogger;

} // getLogger

//------------------------------------------------------------------------------
/*! @brief Returns the logger at the given tree index.

    @param i_idxInTree [in] Index of the logger.
    @param i_bIncrementRefCount [in]
        true to increment the reference counter of the logger.
        false is used on clients side on receiving data.

    @return Pointer to allocated logger or nullptr on error.
*/
CLogger* CIdxTreeLoggers::getLogger( int i_idxInTree, bool i_bIncrementRefCount )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CLogger* pLogger = dynamic_cast<CLogger*>(getEntry(i_idxInTree));

    if( pLogger != nullptr && i_bIncrementRefCount )
    {
        pLogger->incrementRefCount();
    }
    return pLogger;
}

//------------------------------------------------------------------------------
/*! @brief Renames the given logger by replacing the given input pointer with
           the address of the newly referenced logger.

    If the given logger will no longer be referenced it will be destroyed
    (unless locked).

    If at the new position already a logger object is existing the reference
    to this logger will be returned.

    If at the new position no logger is existing a new object is created and
    the address of the newly created object is returned.

    @param io_ppLogger [in, out]
        In:  Pointer to logger which should be renamed. The reference counter
             of this object is decremented. If 0 and if the logger is not locked
             the object will be destroyed.
        Out: Pointer to logger at the new position. This might either be an
             already existing logger whose reference counter is increased or a
             newly created object.
    @param i_strNewObjName [in] New object name.
*/
void CIdxTreeLoggers::renameLogger(
    CLogger**      io_ppLogger,
    const QString& i_strNewObjName )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CLogger* pLogger = *io_ppLogger;

    if( pLogger == nullptr)
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "renameLogger",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "No Logger defined");

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else if( i_strNewObjName.isEmpty() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "renameLogger",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "New ObjectName is empty");

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else if( pLogger->getObjectName().isEmpty() )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "renameLogger",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "Cannot rename an object with an empty object name");

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else if( pLogger->getRefCount() < 1 )
    {
        SErrResultInfo errResultInfo(
            /* errSource     */ nameSpace(), className(), objectName(), "renameLogger",
            /* result        */ EResultArgOutOfRange,
            /* severity      */ EResultSeverityError,
            /* strAddErrInfo */ "Only instances referencing the logger may rename the object");

        if( CErrLog::GetInstance() != nullptr )
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else if( pLogger->getObjectName() != i_strNewObjName )
    {
        QString strNameSpace  = pLogger->getNameSpace();
        QString strClassName  = pLogger->getClassName();
        QString strOldObjName = pLogger->getObjectName();
        EEnabled bEnabled     = pLogger->getEnabled();
        ELogDetailLevel eDetailLevel = pLogger->getDetailLevel();
        QString strDataFilter = pLogger->getDataFilter();

        // The reference counter of the previously referenced object is decremented.
        pLogger->decrementRefCount();

        // If the objects reference counter is greater than 0 all other modules must
        // still refer to the unchanged logger.

        // If no longer referenced ...
        if( pLogger->getRefCount() == 0 )
        {
            // .. the object will be deleted and removed from the tree.
            if( pLogger->isLocked() )
            {
                pLogger->setDeleteOnUnlock(true);
            }
            else
            {
                delete pLogger;
                pLogger = nullptr;
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

        // If already a logger with the name space, class name and new object name is existing ..
        if( pLeaveNew != nullptr )
        {
            pLogger = dynamic_cast<CLogger*>(pLeaveNew);
            pLogger->incrementRefCount();
        }
        // If no logger with the name space, class name and new object name is existing ..
        else // if( pLeaveNew == nullptr )
        {
            // A new logger has to be created and returned.
            pLogger = getLogger(
                strNameSpace, strClassName, i_strNewObjName, bEnabled,
                eDetailLevel, strDataFilter);
        }

        pLogger->setObjectName(i_strNewObjName);

    } // if( pLogger != nullptr && !i_strNewObjName.isEmpty() )

    *io_ppLogger = pLogger;

} // renameLogger

//------------------------------------------------------------------------------
/*! @brief Releases the given logger.

    If not marked as to be deleted on releasing only the reference counter of
    the logger will be decremented and the logger will neither be removed from
    the tree nor destroyed.

    @param i_pLogger [in] Pointer to logger to be released.
*/
void CIdxTreeLoggers::releaseLogger( CLogger* i_pLogger )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pLogger != nullptr )
    {
        if( i_pLogger->deleteOnUnlock() )
        {
            i_pLogger->setDeleteOnUnlock(false); // to avoid an entry into the error log
            delete i_pLogger;
            i_pLogger = nullptr;
        }
        else
        {
            i_pLogger->decrementRefCount();
        }
    }
}

/*==============================================================================
public: // instance methods to insert branch nodes and admin objects
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief This method is used by log clients to insert a branch at the given
           index in the tree.

    The branch becomes a child of the branch given by parameter i_iParentBranchIdxInTree.

    @param i_iParentBranchIdxInTree [in] Tree entry index of the parent entry.
    @param i_strBranchName [in] Branch name to be created.
    @param i_idxInTree [in] Tree entry index at which the new branch will be inserted.

    @return Pointer to allocated logger or nullptr on error.
*/
CIdxTreeEntry* CIdxTreeLoggers::insertBranch(
    int            i_iParentBranchIdxInTree,
    const QString& i_strBranchName,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
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
/*! @brief This method is used by log clients to insert a logger at the given
           index in the tree.

    If there is no logger at the given index a new object is created with the
    given name space, class name and object name and inserted in the index
    tree at the index given by the object id.

    If there is already an object at the given index with a different name space,
    class name or object name nullptr is returned and an err log entry is created.

    As this method is used by log clients the reference counter of the logger
    is !! NOT !! incremented.

    The logger becomes a child of the branch given by parameter
    i_iParentBranchIdxInTree.

    @param i_iParentBranchIdxInTree [in] Tree entry index of the parent entry.
    @param i_strNameSpace [in] Name space of the logger.
    @param i_strClassName [in] Class name of the logger.
    @param i_strObjName [in] Object name of the logger.
    @param i_idxInTree [in] Tree entry index at which the new object will be inserted.

    @return Pointer to allocated logger or nullptr on error.
*/
CLogger* CIdxTreeLoggers::insertLogger(
    int            i_iParentBranchIdxInTree,
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CLogger* pLogger = nullptr;

    QString strMth = "insertLogger";
    QString strAddErrInfo;

    CIdxTreeEntry* pBranchParent = nullptr;

    if( i_iParentBranchIdxInTree >= 0 )
    {
        pBranchParent = getEntry(i_iParentBranchIdxInTree);

        if( pBranchParent == nullptr )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                strAddErrInfo = "Cannot add logger " + i_strObjName + " with tree index " + QString::number(i_idxInTree);
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
                strAddErrInfo = "Cannot add logger " + i_strObjName + " with tree index " + QString::number(i_idxInTree);
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
            pLogger = new CLogger(i_strNameSpace, i_strClassName, i_strObjName, strLeaveName);
            insert(pLogger, pBranchParent, -1, i_idxInTree);
        }
    } // if( i_idxInTree >= 0 )

    return pLogger;

} // insertTraceAdminObj

/*==============================================================================
public: // instance methods to recursively modify admin objects via object index of node entries
==============================================================================*/

//------------------------------------------------------------------------------
void CIdxTreeLoggers::setEnabled( int i_iIdxInTree, EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = getEntry(i_iIdxInTree);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            setEnabled(pTreeEntry, i_enabled);
        }
        else if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);

            if( pLogger != nullptr )
            {
                pLogger->setEnabled(i_enabled);
            }
        }
    }
}

//------------------------------------------------------------------------------
void CIdxTreeLoggers::setDetailLevel(
    int i_iIdxInTree, ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = getEntry(i_iIdxInTree);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            setDetailLevel(pTreeEntry, i_eDetailLevel);
        }
        else if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);

            if( pLogger != nullptr )
            {
                pLogger->setDetailLevel(i_eDetailLevel);
            }
        }
    }
}

//------------------------------------------------------------------------------
void CIdxTreeLoggers::setDataFilter(
    int i_iIdxInTree, const QString& i_strDataFilter )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = getEntry(i_iIdxInTree);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            setDataFilter(pTreeEntry, i_strDataFilter);
        }
        else if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            CLogger* pLogger = dynamic_cast<CLogger*>(pTreeEntry);

            if( pLogger != nullptr )
            {
                pLogger->setDataFilter(i_strDataFilter);
            }
        }
    }
}

/*==============================================================================
public: // instance methods to recursively modify objects via namespace node entries
==============================================================================*/

//------------------------------------------------------------------------------
void CIdxTreeLoggers::setEnabled( CIdxTreeEntry* i_pBranch, EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pBranch != nullptr )
    {
        CIdxTreeEntry* pTreeEntry;
        CLogger*       pLogger;
        int            idxEntry;

        for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = i_pBranch->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                {
                    pLogger = dynamic_cast<CLogger*>(pTreeEntry);

                    if( pLogger != nullptr )
                    {
                        if( pLogger->getEnabled() != i_enabled )
                        {
                            pLogger->setEnabled(i_enabled);
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
}

//------------------------------------------------------------------------------
void CIdxTreeLoggers::setDetailLevel(
    CIdxTreeEntry* i_pBranch, ELogDetailLevel i_eDetailLevel )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pBranch != nullptr )
    {
        CIdxTreeEntry* pTreeEntry;
        CLogger*       pLogger;
        int            idxEntry;

        for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = i_pBranch->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                {
                    pLogger = dynamic_cast<CLogger*>(pTreeEntry);

                    if( pLogger != nullptr )
                    {
                        if( pLogger->getDetailLevel() != i_eDetailLevel )
                        {
                            pLogger->setDetailLevel(i_eDetailLevel);
                        }
                    }
                }
                else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
                {
                    setDetailLevel(pTreeEntry, i_eDetailLevel);
                }
            }
        } // for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
    } // if( i_pBranch != nullptr )
}

//------------------------------------------------------------------------------
void CIdxTreeLoggers::setDataFilter(
    CIdxTreeEntry* i_pBranch, const QString& i_strDataFilter )
//------------------------------------------------------------------------------
{
    CMutexLocker mtxLocker(m_pMtx);

    if( i_pBranch != nullptr )
    {
        CIdxTreeEntry* pTreeEntry;
        CLogger*       pLogger;
        int            idxEntry;

        for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
        {
            pTreeEntry = i_pBranch->at(idxEntry);

            if( pTreeEntry != nullptr )
            {
                if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
                {
                    pLogger = dynamic_cast<CLogger*>(pTreeEntry);

                    if( pLogger != nullptr )
                    {
                        if( pLogger->getDataFilter() != i_strDataFilter )
                        {
                            pLogger->setDataFilter(i_strDataFilter);
                        }
                    }
                }
                else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
                {
                    setDataFilter(pTreeEntry, i_strDataFilter);
                }
            }
        } // for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
    } // if( i_pBranch != nullptr )
} // setTraceDataFilter

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CIdxTreeLoggers::save( const QString& i_strAbsFilePath ) const
//------------------------------------------------------------------------------
{
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
        xmlStreamWriter.writeStartElement("Loggers");

        save(xmlStreamWriter, m_pRoot);

        xmlStreamWriter.writeEndElement();  // Method Trace Admin Objects
        xmlStreamWriter.writeEndDocument();

        if( !errResultInfo.isErrorResult() )
        {
            file.write(byteArrSchema);
        }
    } // if( !errResultInfo.isErrorResult() )

    return ErrResultSuccess;
}

//------------------------------------------------------------------------------
SErrResultInfo CIdxTreeLoggers::recall( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
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

        ELogDetailLevel eDetailLevel;

        xmlStreamTokenType = xmlStreamReader.readNext();

        if( xmlStreamTokenType != QXmlStreamReader::StartDocument )
        {
            xmlStreamReader.raiseError("Invalid XML document");
        }

        xmlStreamTokenType = xmlStreamReader.readNext();
        strElemName = xmlStreamReader.name().toString();

        if( xmlStreamTokenType != QXmlStreamReader::StartElement || strElemName != "Loggers" )
        {
            xmlStreamReader.raiseError("Invalid XML logger objects file (missing root node \"Loggers\")");
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

                    if( strElemName == "Logger" )
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            strNameSpace = "";
                            strClassName = "";
                            strObjName = "";
                            strThread = "";
                            enabled = EEnabled::Yes;
                            eDetailLevel = ELogDetailLevel::None;
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
                                if( xmlStreamReader.attributes().hasAttribute("DetailLevel") )
                                {
                                    strAttr = xmlStreamReader.attributes().value("DetailLevel").toString();
                                    eDetailLevel = CEnumLogDetailLevel::toEnumerator(strAttr);
                                    if( eDetailLevel == ELogDetailLevel::Undefined )
                                    {
                                        xmlStreamReader.raiseError("Attribute \"DetailLevel\" (" + strAttr + ") for \"" + strPath + "\" is out of range");
                                    }
                                }
                                if( xmlStreamReader.attributes().hasAttribute("DataFilter") )
                                {
                                    strDataFilter = xmlStreamReader.attributes().value("DataFilter").toString();
                                }

                                CLogger* pLogger = getLogger(
                                    strNameSpace, strClassName, strObjName, enabled,
                                    eDetailLevel, strDataFilter, false);

                                pLogger->setObjectThreadName(strThread);

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
void CIdxTreeLoggers::save(
    QXmlStreamWriter& i_xmlStreamWriter,
    CIdxTreeEntry*    i_pTreeEntry ) const
//------------------------------------------------------------------------------
{
    // This is a protected method which may only be called through the public
    // methods. The mutex to protect the list and tree has already been locked.

    CIdxTreeEntry* pTreeEntry;
    CLogger*  pLogger;
    int            idxEntry;

    if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
    {
        pLogger = dynamic_cast<CLogger*>(i_pTreeEntry);

        i_xmlStreamWriter.writeStartElement("Logger");
        i_xmlStreamWriter.writeAttribute( "NameSpace", pLogger->getNameSpace() );
        i_xmlStreamWriter.writeAttribute( "ClassName", pLogger->getClassName() );
        i_xmlStreamWriter.writeAttribute( "ObjName", pLogger->getObjectName() );
        i_xmlStreamWriter.writeAttribute( "Thread", pLogger->getObjectThreadName() );
        i_xmlStreamWriter.writeAttribute( "Enabled", CEnumEnabled::toString(pLogger->getEnabled()) );
        i_xmlStreamWriter.writeAttribute( "DetailLevel", CEnumLogDetailLevel::toString(pLogger->getDetailLevel()) );
        i_xmlStreamWriter.writeAttribute( "DataFilter", pLogger->getDataFilter() );
        i_xmlStreamWriter.writeEndElement(/*"Logger"*/);
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

    @param i_strBranchPath [in]
        Path of the branch to be checked whether it or its parent branches
        can be removed.
*/
void CIdxTreeLoggers::removeEmptyBranches( const QString& i_strBranchPath )
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
