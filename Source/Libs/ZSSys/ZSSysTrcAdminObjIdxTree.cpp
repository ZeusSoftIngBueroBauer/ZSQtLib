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
    const QString& i_strObjName,
    QObject*       i_pObjParent,
    int            i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    CIdxTree(
        /* strIdxTreeName    */ i_strObjName,
        /* pRootTreeEntry    */ nullptr,
        /* strNodeSeparator  */ "::",
        /* bCreateMutex      */ true,
        /* pObjParent        */ i_pObjParent,
        /* i_iTrcDetailLevel */ i_iTrcDetailLevel )
{
    QString strAddTrcInfo;

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
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
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
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
/*! Returns the trace admin object with the given name space, class name and
    object name.

    If an admin object with the given name space, class name and object name
    is not yet existing in the index tree a new object is created and added to
    the index tree.
    If an admin object with the given name space, class name and object name
    is already existing a reference counter will be incremented and the already
    existing admin object is returned.
*/
CTrcAdminObj* CIdxTreeTrcAdminObjs::getTraceAdminObj(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    bool           i_bIncrementRefCount )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strAddTrcInfo  = "NameSpace: " + i_strNameSpace;
        strAddTrcInfo += ", ClassName: " + i_strClassName;
        strAddTrcInfo += ", ObjName: " + i_strObjName;
        strAddTrcInfo += ", IncrementRefCount: " + bool2Str(i_bIncrementRefCount);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "getTraceAdminObj",
        /* strMethodInArgs    */ strAddTrcInfo );

    QString strParentBranchPath = buildPathStr(i_strNameSpace, i_strClassName);

    CIdxTreeEntry* pLeave = findLeave(strParentBranchPath, i_strObjName);

    CTrcAdminObj* pTrcAdminObj = nullptr;

    if( pLeave != nullptr )
    {
        pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pLeave);
    }
    else // if( pLeave == nullptr )
    {
        QStringList strlstBranchNames = strParentBranchPath.split(m_strNodeSeparator);
        QString     strParentPath;
        QString     strPath;

        CIdxTreeEntry* pBranch;

        for( auto& strBranchName : strlstBranchNames )
        {
            strPath = buildPathStr(strPath, strBranchName);

            pBranch = findBranch(strPath);

            if( pBranch == nullptr )
            {
                pBranch = createBranch(strBranchName);
                add(pBranch, strParentPath);
            }
            strParentPath = buildPathStr(strParentPath, strBranchName);
        }

        pTrcAdminObj = new CTrcAdminObj(i_strObjName);

        add(pTrcAdminObj, strParentBranchPath);

    } // if( pLeave == nullptr )

    if( i_bIncrementRefCount )
    {
        pTrcAdminObj->incrementRefCount();
    }

    return pTrcAdminObj;

} // getTraceAdminObj

//------------------------------------------------------------------------------
/*! Returns the trace admin object at the given tree index.
*/
CTrcAdminObj* CIdxTreeTrcAdminObjs::getTraceAdminObj( int i_idxInTree ) const
//------------------------------------------------------------------------------
{
    return dynamic_cast<CTrcAdminObj*>(getEntry(i_idxInTree));
}

//------------------------------------------------------------------------------
void CIdxTreeTrcAdminObjs::releaseTraceAdminObj( CTrcAdminObj* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strAddTrcInfo = "TrcAdminObj: " + QString(i_pTrcAdminObj == nullptr ? "nullptr" : i_pTrcAdminObj->path());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "releaseTraceAdminObj",
        /* strMethodInArgs    */ strAddTrcInfo );

    if( i_pTrcAdminObj != nullptr )
    {
        i_pTrcAdminObj->decrementRefCount();
    }

} // releaseTraceAdminObj

/*==============================================================================
public: // instance methods to insert branch nodes and admin objects
==============================================================================*/

