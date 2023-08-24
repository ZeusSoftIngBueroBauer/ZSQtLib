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

#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/******************************************************************************
class CModelIdxTreeEntry
******************************************************************************/

/*=============================================================================
public: // ctors and dtor
=============================================================================*/

//-----------------------------------------------------------------------------
CModelIdxTreeEntry::CModelIdxTreeEntry( CIdxTreeEntry* i_pTreeEntry ) :
//-----------------------------------------------------------------------------
    m_pIdxTree(i_pTreeEntry->tree()),
    m_entryType(CIdxTreeEntry::EEntryType::Undefined),
    m_strKeyInTree(i_pTreeEntry->keyInTree()),
    m_idxInTree(i_pTreeEntry->indexInTree()),
    m_pParentBranch(nullptr),
    m_idxInParentBranch(-1),
    m_bIsSelected(false),
    m_bExcludeLeaves(false),
    m_sortOrder(EIdxTreeSortOrder::Config),
    m_mappModelTreeEntries(),
    m_arpTreeEntries(),
    m_bIsExpanded(false)
{
    if( i_pTreeEntry->isRoot() ) {
        m_entryType = CIdxTreeEntry::EEntryType::Root;
    }
    else if( i_pTreeEntry->isBranch() ) {
        m_entryType = CIdxTreeEntry::EEntryType::Branch;
    }
    else if( i_pTreeEntry->isLeave() ) {
        m_entryType = CIdxTreeEntry::EEntryType::Leave;
    }

} // ctor

//-----------------------------------------------------------------------------
CModelIdxTreeEntry::~CModelIdxTreeEntry()
//-----------------------------------------------------------------------------
{
    CModelIdxTreeEntry* pModelTreeEntry;
    int                 idxEntry;

    if( m_arpTreeEntries.size() > 0 )
    {
        for( idxEntry = m_arpTreeEntries.size() - 1; idxEntry >= 0; --idxEntry )
        {
            pModelTreeEntry = m_arpTreeEntries[idxEntry];

            try
            {
                delete pModelTreeEntry; // calls "remove" as reentry
            }
            catch(...)
            {
            }
            pModelTreeEntry = nullptr;
        }
    }

    // As "remove" has been called as reentry for the deleted children
    // the map and vector must already be empty.
    if( m_mappModelTreeEntries.size() > 0 || m_arpTreeEntries.size() > 0 )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo(
                /* errSource */ NameSpace(), ClassName(), keyInTree(), "dtor",
                /* result    */ EResultListNotEmpty,
                /* severity  */ EResultSeverityCritical );
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

    if( m_pParentBranch != nullptr )
    {
        m_pParentBranch->remove(this);
    }

    m_entryType = static_cast<CIdxTreeEntry::EEntryType>(0);
    //m_strKeyInTree;
    m_idxInTree = 0;
    m_pParentBranch = nullptr;
    m_idxInParentBranch = 0;
    m_bIsSelected = false;
    m_bExcludeLeaves = false;
    m_sortOrder = static_cast<EIdxTreeSortOrder>(0);
    m_mappModelTreeEntries.clear();
    m_arpTreeEntries.clear();
    m_bIsExpanded = false;

} // dtor

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
/*! @brief Returns the pointer to the real index tree entry.

    The real index tree entry is accessed through the key in the tree.
    As the index tree might be modified from different threads the
    real index tree entry may already have been deleted before the
    model gets informed by queued signal/slot connections about the
    deletion of tree entries.

    @note Lock the tree as long as you access the index tree entry and don't
          access the tree entry after removing the lock.
*/
CIdxTreeEntry* CModelIdxTreeEntry::getIdxTreeEntry()
//-----------------------------------------------------------------------------
{
    CIdxTreeLocker idxTreeLocker(m_pIdxTree);
    return m_pIdxTree->findEntry(m_strKeyInTree);
}

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
bool CModelIdxTreeEntry::isRoot() const
//-----------------------------------------------------------------------------
{
    return (m_entryType == CIdxTreeEntry::EEntryType::Root);
}

