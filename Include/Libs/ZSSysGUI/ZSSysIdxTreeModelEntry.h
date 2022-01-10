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
    bool isAboutToBeDestroyed() const;
public: // instance methods
    CIdxTreeEntry* treeEntry() { return m_pTreeEntry; }
public: // instance methods
    EIdxTreeEntryType entryType() const { return m_pTreeEntry->entryType(); }
    QString entryType2Str( int i_alias = ZS::System::EEnumEntryAliasStrName ) const { return m_pTreeEntry->entryType2Str(i_alias); }
    QString name() const { return m_pTreeEntry->name(); }
    QString path() const { return m_pTreeEntry->path(); }
public: // instance methods
    CIdxTree* tree() const { return m_pTreeEntry->tree(); }
    QString keyInTree() const { return m_pTreeEntry->keyInTree(); }
    int indexInTree() const { return m_pTreeEntry->indexInTree(); }
public: // instance methods
    CIdxTreeEntry* parentBranch() const { return m_pTreeEntry->parentBranch(); }
    QString keyInParentBranch() const { return m_pTreeEntry->keyInParentBranch(); }
    int indexInParentBranch() const { return m_pTreeEntry->indexInParentBranch(); }
public: // instance methods
    CModelIdxTree* model() const { return m_pModel; }
    CModelIdxTreeEntry* modelParentBranch() const { return m_pParentBranch; }
    int modelIndexInParentBranch() const { return m_idxInParentBranch; }
public: // instance methods (applying filter)
    int indexInParentBranchsChildListWithSameEntryTypes() const { return m_pTreeEntry->indexInParentBranchsChildListWithSameEntryTypes(); }
    int modelIndexInParentBranchsChildListWithSameEntryTypes() const;
public: // instance methods
    QString getCalculatedKeyInModel() const;
    QString getCalculatedKeyInTree() const;
public: // instance methods
    void setIsSelected( bool i_bIsSelected );
    bool isSelected() const { return m_bIsSelected; }
public: // instance methods
    void setModel( CModelIdxTree* i_pModel ) { m_pModel = i_pModel; }
    void setModelParentBranch( CModelIdxTreeEntry* i_pParent ) { m_pParentBranch = i_pParent; }
    void setModelIndexInParentBranch( int i_idx ) { m_idxInParentBranch = i_idx; }
public: // instance methods
    void setFilter( EIdxTreeEntryType i_entryType );
    EIdxTreeEntryType getFilter() const { return m_entryTypeFilter; }
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
    int modelIndexOf( CModelIdxTreeEntry* i_pTreeEntry ) const { return m_arpTreeEntries.indexOf(i_pTreeEntry); }
public: // instance methods (applying filter)
    int modelIndexOfChildInListWithSameEntryTypes( const CModelIdxTreeEntry* i_pTreeEntry ) const;
public: // instance methods
    CModelIdxTreeEntry* findModelBranch( const QString& i_strName );
    CModelIdxTreeEntry* findModelLeave( const QString& i_strName );
    CModelIdxTreeEntry* findModelEntry( EIdxTreeEntryType i_entryType, const QString& i_strName );
public: // instance methods
    int add( CModelIdxTreeEntry* i_pTreeEntry );
    void remove( CModelIdxTreeEntry* i_pTreeEntry );
    void remove( const QString& i_strKeyInParentBranch );
    void rename( const QString& i_strKeyInParentBranchOrig, const QString& i_strNameNew );
public: // instance methods
    void setIsExpanded( bool i_bIsExpanded );
    bool isExpanded() const { return m_bIsExpanded; }
protected: // instance members
    CIdxTreeEntry*      m_pTreeEntry;
    CModelIdxTree*      m_pModel;
    CModelIdxTreeEntry* m_pParentBranch;
    int                 m_idxInParentBranch;      // Index of this entry in this parent branch's vector of child entries ("local branch index").
    bool                m_bIsSelected;
protected: // instance members
    EIdxTreeEntryType                  m_entryTypeFilter;
    EIdxTreeSortOrder                  m_sortOrder;
    QMap<QString, CModelIdxTreeEntry*> m_mappModelTreeEntries;   // Key is KeyInParentBranch: <EntryTypeSymbol>:<Name> (e.g. "B:A0::B2::C3", "L:A0::B2::o1")
    QVector<CModelIdxTreeEntry*>       m_arpTreeEntries;
    bool                               m_bIsExpanded;

}; // class CModelIdxTreeEntry


} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_IdxTreeModel_h
