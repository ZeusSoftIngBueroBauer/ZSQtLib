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

#ifndef ZSSys_IdxTreeEntry_h
#define ZSSys_IdxTreeEntry_h

#include <QtCore/qmap.h>
#include <QtCore/qvector.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysCommon.h"
#include "ZSSys/ZSSysErrResult.h"

class QMutex;

namespace ZS
{
namespace System
{
class CIdxTree;

enum class EIdxTreeEntryType
{
    Root    = 0,    /*! The root entry may have childrens but does not have a parent. */
    Branch  = 1,    /*! Branches may have childrens. */
    Leave   = 2,    /*! Leaves may not have childrens. */
    Count,
    Undefined
};
ZSSYSDLL_API QString idxTreeEntryType2Str( EIdxTreeEntryType i_eVal, int i_alias = EEnumEntryAliasStrName );
ZSSYSDLL_API QString idxTreeEntryType2Str( int i_iVal, int i_alias = EEnumEntryAliasStrName );
ZSSYSDLL_API EIdxTreeEntryType str2IdxTreeEntryType( const QString& i_str, int i_alias = EEnumEntryAliasStrName );


//******************************************************************************
/*! @brief Base class for all entries in the index tree.

    This base class is used for both branch as well as leave entries.
    A flag indicates whether an entry may have children and is a branch.
    A leave may not have childrens.
    The root entry may not have a parent but children.

    A branch entry keeps its child branches and leaves in the order in which they
    are inserted. Models implemented to indicate the content of the index tree
    must provide features to sort the child entries. Sorting may be e.g.
    - unsorted (in the original order they have been added to the branch)
    - logically (child branches before child leaves both sorted alphabetically)
    - alphabetically (child branches and child leaves in mixed order).
    Please note that a branch may have two childs with the same name but with
    different entry types (one must be a branch and the other one must be leave).
*/
class ZSSYSDLL_API CIdxTreeEntry
//******************************************************************************
{
    friend class CIdxTree;
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::System"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CIdxTreeEntry"; }
public: // ctors
    CIdxTreeEntry( EIdxTreeEntryType i_entryType, const QString& i_strName );
    CIdxTreeEntry( const CIdxTreeEntry& i_other );
public: // dtor
    virtual ~CIdxTreeEntry();
public: // instance methods
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // overridable instance methods
    /*! Virtual method which must be reimplemented by derived classes to
        create an instance of the derived class. */
    virtual CIdxTreeEntry* clone() const;
public: // instance methods
    /*! Returns the entry type of the index tree entry. */
    EIdxTreeEntryType entryType() const { return m_entryType; }
    /*! Returns true if entry is the root entry, false otherwise. */
    bool isRoot() const { return (m_entryType == EIdxTreeEntryType::Root); }
    /*! Returns true if entry is a branch entry, false otherwise. */
    bool isBranch() const { return (m_entryType == EIdxTreeEntryType::Branch); }
    /*! Returns true if entry is a leave entry, false otherwise. */
    bool isLeave() const { return (m_entryType == EIdxTreeEntryType::Leave); }
    QString entryTypeSymbol() const;
    /*! Returns the entry type's string representation. */
    QString entryType2Str( int i_alias = EEnumEntryAliasStrName ) const;
    /*! Returns the name of the index tree entry (e.g. "Volt". */
    QString name() const { return m_strName; }
    QString path() const;
public: // instance methods
    /*! Returns the index tree of the entry. */
    CIdxTree* tree() const { return m_pTree; }
    /*! Returns the unique key of the entry within the index tree (e.g. "L:ZS::Data::CDataTable::Customers". */
    QString keyInTree() const;
    /*! Returns the index of the entry within the index trees vector of entries. */
    int indexInTree() const { return m_idxInTree; }
public: // instance methods
    /*! Returns the parent branch of the entry. */
    CIdxTreeEntry* parentBranch() const { return m_pParentBranch; }
    QString parentBranchName() const;
    QString parentBranchPath() const; // excluding root and excluding name (e.g. "ZS::Data::CDataTable")
    /*! Returns the unique key of the entry within the parent branch (e.g. "L:Customers"). */
    QString keyInParentBranch() const { return m_strKeyInParentBranch; }
    /*! Returns the index of the entry within it's parent branches vector of child entries. */
    int indexInParentBranch() const { return m_idxInParentBranch; }
public: // instance methods
    bool isChildOf( CIdxTreeEntry* i_pBranch ) const;
public: // instance methods (applying filter)
    int indexInParentBranchsChildListWithSameEntryTypes() const;
public: // instance methods
    QString getCalculatedKeyInTree() const;
public: // instance methods
    bool isAboutToBeDestroyed() const;
public: // operators (for branch entries)
    /*! Returns the pointer to the child object at the given index.
        @param i_idx [in] Index of the child entry.
        @return Pointer to child entry. */
    CIdxTreeEntry* operator [] ( int i_idx ) { return m_arpTreeEntries[i_idx]; }
    /*! Returns the pointer to the child object at the given index.
        @param i_idx [in] Index of the child entry.
        @return Pointer to child entry. */
    const CIdxTreeEntry* operator [] ( int i_idx ) const { return m_arpTreeEntries[i_idx]; }
public: // instance methods (for branch entries)
    /*! Returns the number of child entries. */
    int count() const { return m_arpTreeEntries.size(); }
    /*! Returns the number of child entries (same as count). */
    int length() const { return m_arpTreeEntries.size(); }
    /*! Returns the number of child entries (same as count). */
    int size() const { return m_arpTreeEntries.size(); }
    /*! Returns the child entry at the given index. */
    CIdxTreeEntry* at( int i_idx ) const { return m_arpTreeEntries.at(i_idx); }
public: // instance methods (for branch entries)
    int indexOf( const CIdxTreeEntry* i_pChildTreeEntry ) const;
    int indexOf( const QString& i_strKeyInParentBranch ) const;
    int indexOf( EIdxTreeEntryType i_entryType, const QString& i_strName ) const;
    CIdxTreeEntry* find( const QString& i_strKeyInParentBranch ) const;
    CIdxTreeEntry* find( EIdxTreeEntryType i_entryType, const QString& i_strName ) const;
public: // instance methods  (for branch entries) (applying filter)
    int indexOfChildInListWithSameEntryTypes( const CIdxTreeEntry* i_pChildTreeEntry ) const;
protected: // overridables (for branch entries)
    virtual int addChild( CIdxTreeEntry* i_pChildTreeEntry );
    virtual int insertChild( int i_iIdx, CIdxTreeEntry* i_pChildTreeEntry );
    virtual void removeChild( CIdxTreeEntry* i_pChildTreeEntry );
    virtual void renameChild( CIdxTreeEntry* i_pChildTreeEntry, const QString& i_strNameNew );
protected: // overridables
    virtual void setName( const QString& i_strName );
    virtual void setKeyInTree( const QString& i_strKey );
    virtual void setIndexInTree( int i_idx );
protected: // overridables
    virtual void setParentBranch( CIdxTreeEntry* i_pParent );
    virtual void setKeyInParentBranch( const QString& i_strKey );
    virtual void setIndexInParentBranch( int i_idx );
protected: // instance methods
    void setTree( CIdxTree* i_pTree );
protected: // instance members
    EIdxTreeEntryType m_entryType;               /*!< The type may be either Root, Branch or Leave.
                                                      Leave entries may not have children.
                                                      A root entry may not have a parent. */
    QString           m_strName;                 /*!< Name of node (object name). */
    QMutex*           m_pMtx;                    /*!< Mutex to protect the instance if several threads may access it (may be nullptr if not used).
                                                      If you want to protect the index tree entry via this mutex you must derive your own class
                                                      for the tree entry and create the mutex in the derived classes constructor. */
    CIdxTree*         m_pTree;                   /*!< Reference to index tree. */
    QString           m_strKeyInTree;            /*!< Unique key in the index tree: composed of "<EntryTypeSymbol>:<ParentPath>/<Name>" (e.g. "L:ZS::Data::CDataTable::Customers"). */
    int               m_idxInTree;               /*!< Index of this entry in the tree's vector of entries ("global tree index or object Id"). */
    CIdxTreeEntry*    m_pParentBranch;           /*!< Reference to the parent branch. */
    QString           m_strKeyInParentBranch;    /*!< Unique key of the entry in the parent branch composed of "<EntryTypeSymbol>:<Name>" (e.g. "L:Customers"). */
    int               m_idxInParentBranch;       /*!< Index of this entry in this parent branch's vector of child entries ("local branch index"). */
    bool              m_bIsAboutToBeDestroyed;   /*!< True if the destructor is active. */
protected: // instance members
    QMap<QString, CIdxTreeEntry*> m_mappTreeEntries;   /*!< Map with all child entries of the branch.
                                                            Key is: "<EntryTypeSymbol>:<Name>" (e.g. "L:Customers") */
    QVector<CIdxTreeEntry*>       m_arpTreeEntries;    /*!< Vector with all child entries of the branch. */

}; // class CIdxTreeEntry


} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_IdxTreeEntry_h
