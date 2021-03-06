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

#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;


/*******************************************************************************
class CIdxTree : public QObject
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/* class iterator
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the current tree entry the iterator points to.

    @return Pointer to tree entry.
*/
CAbstractIdxTreeEntry* CIdxTree::iterator::operator * () const
//------------------------------------------------------------------------------
{
    return m_pTreeEntryCurr;
}

//------------------------------------------------------------------------------
/*! Compares this iterator with other for equality.

    @param i_other [in] Iterator to be compared with.

    @return true if this and other iteratore points to the same element - false otherwise.
*/
bool CIdxTree::iterator::operator == ( const iterator& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_pIdxTree != i_other.m_pIdxTree )
    {
        bEqual = false;
    }
    else if( m_pTreeEntryCurr != i_other.m_pTreeEntryCurr )
    {
        bEqual = false;
    }
    else if( m_traversalOrder != ETraversalOrder::Undefined
         && i_other.m_traversalOrder != ETraversalOrder::Undefined
         && m_traversalOrder != i_other.m_traversalOrder )
    {
        bEqual = false;
    }
    return bEqual;

} // iterator::operator ==

//------------------------------------------------------------------------------
/*! Compares this iterator with other for equality.

    @param i_other [in] Iterator to be compared with.

    @return true if this and other iteratore points to the same element - false otherwise.
*/
bool CIdxTree::iterator::operator == ( iterator& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_pIdxTree != i_other.m_pIdxTree )
    {
        bEqual = false;
    }
    else if( m_pTreeEntryCurr != i_other.m_pTreeEntryCurr )
    {
        bEqual = false;
    }
    else if( m_traversalOrder != ETraversalOrder::Undefined
         && i_other.m_traversalOrder != ETraversalOrder::Undefined
         && m_traversalOrder != i_other.m_traversalOrder )
    {
        bEqual = false;
    }
    return bEqual;

} // iterator::operator ==

//------------------------------------------------------------------------------
/*! Compares this iterator with other for inequality.

    @param i_other [in] Iterator to be compared with.

    @return true if this and other iteratore are pointing to different elements - false otherwise.
*/
bool CIdxTree::iterator::operator != ( const iterator& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

//------------------------------------------------------------------------------
/*! Compares this iterator with other for inequality.

    @param i_other [in] Iterator to be compared with.

    @return true if this and other iteratore are pointing to different elements - false otherwise.
*/
bool CIdxTree::iterator::operator != ( iterator& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

//------------------------------------------------------------------------------
/*! Increment operator.

    Depending on the traversal order the next element will be evaluated.

    @return Pointer to next tree entry.
*/
CIdxTree::iterator& CIdxTree::iterator::operator ++ ()
//------------------------------------------------------------------------------
{
    if( m_pTreeEntryCurr == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError, "m_pTreeEntryCurr == nullptr");
    }

    CAbstractIdxTreeEntry* pTreeEntryNew = nullptr;

    if( m_traversalOrder == ETraversalOrder::Index )
    {
        int idxInTree = m_pTreeEntryCurr->indexInTree();

        for( ++idxInTree; idxInTree < m_pIdxTree->treeEntriesVec().size(); ++idxInTree )
        {
            pTreeEntryNew = m_pIdxTree->treeEntriesVec()[idxInTree];
            if( pTreeEntryNew != nullptr )
            {
                break;
            }
        }
    }
    else if( m_traversalOrder == ETraversalOrder::PreOrder )
    {
        if( m_pTreeEntryCurr == m_pIdxTree->root() )
        {
            CBranchIdxTreeEntry* pBranchCurr = m_pIdxTree->root();

            if( pBranchCurr->count() > 0 )
            {
                pTreeEntryNew = pBranchCurr->at(0);
            }
        }
        else // if( m_pTreeEntryCurr != m_pIdxTree->root() )
        {
            if( m_pTreeEntryCurr->entryType() == EIdxTreeEntryType::Leave )
            {
                int idxInParentBranch = m_pTreeEntryCurr->indexInParentBranch();
                CBranchIdxTreeEntry* pBranchParent = m_pTreeEntryCurr->parentBranch();

                if( idxInParentBranch >= (pBranchParent->count()-1) )
                {
                    idxInParentBranch = pBranchParent->indexInParentBranch();
                    pBranchParent = pBranchParent->parentBranch();

                    while( pBranchParent != nullptr && idxInParentBranch >= (pBranchParent->count()-1) )
                    {
                        idxInParentBranch = pBranchParent->indexInParentBranch();
                        pBranchParent = pBranchParent->parentBranch();
                    }
                } // if( idxInParentBranch >= (pBranchParent->count()-1) )

                if( pBranchParent != nullptr && idxInParentBranch < (pBranchParent->count()-1) )
                {
                    pTreeEntryNew = pBranchParent->at(idxInParentBranch+1);
                }
            }
            else // if( m_pTreeEntryCurr->entryType() == EIdxTreeEntryType::Branch )
            {
                CBranchIdxTreeEntry* pBranchCurr = dynamic_cast<CBranchIdxTreeEntry*>(m_pTreeEntryCurr);

                if( pBranchCurr->count() > 0 )
                {
                    pTreeEntryNew = pBranchCurr->at(0);
                }
                else
                {
                    int idxInParentBranch = pBranchCurr->indexInParentBranch();
                    CBranchIdxTreeEntry* pBranchParent = pBranchCurr->parentBranch();

                    while( pBranchParent != nullptr && idxInParentBranch >= (pBranchParent->count()-1) )
                    {
                        pBranchCurr = pBranchParent;
                        idxInParentBranch = pBranchCurr->indexInParentBranch();
                        pBranchParent = pBranchParent->parentBranch();
                    }

                    if( pBranchParent != nullptr && idxInParentBranch < (pBranchParent->count()-1) )
                    {
                        pTreeEntryNew = pBranchParent->at(idxInParentBranch+1);
                    }
                }
            }
        } // if( m_pTreeEntryCurr != m_pIdxTree->root() )
    } // if( m_traversalOrder == ETraversalOrder::PreOrder )

    m_pTreeEntryCurr = pTreeEntryNew;

    return *this;

} // operator ++

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an index tree instance.

    @param i_strObjName [in] Name of the index tree.
    @param i_pRootTreeEntry [in] Reference to the root entry of the index tree.
           Usually the tree creates the node itself. However, if the root node needs
           properties that go beyond the default implementation, a user-defined node
           can be created and passed here. The root node becomes the property of the
           tree and is destroyed along with the tree.
    @param i_strNodeSeparator [in] Delimiter (or string) used to separate the individual
           nodes within the tree. Based on the file system, a slash ("/") is used as the default.
           However, two consecutive colons ("::") are also useful. In C ++ those colons are
           used for the namespaces and class names and thus also for the Trace Admin object tree.
           If nodes are added to the tree, the separator must not appear in the name of the nodes.
    @param i_bCreateMutex [in] If the tree can be changed at runtime of the application and
           several threads access the tree at the same time, the methods of the index tree must
           be protected by a mutex.
    @param i_pObjParent [in] Parent object of the index tree. If a parent object is specified,
           the index tree is destroyed along with its parent object.
    @param i_iTrcDetailLevel [in] If greater than 0 (ETraceDetailLevelNone), method tracing for
           the index tree is activated. As trace admin objects itself are leaves of an index tree
           trace admin objects cannot be used for controlling the trace detail level of the index tree.
*/
CIdxTree::CIdxTree(
    const QString&     i_strObjName,
    CRootIdxTreeEntry* i_pRootTreeEntry,
    const QString&     i_strNodeSeparator,
    bool               i_bCreateMutex,
    QObject*           i_pObjParent,
    int                i_iTrcDetailLevel ) :
//------------------------------------------------------------------------------
    QObject(i_pObjParent),
    m_strNodeSeparator(i_strNodeSeparator),
    m_pMtx(nullptr),
    m_mappTreeEntries(),
    m_arpTreeEntries(),
    m_mapFreeIdxs(),
    m_pRoot(i_pRootTreeEntry),
    m_iTrcDetailLevel(i_iTrcDetailLevel)
{
    setObjectName(i_strObjName);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = i_strObjName;
        strMthInArgs += ", RootTreeEntry: " + QString(i_pRootTreeEntry == nullptr ? "nullptr" : i_pRootTreeEntry->name());
        strMthInArgs += ", NodeSeparator: " + i_strNodeSeparator;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_bCreateMutex )
    {
        m_pMtx = new QMutex(QMutex::Recursive);
    }

    if( m_pRoot == nullptr )
    {
        m_pRoot = new CRootIdxTreeEntry(i_strObjName);
    }

    // The root entry will neither be added to the list nor to the map of tree entries.
    m_pRoot->setTree(this);

} // ctor

//------------------------------------------------------------------------------
/*! Destroys the instance.

    The root node together with all child nodes will be destroyed.
*/
CIdxTree::~CIdxTree()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pMtx != nullptr )
    {
        m_pMtx->lock();
    }

    emit aboutToBeDestroyed(this);

    clear(m_pRoot);

    // The root entry has neither been added to the list nor to the map of tree entries.
    // We have to ensure that the destructor of the root entry does not try to remove
    // itself from the index tree by invoking the remove method.
    m_pRoot->setTree(nullptr);

    try
    {
        delete m_pRoot;
    }
    catch(...)
    {
    }

    if( m_pMtx != nullptr )
    {
        m_pMtx->unlock();
    }

    try
    {
        delete m_pMtx;
    }
    catch(...)
    {
    }

    m_strNodeSeparator.clear();
    m_pMtx = nullptr;
    m_mappTreeEntries.clear();
    m_arpTreeEntries.clear();
    m_mapFreeIdxs.clear();
    m_pRoot = nullptr;
    m_iTrcDetailLevel = 0;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Clears the tree by deleting and removing all entries but keeping only the
    root entry.
*/
void CIdxTree::clear()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "clear",
        /* strMethodInArgs    */ strMthInArgs );

    clear(m_pRoot);

} // clear

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Locks the tree and protects the tree from access by other threads.

    If a thread wants to modify a tree by applying several methods one after another
    and must rely on that the tree remains unchanged during that time of if a thread
    wants to iterate through the tree, the tree must be locked by this thread.

    @note A subsequent unlock call is required.
*/
void CIdxTree::lock()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "lock",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pMtx != nullptr )
    {
        m_pMtx->lock();
    }

} // lock

//------------------------------------------------------------------------------
/*! Removes a previous lock of the tree.
*/
void CIdxTree::unlock()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "unlock",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pMtx != nullptr )
    {
        m_pMtx->unlock();
    }

} // unlock

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Connects the passed single strings with the tree's node separator to a path string.

    Example:

        QString strPath = buildPathStr("User", "Data");
        // strPath = "User/Data"

    @param i_str1 [in] String 1.
    @param i_str3 [in] String 2.
*/
QString CIdxTree::buildPathStr( const QString& i_str1, const QString& i_str2 ) const
//------------------------------------------------------------------------------
{
    return ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2);
}

//------------------------------------------------------------------------------
/*! Connects the passed single strings with the tree's node separator to a path string.

    Example:

        QString strPath = buildPathStr("User", "Data", "My");
        // strPath = "User/Data/My"

    @param i_str1 [in] String 1.
    @param i_str2 [in] String 2.
    @param i_str3 [in] String 3.
*/
QString CIdxTree::buildPathStr( const QString& i_str1, const QString& i_str2, const QString& i_str3 ) const
//------------------------------------------------------------------------------
{
    return ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2, i_str3);
}

//------------------------------------------------------------------------------
/*! Connects the passed single strings with the tree's node separator to a path string.

    Example:

        QString strPath = buildPathStr("User", "Data", "My", "Special");
        // strPath = "User/Data/My/Special"

    @param i_str1 [in] String 1.
    @param i_str2 [in] String 2.
    @param i_str3 [in] String 3.
    @param i_str4 [in] String 4.
*/
QString CIdxTree::buildPathStr( const QString& i_str1, const QString& i_str2, const QString& i_str3, const QString& i_str4 ) const
//------------------------------------------------------------------------------
{
    return ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2, i_str3, i_str4);
}

//------------------------------------------------------------------------------
/*! Connects the passed single strings with the tree's node separator to a path string.

    Example:

        QString strPath = buildPathStr("User", "Data", "My", "Special", "Work");
        // strPath = "User/Data/My/Special/Work"

    @param i_str1 [in] String 1.
    @param i_str2 [in] String 2.
    @param i_str3 [in] String 3.
    @param i_str4 [in] String 4.
    @param i_str5 [in] String 5.
*/
QString CIdxTree::buildPathStr( const QString& i_str1, const QString& i_str2, const QString& i_str3, const QString& i_str4, const QString& i_str5 ) const
//------------------------------------------------------------------------------
{
    return ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2, i_str3, i_str4, i_str5);
}

//------------------------------------------------------------------------------
/*! Generates the key of the associated tree node from the given node type and string.

    Example:

        QString strKey = buildKeyInTreeStr(EIdxTreeEntryType::Leave, "User");
        // strKey = "L:User".

    @param i_entryType [in] Node type (Leave or Branch or Root).
    @param i_str1 [in] String 1.
*/
QString CIdxTree::buildKeyInTreeStr( EIdxTreeEntryType i_entryType, const QString& i_str1 ) const
//------------------------------------------------------------------------------
{
    return idxTreeEntryType2Str(i_entryType,EEnumEntryAliasStrSymbol) + ":" + i_str1;
}

//------------------------------------------------------------------------------
/*! Generates the key of the associated tree node from the given node type and the given single strings.

    Example:

        QString strKey = buildKeyInTreeStr(EIdxTreeEntryType::Leave, "User", "Data");
        // strKey = "L:User/Data".

    @param i_entryType [in] Node type (Leave or Branch or Root).
    @param i_str1 [in] String 1.
    @param i_str2 [in] String 2.
*/
QString CIdxTree::buildKeyInTreeStr( EIdxTreeEntryType i_entryType, const QString& i_str1, const QString& i_str2 ) const
//------------------------------------------------------------------------------
{
    return idxTreeEntryType2Str(i_entryType,EEnumEntryAliasStrSymbol) + ":" + ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2);
}

//------------------------------------------------------------------------------
/*! Generates the key of the associated tree node from the given node type and the given single strings.

    Example:

        QString strKey = buildKeyInTreeStr(EIdxTreeEntryType::Leave, "User", "Data", "My");
        // strKey = "L:User/Data/My".

    @param i_entryType [in] Node type (Leave or Branch or Root).
    @param i_str1 [in] String 1.
    @param i_str2 [in] String 2.
    @param i_str3 [in] String 3.
*/
QString CIdxTree::buildKeyInTreeStr( EIdxTreeEntryType i_entryType, const QString& i_str1, const QString& i_str2, const QString& i_str3 ) const
//------------------------------------------------------------------------------
{
    return idxTreeEntryType2Str(i_entryType,EEnumEntryAliasStrSymbol) + ":" + ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2, i_str3);
}

//------------------------------------------------------------------------------
/*! Generates the key of the associated tree node from the given node type and the given single strings.

    Example:

        QString strKey = buildKeyInTreeStr(EIdxTreeEntryType::Leave, "User", "Data", "My", "Special");
        // strKey = "L:User/Data/My/Special";

    @param i_entryType [in] Node type (Leave or Branch or Root).
    @param i_str1 [in] String 1.
    @param i_str2 [in] String 2.
    @param i_str3 [in] String 3.
    @param i_str4 [in] String 4.
*/
QString CIdxTree::buildKeyInTreeStr( EIdxTreeEntryType i_entryType, const QString& i_str1, const QString& i_str2, const QString& i_str3, const QString& i_str4 ) const
//------------------------------------------------------------------------------
{
    return idxTreeEntryType2Str(i_entryType,EEnumEntryAliasStrSymbol) + ":" + ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2, i_str3, i_str4);
}

//------------------------------------------------------------------------------
/*! Generates the key of the associated tree node from the given node type and the given single strings.

    Example:

        QString strKey = buildKeyInTreeStr(EIdxTreeEntryType::Leave, "User", "Data", "My", "Special", "Work");
        // strKey = "L:User/Data/My/Special/Work"

    @param i_entryType [in] Node type (Leave or Branch or Root).
    @param i_str1 [in] String 1.
    @param i_str2 [in] String 2.
    @param i_str3 [in] String 3.
    @param i_str4 [in] String 4.
    @param i_str5 [in] String 5.
*/
QString CIdxTree::buildKeyInTreeStr( EIdxTreeEntryType i_entryType, const QString& i_str1, const QString& i_str2, const QString& i_str3, const QString& i_str4, const QString& i_str5 ) const
//------------------------------------------------------------------------------
{
    return idxTreeEntryType2Str(i_entryType,EEnumEntryAliasStrSymbol) + ":" + ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2, i_str3, i_str4, i_str5);
}

//------------------------------------------------------------------------------
/*! Splitts the last substring from the given path and, if the path name contains
    the type of the node, returns the node type.

    Example:

        QString           strBranchPath;
        QString           strNodeName;
        EIdxTreeEntryType entryType = splitPathStr("L:User/Data/My/Special/Work", &strBranchPath, &strNodeName);

        // entryType     = EIdxTreeEntryType::Leave;
        // strBranchPath = "User/Data/My/Special"
        // strNodeName   = "Work"

    @param i_strPath [in] Path of the node, which can also contain the node type.
    @param o_pstrBranchPath [out] If not equal to nullptr, the path is returned here without the last substring.
    @param o_pstrName [out] If not equal to null tpr the name of the node is returned here.

    @return Undefined or valid node type if the path contained the node type.
*/
EIdxTreeEntryType CIdxTree::splitPathStr( const QString& i_strPath, QString* o_pstrBranchPath, QString* o_pstrName ) const
//------------------------------------------------------------------------------
{
    EIdxTreeEntryType entrType = EIdxTreeEntryType::Undefined;

    QStringList strlst;
    QString     strPath = i_strPath;
    QString     strEntryType;
    QString     strName;
    QString     strBranchPath;

    for( auto iEntryType = 0; iEntryType < static_cast<int>(EIdxTreeEntryType::Count); ++iEntryType )
    {
        strEntryType = idxTreeEntryType2Str(iEntryType, EEnumEntryAliasStrSymbol);

        if( strPath.startsWith(strEntryType+":",Qt::CaseInsensitive) )
        {
            strPath.remove(0, 2);
            break;
        }
        strEntryType = "";
    }

    strlst = strPath.split(m_strNodeSeparator);

    if( strlst.size() >= 1 )
    {
        strName = strlst.last();
        strlst.removeLast();
    }
    if( strlst.size() >= 1 ) // still >= 1? The last one has been removed above.
    {
        strBranchPath = strlst.join(m_strNodeSeparator);
    }

    if( o_pstrName != nullptr )
    {
        *o_pstrName = strName;
    }
    if( o_pstrBranchPath != nullptr )
    {
        *o_pstrBranchPath = strBranchPath;
    }

    if( !strEntryType.isEmpty() )
    {
        entrType = str2IdxTreeEntryType(strEntryType, EEnumEntryAliasStrSymbol);
    }
    return entrType;

} // splitPathStr

