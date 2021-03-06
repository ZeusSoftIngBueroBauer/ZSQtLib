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

#ifndef ZSSys_IdxTree_h
#define ZSSys_IdxTree_h

#include <QtCore/qhash.h>
#include <QtCore/qobject.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysIdxTreeEntries.h"

class QMutex;

namespace ZS
{
namespace System
{
//******************************************************************************
/*! @brief Entries in the index tree are structured both in a tree structure
           and in an index-based vector.

    Each entry within the tree is uniquely identified by its key. This key consists of
    the first letter for the type of the entry (L for leave, B for branch, R for root),
    the path to the entry and the actual name of the entry.

    If an entry is added, it is also entered in a free space in a vector and thus
    receives a unique index value within the tree. Access to the entry using this index
    value is faster than using the key. As long as the entry is not removed from the tree,
    it retains this index value. In other words, if entries are removed from the tree,
    all other entries retain their place within the vector. If the removed entry was not
    at the end of the vector, the element in the vector is marked as free and transferred
    to a list of free entries. Only when the last entry of the vector is removed, i.e. the
    entry with the currently highest possible index value, is the size of the vector and
    the list of free entries corrected.

    The class is particularly suitable for those applications in which the content of the
    tree is usually not changed at runtime and the individual entries must be accessed
    quickly. This is the case, for example, with the Trace Admin objects, in which the
    tree is only expanded but never reduced after program start, since entries from this
    tree are not removed at runtime, but only the reference counter is updated.

    Other applications such as in a database in which a large number of objects
    (groups, enumerations, parameters, tables) are to be managed, with the objects generally
    being created only once when the program is started and then no longer being removed.

    The use of the index tree generally makes sense if it is not only important to be able
    to access the index value as quickly as possible, but also if no (or hardly) entries
    are removed from it at runtime. It is also used for the test library or the structuring
    of physical units.

    If the tree can be changed from different threads at runtime, access to the tree must be
    serialized and protected via a mutex. If two threads make an entry in the tree at the
    same time, this could otherwise lead to inconsistencies within the tree. A flag in the
    structure of the index tree allows this mutex to be created optionally.

    If the tree is accessed from multiple threads and a thread requests that no other thread
    make changes to the tree during the access (e.g. when iterating through the tree), the
    tree can be locked for this thread as long as necessary ( however only if the mutex was
    also created). Of course, it must then be ensured that the lock is then removed again.

    If the tree is accessed from multiple threads, but it is a tree that is only set up once
    at runtime, after which it is no longer changed but only accessed for reading, access
    don't has to be protected via a mutex and there is no need to create the mutex.
*/
class ZSSYSDLL_API CIdxTree : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::System"; }   // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CIdxTree"; }     // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // type definitions and constants
    /*! @brief Iterator for iterating through the tree entries.
        The iterator may be used either to iterate through the trees vector of entries by index or
        through the tree in preorder. */
    class ZSSYSDLL_API iterator
    {
    friend class CIdxTree;
    public:
        /*! Enumeration to define how to itereator through the tree. */
        enum ETraversalOrder {
            Index,      /*!< Iterate through the vector (index based). */
            PreOrder,   /*!< Iterate through the tree using preorder (top to bottom, first to last child index of branches). */
            Count,      /*!< Number of different iteration methods. */
            Undefined   /*!< To indicate that the iteration method is not defined. */
        };
    public:
        /*! Creates an iterator. */
        iterator() {}
    protected:
        /*! Creates an iterator.
            @param i_pIdxTree [in] Pointer to index tree.
            @param i_traversalOrder [in] Defines the iteration method. */
        iterator( CIdxTree* i_pIdxTree, CIdxTree::iterator::ETraversalOrder i_traversalOrder ) :
            m_pIdxTree(i_pIdxTree),
            m_pTreeEntryCurr(nullptr),
            m_traversalOrder(i_traversalOrder)
        {
        }
    public:
        CAbstractIdxTreeEntry* operator * () const;
        bool operator == ( const iterator& i_other ) const;
        bool operator == ( iterator& i_other ) const;
        bool operator != ( const iterator& i_other ) const;
        bool operator != ( iterator& i_other ) const;
        iterator& operator ++ ();
    private:
        CIdxTree*              m_pIdxTree = nullptr;                        /*!< Pointer to the index tree. */
        CAbstractIdxTreeEntry* m_pTreeEntryCurr = nullptr;                  /*!< Current position in the index tree. */
        ETraversalOrder        m_traversalOrder = ETraversalOrder::Index;   /*!< Iteration method. */
    };
public: // ctors and dtor
    CIdxTree(
        const QString&     i_strObjName,
        CRootIdxTreeEntry* i_pRootTreeEntry = nullptr,      // if null an instance of CRootIdxTreeEntry is created
        const QString&     i_strNodeSeparator = "/",
        bool               i_bCreateMutex = false,          // if true each access to member variables will be protected by a mutex
        QObject*           i_pObjParent = nullptr,
        int                i_iTrcDetailLevel = ZS::Trace::ETraceDetailLevelNone );
    virtual ~CIdxTree();
public: // instance methods
    void clear(); // keeps the root entry
signals:
    /*! Signal which will be emitted if the index tree is about to be destroyed.
        @param i_pIdxTree [in] Pointer to index tree. */
    void aboutToBeDestroyed( QObject* i_pIdxTree );
    /*! Signal which will be emitted if a tree entry has been added to the index tree.
        @param i_pIdxTree [in] Pointer to index tree.
        @param i_pTreeEntry [in] Pointer to tree entry which has been added. */
    void treeEntryAdded( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry );
    /*! Signal which will be emitted if a tree entry has been changed.
        If a property of a tree entry is changed the method "onTreeEntryChanged" of the index tree got to
        be invoked to inform the index tree about the change whereupon the index tree emits this signal.
        This reduces the number of signal/slot connections and avoids that the tree entry base class must
        be derived from QObject.
        @param i_pIdxTree [in] Pointer to index tree.
        @param i_pTreeEntry [in] Pointer to tree entry which has been changed. */
    void treeEntryChanged( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry );
    /*! Signal which will be emitted if a tree entry is going to be removed from the index tree.
        If this signal is emitted the entry still belongs to the index tree and the keys and indices a returned
        by the entry are still valid.
        @param i_pIdxTree [in] Pointer to index tree.
        @param i_pTreeEntry [in] Pointer to tree entry which is going to be removed. */
    void treeEntryAboutToBeRemoved( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry );
    /*! Signal which will be emitted if a tree entry has been removed from the index tree.
        If this signal is emitted the entry no longer belongs to the index tree and the keys and indices returned
        by the entry are invalid. For this the unique key and the index of the entry in the index valid before
        removing the entry from the index tree are provided as arguments with the signal.
        @param i_pIdxTree [in] Pointer to index tree.
        @param i_pTreeEntry [in] Pointer to tree entry which has been removed.
        @param i_strKeyInTree [in] Unique key of the entry valid before the entry was removed from the tree.
        @param i_idxInTree [in] Index of the entry valid before the entry was removed from the tree. */
    void treeEntryRemoved( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strKeyInTree, int i_idxInTree );
    /*! Signal which will be emitted if a tree entry is going to be moved within the index tree.
        If this signal is emitted the entry is still a child of it's current branch and does not belong yet
        to the target parent branch.
        @param i_pIdxTree [in] Pointer to index tree.
        @param i_pTreeEntry [in] Pointer to tree entry which is going to be moved to another parent branch.
        @param i_pTargetBranch [in] Pointer to target branch to which the entry will be moved. */
    void treeEntryAboutToBeMoved( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry, ZS::System::CBranchIdxTreeEntry* i_pTargetBranch );
    /*! Signal which will be emitted if a tree entry has been moved within the index tree.
        If this signal is emitted the entry already has been moved to the new target branch.
        For this the unique key and the index of the entry in tree valid before removing the entry from
        the index tree are provided as arguments with the signal.
        @param i_pIdxTree [in] Pointer to index tree.
        @param i_pTreeEntry [in] Pointer to tree entry which has been removed.
        @param i_strKeyInTreePrev [in] Unique key of the entry valid before the entry was moved to the target branch.
        @param i_pTargetBranch [in] Pointer to target branch to which the entry will be moved.
        @note The index of the entry in the index tree remains the same. */
    void treeEntryMoved( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strKeyInTreePrev, ZS::System::CBranchIdxTreeEntry* i_pTargetBranch );
    /*! Signal which will be emitted if a tree entry is going to be renamed.
        If this signal is emitted the entry still has its old name and its old unique key in the index tree.
        @param i_pIdxTree [in] Pointer to index tree.
        @param i_pTreeEntry [in] Pointer to tree entry which is going to be renamed.
        @param i_strNameNew [in] New name of the entry. */
    void treeEntryAboutToBeRenamed( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strNameNew );
    /*! Signal which will be emitted if a tree entry has been renamed.
        If this signal is emitted the entry already has its new name and a the unique key in the index tree has been changed.
        For this the unique key of the entry valid before renaming the entry is provided as an argument with the signal.
        @param i_pIdxTree [in] Pointer to index tree.
        @param i_pTreeEntry [in] Pointer to tree entry which has been removed.
        @param i_strKeyInTreePrev [in] Unique key of the entry valid before the entry was moved to the target branch.
        @param i_strNamePrev [in] Old name of the tree entry.
        @note The index of the entry in the index tree remains the same. */
    void treeEntryRenamed( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strKeyInTreePrev, const QString& i_strNamePrev );
    /*! Signal which will be emitted if the unique key of a tree entry has been changed.
        If this signal is emitted the entry already has been moved or renamed.
        For this the unique key and the index of the entry in tree valid before removing the entry from
        the index tree are provided as arguments with the signal.
        @param i_pIdxTree [in] Pointer to index tree.
        @param i_pTreeEntry [in] Pointer to tree entry which has been removed.
        @param i_strKeyInTreePrev [in] Unique key of the entry valid before the entry was moved to the target branch.
        @note The index of the entry in the index tree remains the same. */
    void treeEntryKeyInTreeChanged( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strKeyInTreePrev );
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // instance methods
    /*! Returns a pointer to the mutex used to protect the index tree if access by different threads. */
    QMutex* mutex() { return m_pMtx; }
    void lock();
    void unlock();
public: // instance methods
    /*! Returns the string used to separate the nodes within the entries path. */
    QString nodeSeparator() const { return m_strNodeSeparator; }
    /*! Returns the pointer to the root entry of the index tree. */
    CRootIdxTreeEntry* root() const { return m_pRoot; }
public: // instance methods
    QString buildPathStr( const QString& i_str1, const QString& i_str2 ) const;
    QString buildPathStr( const QString& i_str1, const QString& i_str2, const QString& i_str3 ) const;
    QString buildPathStr( const QString& i_str1, const QString& i_str2, const QString& i_str3, const QString& i_str4 ) const;
    QString buildPathStr( const QString& i_str1, const QString& i_str2, const QString& i_str3, const QString& i_str4, const QString& i_str5 ) const;
    QString buildKeyInTreeStr( EIdxTreeEntryType i_entryType, const QString& i_str1 ) const;
    QString buildKeyInTreeStr( EIdxTreeEntryType i_entryType, const QString& i_str1, const QString& i_str2 ) const;
    QString buildKeyInTreeStr( EIdxTreeEntryType i_entryType, const QString& i_str1, const QString& i_str2, const QString& i_str3 ) const;
    QString buildKeyInTreeStr( EIdxTreeEntryType i_entryType, const QString& i_str1, const QString& i_str2, const QString& i_str3, const QString& i_str4 ) const;
    QString buildKeyInTreeStr( EIdxTreeEntryType i_entryType, const QString& i_str1, const QString& i_str2, const QString& i_str3, const QString& i_str4, const QString& i_str5 ) const;
    EIdxTreeEntryType splitPathStr( const QString& i_strPath, QString* o_pstrBranchPath, QString* o_pstrName ) const;
public: // overridables (createBranch and createLeave must be overridden to create entries using a generic tree view)
    virtual CBranchIdxTreeEntry* createBranch( const QString& i_strName ) const;
    virtual CLeaveIdxTreeEntry* createLeave( const QString& i_strName ) const;
    virtual CAbstractIdxTreeEntry* createTreeEntry( EIdxTreeEntryType i_entryType, const QString& i_strName ) const;
public: // instance methods
    int treeEntriesVectorSize() const; // the number of used entries might be less
    CAbstractIdxTreeEntry* getEntry( int i_idxObj ) const;
public: // instance methods
    CBranchIdxTreeEntry* findBranch( const QString& i_strPath ) const;
    CBranchIdxTreeEntry* findBranch( const QString& i_strParentPath, const QString& i_strBranchName ) const;
    CLeaveIdxTreeEntry* findLeave( const QString& i_strPath ) const;
    CLeaveIdxTreeEntry* findLeave( const QString& i_strParentPath, const QString& i_strLeaveName ) const;
    CAbstractIdxTreeEntry* findEntry( const QString& i_strKeyInTree ) const;
public: // instance methods
    SErrResultInfo canAdd( CBranchIdxTreeEntry* i_pBranch, const QString& i_strTargetPath ) const;
    SErrResultInfo canAdd( CBranchIdxTreeEntry* i_pBranch, CBranchIdxTreeEntry* i_pTargetBranch ) const;
    SErrResultInfo canAdd( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strTargetPath ) const;
    SErrResultInfo canAdd( CLeaveIdxTreeEntry* i_pLeave, CBranchIdxTreeEntry* i_pTargetBranch ) const;
    SErrResultInfo canAdd( CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath ) const;
    SErrResultInfo canAdd( CAbstractIdxTreeEntry* i_pTreeEntry, CBranchIdxTreeEntry* i_pTargetBranch ) const;
public: // instance methods
    int add( CBranchIdxTreeEntry* i_pBranch, const QString& i_strTargetPath );
    int add( CBranchIdxTreeEntry* i_pBranch, CBranchIdxTreeEntry* i_pTargetBranch );
    int add( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strTargetPath );
    int add( CLeaveIdxTreeEntry* i_pLeave, CBranchIdxTreeEntry* i_pTargetBranch );
    int add( CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath );
    int add( CAbstractIdxTreeEntry* i_pTreeEntry, CBranchIdxTreeEntry* i_pTargetBranch );
public: // instance methods
    SErrResultInfo canInsert( CBranchIdxTreeEntry* i_pBranch, const QString& i_strTargetPath, int i_idxInTargetBranch, int i_idxInTree = -1 ) const;
    SErrResultInfo canInsert( CBranchIdxTreeEntry* i_pBranch, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch, int i_idxInTree = -1 ) const;
    SErrResultInfo canInsert( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strTargetPath, int i_idxInTargetBranch, int i_idxInTree = -1 ) const;
    SErrResultInfo canInsert( CLeaveIdxTreeEntry* i_pLeave, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch, int i_idxInTree = -1 ) const;
    SErrResultInfo canInsert( CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath, int i_idxInTargetBranch, int i_idxInTree = -1 ) const;
    SErrResultInfo canInsert( CAbstractIdxTreeEntry* i_pTreeEntry, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch, int i_idxInTree = -1 ) const;
public: // instance methods
    int insert( CBranchIdxTreeEntry* i_pBranch, const QString& i_strTargetPath, int i_idxInTargetBranch, int i_idxInTree = -1 );
    int insert( CBranchIdxTreeEntry* i_pBranch, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch, int i_idxInTree = -1 );
    int insert( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strTargetPath, int i_idxInTargetBranch, int i_idxInTree = -1 );
    int insert( CLeaveIdxTreeEntry* i_pLeave, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch, int i_idxInTree = -1 );
    int insert( CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath, int i_idxInTargetBranch, int i_idxInTree = -1 );
    int insert( CAbstractIdxTreeEntry* i_pTreeEntry, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch, int i_idxInTree = -1 );
public: // instance methods
    SErrResultInfo canRemove( CBranchIdxTreeEntry* i_pBranch ) const;
    SErrResultInfo canRemove( CLeaveIdxTreeEntry* i_pLeave ) const;
    SErrResultInfo canRemove( CAbstractIdxTreeEntry* i_pTreeEntry ) const;
    SErrResultInfo canRemove( const QString& i_strKeyInTree ) const;
public: // instance methods
    void remove( CBranchIdxTreeEntry* i_pBranch );
    void remove( CLeaveIdxTreeEntry* i_pLeave );
    void remove( CAbstractIdxTreeEntry* i_pTreeEntry );
    void remove( const QString& i_strKeyInTree );
public: // instance methods
    SErrResultInfo canMove( const QString& i_strSourcePath, const QString& i_strTargetPath, int i_idxInTargetBranch = -1 ) const;
    SErrResultInfo canMove( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strTargetPath, int i_idxInTargetBranch = -1 ) const;
    SErrResultInfo canMove( CLeaveIdxTreeEntry* i_pLeave, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1 ) const;
    SErrResultInfo canMove( CBranchIdxTreeEntry* i_pBranch, const QString& i_strTargetPath, int i_idxInTargetBranch = -1 ) const;
    SErrResultInfo canMove( CBranchIdxTreeEntry* i_pBranch, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1 ) const;
    SErrResultInfo canMove( CAbstractIdxTreeEntry* i_pTreeEntry, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1 ) const;
public: // instance methods
    void move( const QString& i_strSourcePath, const QString& i_strTargetPath, int i_idxInTargetBranch = -1 );
    void move( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strTargetPath, int i_idxInTargetBranch = -1 );
    void move( CLeaveIdxTreeEntry* i_pLeave, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1 );
    void move( CBranchIdxTreeEntry* i_pBranch, const QString& i_strTargetPath, int i_idxInTargetBranch = -1 );
    void move( CBranchIdxTreeEntry* i_pBranch, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1 );
    void move( CAbstractIdxTreeEntry* i_pTreeEntry, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1 );
public: // instance methods
    SErrResultInfo canCopy( const QString& i_strSourcePath, const QString& i_strTargetPath, int i_idxInTargetBranch = -1, int i_idxInTree = -1 ) const;
    SErrResultInfo canCopy( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strTargetPath, int i_idxInTargetBranch = -1, int i_idxInTree = -1 ) const;
    SErrResultInfo canCopy( CLeaveIdxTreeEntry* i_pLeave, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1, int i_idxInTree = -1 ) const;
    SErrResultInfo canCopy( CBranchIdxTreeEntry* i_pBranch, const QString& i_strTargetPath, int i_idxInTargetBranch = -1, int i_idxInTree = -1 ) const;
    SErrResultInfo canCopy( CBranchIdxTreeEntry* i_pBranch, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1, int i_idxInTree = -1 ) const;
    SErrResultInfo canCopy( CAbstractIdxTreeEntry* i_pTreeEntry, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1, int i_idxInTree = -1 ) const;
public: // instance methods
    int copy( const QString& i_strSourcePath, const QString& i_strTargetPath, int i_idxInTargetBranch = -1, int i_idxInTree = -1 );
    int copy( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strTargetPath, int i_idxInTargetBranch = -1, int i_idxInTree = -1 );
    int copy( CLeaveIdxTreeEntry* i_pLeave, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1, int i_idxInTree = -1 );
    int copy( CBranchIdxTreeEntry* i_pBranch, const QString& i_strTargetPath, int i_idxInTargetBranch = -1, int i_idxInTree = -1 );
    int copy( CBranchIdxTreeEntry* i_pBranch, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1, int i_idxInTree = -1 );
    int copy( CAbstractIdxTreeEntry* i_pTreeEntry, CBranchIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1, int i_idxInTree = -1 );
public: // instance methods
    SErrResultInfo canRename( const QString& i_strSourcePath, const QString& i_strNameNew ) const;
    SErrResultInfo canRename( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strNameNew ) const;
    SErrResultInfo canRename( CBranchIdxTreeEntry* i_pBranch, const QString& i_strNameNew ) const;
    SErrResultInfo canRename( CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strNameNew ) const;
public: // instance methods
    void rename( const QString& i_strSourcePath, const QString& i_strNameNew );
    void rename( CLeaveIdxTreeEntry* i_pLeave, const QString& i_strNameNew );
    void rename( CBranchIdxTreeEntry* i_pBranch, const QString& i_strNameNew );
    void rename( CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strNameNew );
protected: // instance methods
    void updateKeyInTree( CAbstractIdxTreeEntry* i_pTreeEntry );
protected: // instance methods
    void clear( CBranchIdxTreeEntry* i_pBranch );
protected: // instance methods
    //void blockIndex( int i_idxObj, bool i_bBlock = true );
public: // instance methods for testing
    /*! Returns the map with pointers to all tree entries.
        When using this method and accessing the individual elements in the map,
        special caution is required in multithreaded applications and you may need
        to lock the index tree before accessing the elements and unlock the tree afterwards. */
    QMap<QString, CAbstractIdxTreeEntry*> treeEntriesMap() const { return m_mappTreeEntries; }
    /*! Returns the vector with pointers to all tree entries.
        When using this method and accessing the individual elements in the map,
        special caution is required in multithreaded applications and you may need
        to lock the index tree before accessing the elements and unlock the tree afterwards. */
    QVector<CAbstractIdxTreeEntry*> treeEntriesVec() const { return m_arpTreeEntries; }
    /*! Returns the map with free indices in the vector of entries.
        When using this method and accessing the individual elements in the map,
        special caution is required in multithreaded applications and you may need
        to lock the index tree before accessing the elements and unlock the tree afterwards. */
    QMap<int, int> freeIdxsMap() const { return m_mapFreeIdxs; }
public: // iterator methods
    iterator begin( iterator::ETraversalOrder i_traversalOrder = iterator::ETraversalOrder::Index );
    iterator end();
public: // overridable instance methods (used by friend class CAbstractIdxTreeEntry and its derivates to avoid that the tree entry base classes must inherit QObject to emit signals)
    virtual void onTreeEntryChanged( CAbstractIdxTreeEntry* i_pTreeEntry );
protected: // instance methods (tracing of signals)
    void emit_treeEntryAdded( CIdxTree* i_pIdxTree, CAbstractIdxTreeEntry* i_pTreeEntry );
    void emit_treeEntryChanged( CIdxTree* i_pIdxTree, CAbstractIdxTreeEntry* i_pTreeEntry );
    void emit_treeEntryAboutToBeRemoved( CIdxTree* i_pIdxTree, CAbstractIdxTreeEntry* i_pTreeEntry );
    void emit_treeEntryRemoved( CIdxTree* i_pIdxTree,CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strKeyInTree, int i_idxInTree );
    void emit_treeEntryAboutToBeMoved( CIdxTree* i_pIdxTree, CAbstractIdxTreeEntry* i_pTreeEntry, CBranchIdxTreeEntry* i_pTargetBranch );
    void emit_treeEntryMoved( CIdxTree* i_pIdxTree, CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strKeyInTreePrev, CBranchIdxTreeEntry* i_pTargetBranch );
    void emit_treeEntryAboutToBeRenamed( CIdxTree* i_pIdxTree, CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strNameNew );
    void emit_treeEntryRenamed( CIdxTree* i_pIdxTree, CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strKeyInTreePrev, const QString& i_strNamePrev );
    void emit_treeEntryKeyInTreeChanged( CIdxTree* i_pIdxTree, CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strKeyInTreePrev );
protected: // instance members
    QString                               m_strNodeSeparator;   /*!< String used to seperate the node names with an entries path. */
    QMutex*                               m_pMtx;               /*!< Mutex to protect the instance if accessed by different threads. */
    QMap<QString, CAbstractIdxTreeEntry*> m_mappTreeEntries;    /*!< Map with pointers to all tree entries. */
    QVector<CAbstractIdxTreeEntry*>       m_arpTreeEntries;     /*!< Vector with pointers to all tree entries. */
    QMap<int, int>                        m_mapFreeIdxs;        /*!< Map with free indices in the vector of entries. */
    CRootIdxTreeEntry*                    m_pRoot;              /*!< Pointer to root entry. */
    int                                   m_iTrcDetailLevel;    /*!< Trace detail level for method tracing. */

}; // class CIdxTree

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_IdxTree_h
