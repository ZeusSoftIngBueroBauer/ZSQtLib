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

#ifndef ZSSys_IdxTreeEntries_h
#define ZSSys_IdxTreeEntries_h

#include <QtCore/qobject.h>
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
class CBranchIdxTreeEntry;

enum class EIdxTreeEntryType
{
    Root    = 0,
    Branch  = 1,
    Leave   = 2,
    Count,
    Undefined
};
ZSSYSDLL_API QString idxTreeEntryType2Str( EIdxTreeEntryType i_eVal, int i_alias = ZS::System::EEnumEntryAliasStrName );
ZSSYSDLL_API QString idxTreeEntryType2Str( int i_iVal, int i_alias = ZS::System::EEnumEntryAliasStrName );
ZSSYSDLL_API EIdxTreeEntryType str2IdxTreeEntryType( const QString& i_str, int i_alias = ZS::System::EEnumEntryAliasStrName );


//******************************************************************************
/*! @brief Abstract base class for branches and leaves of the index tree.
*/
class ZSSYSDLL_API CAbstractIdxTreeEntry
//******************************************************************************
{
    friend class CIdxTree;
    friend class CBranchIdxTreeEntry;
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::System"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CAbstractIdxTreeEntry"; }
protected: // ctors
    CAbstractIdxTreeEntry( EIdxTreeEntryType i_entryType, const QString& i_strName );
    CAbstractIdxTreeEntry( const CAbstractIdxTreeEntry& i_other );
public: // dtor
    virtual ~CAbstractIdxTreeEntry();
public: // instance methods
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // must overridables
    /*! Pure virtual method which must be reimplemented by derived classes to
        create an instance of the desired entry type. */
    virtual CAbstractIdxTreeEntry* clone() const = 0;
public: // instance methods
    /*! Returns the entry type of the index tree entry. */
    EIdxTreeEntryType entryType() const { return m_entryType; }
    QString entryTypeSymbol() const;
    /*! Returns the entry type's string representation. */
    QString entryType2Str( int i_alias = ZS::System::EEnumEntryAliasStrName ) const;
    /*! Returns the name of the index tree entry (e.g. "Volt". */
    QString name() const { return m_strName; }
    QString path() const;
public: // instance methods
    /*! Returns the index tree of the entry. */
    CIdxTree* tree() const { return m_pTree; }
    QString keyInTree() const;
    /*! Returns the index of the entry within the index trees vector of entries. */
    int indexInTree() const { return m_idxInTree; }
    /*! Returns the unique key of the entry within the index tree (e.g. "L:ZS::Data::CDataTable::Customers". */
    QString objectName() const { return keyInTree(); }
public: // instance methods
    /*! Returns the parent branch of the entry. */
    CBranchIdxTreeEntry* parentBranch() const { return m_pParentBranch; }
    QString parentBranchName() const;
    QString parentBranchPath() const; // excluding root and excluding name (e.g. "ZS::Data::CDataTable")
    /*! Returns the unique key of the entry within the parent branch (e.g. "L:Customers"). */
    QString keyInParentBranch() const { return m_strKeyInParentBranch; }
    /*! Returns the index of the entry within it's parent branches vector of child entries. */
    int indexInParentBranch() const { return m_idxInParentBranch; }
public: // instance methods
    bool isChildOf( CBranchIdxTreeEntry* i_pBranch ) const;
public: // instance methods (applying filter)
    int indexInParentBranchsChildListWithSameEntryTypes() const;
public: // instance methods
    QString getCalculatedKeyInTree() const;
public: // instance methods
    bool isAboutToBeDestroyed() const;
protected: // instance methods
    /*! Sets the current name of the tree entry. As changing the name also changes its key also setKeyInTree and setKeyInParentBranch must be invoked. */
    void setName( const QString& i_strName ) { m_strName = i_strName; }
    /*! Sets the index tree the entry belongs to. */
    void setTree( CIdxTree* i_pTree ) { m_pTree = i_pTree; }
    /*! Sets the key of the entry in the index tree. */
    void setKeyInTree( const QString& i_strKey ) { m_strKeyInTree = i_strKey; }
    /*! Sets the index of the entry in the index trees vector of entries. */
    void setIndexInTree( int i_idx ) { m_idxInTree = i_idx; }
protected: // instance methods
    /*! Sets the parent branch of the entry. */
    void setParentBranch( CBranchIdxTreeEntry* i_pParent ) { m_pParentBranch = i_pParent; }
    /*! Sets the unique key of the entry within it's parent branch. */
    void setKeyInParentBranch( const QString& i_strKey ) { m_strKeyInParentBranch = i_strKey; }
    /*! Sets the index of the entry within it's parent branches vector of entries. */
    void setIndexInParentBranch( int i_idx ) { m_idxInParentBranch = i_idx; }
protected: // instance members
    EIdxTreeEntryType    m_entryType;               /*!< The type may be either Root, Branch or Leave. */
    QString              m_strName;                 /*!< Name of node (object name). */
    QMutex*              m_pMtx;                    /*!< Mutex to protect the instance if several threads may access it (may be nullptr if not used).
                                                         If you want to protect the index tree entry via this mutex you must derive your own class
                                                         for the tree entry and create the mutex in the derived classes constructor. */
    CIdxTree*            m_pTree;                   /*!< Reference to index tree. */
    QString              m_strKeyInTree;            /*!< Unique key in the index tree: composed of "<EntryTypeSymbol>:<ParentPath>/<Name>" (e.g. "L:ZS::Data::CDataTable::Customers"). */
    int                  m_idxInTree;               /*!< Index of this entry in the tree's vector of entries ("global tree index or object Id"). */
    CBranchIdxTreeEntry* m_pParentBranch;           /*!< Reference to the parent branch. */
    QString              m_strKeyInParentBranch;    /*!< Unique key of the entry in the parent branch composed of "<EntryTypeSymbol>:<Name>" (e.g. "L:Customers"). */
    int                  m_idxInParentBranch;       /*!< Index of this entry in this parent branch's vector of child entries ("local branch index"). */
    bool                 m_bIsAboutToBeDestroyed;   /*!< True if the destructor is active. */

}; // class CAbstractIdxTreeEntry