/*==============================================================================
public: // overridables (createBranch and createLeave must be overridden to create entries using a generic tree view)
==============================================================================*/

//------------------------------------------------------------------------------
/*! Generic method to create a branch entry with the given name.

    If it is necessary to generate generic branch nodes via drag & drop or cut & paste
    a user-defined IdxTree class must be implemented that overrides this method.

    @param i_strName [in] Name of the node to be created.

    @return Newly created branch node.
*/
CBranchIdxTreeEntry* CIdxTree::createBranch( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_strName;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createBranch",
        /* strMethodInArgs    */ strMthInArgs );

    CBranchIdxTreeEntry* pBranch = new CBranchIdxTreeEntry(i_strName);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(pBranch == nullptr ? "null" : pBranch->name());
    }

    return pBranch;

} // createBranch

//------------------------------------------------------------------------------
/*! Generic method to create a leave entry with the given name.

    If it is necessary to generate generic leave nodes via drag & drop or cut & paste
    a user-defined IdxTree class must be implemented that overrides this method.

    @param i_strName [in] Name of the node to be created.

    @return Newly created leave node.
*/
CLeaveIdxTreeEntry* CIdxTree::createLeave( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_strName;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createLeave",
        /* strMethodInArgs    */ strMthInArgs );

    CLeaveIdxTreeEntry* pLeave = new CLeaveIdxTreeEntry(i_strName);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(pLeave == nullptr ? "null" : pLeave->name());
    }

    return pLeave;

} // createLeave

//------------------------------------------------------------------------------
/*! Generic method which either invokes the generic method to create a brach or
    a leave node depending on the given node type.

    Usually it will not be necessary to overwrite the "createTreeEntry" method,
    but the "createBranchEntry" and "createLeaveEntry" methods that are called
    by this method.

    @param i_entryType [in] Type of the node to be created.
    @param i_strName [in] Name of the node to be created.

    @return Newly created node.
*/
CAbstractIdxTreeEntry* CIdxTree::createTreeEntry( EIdxTreeEntryType i_entryType, const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Type: " + idxTreeEntryType2Str(i_entryType);
        strMthInArgs += ", Name: " + i_strName;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createTreeEntry",
        /* strMethodInArgs    */ strMthInArgs );

    CAbstractIdxTreeEntry* pTreeEntry = nullptr;

    if( i_entryType == EIdxTreeEntryType::Root )
    {
    }
    else if( i_entryType == EIdxTreeEntryType::Branch )
    {
        pTreeEntry = createBranch(i_strName);
    }
    else if( i_entryType == EIdxTreeEntryType::Leave )
    {
        pTreeEntry = createLeave(i_strName);
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(pTreeEntry == nullptr ? "null" : pTreeEntry->name());
    }

    return pTreeEntry;

} // createTreeEntry

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the total number of nodes within the tree (without root node).

    Please note that the root entry will neither be added to the list nor to the
    map of tree entries and will not be included by counting the number of nodes.

    @return Number of node entries excluding the root node.
*/
int CIdxTree::treeEntriesVectorSize() const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_arpTreeEntries.size();
}

//------------------------------------------------------------------------------
/*! Returns the node entry with the corresponding index.

    This method is useful when iterating through all nodes within the tree.
    The index is assigned through runtime by reusing free entries within the
    vector. So the method may return a nullptr.

    @param i_idxObj [in] Node entry index.

    @return Pointer to node entry (may be nullptr).
*/
CAbstractIdxTreeEntry* CIdxTree::getEntry( int i_idxObj ) const
//------------------------------------------------------------------------------
{
    CAbstractIdxTreeEntry* pTreeEntry = nullptr;

    QMutexLocker mtxLocker(m_pMtx);

    if( i_idxObj >= 0 && i_idxObj < m_arpTreeEntries.size() )
    {
        // The entry may be nullptr.
        pTreeEntry = m_arpTreeEntries[i_idxObj];
    }

    return pTreeEntry;

} // getEntry

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the branch entry with the given path.

    @param i_strPath [in] Path to the branch. The path may already contain
                          the node entry type which will be ignored in this case.

    @return Pointer to branch entry (nullptr if not found).
*/
CBranchIdxTreeEntry* CIdxTree::findBranch( const QString& i_strPath ) const
//------------------------------------------------------------------------------
{
    QString strEntryType = idxTreeEntryType2Str(EIdxTreeEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = i_strPath;

    if( !strKeyInTree.startsWith(strEntryType + ":") )
    {
        strKeyInTree.insert(0, strEntryType + ":");
    }

    QMutexLocker mtxLocker(m_pMtx);

    return dynamic_cast<CBranchIdxTreeEntry*>(m_mappTreeEntries.value(strKeyInTree,nullptr));

} // findBranch

//------------------------------------------------------------------------------
/*! Returns the branch entry with the given path and name.

    @param i_strParentPath [in] Path to the parent branch. The path may already contain
                                the node entry type which will be ignored in this case.
    @param i_strBranchName [in] Name of the branch.

    @return Pointer to branch entry (nullptr if not found).
*/
CBranchIdxTreeEntry* CIdxTree::findBranch( const QString& i_strParentPath, const QString& i_strBranchName ) const
//------------------------------------------------------------------------------
{
    QString strEntryType = idxTreeEntryType2Str(EIdxTreeEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = i_strParentPath;

    if( i_strParentPath.isEmpty() )
    {
        strKeyInTree = strEntryType + ":" + i_strBranchName;
    }
    else if( i_strParentPath.startsWith(strKeyInTree + ":") )
    {
        strKeyInTree = i_strParentPath + m_strNodeSeparator + i_strBranchName;
    }
    else
    {
        strKeyInTree = strEntryType + ":" + i_strParentPath + m_strNodeSeparator + i_strBranchName;
    }

    QMutexLocker mtxLocker(m_pMtx);

    return dynamic_cast<CBranchIdxTreeEntry*>(m_mappTreeEntries.value(strKeyInTree,nullptr));

} // findBranch

//------------------------------------------------------------------------------
/*! Returns the leave entry with the given path.

    @param i_strPath [in] Path to the leave. The path may already contain
                          the node entry type which will be ignored in this case.

    @return Pointer to leave entry (nullptr if not found).
*/
CLeaveIdxTreeEntry* CIdxTree::findLeave( const QString& i_strPath ) const
//------------------------------------------------------------------------------
{
    QString strEntryType = idxTreeEntryType2Str(EIdxTreeEntryType::Leave, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = i_strPath;

    if( !strKeyInTree.startsWith(strEntryType + ":") )
    {
        strKeyInTree.insert(0, strEntryType + ":");
    }

    QMutexLocker mtxLocker(m_pMtx);

    return dynamic_cast<CLeaveIdxTreeEntry*>(m_mappTreeEntries.value(strKeyInTree,nullptr));

} // findLeave

//------------------------------------------------------------------------------
/*! Returns the leave entry with the given path and name.

    @param i_strParentPath [in] Path to the parent branch. The path may already contain
                                the node entry type which will be ignored in this case.
    @param i_strLeaveName [in] Name of the leave.

    @return Pointer to branch entry (nullptr if not found).
*/
CLeaveIdxTreeEntry* CIdxTree::findLeave( const QString& i_strParentPath, const QString& i_strLeaveName ) const
//------------------------------------------------------------------------------
{
    QString strEntryType = idxTreeEntryType2Str(EIdxTreeEntryType::Leave, EEnumEntryAliasStrSymbol);
    QString strKeyInTree = i_strParentPath;

    if( i_strParentPath.isEmpty() )
    {
        strKeyInTree = strEntryType + ":" + i_strLeaveName;
    }
    else if( i_strParentPath.startsWith(strKeyInTree + ":") )
    {
        strKeyInTree = i_strParentPath + m_strNodeSeparator + i_strLeaveName;
    }
    else
    {
        strKeyInTree = strEntryType + ":" + i_strParentPath + m_strNodeSeparator + i_strLeaveName;
    }

    QMutexLocker mtxLocker(m_pMtx);

    return dynamic_cast<CLeaveIdxTreeEntry*>(m_mappTreeEntries.value(strKeyInTree,nullptr));

} // findLeave

//------------------------------------------------------------------------------
/*! Returns the entry with the given key.

    @param i_strKeyInTree [in] Key of the entry. The key must contain the node
                               entry type (e.g. "L:ZS/Diagram/Pvt").

    @return Pointer to tree entry (nullptr if not found).
*/
CAbstractIdxTreeEntry* CIdxTree::findEntry( const QString& i_strKeyInTree ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_mappTreeEntries.value(i_strKeyInTree, nullptr);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Checks whether the given branch can be added as a child to the given target branch.

    The branch entry may only be added as a child to the target path if not
    already another branch with the same name exists below the target path.

    @param i_pBranch [in] Pointer to branch entry.
    @param i_strTargetPath [in] Path to the parent branch to which the branch should be added.
           The path may already start with the branch node type ("B:").

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be added.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a branch child with the same name below the target path.

    @see CBranchIdxTreeEntry::add
*/
SErrResultInfo CIdxTree::canAdd( CBranchIdxTreeEntry* i_pBranch, const QString& i_strTargetPath ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canAdd",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "add";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pLeave == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CAbstractIdxTreeEntry* pTreeEntry = i_pBranch;

    CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(i_strTargetPath);
    }
    else // if( pTargetBranch != nullptr )
    {
        int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

        if( idxInTargetBranch >= 0 )
        {
            QString strAddErrInfo;
            strAddErrInfo  = pTreeEntry->keyInParentBranch();
            strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( pTargetBranch != nullptr )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canAdd

//------------------------------------------------------------------------------
/*! Checks whether the given branch entry can be added as a child to the given target branch.

    The branch entry may only be added as a child to the target path if not
    already another branch with the same name exists below the target path.

    @param i_pBranch [in] Pointer to branch entry.
    @param i_pTargetBranch [in] Pointer to parent branch to which the branch should be added.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be added.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a branch child with the same name below the target path.

    @see CBranchIdxTreeEntry::add
*/
SErrResultInfo CIdxTree::canAdd( CBranchIdxTreeEntry* i_pBranch, CBranchIdxTreeEntry* i_pTargetBranch ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canAdd",
        /* strMethodInArgs    */ strMthInArgs );

    SErrResultInfo errResultInfo = canAdd(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch), i_pTargetBranch);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canAdd

//------------------------------------------------------------------------------
/*! Checks whether the given leave can be added as a child to the given target branch.

    The branch entry may only be added as a child to the target path if not
    already another leave with the same name exists below the target path.

    @param i_pLeave [in] Pointer to leave entry.
    @param i_strTargetPath [in] Path to the parent branch to which the leave should be added.
           The path may already start with the branch node type ("L:").

    @return Error result struct with additional information.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a leave child with the same name below the target path.

    @see CBranchIdxTreeEntry::add
*/
SErrResultInfo CIdxTree::canAdd( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strTargetPath ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canAdd",
        /* strMethodInArgs    */ strMthInArgs );

    SErrResultInfo errResultInfo = canAdd(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave), i_strTargetPath);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canAdd

//------------------------------------------------------------------------------
/*! Checks whether the given leave can be added as a child to the given target branch.

    The branch entry may only be added as a child to the target path if not
    already another leave with the same name exists below the target path.

    @param i_pLeave [in] Pointer to leave entry.
    @param i_pTargetBranch [in] Pointer to parent branch to which the leave should be added.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be added.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a leave child with the same name below the target path.

    @see CBranchIdxTreeEntry::add
*/
SErrResultInfo CIdxTree::canAdd( CLeaveIdxTreeEntry* i_pLeave, CBranchIdxTreeEntry* i_pTargetBranch ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canAdd",
        /* strMethodInArgs    */ strMthInArgs );

    SErrResultInfo errResultInfo = canAdd(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave), i_pTargetBranch);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canAdd

//------------------------------------------------------------------------------
/*! Checks whether the given tree entry can be added as a child to the given target branch.

    The entry may only be added as a child to the target path if not already another
    entry with the same node type and same name exists below the target path.

    @param i_pTreeEntry [in] Pointer to tree entry.
    @param i_strTargetPath [in] Path to the parent branch to which the entry should be added.
           The path may already start with the branch node type ("L:").

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be added.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same type and name below the target path.

    @see CBranchIdxTreeEntry::add
*/
SErrResultInfo CIdxTree::canAdd( CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canAdd",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "add";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QMutexLocker mtxLocker(m_pMtx);

    CBranchIdxTreeEntry* pTargetBranch = m_pRoot;

    if( !i_strTargetPath.isEmpty() )
    {
        pTargetBranch = findBranch(i_strTargetPath);

        if( pTargetBranch == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjNotInList);
            errResultInfo.setAddErrInfoDscr(i_strTargetPath);
        }
    } // if( !i_strTargetPath.isEmpty() )

    if( !errResultInfo.isErrorResult() )
    {
        errResultInfo = canAdd(i_pTreeEntry, pTargetBranch);
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canAdd

//------------------------------------------------------------------------------
/*! Checks whether the given tree entry can be added as a child to the given target branch.

    The entry may only be added as a child to the target path if not already another
    entry with the same node type and same name exists below the target path.

    @param i_pTreeEntry [in] Pointer to tree entry.
    @param i_pTargetBranch [in] Pointer to parent branch to which the entry should be added.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be added.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same type and name below the target path.

    @see CBranchIdxTreeEntry::add
*/
SErrResultInfo CIdxTree::canAdd( CAbstractIdxTreeEntry* i_pTreeEntry, CBranchIdxTreeEntry* i_pTargetBranch ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canAdd",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "add";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QMutexLocker mtxLocker(m_pMtx);

    if( i_pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTreeEntry == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTargetBranch == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    int idxInTargetBranch = i_pTargetBranch->indexOf(i_pTreeEntry);

    if( idxInTargetBranch >= 0 )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjAlreadyInList);
        errResultInfo.setAddErrInfoDscr(i_pTreeEntry->keyInParentBranch());
    }
    else // if( idxInTargetBranch < 0 )
    {
        QString strEntryType = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
        QString strTargetPath;
        QString strKeyInTree;

        strKeyInTree = strEntryType + ":";

        if( i_pTargetBranch != m_pRoot )
        {
            strTargetPath = i_pTargetBranch->path();
        }
        if( !strTargetPath.isEmpty() )
        {
            strKeyInTree += strTargetPath;

            if( !strKeyInTree.endsWith(m_strNodeSeparator) )
            {
                strKeyInTree += m_strNodeSeparator;
            }
        }
        strKeyInTree += i_pTreeEntry->name();

        if( m_mappTreeEntries.contains(strKeyInTree) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr(strKeyInTree);
        }
    } // if( idxInTargetBranch < 0 )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canAdd

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Adds the given branch entry as a child of the given target branch to the index tree.

    If there is a free index in the tree's list of tree entries this index will be used.
    If no free index is found the entry will be appended at the end of the list.

    The following members of the given child entry will be modified:

    - The reference to the tree will be set at the tree entry.
    - The unique key of the entry within the tree will be set at the entry.
    - The index of the entry in the trees list of entry will be set at the entry.

    @param i_pBranch [in] Pointer to tree entry to be added as a child.
    @param i_strTargetPath [in] Path of the target branch the entry should be added as a child.
           The path may already start with the branch node type ("B:").

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::add
*/
int CIdxTree::add( CBranchIdxTreeEntry* i_pBranch, const QString& i_strTargetPath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "add",
        /* strMethodInArgs    */ strMthInArgs );

    int idxInTree = add(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch), i_strTargetPath);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // add

//------------------------------------------------------------------------------
/*! Adds the given branch entry as a child of the given target branch to the index tree.

    If there is a free index in the tree's list of tree entries this index will be used.
    If no free index is found the entry will be appended at the end of the list.

    The following members of the given child entry will be modified:

    - The reference to the tree will be set at the tree entry.
    - The unique key of the entry within the tree will be set at the entry.
    - The index of the entry in the trees list of entry will be set at the entry.

    @param i_pBranch [in] Pointer to tree entry to be added as a child.
    @param i_pTargetBranch [in] Pointer to target branch the entry should be added as a child.

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::add
*/
int CIdxTree::add( CBranchIdxTreeEntry* i_pBranch, CBranchIdxTreeEntry* i_pTargetBranch )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "add",
        /* strMethodInArgs    */ strMthInArgs );

    int idxInTree = add(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch), i_pTargetBranch);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // add

//------------------------------------------------------------------------------
/*! Adds the given leave entry as a child of the given target branch to the index tree.

    If there is a free index in the list of tree entries this index will be used.
    If no free index is found the entry will be appended at the end of the list.

    The following members of the given child entry will be modified:

    - The reference to the tree will be set at the tree entry.
    - The unique key of the entry within the tree will be set at the entry.
    - The index of the entry in the trees list of entry will be set at the entry.

    @param i_pLeave [in] Pointer to tree entry to be added as a child.
    @param i_strTargetPath [in] Path of the target branch the entry should be added as a child.
           The path may already start with the branch node type ("B:").

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::add
*/
int CIdxTree::add( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strTargetPath )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "add",
        /* strMethodInArgs    */ strMthInArgs );

    int idxInTree = add(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave), i_strTargetPath);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // add