//-----------------------------------------------------------------------------
bool CModelIdxTreeEntry::isBranch() const
//-----------------------------------------------------------------------------
{
    return (m_entryType == CIdxTreeEntry::EEntryType::Branch);
}

//-----------------------------------------------------------------------------
bool CModelIdxTreeEntry::isLeave() const
//-----------------------------------------------------------------------------
{
    return (m_entryType == CIdxTreeEntry::EEntryType::Leave);
}

//-----------------------------------------------------------------------------
QString CModelIdxTreeEntry::entryTypeSymbol() const
//-----------------------------------------------------------------------------
{
    return CIdxTreeEntry::entryType2Str(m_entryType, EEnumEntryAliasStrSymbol);
}

//-----------------------------------------------------------------------------
QString CModelIdxTreeEntry::entryType2Str( int i_alias ) const
//-----------------------------------------------------------------------------
{
    return CIdxTreeEntry::entryType2Str(m_entryType, i_alias);
}


/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
void CModelIdxTreeEntry::setKeyInTree( const QString& i_strNewKeyInTree )
//-----------------------------------------------------------------------------
{
    QString strNamePrev = name();
    m_strKeyInTree = i_strNewKeyInTree;
    m_pParentBranch->onChildRenamed(this, strNamePrev);
}

//-----------------------------------------------------------------------------
QString CModelIdxTreeEntry::keyInTree() const
//-----------------------------------------------------------------------------
{
    return m_strKeyInTree;
}

//-----------------------------------------------------------------------------
int CModelIdxTreeEntry::indexInTree() const
//-----------------------------------------------------------------------------
{
    return m_idxInTree;
}

//-----------------------------------------------------------------------------
QString CModelIdxTreeEntry::name() const
//-----------------------------------------------------------------------------
{
    QString strName;
    m_pIdxTree->splitPathStr(m_strKeyInTree, nullptr, &strName);
    return strName;
}

//-----------------------------------------------------------------------------
QString CModelIdxTreeEntry::path() const
//-----------------------------------------------------------------------------
{
    QString strPath;
    m_pIdxTree->splitPathStr(m_strKeyInTree, &strPath, nullptr);
    return strPath;
}

//-----------------------------------------------------------------------------
QString CModelIdxTreeEntry::keyInParentBranch() const
//-----------------------------------------------------------------------------
{
    return entryType2Str(EEnumEntryAliasStrSymbol) + ":" + name();
}

////-----------------------------------------------------------------------------
//int CModelIdxTreeEntry::indexInParentBranch() const
////-----------------------------------------------------------------------------
//{
//    CIdxTreeLocker idxTreeLocker(m_pIdxTree);
//    int idx = -1;
//    CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTree);
//    if( pTreeEntry != nullptr )
//    {
//        idx = pTreeEntry->indexInParentBranch();
//    }
//    return idx;
//}

////-----------------------------------------------------------------------------
//int CModelIdxTreeEntry::indexInParentBranchsChildListWithSameEntryTypes() const
////-----------------------------------------------------------------------------
//{
//    CIdxTreeLocker idxTreeLocker(m_pIdxTree);
//    int idx = -1;
//    CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTree);
//    if( pTreeEntry != nullptr )
//    {
//        idx = pTreeEntry->indexInParentBranchsChildListWithSameEntryTypes();
//    }
//    return idx;
//}

/*=============================================================================
public: // instance methods (applying filter)
=============================================================================*/

//-----------------------------------------------------------------------------
int CModelIdxTreeEntry::indexInParentBranchsChildListWithSameEntryTypes() const
//-----------------------------------------------------------------------------
{
    int idx = -1;

    // To be filtered by entry type
    if( m_pParentBranch != nullptr )
    {
        idx = m_pParentBranch->indexOfChildInListWithSameEntryTypes(this);
    }
    return idx;
}

