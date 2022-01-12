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

#include "ZSSys/ZSSysIdxTreeEntry.h"
#include "ZSSys/ZSSysIdxTree.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
Type definitions and constants
*******************************************************************************/

/* enum class EIdxTreeEntryType
==============================================================================*/

static const SEnumEntry s_arEnumStrIdxTreeEntryTypes[] = {           // IdxName,     Symbol
    /*  0 */ SEnumEntry( static_cast<int>(EIdxTreeEntryType::Root),      "Root",      "R" ),
    /*  1 */ SEnumEntry( static_cast<int>(EIdxTreeEntryType::Branch),    "Branch",    "B" ),
    /*  2 */ SEnumEntry( static_cast<int>(EIdxTreeEntryType::Leave),     "Leave",     "L" ),
    /*  3 */ SEnumEntry( static_cast<int>(EIdxTreeEntryType::Count),     "Count",     "*" ),
    /*  4 */ SEnumEntry( static_cast<int>(EIdxTreeEntryType::Undefined), "Undefined", "?" )
};

//------------------------------------------------------------------------------
/*! Converts the given tree entry type into a string.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @param i_eVal [in] Enumerator to be converted.
    @param i_alias [in] Range [IdxName, Symbol].
                        Selects one of the available strings.

    @return String representation of the given tree entry type.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range or
          - with Result = ArgOutOfRange if the alias is out of range.
*/
QString ZS::System::idxTreeEntryType2Str( EIdxTreeEntryType i_eVal, int i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrIdxTreeEntryTypes, _ZSArrLen(s_arEnumStrIdxTreeEntryTypes), static_cast<int>(i_eVal), i_alias);
}

//------------------------------------------------------------------------------
/*! Converts the given tree entry type into a string.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @param i_iVal [in] Enumerator to be converted.
    @param i_alias [in] Range [IdxName, Symbol].
                        Selects one of the available strings.

    @return String representation of the given tree entry type.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range or
          - with Result = ArgOutOfRange if the alias is out of range.
*/
QString ZS::System::idxTreeEntryType2Str( int i_iVal, int i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrIdxTreeEntryTypes, _ZSArrLen(s_arEnumStrIdxTreeEntryTypes), i_iVal, i_alias);
}

//------------------------------------------------------------------------------
/*! Converts the string into the corresponding tree entry type.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @param i_str [in] String to be converted into the enumerator.
    @param i_alias [in] Range [IdxName, Symbol].
                        Selects one of the available strings.
                        If Undefined is used the string will be compared with all possible strings.
                        If a valid index is used the passed string will only be compared with this alias string only.

    @return Enumerator corresponding to the given string. Undefined if the conversion failed.
*/
EIdxTreeEntryType ZS::System::str2IdxTreeEntryType( const QString& i_str, int i_alias )
//------------------------------------------------------------------------------
{
    EIdxTreeEntryType entryType = EIdxTreeEntryType::Undefined;

    int iVal = SEnumEntry::str2Enumerator(s_arEnumStrIdxTreeEntryTypes, _ZSArrLen(s_arEnumStrIdxTreeEntryTypes), i_str, i_alias, Qt::CaseInsensitive);

    if( iVal >= 0 && iVal < static_cast<int>(EIdxTreeEntryType::Count) )
    {
        entryType = static_cast<EIdxTreeEntryType>(iVal);
    }
    return entryType;
}


/******************************************************************************
class CIdxTreeEntry
******************************************************************************/

/*=============================================================================
public: // ctors
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Constructs an index tree entry.

    After creation the entry has to be added to the index tree stating the
    desired target parent entry.

    @param i_entryType [in] Tree entry type.
    @param i_strName [in] Name of the entry.
*/
CIdxTreeEntry::CIdxTreeEntry(
    EIdxTreeEntryType i_entryType,
    const QString&    i_strName ) :
//-----------------------------------------------------------------------------
    m_entryType(i_entryType),
    m_strName(i_strName),
    m_pMtx(nullptr),
    m_pTree(nullptr),
    m_strKeyInTree(),
    m_idxInTree(-1),
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
    m_pParentBranch(nullptr),
    m_strKeyInParentBranch(i_other.m_strKeyInParentBranch),
    m_idxInParentBranch(-1),
    m_bIsAboutToBeDestroyed(false),
    m_mappTreeEntries(),
    m_arpTreeEntries()
{
} // ctor