//------------------------------------------------------------------------------
/*! Adds the given leave entry as a child of the given target branch to the index tree.

    If there is a free index in the list of tree entries this index will be used.
    If no free index is found the entry will be appended at the end of the list.

    The following members of the given child entry will be modified:

    - The reference to the tree will be set at the tree entry.
    - The unique key of the entry within the tree will be set at the entry.
    - The index of the entry in the trees list of entry will be set at the entry.

    @param i_pLeave [in] Pointer to tree entry to be added as a child.
    @param i_pTargetBranch [in] Pointer to target branch the entry should be added as a child.

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::add
*/
int CIdxTree::add( CLeaveIdxTreeEntry* i_pLeave, CBranchIdxTreeEntry* i_pTargetBranch )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "add",
        /* strMethodInArgs    */ strMthInArgs );

    int idxInTree = add(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave), i_pTargetBranch);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // add

//------------------------------------------------------------------------------
/*! Adds the given tree entry as a child of the given target branch to the index tree.

    If there is a free index in the list of tree entries this index will be used.
    If no free index is found the entry will be appended at the end of the list.

    The following members of the given child entry will be modified:

    - The reference to the tree will be set at the tree entry.
    - The unique key of the entry within the tree will be set at the entry.
    - The index of the entry in the trees list of entry will be set at the entry.

    @param i_pTreeEntry [in] Pointer to tree entry to be added as a child.
    @param i_strTargetPath [in] Path of the target branch the entry should be added as a child.
           The path may already start with the branch node type ("B:").

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::add
*/
int CIdxTree::add( CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "add",
        /* strMethodInArgs    */ strMthInArgs );

    QMutexLocker mtxLocker(m_pMtx);

    CBranchIdxTreeEntry* pTargetBranch = m_pRoot;

    if( !i_strTargetPath.isEmpty() )
    {
        pTargetBranch = findBranch(i_strTargetPath);

        if( pTargetBranch == nullptr )
        {
            QStringList strlstBranches = i_strTargetPath.split(m_strNodeSeparator, QtSkipEmptyParts);

            // Please note that the name of the root entry is not included in the TargetPath of the tree entries.
            CBranchIdxTreeEntry* pTargetBranchPrev = m_pRoot;
            QString              strTargetPathPrev;

            for( QString strBranchName : strlstBranches )
            {
                pTargetBranch = findBranch(strTargetPathPrev, strBranchName);

                if( pTargetBranch == nullptr )
                {
                    pTargetBranch = createBranch(strBranchName);
                    add(pTargetBranch, pTargetBranchPrev);
                }
                pTargetBranchPrev = pTargetBranch;
                strTargetPathPrev = pTargetBranchPrev == m_pRoot ? "" : pTargetBranchPrev->path();
            }
        } // if( pTargetBranch == nullptr )
    } // if( !i_strTargetPath.isEmpty() )

    int idxInTree = add(i_pTreeEntry, pTargetBranch);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // add

//------------------------------------------------------------------------------
/*! Adds the given tree entry as a child of the given target branch to the index tree.

    If there is a free index in the list of tree entries this index will be used.
    If no free index is found the entry will be appended at the end of the list.

    The following members of the given child entry will be modified:

    - The reference to the tree will be set at the tree entry.
    - The unique key of the entry within the tree will be set at the entry.
    - The index of the entry in the trees list of entry will be set at the entry.

    @param i_pTreeEntry [in] Pointer to tree entry to be added as a child.
    @param i_pTargetBranch [in] Pointer to target branch the entry should be added as a child.

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::add
*/
int CIdxTree::add( CAbstractIdxTreeEntry* i_pTreeEntry, CBranchIdxTreeEntry* i_pTargetBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "add",
        /* strMethodInArgs    */ strMthInArgs );

    QMutexLocker mtxLocker(m_pMtx);

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }
    if( i_pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    int idxInTree = -1;

    int idxInTargetBranch = i_pTargetBranch->add(i_pTreeEntry);

    if( idxInTargetBranch >= 0 )
    {
        QString strEntryType = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
        QString strTargetPath;
        QString strKeyInTree;

        strKeyInTree = strEntryType + ":";

        if( i_pTargetBranch != m_pRoot )
        {
            strTargetPath = i_pTargetBranch->path();
        }
        if( !strTargetPath.isEmpty() )
        {
            strKeyInTree += strTargetPath;

            if( !strKeyInTree.endsWith(m_strNodeSeparator) )
            {
                strKeyInTree += m_strNodeSeparator;
            }
        }
        strKeyInTree += i_pTreeEntry->name();

        if( m_mappTreeEntries.contains(strKeyInTree) )
        {
            throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKeyInTree);
        }

        m_mappTreeEntries.insert(strKeyInTree, i_pTreeEntry);

        if( m_mapFreeIdxs.size() > 0 )
        {
            #if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
            idxInTree = m_mapFreeIdxs.firstKey();
            #else
            idxInTree = m_mapFreeIdxs.begin().key();
            #endif
            m_mapFreeIdxs.remove(idxInTree);
            m_arpTreeEntries[idxInTree] = i_pTreeEntry;
        }
        else
        {
            idxInTree = m_arpTreeEntries.size();
            m_arpTreeEntries.append(i_pTreeEntry);
        }

        i_pTreeEntry->setTree(this);
        i_pTreeEntry->setKeyInTree(strKeyInTree);
        i_pTreeEntry->setIndexInTree(idxInTree);

        emit_treeEntryAdded(this, i_pTreeEntry);

    } // if( idxInTargetBranch >= 0 )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // add

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Checks whether the given branch can be inserted as a child to the given
    target branch at the given index.

    The branch entry may only be added as a child to the target path if not
    already another branch with the same name exists below the target path.

    If the optional parameter i_idxInTree is greator or equal to zero it will
    also be checked whether this index is free.

    @param i_pBranch [in] Pointer to branch entry.
    @param i_strTargetPath [in] Path of the parent branch to which the branch should be added.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be inserted.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a branch child with the same name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list or at
              the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canInsert(
    CBranchIdxTreeEntry* i_pBranch,
    const QString&       i_strTargetPath,
    int                  i_idxInTargetBranch,
    int                  i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canInsert",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "insert";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pLeave == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QMutexLocker mtxLocker(m_pMtx);

    CAbstractIdxTreeEntry* pTreeEntry = i_pBranch;

    CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(i_strTargetPath);
    }
    else // if( pTargetBranch != nullptr )
    {
        if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
        {
            QString strAddErrInfo;
            strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
            strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultIdxOutOfRange);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
        else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
        {
            int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

            if( idxInTargetBranch >= 0 )
            {
                QString strAddErrInfo;
                strAddErrInfo  = pTreeEntry->keyInParentBranch();
                strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultObjAlreadyInList);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
        } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )

        if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )
        {
            if( getEntry(i_idxInTree) != nullptr )
            {
                QString strAddErrInfo = "IdxInTree (=" + QString::number(i_idxInTree) + ") is already used";
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultIdxOutOfRange);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
        } // if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )
    } // if( pTargetBranch != nullptr )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canInsert

//------------------------------------------------------------------------------
/*! Checks whether the given branch can be inserted as a child to the given
    target branch at the given index.

    The branch entry may only be added as a child to the target path if not
    already another branch with the same name exists below the target path.

    If the optional parameter i_idxInTree is greator or equal to zero it will
    also be checked whether this index is free.

    @param i_pBranch [in] Pointer to branch entry.
    @param i_pTargetBranch [in] Pointer to the parent branch to which the branch should be added.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be inserted.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a branch child with the same name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list or at
              the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canInsert(
    CBranchIdxTreeEntry* i_pBranch,
    CBranchIdxTreeEntry* i_pTargetBranch,
    int                  i_idxInTargetBranch,
    int                  i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs += ", Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canInsert",
        /* strMethodInArgs    */ strMthInArgs );

    SErrResultInfo errResultInfo = canInsert(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch), i_pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canInsert

//------------------------------------------------------------------------------
/*! Checks whether the given leave can be inserted as a child to the given
    target branch at the given index.

    The branch entry may only be added as a child to the target path if not
    already another branch with the same name exists below the target path.

    If the optional parameter i_idxInTree is greator or equal to zero it will
    also be checked whether this index is free.

    @param i_pLeave [in] Pointer to leave entry.
    @param i_strTargetPath [in] Path of the parent branch to which the branch should be added.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be inserted.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a branch child with the same name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list or at
              the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canInsert(
    CLeaveIdxTreeEntry* i_pLeave,
    const QString&      i_strTargetPath,
    int                 i_idxInTargetBranch,
    int                 i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canInsert",
        /* strMethodInArgs    */ strMthInArgs );

    SErrResultInfo errResultInfo = canInsert(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave), i_strTargetPath, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canInsert

//------------------------------------------------------------------------------
/*! Checks whether the given leave can be inserted as a child to the given
    target branch at the given index.

    The branch entry may only be added as a child to the target path if not
    already another branch with the same name exists below the target path.

    If the optional parameter i_idxInTree is greator or equal to zero it will
    also be checked whether this index is free.

    @param i_pLeave [in] Pointer to leave entry.
    @param i_pTargetBranch [in] Pointer to parent branch to which the branch should be added.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be inserted.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a branch child with the same name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list or at
              the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canInsert(
    CLeaveIdxTreeEntry*  i_pLeave,
    CBranchIdxTreeEntry* i_pTargetBranch,
    int                  i_idxInTargetBranch,
    int                  i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canInsert",
        /* strMethodInArgs    */ strMthInArgs );

    SErrResultInfo errResultInfo = canInsert(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave), i_pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canInsert

//------------------------------------------------------------------------------
/*! Checks whether the given entry can be inserted as a child to the given
    target branch at the given index.

    The entry may only be added as a child to the target path if not
    already another branch with the same name exists below the target path.

    If the optional parameter i_idxInTree is greator or equal to zero it will
    also be checked whether this index is free.

    @param i_pTreeEntry [in] Pointer to tree entry.
    @param i_strTargetPath [in] Path of the parent branch to which the branch should be added.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be inserted.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a branch child with the same name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list or at
              the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canInsert(
    CAbstractIdxTreeEntry* i_pTreeEntry,
    const QString&         i_strTargetPath,
    int                    i_idxInTargetBranch,
    int                    i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canInsert",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "insert";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QMutexLocker mtxLocker(m_pMtx);

    if( i_pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTreeEntry == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CBranchIdxTreeEntry* pTargetBranch = m_pRoot;

    if( !i_strTargetPath.isEmpty() )
    {
        pTargetBranch = findBranch(i_strTargetPath);

        if( pTargetBranch == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjNotInList);
            errResultInfo.setAddErrInfoDscr(i_strTargetPath);
        }
    } // if( !i_strTargetPath.isEmpty() )

    if( !errResultInfo.isErrorResult() )
    {
        errResultInfo = canInsert(i_pTreeEntry, pTargetBranch, i_idxInTargetBranch, i_idxInTree);
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canInsert

//------------------------------------------------------------------------------
/*! Checks whether the given entry can be inserted as a child to the given
    target branch at the given index.

    The entry may only be added as a child to the target path if not
    already another branch with the same name exists below the target path.

    If the optional parameter i_idxInTree is greator or equal to zero it will
    also be checked whether this index is free.

    @param i_pTreeEntry [in] Pointer to tree entry.
    @param i_pTargetBranch [in] Pointer to parent branch to which the branch should be added.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be inserted.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a branch child with the same name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list or at
              the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canInsert(
    CAbstractIdxTreeEntry* i_pTreeEntry,
    CBranchIdxTreeEntry*   i_pTargetBranch,
    int                    i_idxInTargetBranch,
    int                    i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canInsert",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "insert";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTreeEntry == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTargetBranch == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QMutexLocker mtxLocker(m_pMtx);

    if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > i_pTargetBranch->count() )
    {
        QString strAddErrInfo;
        strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
        strAddErrInfo += "is out of range [0.." + QString::number(i_pTargetBranch->count()) + "]";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultIdxOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }
    else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
    {
        int idxInTargetBranch = i_pTargetBranch->indexOf(i_pTreeEntry);

        if( idxInTargetBranch >= 0 )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr(i_pTreeEntry->keyInParentBranch());
        }
        else // if( idxInTargetBranch < 0 )
        {
            QString strEntryType = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
            QString strTargetPath;
            QString strKeyInTree;

            strKeyInTree = strEntryType + ":";

            if( i_pTargetBranch != m_pRoot )
            {
                strTargetPath = i_pTargetBranch->path();
            }
            if( !strTargetPath.isEmpty() )
            {
                strKeyInTree += strTargetPath;

                if( !strKeyInTree.endsWith(m_strNodeSeparator) )
                {
                    strKeyInTree += m_strNodeSeparator;
                }
            }
            strKeyInTree += i_pTreeEntry->name();

            if( m_mappTreeEntries.contains(strKeyInTree) )
            {
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultObjAlreadyInList);
                errResultInfo.setAddErrInfoDscr(strKeyInTree);
            }
        } // if( idxInTargetBranch < 0 )
    } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )

    if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )
    {
        if( getEntry(i_idxInTree) != nullptr )
        {
            QString strAddErrInfo;
            strAddErrInfo  = "IdxInTree (=" + QString::number(i_idxInTree) + ") is already used";
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultIdxOutOfRange);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canInsert

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Inserts the given branch entry to the index tree by inserting the entry as a
    child of the target branch at the given index.

    If the optional parameter i_idxInTree is greator or equal to zero this index will be used.

    Otherwise (default) if there is a free index in the tree's list of tree entries this index
    will be used. If no free index is found the entry will be appended at the end of the list.

    If inserting in the list of childs of a branch the index of all childs after
    the insertion index will be incremented by one.

    The following members of the given child entry will be modified:

    - The reference to the tree will be set at the tree entry.
    - The unique key of the entry within the tree will be set at the entry.
    - The index of the entry in the trees list of entry will be set at the entry.

    @param i_pBranch [in] Pointer to tree entry to be added as a child.
    @param i_strTargetPath [in] Path of the target branch the entry should be added as a child.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::insert
*/
int CIdxTree::insert(
    CBranchIdxTreeEntry* i_pBranch,
    const QString&       i_strTargetPath,
    int                  i_idxInTargetBranch,
    int                  i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insert",
        /* strMethodInArgs    */ strMthInArgs );

    int idxInTree = insert(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch), i_strTargetPath, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // insert

//------------------------------------------------------------------------------
/*! Inserts the given branch entry to the index tree by inserting the entry as a
    child of the target branch at the given index.

    If the optional parameter i_idxInTree is greator or equal to zero this index will be used.

    Otherwise (default) if there is a free index in the tree's list of tree entries this index
    will be used. If no free index is found the entry will be appended at the end of the list.

    The following members of the given child entry will be modified:

    - The reference to the tree will be set at the tree entry.
    - The unique key of the entry within the tree will be set at the entry.
    - The index of the entry in the trees list of entry will be set at the entry.

    @param i_pBranch [in] Pointer to tree entry to be added as a child.
    @param i_pTargetBranch [in] Pointer to the target branch the entry should be added as a child.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::insert
*/
int CIdxTree::insert(
    CBranchIdxTreeEntry* i_pBranch,
    CBranchIdxTreeEntry* i_pTargetBranch,
    int                  i_idxInTargetBranch,
    int                  i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insert",
        /* strMethodInArgs    */ strMthInArgs );

    int idxInTree = insert(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch), i_pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // insert

//------------------------------------------------------------------------------
/*! Inserts the given leave entry to the index tree by inserting the entry as a
    child of the target branch at the given index.

    If the optional parameter i_idxInTree is greator or equal to zero this index will be used.

    Otherwise (default) if there is a free index in the tree's list of tree entries this index
    will be used. If no free index is found the entry will be appended at the end of the list.

    The following members of the given child entry will be modified:

    - The reference to the tree will be set at the tree entry.
    - The unique key of the entry within the tree will be set at the entry.
    - The index of the entry in the trees list of entry will be set at the entry.

    @param i_pLeave [in] Pointer to tree entry to be added as a child.
    @param i_strTargetPath [in] Path of the target branch the entry should be added as a child.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::insert
*/
int CIdxTree::insert(
    CLeaveIdxTreeEntry* i_pLeave,
    const QString&      i_strTargetPath,
    int                 i_idxInTargetBranch,
    int                 i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insert",
        /* strMethodInArgs    */ strMthInArgs );

    int idxInTree = insert(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave), i_strTargetPath, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // insert

//------------------------------------------------------------------------------
/*! Inserts the given leave entry to the index tree by inserting the entry as a
    child of the target branch at the given index.

    If the optional parameter i_idxInTree is greator or equal to zero this index will be used.

    Otherwise (default) if there is a free index in the tree's list of tree entries this index
    will be used. If no free index is found the entry will be appended at the end of the list.

    The following members of the given child entry will be modified:

    - The reference to the tree will be set at the tree entry.
    - The unique key of the entry within the tree will be set at the entry.
    - The index of the entry in the trees list of entry will be set at the entry.

    @param i_pLeave [in] Pointer to tree entry to be added as a child.
    @param i_pTargetBranch [in] Pointer to the target branch the entry should be added as a child.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::insert
*/
int CIdxTree::insert(
    CLeaveIdxTreeEntry*  i_pLeave,
    CBranchIdxTreeEntry* i_pTargetBranch,
    int                  i_idxInTargetBranch,
    int                  i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insert",
        /* strMethodInArgs    */ strMthInArgs );

    int idxInTree = insert(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave), i_pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // insert