//------------------------------------------------------------------------------
/*! Inserts a branch into the index tree.

*/
CIdxTreeEntry* CIdxTreeTrcAdminObjs::insertBranch(
    int            i_iParentBranchIdxInTree,
    const QString& i_strBranchName,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strAddTrcInfo  = "ParentBranchIdxInTree: " + QString::number(i_iParentBranchIdxInTree);
        strAddTrcInfo += ", Name: " + i_strBranchName;
        strAddTrcInfo += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insertBranch",
        /* strMethodInArgs    */ strAddTrcInfo );

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
/*! Returns the trace admin object at the index given by the object id.

    If there is no admin object at the given index a new object is created
    with the given name space, class name and object name and inserted in the
    index tree at the index given by the object id.

    If there is already an object at the given index with a different name space,
    class name or object name nullptr is returned and an err log entry is created.
*/
CTrcAdminObj* CIdxTreeTrcAdminObjs::insertTraceAdminObj(
    int            i_iParentBranchIdxInTree,
    const QString& i_strObjName,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strAddTrcInfo  = "ParentBranchIdxInTree: " + QString::number(i_iParentBranchIdxInTree);
        strAddTrcInfo += ", ObjName: " + i_strObjName;
        strAddTrcInfo += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insertTraceAdminObj",
        /* strMethodInArgs    */ strAddTrcInfo );

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
            pTrcAdminObj = new CTrcAdminObj(i_strObjName);
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
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strAddTrcInfo  = "ObjId: " + QString::number(i_iObjId);
        strAddTrcInfo += ", Enabled: " + CEnumEnabled::toString(i_enabled);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setEnabled",
        /* strMethodInArgs    */ strAddTrcInfo );

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
void CIdxTreeTrcAdminObjs::setTraceDetailLevel( int i_iObjId, int i_iDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strAddTrcInfo  = "ObjId: " + QString::number(i_iObjId);
        strAddTrcInfo += ", DetailLevel: " + QString::number(i_iDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setTraceDetailLevel",
        /* strMethodInArgs    */ strAddTrcInfo );

    CIdxTreeEntry* pTreeEntry = getEntry(i_iObjId);

    if( pTreeEntry != nullptr )
    {
        if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            setTraceDetailLevel(pTreeEntry, i_iDetailLevel);
        }
        else if( pTreeEntry->entryType() == EIdxTreeEntryType::Leave )
        {
            CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pTreeEntry);

            if( pTrcAdminObj != nullptr )
            {
                pTrcAdminObj->setTraceDetailLevel(i_iDetailLevel);
            }
        }
    }

} // setTraceDetailLevel

/*==============================================================================
public: // instance methods to recursively modify admin objects via namespace node entries
==============================================================================*/

//------------------------------------------------------------------------------
void CIdxTreeTrcAdminObjs::setEnabled( CIdxTreeEntry* i_pBranch, EEnabled i_enabled )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strAddTrcInfo  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->path());
        strAddTrcInfo += ", Enabled: " + CEnumEnabled::toString(i_enabled);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setEnabled",
        /* strMethodInArgs    */ strAddTrcInfo );

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
void CIdxTreeTrcAdminObjs::setTraceDetailLevel( CIdxTreeEntry* i_pBranch, int i_iDetailLevel )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strAddTrcInfo  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->path());
        strAddTrcInfo += ", DetailLevel: " + QString::number(i_iDetailLevel);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setTraceDetailLevel",
        /* strMethodInArgs    */ strAddTrcInfo );

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
                        if( pTrcAdminObj->getTraceDetailLevel() != i_iDetailLevel )
                        {
                            pTrcAdminObj->setTraceDetailLevel(i_iDetailLevel);
                        }
                    }
                }
                else // if( pTreeEntry->entryType() == EIdxTreeEntryType::Root || Branch )
                {
                    setTraceDetailLevel(pTreeEntry, i_iDetailLevel );
                }
            }
        } // for( idxEntry = 0; idxEntry < i_pBranch->count(); ++idxEntry )
    } // if( i_pBranch != nullptr )

} // setTraceDetailLevel

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CIdxTreeTrcAdminObjs::save( const QString& i_strAbsFilePath ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strAddTrcInfo = i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "save",
        /* strMethodInArgs    */ strAddTrcInfo );

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
    QMutexLocker mtxLocker(m_pMtx);

    QString strAddTrcInfo;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strAddTrcInfo = i_strAbsFilePath;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ dynamic_cast<CTrcServer*>(parent()), // may be nullptr if the parent is not the trace server
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "recall",
        /* strMethodInArgs    */ strAddTrcInfo );

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
        QXmlStreamReader            xmlStreamReader(&file);
        QXmlStreamReader::TokenType xmlStreamTokenType;
        QString                     strElemName;
        QString                     strAttr;
        QString                     strPath;
        QString                     strNameSpace;
        QString                     strClassName;
        QString                     strObjName;
        QString                     strThread;
        EEnabled                    enabled;
        int                         iDetailLevel;

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
                            iDetailLevel = ETraceDetailLevelNone;

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
                                    iDetailLevel = str2TraceDetailLevel(strAttr);
                                    if( iDetailLevel == ETraceDetailLevelUndefined )
                                    {
                                        xmlStreamReader.raiseError("Attribute \"DetailLevel\" (" + strAttr + ") for \"" + strPath + "\" is out of range");
                                    }
                                }

                                CTrcAdminObj* pTrcAdminObj = getTraceAdminObj(strNameSpace, strClassName, strObjName, false);

                                pTrcAdminObj->setObjectThreadName(strThread);
                                pTrcAdminObj->setEnabled(enabled);
                                pTrcAdminObj->setTraceDetailLevel(iDetailLevel);

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
        i_xmlStreamWriter.writeAttribute( "DetailLevel", traceDetailLevel2Str(pTrcAdminObj->getTraceDetailLevel()) );
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
