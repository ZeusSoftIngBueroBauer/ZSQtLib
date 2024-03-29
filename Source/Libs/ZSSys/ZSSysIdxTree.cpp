/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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
#include "ZSSys/ZSSysMutex.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcMthFile.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


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
CIdxTreeEntry* CIdxTree::iterator::operator * () const
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

    CIdxTreeEntry* pTreeEntryNew = nullptr;

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
            CIdxTreeEntry* pBranchCurr = m_pIdxTree->root();

            if( pBranchCurr->count() > 0 )
            {
                pTreeEntryNew = pBranchCurr->at(0);
            }
        }
        else // if( m_pTreeEntryCurr != m_pIdxTree->root() )
        {
            if( m_pTreeEntryCurr->isLeave() )
            {
                int idxInParentBranch = m_pTreeEntryCurr->indexInParentBranch();
                CIdxTreeEntry* pBranchParent = m_pTreeEntryCurr->parentBranch();

                if( idxInParentBranch >= (pBranchParent->count()-1) )
                {
                    idxInParentBranch = pBranchParent->indexInParentBranch();
                    pBranchParent = pBranchParent->parentBranch();

                    while( pBranchParent != nullptr && idxInParentBranch >= (pBranchParent->count()-1) )
                    {
                        idxInParentBranch = pBranchParent->indexInParentBranch();
                        pBranchParent = pBranchParent->parentBranch();
                    }
                }
                if( pBranchParent != nullptr && idxInParentBranch < (pBranchParent->count()-1) )
                {
                    pTreeEntryNew = pBranchParent->at(idxInParentBranch+1);
                }
            }
            else
            {
                CIdxTreeEntry* pBranchCurr = m_pTreeEntryCurr;

                if( pBranchCurr->count() > 0 )
                {
                    pTreeEntryNew = pBranchCurr->at(0);
                }
                else
                {
                    int idxInParentBranch = pBranchCurr->indexInParentBranch();
                    CIdxTreeEntry* pBranchParent = pBranchCurr->parentBranch();

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
           The trace server passes itself as the parent object. If the parent object is the
           trace server the index tree will not create a trace admin object to trace the method calls.
    @param i_eTrcDetailLevel [in] If greater than 0 (EMethodTraceDetailLevel::None), method tracing for
           the index tree is activated. As trace admin objects itself are leaves of an index tree
           trace admin objects cannot be used for controlling the trace detail level of the index tree.
    @param i_eTrcDetailLevelMutex [in] If greater than 0 (EMethodTraceDetailLevel::None), method tracing for
           the mutex protecting the index tree is activated.
*/
CIdxTree::CIdxTree(
    const QString& i_strObjName,
    CIdxTreeEntry* i_pRootTreeEntry,
    const QString& i_strNodeSeparator,
    bool           i_bCreateMutex,
    QObject*       i_pObjParent,
    EMethodTraceDetailLevel i_eTrcDetailLevel,
    EMethodTraceDetailLevel i_eTrcDetailLevelMutex ) :
//------------------------------------------------------------------------------
    QObject(i_pObjParent),
    m_strNodeSeparator(i_strNodeSeparator),
    m_pMtx(nullptr),
    m_mappTreeEntries(),
    m_arpTreeEntries(),
    m_mapFreeIdxs(),
    m_mappShortcutTreeEntries(),
    m_pRoot(i_pRootTreeEntry),
    m_eTrcMthFileDetailLevel(i_eTrcDetailLevel),
    m_pTrcMthFile(nullptr),
    m_pTrcAdminObj(nullptr)
{
    setObjectName(i_strObjName);

    // If the tree's parent is the trace server or the trace client the
    // detail level of trace outputs may not be controlled by trace admin
    // objects as they belong to the index tree of the trace server or client.
    if( dynamic_cast<CTrcServer*>(i_pObjParent) == nullptr )
    {
        m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

        if( m_pTrcAdminObj != nullptr )
        {
            m_eTrcMthFileDetailLevel = m_pTrcAdminObj->getMethodCallsTraceDetailLevel();

            QObject::connect(
                m_pTrcAdminObj, &CTrcAdminObj::changed,
                this, &CIdxTree::onTrcAdminObjChanged,
                Qt::DirectConnection );
        }
    }
    else
    {
        QString strLocalTrcFileAbsFilePath = CTrcServer::GetLocalTrcFileAbsoluteFilePath();
        m_pTrcMthFile = CTrcMthFile::Alloc(strLocalTrcFileAbsFilePath);
    }

    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = i_strObjName;
        strMthInArgs += ", RootTreeEntry: " + QString(i_pRootTreeEntry == nullptr ? "nullptr" : i_pRootTreeEntry->name());
        strMthInArgs += ", NodeSeparator: " + i_strNodeSeparator;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_bCreateMutex )
    {
        #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        m_pMtx = new CRecursiveMutex("CIdxTree-" + i_strObjName, i_eTrcDetailLevelMutex);
        #else
        m_pMtx = new CMutex("CIdxTree-" + i_strObjName, i_eTrcDetailLevelMutex, QMutex::Recursive);
        #endif
    }

    if( m_pRoot == nullptr )
    {
        m_pRoot = new CIdxTreeEntry(CIdxTreeEntry::EEntryType::Root, i_strObjName);
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
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    if( m_pMtx != nullptr )
    {
        m_pMtx->lock();
    }

    emit aboutToBeDestroyed();

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

    if( m_pTrcMthFile != nullptr )
    {
        m_pTrcMthFile->close();
        CTrcMthFile::Free(m_pTrcMthFile);
    }

    mthTracer.onAdminObjAboutToBeReleased();

    if( m_pTrcAdminObj != nullptr )
    {
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }

    m_strNodeSeparator.clear();
    m_pMtx = nullptr;
    m_mappTreeEntries.clear();
    m_arpTreeEntries.clear();
    m_mapFreeIdxs.clear();
    m_mappShortcutTreeEntries.clear();
    m_pRoot = nullptr;
    m_eTrcMthFileDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_pTrcMthFile = nullptr;
    m_pTrcAdminObj = nullptr;

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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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
QString CIdxTree::buildPathStr(
    const QString& i_str1, const QString& i_str2 ) const
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
QString CIdxTree::buildPathStr(
    const QString& i_str1, const QString& i_str2, const QString& i_str3 ) const
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
QString CIdxTree::buildPathStr(
    const QString& i_str1, const QString& i_str2,
    const QString& i_str3, const QString& i_str4 ) const
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
QString CIdxTree::buildPathStr(
    const QString& i_str1, const QString& i_str2,
    const QString& i_str3, const QString& i_str4,
    const QString& i_str5 ) const
//------------------------------------------------------------------------------
{
    return ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2, i_str3, i_str4, i_str5);
}

//------------------------------------------------------------------------------
/*! Generates the key of the associated tree node from the given node type and string.

    Example:

        QString strKey = buildKeyInTreeStr("L", "User");
        // strKey = "L:User".

    @param i_strEntryTypeSymbol [in] Symbol of the entry type ("L", "B" or "R").
    @param i_str1 [in] String 1.
*/
QString CIdxTree::buildKeyInTreeStr(
    const QString& i_strEntryTypeSymbol, const QString& i_str1 ) const
//------------------------------------------------------------------------------
{
    return i_strEntryTypeSymbol + ":" + i_str1;
}

//------------------------------------------------------------------------------
/*! Generates the key of the associated tree node from the given node type and the given single strings.

    Example:

        QString strKey = buildKeyInTreeStr("L", "User", "Data");
        // strKey = "L:User/Data".

    @param i_strEntryTypeSymbol [in] Symbol of the entry type ("L", "B" or "R").
    @param i_str1 [in] String 1.
    @param i_str2 [in] String 2.
*/
QString CIdxTree::buildKeyInTreeStr(
    const QString& i_strEntryTypeSymbol,
    const QString& i_str1, const QString& i_str2 ) const
//------------------------------------------------------------------------------
{
    return i_strEntryTypeSymbol + ":" + ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2);
}

//------------------------------------------------------------------------------
/*! Generates the key of the associated tree node from the given node type and the given single strings.

    Example:

        QString strKey = buildKeyInTreeStr("L", "User", "Data", "My");
        // strKey = "L:User/Data/My".

    @param i_strEntryTypeSymbol [in] Symbol of the entry type ("L", "B" or "R").
    @param i_str1 [in] String 1.
    @param i_str2 [in] String 2.
    @param i_str3 [in] String 3.
*/
QString CIdxTree::buildKeyInTreeStr(
    const QString& i_strEntryTypeSymbol,
    const QString& i_str1, const QString& i_str2, const QString& i_str3 ) const
//------------------------------------------------------------------------------
{
    return i_strEntryTypeSymbol + ":" + ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2, i_str3);
}

//------------------------------------------------------------------------------
/*! Generates the key of the associated tree node from the given node type and the given single strings.

    Example:

        QString strKey = buildKeyInTreeStr("L", "User", "Data", "My", "Special");
        // strKey = "L:User/Data/My/Special";

    @param i_strEntryTypeSymbol [in] Symbol of the entry type ("L", "B" or "R").
    @param i_str1 [in] String 1.
    @param i_str2 [in] String 2.
    @param i_str3 [in] String 3.
    @param i_str4 [in] String 4.
*/
QString CIdxTree::buildKeyInTreeStr(
    const QString& i_strEntryTypeSymbol,
    const QString& i_str1, const QString& i_str2,
    const QString& i_str3, const QString& i_str4 ) const
//------------------------------------------------------------------------------
{
    return i_strEntryTypeSymbol + ":" + ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2, i_str3, i_str4);
}