//------------------------------------------------------------------------------
/*! Inserts the given entry to the index tree by inserting the entry as a
    child of the target branch at the given index.

    If the optional parameter i_idxInTree is greator or equal to zero this index will be used.

    Otherwise (default) if there is a free index in the tree's list of tree entries this index
    will be used. If no free index is found the entry will be appended at the end of the list.

    The following members of the given child entry will be modified:

    - The reference to the tree will be set at the tree entry.
    - The unique key of the entry within the tree will be set at the entry.
    - The index of the entry in the trees list of entry will be set at the entry.

    @param i_pTreeEntry [in] Pointer to tree entry to be added as a child.
    @param i_strTargetPath [in] Path of the target branch the entry should be added as a child.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::insert
*/
int CIdxTree::insert(
    CAbstractIdxTreeEntry* i_pTreeEntry,
    const QString&         i_strTargetPath,
    int                    i_idxInTargetBranch,
    int                    i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insert",
        /* strMethodInArgs    */ strMthInArgs );

    QMutexLocker mtxLocker(m_pMtx);

    CBranchIdxTreeEntry* pTargetBranch = m_pRoot;

    if( !i_strTargetPath.isEmpty() )
    {
        pTargetBranch = findBranch(i_strTargetPath);

        if( pTargetBranch == nullptr )
        {
            QStringList strlstBranches = i_strTargetPath.split(m_strNodeSeparator, QtSkipEmptyParts);

            // Please note that the name of the root entry is not included in the TargetPath of the tree entries.
            CBranchIdxTreeEntry* pTargetBranchPrev = m_pRoot;
            QString              strTargetPathPrev;

            for( QString strBranchName : strlstBranches )
            {
                pTargetBranch = findBranch(strTargetPathPrev, strBranchName);

                if( pTargetBranch == nullptr )
                {
                    pTargetBranch = createBranch(strBranchName);
                    add(pTargetBranch, pTargetBranchPrev);
                }
                pTargetBranchPrev = pTargetBranch;
                strTargetPathPrev = pTargetBranchPrev == m_pRoot ? "" : pTargetBranchPrev->path();
            }
        } // if( pTargetBranch == nullptr )
    } // if( !i_strTargetPath.isEmpty() )

    int idxInTree = insert(i_pTreeEntry, pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // insert

//------------------------------------------------------------------------------
/*! Inserts the given entry to the index tree by inserting the entry as a
    child of the target branch at the given index.

    If the optional parameter i_idxInTree is greator or equal to zero this index will be used.

    Otherwise (default) if there is a free index in the tree's list of tree entries this index
    will be used. If no free index is found the entry will be appended at the end of the list.

    The following members of the given child entry will be modified:

    - The reference to the tree will be set at the tree entry.
    - The unique key of the entry within the tree will be set at the entry.
    - The index of the entry in the trees list of entry will be set at the entry.

    @param i_pTreeEntry [in] Pointer to tree entry to be added as a child.
    @param i_pTargetBranch [in] Pointer to the target branch the entry should be added as a child.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = EResultObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.
          - with Result = EResultIdxOutOfRange if an entry already exists at the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
int CIdxTree::insert(
    CAbstractIdxTreeEntry* i_pTreeEntry,
    CBranchIdxTreeEntry*   i_pTargetBranch,
    int                    i_idxInTargetBranch,
    int                    i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insert",
        /* strMethodInArgs    */ strMthInArgs );

    QMutexLocker mtxLocker(m_pMtx);

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }
    if( i_pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    int idxInTree = -1;

    int idxInTargetBranch = -1;

    if( i_idxInTargetBranch >= 0 )
    {
        idxInTargetBranch = i_pTargetBranch->insert(i_idxInTargetBranch, i_pTreeEntry);
    }
    else
    {
        idxInTargetBranch = i_pTargetBranch->add(i_pTreeEntry);
    }

    if( idxInTargetBranch >= 0 )
    {
        QString strEntryType = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
        QString strTargetPath;
        QString strKeyInTree;

        strKeyInTree = strEntryType + ":";

        if( i_pTargetBranch != m_pRoot )
        {
            strTargetPath = i_pTargetBranch->path();
        }
        if( !strTargetPath.isEmpty() )
        {
            strKeyInTree += strTargetPath;

            if( !strKeyInTree.endsWith(m_strNodeSeparator) )
            {
                strKeyInTree += m_strNodeSeparator;
            }
        }
        strKeyInTree += i_pTreeEntry->name();

        if( m_mappTreeEntries.contains(strKeyInTree) )
        {
            throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKeyInTree);
        }

        if( i_idxInTree >= 0 )
        {
            idxInTree = i_idxInTree;

            if( idxInTree < m_arpTreeEntries.size() )
            {
                if( m_arpTreeEntries[idxInTree] != nullptr )
                {
                    QString strAddErrInfo = "IdxInTree (=" + QString::number(idxInTree) + ") is already used";
                    SErrResultInfo errResultInfo = ErrResultInfoCritical("insert", EResultIdxOutOfRange, strAddErrInfo);
                    throw CException(__FILE__, __LINE__, errResultInfo);
                }
                if( m_mapFreeIdxs.contains(idxInTree) )
                {
                    m_mapFreeIdxs.remove(idxInTree);
                }
            }
            else // if( i_idxInTree >= m_arpTreeEntries.size() )
            {
                int iSizePrev = m_arpTreeEntries.size();
                int iSizeNew  = i_idxInTree + 1;

                m_arpTreeEntries.resize(iSizeNew);
                memset(&m_arpTreeEntries.data()[iSizePrev], 0x00, (iSizeNew-iSizePrev) * sizeof(CAbstractIdxTreeEntry*));

                for( int idxTmp = iSizePrev; idxTmp < idxInTree; ++idxTmp )
                {
                    m_mapFreeIdxs.insert(idxTmp, idxTmp);
                }
            }

            m_mappTreeEntries.insert(strKeyInTree, i_pTreeEntry);
            m_arpTreeEntries[idxInTree] = i_pTreeEntry;

        } // if( i_idxInTree >= 0 )

        else // if( i_idxInTree < 0 )
        {
            m_mappTreeEntries.insert(strKeyInTree, i_pTreeEntry);

            if( m_mapFreeIdxs.size() > 0 )
            {
                #if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
                idxInTree = m_mapFreeIdxs.firstKey();
                #else
                idxInTree = m_mapFreeIdxs.begin().key();
                #endif
                m_mapFreeIdxs.remove(idxInTree);
                m_arpTreeEntries[idxInTree] = i_pTreeEntry;
            }
            else
            {
                idxInTree = m_arpTreeEntries.size();
                m_arpTreeEntries.append(i_pTreeEntry);
            }
        }

        i_pTreeEntry->setTree(this);
        i_pTreeEntry->setKeyInTree(strKeyInTree);
        i_pTreeEntry->setIndexInTree(idxInTree);

        emit_treeEntryAdded(this, i_pTreeEntry);

    } // if( idxInTargetBranch >= 0 )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // insert

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Checks whether the given branch can be removed from the tree.

    The entry may only be removed if it belongs to the tree and if it is not
    the root entry.

    @param i_pBranch [in] Pointer to branch entry.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be removed.
            - EResultInvalidMethodCall (Severity: Error) .. The root entry cannot be removed.
            - EResultObjNotInList (Severtiy: Error) .. The entry does not belong to the tree.
*/
SErrResultInfo CIdxTree::canRemove( CBranchIdxTreeEntry* i_pBranch ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canRemove",
        /* strMethodInArgs    */ strMthInArgs );

    QMutexLocker mtxLocker(m_pMtx);

    SErrResultInfo errResultInfo = canRemove(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch));

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canRemove

//------------------------------------------------------------------------------
/*! Checks whether the given leave can be removed from the tree.

    The entry may only be removed if it belongs to the tree.

    @param i_pLeave [in] Pointer to leave entry.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be removed.
            - EResultObjNotInList (Severtiy: Error) .. The entry does not belong to the tree.
*/
SErrResultInfo CIdxTree::canRemove( CLeaveIdxTreeEntry* i_pLeave ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canRemove",
        /* strMethodInArgs    */ strMthInArgs );

    QMutexLocker mtxLocker(m_pMtx);

    SErrResultInfo errResultInfo = canRemove(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave));

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canRemove

//------------------------------------------------------------------------------
/*! Checks whether the given entry can be removed from the tree.

    The entry may only be removed if it belongs to the tree and if it is not
    the root entry.

    @param i_pTreeEntry [in] Pointer to entry.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be removed.
            - EResultInvalidMethodCall (Severity: Error) .. The root entry cannot be removed.
            - EResultObjNotInList (Severtiy: Error) .. The entry does not belong to the tree.
*/
SErrResultInfo CIdxTree::canRemove( CAbstractIdxTreeEntry* i_pTreeEntry ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canRemove",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    QString strMth = "remove";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QMutexLocker mtxLocker(m_pMtx);

    QString strKeyInTree = i_pTreeEntry->keyInTree();

    if( i_pTreeEntry == m_pRoot )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidMethodCall);
        errResultInfo.setAddErrInfoDscr("Root entry cannot be removed");
    }
    else if( !m_mappTreeEntries.contains(strKeyInTree) )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(strKeyInTree);
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canRemove

//------------------------------------------------------------------------------
/*! Checks whether the given entry can be removed from the tree.

    The entry may only be removed if it belongs to the tree and if it is not
    the root entry.

    @param i_strKeyInTree [in] Unique key of the entry to be removed.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be removed.
            - EResultInvalidMethodCall (Severity: Error) .. The root entry cannot be removed.
            - EResultObjNotInList (Severtiy: Error) .. The entry does not belong to the tree.
*/
SErrResultInfo CIdxTree::canRemove( const QString& i_strKeyInTree ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_strKeyInTree;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canRemove",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "remove";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    QMutexLocker mtxLocker(m_pMtx);

    CAbstractIdxTreeEntry* pTreeEntry = findEntry(i_strKeyInTree);

    if( pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(i_strKeyInTree);
    }

    if( !errResultInfo.isErrorResult() )
    {
        errResultInfo = canRemove(pTreeEntry);
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canRemove

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Removes the given branch from the tree.

    This method is either called when an entry is going about to be destroyed
    or if an entry is moved within the tree.

    The entry will be removed from the map of tree entries by removing the
    unique key of the entry from the map.

    The entry will also be removed from the vector of tree entries by setting the element
    of the vector to nullptr. If there are no more valid entries in the vector
    after the removed entry the vector will be downsized so that the last element
    in the vector becomes a valid entry again. After removing an entry there might
    be empty entries in the vector. Those empty entries may be reused later wenn
    adding or inserting new entries.

    The following members of the given child entry will be modified:

    - The reference to the tree will be invalidated (set to nullptr).
    - The unique key of the entry will be invalidated (set to an empty string).
    - The tree index of the entry will be invalidated (set to -1).

    @param i_pBranch [in] Pointer to branch to be removed.

    @note The entry will not be destroyed.

    @note Throws a critical exception with Result
          - EResultObjNotInList if the entry does not belong to the tree.
          - EResultInvalidMethodCall (Severity: Error) .. The root entry cannot be removed.

    @see CBranchIdxTreeEntry::remove( CAbstractIdxTreeEntry* i_pTreeEntry )
*/
void CIdxTree::remove( CBranchIdxTreeEntry* i_pBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "remove",
        /* strMethodInArgs    */ strMthInArgs );

    QMutexLocker mtxLocker(m_pMtx);

    return remove(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch));

} // remove

//------------------------------------------------------------------------------
/*! Removes the given leave from the tree.

    This method is either called when an entry is going about to be destroyed
    or if an entry is moved within the tree.

    The entry will be removed from the map of tree entries by removing the
    unique key of the entry from the map.

    The entry will also be removed from the vector of tree entries by setting the element
    of the vector to nullptr. If there are no more valid entries in the vector
    after the removed entry the vector will be downsized so that the last element
    in the vector becomes a valid entry again. After removing an entry there might
    be empty entries in the vector. Those empty entries may be reused later wenn
    adding or inserting new entries.

    The following members of the given child entry will be modified:

    - The reference to the tree will be invalidated (set to nullptr).
    - The unique key of the entry will be invalidated (set to an empty string).
    - The tree index of the entry will be invalidated (set to -1).

    @param i_pLeave [in] Pointer to leave to be removed.

    @note The entry will not be destroyed.

    @note Throws a critical exception with Result
          - EResultObjNotInList if the entry does not belong to the tree.

    @see CBranchIdxTreeEntry::remove( CAbstractIdxTreeEntry* i_pTreeEntry )
*/
void CIdxTree::remove( CLeaveIdxTreeEntry* i_pLeave )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "remove",
        /* strMethodInArgs    */ strMthInArgs );

    QMutexLocker mtxLocker(m_pMtx);

    return remove(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave));

} // remove

//------------------------------------------------------------------------------
/*! Removes the given entry from the tree.

    This method is either called when an entry is going about to be destroyed
    or if an entry is moved within the tree.

    The entry will be removed from the map of tree entries by removing the
    unique key of the entry from the map.

    The entry will also be removed from the vector of tree entries by setting the element
    of the vector to nullptr. If there are no more valid entries in the vector
    after the removed entry the vector will be downsized so that the last element
    in the vector becomes a valid entry again. After removing an entry there might
    be empty entries in the vector. Those empty entries may be reused later wenn
    adding or inserting new entries.

    The following members of the given child entry will be modified:

    - The reference to the tree will be invalidated (set to nullptr).
    - The unique key of the entry will be invalidated (set to an empty string).
    - The tree index of the entry will be invalidated (set to -1).

    @param i_pTreeEntry [in] Pointer to tree entry to be removed.

    @note The entry will not be destroyed.

    @note Throws a critical exception with Result
          - EResultObjNotInList if the entry does not belong to the tree.
          - EResultInvalidMethodCall (Severity: Error) .. The root entry cannot be removed.

    @see CBranchIdxTreeEntry::remove( CAbstractIdxTreeEntry* i_pTreeEntry )
*/
void CIdxTree::remove( CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "remove",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    QMutexLocker mtxLocker(m_pMtx);

    emit_treeEntryAboutToBeRemoved(this, i_pTreeEntry);

    QString strKeyInTree = i_pTreeEntry->keyInTree();
    int     idxInTree    = i_pTreeEntry->indexInTree();

    if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Root || i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
    {
        // dynamic_cast may not work if the remove method has been called by
        // the dtor of the branch entry. But if the dynamic_cast returns nullptr
        // we can be sure that all children of the branch have already been
        // destroyed and removed from the tree.
        CBranchIdxTreeEntry* pBranch = dynamic_cast<CBranchIdxTreeEntry*>(i_pTreeEntry);

        if( pBranch != nullptr && pBranch->count() > 0 )
        {
            CAbstractIdxTreeEntry* pTreeEntry;
            int                    idxEntry;

            for( idxEntry = pBranch->count()-1; idxEntry >= 0; --idxEntry )
            {
                pTreeEntry = pBranch->at(idxEntry);
                remove(pTreeEntry);
            }
        }
    } // if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Root || i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch )

    if( i_pTreeEntry == m_pRoot )
    {
        m_pRoot = nullptr;
        i_pTreeEntry->setTree(nullptr);
    }
    else // if( i_pTreeEntry != m_pRoot )
    {
        if( !m_mappTreeEntries.contains(strKeyInTree) )
        {
            throw CException(__FILE__, __LINE__, EResultObjNotInList, strKeyInTree);
        }

        m_mappTreeEntries.remove(strKeyInTree);

        if( idxInTree < 0 || idxInTree >= m_arpTreeEntries.size() )
        {
            throw CException(__FILE__, __LINE__, EResultIdxOutOfRange, QString::number(idxInTree));
        }

        // Removed last element in vector ...
        if( idxInTree == (m_arpTreeEntries.size()-1) )
        {
            m_arpTreeEntries.remove(idxInTree);

            // Update map with free list indices if the next-to-last element became the last element.
            while( m_mapFreeIdxs.contains(m_arpTreeEntries.size()-1) )
            {
                m_mapFreeIdxs.remove(m_arpTreeEntries.size()-1);
                m_arpTreeEntries.remove(m_arpTreeEntries.size()-1);
            }
        }
        // Removed element somewhere before end of vector ...
        else
        {
            // Don't resize the list. The enties must keep their Ids.
            // But update list with free indices.
            m_arpTreeEntries[idxInTree] = nullptr;
            m_mapFreeIdxs.insert(idxInTree, idxInTree);
        }

        i_pTreeEntry->setTree(nullptr);
        i_pTreeEntry->setKeyInTree("");
        i_pTreeEntry->setIndexInTree(-1);

        CBranchIdxTreeEntry* pTargetBranch = i_pTreeEntry->parentBranch();

        pTargetBranch->remove(i_pTreeEntry);

    } // if( i_pTreeEntry != m_pRoot )

    emit_treeEntryRemoved(this, i_pTreeEntry, strKeyInTree, idxInTree);

} // remove

//------------------------------------------------------------------------------
/*! Removes the given entry from the tree.

    This method is either called when an entry is going about to be destroyed
    or if an entry is moved within the tree.

    The entry will be removed from the map of tree entries by removing the
    unique key of the entry from the map.

    The entry will also be removed from the vector of tree entries by setting the element
    of the vector to nullptr. If there are no more valid entries in the vector
    after the removed entry the vector will be downsized so that the last element
    in the vector becomes a valid entry again. After removing an entry there might
    be empty entries in the vector. Those empty entries may be reused later wenn
    adding or inserting new entries.

    The following members of the given child entry will be modified:

    - The reference to the tree will be invalidated (set to nullptr).
    - The unique key of the entry will be invalidated (set to an empty string).
    - The tree index of the entry will be invalidated (set to -1).

    @param i_strKeyInTree [in] Unique key of the entry to be removed.

    @note The entry will not be destroyed.

    @note Throws a critical exception with Result
          - EResultObjNotInList if the entry does not belong to the tree.
          - EResultInvalidMethodCall (Severity: Error) .. The root entry cannot be removed.

    @see CBranchIdxTreeEntry::remove( CAbstractIdxTreeEntry* i_pTreeEntry )
*/
void CIdxTree::remove( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = i_strKeyInTree;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "remove",
        /* strMethodInArgs    */ strMthInArgs );

    QMutexLocker mtxLocker(m_pMtx);

    CAbstractIdxTreeEntry* pTreeEntry = findEntry(i_strKeyInTree);

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strKeyInTree);
    }

    return remove(pTreeEntry);

} // remove

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Checks whether the given source entry can be moved as a child to the given
    target branch with the given index.

    The entry may only be moved as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    @param i_strSourcePath [in] Path to the source entry to be moved.
    @param i_strTargetPath [in] Path of the parent branch to which the entry should be moved.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be moved.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canMove(
    const QString& i_strSourcePath,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "SourcePath: " + i_strSourcePath;
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canMove",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "move";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    CAbstractIdxTreeEntry* pTreeEntry = findEntry(i_strSourcePath);

    if( pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(i_strSourcePath);
    }
    else // if( pTreeEntry != nullptr )
    {
        CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

        if( pTargetBranch == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjNotInList);
            errResultInfo.setAddErrInfoDscr(i_strTargetPath);
        }
        else // if( pTargetBranch != nullptr )
        {
            if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
                strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultIdxOutOfRange);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
            else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
            {
                int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

                if( idxInTargetBranch >= 0 )
                {
                    QString strAddErrInfo;
                    strAddErrInfo  = pTreeEntry->keyInParentBranch();
                    strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultObjAlreadyInList);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                }
            } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
        } // if( pTargetBranch != nullptr )
    } // if( pTreeEntry != nullptr )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canMove

