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

#include "ZSSys/ZSSysIdxTreeEntry.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/******************************************************************************/
class CInitModuleSysIdxTreeEntry
/* Please note:
   The class name should be unique for the whole system. Otherwise the compiler
   may be confused and using a CInitModule class from other modules to create
   the static InitModule instance.
*******************************************************************************/
{
public: // ctor
    CInitModuleSysIdxTreeEntry()
    {
    }
};

static CInitModuleSysIdxTreeEntry s_initModule;

/*******************************************************************************
Type definitions and constants
*******************************************************************************/

/* enum class EIdxTreeEntryType
==============================================================================*/

static const SEnumEntry s_arEnumStrIdxTreeEntryTypes[] = {
    /*  0 */ SEnumEntry( static_cast<int>(CIdxTreeEntry::EEntryType::Root),      "Root",      "R" ),
    /*  1 */ SEnumEntry( static_cast<int>(CIdxTreeEntry::EEntryType::Branch),    "Branch",    "B" ),
    /*  2 */ SEnumEntry( static_cast<int>(CIdxTreeEntry::EEntryType::Leave),     "Leave",     "L" ),
    /*  3 */ SEnumEntry( static_cast<int>(CIdxTreeEntry::EEntryType::Count),     "Count",     "*" ),
    /*  4 */ SEnumEntry( static_cast<int>(CIdxTreeEntry::EEntryType::Undefined), "Undefined", "?" )
};


/******************************************************************************
class CIdxTreeEntry
******************************************************************************/

/*=============================================================================
public: // class methods
=============================================================================*/

//-----------------------------------------------------------------------------
/*! @brief Converts the base entry type into its string representation.
*/
QString CIdxTreeEntry::entryType2Str( EEntryType i_entryType, int i_alias )
//-----------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(
        s_arEnumStrIdxTreeEntryTypes,
        _ZSArrLen(s_arEnumStrIdxTreeEntryTypes),
        static_cast<int>(i_entryType), i_alias);
}

//-----------------------------------------------------------------------------
/*! @brief Converts the string into the enumerator.
*/
CIdxTreeEntry::EEntryType CIdxTreeEntry::str2EntryType(const QString& i_strEntryType, int i_alias)
//-----------------------------------------------------------------------------
{
    EEntryType entryType = EEntryType::Undefined;
    int iEntryType = SEnumEntry::str2Enumerator(
        s_arEnumStrIdxTreeEntryTypes,
        _ZSArrLen(s_arEnumStrIdxTreeEntryTypes),
        i_strEntryType, i_alias);
    if (iEntryType >= 0 && iEntryType < static_cast<int>(EEntryType::Count)) {
        entryType = static_cast<EEntryType>(iEntryType);
    }
    return entryType;
}

/*=============================================================================
public: // ctors
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Constructs an index tree entry.

    After creation the entry has to be added to the index tree stating the
    desired target parent entry.

    @param i_entryType [in] Basic tree entry type.
        Either Leave, which may not contain child nodes or
        Branch, which may contain child.
    @param i_strName [in] Name of the entry.
*/
CIdxTreeEntry::CIdxTreeEntry( EEntryType i_entryType, const QString& i_strName ) :
//-----------------------------------------------------------------------------
    m_entryType(i_entryType),
    m_strName(i_strName),
    m_pMtx(nullptr),
    m_pTree(nullptr),
    m_strKeyInTree(),
    m_idxInTree(-1),
    m_strlstShortcuts(),
    m_pParentBranch(nullptr),
    m_strKeyInParentBranch(),
    m_idxInParentBranch(-1),
    m_bIsAboutToBeDestroyed(false),
    m_mappTreeEntries(),
    m_arpTreeEntries()
{
} // ctor

//-----------------------------------------------------------------------------
/*! Constructs a copy of i_other.

    After creation the entry has to be added to the index tree stating the
    desired target parent entry.

    @param i_other [in] Tree entry to be copied.

    @note The childs of i_other will not be copied.
*/
CIdxTreeEntry::CIdxTreeEntry( const CIdxTreeEntry& i_other ) :
//-----------------------------------------------------------------------------
    m_entryType(i_other.m_entryType),
    m_strName(i_other.m_strName),
    m_pMtx(nullptr),
    m_pTree(nullptr),
    m_strKeyInTree(),
    m_idxInTree(-1),
    m_strlstShortcuts(),
    m_pParentBranch(nullptr),
    m_strKeyInParentBranch(),
    m_idxInParentBranch(-1),
    m_bIsAboutToBeDestroyed(false),
    m_mappTreeEntries(),
    m_arpTreeEntries()
{
} // ctor

