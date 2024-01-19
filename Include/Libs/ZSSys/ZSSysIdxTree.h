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

#ifndef ZSSys_IdxTree_h
#define ZSSys_IdxTree_h

#include <QtCore/qhash.h>
#include <QtCore/qobject.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysIdxTreeEntry.h"

namespace ZS
{
namespace System
{
class CRecursiveMutex;
class CTrcAdminObj;
class CTrcMthFile;

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
    static QString NameSpace() { return "ZS::System"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CIdxTree"; }
public: // type definitions and constants
    /*! @brief Iterator for iterating through the tree entries.
        The iterator may be used either to iterate through the trees vector of entries by index or
        through the tree in preorder.
        @note If the index tree is modified by different threads you need to lock the
              index tree when iterating through the entries. */
    class ZSSYSDLL_API iterator
    {
    friend class CIdxTree;
    public:
        /*! Enumeration to define how to iterate through the tree. */
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
        CIdxTreeEntry* operator * () const;
        bool operator == ( const iterator& i_other ) const;
        bool operator == ( iterator& i_other ) const;
        bool operator != ( const iterator& i_other ) const;
        bool operator != ( iterator& i_other ) const;
        iterator& operator ++ ();
    private:
        CIdxTree*       m_pIdxTree = nullptr;                        /*!< Pointer to the index tree. */
        CIdxTreeEntry*  m_pTreeEntryCurr = nullptr;                  /*!< Current position in the index tree. */
        ETraversalOrder m_traversalOrder = ETraversalOrder::Index;   /*!< Iteration method. */
    };
public: // ctors and dtor
    CIdxTree(
        const QString& i_strObjName,
        CIdxTreeEntry* i_pRootTreeEntry = nullptr,
        const QString& i_strNodeSeparator = "/",
        bool           i_bCreateMutex = false,
        QObject*       i_pObjParent = nullptr,
        EMethodTraceDetailLevel i_eTrcDetailLevel = EMethodTraceDetailLevel::None,
        EMethodTraceDetailLevel i_eTrcDetailLevelMutex = EMethodTraceDetailLevel::None );
    virtual ~CIdxTree();
public: // instance methods
    void clear(); // keeps the root entry
signals:
    /*! Signal which will be emitted if the index tree is about to be destroyed. */
    void aboutToBeDestroyed();
    /*! Signal which will be emitted if a tree entry has been added to the index tree.
        @param i_strKeyInTree [in] Key in index tree of tree entry which has been added. */
    void treeEntryAdded( const QString& i_strKeyInTree );
    /*! Signal which will be emitted if a tree entry has been changed.
        If a property of a tree entry is changed the method "onTreeEntryChanged" of the index tree got to
        be invoked to inform the index tree about the change whereupon the index tree emits this signal.
        This reduces the number of signal/slot connections and avoids that the tree entry base class must
        be derived from QObject.
        @param i_strKeyInTree [in] Unique key of the entry which has been changed. */
    void treeEntryChanged( const QString& i_strKeyInTree );
    /*! Signal which will be emitted if a tree entry is going to be removed from the index tree.
        If this signal is emitted the entry still belongs to the index tree and the keys and indices a returned
        by the entry are still valid.
        @param i_entryType [in] Entry type.
        @param i_strKeyInTree [in] Unique key of the entry which will be removed from the tree.
        @param i_idxInTree [in] Index of the entry which will be removed from the tree. */
    void treeEntryAboutToBeRemoved( const QString& i_strKeyInTree, int i_idxInTree );
    /*! Signal which will be emitted if a tree entry has been removed from the index tree.
        If this signal is emitted the entry no longer belongs to the index tree and the keys and indices returned
        by the entry are invalid. For this the unique key and the index of the entry in the index valid before
        removing the entry from the index tree are provided as arguments with the signal.
        @param i_entryType [in] Entry type.
        @param i_strKeyInTree [in] Unique key of the entry valid before the entry was removed from the tree.
        @param i_idxInTree [in] Index of the entry valid before the entry was removed from the tree. */
    void treeEntryRemoved( const QString& i_strKeyInTree, int i_idxInTree );
    /*! Signal which will be emitted if a tree entry is going to be moved within the index tree.
        If this signal is emitted the entry is still a child of it's current branch and does not belong yet
        to the target parent branch.
        @param i_strOrigKeyInTree [in] Unique key of the entry which is going to be moved to another parent branch.
        @param i_strKeyInTreeOfTargetBranch [in] Unique Key of the target branch to which the entry will be moved. */
    void treeEntryAboutToBeMoved( const QString& i_strOrigKeyInTree, const QString& i_strKeyInTreeOfTargetBranch );
    /*! Signal which will be emitted if a tree entry has been moved within the index tree.
        If this signal is emitted the entry already has been moved to the new target branch.
        For this the unique key and the index of the entry in tree valid before removing the entry from
        the index tree are provided as arguments with the signal.
        @param i_strNewKeyInTree [in] The new unique key of the entry after moving the entry to the new branch.
        @param i_strOrigKeyInTree [in] The original unique key of the entry before moving the entry to the new branch.
        @param i_strKeyInTreeOfTargetBranch [in] Unique Key of the target branch to which the entry has been moved.
        @note The index of the entry in the index tree remains the same. */
    void treeEntryMoved( const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strKeyInTreeOfTargetBranch );
    /*! Signal which will be emitted if a tree entry is going to be renamed.
        If this signal is emitted the entry still has its old name and its old unique key in the index tree.
        @param i_strOrigKeyInTree [in] Unique key of the entry which is going to be renamed.
        @param i_strNewName [in] New name of the entry. */
    void treeEntryAboutToBeRenamed( const QString& i_strOrigKeyInTree, const QString& i_strNewName );
    /*! Signal which will be emitted if a tree entry has been renamed.
        If this signal is emitted the entry already has its new name and a the unique key in the index tree has been changed.
        For this the unique key of the entry valid before renaming the entry is provided as an argument with the signal.
        @param i_strNewKeyInTree [in] The new unique key of the entry after renaming it.
        @param i_strOrigKeyInTree [in] The original unique key of the entry before renaming it.
        @param i_strOrigName [in] Old name of the tree entry.
        @note The index of the entry in the index tree remains the same. */
    void treeEntryRenamed( const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strOrigName );
    /*! Signal which will be emitted if the unique key of a tree entry has been changed.
        If this signal is emitted the entry already has been moved or renamed.
        For this the unique key and the index of the entry in tree valid before removing the entry from
        the index tree are provided as arguments with the signal.
        @param i_strNewKeyInTree [in] The new unique key of the entry.
        @param i_strOrigKeyInTree [in] The original unique key of the entry valid before the entry was moved to the target branch.
        @note The index of the entry in the index tree remains the same. */
    void treeEntryKeyInTreeChanged( const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree );
public: // instance methods
    /*! Returns a pointer to the mutex used to protect the index tree if access by different threads. */
    CRecursiveMutex* mutex() { return m_pMtx; }
    Q_INVOKABLE void lock();
    Q_INVOKABLE void unlock();
public: // instance methods
    /*! Returns the string used to separate the nodes within the entries path. */
    QString nodeSeparator() const { return m_strNodeSeparator; }
    /*! Returns the pointer to the root entry of the index tree. */
    CIdxTreeEntry* root() const { return m_pRoot; }
public: // instance methods
    QString buildPathStr( const QString& i_str1, const QString& i_str2 ) const;
    QString buildPathStr( const QString& i_str1, const QString& i_str2, const QString& i_str3 ) const;
    QString buildPathStr( const QString& i_str1, const QString& i_str2, const QString& i_str3, const QString& i_str4 ) const;
    QString buildPathStr( const QString& i_str1, const QString& i_str2, const QString& i_str3, const QString& i_str4, const QString& i_str5 ) const;
    QString buildKeyInTreeStr( const QString& i_strEntryTypeSymbol, const QString& i_str1 ) const;
    QString buildKeyInTreeStr( const QString& i_strEntryTypeSymbol, const QString& i_str1, const QString& i_str2 ) const;
    QString buildKeyInTreeStr( const QString& i_strEntryTypeSymbol, const QString& i_str1, const QString& i_str2, const QString& i_str3 ) const;
    QString buildKeyInTreeStr( const QString& i_strEntryTypeSymbol, const QString& i_str1, const QString& i_str2, const QString& i_str3, const QString& i_str4 ) const;
    QString buildKeyInTreeStr( const QString& i_strEntryTypeSymbol, const QString& i_str1, const QString& i_str2, const QString& i_str3, const QString& i_str4, const QString& i_str5 ) const;
    QString splitPathStr( const QString& i_strPath, QString* o_pstrBranchPath, QString* o_pstrName ) const;
public: // overridables (createBranch and createLeave must be overridden to create entries using a generic tree view)
    virtual CIdxTreeEntry* createBranch( const QString& i_strName ) const;
    virtual CIdxTreeEntry* createLeave( const QString& i_strName ) const;
public: // instance methods
    int treeEntriesVectorSize() const; // the number of used entries might be less
    CIdxTreeEntry* getEntry( int i_idxInTree ) const; // may return nullptr as some vector entries may have been freed
public: // instance methods
    CIdxTreeEntry* findBranch( const QString& i_strPath ) const;
    CIdxTreeEntry* findBranch( const QString& i_strParentPath, const QString& i_strBranchName ) const;
    CIdxTreeEntry* findLeave( const QString& i_strPath ) const;
    CIdxTreeEntry* findLeave( const QString& i_strParentPath, const QString& i_strLeaveName ) const;
    CIdxTreeEntry* findEntry( const QString& i_strKeyInTree ) const;
public: // instance methods
    SErrResultInfo canAdd( CIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath ) const;
    SErrResultInfo canAdd( CIdxTreeEntry* i_pTreeEntry, CIdxTreeEntry* i_pTargetBranch ) const;
public: // instance methods
    int add( CIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath );
    int add( CIdxTreeEntry* i_pTreeEntry, CIdxTreeEntry* i_pTargetBranch = nullptr );
public: // instance methods
    SErrResultInfo canInsert( CIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath, int i_idxInTargetBranch, int i_idxInTree = -1 ) const;
    SErrResultInfo canInsert( CIdxTreeEntry* i_pTreeEntry, CIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch, int i_idxInTree = -1 ) const;
public: // instance methods
    int insert( CIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath, int i_idxInTargetBranch, int i_idxInTree = -1 );
    int insert( CIdxTreeEntry* i_pTreeEntry, CIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch, int i_idxInTree = -1 );
public: // instance methods
    SErrResultInfo canRemove( CIdxTreeEntry* i_pTreeEntry ) const;
    SErrResultInfo canRemove( const QString& i_strKeyInTree ) const;
public: // instance methods
    void remove( CIdxTreeEntry* i_pTreeEntry );
    void remove( const QString& i_strKeyInTree );
public: // instance methods
    SErrResultInfo canMove( const QString& i_strSourcePath, const QString& i_strTargetPath, int i_idxInTargetBranch = -1 ) const;
    SErrResultInfo canMove( CIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath, int i_idxInTargetBranch = -1 ) const;
    SErrResultInfo canMove( CIdxTreeEntry* i_pTreeEntry, CIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1 ) const;
public: // instance methods
    void move( const QString& i_strSourcePath, const QString& i_strTargetPath, int i_idxInTargetBranch = -1 );
    void move( CIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath, int i_idxInTargetBranch = -1 );
    void move( CIdxTreeEntry* i_pTreeEntry, CIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1 );
public: // instance methods
    SErrResultInfo canCopy( const QString& i_strSourcePath, const QString& i_strTargetPath, int i_idxInTargetBranch = -1, int i_idxInTree = -1 ) const;
    SErrResultInfo canCopy( CIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath, int i_idxInTargetBranch = -1, int i_idxInTree = -1 ) const;
    SErrResultInfo canCopy( CIdxTreeEntry* i_pTreeEntry, CIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1, int i_idxInTree = -1 ) const;
public: // instance methods
    int copy( const QString& i_strSourcePath, const QString& i_strTargetPath, int i_idxInTargetBranch = -1, int i_idxInTree = -1 );
    int copy( CIdxTreeEntry* i_pTreeEntry, const QString& i_strTargetPath, int i_idxInTargetBranch = -1, int i_idxInTree = -1 );
    int copy( CIdxTreeEntry* i_pTreeEntry, CIdxTreeEntry* i_pTargetBranch, int i_idxInTargetBranch = -1, int i_idxInTree = -1 );
public: // instance methods
    SErrResultInfo canRename( const QString& i_strSourcePath, const QString& i_strNameNew ) const;
    SErrResultInfo canRename( CIdxTreeEntry* i_pTreeEntry, const QString& i_strNameNew ) const;
public: // instance methods
    void rename( const QString& i_strSourcePath, const QString& i_strNameNew );
    void rename( CIdxTreeEntry* i_pTreeEntry, const QString& i_strNameNew );
public: // instance methods
    SErrResultInfo canAddShortcut( const QString& i_strUniqueName ) const;
    void addShortcut( CIdxTreeEntry* i_pTreeEntry, const QString& i_strUniqueName );
    SErrResultInfo removeShortcut( const QString& i_strUniqueName );
protected: // instance methods
    void updateKeyInTree( CIdxTreeEntry* i_pTreeEntry );
protected: // instance methods
    void clear( CIdxTreeEntry* i_pBranch );
public: // instance methods for testing
    /*! Returns the map with pointers to all tree entries.
        When using this method and accessing the individual elements in the map,
        special caution is required in multithreaded applications and you may need
        to lock the index tree before accessing the elements and unlock the tree afterwards. */
    QMap<QString, CIdxTreeEntry*> treeEntriesMap() const { return m_mappTreeEntries; }
    /*! Returns the vector with pointers to all tree entries.
        When using this method and accessing the individual elements in the map,
        special caution is required in multithreaded applications and you may need
        to lock the index tree before accessing the elements and unlock the tree afterwards. */
    QVector<CIdxTreeEntry*> treeEntriesVec() const { return m_arpTreeEntries; }
    /*! Returns the map with free indices in the vector of entries.
        When using this method and accessing the individual elements in the map,
        special caution is required in multithreaded applications and you may need
        to lock the index tree before accessing the elements and unlock the tree afterwards. */
    QMap<int, int> freeIdxsMap() const { return m_mapFreeIdxs; }
public: // iterator methods
    iterator begin( iterator::ETraversalOrder i_traversalOrder = iterator::ETraversalOrder::Index );
    iterator end();
public: // overridable instance methods (used by friend class CIdxTreeEntry and its derivates to avoid that the tree entry base classes must inherit QObject to emit signals)
    virtual void onTreeEntryChanged( CIdxTreeEntry* i_pTreeEntry );
protected: // instance methods (tracing of signals)
    void emit_treeEntryAdded( const QString& i_strKeyInTree );
    void emit_treeEntryChanged( const QString& i_strKeyInTree );
    void emit_treeEntryAboutToBeRemoved( const QString& i_strKeyInTree, int i_idxInTree );
    void emit_treeEntryRemoved( const QString& i_strKeyInTree, int i_idxInTree );
    void emit_treeEntryAboutToBeMoved( const QString& i_strOrigKeyInTree, const QString& i_strKeyInTreeOfTargetBranch );
    void emit_treeEntryMoved( const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strKeyInTreeOfTargetBranch );
    void emit_treeEntryAboutToBeRenamed( const QString& i_strOrigKeyInTree, const QString& i_strNewName );
    void emit_treeEntryRenamed( const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strOrigName );
    void emit_treeEntryKeyInTreeChanged( const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree );
protected slots:
    void onTrcAdminObjChanged( QObject* i_pTrcAdminObj );
protected: // instance members
    /*!< String used to seperate the node names with an entries path. */
    QString m_strNodeSeparator;
    /*!< Mutex to protect the instance if accessed by different threads. */
    mutable CRecursiveMutex* m_pMtx;
    /*!< Map with pointers to all tree entries. */
    QMap<QString, CIdxTreeEntry*> m_mappTreeEntries;
    /*!< Vector with pointers to all tree entries. */
    QVector<CIdxTreeEntry*> m_arpTreeEntries;
    /*!< Map with free indices in the vector of entries. */
    QMap<int, int> m_mapFreeIdxs;
    /*!< Map with pointers to tree entry for which a unique name as a shortcut has been added. */
    QMap<QString, CIdxTreeEntry*> m_mappShortcutTreeEntries;
    /*!< Pointer to root entry. */
    CIdxTreeEntry* m_pRoot;
    /*!< Trace detail level for method tracing.
         Trace output may not be controlled by trace admin objects
         if the index tree belongs the trace server. */
    EMethodTraceDetailLevel m_eTrcMthFileDetailLevel;
    /*<! Reference to local trace method file. Used if the Ipc Server belongs to
         the trace server itself and tracing through trace server cannot be used. */
    ZS::System::CTrcMthFile* m_pTrcMthFile;
    /*!< Trace admin object to control trace outputs of the class.
         The object will not be created if the index tree's belongs to the trace server. */
    CTrcAdminObj* m_pTrcAdminObj;

}; // class CIdxTree

//******************************************************************************
/*! @brief Locker class to lock and unlock the index tree instance.
*/
class CIdxTreeLocker
//******************************************************************************
{
public:
    CIdxTreeLocker(CIdxTree* i_pIdxTree) : m_pIdxTree(i_pIdxTree) {
        m_pIdxTree->lock();
    }
    ~CIdxTreeLocker() {
        m_pIdxTree->unlock();
    }
private:
    CIdxTree* m_pIdxTree;
};

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_IdxTree_h