/*=============================================================================
public: // instance methods
=============================================================================*/

////-----------------------------------------------------------------------------
//QString CModelIdxTreeEntry::getCalculatedKeyInModel() const
////-----------------------------------------------------------------------------
//{
//    CIdxTree* pIdxTree = m_pModel->idxTree();
//
//    QString strEntryType     = entryType2Str(EEnumEntryAliasStrSymbol);
//    QString strNodeSeparator = pIdxTree != nullptr ? pIdxTree->nodeSeparator() : "/";
//    QString strKeyInModel    = strEntryType + ":";
//    QString strParentPath;
//
//    CModelIdxTreeEntry* pParentBranch = modelParentBranch();
//
//    CModelIdxTreeEntry* pRoot = m_pModel != nullptr ? m_pModel->modelRoot() : nullptr;
//
//    if( pParentBranch != nullptr && pParentBranch != pRoot )
//    {
//        strParentPath = pParentBranch->path();
//    }
//    if( !strParentPath.isEmpty() )
//    {
//        strKeyInModel += strParentPath;
//
//        if( !strKeyInModel.endsWith(strNodeSeparator) )
//        {
//            strKeyInModel += strNodeSeparator;
//        }
//    }
//
//    strKeyInModel += name();
//
//    return strKeyInModel;
//
//} // getCalculatedKeyInModel

////-----------------------------------------------------------------------------
//QString CModelIdxTreeEntry::getCalculatedKeyInTree() const
////-----------------------------------------------------------------------------
//{
//    QString strKeyInTree;
//
//    if( m_pTreeEntry != nullptr )
//    {
//        strKeyInTree = m_pTreeEntry->getCalculatedKeyInTree();
//    }
//    return strKeyInTree;
//}

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
void CModelIdxTreeEntry::setIsSelected( bool i_bIsSelected )
//-----------------------------------------------------------------------------
{
    m_bIsSelected = i_bIsSelected;
}

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
void CModelIdxTreeEntry::setExcludeLeaves( bool i_bExcludeLeaves )
//-----------------------------------------------------------------------------
{
    m_bExcludeLeaves = i_bExcludeLeaves;
}

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
void CModelIdxTreeEntry::setSortOrder( EIdxTreeSortOrder i_sortOrder )
//-----------------------------------------------------------------------------
{
    m_sortOrder = i_sortOrder;
}

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
int CModelIdxTreeEntry::indexOf( CModelIdxTreeEntry* i_pModelTreeEntry ) const
//-----------------------------------------------------------------------------
{
    return m_arpTreeEntries.indexOf(i_pModelTreeEntry);
}

//-----------------------------------------------------------------------------
int CModelIdxTreeEntry::indexOf( const QString& i_strKeyInParentBranch ) const
//-----------------------------------------------------------------------------
{
    CModelIdxTreeEntry* pModelTreeEntry = findEntry(i_strKeyInParentBranch);
    return m_arpTreeEntries.indexOf(pModelTreeEntry);
}

//-----------------------------------------------------------------------------
int CModelIdxTreeEntry::indexOf( const QString& i_strEntryTypeSymbol, const QString& i_strName ) const
//-----------------------------------------------------------------------------
{
    CModelIdxTreeEntry* pModelTreeEntry = findEntry(i_strEntryTypeSymbol, i_strName);
    return m_arpTreeEntries.indexOf(pModelTreeEntry);
}

/*=============================================================================
public: // instance methods (applying filter)
=============================================================================*/

//-----------------------------------------------------------------------------
int CModelIdxTreeEntry::indexOfChildInListWithSameEntryTypes(
    const CModelIdxTreeEntry* i_pModelTreeEntry ) const