//------------------------------------------------------------------------------
/*! Generates the key of the associated tree node from the given node type and the given single strings.

    Example:

        QString strKey = buildKeyInTreeStr("L", "User", "Data", "My", "Special", "Work");
        // strKey = "L:User/Data/My/Special/Work"

    @param i_strEntryTypeSymbol [in] Symbol of the entry type ("L", "B" or "R").
    @param i_str1 [in] String 1.
    @param i_str2 [in] String 2.
    @param i_str3 [in] String 3.
    @param i_str4 [in] String 4.
    @param i_str5 [in] String 5.
*/
QString CIdxTree::buildKeyInTreeStr(
    const QString& i_strEntryTypeSymbol,
    const QString& i_str1, const QString& i_str2,
    const QString& i_str3, const QString& i_str4,
    const QString& i_str5 ) const
//------------------------------------------------------------------------------
{
    return i_strEntryTypeSymbol + ":" + ZS::System::buildPathStr(m_strNodeSeparator, i_str1, i_str2, i_str3, i_str4, i_str5);
}

//------------------------------------------------------------------------------
/*! Splits the last substring from the given path and, if the path name contains
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
    @param o_pstrName [out] If not equal to nullptpr the name of the node is returned here.

    @return Empty string or valid node type if the path contained the node type.
*/
QString CIdxTree::splitPathStr(
    const QString& i_strPath, QString* o_pstrBranchPath, QString* o_pstrName ) const