//-----------------------------------------------------------------------------
/*! Destroys the index tree entry.

    If the entry still belongs to the tree the entry will be removed from the
    tree and it's parent branch.

    If the entry is a branch and has children all childrens of the branch will
    also be destroyed and removed from the index tree (recursively).
*/
CIdxTreeEntry::~CIdxTreeEntry()
//-----------------------------------------------------------------------------
{
    if( m_pMtx != nullptr )
    {
        m_pMtx->lock();
    }

    m_bIsAboutToBeDestroyed = true;

    clear();

    m_entryType = static_cast<EEntryType>(0);
    //m_strName.clear();
    m_pMtx = nullptr;
    m_pTree = nullptr;
    //m_strKeyInTree.clear();
    m_idxInTree = 0;
    m_strlstShortcuts.clear();
    m_pParentBranch = nullptr;
    //m_strKeyInParentBranch.clear();
    m_idxInParentBranch = 0;
    m_bIsAboutToBeDestroyed = false;

} // dtor

/*=============================================================================
protected: // destructor
=============================================================================*/

//-----------------------------------------------------------------------------
/*! @brief Clears the tree entry by removing itself from the index tree.

    Also all children of this tree entry will be recursively removed and deleted.
*/
void CIdxTreeEntry::clear()
//-----------------------------------------------------------------------------
{
    if( m_arpTreeEntries.size() > 0 )
    {
        for( int idxEntry = m_arpTreeEntries.size() - 1; idxEntry >= 0; --idxEntry )
        {
            CIdxTreeEntry* pTreeEntry = m_arpTreeEntries[idxEntry];
            try {
                // The dtor of the childs will recursively call "clear"
                // until a child is reached which has no child.
                delete pTreeEntry;
            }
            catch(...) {
            }
            pTreeEntry = nullptr;
        }
    }

    // As "clear" has been called as reentry for the deleted children
    // the map and vector must already be empty.
    if( m_mappTreeEntries.size() > 0 || m_arpTreeEntries.size() > 0 )
    {
        if( CErrLog::GetInstance() != nullptr ) {
            SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
                nameSpace(), className(), keyInTree(), "dtor",
                EResultListNotEmpty, ZS::System::EResultSeverityCritical, "");
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
        m_mappTreeEntries.clear();
        m_arpTreeEntries.clear();
    }

    if( m_pTree != nullptr ) {
        m_pTree->remove(this);
    }
    m_pTree = nullptr;

    if( m_pParentBranch != nullptr ) {
        m_pParentBranch->removeChild(this);
    }
    m_pParentBranch = nullptr;

    try {
        delete m_pMtx;
    }
    catch(...)
    {
    }
    m_pMtx = nullptr;

} // clear

/*=============================================================================
public: // operators
=============================================================================*/

/*=============================================================================
public: // overridables of base class CIdxTreeEntry
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Creates a clone of the entry.

    @note Cloning a branch does not copy the childs of the branch but creates an empty branch.
*/
CIdxTreeEntry* CIdxTreeEntry::clone() const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return new CIdxTreeEntry(*this);
}

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Returns the symbol of the entry type as a string.

    @return String representation of the entry type (Range ["R", "B", "L"]).
*/
QString CIdxTreeEntry::entryTypeSymbol() const
//-----------------------------------------------------------------------------
{
    return entryType2Str(EEnumEntryAliasStrSymbol);
}

//-----------------------------------------------------------------------------
/*! Returns the string representation of the entry type.

    @param i_alias [in] Range [IdxName, Symbol]
        Selects one of the configured enumeration alias strings.

    @return String representation of the entry type depending on the desired alias.
*/
QString CIdxTreeEntry::entryType2Str( int i_alias ) const
//-----------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(
        s_arEnumStrIdxTreeEntryTypes,
        _ZSArrLen(s_arEnumStrIdxTreeEntryTypes),
        static_cast<int>(m_entryType), i_alias);
}

//-----------------------------------------------------------------------------
/*! Returns the path string of the entry within the index tree.

    The path ends with the name of the node.
    The path does not start with the type name and does not contain the name
    of the root entry.

    @return Path string (e.g. "ZS::Data::CDataTable::Customers").
*/
QString CIdxTreeEntry::path() const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strPath;

    if( m_pTree != nullptr )
    {
        CIdxTreeEntry* pParentBranch = m_pParentBranch;

        QString strNodeSeparator = m_pTree->nodeSeparator();

        while( pParentBranch != nullptr && pParentBranch != m_pTree->root() )
        {
            strPath.insert(0, pParentBranch->name() + strNodeSeparator);
            pParentBranch = pParentBranch->parentBranch();
        }
    } // if( m_pTree != nullptr )

    strPath += m_strName;

    return strPath;

} // path

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Returns the node separator.

    @return Node separater which has been set at the index tree.