//------------------------------------------------------------------------------
/*! Checks whether the given leave can be moved as a child to the given
    target branch with the given index.

    The entry may only be moved as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    @param i_pLeave [in] Pointer to leave entry to be moved.
    @param i_strTargetPath [in] Path of the parent branch to which the entry should be moved.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be moved.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canMove(
    CLeaveIdxTreeEntry* i_pLeave,
    const QString&      i_strTargetPath,
    int                 i_idxInTargetBranch ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->keyInTree());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canMove",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "move";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pLeave == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pLeave == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CAbstractIdxTreeEntry* pTreeEntry = i_pLeave;

    CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(i_strTargetPath);
    }
    else // if( pTargetBranch != nullptr )
    {
        if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
        {
            QString strAddErrInfo;
            strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
            strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultIdxOutOfRange);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
        else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
        {
            int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

            if( idxInTargetBranch >= 0 )
            {
                QString strAddErrInfo;
                strAddErrInfo  = pTreeEntry->keyInParentBranch();
                strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultObjAlreadyInList);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
        } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
    } // if( pTargetBranch != nullptr )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canMove

//------------------------------------------------------------------------------
/*! Checks whether the given leave can be moved as a child to the given
    target branch with the given index.

    The entry may only be moved as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    @param i_pLeave [in] Pointer to leave entry to be moved.
    @param i_pTargetBranch [in] Pointer to parent branch entry to which the entry should be moved.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be moved.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canMove(
    CLeaveIdxTreeEntry*  i_pLeave,
    CBranchIdxTreeEntry* i_pTargetBranch,
    int                  i_idxInTargetBranch ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canMove",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "move";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pLeave == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pLeave == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTargetBranch == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CAbstractIdxTreeEntry* pTreeEntry = i_pLeave;

    CBranchIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
    {
        QString strAddErrInfo;
        strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
        strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultIdxOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }
    else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
    {
        int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

        if( idxInTargetBranch >= 0 )
        {
            QString strAddErrInfo;
            strAddErrInfo  = pTreeEntry->keyInParentBranch();
            strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canMove

//------------------------------------------------------------------------------
/*! Checks whether the given branch can be moved as a child to the given
    target branch with the given index.

    The entry may only be moved as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    @param i_pBranch [in] Pointer to branch entry to be moved.
    @param i_strTargetPath [in] Path of the parent branch to which the entry should be moved.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be moved.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canMove(
    CBranchIdxTreeEntry* i_pBranch,
    const QString&       i_strTargetPath,
    int                  i_idxInTargetBranch ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canMove",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "move";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pBranch == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CAbstractIdxTreeEntry* pTreeEntry = i_pBranch;

    CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(i_strTargetPath);
    }
    else // if( pTargetBranch != nullptr )
    {
        if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
        {
            QString strAddErrInfo;
            strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
            strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultIdxOutOfRange);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
        else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
        {
            int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

            if( idxInTargetBranch >= 0 )
            {
                QString strAddErrInfo;
                strAddErrInfo  = pTreeEntry->keyInParentBranch();
                strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultObjAlreadyInList);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
        } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
    } // if( pTargetBranch != nullptr )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canMove

//------------------------------------------------------------------------------
/*! Checks whether the given branch can be moved as a child to the given
    target branch with the given index.

    The entry may only be moved as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    @param i_pBranch [in] Pointer to branch entry to be moved.
    @param i_pTargetBranch [in] Pointer to parent branch to which the entry should be moved.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be moved.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canMove(
    CBranchIdxTreeEntry* i_pBranch,
    CBranchIdxTreeEntry* i_pTargetBranch,
    int                  i_idxInTargetBranch ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canMove",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "move";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pBranch == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTargetBranch == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CAbstractIdxTreeEntry* pTreeEntry = i_pBranch;

    CBranchIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
    {
        QString strAddErrInfo;
        strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
        strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultIdxOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }
    else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
    {
        int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

        if( idxInTargetBranch >= 0 )
        {
            QString strAddErrInfo;
            strAddErrInfo  = pTreeEntry->keyInParentBranch();
            strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canMove

//------------------------------------------------------------------------------
/*! Checks whether the given entry can be moved as a child to the given
    target branch with the given index.

    The entry may only be moved as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    @param i_pTreeEntry [in] Pointer to entry to be moved.
    @param i_pTargetBranch [in] Pointer to parent branch to which the entry should be moved.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be moved.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canMove(
    CAbstractIdxTreeEntry* i_pTreeEntry,
    CBranchIdxTreeEntry*   i_pTargetBranch,
    int                    i_idxInTargetBranch ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canMove",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "move";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTreeEntry == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTargetBranch == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QMutexLocker mtxLocker(m_pMtx);

    CAbstractIdxTreeEntry* pTreeEntry = i_pTreeEntry;

    CBranchIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
    {
        QString strAddErrInfo;
        strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
        strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultIdxOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }
    else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
    {
        int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

        if( idxInTargetBranch >= 0 )
        {
            QString strAddErrInfo;
            strAddErrInfo  = pTreeEntry->keyInParentBranch();
            strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canMove

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Moves the given source entry to the given target branch by inserting the entry as a
    child of the target branch at the given index.

    If the entry is a branch with childs also a childs will be moved to the target branch.

    The following members of the given child entry will be modified:

    - The unique key of the entry and all its childs will be updated.

    @param i_strSourcePath [in] Path of the tree entry to be moved.
    @param i_strTargetPath [in] Path of the target branch the entry should be moved to.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @note The index of the entry in the trees vector of entries will not be changed.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::remove
    @see CBranchIdxTreeEntry::insert
    @see CBranchIdxTreeEntry::add
*/
void CIdxTree::move(
    const QString& i_strSourcePath,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "SourcePath: " + i_strSourcePath;
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "move",
        /* strMethodInArgs    */ strMthInArgs );

    CAbstractIdxTreeEntry* pTreeEntry = findEntry(i_strSourcePath);

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strSourcePath);
    }

    CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strTargetPath);
    }

    return move(pTreeEntry, pTargetBranch, i_idxInTargetBranch);

} // move

//------------------------------------------------------------------------------
/*! Moves the given leave to the given target branch by inserting the entry as a
    child of the target branch at the given index.

    The following members of the given child entry will be modified:

    - The unique key of the entry and all its childs will be updated.

    @param i_pLeave [in] Pointer to tree entry to be moved.
    @param i_strTargetPath [in] Path of the target branch the entry should be moved to.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @note The index of the entry in the trees vector of entries will not be changed.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::remove
    @see CBranchIdxTreeEntry::insert
    @see CBranchIdxTreeEntry::add
*/
void CIdxTree::move(
    CLeaveIdxTreeEntry* i_pLeave,
    const QString&      i_strTargetPath,
    int                 i_idxInTargetBranch )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->keyInTree());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "move",
        /* strMethodInArgs    */ strMthInArgs );

    CAbstractIdxTreeEntry* pTreeEntry = dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave);

    CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strTargetPath);
    }

    return move(pTreeEntry, pTargetBranch, i_idxInTargetBranch);

} // move

//------------------------------------------------------------------------------
/*! Moves the given leave to the given target branch by inserting the entry as a
    child of the target branch at the given index.

    The following members of the given child entry will be modified:

    - The unique key of the entry and all its childs will be updated.

    @param i_pLeave [in] Pointer to tree entry to be moved.
    @param i_pTargetBranch [in] Pointer to target branch the entry should be moved to.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @note The index of the entry in the trees vector of entries will not be changed.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::remove
    @see CBranchIdxTreeEntry::insert
    @see CBranchIdxTreeEntry::add
*/
void CIdxTree::move(
    CLeaveIdxTreeEntry*  i_pLeave,
    CBranchIdxTreeEntry* i_pTargetBranch,
    int                  i_idxInTargetBranch )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "move",
        /* strMethodInArgs    */ strMthInArgs );

    return move(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave), i_pTargetBranch, i_idxInTargetBranch);

} // move

//------------------------------------------------------------------------------
/*! Moves the given branch to the given target branch by inserting the entry as a
    child of the target branch at the given index.

    All childs of the branch will be moved to the target branch.

    The following members of the given child entry will be modified:

    - The unique key of the entry and all its childs will be updated.

    @param i_pBranch [in] Pointer to tree entry to be moved.
    @param i_strTargetPath [in] Path of the target branch the entry should be moved to.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @note The index of the entry in the trees vector of entries will not be changed.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::remove
    @see CBranchIdxTreeEntry::insert
    @see CBranchIdxTreeEntry::add
*/
void CIdxTree::move(
    CBranchIdxTreeEntry* i_pBranch,
    const QString&       i_strTargetPath,
    int                  i_idxInTargetBranch )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "move",
        /* strMethodInArgs    */ strMthInArgs );

    CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strTargetPath);
    }

    return move(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch), pTargetBranch, i_idxInTargetBranch);

} // move

//------------------------------------------------------------------------------
/*! Moves the given branch to the given target branch by inserting the entry as a
    child of the target branch at the given index.

    All childs of the branch will be moved to the target branch.

    The following members of the given child entry will be modified:

    - The unique key of the entry and all its childs will be updated.

    @param i_pBranch [in] Pointer to tree entry to be moved.
    @param i_pTargetBranch [in] Pointer to target branch the entry should be moved to.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @note The index of the entry in the trees vector of entries will not be changed.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::remove
    @see CBranchIdxTreeEntry::insert
    @see CBranchIdxTreeEntry::add
*/
void CIdxTree::move(
    CBranchIdxTreeEntry* i_pBranch,
    CBranchIdxTreeEntry* i_pTargetBranch,
    int                  i_idxInTargetBranch )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "move",
        /* strMethodInArgs    */ strMthInArgs );

    return move(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch), i_pTargetBranch, i_idxInTargetBranch);

} // move

//------------------------------------------------------------------------------
/*! Moves the given tree entry to the given target branch by inserting the entry as a
    child of the target branch at the given index.

    If the entry is a branch with childs also a childs will be moved to the target branch.

    The following members of the given child entry will be modified:

    - The unique key of the entry and all its childs will be updated.

    @param i_pTreeEntry [in] Pointer to tree entry to be moved.
    @param i_pTargetBranch [in] Pointer to target branch the entry should be moved to.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @note The index of the entry in the trees vector of entries will not be changed.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::remove
    @see CBranchIdxTreeEntry::insert
    @see CBranchIdxTreeEntry::add
*/
void CIdxTree::move(
    CAbstractIdxTreeEntry* i_pTreeEntry,
    CBranchIdxTreeEntry*   i_pTargetBranch,
    int                    i_idxInTargetBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "move",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }
    if( i_pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    QMutexLocker mtxLocker(m_pMtx);

    QString strKeyInTreePrev = i_pTreeEntry->keyInTree();

    emit_treeEntryAboutToBeMoved(this, i_pTreeEntry, i_pTargetBranch);

    CBranchIdxTreeEntry* pParentBranch = i_pTreeEntry->parentBranch();

    pParentBranch->remove(i_pTreeEntry);

    int idxInParentBranch = -1;

    if( i_idxInTargetBranch >= 0 )
    {
        idxInParentBranch = i_pTargetBranch->insert(i_idxInTargetBranch, i_pTreeEntry);
    }
    else
    {
        idxInParentBranch = i_pTargetBranch->add(i_pTreeEntry);
    }

    if( idxInParentBranch >= 0 )
    {
        updateKeyInTree(i_pTreeEntry);
    }

    emit_treeEntryMoved(this, i_pTreeEntry, strKeyInTreePrev, i_pTargetBranch);

} // move

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Checks whether the given source entry can be copied as a child to the given
    target branch with the given index.

    The entry may only be copied as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    If the optional parameter i_idxInTree is greator or equal to zero it will
    also be checked whether this index is free.

    @param i_strSourcePath [in] Path to the source entry to be copied.
    @param i_strTargetPath [in] Path of the parent branch to which the entry should be copied.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be inserted.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list or at
              the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canCopy(
    const QString& i_strSourcePath,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch,
    int            i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "SourcePath: " + i_strSourcePath;
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canCopy",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "copy";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    CAbstractIdxTreeEntry* pTreeEntry = findEntry(i_strSourcePath);

    if( pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(i_strSourcePath);
    }
    else // if( pTreeEntry != nullptr )
    {
        CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

        if( pTargetBranch == nullptr )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjNotInList);
            errResultInfo.setAddErrInfoDscr(i_strTargetPath);
        }
        else // if( pTargetBranch != nullptr )
        {
            if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
                strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultIdxOutOfRange);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
            else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
            {
                int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

                if( idxInTargetBranch >= 0 )
                {
                    QString strAddErrInfo;
                    strAddErrInfo  = pTreeEntry->keyInParentBranch();
                    strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultObjAlreadyInList);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                }
            } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )

            if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )
            {
                if( getEntry(i_idxInTree) != nullptr )
                {
                    QString strAddErrInfo;
                    strAddErrInfo  = "IdxInTree (=" + QString::number(i_idxInTree) + ") is already used";
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultIdxOutOfRange);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                }
            } // if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )
        } // if( pTargetBranch != nullptr )
    } // if( pTreeEntry != nullptr )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canCopy