//------------------------------------------------------------------------------
{
    QStringList strlst;
    QString     strPath = i_strPath;
    QString     strEntryType;
    QString     strName;
    QString     strBranchPath;

    for( int iEntryType = 0; iEntryType < static_cast<int>(CIdxTreeEntry::EEntryType::Count); ++iEntryType )
    {
        CIdxTreeEntry::EEntryType entryType = static_cast<CIdxTreeEntry::EEntryType>(iEntryType);
        strEntryType = CIdxTreeEntry::entryType2Str(entryType, EEnumEntryAliasStrSymbol);

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

    return strEntryType;

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
CIdxTreeEntry* CIdxTree::createBranch( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strName;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createBranch",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pBranch = new CIdxTreeEntry(CIdxTreeEntry::EEntryType::Branch, i_strName);

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
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
CIdxTreeEntry* CIdxTree::createLeave( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strName;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "createLeave",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pLeave = new CIdxTreeEntry(CIdxTreeEntry::EEntryType::Leave, i_strName);

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(pLeave == nullptr ? "null" : pLeave->name());
    }

    return pLeave;

} // createLeave

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
    // When the mutex creates trace output also this method should be traced.
    EMethodTraceDetailLevel eTrcDetailLevel = m_pMtx == nullptr ? EMethodTraceDetailLevel::None : m_pMtx->getMethodTraceDetailLevel();
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "treeEntriesVectorSize",
        /* strMethodInArgs    */ "" );

    CMutexLocker mtxLocker(m_pMtx);
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
CIdxTreeEntry* CIdxTree::getEntry( int i_idxObj ) const
//------------------------------------------------------------------------------
{
    // When the mutex creates trace output also this method should be traced.
    EMethodTraceDetailLevel eTrcDetailLevel = m_pMtx == nullptr ? EMethodTraceDetailLevel::None : m_pMtx->getMethodTraceDetailLevel();
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "getEntry",
        /* strMethodInArgs    */ "" );

    CIdxTreeEntry* pTreeEntry = nullptr;

    CMutexLocker mtxLocker(m_pMtx);

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
CIdxTreeEntry* CIdxTree::findBranch( const QString& i_strPath ) const
//------------------------------------------------------------------------------
{
    // When the mutex creates trace output also this method should be traced.
    EMethodTraceDetailLevel eTrcDetailLevel = m_pMtx == nullptr ? EMethodTraceDetailLevel::None : m_pMtx->getMethodTraceDetailLevel();
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "findBranch",
        /* strMethodInArgs    */ "" );

    QString strEntryType =
        CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strPath;
    QString strKeyInTree;

    if( i_strPath.startsWith(strEntryType + ":") )
    {
        strKeyInTree = i_strPath;
        strPath = strKeyInTree.mid(strEntryType.length() + 1);
    }
    else
    {
        strKeyInTree = strEntryType + ":" + i_strPath;
        strPath = i_strPath;
    }

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = nullptr;

    if( m_mappShortcutTreeEntries.contains(strPath) )
    {
        pTreeEntry = m_mappShortcutTreeEntries.value(strPath, nullptr);
    }
    else
    {
        pTreeEntry = m_mappTreeEntries.value(strKeyInTree, nullptr);
    }

    // Still not found? Maybe shortcuts have been added to starting
    // sections of the given path.
    if( pTreeEntry == nullptr && !m_mappShortcutTreeEntries.isEmpty() )
    {
        QStringList strlstPathSections = strPath.split(m_strNodeSeparator);
        QString strPathTmp;
        QString strPathRemaining;
        for( const QString& strSection : strlstPathSections )
        {
            if( strPathTmp.isEmpty() ) {
                strPathTmp = strSection;
            } else {
                strPathTmp += m_strNodeSeparator + strSection;
            }
            if( pTreeEntry == nullptr ) {
                pTreeEntry = m_mappShortcutTreeEntries.value(strPathTmp, nullptr);
            } else {
                strPathRemaining += m_strNodeSeparator + strSection;
            }
        }
        if( pTreeEntry != nullptr )
        {
            strKeyInTree = strEntryType + ":" + pTreeEntry->path() + strPathRemaining;
            pTreeEntry = m_mappTreeEntries.value(strKeyInTree, nullptr);
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(pTreeEntry == nullptr ? "null" : pTreeEntry->keyInTree());
    }

    return pTreeEntry;

} // findBranch

//------------------------------------------------------------------------------
/*! Returns the branch entry with the given path and name.

    @param i_strParentPath [in] Path to the parent branch. The path may already contain
                                the node entry type which will be ignored in this case.
    @param i_strBranchName [in] Name of the branch.

    @return Pointer to branch entry (nullptr if not found).
*/
CIdxTreeEntry* CIdxTree::findBranch( const QString& i_strParentPath, const QString& i_strBranchName ) const
//------------------------------------------------------------------------------
{
    // When the mutex creates trace output also this method should be traced.
    EMethodTraceDetailLevel eTrcDetailLevel = m_pMtx == nullptr ? EMethodTraceDetailLevel::None : m_pMtx->getMethodTraceDetailLevel();
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "findBranch",
        /* strMethodInArgs    */ "" );

    QString strEntryType =
        CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strPath;
    QString strKeyInTree;

    if( i_strParentPath.isEmpty() )
    {
        strPath = i_strBranchName;
        strKeyInTree = strEntryType + ":" + strPath;
    }
    else if( i_strParentPath.startsWith(strEntryType + ":") )
    {
        strKeyInTree = i_strParentPath + m_strNodeSeparator + i_strBranchName;
        strPath = strKeyInTree.mid(strEntryType.length() + 1);
    }
    else
    {
        strPath = i_strParentPath + m_strNodeSeparator + i_strBranchName;
        strKeyInTree = strEntryType + ":" + strPath;
    }

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = nullptr;

    if( m_mappShortcutTreeEntries.contains(strPath) )
    {
        pTreeEntry = m_mappShortcutTreeEntries.value(strPath, nullptr);
    }
    else
    {
        pTreeEntry = m_mappTreeEntries.value(strKeyInTree, nullptr);
    }

    // Still not found? Maybe shortcuts have been added to starting
    // sections of the given path.
    if( pTreeEntry == nullptr && !m_mappShortcutTreeEntries.isEmpty() )
    {
        QStringList strlstPathSections = strPath.split(m_strNodeSeparator);
        QString strPathTmp;
        QString strPathRemaining;
        for( const QString& strSection : strlstPathSections )
        {
            if( strPathTmp.isEmpty() ) {
                strPathTmp = strSection;
            } else {
                strPathTmp += m_strNodeSeparator + strSection;
            }
            if( pTreeEntry == nullptr ) {
                pTreeEntry = m_mappShortcutTreeEntries.value(strPathTmp, nullptr);
            } else {
                strPathRemaining += m_strNodeSeparator + strSection;
            }
        }
        if( pTreeEntry != nullptr )
        {
            strKeyInTree = strEntryType + ":" + pTreeEntry->path() + strPathRemaining;
            pTreeEntry = m_mappTreeEntries.value(strKeyInTree, nullptr);
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(pTreeEntry == nullptr ? "null" : pTreeEntry->keyInTree());
    }

    return pTreeEntry;

} // findBranch

//------------------------------------------------------------------------------
/*! Returns the leave entry with the given path.

    @param i_strPath [in] Path to the leave. The path may already contain
                          the node entry type which will be ignored in this case.

    @return Pointer to leave entry (nullptr if not found).
*/
CIdxTreeEntry* CIdxTree::findLeave( const QString& i_strPath ) const
//------------------------------------------------------------------------------
{
    // When the mutex creates trace output also this method should be traced.
    EMethodTraceDetailLevel eTrcDetailLevel = m_pMtx == nullptr ? EMethodTraceDetailLevel::None : m_pMtx->getMethodTraceDetailLevel();
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "findLeave",
        /* strMethodInArgs    */ "" );

    QString strEntryType =
        CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Leave, EEnumEntryAliasStrSymbol);
    QString strPath;
    QString strKeyInTree;

    if( i_strPath.startsWith(strEntryType + ":") )
    {
        strKeyInTree = i_strPath;
        strPath = strKeyInTree.mid(strEntryType.length() + 1);
    }
    else
    {
        strKeyInTree = strEntryType + ":" + i_strPath;
        strPath = i_strPath;
    }

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = nullptr;

    if( m_mappShortcutTreeEntries.contains(i_strPath) )
    {
        pTreeEntry = m_mappShortcutTreeEntries.value(i_strPath, nullptr);
    }
    else
    {
        pTreeEntry = m_mappTreeEntries.value(strKeyInTree, nullptr);
    }

    // Still not found? Maybe shortcuts have been added to starting
    // sections of the given path.
    if( pTreeEntry == nullptr && !m_mappShortcutTreeEntries.isEmpty() )
    {
        QStringList strlstPathSections = strPath.split(m_strNodeSeparator);
        QString strPathTmp;
        QString strPathRemaining;
        for( const QString& strSection : strlstPathSections )
        {
            if( strPathTmp.isEmpty() ) {
                strPathTmp = strSection;
            } else {
                strPathTmp += m_strNodeSeparator + strSection;
            }
            if( pTreeEntry == nullptr ) {
                pTreeEntry = m_mappShortcutTreeEntries.value(strPathTmp, nullptr);
            } else {
                strPathRemaining += m_strNodeSeparator + strSection;
            }
        }
        if( pTreeEntry != nullptr )
        {
            strKeyInTree = strEntryType + ":" + pTreeEntry->path() + strPathRemaining;
            pTreeEntry = m_mappTreeEntries.value(strKeyInTree, nullptr);
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(pTreeEntry == nullptr ? "null" : pTreeEntry->keyInTree());
    }

    return pTreeEntry;

} // findLeave

//------------------------------------------------------------------------------
/*! Returns the leave entry with the given path and name.

    @param i_strParentPath [in] Path to the parent branch. The path may already contain
                                the node entry type which will be ignored in this case.
    @param i_strLeaveName [in] Name of the leave.

    @return Pointer to branch entry (nullptr if not found).
*/
CIdxTreeEntry* CIdxTree::findLeave( const QString& i_strParentPath, const QString& i_strLeaveName ) const
//------------------------------------------------------------------------------
{
    // When the mutex creates trace output also this method should be traced.
    EMethodTraceDetailLevel eTrcDetailLevel = m_pMtx == nullptr ? EMethodTraceDetailLevel::None : m_pMtx->getMethodTraceDetailLevel();
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "findLeave",
        /* strMethodInArgs    */ "" );

    QString strEntryType =
        CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Leave, EEnumEntryAliasStrSymbol);
    QString strPath;
    QString strKeyInTree;

    if( i_strParentPath.isEmpty() )
    {
        strPath = i_strLeaveName;
        strKeyInTree = strEntryType + ":" + strPath;
    }
    else if( i_strParentPath.startsWith(strEntryType + ":") )
    {
        strKeyInTree = i_strParentPath + m_strNodeSeparator + i_strLeaveName;
        strPath = strKeyInTree.mid(strEntryType.length() + 1);
    }
    else
    {
        strPath = i_strParentPath + m_strNodeSeparator + i_strLeaveName;
        strKeyInTree = strEntryType + ":" + strPath;
    }

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = nullptr;

    if( m_mappShortcutTreeEntries.contains(strPath) )
    {
        pTreeEntry = m_mappShortcutTreeEntries.value(strPath, nullptr);
    }
    else
    {
        pTreeEntry = m_mappTreeEntries.value(strKeyInTree, nullptr);
    }

    // Still not found? Maybe shortcuts have been added to starting
    // sections of the given path.
    if( pTreeEntry == nullptr && !m_mappShortcutTreeEntries.isEmpty() )
    {
        QStringList strlstPathSections = strPath.split(m_strNodeSeparator);
        QString strPathTmp;
        QString strPathRemaining;
        for( const QString& strSection : strlstPathSections )
        {
            if( strPathTmp.isEmpty() ) {
                strPathTmp = strSection;
            } else {
                strPathTmp += m_strNodeSeparator + strSection;
            }
            if( pTreeEntry == nullptr ) {
                pTreeEntry = m_mappShortcutTreeEntries.value(strPathTmp, nullptr);
            } else {
                strPathRemaining += m_strNodeSeparator + strSection;
            }
        }
        if( pTreeEntry != nullptr )
        {
            strKeyInTree = strEntryType + ":" + pTreeEntry->path() + strPathRemaining;
            pTreeEntry = m_mappTreeEntries.value(strKeyInTree, nullptr);
        }
    }

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(pTreeEntry == nullptr ? "null" : pTreeEntry->keyInTree());
    }

    return pTreeEntry;

} // findLeave