//******************************************************************************
/*! @brief Class for branches of the index tree.

    The branch keeps its child branches and leaves in the order in which they
    are inserted. Models implemented to indicate the content of the index tree
    must provide features to sort the child entries. Sorting may be e.g.
    - unsorted (in the original order they have been added to the branch)
    - logically (child branches before child leaves both sorted alphabetically)
    - alphabetically (child branches and child leaves in mixed order).
    Please note that a branch may have two childs with the same name but with
    different entry types (one must be a branch and the other one must be leave).
*/
class ZSSYSDLL_API CBranchIdxTreeEntry : public CAbstractIdxTreeEntry
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::System"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CBranchIdxTreeEntry"; }
public: // ctors and dtor
    CBranchIdxTreeEntry( const QString& i_strName );
    CBranchIdxTreeEntry( const CBranchIdxTreeEntry& i_other );
    virtual ~CBranchIdxTreeEntry();
protected: // ctor for class CRootIdxTreeEntry
    CBranchIdxTreeEntry( EIdxTreeEntryType i_entryType, const QString& i_strName );
public: // instance methods
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const override { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const override { return ClassName(); }
public: // must overridables of base class CAbstractIdxTreeEntry
    virtual CAbstractIdxTreeEntry* clone() const override;
public: // operators
    /*! Returns the pointer to the child object at the given index.
        @param i_idx [in] Index of the child entry.
        @return Pointer to child entry. */
    CAbstractIdxTreeEntry* operator [] ( int i_idx ) { return m_arpTreeEntries[i_idx]; }
    /*! Returns the pointer to the child object at the given index.
        @param i_idx [in] Index of the child entry.
        @return Pointer to child entry. */
    const CAbstractIdxTreeEntry* operator [] ( int i_idx ) const { return m_arpTreeEntries[i_idx]; }
public: // instance methods
    /*! Returns the number of child entries. */
    int count() const { return m_arpTreeEntries.size(); }
    /*! Returns the number of child entries (same as count). */
    int length() const { return m_arpTreeEntries.size(); }
    /*! Returns the number of child entries (same as count). */
    int size() const { return m_arpTreeEntries.size(); }
    /*! Returns the child entry at the given index. */
    CAbstractIdxTreeEntry* at( int i_idx ) const { return m_arpTreeEntries.at(i_idx); }
public: // instance methods
    int indexOf( const CAbstractIdxTreeEntry* i_pTreeEntry ) const;
    int indexOf( const QString& i_strKeyInParentBranch ) const;
    int indexOf( EIdxTreeEntryType i_entryType, const QString& i_strName ) const;
public: // instance methods (applying filter)
    int indexOfChildInListWithSameEntryTypes( const CAbstractIdxTreeEntry* i_pTreeEntry ) const;
public: // instance methods
    int add( CAbstractIdxTreeEntry* i_pTreeEntry );
    int insert( int i_iIdx, CAbstractIdxTreeEntry* i_pTreeEntry );
    void remove( CAbstractIdxTreeEntry* i_pTreeEntry );
    void rename( CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strNameNew );
protected: // instance members
    QMap<QString, CAbstractIdxTreeEntry*> m_mappTreeEntries;   /*!< Map with all child entries of the branch.
                                                                    Key is: "<EntryTypeSymbol>:<Name>" (e.g. "L:Customers") */
    QVector<CAbstractIdxTreeEntry*>       m_arpTreeEntries;    /*!< Vector with all child entries of the branch. */

}; // class CBranchIdxTreeEntry


//******************************************************************************
/*! @brief Class for the root entry of the index tree.
*/
class ZSSYSDLL_API CRootIdxTreeEntry : public CBranchIdxTreeEntry
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::System"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CRootIdxTreeEntry"; }
public: // ctors and dtor
    CRootIdxTreeEntry( const QString& i_strName );
    CRootIdxTreeEntry( const CRootIdxTreeEntry& i_other );
    virtual ~CRootIdxTreeEntry();
public: // instance methods
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const override { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const override { return ClassName(); }
public: // must overridables of base class CAbstractIdxTreeEntry
    virtual CAbstractIdxTreeEntry* clone() const override;

}; // class CRootIdxTreeEntry


//******************************************************************************
/*! @brief Class for leave entries of the index tree.
*/
class ZSSYSDLL_API CLeaveIdxTreeEntry : public CAbstractIdxTreeEntry
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::System"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CLeaveIdxTreeEntry"; }
public: // ctors and dtor
    CLeaveIdxTreeEntry( const QString& i_strName );
    CLeaveIdxTreeEntry( const CLeaveIdxTreeEntry& i_other );
    virtual ~CLeaveIdxTreeEntry();
public: // instance methods
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const override { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const override { return ClassName(); }
public: // must overridables of base class CAbstractIdxTreeEntry
    virtual CAbstractIdxTreeEntry* clone() const override;

}; // class CLeaveIdxTreeEntry

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_IdxTreeEntries_h