//------------------------------------------------------------------------------
/*! Checks whether the given leave can be copied as a child to the given
    target branch with the given index.

    The entry may only be copied as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    If the optional parameter i_idxInTree is greator or equal to zero it will
    also be checked whether this index is free.

    @param i_pLeave [in] Pointer to the source entry to be copied.
    @param i_strTargetPath [in] Path of the parent branch to which the entry should be copied.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be inserted.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list or at
              the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canCopy(
    CLeaveIdxTreeEntry* i_pLeave,
    const QString&      i_strTargetPath,
    int                 i_idxInTargetBranch,
    int                 i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->keyInTree());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canCopy",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "copy";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pLeave == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pLeave == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CAbstractIdxTreeEntry* pTreeEntry = i_pLeave;

    CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(i_strTargetPath);
    }
    else // if( pTargetBranch != nullptr )
    {
        if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
        {
            QString strAddErrInfo;
            strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
            strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultIdxOutOfRange);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
        else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
        {
            int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

            if( idxInTargetBranch >= 0 )
            {
                QString strAddErrInfo;
                strAddErrInfo  = pTreeEntry->keyInParentBranch();
                strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultObjAlreadyInList);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
        } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )

        if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )
        {
            if( getEntry(i_idxInTree) != nullptr )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "IdxInTree (=" + QString::number(i_idxInTree) + ") is already used";
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultIdxOutOfRange);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
        } // if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )
    } // if( pTargetBranch != nullptr )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canCopy

//------------------------------------------------------------------------------
/*! Checks whether the given leave can be copied as a child to the given
    target branch with the given index.

    The entry may only be copied as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    If the optional parameter i_idxInTree is greator or equal to zero it will
    also be checked whether this index is free.

    @param i_pLeave [in] Pointer to the source entry to be copied.
    @param i_pTargetBranch [in] Pointer to parent branch to which the entry should be copied.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be inserted.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list or at
              the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canCopy(
    CLeaveIdxTreeEntry*  i_pLeave,
    CBranchIdxTreeEntry* i_pTargetBranch,
    int                  i_idxInTargetBranch,
    int                  i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canCopy",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "copy";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pLeave == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pLeave == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTargetBranch == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CAbstractIdxTreeEntry* pTreeEntry = i_pLeave;

    CBranchIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
    {
        QString strAddErrInfo;
        strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
        strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultIdxOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }
    else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
    {
        int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

        if( idxInTargetBranch >= 0 )
        {
            QString strAddErrInfo;
            strAddErrInfo  = pTreeEntry->keyInParentBranch();
            strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )

    if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )
    {
        if( getEntry(i_idxInTree) != nullptr )
        {
            QString strAddErrInfo;
            strAddErrInfo  = "IdxInTree (=" + QString::number(i_idxInTree) + ") is already used";
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultIdxOutOfRange);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canCopy

//------------------------------------------------------------------------------
/*! Checks whether the given branch can be copied as a child to the given
    target branch with the given index.

    The entry may only be copied as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    If the optional parameter i_idxInTree is greator or equal to zero it will
    also be checked whether this index is free.

    @param i_pBranch [in] Pointer to the source entry to be copied.
    @param i_strTargetPath [in] Path of the parent branch to which the entry should be copied.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be inserted.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list or at
              the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canCopy(
    CBranchIdxTreeEntry* i_pBranch,
    const QString&       i_strTargetPath,
    int                  i_idxInTargetBranch,
    int                  i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canCopy",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "copy";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pBranch == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CAbstractIdxTreeEntry* pTreeEntry = i_pBranch;

    CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(i_strTargetPath);
    }
    else // if( pTargetBranch != nullptr )
    {
        if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
        {
            QString strAddErrInfo;
            strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
            strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultIdxOutOfRange);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
        else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
        {
            int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

            if( idxInTargetBranch >= 0 )
            {
                QString strAddErrInfo;
                strAddErrInfo  = pTreeEntry->keyInParentBranch();
                strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultObjAlreadyInList);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
        } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )

        if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )
        {
            if( getEntry(i_idxInTree) != nullptr )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "IdxInTree (=" + QString::number(i_idxInTree) + ") is already used";
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultIdxOutOfRange);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
        } // if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )
    } // if( pTargetBranch != nullptr )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canCopy

//------------------------------------------------------------------------------
/*! Checks whether the given branch can be copied as a child to the given
    target branch with the given index.

    The entry may only be copied as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    If the optional parameter i_idxInTree is greator or equal to zero it will
    also be checked whether this index is free.

    @param i_pBranch [in] Pointer to the source entry to be copied.
    @param i_pTargetBranch [in] Pointer to parent branch to which the entry should be copied.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be inserted.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list or at
              the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canCopy(
    CBranchIdxTreeEntry* i_pBranch,
    CBranchIdxTreeEntry* i_pTargetBranch,
    int                  i_idxInTargetBranch,
    int                  i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canCopy",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "copy";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pBranch == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTargetBranch == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CAbstractIdxTreeEntry* pTreeEntry = i_pBranch;

    CBranchIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
    {
        QString strAddErrInfo;
        strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
        strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultIdxOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }
    else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
    {
        int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

        if( idxInTargetBranch >= 0 )
        {
            QString strAddErrInfo;
            strAddErrInfo  = pTreeEntry->keyInParentBranch();
            strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )

    if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )
    {
        if( getEntry(i_idxInTree) != nullptr )
        {
            QString strAddErrInfo;
            strAddErrInfo  = "IdxInTree (=" + QString::number(i_idxInTree) + ") is already used";
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultIdxOutOfRange);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canCopy

//------------------------------------------------------------------------------
/*! Checks whether the given source entry can be copied as a child to the given
    target branch with the given index.

    The entry may only be copied as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    If the optional parameter i_idxInTree is greator or equal to zero it will
    also be checked whether this index is free.

    @param i_pTreeEntry [in] Pointer to the source entry to be copied.
    @param i_pTargetBranch [in] Pointer to parent branch to which the entry should be copied.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be inserted.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list or at
              the given tree index.

    @see CBranchIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canCopy(
    CAbstractIdxTreeEntry* i_pTreeEntry,
    CBranchIdxTreeEntry*   i_pTargetBranch,
    int                    i_idxInTargetBranch,
    int                    i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canCopy",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "copy";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    if( i_pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTreeEntry == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_pTargetBranch == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTargetBranch == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QMutexLocker mtxLocker(m_pMtx);

    CAbstractIdxTreeEntry* pTreeEntry = i_pTreeEntry;

    CBranchIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( i_idxInTargetBranch >= 0 && i_idxInTargetBranch > pTargetBranch->count() )
    {
        QString strAddErrInfo;
        strAddErrInfo  = "IdxInTargetBranch (=" + QString::number(i_idxInTargetBranch) + ") ";
        strAddErrInfo += "is out of range [0.." + QString::number(pTargetBranch->count()) + "]";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultIdxOutOfRange);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }
    else // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
    {
        int idxInTargetBranch = pTargetBranch->indexOf(pTreeEntry->keyInParentBranch());

        if( idxInTargetBranch >= 0 )
        {
            QString strAddErrInfo;
            strAddErrInfo  = pTreeEntry->keyInParentBranch();
            strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )

    if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )
    {
        if( getEntry(i_idxInTree) != nullptr )
        {
            QString strAddErrInfo;
            strAddErrInfo  = "IdxInTree (=" + QString::number(i_idxInTree) + ") is already used";
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultIdxOutOfRange);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    } // if( !errResultInfo.isErrorResult() && i_idxInTree >= 0 )

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canCopy

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Copies the given source entry to the given target branch by inserting a copy
    of the entry as a child of the target branch at the given index.

    If the entry is a branch with childs also a childs will be copied to the target branch.

    If the optional parameter i_idxInTree is greator or equal to zero this index will be used.

    Otherwise (default) if there is a free index in the tree's list of tree entries this index
    will be used. If no free index is found the entry will be appended at the end of the list.

    @param i_strSourcePath [in] Path of the tree entry to be copied.
    @param i_strTargetPath [in] Path of the target branch the entry should be copied to.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the copy of the entry should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entries copy in the trees vector of entries.

    @see CBranchIdxTreeEntry::insert
    @see CBranchIdxTreeEntry::add
*/
int CIdxTree::copy(
    const QString& i_strSourcePath,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "SourcePath: " + i_strSourcePath;
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "copy",
        /* strMethodInArgs    */ strMthInArgs );

    CAbstractIdxTreeEntry* pTreeEntry = findEntry(i_strSourcePath);

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strSourcePath);
    }

    CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strTargetPath);
    }

    int idxInTree = copy(pTreeEntry, pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // copy

//------------------------------------------------------------------------------
/*! Copies the given leave to the given target branch by inserting a copy
    of the entry as a child of the target branch at the given index.

    If the optional parameter i_idxInTree is greator or equal to zero this index will be used.

    Otherwise (default) if there is a free index in the tree's list of tree entries this index
    will be used. If no free index is found the entry will be appended at the end of the list.

    @param i_pLeave [in] Pointer to tree entry to be copied.
    @param i_strTargetPath [in] Path of the target branch the entry should be copied to.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the copy of the entry should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entries copy in the trees vector of entries.

    @see CBranchIdxTreeEntry::insert
    @see CBranchIdxTreeEntry::add
*/
int CIdxTree::copy(
    CLeaveIdxTreeEntry* i_pLeave,
    const QString&      i_strTargetPath,
    int                 i_idxInTargetBranch,
    int                 i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->keyInTree());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "copy",
        /* strMethodInArgs    */ strMthInArgs );

    CAbstractIdxTreeEntry* pTreeEntry = dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave);

    CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strTargetPath);
    }

    int idxInTree = copy(pTreeEntry, pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // copy

//------------------------------------------------------------------------------
/*! Copies the given leave to the given target branch by inserting a copy
    of the entry as a child of the target branch at the given index.

    If the optional parameter i_idxInTree is greator or equal to zero this index will be used.

    Otherwise (default) if there is a free index in the tree's list of tree entries this index
    will be used. If no free index is found the entry will be appended at the end of the list.

    @param i_pLeave [in] Pointer to tree entry to be copied.
    @param i_pTargetBranch [in] Pointer to target branch the entry should be copied to.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the copy of the entry should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entries copy in the trees vector of entries.

    @see CBranchIdxTreeEntry::insert
    @see CBranchIdxTreeEntry::add
*/
int CIdxTree::copy(
    CLeaveIdxTreeEntry*  i_pLeave,
    CBranchIdxTreeEntry* i_pTargetBranch,
    int                  i_idxInTargetBranch,
    int                  i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "copy",
        /* strMethodInArgs    */ strMthInArgs );

    int idxInTree = copy(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave), i_pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // copy

//------------------------------------------------------------------------------
/*! Copies the given branch to the given target branch by inserting a copy
    of the entry as a child of the target branch at the given index.

    Also all childs of the branch will be copied to the target branch.

    If the optional parameter i_idxInTree is greator or equal to zero this index will be used.

    Otherwise (default) if there is a free index in the tree's list of tree entries this index
    will be used. If no free index is found the entry will be appended at the end of the list.

    @param i_pBranch [in] Pointer to tree entry to be copied.
    @param i_strTargetPath [in] Path of the target branch the entry should be copied to.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the copy of the entry should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entries copy in the trees vector of entries.

    @see CBranchIdxTreeEntry::insert
    @see CBranchIdxTreeEntry::add
*/
int CIdxTree::copy(
    CBranchIdxTreeEntry* i_pBranch,
    const QString&       i_strTargetPath,
    int                  i_idxInTargetBranch,
    int                  i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "copy",
        /* strMethodInArgs    */ strMthInArgs );

    CBranchIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strTargetPath);
    }

    int idxInTree = copy(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch), pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // copy

//------------------------------------------------------------------------------
/*! Copies the given branch to the given target branch by inserting a copy
    of the entry as a child of the target branch at the given index.

    Also all childs of the branch will be copied to the target branch.

    If the optional parameter i_idxInTree is greator or equal to zero this index will be used.

    Otherwise (default) if there is a free index in the tree's list of tree entries this index
    will be used. If no free index is found the entry will be appended at the end of the list.

    @param i_pBranch [in] Pointer to tree entry to be copied.
    @param i_pTargetBranch [in] Pointer to target branch the entry should be copied to.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the copy of the entry should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entries copy in the trees vector of entries.

    @see CBranchIdxTreeEntry::insert
    @see CBranchIdxTreeEntry::add
*/
int CIdxTree::copy(
    CBranchIdxTreeEntry* i_pBranch,
    CBranchIdxTreeEntry* i_pTargetBranch,
    int                  i_idxInTargetBranch,
    int                  i_idxInTree )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "copy",
        /* strMethodInArgs    */ strMthInArgs );

    int idxInTree = copy(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch), i_pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // copy

//------------------------------------------------------------------------------
/*! Copies the given tree entry to the given target branch by inserting a copy
    of the entry as a child of the target branch at the given index.

    If the entry is a branch with childs also a childs will be copied to the target branch.

    If the optional parameter i_idxInTree is greator or equal to zero this index will be used.

    Otherwise (default) if there is a free index in the tree's list of tree entries this index
    will be used. If no free index is found the entry will be appended at the end of the list.

    All copies will get a unique name by appending "Copy" to the source name (e.g. "Customers Copy").
    If there is already a child ending with "Copy" the name will be appended with a
    consecutive number to become unique in the target branch (e.g. "Customers Copy 3").

    @param i_pTreeEntry [in] Pointer to tree entry to be copied.
    @param i_pTargetBranch [in] Pointer to target branch the entry should be copied to.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the copy of the entry should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entries copy in the trees vector of entries.

    @see CBranchIdxTreeEntry::insert
    @see CBranchIdxTreeEntry::add
*/
int CIdxTree::copy(
    CAbstractIdxTreeEntry* i_pTreeEntry,
    CBranchIdxTreeEntry*   i_pTargetBranch,
    int                    i_idxInTargetBranch,
    int                    i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "copy",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }
    if( i_pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    QMutexLocker mtxLocker(m_pMtx);

    QString strName = i_pTreeEntry->name();

    if( i_pTargetBranch->indexOf(i_pTreeEntry->entryType(), strName) >= 0 )
    {
        QString strUniqueName = strName + " Copy";
        int     iCopies = 1;

        while( true )
        {
            if( i_pTargetBranch->indexOf(i_pTreeEntry->entryType(), strUniqueName) >= 0 )
            {
                strUniqueName = strName + " Copy " + QString::number(++iCopies);
            }
            else
            {
                break;
            }
        }
        strName = strUniqueName;

    } // if( i_pTargetBranch->indexOf(i_pTreeEntry->entryType(), strName) >= 0 )

    CAbstractIdxTreeEntry* pTreeEntryTrg = createTreeEntry(i_pTreeEntry->entryType(), strName);

    int idxInTree = insert(pTreeEntryTrg, i_pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
    {
        CBranchIdxTreeEntry*   pBranch = dynamic_cast<CBranchIdxTreeEntry*>(i_pTreeEntry);
        CBranchIdxTreeEntry*   pTargetBranch = dynamic_cast<CBranchIdxTreeEntry*>(pTreeEntryTrg);
        CAbstractIdxTreeEntry* pTreeEntry;
        int                    idxEntry;

        for( idxEntry = 0; idxEntry < pBranch->count(); ++idxEntry )
        {
            pTreeEntry = pBranch->at(idxEntry);

            copy(pTreeEntry, pTargetBranch);
        }
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // copy

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Checks whether the given entry can be renamed to the given new name.

    The entry may only be renamed to the desired name if not already
    another entry with the same name and type exists below the target path.

    @param i_strSourcePath [in] Path to entry to be renamed.
    @param i_strNameNew [in] Desired new name of the entry.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be renamed.
            - EResultObjNotInList (Severtiy: Error) .. There is no entry with
              the given source path in the tree.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.

    @see CLeaveIdxTreeEntry::rename
    @see CBranchIdxTreeEntry::rename
*/
SErrResultInfo CIdxTree::canRename( const QString& i_strSourcePath, const QString& i_strNameNew ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "SourcePath: " + i_strSourcePath;
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canRename",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "rename";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    CAbstractIdxTreeEntry* pTreeEntry = findEntry(i_strSourcePath);

    if( pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(i_strSourcePath + " does not belong to the index tree.");
    }
    else
    {
        errResultInfo = canRename(pTreeEntry, i_strNameNew);
    }

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_iTrcDetailLevel >= ETraceDetailLevelVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_iTrcDetailLevel >= ETraceDetailLevelRuntimeInfo ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }
    return errResultInfo;

} // canRename

//------------------------------------------------------------------------------
/*! Checks whether the given leave can be renamed to the given new name.

    The entry may only be renamed to the desired name if not already
    another entry with the same name and type exists below the target path.

    @param i_pLeave [in] Pointer to leave to be renamed.
    @param i_strNameNew [in] Desired new name of the entry.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be renamed.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.

    @see CLeaveIdxTreeEntry::rename
*/
SErrResultInfo CIdxTree::canRename( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strNameNew ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->keyInTree());
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canRename",
        /* strMethodInArgs    */ strMthInArgs );

    return canRename(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave), i_strNameNew);

} // canRename

//------------------------------------------------------------------------------
/*! Checks whether the given branch can be renamed to the given new name.

    The entry may only be renamed to the desired name if not already
    another entry with the same name and type exists below the target path.

    @param i_pBranch [in] Pointer to branch to be renamed.
    @param i_strNameNew [in] Desired new name of the entry.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be renamed.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.

    @see CBranchIdxTreeEntry::rename
*/
SErrResultInfo CIdxTree::canRename( CBranchIdxTreeEntry* i_pBranch, const QString& i_strNameNew ) const
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canRename",
        /* strMethodInArgs    */ strMthInArgs );

    return canRename(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch), i_strNameNew);

} // canRename

//------------------------------------------------------------------------------
/*! Checks whether the given entry can be renamed to the given new name.

    The entry may only be renamed to the desired name if not already
    another entry with the same name and type exists below the target path.

    @param i_pTreeEntry [in] Pointer to entry to be renamed.
    @param i_strNameNew [in] Desired new name of the entry.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be renamed.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.

    @see CLeaveIdxTreeEntry::rename
    @see CBranchIdxTreeEntry::rename
*/
SErrResultInfo CIdxTree::canRename( CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strNameNew ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canRename",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    QMutexLocker mtxLocker(m_pMtx);

    QString strMth = "rename";

    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), strMth);

    CBranchIdxTreeEntry* pParentBranch = i_pTreeEntry->parentBranch();

    if( pParentBranch != nullptr )
    {
        int idxInParentBranch = pParentBranch->indexOf(i_pTreeEntry->entryType(), i_strNameNew);

        if( idxInParentBranch >= 0 )
        {
            QString strAddErrInfo;
            strAddErrInfo  = "A " + i_pTreeEntry->entryType2Str();
            strAddErrInfo += " named " + i_strNameNew;
            strAddErrInfo += " already belongs to " + pParentBranch->keyInTree();
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    }

    return errResultInfo;

} // canRename

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Renames the given source entry to the given new name.

    The entry may only be renamed to the desired name if not already
    another entry with the same name and type exists below the target path.

    The following members of the given entry together with all its childs will be modified:

    - The unique key of the entry and all its childs will be updated.

    @param i_strSourcePath [in] Path of the tree entry to be renamed.
    @param i_strNameNew [in] New name of entry.

    @note The index of the entry in the trees vector of entries will not be changed.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CLeaveIdxTreeEntry::rename
    @see CBranchIdxTreeEntry::rename
*/
void CIdxTree::rename( const QString& i_strSourcePath, const QString& i_strNameNew )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "SourcePath: " + i_strSourcePath;
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "rename",
        /* strMethodInArgs    */ strMthInArgs );

    CAbstractIdxTreeEntry* pTreeEntry = findEntry(i_strSourcePath);

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strSourcePath);
    }

    return rename(pTreeEntry, i_strNameNew);

} // rename

//------------------------------------------------------------------------------
/*! Renames the given leave to the given new name.

    The entry may only be renamed to the desired name if not already
    another entry with the same name and type exists below the target path.

    The following members of the given entry will be modified:

    - The unique key of the entry will be updated.

    @param i_pLeave [in] Pointer to leave to be renamed.
    @param i_strNameNew [in] New name of entry.

    @note The index of the entry in the trees vector of entries will not be changed.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CLeaveIdxTreeEntry::rename
*/
void CIdxTree::rename( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strNameNew )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Leave: " + QString(i_pLeave == nullptr ? "nullptr" : i_pLeave->keyInTree());
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "rename",
        /* strMethodInArgs    */ strMthInArgs );

    return rename(dynamic_cast<CAbstractIdxTreeEntry*>(i_pLeave), i_strNameNew);

} // rename

//------------------------------------------------------------------------------
/*! Renames the given branch to the given new name.

    The entry may only be renamed to the desired name if not already
    another entry with the same name and type exists below the target path.

    The following members of the given entry together with all its childs will be modified:

    - The unique key of the entry and all its childs will be updated.

    @param i_pBranch [in] Poiner to tree entry to be renamed.
    @param i_strNameNew [in] New name of entry.

    @note The index of the entry in the trees vector of entries will not be changed.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CBranchIdxTreeEntry::rename
*/
void CIdxTree::rename( CBranchIdxTreeEntry* i_pBranch, const QString& i_strNameNew )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "rename",
        /* strMethodInArgs    */ strMthInArgs );

    return rename(dynamic_cast<CAbstractIdxTreeEntry*>(i_pBranch), i_strNameNew);

} // rename

//------------------------------------------------------------------------------
/*! Renames the given tree entry to the given new name.

    The entry may only be renamed to the desired name if not already
    another entry with the same name and type exists below the target path.

    The following members of the given entry together with all its childs will be modified:

    - The unique key of the entry and all its childs will be updated.

    @param i_pTreeEntry [in] Poiner to tree entry to be renamed.
    @param i_strNameNew [in] New name of entry.

    @note The index of the entry in the trees vector of entries will not be changed.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CLeaveIdxTreeEntry::rename
    @see CBranchIdxTreeEntry::rename
*/
void CIdxTree::rename( CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strNameNew )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "rename",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    QMutexLocker mtxLocker(m_pMtx);

    QString strKeyInTreePrev = i_pTreeEntry->keyInTree();
    QString strNamePrev = i_pTreeEntry->name();

    emit_treeEntryAboutToBeRenamed(this, i_pTreeEntry, i_strNameNew);

    CBranchIdxTreeEntry* pParentBranch = i_pTreeEntry->parentBranch();

    pParentBranch->rename(i_pTreeEntry, i_strNameNew);

    updateKeyInTree(i_pTreeEntry);

    emit_treeEntryRenamed(this, i_pTreeEntry, strKeyInTreePrev, strNamePrev);

} // rename

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Auxiliary method to update the key of the given tree entry in tree's the map of entries.

    If the given entry is a branch the keys of all childs will also be updated.

    @param i_pTreeEntry [in] Pointer to entry whose key has to be updated.