*/
QString CIdxTreeEntry::nodeSeparator() const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    return m_pTree != nullptr ? m_pTree->nodeSeparator() : "::";
}

//-----------------------------------------------------------------------------
/*! Returns the unique key string of the entry within the index tree.

    The key does not include the name of the root entry.

    @return Unique key of the tree entry (e.g. "L:ZS::Data::CDataTable::Customers").
*/
QString CIdxTreeEntry::keyInTree() const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    QString strKeyInTree;
    if( isRoot() ) {
        // Just to have something to write into the log file.
        // The root does not really have a key in the tree.
        strKeyInTree = entryTypeSymbol() + ":" + m_strName;
    }
    else {
        strKeyInTree = m_strKeyInTree;
    }
    return strKeyInTree;
}

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Returns the name of the parent branch.

    If the parent branch is the root entry an empty string is returned.

    @return Name of parent branch (e.g. "CDataTable").
*/
QString CIdxTreeEntry::parentBranchName() const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    QString strName;
    if( m_pParentBranch != nullptr && !m_pParentBranch->isRoot() ) {
        strName = m_pParentBranch->name();
    }
    return strName;
}

//-----------------------------------------------------------------------------
/*! Returns the path of the parent branch of the tree entry.

    The path does not include entry type and not the name of the root entry.

    @return Path of the parent branch (e.g. "ZS::Data::CDataTable").
*/
QString CIdxTreeEntry::parentBranchPath() const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    QString strPath;
    if( m_pParentBranch != nullptr && !m_pParentBranch->isRoot() ) {
        strPath = m_pParentBranch->path();
    }
    return strPath;
}

//-----------------------------------------------------------------------------
/*! Returns the parent branch's key in the tree.

    @return Key of the parent branch (e.g. "B:ZS::Data::CDataTable").
*/
QString CIdxTreeEntry::parentBranchKeyInTree() const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    QString strKeyInTree;
    if( m_pParentBranch != nullptr ) {
        strKeyInTree = m_pParentBranch->keyInTree();
    }
    return strKeyInTree;
}

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Returns true if the entry is a child of the given parent entry.

    @param i_pBranch [in] Pointer to parent branch.

    @return true if the entry is a child of the parent entry - false otherwise.
*/
bool CIdxTreeEntry::isChildOf( CIdxTreeEntry* i_pBranch ) const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    bool bIs = false;

    if( i_pBranch != nullptr )
    {
        CIdxTreeEntry* pBranchParent = m_pParentBranch;

        while( pBranchParent != nullptr )
        {
            bIs = (pBranchParent == i_pBranch);

            if( bIs )
            {
                break;
            }
            pBranchParent = pBranchParent->parentBranch();
        }
    } // if( i_pBranch != nullptr )

    return bIs;

} // isChildOf

/*=============================================================================
public: // instance methods (applying filter)
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Returns the index of the tree entry in its parent branch considering only entries
    with the same entry type.

    @return Index of the entry in its parent branch.
*/
int CIdxTreeEntry::indexInParentBranchsChildListWithSameEntryTypes() const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idx = -1;

    // To be filtered by entry type
    if( m_pParentBranch != nullptr )
    {
        idx = m_pParentBranch->indexOfChildInListWithSameEntryTypes(this);
    }
    return idx;

} // indexInParentBranchsChildListWithSameEntryTypes

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
/*! The method calculates the unique key of the entry by iterating through the
    the tree upwards to the root entry.

    By adding the entry to the index tree the unique key in the tree will be
    calculated by the index tree using the key of the parent branch and will
    be set at the tree entry. This method may be used to check during runtime
    whether the unique key set at the tree entry is correct.

    @return Calculated unique key of the entry.
*/
QString CIdxTreeEntry::getCalculatedKeyInTree() const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strEntryType     = entryType2Str(EEnumEntryAliasStrSymbol);
    QString strNodeSeparator = m_pTree != nullptr ? m_pTree->nodeSeparator() : "/";
    QString strKeyInTree     = strEntryType + ":";
    QString strParentPath;

    CIdxTreeEntry* pParentBranch = parentBranch();

    CIdxTreeEntry* pRoot = m_pTree != nullptr ? m_pTree->root() : nullptr;

    if( pParentBranch != nullptr && pParentBranch != pRoot )
    {
        strParentPath = pParentBranch->path();
    }
    if( !strParentPath.isEmpty() )
    {
        strKeyInTree += strParentPath;

        if( !strKeyInTree.endsWith(strNodeSeparator) )
        {
            strKeyInTree += strNodeSeparator;
        }
    }

    strKeyInTree += name();

    return strKeyInTree;

} // getCalculatedKeyInTree

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Returns true if the destructor is active.

    If the destructor is active dynamic casts to derived classes will return nullptr.
    This is especially important if e.g. a branch is deleted and the destructor
    of the base class invokes "CIdxTree::remove" to remove the entry from the
    index tree wherupon the index tree invokes "CIdxTreeEntry::remove" to
    remove the entry from its parent branch. In this case the index tree and the
    parent branch may not cast the entry to the derived class to access the child
    list of the removed branch.

    @return True if the destructor is active - false otherwise.