//-----------------------------------------------------------------------------
{
    int idxInParentBranch = -1;

    QString strEntryTypeFilter = i_pModelTreeEntry->entryTypeSymbol();

    CModelIdxTreeEntry* pModelTreeEntryTmp;

    for( int idxTmp = 0; idxTmp < m_arpTreeEntries.size(); ++idxTmp )
    {
        pModelTreeEntryTmp = m_arpTreeEntries[idxTmp];

        if( pModelTreeEntryTmp->entryTypeSymbol() == strEntryTypeFilter )
        {
            ++idxInParentBranch;
        }
        if( pModelTreeEntryTmp == i_pModelTreeEntry )
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
CModelIdxTreeEntry* CModelIdxTreeEntry::findBranch( const QString& i_strName ) const
//-----------------------------------------------------------------------------
{
    QString strEntryType =
        CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranch = i_strName;
    if( !strKeyInParentBranch.startsWith(strEntryType + ":") ) {
        strKeyInParentBranch.insert(0, strEntryType + ":");
    }
    return m_mappModelTreeEntries.value(strKeyInParentBranch, nullptr);
}

//-----------------------------------------------------------------------------
CModelIdxTreeEntry* CModelIdxTreeEntry::findLeave( const QString& i_strName ) const
//-----------------------------------------------------------------------------
{
    QString strEntryType =
        CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Leave, EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranch = i_strName;
    if( !strKeyInParentBranch.startsWith(strEntryType + ":") ) {
        strKeyInParentBranch.insert(0, strEntryType + ":");
    }
    return m_mappModelTreeEntries.value(strKeyInParentBranch, nullptr);
}

//-----------------------------------------------------------------------------
CModelIdxTreeEntry* CModelIdxTreeEntry::findEntry( const QString& i_strKeyInParentBranch ) const
//-----------------------------------------------------------------------------
{
    return m_mappModelTreeEntries.value(i_strKeyInParentBranch, nullptr);
}

//-----------------------------------------------------------------------------
CModelIdxTreeEntry* CModelIdxTreeEntry::findEntry(
    const QString& i_strEntryTypeSymbol, const QString& i_strName ) const
//-----------------------------------------------------------------------------
{
    QString strKeyInParentBranch = i_strName;
    if( !strKeyInParentBranch.startsWith(i_strEntryTypeSymbol + ":") ) {
        strKeyInParentBranch.insert(0, i_strEntryTypeSymbol + ":");
    }
    return m_mappModelTreeEntries.value(strKeyInParentBranch, nullptr);
}

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
int CModelIdxTreeEntry::add( CModelIdxTreeEntry* i_pModelTreeEntry )
//-----------------------------------------------------------------------------
{
    int idxInParentBranch = -1;

    QString strKeyInParentBranch = i_pModelTreeEntry->keyInParentBranch();

    if( m_mappModelTreeEntries.contains(strKeyInParentBranch) )
    {
        throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKeyInParentBranch);
    }

    if( m_sortOrder == EIdxTreeSortOrder::Ascending )
    {
        QMap<QString, CModelIdxTreeEntry*>::iterator itModelEntry =
            m_mappModelTreeEntries.insert(strKeyInParentBranch, i_pModelTreeEntry);

        // Inserted before this entry (or appended at the end).
        ++itModelEntry;

        // If appended at the end ..
        if( itModelEntry == m_mappModelTreeEntries.end() )
        {
            idxInParentBranch = m_arpTreeEntries.size();
            m_arpTreeEntries.append(i_pModelTreeEntry);
        }
        // If inserted before an existing entry ..
        else
        {
            idxInParentBranch = itModelEntry.value()->indexInParentBranch();
            m_arpTreeEntries.append(nullptr);

            // Move all following entries one index backwards.
            for( int idxEntry = m_arpTreeEntries.size()-2; idxEntry >= idxInParentBranch; --idxEntry )
            {
                CModelIdxTreeEntry* pModelTreeEntry = m_arpTreeEntries[idxEntry];
                m_arpTreeEntries[idxEntry+1] = pModelTreeEntry;
                pModelTreeEntry->setIndexInParentBranch(idxEntry+1);
            }
            m_arpTreeEntries[idxInParentBranch] = i_pModelTreeEntry;
        }
    }
    else if( m_sortOrder == EIdxTreeSortOrder::Descending )
    {
        m_mappModelTreeEntries.insert(strKeyInParentBranch, i_pModelTreeEntry);

        QMapIterator<QString, CModelIdxTreeEntry*> itModelEntry(m_mappModelTreeEntries);
        itModelEntry.toBack();
        while(itModelEntry.hasPrevious())
        {
            itModelEntry.previous();
            ++idxInParentBranch;
            if( m_mappModelTreeEntries.value(itModelEntry.key()) == i_pModelTreeEntry)
            {
                break;
            }
        }

        // If appended at the end ..
        if( idxInParentBranch == m_arpTreeEntries.size() )
        {
            m_arpTreeEntries.append(i_pModelTreeEntry);
        }
        // If inserted before an existing entry ..
        else
        {
            m_arpTreeEntries.append(nullptr);

            // Move all following entries one index backwards.
            for( int idxEntry = m_arpTreeEntries.size()-2; idxEntry >= idxInParentBranch; --idxEntry )
            {
                CModelIdxTreeEntry* pModelTreeEntry = m_arpTreeEntries[idxEntry];
                m_arpTreeEntries[idxEntry+1] = pModelTreeEntry;
                pModelTreeEntry->setIndexInParentBranch(idxEntry+1);
            }
            m_arpTreeEntries[idxInParentBranch] = i_pModelTreeEntry;
        }
    }
    else
    {
        CIdxTreeLocker idxTreeLocker(m_pIdxTree);

        CIdxTreeEntry* pTreeEntry = i_pModelTreeEntry->getIdxTreeEntry();

        if (!(m_bExcludeLeaves && i_pModelTreeEntry->isLeave()))
        {
            idxInParentBranch = pTreeEntry->indexInParentBranch();

            if( idxInParentBranch < 0 || idxInParentBranch > m_arpTreeEntries.size() )
            {
                QString strAddErrInfo;
                strAddErrInfo  = "IdxInParentBranch (=" + QString::number(idxInParentBranch) + ") is out of range";
                strAddErrInfo += " [0 .. " + QString::number(m_arpTreeEntries.size()) + "]";
                SErrResultInfo errResultInfo(
                    /* errSource  */ NameSpace(), ClassName(), keyInTree(), "add(" + i_pModelTreeEntry->keyInTree() + ")",
                    /* result     */ EResultInternalProgramError,
                    /* severity   */ EResultSeverityCritical,
                    /* strAddInfo */ strAddErrInfo );
                throw CException(__FILE__, __LINE__, errResultInfo);
            }

            m_mappModelTreeEntries.insert(strKeyInParentBranch, i_pModelTreeEntry);

            // If appended at the end ..
            if( idxInParentBranch == m_arpTreeEntries.size() )
            {
                m_arpTreeEntries.append(i_pModelTreeEntry);
            }
            // If inserted before an existing entry ..
            else // if( idxInParentBranch < m_arpTreeEntries.size() )
            {
                m_arpTreeEntries.append(nullptr);

                // Move all following entries one index backwards.
                for( int idxEntry = m_arpTreeEntries.size()-2; idxEntry >= idxInParentBranch; --idxEntry )
                {
                    CModelIdxTreeEntry* pModelTreeEntry = m_arpTreeEntries[idxEntry];
                    m_arpTreeEntries[idxEntry+1] = pModelTreeEntry;
                    pModelTreeEntry->setIndexInParentBranch(idxEntry+1);
                }
                m_arpTreeEntries[idxInParentBranch] = i_pModelTreeEntry;
            }
        }
    }

    i_pModelTreeEntry->setParentBranch(this);
    i_pModelTreeEntry->setIndexInParentBranch(idxInParentBranch);

    return idxInParentBranch;

} // add

//-----------------------------------------------------------------------------
void CModelIdxTreeEntry::remove( CModelIdxTreeEntry* i_pModelTreeEntry )
//-----------------------------------------------------------------------------
{
    int idxInParentBranch = i_pModelTreeEntry->indexInParentBranch();

    if( idxInParentBranch < 0 || idxInParentBranch >= m_arpTreeEntries.size() )
    {
        throw CException(__FILE__, __LINE__, EResultIdxOutOfRange, QString::number(idxInParentBranch));
    }

    QString strKeyInParentBranch = i_pModelTreeEntry->keyInParentBranch();

    if( !m_mappModelTreeEntries.contains(strKeyInParentBranch) )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, strKeyInParentBranch);
    }

    m_arpTreeEntries.remove(idxInParentBranch);
    m_mappModelTreeEntries.remove(strKeyInParentBranch);

    i_pModelTreeEntry->setParentBranch(nullptr);
    i_pModelTreeEntry->setIndexInParentBranch(-1);

    for( int idxEntry = idxInParentBranch; idxEntry < m_arpTreeEntries.size(); ++idxEntry )
    {
        CModelIdxTreeEntry* pModelTreeEntry = m_arpTreeEntries[idxEntry];
        pModelTreeEntry->setIndexInParentBranch(idxEntry);
    }
} // remove