*/
void CIdxTree::updateKeyInTree( CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "updateKeyInTree",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    QString strKeyInTreePrev = i_pTreeEntry->keyInTree();

    m_mappTreeEntries.remove(strKeyInTreePrev);

    QString strEntryType = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
    QString strParentPath;

    QString strKeyInTree = strEntryType + ":";

    CBranchIdxTreeEntry* pParentBranch = i_pTreeEntry->parentBranch();

    if( pParentBranch != m_pRoot )
    {
        strParentPath = pParentBranch->path();
    }
    if( !strParentPath.isEmpty() )
    {
        strKeyInTree += strParentPath;

        if( !strKeyInTree.endsWith(m_strNodeSeparator) )
        {
            strKeyInTree += m_strNodeSeparator;
        }
    }
    strKeyInTree += i_pTreeEntry->name();

    if( m_mappTreeEntries.contains(strKeyInTree) )
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKeyInTree);
    }

    m_mappTreeEntries.insert(strKeyInTree, i_pTreeEntry);

    i_pTreeEntry->setKeyInTree(strKeyInTree);

    emit_treeEntryKeyInTreeChanged(this, i_pTreeEntry, strKeyInTreePrev);

    if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
    {
        CBranchIdxTreeEntry*   pBranch = dynamic_cast<CBranchIdxTreeEntry*>(i_pTreeEntry);
        CAbstractIdxTreeEntry* pTreeEntry;
        int                    idxEntry;

        for( idxEntry = 0; idxEntry < pBranch->count(); ++idxEntry )
        {
            pTreeEntry = pBranch->at(idxEntry);

            updateKeyInTree(pTreeEntry);
        }
    } // if( i_pTreeEntry->entryType() == EIdxTreeEntryType::Branch )

} // updateKeyInTree

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Auxiliary method to delete all childs of the given branch.

    The given branch will neither be removed from the tree nor deleted.

    @param i_pBranch [in] Pointer to branch to be removed and (optionally) deleted.
*/
void CIdxTree::clear( CBranchIdxTreeEntry* i_pBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "clear",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    CAbstractIdxTreeEntry* pTreeEntry;
    int                    idxEntry;

    for( idxEntry = i_pBranch->count()-1; idxEntry >= 0; --idxEntry )
    {
        pTreeEntry = i_pBranch->at(idxEntry);

        if( pTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            clear(dynamic_cast<CBranchIdxTreeEntry*>(pTreeEntry));
        }

        // The dtor of the tree entry will remove the entry
        // from the index tree and parent branch.
        delete pTreeEntry;
        pTreeEntry = nullptr;
    }

} // clear

/*==============================================================================
public: // iterator methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns an iterator pointing to the first item of the tree.

    @param i_traversalOrder [in] Defines the way how the iterator will walk through the tree.

    @return Iterator pointing to the first item in the tree.
*/
CIdxTree::iterator CIdxTree::begin( iterator::ETraversalOrder i_traversalOrder )
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    CIdxTree::iterator itIdxTree(this, i_traversalOrder);
    if( i_traversalOrder == iterator::ETraversalOrder::Index )
    {
        for( int idxInTree = 0; idxInTree < m_arpTreeEntries.size(); ++idxInTree )
        {
            itIdxTree.m_pTreeEntryCurr = m_arpTreeEntries[idxInTree];
            if( itIdxTree.m_pTreeEntryCurr != nullptr )
            {
                break;
            }
        }
    }
    else if( i_traversalOrder == iterator::ETraversalOrder::PreOrder )
    {
        itIdxTree.m_pTreeEntryCurr = m_pRoot;
    }
    return itIdxTree;

} // iterator::begin()

//------------------------------------------------------------------------------
/*! Returns an iterator pointing to the imaginary last item of the tree.

    @return Iterator pointing to the imaginary last item in the tree.
*/
CIdxTree::iterator CIdxTree::end()
//------------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    CIdxTree::iterator itIdxTree(this, iterator::ETraversalOrder::Undefined);
    itIdxTree.m_pTreeEntryCurr = nullptr;
    return itIdxTree;

} // iterator::end()

/*==============================================================================
public: // overridable instance methods (used by friend class CAbstractIdxTreeEntry and its derivates to avoid that the tree entry base classes must inherit QObject to emit signals)
==============================================================================*/

//------------------------------------------------------------------------------
/*! Method to be called by the derived tree entry class to inform the index tree
    that a propery of the entry has been changed.

    The tree will emit the "treeEntryChanged" signal if this method is called.

    Using this method instead of connecting to a "entryChanged" signal of the
    tree entries reduces the number of signal/slot connections and avoids that
    the tree entry base class must be derived from QObject.
    */
void CIdxTree::onTreeEntryChanged( CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTreeEntryChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit_treeEntryChanged(this, i_pTreeEntry);

} // onTreeEntryChanged

/*==============================================================================
protected: // instance methods (tracing of signals)
==============================================================================*/

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to created method trace outputs
    whenever the index tree emits the "treeEntryAdded" signal.

    @param i_pIdxTree [in] Pointer to index tree.
    @param i_pTreeEntry [in] Pointer to tree entry which has been added.
*/
void CIdxTree::emit_treeEntryAdded( CIdxTree* i_pIdxTree, CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryAdded",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryAdded(i_pIdxTree, i_pTreeEntry);

} // emit_treeEntryAdded

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryChanged" signal.

    @param i_pIdxTree [in] Pointer to index tree.
    @param i_pTreeEntry [in] Pointer to tree entry which has been changed.
*/
void CIdxTree::emit_treeEntryChanged( CIdxTree* i_pIdxTree, CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryChanged(i_pIdxTree, i_pTreeEntry);

} // emit_treeEntryChanged

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryAboutToBeRemoved" signal.

    @param i_pIdxTree [in] Pointer to index tree.
    @param i_pTreeEntry [in] Pointer to tree entry which is goint to be removed.
*/
void CIdxTree::emit_treeEntryAboutToBeRemoved( CIdxTree* i_pIdxTree, CAbstractIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryAboutToBeRemoved",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryAboutToBeRemoved(i_pIdxTree, i_pTreeEntry);

} // emit_treeEntryAboutToBeRemoved

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryRemoved" signal.

    @param i_pIdxTree [in] Pointer to index tree.
    @param i_pTreeEntry [in] Pointer to tree entry which has been removed.
    @param i_strKeyInTree [in] Unique key of the entry valid before the entry was removed from the tree.
    @param i_idxInTree [in] Index of the entry valid before the entry was removed from the tree.
*/
void CIdxTree::emit_treeEntryRemoved(
    CIdxTree*              i_pIdxTree,
    CAbstractIdxTreeEntry* i_pTreeEntry,
    const QString&         i_strKeyInTree,
    int                    i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", KeyInTree: " + i_strKeyInTree;
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryRemoved",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryRemoved(i_pIdxTree, i_pTreeEntry, i_strKeyInTree, i_idxInTree);

} // emit_treeEntryRemoved

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryAboutToBeMoved" signal.

    @param i_pIdxTree [in] Pointer to index tree.
    @param i_pTreeEntry [in] Pointer to tree entry which is going to be moved to another parent branch.
    @param i_pTargetBranch [in] Pointer to target branch to which the entry will be moved.
*/
void CIdxTree::emit_treeEntryAboutToBeMoved(
    CIdxTree*              i_pIdxTree,
    CAbstractIdxTreeEntry* i_pTreeEntry,
    CBranchIdxTreeEntry*   i_pTargetBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryAboutToBeMoved",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryAboutToBeMoved(i_pIdxTree, i_pTreeEntry, i_pTargetBranch);

} // emit_treeEntryAboutToBeMoved

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryeMoved" signal.

    @param i_pIdxTree [in] Pointer to index tree.
    @param i_pTreeEntry [in] Pointer to tree entry which has been removed.
    @param i_strKeyInTreePrev [in] Unique key of the entry valid before the entry was moved to the target branch.
    @param i_pTargetBranch [in] Pointer to target branch to which the entry will be moved.
    @note The index of the entry in the index tree remains the same.
*/
void CIdxTree::emit_treeEntryMoved(
    CIdxTree*              i_pIdxTree,
    CAbstractIdxTreeEntry* i_pTreeEntry,
    const QString&         i_strKeyInTreePrev,
    CBranchIdxTreeEntry*   i_pTargetBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", KeyInTreePrev: " + i_strKeyInTreePrev;
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryMoved",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryMoved(i_pIdxTree, i_pTreeEntry, i_strKeyInTreePrev, i_pTargetBranch);

} // emit_treeEntryMoved

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryAboutToBeRenamed" signal.

    @param i_pIdxTree [in] Pointer to index tree.
    @param i_pTreeEntry [in] Pointer to tree entry which is going to be renamed.
    @param i_strNameNew [in] New name of the entry.
*/
void CIdxTree::emit_treeEntryAboutToBeRenamed(
    CIdxTree*              i_pIdxTree,
    CAbstractIdxTreeEntry* i_pTreeEntry,
    const QString&         i_strNameNew )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryAboutToBeRenamed",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryAboutToBeRenamed(i_pIdxTree, i_pTreeEntry, i_strNameNew);

} // emit_treeEntryAboutToBeRenamed

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryRenamed" signal.

    @param i_pIdxTree [in] Pointer to index tree.
    @param i_pTreeEntry [in] Pointer to tree entry which has been removed.
    @param i_strKeyInTreePrev [in] Unique key of the entry valid before the entry was moved to the target branch.
    @param i_strNamePrev [in] Old name of the tree entry.
*/
void CIdxTree::emit_treeEntryRenamed(
    CIdxTree*              i_pIdxTree,
    CAbstractIdxTreeEntry* i_pTreeEntry,
    const QString&         i_strKeyInTreePrev,
    const QString&         i_strNamePrev )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", KeyInTreePrev: " + i_strKeyInTreePrev;
        strMthInArgs += ", NamePrev: " + i_strNamePrev;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryRenamed",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryRenamed(i_pIdxTree, i_pTreeEntry, i_strKeyInTreePrev, i_strNamePrev);

} // emit_treeEntryRenamed

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryKeyInTreeChanged" signal.

    @param i_pIdxTree [in] Pointer to index tree.
    @param i_pTreeEntry [in] Pointer to tree entry which has been removed.
    @param i_strKeyInTreePrev [in] Unique key of the entry valid before the entry was moved to the target branch.
    @note The index of the entry in the index tree remains the same.
*/
void CIdxTree::emit_treeEntryKeyInTreeChanged(
    CIdxTree*              i_pIdxTree,
    CAbstractIdxTreeEntry* i_pTreeEntry,
    const QString&         i_strKeyInTreePrev )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_iTrcDetailLevel >= ETraceDetailLevelMethodArgs )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
        strMthInArgs += ", TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", KeyInTreePrev: " + i_strKeyInTreePrev;
    }

    CMethodTracer mthTracer(
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* iTrcDetailLevel    */ m_iTrcDetailLevel,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryKeyInTreeChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryKeyInTreeChanged(i_pIdxTree, i_pTreeEntry, i_strKeyInTreePrev);

} // emit_treeEntryKeyInTreeChanged


/*******************************************************************************
class CBSTree
*******************************************************************************/

#if 0

template <typename T> class CBSTree;

template <typename T>
struct SBSTreeNode
{
public:
    SBSTreeNode<T>() :
        m_val(),
        m_pUp(nullptr),
        m_pLeft(nullptr),
        m_pRight(nullptr)
    {
    }
    ~SBSTreeNode<T>()
    {
        m_pUp = nullptr;
        m_pLeft = nullptr;
        m_pRight = nullptr;
    }
public:
    SBSTreeNode& operator = ( const T& i_val )
    {
        m_val = i_val;
        return *this;
    }
    bool operator == ( SBSTreeNode& i_other )
    {
        bool bEqual = true;
        if( m_val != i_other.m_val ) {
            bEqual = false;
        } else if ( m_pUp != i_other.m_pUp ) {
            bEqual = false;
        } else if ( m_pLeft != i_other.m_pLeft ) {
            bEqual = false;
        } else if ( m_pRight != i_other.m_pRight ) {
            bEqual = false;
        }
        return bEqual;
    }
    bool operator != ( SBSTreeNode& i_other )
    {
        return !(*this == i_other);
    }
public:
    T            m_val;
    SBSTreeNode* m_pUp;
    SBSTreeNode* m_pLeft;
    SBSTreeNode* m_pRight;
};

template <typename T>
class CBSTree
{
public:
    CBSTree() :
        m_pBSTree(nullptr)
    {
    }
    ~CBSTree()
    {
        destroy(m_pBSTree);
    }
public:
    class iterator
    {
    friend class CBSTree<T>;
    enum ETraversalOrder {
        EPreOrder,
        EPostOrder,
        EInOrder
    };
    public:
        iterator() {}
        iterator( SBSTreeNode<T>* i_pNode ) : m_pNodeCurr(i_pNode) {}
    public:
        T operator * () const;
        bool operator == ( iterator& i_other );
        bool operator != ( iterator& i_other );
        iterator& operator ++ ();
    private:
        SBSTreeNode<T>* m_pRoot = nullptr;
        SBSTreeNode<T>* m_pNodeCurr = nullptr;
        ETraversalOrder m_travOrd = EPostOrder;
    #ifdef _DEBUG
    public:
        int m_iSearchSteps = 0;
    #endif
    };
public:
    void add( const T& i_val );
    void destroy( SBSTreeNode<T>* i_pNode ) { /* not necessary in this test*/ }
    iterator search( const T& i_val );
public:
    iterator beginPreorder();
    iterator beginPostorder();
    iterator beginInorder();
    iterator end();
protected:
    void traversePostorder( SBSTreeNode<T>*& i_pNode );
protected:
    void add( SBSTreeNode<T>*& i_pNode, SBSTreeNode<T>* i_pNodeUp, const T& i_val );
private:
    SBSTreeNode<T>* m_pBSTree;
};

template <typename T>
void CBSTree<T>::add( const T& i_val )
{
    add(m_pBSTree, nullptr, i_val);
}

template <typename T>
void CBSTree<T>::add( SBSTreeNode<T>*& i_pNode, SBSTreeNode<T>* i_pNodeUp, const T& i_val )
{
    if( i_pNode == nullptr )
    {
        i_pNode = new SBSTreeNode<T>();
        *i_pNode = i_val;
        i_pNode->m_pUp = i_pNodeUp;
    }
    else if( i_val == i_pNode->m_val )
    {
        throw std::exception("Value already existing");
    }
    else if( i_val < i_pNode->m_val )
    {
        add(i_pNode->m_pLeft, i_pNode, i_val);
    }
    else // if( i_val > i_pNode->m_val )
    {
        add(i_pNode->m_pRight, i_pNode, i_val);
    }
}

template <typename T>
typename CBSTree<T>::iterator CBSTree<T>::search( const T& i_val )
{
    SBSTreeNode<T>* pNode = m_pBSTree;
    #ifdef _DEBUG
    auto iSearchSteps = 0;
    #endif
    while( pNode != nullptr )
    {
        if( i_val == pNode->m_val )
        {
            break;
        }
        else if( i_val < pNode->m_val )
        {
            pNode = pNode->m_pLeft;
        }
        else // if( i_val > pNode->m_val )
        {
            pNode = pNode->m_pRight;
        }
        #ifdef _DEBUG
        ++iSearchSteps;
        #endif
    }
    CBSTree<T>::iterator it(pNode);
    #ifdef _DEBUG
    it.m_iSearchSteps = iSearchSteps;
    #endif
    return it;
}

template <typename T>
typename CBSTree<T>::iterator CBSTree<T>::beginPreorder()
{
    CBSTree<T>::iterator it(m_pBSTree);
    it.m_pRoot = m_pBSTree;
    it.m_travOrd = CBSTree<T>::iterator::EPreOrder;
    return it;
}

template <typename T>
typename CBSTree<T>::iterator CBSTree<T>::beginPostorder()
{
    SBSTreeNode<T>* pNode = m_pBSTree;
    traversePostorder(pNode);
    CBSTree<T>::iterator it(pNode);
    it.m_pRoot = m_pBSTree;
    it.m_travOrd = CBSTree<T>::iterator::EPostOrder;
    return it;
}

template <typename T>
typename CBSTree<T>::iterator CBSTree<T>::beginInorder()
{
    SBSTreeNode<T>* pNode = m_pBSTree;
    if( pNode != nullptr )
    {
        while( pNode->m_pLeft != nullptr )
        {
            pNode = pNode->m_pLeft;
        }
    }
    CBSTree<T>::iterator it(pNode);
    it.m_pRoot = m_pBSTree;
    it.m_travOrd = CBSTree<T>::iterator::EInOrder;
    return it;
}

template <typename T>
typename CBSTree<T>::iterator CBSTree<T>::end()
{
    return iterator(nullptr);
}

template <typename T>
void CBSTree<T>::traversePostorder( SBSTreeNode<T>*& i_pNode )
{
    if( i_pNode != nullptr )
    {
        if( i_pNode->m_pLeft != nullptr )
        {
            i_pNode = i_pNode->m_pLeft;
            traversePostorder(i_pNode);
        }
        else if( i_pNode->m_pRight != nullptr )
        {
            i_pNode = i_pNode->m_pRight;
            traversePostorder(i_pNode);
        }
    }
}

template <typename T>
T CBSTree<T>::iterator::operator * () const
{
    return m_pNodeCurr->m_val;
}

template <typename T>
bool CBSTree<T>::iterator::operator == ( iterator& i_other )
{
    bool bEqual = false;
    if( m_pNodeCurr == nullptr && i_other.m_pNodeCurr == nullptr )
    {
        bEqual = true;
    }
    else if( m_pNodeCurr != nullptr && i_other.m_pNodeCurr != nullptr )
    {
        bEqual = (*m_pNodeCurr == *i_other.m_pNodeCurr);
    }
    return bEqual;
}

template <typename T>
bool CBSTree<T>::iterator::operator != ( iterator& i_other )
{
    return !(*this == i_other);
}

