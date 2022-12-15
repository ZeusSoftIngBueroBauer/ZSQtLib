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

#ifndef ZSSysGUI_IdxTreeModelEntry_h
#define ZSSysGUI_IdxTreeModelEntry_h

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"

namespace ZS
{
namespace System
{
class CIdxTreeEntry;

namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CModelIdxTreeEntry
/* The index of the child entries (returned by methods as "at", "indexOf"
 * operator [], ..) depend on the selected sort order.
*******************************************************************************/
{
    friend class CModelIdxTree;
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CModelIdxTreeEntry"; }
public: // ctors
    CModelIdxTreeEntry( CIdxTreeEntry* i_pTreeEntry );
public: // dtor
    virtual ~CModelIdxTreeEntry();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    CIdxTreeEntry* treeEntry();
public: // instance methods
    EIdxTreeEntryType entryType() const;
    QString entryType2Str( int i_alias = ZS::System::EEnumEntryAliasStrName ) const;
    void setKeyInTree( const QString& i_strNewKeyInTree );
    QString keyInTree() const;
    int indexInTree() const;
    QString name() const;
    QString path() const;
public: // instance methods
    QString keyInParentBranch() const;
    int indexInParentBranch() const;
    int indexInParentBranchsChildListWithSameEntryTypes() const;
public: // instance methods
    CModelIdxTreeEntry* modelParentBranch() const { return m_pParentBranch; }
    int modelIndexInParentBranch() const { return m_idxInParentBranch; }
public: // instance methods (applying filter)
    int modelIndexInParentBranchsChildListWithSameEntryTypes() const;
public: // instance methods
    void setIsSelected( bool i_bIsSelected );
    bool isSelected() const { return m_bIsSelected; }
public: // instance methods
    void setModelParentBranch( CModelIdxTreeEntry* i_pParent ) { m_pParentBranch = i_pParent; }
    void setModelIndexInParentBranch( int i_idx ) { m_idxInParentBranch = i_idx; }
public: // instance methods
    void setExcludeLeaves( bool i_bExcludeLeaves );
    bool areLeavesExluded() const { return m_bExcludeLeaves; }
public: // instance methods
    void setSortOrder( EIdxTreeSortOrder i_sortOrder );
    EIdxTreeSortOrder sortOrder() const { return m_sortOrder; }
public: // operators
    CModelIdxTreeEntry* operator [] ( int i_idx ) { return m_arpTreeEntries[i_idx]; }
    const CModelIdxTreeEntry* operator [] ( int i_idx ) const { return m_arpTreeEntries[i_idx]; }
public: // instance methods
    int count() const { return m_arpTreeEntries.size(); }
    int length() const { return m_arpTreeEntries.size(); }
    int size() const { return m_arpTreeEntries.size(); }
    CModelIdxTreeEntry* at( int i_idx ) const { return m_arpTreeEntries.at(i_idx); }
public: // instance methods
    int modelIndexOf( CModelIdxTreeEntry* i_pModelTreeEntry ) const { return m_arpTreeEntries.indexOf(i_pModelTreeEntry); }
public: // instance methods (applying filter)
    int modelIndexOfChildInListWithSameEntryTypes( const CModelIdxTreeEntry* i_pModelTreeEntry ) const;
public: // instance methods
    CModelIdxTreeEntry* findModelBranch( const QString& i_strName );
    CModelIdxTreeEntry* findModelLeave( const QString& i_strName );
    CModelIdxTreeEntry* findModelEntry( EIdxTreeEntryType i_entryType, const QString& i_strName );
public: // instance methods
    int add( CModelIdxTreeEntry* i_pModelTreeEntry );
    void remove( CModelIdxTreeEntry* i_pModelTreeEntry );
    void remove( const QString& i_strKeyInParentBranch );
protected:
    void onChildRenamed( CModelIdxTreeEntry* i_pModelTreeEntry, const QString& i_strNamePrev );
public: // instance methods
    void setIsExpanded( bool i_bIsExpanded );
    bool isExpanded() const { return m_bIsExpanded; }
protected: // instance methods (index tree entry properties)
    CIdxTree*           m_pIdxTree;
    EIdxTreeEntryType   m_entryType;
    QString             m_strKeyInTree;
    int                 m_idxInTree;
protected: // instance methods (model entry properties)
    CModelIdxTreeEntry* m_pParentBranch;
    int                 m_idxInParentBranch;      // Index of this entry in this parent branch's vector of child entries ("local branch index").
    bool                m_bIsSelected;
protected: // instance members
    bool                               m_bExcludeLeaves;
    EIdxTreeSortOrder                  m_sortOrder;
    QMap<QString, CModelIdxTreeEntry*> m_mappModelTreeEntries;   // Key is KeyInParentBranch: <EntryTypeSymbol>:<Name> (e.g. "B:A0::B2::C3", "L:A0::B2::o1")
    QVector<CModelIdxTreeEntry*>       m_arpTreeEntries;
    bool                               m_bIsExpanded;

}; // class CModelIdxTreeEntry


} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_IdxTreeModel_h