//-----------------------------------------------------------------------------
void CModelIdxTreeEntry::remove( const QString& i_strKeyInParentBranch )
//-----------------------------------------------------------------------------
{
    CModelIdxTreeEntry* pModelTreeEntry = m_mappModelTreeEntries.value(i_strKeyInParentBranch, nullptr);

    if( pModelTreeEntry == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultObjNotInList, i_strKeyInParentBranch);
    }

    int idxInParentBranch = m_arpTreeEntries.indexOf(pModelTreeEntry);

    if( idxInParentBranch < 0 || idxInParentBranch >= m_arpTreeEntries.size() )
    {
        throw CException(__FILE__, __LINE__, EResultIdxOutOfRange, QString::number(idxInParentBranch));
    }

    m_arpTreeEntries.remove(idxInParentBranch);
    m_mappModelTreeEntries.remove(i_strKeyInParentBranch);

    pModelTreeEntry->setParentBranch(nullptr);
    pModelTreeEntry->setIndexInParentBranch(-1);

    for( int idxEntry = idxInParentBranch; idxEntry < m_arpTreeEntries.size(); ++idxEntry )
    {
        pModelTreeEntry = m_arpTreeEntries[idxEntry];
        pModelTreeEntry->setIndexInParentBranch(idxEntry);
    }
} // remove