template <typename T>
typename CBSTree<T>::iterator& CBSTree<T>::iterator::operator ++ ()
{
    if( m_pNodeCurr == nullptr )
    {
        throw std::exception("iterator out of tree bounds");
    }
    SBSTreeNode<T>* pNodePrev = m_pNodeCurr;
    SBSTreeNode<T>* pNodeNew = nullptr;
    if( m_travOrd == EPreOrder )
    {
        if( m_pNodeCurr->m_pLeft != nullptr )
        {
            pNodeNew = m_pNodeCurr->m_pLeft;
        }
        else if( m_pNodeCurr->m_pRight != nullptr )
        {
            pNodeNew = m_pNodeCurr->m_pRight;
        }
        else if( m_pNodeCurr->m_pUp != nullptr )
        {
            pNodeNew = m_pNodeCurr->m_pUp;
            while( pNodeNew != nullptr )
            {
                if( (pNodeNew->m_pRight != nullptr) && (pNodeNew->m_pRight != pNodePrev) )
                {
                    pNodeNew = pNodeNew->m_pRight;
                    break;
                }
                pNodePrev = pNodeNew;
                pNodeNew = pNodeNew->m_pUp;
            }
        }
        m_pNodeCurr = pNodeNew;
    }
    else if( m_travOrd == EPostOrder )
    {
        if( m_pNodeCurr->m_pLeft == nullptr )
        {
            pNodeNew = m_pNodeCurr->m_pUp;
            if( pNodeNew->m_pRight != nullptr && pNodeNew->m_pRight != pNodePrev )
            {
                pNodeNew = pNodeNew->m_pRight;
                while( pNodeNew->m_pLeft != nullptr )
                {
                    pNodeNew = pNodeNew->m_pLeft;
                }
            }
        }
        else
        {
            if( m_pNodeCurr->m_pUp == nullptr )
            {
                if( m_pNodeCurr != m_pRoot )
                {
                    pNodeNew = m_pNodeCurr->m_pRight;
                }
            }
            else if( m_pNodeCurr->m_pUp->m_pRight != nullptr && m_pNodeCurr->m_pUp->m_pRight != pNodePrev )
            {
                pNodeNew = m_pNodeCurr->m_pUp->m_pRight;
                while( pNodeNew->m_pLeft != nullptr )
                {
                    pNodeNew = pNodeNew->m_pLeft;
                }
            }
            else
            {
                pNodeNew = m_pNodeCurr->m_pUp;
            }
        }
        m_pNodeCurr = pNodeNew;
    }
    else if( m_travOrd == EInOrder )
    {
        if( m_pNodeCurr->m_pLeft == nullptr )
        {
            if( m_pNodeCurr->m_pRight != nullptr )
            {
                pNodeNew = m_pNodeCurr->m_pRight;
                while( pNodeNew->m_pLeft != nullptr )
                {
                    pNodeNew = pNodeNew->m_pLeft;
                }
            }
            else
            {
                if( m_pNodeCurr->m_pUp != nullptr )
                {
                    if( m_pNodeCurr->m_pUp != nullptr && m_pNodeCurr->m_pUp->m_pLeft == m_pNodeCurr )
                    {
                        pNodeNew = m_pNodeCurr->m_pUp;
                    }
                    else
                    {
                        pNodePrev = m_pNodeCurr;
                        pNodeNew = m_pNodeCurr->m_pUp;
                        while( pNodeNew != nullptr )
                        {
                            pNodePrev = pNodeNew;
                            pNodeNew = pNodeNew->m_pUp;
                            if( pNodeNew != nullptr && pNodeNew->m_pLeft == pNodePrev )
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if( m_pNodeCurr->m_pRight != nullptr )
            {
                pNodeNew = m_pNodeCurr->m_pRight;
                while( pNodeNew->m_pLeft != nullptr )
                {
                    pNodeNew = pNodeNew->m_pLeft;
                }
            }
            else
            {
                pNodePrev = m_pNodeCurr;
                pNodeNew = m_pNodeCurr->m_pUp;
                while( pNodeNew != nullptr )
                {
                    if( pNodeNew != nullptr && pNodeNew->m_pLeft == pNodePrev )
                    {
                        break;
                    }
                    pNodePrev = pNodeNew;
                    pNodeNew = pNodeNew->m_pUp;
                }
            }
        }
        m_pNodeCurr = pNodeNew;
    }
    return *this;
}

#endif

/*******************************************************************************
Entry point for the application.
*******************************************************************************/

#if 0
#define _ArrLen(_arr) static_cast<int>((sizeof(_arr)/sizeof(_arr[0])))

//------------------------------------------------------------------------------
int main( int argc, char* argv[] )
//------------------------------------------------------------------------------
{
    int iResult = 0;

    int iSize;

    std::cout << "\n";
    std::cout << "**************************************************************************\n";
    std::cout << "                  6                                                       \n";
    std::cout << "            4           8                                                 \n";
    std::cout << "         2     5     7     9                                              \n";
    std::cout << "**************************************************************************\n";
    std::cout << "\n";

    CBSTree<int> bt1;

    auto it = bt1.end();

    int ariBT1[] = { 6, 4, 8, 2, 5, 7, 9 };

    for( auto idxVal = 0; idxVal < _ArrLen(ariBT1); ++idxVal )
    {
        bt1.add( ariBT1[idxVal] );
    }

    //--------------------------------------------------------------------------
    // Search
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test Search\n";
    std::cout << "-------------\n";

    auto iSearchResult1 = 0;

    //                                                6
    //                                          4           8
    //                                       2     5     7     9
    //int ariBT1[]                      = {  6,  4,  8,  2,  5,  7,  9 };
    int ariSearchValsBT1[]              = {  6,  4,  8,  2,  5,  7,  9 };
    int ariDesiredResultsSearchVals1[]  = {  0,  0,  0,  0,  0,  0,  0 };
    int ariActualResultsSearchVals1[]   = { -1, -1, -1, -1, -1, -1, -1 };
    #ifdef _DEBUG
    int ariDesiredResultsSearchSteps1[] = {  0,  1,  1,  2,  2,  2,  2 };
    int ariActualResultsSearchSteps1[]  = { -1, -1, -1, -1, -1, -1, -1 };
    #endif

    for( auto idxVal = 0; idxVal < _ArrLen(ariSearchValsBT1); ++idxVal )
    {
        it = bt1.search(ariSearchValsBT1[idxVal]);

        if( it == bt1.end() )
        {
            ariActualResultsSearchVals1[idxVal] = -1;
        }
        else
        {
            ariActualResultsSearchVals1[idxVal] = 0;

            #ifdef _DEBUG
            ariActualResultsSearchSteps1[idxVal] = it.m_iSearchSteps;
            #endif
        }
        if( ariActualResultsSearchVals1[idxVal] != ariDesiredResultsSearchVals1[idxVal] )
        {
            iSearchResult1 = -1;
        }
        #ifdef _DEBUG
        if( ariActualResultsSearchSteps1[idxVal] != ariDesiredResultsSearchSteps1[idxVal] )
        {
            iSearchResult1 = -1;
        }
        #endif
    }
    if( iResult == 0 )
    {
        iResult = iSearchResult1;
    }

    std::cout << "Search Values:          ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariSearchValsBT1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariSearchValsBT1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Desired Search Results: ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsSearchVals1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Search Results:  ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsSearchVals1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Desired Search Steps:   ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsSearchSteps1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Search Steps:    ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsSearchSteps1[idxVal];
    }
    std::cout << "\n";

    if( iSearchResult1 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    //--------------------------------------------------------------------------
    // Preorder
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test PreOrder\n";
    std::cout << "-------------\n";

    auto iPreorderResult1 = 0;

    //                                             6
    //                                       4           8
    //                                    2     5     7     9
    int ariDesiredResultsPreorder1[] = {  6,  4,  2,  5,  8,  7,  9 };
    int ariActualResultsPreorder1[]  = { -1, -1, -1, -1, -1, -1, -1 };

    it = bt1.beginPreorder();

    iSize = 0;
    for( auto idxVal = 0; it != bt1.end(); ++it, ++idxVal, ++iSize )
    {
        ariActualResultsPreorder1[idxVal] = *it;

        if( ariActualResultsPreorder1[idxVal] != ariDesiredResultsPreorder1[idxVal] )
        {
            iPreorderResult1 = -1;
        }
    }
    if( iSize != _ArrLen(ariBT1) )
    {
        iPreorderResult1 = -1;
    }
    if( iResult == 0 )
    {
        iResult = iPreorderResult1;
    }

    std::cout << "Desired Results: ";
    std::cout << "[" << _ArrLen(ariBT1) << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPreorder1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsPreorder1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Results:  ";
    std::cout << "[" << iSize << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPreorder1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsPreorder1[idxVal];
    }
    std::cout << "\n";

    if( iPreorderResult1 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    //--------------------------------------------------------------------------
    // PostOrder
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test PostOrder\n";
    std::cout << "--------------\n";

    auto iPostorderResult1 = 0;

    //                                              6
    //                                        4           8
    //                                     2     5     7     9
    //int ariBT1[]                    = {  6,  4,  8,  2,  5,  7,  9 };
    int ariDesiredResultsPostorder1[] = {  2,  5,  4,  7,  9,  8,  6 };
    int ariActualResultsPostorder1[]  = { -1, -1, -1, -1, -1, -1, -1 };

    it = bt1.beginPostorder();

    iSize = 0;
    for( auto idxVal = 0; it != bt1.end(); ++it, ++idxVal, ++iSize )
    {
        ariActualResultsPostorder1[idxVal] = *it;

        if( ariActualResultsPostorder1[idxVal] != ariDesiredResultsPostorder1[idxVal] )
        {
            iPostorderResult1 = -1;
        }
    }
    if( iSize != _ArrLen(ariBT1) )
    {
        iPostorderResult1 = -1;
    }
    if( iResult == 0 )
    {
        iResult = iPostorderResult1;
    }

    std::cout << "Desired Results: ";
    std::cout << "[" << _ArrLen(ariBT1) << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPostorder1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsPostorder1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Results:  ";
    std::cout << "[" << iSize << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPostorder1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsPostorder1[idxVal];
    }
    std::cout << "\n";

    if( iPostorderResult1 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    //--------------------------------------------------------------------------
    // InOrder
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test InOrder\n";
    std::cout << "------------\n";

    auto iInorderResult1 = 0;

    //                                                    6
    //                                              4           8
    //                                           2     5     7     9
    //int ariBT1[]                  = {  6,  4,  8,  2,  5,  7,  9 };
    int ariDesiredResultsInorder1[] = {  2,  4,  5,  6,  7,  8,  9 };
    int ariActualResultsInorder1[]  = { -1, -1, -1, -1, -1, -1, -1 };

    it = bt1.beginInorder();

    iSize = 0;
    for( auto idxVal = 0; it != bt1.end(); ++it, ++idxVal, ++iSize )
    {
        ariActualResultsInorder1[idxVal] = *it;

        if( ariActualResultsInorder1[idxVal] != ariDesiredResultsInorder1[idxVal] )
        {
            iInorderResult1 = -1;
        }
    }
    if( iSize != _ArrLen(ariBT1) )
    {
        iInorderResult1 = -1;
    }
    if( iResult == 0 )
    {
        iResult = iInorderResult1;
    }

    std::cout << "Desired Results: ";
    std::cout << "[" << _ArrLen(ariBT1) << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsInorder1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsInorder1[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Results:  ";
    std::cout << "[" << iSize << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsInorder1); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsInorder1[idxVal];
    }
    std::cout << "\n";

    if( iInorderResult1 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    std::cout << "\n";
    std::cout << "**************************************************************************\n";
    std::cout << "                 59                                                       \n";
    std::cout << "         49                  81                                           \n";
    std::cout << "     45                  79                                               \n";
    std::cout << "         46          77                                                   \n";
    std::cout << "             47                                                           \n";
    std::cout << "**************************************************************************\n";
    std::cout << "\n";

    CBSTree<int> bt2;

    int ariBT2[] = { 59, 81, 79, 77, 49, 45, 46, 47 };

    for( auto idxVal = 0; idxVal < _ArrLen(ariBT2); ++idxVal )
    {
        bt2.add( ariBT2[idxVal] );
    }

    //--------------------------------------------------------------------------
    // Search
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test Search\n";
    std::cout << "-------------\n";

    auto iSearchResult2 = 0;

    //                                                  59
    //                                          49                  81
    //                                      45                  79
    //                                          46          77
    //                                              47
    //int ariBT2[]                      = { 59, 81, 79, 77, 49, 45, 46, 47 };
    int ariSearchValsBT2[]              = { 59, 81, 79, 77, 49, 45, 46, 47 };
    int ariDesiredResultsSearchVals2[]  = {  0,  0,  0,  0,  0,  0,  0,  0 };
    int ariActualResultsSearchVals2[]   = { -1, -1, -1, -1, -1, -1, -1, -1 };
    #ifdef _DEBUG
    int ariDesiredResultsSearchSteps2[] = {  0,  1,  2,  3,  1,  2,  3,  4 };
    int ariActualResultsSearchSteps2[]  = { -1, -1, -1, -1, -1, -1, -1, -1 };
    #endif

    for( auto idxVal = 0; idxVal < _ArrLen(ariSearchValsBT2); ++idxVal )
    {
        it = bt2.search(ariSearchValsBT2[idxVal]);

        if( it == bt2.end() )
        {
            ariActualResultsSearchVals2[idxVal] = -1;
        }
        else
        {
            ariActualResultsSearchVals2[idxVal] = 0;

            #ifdef _DEBUG
            ariActualResultsSearchSteps2[idxVal] = it.m_iSearchSteps;
            #endif
        }
        if( ariActualResultsSearchVals2[idxVal] != ariDesiredResultsSearchVals2[idxVal] )
        {
            iSearchResult1 = -1;
        }
        #ifdef _DEBUG
        if( ariActualResultsSearchSteps2[idxVal] != ariDesiredResultsSearchSteps2[idxVal] )
        {
            iSearchResult1 = -1;
        }
        #endif
    }
    if( iResult == 0 )
    {
        iResult = iSearchResult1;
    }

    std::cout << "Search Values:          ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariSearchValsBT2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariSearchValsBT2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Desired Search Results: ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsSearchVals2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Search Results:  ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsSearchVals2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Desired Search Steps:   ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsSearchSteps2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Search Steps:    ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariBT2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsSearchSteps2[idxVal];
    }
    std::cout << "\n";

    if( iSearchResult1 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    //--------------------------------------------------------------------------
    // Preorder
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test PreOrder\n";
    std::cout << "-------------\n";

    auto iPreorderResult2 = 0;

    //                                                  59
    //                                          49                  81
    //                                      45                  79
    //                                          46          77
    //                                              47
    int ariDesiredResultsPreorder2[] = { 59, 49, 45, 46, 47, 81, 79, 77 };
    int ariActualResultsPreorder2[]  = { -1, -1, -1, -1, -1, -1, -1, -1 };

    it = bt2.beginPreorder();

    iSize = 0;
    for( auto idxVal = 0; it != bt2.end(); ++it, ++idxVal, ++iSize )
    {
        ariActualResultsPreorder2[idxVal] = *it;

        if( ariActualResultsPreorder2[idxVal] != ariDesiredResultsPreorder2[idxVal] )
        {
            iPreorderResult2 = -1;
        }
    }
    if( iSize != _ArrLen(ariBT2) )
    {
        iPreorderResult2 = -1;
    }
    if( iResult == 0 )
    {
        iResult = iPreorderResult2;
    }

    std::cout << "Desired Results: ";
    std::cout << "[" << _ArrLen(ariBT2) << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPreorder2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsPreorder2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Results:  ";
    std::cout << "[" << iSize << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPreorder2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsPreorder2[idxVal];
    }
    std::cout << "\n";

    if( iPreorderResult2 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    //--------------------------------------------------------------------------
    // PostOrder
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test PostOrder\n";
    std::cout << "--------------\n";

    auto iPostorderResult2 = 0;

    //                                                  59
    //                                          49                  81
    //                                      45                  79
    //                                          46          77
    //                                              47
    int ariDesiredResultsPostorder2[] = { 47, 46, 45, 49, 77, 79, 81, 59 };
    int ariActualResultsPostorder2[]  = { -1, -1, -1, -1, -1, -1, -1, -1 };

    it = bt2.beginPostorder();

    iSize = 0;
    for( auto idxVal = 0; it != bt2.end(); ++it, ++idxVal, ++iSize )
    {
        ariActualResultsPostorder2[idxVal] = *it;

        if( ariActualResultsPostorder2[idxVal] != ariDesiredResultsPostorder2[idxVal] )
        {
            iPostorderResult2 = -1;
        }
    }
    if( iSize != _ArrLen(ariBT2) )
    {
        iPostorderResult2 = -1;
    }
    if( iResult == 0 )
    {
        iResult = iPostorderResult2;
    }

    std::cout << "Desired Results: ";
    std::cout << "[" << _ArrLen(ariBT2) << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPostorder2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsPostorder2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Results:  ";
    std::cout << "[" << iSize << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsPostorder2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsPostorder2[idxVal];
    }
    std::cout << "\n";

    if( iPostorderResult2 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    //--------------------------------------------------------------------------
    // InOrder
    //--------------------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Test InOrder\n";
    std::cout << "------------\n";

    auto iInorderResult2 = 0;

    //                                                  59
    //                                          49                  81
    //                                      45                  79
    //                                          46          77
    //                                              47
    int ariDesiredResultsInorder2[] = { 45, 46, 47, 49, 59, 77, 79, 81 };
    int ariActualResultsInorder2[]  = { -1, -1, -1, -1, -1, -1, -1, -1 };

    it = bt2.beginInorder();

    iSize = 0;
    for( auto idxVal = 0; it != bt2.end(); ++it, ++idxVal, ++iSize )
    {
        ariActualResultsInorder2[idxVal] = *it;

        if( ariActualResultsInorder2[idxVal] != ariDesiredResultsInorder2[idxVal] )
        {
            iInorderResult2 = -1;
        }
    }
    if( iSize != _ArrLen(ariBT2) )
    {
        iInorderResult2 = -1;
    }
    if( iResult == 0 )
    {
        iResult = iInorderResult2;
    }

    std::cout << "Desired Results: ";
    std::cout << "[" << _ArrLen(ariBT2) << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsInorder2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariDesiredResultsInorder2[idxVal];
    }
    std::cout << "\n";

    std::cout << "Actual Results:  ";
    std::cout << "[" << iSize << "] ";
    for( auto idxVal = 0; idxVal < _ArrLen(ariDesiredResultsInorder2); ++idxVal )
    {
        if( idxVal > 0 )
        {
            std::cout << ", ";
        }
        std::cout << ariActualResultsInorder2[idxVal];
    }
    std::cout << "\n";

    if( iInorderResult2 == 0 )
    {
        std::cout << "TEST OK\n";
    }
    else
    {
        std::cout << "TEST FAILED\n";
    }

    return iResult;

} // main
#endif