//------------------------------------------------------------------------------
/*! Returns the entry with the given key.

    @param i_strKeyInTree [in] Key of the entry. The key must contain the node
                               entry type (e.g. "L:ZS/Diagram/Pvt").

    @return Pointer to tree entry (nullptr if not found).
*/
CIdxTreeEntry* CIdxTree::findEntry( const QString& i_strKeyInTree ) const
//------------------------------------------------------------------------------
{
    // When the mutex creates trace output also this method should be traced.
    EMethodTraceDetailLevel eTrcDetailLevel = m_pMtx == nullptr ? EMethodTraceDetailLevel::None : m_pMtx->getMethodTraceDetailLevel();
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "findEntry",
        /* strMethodInArgs    */ "" );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = m_mappTreeEntries.value(i_strKeyInTree, nullptr);

    if( pTreeEntry == nullptr )
    {
        if( m_pRoot != nullptr && m_pRoot->keyInTree() == i_strKeyInTree )
        {
            pTreeEntry = m_pRoot;
        }
    }
    if( pTreeEntry == nullptr )
    {
        QString strEntryType =
            CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Leave, EEnumEntryAliasStrSymbol);
        QString strPath = i_strKeyInTree;

        if( i_strKeyInTree.startsWith(strEntryType + ":") )
        {
            strPath = i_strKeyInTree.mid(strEntryType.length() + 1);
        }
        pTreeEntry = m_mappShortcutTreeEntries.value(strPath, nullptr);
    }
    if( pTreeEntry == nullptr )
    {
        QString strEntryType =
            CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
        QString strPath = i_strKeyInTree;

        if( i_strKeyInTree.startsWith(strEntryType + ":") )
        {
            strPath = i_strKeyInTree.mid(strEntryType.length() + 1);
        }
        pTreeEntry = m_mappShortcutTreeEntries.value(strPath, nullptr);
    }
    return pTreeEntry;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

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

    @see CIdxTreeEntry::add
