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
public: // type definitions and constants
    enum class EEntryType
    {
        Root    = 0,    /*! The root entry may have childrens but does not have a parent. */
        Branch  = 1,    /*! Branches may have childrens. */
        Leave   = 2,    /*! Leaves may not have childrens. */
        Count,
        Undefined
    };
    static QString entryType2Str( EEntryType i_entryType, int i_alias = EEnumEntryAliasStrName );
    static EEntryType str2EntryType( const QString& i_strEntryType, int i_alias = EEnumEntryAliasStrUndefined );
public: // ctors and dtor
    CIdxTreeEntry( EEntryType i_entryType, const QString& i_strName );
    CIdxTreeEntry( const CIdxTreeEntry& i_other );
    virtual ~CIdxTreeEntry();
protected: // destructor
    void clear();
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
    /*! Returns true if entry is the root entry, false otherwise. */
    virtual bool isRoot() const { return (m_entryType == EEntryType::Root); }
    /*! Returns true if entry is a branch entry, false otherwise.
        Must be overrided by derived class if more than just on branch
        type should be supported and the entry type is something different
        than EEntryType::Branch. */
    virtual bool isBranch() const { return (m_entryType == EEntryType::Branch); }
    /*! Returns true if entry is a leave entry, false otherwise.
        Must be overrided by derived class if more than just on leave
        type should be supported and the entry type is something different
        than EEntryType::Leave. */
    virtual bool isLeave() const { return (m_entryType == EEntryType::Leave); }
    /*! Returns the entry type's string representation as a short symbol.
        Must be overrided by derived class if more than just on branch or
        leave type should be supported and the entry type is something different
        than EEntryType::Branch or EEntryType::Leave. */
    virtual QString entryTypeSymbol() const;
    /*! Returns the entry type's string representation.
        Must be overrided by derived class if more than just on branch or
        leave type should be supported and the entry type is something different
        than EEntryType::Branch or EEntryType::Leave. */
    virtual QString entryType2Str( int i_alias = EEnumEntryAliasStrName ) const;
public: // instance methods
    /*! Returns the name of the tree entry (e.g. "Volt". */
    QString name() const { return m_strName; }
    /*! Returns the path (incl. name, excluding the name of the root entry)
        of the tree entry (e.g. "Voltage::Volt)". */
    QString path() const;
public: // instance methods
    /*! Returns the index tree of the entry. */
    CIdxTree* tree() const { return m_pTree; }
    /*! Returns the node separator of the index tree (e.g. ".", "::", "/", etc.). */
    QString nodeSeparator() const;
    /*! Returns the unique key of the tree entry.
        This includes the Entry type followed by ":" and the path of the entry.
        E.g. "L:Voltage.Volt". */
    QString keyInTree() const;
    /*! Returns the index of the entry within vector of tree entries. */
    int indexInTree() const { return m_idxInTree; }
public: // instance methods
    /*! Returns the parent branch of the entry. */
    CIdxTreeEntry* parentBranch() const { return m_pParentBranch; }
    QString parentBranchName() const;
    QString parentBranchPath() const; // excluding root and excluding name (e.g. "ZS::Data::CDataTable")
    QString parentBranchKeyInTree() const;
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
    int indexOf( const QString& i_strEntryTypeSymbol, const QString& i_strName ) const;
    CIdxTreeEntry* find( const QString& i_strKeyInParentBranch ) const;
    CIdxTreeEntry* find( const QString& i_strEntryTypeSymbol, const QString& i_strName ) const;
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
    void addShortcut( const QString& i_strUniqueName );
    void removeShortcut( const QString& i_strUniqueName );
    QStringList shortcuts() const { return m_strlstShortcuts; }
    void setTree( CIdxTree* i_pTree );
public: // class methods
    /*! Converts the base entry type into its string representation. */
    static QString entryType2Str( EEntryType i_entryType, int i_alias = EEnumEntryAliasStrName );
protected: // instance members
    /*!< The type may be either Root, Branch or Leave.
         Leave entries may not have children.
         A root entry may not have a parent. */
    EEntryType m_entryType;
    /*!< Name of node. */
    QString m_strName;
    /*!< Mutex to protect the instance if several threads may access it
         (may be nullptr if not used). If you want to protect the index tree entry via
         this mutex you must derive your own class for the tree entry and create the
         mutex in the derived classes constructor. */
    QMutex* m_pMtx;
    /*!< Reference to index tree. */
    CIdxTree* m_pTree;
    /*!< Unique key in the index tree:
         composed of "<EntryTypeSymbol>:<ParentPath>/<Name>"
         (e.g. "L:ZS::Data::CDataTable::Customers"). */
    QString m_strKeyInTree;
    /*!< Index of this entry in the tree's vector of entries
         ("global tree index or object Id"). */
    int m_idxInTree;
    /*!< For faster or easier access to the index tree entry
         unique names can be addes as shortcuts to the index tree.
         The shortcuts are stored in the tree entry so that they
         can be fastly removed (without scanning the whole shortcut map)
         from the index tree together with the tree entry. */
    QStringList m_strlstShortcuts;
    /*!< Reference to the parent branch. */
    CIdxTreeEntry* m_pParentBranch;
    /*!< Unique key of the entry in the parent branch composed of
         "<EntryTypeSymbol>:<Name>" (e.g. "L:Customers"). */
    QString m_strKeyInParentBranch;
    /*!< Index of this entry in this parent branch's vector
         of child entries ("local branch index"). */
    int m_idxInParentBranch;
    /*!< True if the destructor is active. */
    bool m_bIsAboutToBeDestroyed;
    /*!< Map with all child entries of the branch.
         Key is: "<EntryTypeSymbol>:<Name>" (e.g. "L:Customers") */
    QMap<QString, CIdxTreeEntry*> m_mappTreeEntries;
    /*!< Vector with all child entries of the branch. */
    QVector<CIdxTreeEntry*> m_arpTreeEntries;

}; // class CIdxTreeEntry


} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_IdxTreeEntry_h
