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
protected: // ctors
=============================================================================*/

//-----------------------------------------------------------------------------
CModelIdxTreeEntry::CModelIdxTreeEntry( CIdxTreeEntry* i_pTreeEntry ) :
//-----------------------------------------------------------------------------
    m_pIdxTree(i_pTreeEntry->tree()),
    m_entryType(i_pTreeEntry->entryType()),
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
} // ctor

/*=============================================================================
public: // dtor
=============================================================================*/

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

    m_entryType = static_cast<EIdxTreeEntryType>(0);
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
CIdxTreeEntry* CModelIdxTreeEntry::treeEntry()
//-----------------------------------------------------------------------------
{
    return m_pIdxTree->findEntry(m_strKeyInTree);
}

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
EIdxTreeEntryType CModelIdxTreeEntry::entryType() const
//-----------------------------------------------------------------------------
{
    return m_entryType;
}

//-----------------------------------------------------------------------------
QString CModelIdxTreeEntry::entryType2Str( int i_alias ) const
//-----------------------------------------------------------------------------
{
    return idxTreeEntryType2Str(m_entryType, i_alias);
}

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
    QString strKey;
    CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTree);
    if( pTreeEntry != nullptr )
    {
        strKey = pTreeEntry->keyInParentBranch();
    }
    else
    {
        strKey = entryType2Str(EEnumEntryAliasStrSymbol) + ":" + name();
    }
    return strKey;
}

//-----------------------------------------------------------------------------
int CModelIdxTreeEntry::indexInParentBranch() const
//-----------------------------------------------------------------------------
{
    int idx = -1;
    CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTree);
    if( pTreeEntry != nullptr )
    {
        idx = pTreeEntry->indexInParentBranch();
    }
    return idx;
}

//-----------------------------------------------------------------------------
int CModelIdxTreeEntry::indexInParentBranchsChildListWithSameEntryTypes() const
//-----------------------------------------------------------------------------
{
    int idx = -1;
    CIdxTreeEntry* pTreeEntry = m_pIdxTree->findEntry(m_strKeyInTree);
    if( pTreeEntry != nullptr )
    {
        idx = pTreeEntry->indexInParentBranchsChildListWithSameEntryTypes();
    }
    return idx;
}

/*=============================================================================
public: // instance methods (applying filter)
=============================================================================*/

//-----------------------------------------------------------------------------
int CModelIdxTreeEntry::modelIndexInParentBranchsChildListWithSameEntryTypes() const
//-----------------------------------------------------------------------------
{
    int idx = -1;

    // To be filtered by entry type
    if( m_pParentBranch != nullptr )
    {
        idx = m_pParentBranch->modelIndexOfChildInListWithSameEntryTypes(this);
    }
    return idx;

} // modelIndexInParentBranchsChildListWithSameEntryTypes

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
public: // instance methods (applying filter)
=============================================================================*/

//-----------------------------------------------------------------------------
int CModelIdxTreeEntry::modelIndexOfChildInListWithSameEntryTypes(
    const CModelIdxTreeEntry* i_pModelTreeEntry ) const
//-----------------------------------------------------------------------------
{
    int idxInParentBranch = -1;

    EIdxTreeEntryType entryTypeFilter = i_pModelTreeEntry->entryType();

    CModelIdxTreeEntry* pModelTreeEntryTmp;
    int                 idxTmp;

    for( idxTmp = 0; idxTmp < m_arpTreeEntries.size(); ++idxTmp )
    {
        pModelTreeEntryTmp = m_arpTreeEntries[idxTmp];

        if( pModelTreeEntryTmp->entryType() == entryTypeFilter )
        {
            ++idxInParentBranch;
        }
        if( pModelTreeEntryTmp == i_pModelTreeEntry )
        {
            break;
        }
    }

    return idxInParentBranch;

} // modelIndexOfChildInListWithSameEntryTypes

/*=============================================================================
public: // instance methods
=============================================================================*/