/*=============================================================================
public: // dtor
=============================================================================*/

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

    CIdxTreeEntry* pTreeEntry;
    int            idxEntry;

    if( m_arpTreeEntries.size() > 0 )
    {
        for( idxEntry = m_arpTreeEntries.size() - 1; idxEntry >= 0; --idxEntry )
        {
            pTreeEntry = m_arpTreeEntries[idxEntry];

            try
            {
                delete pTreeEntry; // calls "remove" as reentry
            }
            catch(...)
            {
            }
            pTreeEntry = nullptr;
        }
    }

    // As "remove" has been called as reentry for the deleted children
    // the map and vector must already be empty.
    if( m_mappTreeEntries.size() > 0 || m_arpTreeEntries.size() > 0 )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo = ErrResultInfoCritical("dtor", EResultListNotEmpty, "");
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    m_mappTreeEntries.clear();
    m_arpTreeEntries.clear();

    if( m_pTree != nullptr )
    {
        m_pTree->remove(this);
    }

    if( m_pParentBranch != nullptr )
    {
        m_pParentBranch->remove(this);
    }

    try
    {
        delete m_pMtx;
    }
    catch(...)
    {
    }

    m_entryType = static_cast<EIdxTreeEntryType>(0);
    //m_strName.clear();
    m_pMtx = nullptr;
    m_pTree = nullptr;
    //m_strKeyInTree.clear();
    m_idxInTree = 0;
    m_pParentBranch = nullptr;
    //m_strKeyInParentBranch.clear();
    m_idxInParentBranch = 0;
    m_bIsAboutToBeDestroyed = false;

} // dtor

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
    return ZS::System::idxTreeEntryType2Str(static_cast<int>(m_entryType), EEnumEntryAliasStrSymbol);
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
    return ZS::System::idxTreeEntryType2Str(static_cast<int>(m_entryType), i_alias);
}

//-----------------------------------------------------------------------------
/*! Returns the path string of the entry within the index tree.

    The path does not contain the type of the entry.

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
/*! Returns the unique key string of the entry within the index tree.

    The key does not include the name of the root entry.

    @return Unique key of the tree entry (e.g. "L:ZS::Data::CDataTable::Customers").
*/
QString CIdxTreeEntry::keyInTree() const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    QString strKeyInTree;

    if( m_entryType == EIdxTreeEntryType::Root )
    {
        // Just to have something to write into the log file.
        // The root does not really have a key in the tree.
        strKeyInTree = idxTreeEntryType2Str(m_entryType,EEnumEntryAliasStrSymbol) + ":" + m_strName;
    }
    else
    {
        strKeyInTree = m_strKeyInTree;
    }

    return strKeyInTree;

} // keyInTree

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

    if( m_pParentBranch != nullptr && m_pParentBranch->entryType() != EIdxTreeEntryType::Root )
    {
        strName = m_pParentBranch->name();
    }

    return strName;

} // parentBranchName

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

    if( m_pParentBranch != nullptr && m_pParentBranch->entryType() != EIdxTreeEntryType::Root )
    {
        strPath = m_pParentBranch->path();
    }

    return strPath;

} // parentBranchPath

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