//-----------------------------------------------------------------------------
void CModelIdxTreeEntry::onChildRenamed(
    CModelIdxTreeEntry* i_pModelTreeEntry,
    const QString&      i_strNamePrev )
//-----------------------------------------------------------------------------
{
    int idxInParentBranchPrev = i_pModelTreeEntry->indexInParentBranch();

    if( idxInParentBranchPrev < 0 || idxInParentBranchPrev >= m_arpTreeEntries.size() )
    {
        throw CException(__FILE__, __LINE__, EResultIdxOutOfRange, QString::number(idxInParentBranchPrev));
    }

    QString strKeyInParentBranchPrev = i_pModelTreeEntry->entryType2Str(EEnumEntryAliasStrSymbol) + ":" + i_strNamePrev;;
    QString strKeyInParentBranchNew = i_pModelTreeEntry->keyInParentBranch();

    if( strKeyInParentBranchNew != strKeyInParentBranchPrev )
    {
        if( !m_mappModelTreeEntries.contains(strKeyInParentBranchPrev) )
        {
            throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKeyInParentBranchNew);
        }
        if( m_mappModelTreeEntries.contains(strKeyInParentBranchNew) )
        {
            throw CException(__FILE__, __LINE__, EResultObjAlreadyInList, strKeyInParentBranchNew);
        }

        m_mappModelTreeEntries.remove(strKeyInParentBranchPrev);

        if( m_sortOrder == EIdxTreeSortOrder::Ascending )
        {
            m_arpTreeEntries.removeOne(i_pModelTreeEntry);

            QMap<QString, CModelIdxTreeEntry*>::iterator itModelEntry =
                m_mappModelTreeEntries.insert(strKeyInParentBranchNew, i_pModelTreeEntry);

            // Inserted before this entry (or appended at the end).
            ++itModelEntry;

            int idxInParentBranch = -1;

            // If appended at the end ..
            if( itModelEntry == m_mappModelTreeEntries.end() )
            {
                idxInParentBranch = m_arpTreeEntries.size();
                m_arpTreeEntries.append(i_pModelTreeEntry);
            }
            // If inserted before an existing entry ..
            else // if( itModelEntry != m_mappModelTreeEntries.end() )
            {
                idxInParentBranch = itModelEntry.value()->indexInParentBranch();
                m_arpTreeEntries.append(nullptr);

                // Move all following entries one index backwards.
                for( int idxEntry = m_arpTreeEntries.size()-2; idxEntry >= idxInParentBranch; --idxEntry )
                {
                    CModelIdxTreeEntry* pModelTreeEntry = m_arpTreeEntries[idxEntry];
                    m_arpTreeEntries[idxEntry+1] = pModelTreeEntry;
                    pModelTreeEntry->setIndexInParentBranch(idxEntry+1);
                }
                m_arpTreeEntries[idxInParentBranch] = i_pModelTreeEntry;
            }
            i_pModelTreeEntry->setIndexInParentBranch(idxInParentBranch);
        }
        else if( m_sortOrder == EIdxTreeSortOrder::Descending )
        {
            m_arpTreeEntries.removeOne(i_pModelTreeEntry);

            m_mappModelTreeEntries.insert(strKeyInParentBranchNew, i_pModelTreeEntry);

            int idxInParentBranch = -1;

            QMapIterator<QString, CModelIdxTreeEntry*> itModelEntry(m_mappModelTreeEntries);
            itModelEntry.toBack();
            while(itModelEntry.hasPrevious())
            {
                itModelEntry.previous();
                ++idxInParentBranch;
                if( m_mappModelTreeEntries.value(itModelEntry.key()) == i_pModelTreeEntry)
                {
                    break;
                }
            }

            // If appended at the end ..
            if( idxInParentBranch == m_arpTreeEntries.size() )
            {
                m_arpTreeEntries.append(i_pModelTreeEntry);
            }
            // If inserted before an existing entry ..
            else
            {
                m_arpTreeEntries.append(nullptr);

                // Move all following entries one index backwards.
                for( int idxEntry = idxInParentBranch; idxEntry < m_arpTreeEntries.size()-1; ++idxEntry )
                {
                    CModelIdxTreeEntry* pModelTreeEntry = m_arpTreeEntries[idxEntry];
                    m_arpTreeEntries[idxEntry+1] = pModelTreeEntry;
                    pModelTreeEntry->setIndexInParentBranch(idxEntry+1);
                }
                m_arpTreeEntries[idxInParentBranch] = i_pModelTreeEntry;
            }
            i_pModelTreeEntry->setIndexInParentBranch(idxInParentBranch);
        }
        else // if( m_sortOrder == EIdxTreeSortOrder::Config )
        {
            m_mappModelTreeEntries.insert(strKeyInParentBranchNew, i_pModelTreeEntry);
        }
    } // if( strKeyInParentBranchNew != strKeyInParentBranchPrev )
} // onChildRenamed

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
void CModelIdxTreeEntry::setIsExpanded( bool i_bIsExpanded )
//-----------------------------------------------------------------------------
{
    m_bIsExpanded = i_bIsExpanded;
}