*/
bool CIdxTreeEntry::isAboutToBeDestroyed() const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    return m_bIsAboutToBeDestroyed;

} // isAboutToBeDestroyed

/*=============================================================================
public: // instance methods (for branch entries)
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Returns the index of the given child tree entry in the branch.

    @param i_pChildTreeEntry [in] Pointer to child entry whose index is desired.

    @return Index of the given child entry in the branch.

    @note If the entry is added as a child to the branch the index in the parent
          branch is explicitely set at the child entry. This index is read from
          the given child entry. So you could also invoke "i_pTreeEntry->indexInParentBranch"
          to get the child entries index in the parent branch. This "indexOf" method
          additionally checks whether the index set at the child entry is correct
          throwing an "IdxOutOfRange" exception if this is not the case.

    @note Throws a critical exception
          - with Result = IdxOutOfRange if the child's index in the parent branch
                          is not correct.
*/
int CIdxTreeEntry::indexOf( const CIdxTreeEntry* i_pChildTreeEntry ) const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = i_pChildTreeEntry->indexInParentBranch();

    if( idxInParentBranch < 0 || idxInParentBranch >= m_arpTreeEntries.size() )
    {
        QString strAddErrInfo = i_pChildTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "indexOf",
            EResultIdxOutOfRange, ZS::System::EResultSeverityCritical, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CIdxTreeEntry* pTreeEntry = const_cast<CIdxTreeEntry*>(i_pChildTreeEntry);

    if( idxInParentBranch != m_arpTreeEntries.indexOf(pTreeEntry) )
    {
        QString strAddErrInfo = i_pChildTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "indexOf",
            EResultIdxOutOfRange, ZS::System::EResultSeverityCritical, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    return idxInParentBranch;

} // indexOf

//-----------------------------------------------------------------------------
/*! Returns the index of the given child tree entry in the branch.

    @param i_strKeyInParentBranch [in] Unique key of the child entry in the branch (e.g. "L:Customers").

    @return Index of the given child entry in the branch.

    @note If the entry is added as a child to the branch the index in the parent
          branch is explicitely set at the child entry. This index is read from
          the given child entry. So you could also invoke "i_pTreeEntry->indexInParentBranch"
          to get the child entries index in the parent branch. This "indexOf" method
          additionally checks whether the index set at the child entry is correct
          throwing an "IdxOutOfRange" exception if this is not the case.

    @note Throws a critical exception
          - with Result = IdxOutOfRange if the child's index in the parent branch
                          is not correct.
*/
int CIdxTreeEntry::indexOf( const QString& i_strKeyInParentBranch ) const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = -1;

    CIdxTreeEntry* pTreeEntry = m_mappTreeEntries.value(i_strKeyInParentBranch, nullptr);

    if( pTreeEntry != nullptr )
    {
        idxInParentBranch = pTreeEntry->indexInParentBranch();

        if( idxInParentBranch < 0 || idxInParentBranch >= m_arpTreeEntries.size() )
        {
            QString strAddErrInfo = pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
            SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
                nameSpace(), className(), keyInTree(), "indexOf",
                EResultIdxOutOfRange, ZS::System::EResultSeverityCritical, strAddErrInfo);
            throw CException(__FILE__, __LINE__, errResultInfo);
        }

        if( idxInParentBranch != m_arpTreeEntries.indexOf(pTreeEntry) )
        {
            QString strAddErrInfo = pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
            SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
                nameSpace(), className(), keyInTree(), "indexOf",
                EResultIdxOutOfRange, ZS::System::EResultSeverityCritical, strAddErrInfo);
            throw CException(__FILE__, __LINE__, errResultInfo);
        }
    } // if( pTreeEntry != nullptr )

    return idxInParentBranch;

} // indexOf