//-----------------------------------------------------------------------------
/*! Returns the index of the given child tree entry in the branch.

    @param i_pTreeEntry [in] Pointer to child entry whose index is desired.

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
int CIdxTreeEntry::indexOf( const CIdxTreeEntry* i_pTreeEntry ) const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = i_pTreeEntry->indexInParentBranch();

    if( idxInParentBranch < 0 || idxInParentBranch >= m_arpTreeEntries.size() )
    {
        QString strAddErrInfo = i_pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
        SErrResultInfo errResultInfo = ErrResultInfoCritical("indexOf", EResultIdxOutOfRange, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    CIdxTreeEntry* pTreeEntry = const_cast<CIdxTreeEntry*>(i_pTreeEntry);

    if( idxInParentBranch != m_arpTreeEntries.indexOf(pTreeEntry) )
    {
        QString strAddErrInfo = i_pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
        SErrResultInfo errResultInfo = ErrResultInfoCritical("indexOf", EResultIdxOutOfRange, strAddErrInfo);
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
            SErrResultInfo errResultInfo = ErrResultInfoCritical("indexOf", EResultIdxOutOfRange, strAddErrInfo);
            throw CException(__FILE__, __LINE__, errResultInfo);
        }

        if( idxInParentBranch != m_arpTreeEntries.indexOf(pTreeEntry) )
        {
            QString strAddErrInfo = pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
            SErrResultInfo errResultInfo = ErrResultInfoCritical("indexOf", EResultIdxOutOfRange, strAddErrInfo);
            throw CException(__FILE__, __LINE__, errResultInfo);
        }
    } // if( pTreeEntry != nullptr )

    return idxInParentBranch;

} // indexOf

//-----------------------------------------------------------------------------
/*! Returns the index of the given child tree entry in the branch.

    @param i_entryType [in] Type of the child entry.
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
int CIdxTreeEntry::indexOf( EIdxTreeEntryType i_entryType, const QString& i_strName ) const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = -1;

    QString strEntryType = idxTreeEntryType2Str(i_entryType, EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranch = strEntryType + ":" + i_strName;
    CIdxTreeEntry* pTreeEntry = m_mappTreeEntries.value(strKeyInParentBranch, nullptr);

    if( pTreeEntry != nullptr )
    {
        idxInParentBranch = pTreeEntry->indexInParentBranch();

        if( idxInParentBranch < 0 || idxInParentBranch >= m_arpTreeEntries.size() )
        {
            QString strAddErrInfo = pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
            SErrResultInfo errResultInfo = ErrResultInfoCritical("indexOf", EResultIdxOutOfRange, strAddErrInfo);
            throw CException(__FILE__, __LINE__, errResultInfo);
        }

        if( idxInParentBranch != m_arpTreeEntries.indexOf(pTreeEntry) )
        {
            QString strAddErrInfo = pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
            SErrResultInfo errResultInfo = ErrResultInfoCritical("indexOf", EResultIdxOutOfRange, strAddErrInfo);
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
CIdxTreeEntry* CIdxTreeEntry::find( EIdxTreeEntryType i_entryType, const QString& i_strName ) const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);
    QString strEntryType = idxTreeEntryType2Str(i_entryType, EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranch = strEntryType + ":" + i_strName;
    return m_mappTreeEntries.value(strKeyInParentBranch, nullptr);
}

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Returns the index of the given child tree entry in the branch considering
    only entries with the same entry type of the given child entry.

    @param i_pTreeEntry [in] Pointer to child entry whose index is desired.

    @return Index of the given child entry in the branch.
*/
int CIdxTreeEntry::indexOfChildInListWithSameEntryTypes( const CIdxTreeEntry* i_pTreeEntry ) const
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = -1;

    EIdxTreeEntryType entryTypeFilter = i_pTreeEntry->entryType();

    CIdxTreeEntry* pTreeEntryTmp;
    int                    idxTmp;

    for( idxTmp = 0; idxTmp < m_arpTreeEntries.size(); ++idxTmp )
    {
        pTreeEntryTmp = m_arpTreeEntries[idxTmp];

        if( pTreeEntryTmp->entryType() == entryTypeFilter )
        {
            ++idxInParentBranch;
        }
        if( pTreeEntryTmp == i_pTreeEntry )
        {
            break;
        }
    }

    return idxInParentBranch;

} // indexOfChildInListWithSameEntryTypes

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
/*! Adds the given tree entry as a child of the branch.

    The entry will be appended at the end of the list of child entries.
    A reference to the child entry will be inserted in the map of the child entries
    with a unique key build of the entry types symbol followed by the node separator
    and the name of the child entry.

    The following members of the given child entry will be modified:

    - The branch will be set as the parent branch of the given child entry.
    - The unique key of the child entry within the branch will be set at the child entry.
    - The index of the child entry in the branch will be set at the child entry.

    @param i_pTreeEntry [in] Pointer to tree entry to be added as a child.

    @return Index of the child entry.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.
*/
int CIdxTreeEntry::add( CIdxTreeEntry* i_pTreeEntry )
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = -1;

    QString strEntryType = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranch = strEntryType + ":" + i_pTreeEntry->name();

    if( i_pTreeEntry->parentBranch() != nullptr )
    {
        QString strErrInfo = i_pTreeEntry->keyInTree() + " already belongs to " + i_pTreeEntry->parentBranch()->keyInTree();
        SErrResultInfo errResultInfo = ErrResultInfoCritical("add", EResultObjAlreadyInList, strErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( m_mappTreeEntries.contains(strKeyInParentBranch) )
    {
        SErrResultInfo errResultInfo = ErrResultInfoCritical("add", EResultObjAlreadyInList, strKeyInParentBranch);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    idxInParentBranch = m_arpTreeEntries.size();

    m_arpTreeEntries.append(i_pTreeEntry);
    m_mappTreeEntries.insert(strKeyInParentBranch, i_pTreeEntry);

    i_pTreeEntry->setParentBranch(this);
    i_pTreeEntry->setKeyInParentBranch(strKeyInParentBranch);
    i_pTreeEntry->setIndexInParentBranch(idxInParentBranch);

    return idxInParentBranch;

} // add

//-----------------------------------------------------------------------------
/*! Inserts the given tree entry at the given index as a child of the branch.

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
    @param i_pTreeEntry [in] Pointer to tree entry to be inserted as a child.

    @return Index of the child entry. Might be different from the given index if the index
            was less than 0 or greater the current size of the child list.

    @note If the child entry is inserted but not appended at the end of the list
          the index of all following child entries will be corrected.

    @note Throws a critical exception
          - with Result = ObjAlreadyInList if a child with the same name and
                          type already belongs to the branch.
*/
int CIdxTreeEntry::insert( int i_iIdx, CIdxTreeEntry* i_pTreeEntry )
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = -1;

    QString strEntryType = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranch = strEntryType + ":" + i_pTreeEntry->name();

    if( i_pTreeEntry->parentBranch() != nullptr )
    {
        QString strErrInfo = i_pTreeEntry->keyInTree() + " already belongs to " + i_pTreeEntry->parentBranch()->keyInTree();
        SErrResultInfo errResultInfo = ErrResultInfoCritical("insert", EResultObjAlreadyInList, strErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( m_mappTreeEntries.contains(strKeyInParentBranch) )
    {
        SErrResultInfo errResultInfo = ErrResultInfoCritical("insert", EResultObjAlreadyInList, strKeyInParentBranch);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( i_iIdx >= 0 && i_iIdx < m_arpTreeEntries.size() )
    {
        idxInParentBranch = i_iIdx;
        m_arpTreeEntries.insert(idxInParentBranch, i_pTreeEntry);

        CIdxTreeEntry* pTreeEntry;
        int                    idxEntry;

        for( idxEntry = idxInParentBranch+1; idxEntry < m_arpTreeEntries.size(); ++idxEntry )
        {
            pTreeEntry = m_arpTreeEntries[idxEntry];
            pTreeEntry->setIndexInParentBranch(idxEntry);
        }
    }
    else
    {
        idxInParentBranch = m_arpTreeEntries.size();
        m_arpTreeEntries.append(i_pTreeEntry);
    }

    m_mappTreeEntries.insert(strKeyInParentBranch, i_pTreeEntry);

    i_pTreeEntry->setParentBranch(this);
    i_pTreeEntry->setKeyInParentBranch(strKeyInParentBranch);
    i_pTreeEntry->setIndexInParentBranch(idxInParentBranch);

    return idxInParentBranch;

} // insert

//-----------------------------------------------------------------------------
/*! Removes the given tree entry from the branch.

    This method is either called when an entry is going about to be destroyed
    or if an entry is moved within the tree.

    The following members of the given child entry will be modified:

    - The parent branch of the given child entry will be set to nullptr.
    - The unique key of the child entry within the branch will be set to an empty string.
    - The index of the child entry in the branch will be set to -1.

    @param i_pTreeEntry [in] Pointer to tree entry to be removed.

    @note If the child entry is not the last entry in the list of child entries
          the index of all following child entries will be corrected.

    @note Throws a critical exception
          - with Result = IdxOutOfRange if the child's index in parent branch
                          does not correspond to the index as expected by the branch.
    @note Throws a critical exception
          - with Result = ObjNotInList if the tree entry is not a child of the branch.
*/
void CIdxTreeEntry::remove( CIdxTreeEntry* i_pTreeEntry )
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = i_pTreeEntry->indexInParentBranch();

    if( idxInParentBranch < 0 || idxInParentBranch >= m_arpTreeEntries.size() )
    {
        QString strAddErrInfo = i_pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
        SErrResultInfo errResultInfo = ErrResultInfoCritical("remove", EResultIdxOutOfRange, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( idxInParentBranch != m_arpTreeEntries.indexOf(i_pTreeEntry) )
    {
        QString strAddErrInfo = i_pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
        SErrResultInfo errResultInfo = ErrResultInfoCritical("indexOf", EResultIdxOutOfRange, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QString strKeyInParentBranch = i_pTreeEntry->keyInParentBranch();

    if( !m_mappTreeEntries.contains(strKeyInParentBranch) )
    {
        SErrResultInfo errResultInfo = ErrResultInfoCritical("remove", EResultObjNotInList, i_pTreeEntry->keyInParentBranch());
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    m_arpTreeEntries.remove(idxInParentBranch);
    m_mappTreeEntries.remove(strKeyInParentBranch);

    i_pTreeEntry->setParentBranch(nullptr);
    i_pTreeEntry->setKeyInParentBranch("");
    i_pTreeEntry->setIndexInParentBranch(-1);

    CIdxTreeEntry* pTreeEntry;
    int            idxEntry;

    for( idxEntry = idxInParentBranch; idxEntry < m_arpTreeEntries.size(); ++idxEntry )
    {
        pTreeEntry = m_arpTreeEntries[idxEntry];
        pTreeEntry->setIndexInParentBranch(idxEntry);
    }

} // remove

//-----------------------------------------------------------------------------
/*! Renames the given tree entry to the given new name.

    The following members of the given child entry will be modified:

    - The unique key of the child entry within the branch will be modified.

    @param i_pTreeEntry [in] Pointer to tree entry to be renamed.
    @param i_strNameNew [in] New name of the child entry.

    @note Throws a critical exception
          - with Result = IdxOutOfRange if the child's index in parent branch
                          does not correspond to the index as expected by the branch.
    @note Throws a critical exception
          - with Result = ObjNotInList if the tree entry is not a child of the branch.
*/
void CIdxTreeEntry::rename( CIdxTreeEntry* i_pTreeEntry, const QString& i_strNameNew )
//-----------------------------------------------------------------------------
{
    QMutexLocker mtxLocker(m_pMtx);

    int idxInParentBranch = i_pTreeEntry->indexInParentBranch();

    if( idxInParentBranch < 0 || idxInParentBranch >= m_arpTreeEntries.size() )
    {
        QString strAddErrInfo = i_pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
        SErrResultInfo errResultInfo = ErrResultInfoCritical("rename", EResultIdxOutOfRange, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    if( idxInParentBranch != m_arpTreeEntries.indexOf(i_pTreeEntry) )
    {
        QString strAddErrInfo = i_pTreeEntry->keyInTree() + ".indexInParentBranch: " + QString::number(idxInParentBranch);
        SErrResultInfo errResultInfo = ErrResultInfoCritical("indexOf", EResultIdxOutOfRange, strAddErrInfo);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QString strKeyInParentBranchOrig = i_pTreeEntry->keyInParentBranch();

    if( !m_mappTreeEntries.contains(strKeyInParentBranchOrig) )
    {
        SErrResultInfo errResultInfo = ErrResultInfoCritical("rename", EResultObjNotInList, strKeyInParentBranchOrig);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    QString strEntryType = i_pTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranchNew = strEntryType + ":" + i_strNameNew;

    if( m_mappTreeEntries.contains(strKeyInParentBranchNew) )
    {
        SErrResultInfo errResultInfo = ErrResultInfoCritical("rename", EResultObjAlreadyInList, strKeyInParentBranchNew);
        throw CException(__FILE__, __LINE__, errResultInfo);
    }

    i_pTreeEntry->setName(i_strNameNew);

    m_mappTreeEntries.remove(strKeyInParentBranchOrig);
    m_mappTreeEntries.insert(strKeyInParentBranchNew, i_pTreeEntry);

    i_pTreeEntry->setKeyInParentBranch(strKeyInParentBranchNew);

} // rename