*/
SErrResultInfo CIdxTree::canAdd( CIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canAdd",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "add";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTargetBranch = m_pRoot;

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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
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
                                If nullptr is passed the entry will be added to the root.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be added.
            - EResultObjNotInList (Severity: Error) .. Target path not found.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same type and name below the target path.

    @see CIdxTreeEntry::add
*/
SErrResultInfo CIdxTree::canAdd( CIdxTreeEntry* i_pTreeEntry, CIdxTreeEntry* i_pTargetBranch ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canAdd",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "add";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    CMutexLocker mtxLocker(m_pMtx);

    if( i_pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTreeEntry == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( pTargetBranch == nullptr )
    {
        pTargetBranch = m_pRoot;
    }

    int idxInTargetBranch = pTargetBranch->indexOf(i_pTreeEntry);

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

        if( pTargetBranch != m_pRoot )
        {
            strTargetPath = pTargetBranch->path();
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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canAdd

/*==============================================================================
public: // instance methods
==============================================================================*/

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

    @see CIdxTreeEntry::add
*/
int CIdxTree::add( CIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "add",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTargetBranch = m_pRoot;

    if( !i_strTargetPath.isEmpty() )
    {
        pTargetBranch = findBranch(i_strTargetPath);

        if( pTargetBranch == nullptr )
        {
            #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
            QStringList strlstBranches = i_strTargetPath.split(m_strNodeSeparator, Qt::SkipEmptyParts);
            #else
            QStringList strlstBranches = i_strTargetPath.split(m_strNodeSeparator, QString::SkipEmptyParts);
            #endif

            // Please note that the name of the root entry is not included in the TargetPath of the tree entries.
            CIdxTreeEntry* pTargetBranchPrev = m_pRoot;

            QString strTargetPathPrev;

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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
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
                                If nullptr is passed the entry will be added to the root.

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CIdxTreeEntry::add
*/
int CIdxTree::add( CIdxTreeEntry* i_pTreeEntry, CIdxTreeEntry* i_pTargetBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "add",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    CIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( pTargetBranch == nullptr )
    {
        pTargetBranch = m_pRoot;
    }

    int idxInTree = -1;

    int idxInTargetBranch = pTargetBranch->addChild(i_pTreeEntry);

    if( idxInTargetBranch >= 0 )
    {
        QString strEntryType = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
        QString strTargetPath;
        QString strKeyInTree;

        strKeyInTree = strEntryType + ":";

        if( pTargetBranch != m_pRoot )
        {
            strTargetPath = pTargetBranch->path();
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

        emit_treeEntryAdded(strKeyInTree);

    } // if( idxInTargetBranch >= 0 )

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // add

/*==============================================================================
public: // instance methods
==============================================================================*/

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

    @see CIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canInsert(
    CIdxTreeEntry* i_pTreeEntry,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch,
    int            i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canInsert",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "insert";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    CMutexLocker mtxLocker(m_pMtx);

    if( i_pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTreeEntry == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CIdxTreeEntry* pTargetBranch = m_pRoot;

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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
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
                                If nullptr is passed the entry will be added to the root.
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

    @see CIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canInsert(
    CIdxTreeEntry* i_pTreeEntry,
    CIdxTreeEntry* i_pTargetBranch,
    int            i_idxInTargetBranch,
    int            i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canInsert",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "insert";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    if( i_pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTreeEntry == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( pTargetBranch == nullptr )
    {
        pTargetBranch = m_pRoot;
    }

    CMutexLocker mtxLocker(m_pMtx);

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
        int idxInTargetBranch = pTargetBranch->indexOf(i_pTreeEntry);

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

            if( pTargetBranch != m_pRoot )
            {
                strTargetPath = pTargetBranch->path();
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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canInsert

/*==============================================================================
public: // instance methods
==============================================================================*/

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

    @see CIdxTreeEntry::insert
*/
int CIdxTree::insert(
    CIdxTreeEntry* i_pTreeEntry,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insert",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTargetBranch = m_pRoot;

    if( !i_strTargetPath.isEmpty() )
    {
        pTargetBranch = findBranch(i_strTargetPath);

        if( pTargetBranch == nullptr )
        {
            #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
            QStringList strlstBranches = i_strTargetPath.split(m_strNodeSeparator, Qt::SkipEmptyParts);
            #else
            QStringList strlstBranches = i_strTargetPath.split(m_strNodeSeparator, QString::SkipEmptyParts);
            #endif

            // Please note that the name of the root entry is not included in the TargetPath of the tree entries.
            CIdxTreeEntry* pTargetBranchPrev = m_pRoot;
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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
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
                                If nullptr is passed the entry will be added to the root.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entry.

    @note Throws a critical exception
          - with Result = EResultObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.
          - with Result = EResultIdxOutOfRange if an entry already exists at the given tree index.

    @see CIdxTreeEntry::insert
*/
int CIdxTree::insert(
    CIdxTreeEntry* i_pTreeEntry,
    CIdxTreeEntry* i_pTargetBranch,
    int            i_idxInTargetBranch,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "insert",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    CIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( pTargetBranch == nullptr )
    {
        pTargetBranch = m_pRoot;
    }

    int idxInTree = -1;

    int idxInTargetBranch = -1;

    if( i_idxInTargetBranch >= 0 )
    {
        idxInTargetBranch = pTargetBranch->insertChild(i_idxInTargetBranch, i_pTreeEntry);
    }
    else
    {
        idxInTargetBranch = pTargetBranch->addChild(i_pTreeEntry);
    }

    if( idxInTargetBranch >= 0 )
    {
        QString strEntryType = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
        QString strTargetPath;
        QString strKeyInTree;

        strKeyInTree = strEntryType + ":";

        if( pTargetBranch != m_pRoot )
        {
            strTargetPath = pTargetBranch->path();
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
                memset(&m_arpTreeEntries.data()[iSizePrev], 0x00, (iSizeNew-iSizePrev) * sizeof(CIdxTreeEntry*));

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

        emit_treeEntryAdded(strKeyInTree);

    } // if( idxInTargetBranch >= 0 )

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        mthTracer.setMethodReturn(QString::number(idxInTree));
    }

    return idxInTree;

} // insert

/*==============================================================================
public: // instance methods
==============================================================================*/

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
SErrResultInfo CIdxTree::canRemove( CIdxTreeEntry* i_pTreeEntry ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    CMutexLocker mtxLocker(m_pMtx);

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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strKeyInTree;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canRemove",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "remove";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = findEntry(i_strKeyInTree);

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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canRemove

/*==============================================================================
public: // instance methods
==============================================================================*/

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

    @note The entry will not be destroyed. If the entry has childrens those
          childrens will also not be destroyed by this method.
          Destroying the childrens is the task of the dtor of the tree entry.
          The tree entries dtor will first destroy all children before removing
          itself from the tree.

    @note Throws a critical exception with Result
          - EResultObjNotInList if the entry does not belong to the tree.
          - EResultInvalidMethodCall (Severity: Error) .. The root entry cannot be removed.

    @see CIdxTreeEntry::remove( CIdxTreeEntry* i_pTreeEntry )
*/
void CIdxTree::remove( CIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "remove",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    CMutexLocker mtxLocker(m_pMtx);

    emit_treeEntryAboutToBeRemoved(i_pTreeEntry->keyInTree(), i_pTreeEntry->indexInTree());

    QString strKeyInTree = i_pTreeEntry->keyInTree();
    int     idxInTree    = i_pTreeEntry->indexInTree();

    if( i_pTreeEntry->isRoot() || i_pTreeEntry->isBranch() )
    {
        // dynamic_cast may not work if the remove method has been called by
        // the dtor of the branch entry. But if the dynamic_cast returns nullptr
        // we can be sure that all children of the branch have already been
        // destroyed and removed from the tree.
        CIdxTreeEntry* pBranch = i_pTreeEntry;

        if( pBranch != nullptr && pBranch->count() > 0 )
        {
            for( int idxEntry = pBranch->count()-1; idxEntry >= 0; --idxEntry )
            {
                CIdxTreeEntry* pTreeEntry = pBranch->at(idxEntry);
                remove(pTreeEntry);
            }
        }
    }

    if( i_pTreeEntry == m_pRoot )
    {
        m_pRoot = nullptr;
        i_pTreeEntry->setTree(nullptr);
    }
    else
    {
        if( !m_mappTreeEntries.contains(strKeyInTree) )
        {
            throw CException(__FILE__, __LINE__, EResultObjNotInList, strKeyInTree);
        }

        m_mappTreeEntries.remove(strKeyInTree);

        for( const QString& strShortcut : i_pTreeEntry->shortcuts() )
        {
            if( m_mappShortcutTreeEntries.contains(strShortcut) )
            {
                m_mappShortcutTreeEntries.remove(strShortcut);
            }
            i_pTreeEntry->removeShortcut(strShortcut);
        }

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

        CIdxTreeEntry* pTargetBranch = i_pTreeEntry->parentBranch();

        pTargetBranch->removeChild(i_pTreeEntry);

    } // if( i_pTreeEntry != m_pRoot )

    emit_treeEntryRemoved(i_pTreeEntry->keyInTree(), i_pTreeEntry->indexInTree());

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

    @see CIdxTreeEntry::remove( CIdxTreeEntry* i_pTreeEntry )
*/
void CIdxTree::remove( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strKeyInTree;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "remove",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = findEntry(i_strKeyInTree);

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

    @see CIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canMove(
    const QString& i_strSourcePath,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SourcePath: " + i_strSourcePath;
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canMove",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    QString strMth = "move";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    CIdxTreeEntry* pTreeEntry = findEntry(i_strSourcePath);

    if( pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(i_strSourcePath);
    }
    else // if( pTreeEntry != nullptr )
    {
        CIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canMove

//------------------------------------------------------------------------------
/*! Checks whether the given branch can be moved as a child to the given
    target branch with the given index.

    The entry may only be moved as a child to the target path if not already
    another entry with the same name and type exists below the target path.

    @param i_pTreeEntry [in] Pointer to tree entry to be moved.
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

    @see CIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canMove(
    CIdxTreeEntry* i_pTreeEntry,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canMove",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    QString strMth = "move";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    if( i_pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTreeEntry == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

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
            int idxInTargetBranch = pTargetBranch->indexOf(i_pTreeEntry->keyInParentBranch());

            if( idxInTargetBranch >= 0 )
            {
                QString strAddErrInfo;
                strAddErrInfo  = i_pTreeEntry->keyInParentBranch();
                strAddErrInfo += " already belongs to branch " + pTargetBranch->keyInTree();
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultObjAlreadyInList);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
        } // if( i_idxInTargetBranch < 0 || i_idxInTargetBranch <= pTargetBranch->count() )
    } // if( pTargetBranch != nullptr )

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
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
                                If nullptr is passed the entry will be added to the root.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @return Error result struct with additional information.
            - EResultSuccess .. The entry may be moved.
            - EResultIdxOutOfRange (Severity: Error) .. The given target index is out of range.
            - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
              a child with the same and type name below the target path.
            - EResultIdxOutOfRange (Severtiy: Error) .. There already exists
              an entry at the given index in the branch's child list.

    @see CIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canMove(
    CIdxTreeEntry* i_pTreeEntry,
    CIdxTreeEntry* i_pTargetBranch,
    int            i_idxInTargetBranch ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canMove",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "move";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    if( i_pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTreeEntry == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( pTargetBranch == nullptr )
    {
        pTargetBranch = m_pRoot;
    }

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = i_pTreeEntry;

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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
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

    @see CIdxTreeEntry::remove
    @see CIdxTreeEntry::insert
    @see CIdxTreeEntry::add
*/
void CIdxTree::move(
    const QString& i_strSourcePath,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SourcePath: " + i_strSourcePath;
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "move",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = findEntry(i_strSourcePath);

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strSourcePath);
    }

    CIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strTargetPath);
    }

    return move(pTreeEntry, pTargetBranch, i_idxInTargetBranch);

} // move

//------------------------------------------------------------------------------
/*! Moves the given branch to the given target branch by inserting the entry as a
    child of the target branch at the given index.

    All childs of the branch will be moved to the target branch.

    The following members of the given child entry will be modified:

    - The unique key of the entry and all its childs will be updated.

    @param i_pTreeEntry [in] Pointer to tree entry to be moved.
    @param i_strTargetPath [in] Path of the target branch the entry should be moved to.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @note The index of the entry in the trees vector of entries will not be changed.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CIdxTreeEntry::remove
    @see CIdxTreeEntry::insert
    @see CIdxTreeEntry::add
*/
void CIdxTree::move(
    CIdxTreeEntry* i_pTreeEntry,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Branch: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "move",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strTargetPath);
    }

    return move(i_pTreeEntry, pTargetBranch, i_idxInTargetBranch);

} // move

//------------------------------------------------------------------------------
/*! Moves the given tree entry to the given target branch by inserting the entry as a
    child of the target branch at the given index.

    If the entry is a branch with childs also a childs will be moved to the target branch.

    The following members of the given child entry will be modified:

    - The unique key of the entry and all its childs will be updated.

    @param i_pTreeEntry [in] Pointer to tree entry to be moved.
    @param i_pTargetBranch [in] Pointer to target branch the entry should be moved to.
                                If nullptr is passed the entry will be added to the root.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the child should be inserted.

    @note The index of the entry in the trees vector of entries will not be changed.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.

    @see CIdxTreeEntry::remove
    @see CIdxTreeEntry::insert
    @see CIdxTreeEntry::add
*/
void CIdxTree::move(
    CIdxTreeEntry* i_pTreeEntry,
    CIdxTreeEntry* i_pTargetBranch,
    int            i_idxInTargetBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "move",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    CIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( pTargetBranch == nullptr )
    {
        pTargetBranch = m_pRoot;
    }

    CMutexLocker mtxLocker(m_pMtx);

    QString strKeyInTreePrev = i_pTreeEntry->keyInTree();

    emit_treeEntryAboutToBeMoved(strKeyInTreePrev, pTargetBranch->keyInTree());

    CIdxTreeEntry* pParentBranch = i_pTreeEntry->parentBranch();

    pParentBranch->removeChild(i_pTreeEntry);

    int idxInParentBranch = -1;

    if( i_idxInTargetBranch >= 0 )
    {
        idxInParentBranch = pTargetBranch->insertChild(i_idxInTargetBranch, i_pTreeEntry);
    }
    else
    {
        idxInParentBranch = pTargetBranch->addChild(i_pTreeEntry);
    }

    if( idxInParentBranch >= 0 )
    {
        updateKeyInTree(i_pTreeEntry);
    }

    emit_treeEntryMoved(i_pTreeEntry->keyInTree(), strKeyInTreePrev, pTargetBranch->keyInTree());

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

    @see CIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canCopy(
    const QString& i_strSourcePath,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch,
    int            i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SourcePath: " + i_strSourcePath;
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canCopy",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    QString strMth = "copy";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    CIdxTreeEntry* pTreeEntry = findEntry(i_strSourcePath);

    if( pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultObjNotInList);
        errResultInfo.setAddErrInfoDscr(i_strSourcePath);
    }
    else // if( pTreeEntry != nullptr )
    {
        CIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
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

    @param i_pTreeEntry [in] Pointer to the source entry to be copied.
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

    @see CIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canCopy(
    CIdxTreeEntry* i_pTreeEntry,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch,
    int            i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canCopy",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    QString strMth = "copy";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    if( i_pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTreeEntry == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

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
            int idxInTargetBranch = pTargetBranch->indexOf(i_pTreeEntry->keyInParentBranch());

            if( idxInTargetBranch >= 0 )
            {
                QString strAddErrInfo;
                strAddErrInfo  = i_pTreeEntry->keyInParentBranch();
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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
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
                                If nullptr is passed the entry will be added to the root.
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

    @see CIdxTreeEntry::insert
*/
SErrResultInfo CIdxTree::canCopy(
    CIdxTreeEntry* i_pTreeEntry,
    CIdxTreeEntry* i_pTargetBranch,
    int            i_idxInTargetBranch,
    int            i_idxInTree ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canCopy",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "copy";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    if( i_pTreeEntry == nullptr )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_pTreeEntry == nullptr");
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( pTargetBranch == nullptr )
    {
        pTargetBranch = m_pRoot;
    }

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = i_pTreeEntry;

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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
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

    @see CIdxTreeEntry::insert
    @see CIdxTreeEntry::add
*/
int CIdxTree::copy(
    const QString& i_strSourcePath,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SourcePath: " + i_strSourcePath;
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "copy",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = findEntry(i_strSourcePath);

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strSourcePath);
    }

    CIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strTargetPath);
    }

    int idxInTree = copy(pTreeEntry, pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
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

    @param i_pTreeEntry [in] Pointer to tree entry to be copied.
    @param i_strTargetPath [in] Path of the target branch the entry should be copied to.
           The path may already start with the branch node type ("B:").
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the copy of the entry should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entries copy in the trees vector of entries.

    @see CIdxTreeEntry::insert
    @see CIdxTreeEntry::add
*/
int CIdxTree::copy(
    CIdxTreeEntry* i_pTreeEntry,
    const QString& i_strTargetPath,
    int            i_idxInTargetBranch,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetPath: " + i_strTargetPath;
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "copy",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTargetBranch = findBranch(i_strTargetPath);

    if( pTargetBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strTargetPath);
    }

    int idxInTree = copy(i_pTreeEntry, pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
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
                                If nullptr is passed the entry will be added to the root.
    @param i_idxInTargetBranch [in] Index in the list of the target branch's
           child entries where the copy of the entry should be inserted.
    @param i_idxInTree [in] If greater or equal to zero this argument defines
           the index in the list of tree entries where the entry should be inserted.

    @return Index of the tree entries copy in the trees vector of entries.

    @see CIdxTreeEntry::insert
    @see CIdxTreeEntry::add
*/
int CIdxTree::copy(
    CIdxTreeEntry* i_pTreeEntry,
    CIdxTreeEntry* i_pTargetBranch,
    int            i_idxInTargetBranch,
    int            i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", TargetBranch: " + QString(i_pTargetBranch == nullptr ? "nullptr" : i_pTargetBranch->keyInTree());
        strMthInArgs += ", IdxInTargetBranch: " + QString::number(i_idxInTargetBranch);
        strMthInArgs += ", IdxInTree: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "copy",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    CIdxTreeEntry* pTargetBranch = i_pTargetBranch;

    if( pTargetBranch == nullptr )
    {
        pTargetBranch = m_pRoot;
    }

    CMutexLocker mtxLocker(m_pMtx);

    QString strName = i_pTreeEntry->name();

    if( pTargetBranch->indexOf(i_pTreeEntry->entryTypeSymbol(), strName) >= 0 )
    {
        QString strUniqueName = strName + " Copy";
        int     iCopies = 1;

        while( true )
        {
            if( pTargetBranch->indexOf(i_pTreeEntry->entryTypeSymbol(), strUniqueName) >= 0 )
            {
                strUniqueName = strName + " Copy " + QString::number(++iCopies);
            }
            else
            {
                break;
            }
        }
        strName = strUniqueName;
    }

    CIdxTreeEntry* pTreeEntryTrg = nullptr;

    if( i_pTreeEntry->isBranch() ) {
        pTreeEntryTrg = createBranch(strName);
    }
    else if( i_pTreeEntry->isLeave() ) {
        pTreeEntryTrg = createLeave(strName);
    }

    int idxInTree = insert(pTreeEntryTrg, pTargetBranch, i_idxInTargetBranch, i_idxInTree);

    if( i_pTreeEntry->isBranch() )
    {
        CIdxTreeEntry* pBranch = i_pTreeEntry;
        CIdxTreeEntry* pTargetBranch = pTreeEntryTrg;

        for( int idxEntry = 0; idxEntry < pBranch->count(); ++idxEntry )
        {
            CIdxTreeEntry* pTreeEntry = pBranch->at(idxEntry);

            copy(pTreeEntry, pTargetBranch);
        }
    }

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
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

    @see CIdxTreeEntry::rename
    @see CIdxTreeEntry::rename
*/
SErrResultInfo CIdxTree::canRename( const QString& i_strSourcePath, const QString& i_strNameNew ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SourcePath: " + i_strSourcePath;
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canRename",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    QString strMth = "rename";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    CIdxTreeEntry* pTreeEntry = findEntry(i_strSourcePath);

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

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }
    return errResultInfo;

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

    @see CIdxTreeEntry::rename
    @see CIdxTreeEntry::rename
*/
SErrResultInfo CIdxTree::canRename( CIdxTreeEntry* i_pTreeEntry, const QString& i_strNameNew ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canRename",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    CMutexLocker mtxLocker(m_pMtx);

    QString strMth = "rename";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    CIdxTreeEntry* pParentBranch = i_pTreeEntry->parentBranch();

    if( pParentBranch != nullptr )
    {
        int idxInParentBranch =
            pParentBranch->indexOf(i_pTreeEntry->entryTypeSymbol(), i_strNameNew);

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

    @see CIdxTreeEntry::rename
    @see CIdxTreeEntry::rename
*/
void CIdxTree::rename( const QString& i_strSourcePath, const QString& i_strNameNew )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "SourcePath: " + i_strSourcePath;
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "rename",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    CIdxTreeEntry* pTreeEntry = findEntry(i_strSourcePath);

    if( pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strSourcePath);
    }

    return rename(pTreeEntry, i_strNameNew);

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

    @see CIdxTreeEntry::rename
    @see CIdxTreeEntry::rename
*/
void CIdxTree::rename( CIdxTreeEntry* i_pTreeEntry, const QString& i_strNameNew )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "rename",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    CMutexLocker mtxLocker(m_pMtx);

    QString strKeyInTreePrev = i_pTreeEntry->keyInTree();
    QString strNamePrev = i_pTreeEntry->name();

    emit_treeEntryAboutToBeRenamed(strKeyInTreePrev, i_strNameNew);

    CIdxTreeEntry* pParentBranch = i_pTreeEntry->parentBranch();

    pParentBranch->renameChild(i_pTreeEntry, i_strNameNew);

    updateKeyInTree(i_pTreeEntry);

    emit_treeEntryRenamed(i_pTreeEntry->keyInTree(), strKeyInTreePrev, strNamePrev);

} // rename

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Checks whether the given unique name can be added to the map of shourtcuts.

    If the given shortcut name already exists the method returns en error result.

    @param i_strUniqueName [in] Unique shortcut key.

    @return Error result struct with additional information.
        - EResultSuccess .. The entry may be added.
        - EResultObjAlreadyInList (Severtiy: Error) .. There already exists
            an entry with the given shortcut.
*/
SErrResultInfo CIdxTree::canAddShortcut( const QString& i_strUniqueName ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strUniqueName;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "canAddShortcut",
        /* strMethodInArgs    */ strMthInArgs );

    QString strMth = "addShortcut";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    CMutexLocker mtxLocker(m_pMtx);

    if( i_strUniqueName.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_strUniqueName.isEmpty()");
    }
    else
    {
        if( m_mappShortcutTreeEntries.contains(i_strUniqueName) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr(
                "A shortcut with " + i_strUniqueName + " is already existing.");
        }
    }

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // canAddShortcut

//------------------------------------------------------------------------------
/*! Adds the given tree entry to the map with shortcuts.

    The shourtcut name must be unique.
    If the given shortcut name already exists the shortcut is refused and the
    method returns en error result.

    @param i_pTreeEntry [in] Pointer to tree entry to be added to the map with shortcuts.
    @param i_strUniqueName [in] Unique shortcut key.

    @return throws an exception if the shortcuts unique name is already existing.
*/
void CIdxTree::addShortcut( CIdxTreeEntry* i_pTreeEntry, const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->name());
        strMthInArgs += ", UniqueName: " + i_strUniqueName;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "addShortcut",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    QString strMth = "addShortcut";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    if( i_strUniqueName.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_strUniqueName.isEmpty()");
    }
    else
    {
        if( m_mappShortcutTreeEntries.contains(i_strUniqueName) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjAlreadyInList);
            errResultInfo.setAddErrInfoDscr(
                "A shortcut with " + i_strUniqueName + " is already existing.");
        }
        else
        {
            m_mappShortcutTreeEntries[i_strUniqueName] = i_pTreeEntry;
            i_pTreeEntry->addShortcut(i_strUniqueName);
        }
    }

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    if( errResultInfo.isErrorResult() )
    {
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

} // addShortcut

//------------------------------------------------------------------------------
/*! Adds the given tree entry to the map with shortcuts.

    The shourtcut name must be unique.
    If the given shortcut name already exists the shortcut is refused and the
    method returns en error result.

    @param i_pTreeEntry [in] Pointer to tree entry to be added to the map with shortcuts.
    @param i_strUniqueName [in] Unique shortcut key.

    @return true, if the shortcut key was successfully added, false otherwise.
*/
SErrResultInfo CIdxTree::removeShortcut( const QString& i_strUniqueName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strUniqueName;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "removeShortcut",
        /* strMethodInArgs    */ strMthInArgs );

    CMutexLocker mtxLocker(m_pMtx);

    QString strMth = "removeShortcut";

    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), strMth);

    if( i_strUniqueName.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultArgOutOfRange);
        errResultInfo.setAddErrInfoDscr("i_strUniqueName.isEmpty()");
    }
    else
    {
        if( !m_mappShortcutTreeEntries.contains(i_strUniqueName) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultObjNotInList);
            errResultInfo.setAddErrInfoDscr(
                "A shortcut with " + i_strUniqueName + " is not existing.");
        }
        else
        {
            CIdxTreeEntry* pTreeEntry = m_mappShortcutTreeEntries[i_strUniqueName];
            pTreeEntry->removeShortcut(i_strUniqueName);
            m_mappShortcutTreeEntries.remove(i_strUniqueName);
        }
    }

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        int iErrResultInfoDetailLevel = 0;
        if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsVerbose ) iErrResultInfoDetailLevel = 2;
        else if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsDetailed ) iErrResultInfoDetailLevel = 1;
        mthTracer.setMethodReturn(errResultInfo.toString(iErrResultInfoDetailLevel));
    }

    return errResultInfo;

} // removeShortcut

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Auxiliary method to update the key of the given tree entry in tree's the map of entries.

    If the given entry is a branch the keys of all childs will also be updated.

    @param i_pTreeEntry [in] Pointer to entry whose key has to be updated.
*/
void CIdxTree::updateKeyInTree( CIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
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

    CIdxTreeEntry* pParentBranch = i_pTreeEntry->parentBranch();

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

    emit_treeEntryKeyInTreeChanged(strKeyInTree, strKeyInTreePrev);

    if( i_pTreeEntry->isBranch() )
    {
        CIdxTreeEntry* pBranch = i_pTreeEntry;

        for( int idxEntry = 0; idxEntry < pBranch->count(); ++idxEntry )
        {
            CIdxTreeEntry* pTreeEntry = pBranch->at(idxEntry);

            updateKeyInTree(pTreeEntry);
        }
    }
} // updateKeyInTree

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Auxiliary method to delete all childs of the given branch.

    The given branch will neither be removed from the tree nor deleted.

    @param i_pBranch [in] Pointer to branch to be removed and (optionally) deleted.
*/
void CIdxTree::clear( CIdxTreeEntry* i_pBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Branch: " + QString(i_pBranch == nullptr ? "nullptr" : i_pBranch->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "clear",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pBranch == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    }

    for( int idxEntry = i_pBranch->count()-1; idxEntry >= 0; --idxEntry )
    {
        CIdxTreeEntry* pTreeEntry = i_pBranch->at(idxEntry);

        if( pTreeEntry->isBranch() )
        {
            clear(pTreeEntry);
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
    // When the mutex creates trace output also this method should be traced.
    EMethodTraceDetailLevel eTrcDetailLevel = m_pMtx == nullptr ? EMethodTraceDetailLevel::None : m_pMtx->getMethodTraceDetailLevel();
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "begin",
        /* strMethodInArgs    */ "" );

    CMutexLocker mtxLocker(m_pMtx);

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
}

//------------------------------------------------------------------------------
/*! Returns an iterator pointing to the imaginary last item of the tree.

    @return Iterator pointing to the imaginary last item in the tree.
*/
CIdxTree::iterator CIdxTree::end()
//------------------------------------------------------------------------------
{
    // When the mutex creates trace output also this method should be traced.
    EMethodTraceDetailLevel eTrcDetailLevel = m_pMtx == nullptr ? EMethodTraceDetailLevel::None : m_pMtx->getMethodTraceDetailLevel();
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "end",
        /* strMethodInArgs    */ "" );

    CMutexLocker mtxLocker(m_pMtx);
    CIdxTree::iterator itIdxTree(this, iterator::ETraversalOrder::Undefined);
    itIdxTree.m_pTreeEntryCurr = nullptr;
    return itIdxTree;
}

/*==============================================================================
public: // overridable instance methods (used by friend class CIdxTreeEntry and its derivates to avoid that the tree entry base classes must inherit QObject to emit signals)
==============================================================================*/

//------------------------------------------------------------------------------
/*! Method to be called by the derived tree entry class to inform the index tree
    that a propery of the entry has been changed.

    The tree will emit the "treeEntryChanged" signal if this method is called.

    Using this method instead of connecting to a "entryChanged" signal of the
    tree entries reduces the number of signal/slot connections and avoids that
    the tree entry base class must be derived from QObject.
    */
void CIdxTree::onTreeEntryChanged( CIdxTreeEntry* i_pTreeEntry )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "TreeEntry: " + QString(i_pTreeEntry == nullptr ? "nullptr" : i_pTreeEntry->keyInTree());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTreeEntryChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit_treeEntryChanged(i_pTreeEntry->keyInTree());
}

/*==============================================================================
protected: // instance methods (tracing of signals)
==============================================================================*/

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to created method trace outputs
    whenever the index tree emits the "treeEntryAdded" signal.

    @param i_strKeyInTree [in] Key in index tree of tree entry which has been added.
*/
void CIdxTree::emit_treeEntryAdded( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strKeyInTree;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryAdded",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryAdded(i_strKeyInTree);
}

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryChanged" signal.

    @param i_strKeyInTree [in] Unique key of the entry which has been changed.
*/
void CIdxTree::emit_treeEntryChanged( const QString& i_strKeyInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = i_strKeyInTree;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryChanged(i_strKeyInTree);
}

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryAboutToBeRemoved" signal.

    @param i_strKeyInTree [in] Unique key of the entry which will be removed from the tree.
    @param i_idxInTree [in] Index of the entry which will be removed from the tree.
*/
void CIdxTree::emit_treeEntryAboutToBeRemoved(
    const QString& i_strKeyInTree,
    int i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Key: " + i_strKeyInTree;
        strMthInArgs += ", Idx: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryAboutToBeRemoved",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryAboutToBeRemoved(i_strKeyInTree, i_idxInTree);
}

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryRemoved" signal.

    @param i_strKeyInTree [in] Unique key of the entry valid before the entry was removed from the tree.
    @param i_idxInTree [in] Index of the entry valid before the entry was removed from the tree.
*/
void CIdxTree::emit_treeEntryRemoved( const QString& i_strKeyInTree, int i_idxInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Key: " + i_strKeyInTree;
        strMthInArgs += ", Idx: " + QString::number(i_idxInTree);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryRemoved",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryRemoved(i_strKeyInTree, i_idxInTree);
}

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryAboutToBeMoved" signal.

    @param i_strOrigKeyInTree [in] Unique key of the entry which is going to be moved to another parent branch.
    @param i_strKeyInTreeOfTargetBranch [in] Unique Key of the target branch to which the entry will be moved.
*/
void CIdxTree::emit_treeEntryAboutToBeMoved(
    const QString& i_strOrigKeyInTree,
    const QString& i_strKeyInTreeOfTargetBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "OrigKey: " + i_strOrigKeyInTree;
        strMthInArgs += ", TargetBranch: " + i_strKeyInTreeOfTargetBranch;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryAboutToBeMoved",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryAboutToBeMoved(i_strOrigKeyInTree, i_strKeyInTreeOfTargetBranch);
}

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryeMoved" signal.

    @param i_strNewKeyInTree [in] The new unique key of the entry after moving the entry to the new branch.
    @param i_strOrigKeyInTree [in] The original unique key of the entry before moving the entry to the new branch.
    @param i_strKeyInTreeOfTargetBranch [in] Unique Key of the target branch to which the entry has been moved.
    @note The index of the entry in the index tree remains the same.
*/
void CIdxTree::emit_treeEntryMoved(
    const QString& i_strNewKeyInTree,
    const QString& i_strOrigKeyInTree,
    const QString& i_strKeyInTreeOfTargetBranch )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "NewKey: " + i_strNewKeyInTree;
        strMthInArgs += ", OrigKey: " + i_strOrigKeyInTree;
        strMthInArgs += ", TargetBranch: " + i_strKeyInTreeOfTargetBranch;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryMoved",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryMoved(i_strNewKeyInTree, i_strOrigKeyInTree, i_strKeyInTreeOfTargetBranch);
}

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryAboutToBeRenamed" signal.

    @param i_strOrigKeyInTree [in] Unique key of the entry which is going to be renamed.
    @param i_strNewName [in] New name of the entry.
*/
void CIdxTree::emit_treeEntryAboutToBeRenamed(
    const QString& i_strOrigKeyInTree,
    const QString& i_strNameNew )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "OrigKey: " + i_strOrigKeyInTree;
        strMthInArgs += ", NameNew: " + i_strNameNew;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryAboutToBeRenamed",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryAboutToBeRenamed(i_strOrigKeyInTree, i_strNameNew);
}

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryRenamed" signal.

    @param i_strNewKeyInTree [in] The new unique key of the entry after renaming it.
    @param i_strOrigKeyInTree [in] The original unique key of the entry before renaming it.
    @param i_strOrigName [in] Old name of the tree entry.
*/
void CIdxTree::emit_treeEntryRenamed(
    const QString& i_strNewKeyInTree,
    const QString& i_strOrigKeyInTree,
    const QString& i_strOrigName )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "NewKey: " + i_strNewKeyInTree;
        strMthInArgs += ", OrigKeyInTree: " + i_strOrigKeyInTree;
        strMthInArgs += ", OrigName: " + i_strOrigName;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryRenamed",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryRenamed(i_strNewKeyInTree, i_strOrigKeyInTree, i_strOrigName);
}

//------------------------------------------------------------------------------
/*! Auxiliary method which has been invented to create method trace outputs
    whenever the index tree emits the "treeEntryKeyInTreeChanged" signal.

    @param i_strNewKeyInTree [in] The new unique key of the entry.
    @param i_strOrigKeyInTree [in] The original unique key of the entry valid before the entry was moved to the target branch.
    @note The index of the entry in the index tree remains the same.
*/
void CIdxTree::emit_treeEntryKeyInTreeChanged(
    const QString& i_strNewKeyInTree,
    const QString& i_strOrigKeyInTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcMthFileDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "NewKey: " + i_strNewKeyInTree;
        strMthInArgs += ", OrigKeyInTree: " + i_strOrigKeyInTree;
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcMthFile        */ m_pTrcMthFile,
        /* eTrcDetailLevel    */ m_eTrcMthFileDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "emit_treeEntryKeyInTreeChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit treeEntryKeyInTreeChanged(i_strNewKeyInTree, i_strOrigKeyInTree);
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
/*! Slot connected to the changed signal of the trace admin object.

    The trace server uses the index tree to host the trace admin objects.
    If the tree's parent is the trace server the detail level of trace outputs
    for the index tree cannot be controlled by trace admin objects.
    To allow modifying the trace detail also if the trace server hosts the
    index tree the member m_eTrcMthFileDetailLevel may be directly set. This member
    is also used if the index tree does not belong to the trace server. But
    for this if the trace admin objects detail level is changed the detail level
    got to be forwarded to the member.

    @param i_pTrcAdminObj [in] Pointer to trace admin object.
*/
void CIdxTree::onTrcAdminObjChanged( QObject* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(i_pTrcAdminObj);

    if( pTrcAdminObj != nullptr && m_pTrcAdminObj == pTrcAdminObj )
    {
        m_eTrcMthFileDetailLevel = pTrcAdminObj->getMethodCallsTraceDetailLevel();
    }
}