//-----------------------------------------------------------------------------
/*! Returns the index of the given child tree entry in the branch.

    @param i_strEntryTypeSymbol [in] Type of the child entry.
    @param i_strName [in] Name of the child entry in the branch (e.g. "Customers").

    @return Index of the given child entry in the branch.

    @note If the entry is added as a child to the branch the index in the parent
          branch is explicitely set at the child entry. This index is read from
          the given child entry. So you could also invoke "i_pTreeEntry->indexInParentBranch"
          to get the child entries index in the parent branch. This "indexOf" method
          additionally checks whether the index set at the child entry is correct
          throwing an "IdxOutOfRange" exception if this is not the case.

    @note Throws a critical exception
          - with Result = IdxOutOfRange if the child's index in the parent branch
                          is not correct.
*/
int CIdxTreeEntry::indexOf( const QString& i_strEntryTypeSymbol, const QString& i_strName ) const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = -1;

    QString strKeyInParentBranch = i_strEntryTypeSymbol + ":" + i_strName;
    CIdxTreeEntry* pTreeEntry = m_mappTreeEntries.value(strKeyInParentBranch, nullptr);

    if( pTreeEntry != nullptr )
    {
        idxInParentBranch = pTreeEntry->indexInParentBranch();

        if( idxInParentBranch < 0 || idxInParentBranch >= m_arpTreeEntries.size() )
        {
            QString strAddErrInfo = pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
            SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
                nameSpace(), className(), keyInTree(), "indexOf",
                EResultIdxOutOfRange, ZS::System::EResultSeverityCritical, strAddErrInfo);
            throw CException(__FILE__, __LINE__, errResultInfo);
        }

        if( idxInParentBranch != m_arpTreeEntries.indexOf(pTreeEntry) )
        {
            QString strAddErrInfo = pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
            SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
                nameSpace(), className(), keyInTree(), "indexOf",
                EResultIdxOutOfRange, ZS::System::EResultSeverityCritical, strAddErrInfo);
            throw CException(__FILE__, __LINE__, errResultInfo);
        }
    } // if( pTreeEntry != nullptr )

    return idxInParentBranch;

} // indexOf

//-----------------------------------------------------------------------------
/*! Returns the tree entry for the given key in parent branch.

    @param i_strKeyInParentBranch [in] Unique key of the child entry in the branch (e.g. "L:Customers").

    @return Pointer to child entry if an entry with the given key exists.
            nullptr otherwise.
*/
CIdxTreeEntry* CIdxTreeEntry::find( const QString& i_strKeyInParentBranch ) const
//-----------------------------------------------------------------------------
{
    return m_mappTreeEntries.value(i_strKeyInParentBranch, nullptr);
}

//-----------------------------------------------------------------------------
/*! Returns the tree entry for the given entry type and entry name.

    @param i_entryType [in] Type of the child entry.
    @param i_strName [in] Name of the child entry in the branch (e.g. "Customers").

    @return Pointer to child entry if an entry with the given type and name exists.
            nullptr otherwise.
*/
CIdxTreeEntry* CIdxTreeEntry::find( const QString& i_strEntryTypeSymbol, const QString& i_strName ) const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    QString strKeyInParentBranch = i_strEntryTypeSymbol + ":" + i_strName;
    return m_mappTreeEntries.value(strKeyInParentBranch, nullptr);
}

/*=============================================================================
public: // instance methods (for branch entries)
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Returns the index of the given child tree entry in the branch considering
    only entries with the same entry type of the given child entry.

    @param i_pChildTreeEntry [in] Pointer to child entry whose index is desired.

    @return Index of the given child entry in the branch.
*/
int CIdxTreeEntry::indexOfChildInListWithSameEntryTypes( const CIdxTreeEntry* i_pChildTreeEntry ) const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = -1;

    QString strEntryTypeFilter = i_pChildTreeEntry->entryTypeSymbol();

    CIdxTreeEntry* pTreeEntryTmp;
    int            idxTmp;

    for( idxTmp = 0; idxTmp < m_arpTreeEntries.size(); ++idxTmp )
    {
        pTreeEntryTmp = m_arpTreeEntries[idxTmp];

        if( pTreeEntryTmp->entryTypeSymbol() == strEntryTypeFilter )
        {
            ++idxInParentBranch;
        }
        if( pTreeEntryTmp == i_pChildTreeEntry )
        {
            break;
        }
    }

    return idxInParentBranch;

} // indexOfChildInListWithSameEntryTypes