//-----------------------------------------------------------------------------
CModelIdxTreeEntry* CModelIdxTreeEntry::findModelBranch( const QString& i_strName )
//-----------------------------------------------------------------------------
{
    QString strEntryType = idxTreeEntryType2Str(EIdxTreeEntryType::Branch, EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranch = i_strName;

    if( !strKeyInParentBranch.startsWith(strEntryType + ":") )
    {
        strKeyInParentBranch.insert(0, strEntryType + ":");
    }

    return m_mappModelTreeEntries.value(strKeyInParentBranch, nullptr);

} // findModelBranch

//-----------------------------------------------------------------------------
CModelIdxTreeEntry* CModelIdxTreeEntry::findModelLeave( const QString& i_strName )
//-----------------------------------------------------------------------------
{
    QString strEntryType = idxTreeEntryType2Str(EIdxTreeEntryType::Leave, EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranch = i_strName;

    if( !strKeyInParentBranch.startsWith(strEntryType + ":") )
    {
        strKeyInParentBranch.insert(0, strEntryType + ":");
    }

    return m_mappModelTreeEntries.value(strKeyInParentBranch, nullptr);

} // findModelLeave

//-----------------------------------------------------------------------------
CModelIdxTreeEntry* CModelIdxTreeEntry::findModelEntry( EIdxTreeEntryType i_entryType, const QString& i_strName )
//-----------------------------------------------------------------------------
{
    QString strEntryType = idxTreeEntryType2Str(i_entryType, EEnumEntryAliasStrSymbol);
    QString strKeyInParentBranch = i_strName;

    if( !strKeyInParentBranch.startsWith(strEntryType + ":") )
    {
        strKeyInParentBranch.insert(0, strEntryType + ":");
    }

    return m_mappModelTreeEntries.value(strKeyInParentBranch, nullptr);

} // findModelEntry

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
            idxInParentBranch = itModelEntry.value()->modelIndexInParentBranch();
            m_arpTreeEntries.append(nullptr);

            // Move all following entries one index backwards.
            for( int idxEntry = m_arpTreeEntries.size()-2; idxEntry >= idxInParentBranch; --idxEntry )
            {
                CModelIdxTreeEntry* pModelTreeEntry = m_arpTreeEntries[idxEntry];
                m_arpTreeEntries[idxEntry+1] = pModelTreeEntry;
                pModelTreeEntry->setModelIndexInParentBranch(idxEntry+1);
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
            for( int idxEntry = idxInParentBranch; idxEntry < m_arpTreeEntries.size()-1; ++idxEntry )
            {
                CModelIdxTreeEntry* pModelTreeEntry = m_arpTreeEntries[idxEntry];
                m_arpTreeEntries[idxEntry+1] = pModelTreeEntry;
                pModelTreeEntry->setModelIndexInParentBranch(idxEntry+1);
            }
            m_arpTreeEntries[idxInParentBranch] = i_pModelTreeEntry;
        }
    }
    else
    {
        if( !m_bExcludeLeaves )
        {
            idxInParentBranch = i_pModelTreeEntry->indexInParentBranch();
        }
        else if( i_pModelTreeEntry->entryType() != EIdxTreeEntryType::Leave )
        {
            idxInParentBranch = i_pModelTreeEntry->indexInParentBranchsChildListWithSameEntryTypes();
        }

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
                pModelTreeEntry->setModelIndexInParentBranch(idxEntry+1);
            }
            m_arpTreeEntries[idxInParentBranch] = i_pModelTreeEntry;
        }
    }

    i_pModelTreeEntry->setModelParentBranch(this);
    i_pModelTreeEntry->setModelIndexInParentBranch(idxInParentBranch);

    return idxInParentBranch;

} // add

//-----------------------------------------------------------------------------
void CModelIdxTreeEntry::remove( CModelIdxTreeEntry* i_pModelTreeEntry )
//-----------------------------------------------------------------------------
{
    int idxInParentBranch = i_pModelTreeEntry->modelIndexInParentBranch();

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

    i_pModelTreeEntry->setModelParentBranch(nullptr);
    i_pModelTreeEntry->setModelIndexInParentBranch(-1);

    for( int idxEntry = idxInParentBranch; idxEntry < m_arpTreeEntries.size(); ++idxEntry )
    {
        CModelIdxTreeEntry* pModelTreeEntry = m_arpTreeEntries[idxEntry];
        pModelTreeEntry->setModelIndexInParentBranch(idxEntry);
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

    pModelTreeEntry->setModelParentBranch(nullptr);
    pModelTreeEntry->setModelIndexInParentBranch(-1);

    for( int idxEntry = idxInParentBranch; idxEntry < m_arpTreeEntries.size(); ++idxEntry )
    {
        pModelTreeEntry = m_arpTreeEntries[idxEntry];
        pModelTreeEntry->setModelIndexInParentBranch(idxEntry);
    }

} // remove

//-----------------------------------------------------------------------------
void CModelIdxTreeEntry::onChildRenamed(
    CModelIdxTreeEntry* i_pModelTreeEntry,
    const QString&      i_strNamePrev )
//-----------------------------------------------------------------------------
{
    int idxInParentBranchPrev = i_pModelTreeEntry->modelIndexInParentBranch();

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
                idxInParentBranch = itModelEntry.value()->modelIndexInParentBranch();
                m_arpTreeEntries.append(nullptr);

                // Move all following entries one index backwards.
                for( int idxEntry = m_arpTreeEntries.size()-2; idxEntry >= idxInParentBranch; --idxEntry )
                {
                    CModelIdxTreeEntry* pModelTreeEntry = m_arpTreeEntries[idxEntry];
                    m_arpTreeEntries[idxEntry+1] = pModelTreeEntry;
                    pModelTreeEntry->setModelIndexInParentBranch(idxEntry+1);
                }
                m_arpTreeEntries[idxInParentBranch] = i_pModelTreeEntry;
            }
            i_pModelTreeEntry->setModelIndexInParentBranch(idxInParentBranch);
        }
        else if( m_sortOrder == EIdxTreeSortOrder::Descending )
        {
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
                    pModelTreeEntry->setModelIndexInParentBranch(idxEntry+1);
                }
                m_arpTreeEntries[idxInParentBranch] = i_pModelTreeEntry;
            }
            i_pModelTreeEntry->setModelIndexInParentBranch(idxInParentBranch);
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