/*=============================================================================
protected: // instance methods (for branch entries)
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Adds the given tree entry as a child of the branch.

    This method is for internal use only and is called by the index tree.

    The entry will be appended at the end of the list of child entries.
    A reference to the child entry will be inserted in the map of the child entries
    with a unique key build of the entry types symbol followed by the node separator
    and the name of the child entry.

    The following members of the given child entry will be modified:

    - The branch will be set as the parent branch of the given child entry.
    - The unique key of the child entry within the branch will be set at the child entry.
    - The index of the child entry in the branch will be set at the child entry.

    @param i_pChildTreeEntry [in] Pointer to tree entry to be added as a child.

    @return Index of the child entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.
*/
int CIdxTreeEntry::addChild( CIdxTreeEntry* i_pChildTreeEntry )
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = -1;

    QString strEntryType = i_pChildTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranch = strEntryType + ":" + i_pChildTreeEntry->name();

    if( i_pChildTreeEntry->parentBranch() != nullptr )
    {
        QString strAddErrInfo = i_pChildTreeEntry->keyInTree() + " already belongs to " + i_pChildTreeEntry->parentBranch()->keyInTree();
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "addChild",
            EResultObjAlreadyInList, ZS::System::EResultSeverityCritical, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( m_mappTreeEntries.contains(strKeyInParentBranch) )
    {
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "addChild",
            EResultObjAlreadyInList, ZS::System::EResultSeverityCritical, strKeyInParentBranch);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    idxInParentBranch = m_arpTreeEntries.size();

    m_arpTreeEntries.append(i_pChildTreeEntry);
    m_mappTreeEntries.insert(strKeyInParentBranch, i_pChildTreeEntry);

    i_pChildTreeEntry->setParentBranch(this);
    i_pChildTreeEntry->setKeyInParentBranch(strKeyInParentBranch);
    i_pChildTreeEntry->setIndexInParentBranch(idxInParentBranch);

    return idxInParentBranch;

} // addChild

//-----------------------------------------------------------------------------
/*! Inserts the given tree entry at the given index as a child of the branch.

    This method is for internal use only and is called by the index tree.

    If the given index is lower than 0 or greater than the number of entries in the
    list of child entries the entry will be appended at the end of the list.
    A reference to the child entry will be inserted in the map of the child entries
    with a unique key build of the entry types symbol followed by the node separator
    and the name of the child entry.

    The following members of the given child entry will be modified:

    - The branch will be set as the parent branch of the given child entry.
    - The unique key of the child entry within the branch will be set at the child entry.
    - The index of the child entry in the branch will be set at the child entry.

    @param i_iIdx [in] Index in the list of child entries where the child should be inserted.
    @param i_pChildTreeEntry [in] Pointer to tree entry to be inserted as a child.

    @return Index of the child entry. Might be different from the given index if the index
            was less than 0 or greater the current size of the child list.

    @note If the child entry is inserted but not appended at the end of the list
          the index of all following child entries will be corrected.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.
*/
int CIdxTreeEntry::insertChild( int i_iIdx, CIdxTreeEntry* i_pChildTreeEntry )
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = -1;

    QString strEntryType = i_pChildTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranch = strEntryType + ":" + i_pChildTreeEntry->name();

    if( i_pChildTreeEntry->parentBranch() != nullptr )
    {
        QString strAddErrInfo = i_pChildTreeEntry->keyInTree() + " already belongs to " + i_pChildTreeEntry->parentBranch()->keyInTree();
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "insertChild",
            EResultObjAlreadyInList, ZS::System::EResultSeverityCritical, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( m_mappTreeEntries.contains(strKeyInParentBranch) )
    {
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "insertChild",
            EResultObjAlreadyInList, ZS::System::EResultSeverityCritical, strKeyInParentBranch);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_iIdx >= 0 && i_iIdx < m_arpTreeEntries.size() )
    {
        idxInParentBranch = i_iIdx;
        m_arpTreeEntries.insert(idxInParentBranch, i_pChildTreeEntry);

        CIdxTreeEntry* pTreeEntry;
        int            idxEntry;

        for( idxEntry = idxInParentBranch+1; idxEntry < m_arpTreeEntries.size(); ++idxEntry )
        {
            pTreeEntry = m_arpTreeEntries[idxEntry];
            pTreeEntry->setIndexInParentBranch(idxEntry);
        }
    }
    else
    {
        idxInParentBranch = m_arpTreeEntries.size();
        m_arpTreeEntries.append(i_pChildTreeEntry);
    }

    m_mappTreeEntries.insert(strKeyInParentBranch, i_pChildTreeEntry);

    i_pChildTreeEntry->setParentBranch(this);
    i_pChildTreeEntry->setKeyInParentBranch(strKeyInParentBranch);
    i_pChildTreeEntry->setIndexInParentBranch(idxInParentBranch);

    return idxInParentBranch;

} // insertChild

//-----------------------------------------------------------------------------
/*! Removes the given tree entry from the branch.

    This method is for internal use only and is called by the index tree.

    This method is either called when an entry is going about to be destroyed
    or if an entry is moved within the tree.

    The following members of the given child entry will be modified:

    - The parent branch of the given child entry will be set to nullptr.
    - The unique key of the child entry within the branch will be set to an empty string.
    - The index of the child entry in the branch will be set to -1.

    @param i_pChildTreeEntry [in] Pointer to tree entry to be removed.

    @note If the child entry is not the last entry in the list of child entries
          the index of all following child entries will be corrected.

    @note Throws a critical exception
          - with Result = IdxOutOfRange if the child's index in parent branch
                          does not correspond to the index as expected by the branch.
    @note Throws a critical exception
          - with Result = ObjNotInList if the tree entry is not a child of the branch.
*/
void CIdxTreeEntry::removeChild( CIdxTreeEntry* i_pChildTreeEntry )
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = i_pChildTreeEntry->indexInParentBranch();

    if( idxInParentBranch < 0 || idxInParentBranch >= m_arpTreeEntries.size() )
    {
        QString strAddErrInfo = i_pChildTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "removeChild",
            EResultIdxOutOfRange, ZS::System::EResultSeverityCritical, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( idxInParentBranch != m_arpTreeEntries.indexOf(i_pChildTreeEntry) )
    {
        QString strAddErrInfo = i_pChildTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "removeChild",
            EResultIdxOutOfRange, ZS::System::EResultSeverityCritical, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QString strKeyInParentBranch = i_pChildTreeEntry->keyInParentBranch();

    if( !m_mappTreeEntries.contains(strKeyInParentBranch) )
    {
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "removeChild",
            EResultObjNotInList, ZS::System::EResultSeverityCritical,
            i_pChildTreeEntry->keyInParentBranch());
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    m_arpTreeEntries.remove(idxInParentBranch);
    m_mappTreeEntries.remove(strKeyInParentBranch);

    i_pChildTreeEntry->setParentBranch(nullptr);
    i_pChildTreeEntry->setKeyInParentBranch("");
    i_pChildTreeEntry->setIndexInParentBranch(-1);

    CIdxTreeEntry* pTreeEntry;
    int            idxEntry;

    for( idxEntry = idxInParentBranch; idxEntry < m_arpTreeEntries.size(); ++idxEntry )
    {
        pTreeEntry = m_arpTreeEntries[idxEntry];
        pTreeEntry->setIndexInParentBranch(idxEntry);
    }

} // removeChild

//-----------------------------------------------------------------------------
/*! Renames the given child tree entry to the given new name.

    This method is for internal use only and is called by the index tree.

    The following members of the given child entry will be modified:

    - The unique key of the child entry within the branch will be modified.

    @param i_pChildTreeEntry [in] Pointer to child tree entry to be renamed.
    @param i_strNameNew [in] New name of the child entry.

    @note Throws a critical exception
          - with Result = IdxOutOfRange if the child's index in parent branch
                          does not correspond to the index as expected by the branch.
    @note Throws a critical exception
          - with Result = ObjNotInList if the tree entry is not a child of the branch.
*/
void CIdxTreeEntry::renameChild( CIdxTreeEntry* i_pChildTreeEntry, const QString& i_strNameNew )
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = i_pChildTreeEntry->indexInParentBranch();

    if( idxInParentBranch < 0 || idxInParentBranch >= m_arpTreeEntries.size() )
    {
        QString strAddErrInfo = i_pChildTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "renameChild",
            EResultIdxOutOfRange, ZS::System::EResultSeverityCritical, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( idxInParentBranch != m_arpTreeEntries.indexOf(i_pChildTreeEntry) )
    {
        QString strAddErrInfo = i_pChildTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "renameChild",
            EResultIdxOutOfRange, ZS::System::EResultSeverityCritical, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QString strKeyInParentBranchOrig = i_pChildTreeEntry->keyInParentBranch();

    if( !m_mappTreeEntries.contains(strKeyInParentBranchOrig) )
    {
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "renameChild",
            EResultObjNotInList, ZS::System::EResultSeverityCritical, strKeyInParentBranchOrig);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QString strEntryType = i_pChildTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranchNew = strEntryType + ":" + i_strNameNew;

    if( m_mappTreeEntries.contains(strKeyInParentBranchNew) )
    {
        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
            nameSpace(), className(), keyInTree(), "renameChild",
            EResultObjAlreadyInList, ZS::System::EResultSeverityCritical, strKeyInParentBranchNew);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    i_pChildTreeEntry->setName(i_strNameNew);

    m_mappTreeEntries.remove(strKeyInParentBranchOrig);
    m_mappTreeEntries.insert(strKeyInParentBranchNew, i_pChildTreeEntry);

    i_pChildTreeEntry->setKeyInParentBranch(strKeyInParentBranchNew);

} // renameChild

/*=============================================================================
protected: // overridables
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Sets the current name of the tree entry.

    This method is for internal use only and is called by the index tree
    if an entry is renamed.

    Changing the name also changes its key.
    If the name of a branch is changed also the keys of all children must
    be set correspondingly. This is handled by the index tree.

    @param i_strName [in] New name of the tree entry.
*/
//-----------------------------------------------------------------------------
void CIdxTreeEntry::setName( const QString& i_strName )
{
    m_strName = i_strName;
}

//-----------------------------------------------------------------------------
/*! Sets the key of the entry in the index tree.

    This method is for internal use only and is implicitely called by the
    index tree if the name or the parent of an entry is changed.

    @param i_strKey [in] New key of the tree entry.
*/
//-----------------------------------------------------------------------------
void CIdxTreeEntry::setKeyInTree( const QString& i_strKey )
{
    m_strKeyInTree = i_strKey;
}

//-----------------------------------------------------------------------------
/*! Sets the index of the entry in the index trees vector of entries.

    This method is for internal use only and is called by the index tree on
    adding new entries.

    @param i_idx [in] New index of the tree entry in the vector of the index tree.
*/
//-----------------------------------------------------------------------------
void CIdxTreeEntry::setIndexInTree( int i_idx )
{
    m_idxInTree = i_idx;
}

/*=============================================================================
protected: // overridables
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Sets the parent branch of the entry.

    This method is for internal use only and is called by the index tree on
    moving tree entries from one branch to another or on adding new entries.

    @param i_pParent [in] Pointer to new parent entry.
*/
//-----------------------------------------------------------------------------
void CIdxTreeEntry::setParentBranch( CIdxTreeEntry* i_pParent )
{
    m_pParentBranch = i_pParent;
}

//-----------------------------------------------------------------------------
/*! Sets the unique key of the entry within it's parent branch.

    This method is for internal use only and is called by the index tree on
    moving tree entries from one branch to another or on adding new entries.

    @param i_strKey [in] New key in parent branch.

*/
//-----------------------------------------------------------------------------
void CIdxTreeEntry::setKeyInParentBranch( const QString& i_strKey )
{
    m_strKeyInParentBranch = i_strKey;
}

//-----------------------------------------------------------------------------
/*! Sets the index of the entry within it's parent branches vector of entries.

    This method is for internal use only and is called by the index tree on
    moving tree entries from one branch to another or on adding new entries.

    @param i_idx [in] New index of the entry in the vector of its parent branch.
*/
//-----------------------------------------------------------------------------
void CIdxTreeEntry::setIndexInParentBranch( int i_idx )
{
    m_idxInParentBranch = i_idx;
}

/*=============================================================================
protected: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
/*! @brief Adds the given unique name to the list of shortcuts.

    Shortcuts to tree entries can only be added by invoking the
    "addShortcut" method of the index tree class.
    To remove the shortcuts again if the tree entry is removed from
    the index tree the shortcuts are saved in the tree entry.
    Only the index tree may invoke the "addShortcut" methof the
    tree entries.

    @param i_strUniqueName [in]
        Unique name as a shortcut in the index tree to this entry.
*/
void CIdxTreeEntry::addShortcut( const QString& i_strUniqueName )
//-----------------------------------------------------------------------------
{
    m_strlstShortcuts.append(i_strUniqueName);
}

//-----------------------------------------------------------------------------
/*! @brief Removes the given unique name to the list of shortcuts.

    Shortcuts to tree entries can only be removed by invoking the
    "removeShortcut" method of the index tree class or if tree entries
    are removed from the index tree.

    To remove the shortcuts again if the tree entry is removed from
    the index tree the shortcuts are saved in the tree entry.
    Only the index tree may invoke the "removeShortcut" method the
    tree entries.

    @param i_strUniqueName [in]
        Unique name as a shortcut in the index tree to this entry.
*/
void CIdxTreeEntry::removeShortcut( const QString& i_strUniqueName )
//-----------------------------------------------------------------------------
{
    m_strlstShortcuts.removeOne(i_strUniqueName);
}

//-----------------------------------------------------------------------------
/*! Sets the index tree the entry belongs to.

    This method is for internal use only and is called by the index tree on
    on adding new entries.

    @param i_pTree [in] Pointer to index tree the tree entry belongs to.
*/
//-----------------------------------------------------------------------------
void CIdxTreeEntry::setTree( CIdxTree* i_pTree )
{
    m_pTree = i_pTree;
}
