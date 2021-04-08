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

#ifndef ZSSysGUI_IdxTreeModel_h
#define ZSSysGUI_IdxTreeModel_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysIdxTree.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}
namespace System
{
namespace GUI
{
class CModelAbstractTreeEntry;
class CModelBranchTreeEntry;
class CModelLeaveTreeEntry;
class CModelRootTreeEntry;

/*******************************************************************************
Type definitions and constants
*******************************************************************************/

//------------------------------------------------------------------------------
enum EIdxTreeSortOrder
//------------------------------------------------------------------------------
{
    EIdxTreeSortOrderConfig              = 0,    // As provided (configured) by the referenced index tree branches.
    EIdxTreeSortOrderAscending           = 1,    // Child branches alphabetically sorted in ascending order followed by the leaves.
    //EIdxTreeSortOrderLogicalDescending = 2,    // Child branches alphabetically sorted in descending order followed by the leaves.
    //EIdxTreeSortOrderByNameAscending   = 3,    // Child branches and leaves mixed alphabetically sorted in ascending order.
    //EIdxTreeSortOrderByNameDescending  = 4,    // Child branches and leaves mixed alphabetically sorted in descending order.
    EIdxTreeSortOrderCount,
    EIdxTreeSortOrderUndefined
};

ZSSYSGUIDLL_API QString idxTreeSortOrder2Str( EIdxTreeSortOrder i_eVal, int i_alias = ZS::System::EEnumEntryAliasStrName );
ZSSYSGUIDLL_API QString idxTreeSortOrder2Str( int i_iVal, int i_alias = ZS::System::EEnumEntryAliasStrName );
ZSSYSGUIDLL_API QPixmap idxTreeSortOrder2Pixmap( EIdxTreeSortOrder i_eVal, const QSize& i_sz = QSize(24,24));
ZSSYSGUIDLL_API QPixmap idxTreeSortOrder2Pixmap( int i_iVal, const QSize& i_sz = QSize(24,24));
ZSSYSGUIDLL_API EIdxTreeSortOrder str2IdxTreeSortOrder( const QString& i_strVal, int i_alias = ZS::System::EEnumEntryAliasStrName );


//******************************************************************************
class ZSSYSGUIDLL_API CModelIdxTree : public QAbstractItemModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CModelIdxTree"; }
public: // type definitions and constants
    class ZSSYSGUIDLL_API iterator
    {
    friend class CModelIdxTree;
    public:
        enum ETraversalOrder {
            ETraversalOrderIndex,      // iterate throug the vector (index based)
            ETraversalOrderPreOrder,   // traverse through the tree in preorder (top to bottom, first to last child index of branches)
            ETraversalOrderCount,
            ETraversalOrderUndefined
        };
    public:
        iterator() : m_pModel(nullptr), m_pModelTreeEntryCurr(nullptr), m_traversalOrder(ETraversalOrderIndex) {}
    protected:
        iterator( CModelIdxTree* i_pModel, CModelIdxTree::iterator::ETraversalOrder i_traversalOrder ) :
            m_pModel(i_pModel),
            m_pModelTreeEntryCurr(nullptr),
            m_traversalOrder(i_traversalOrder)
        {
        }
    public:
        CModelAbstractTreeEntry* operator * () const;
        bool operator == ( const iterator& i_other ) const;
        bool operator == ( iterator& i_other ) const;
        bool operator != ( const iterator& i_other ) const;
        bool operator != ( iterator& i_other ) const;
        iterator& operator ++ ();
    private:
        CModelIdxTree*           m_pModel;
        CModelAbstractTreeEntry* m_pModelTreeEntryCurr;
        ETraversalOrder          m_traversalOrder;
    };
public: // type definitions and constants
    enum EColumn {
        EColumnTreeEntryName     = 0,
        EColumnInternalId        = 1,
        EColumnTreeEntryType     = 2,
        EColumnIdxInTree         = 3,
        EColumnIdxInParentBranch = 4,
        EColumnKeyInTree         = 5,
        EColumnKeyInParentBranch = 6,
        EColumnCount
    };
public: // class methods
    static QIcon GetIcon( EIdxTreeEntryType i_entryType );
    static QString ModelIdx2Str( const QModelIndex& i_modelIdx );
protected: // class methods
    static bool AreIconsCreated();
public: // ctors and dtor
    CModelIdxTree(
        CIdxTree* i_pIdxTree = nullptr,
        QObject*  i_pObjParent = nullptr,
        int       i_iTrcDetailLevel = ZS::Trace::ETraceDetailLevelNone );
    virtual ~CModelIdxTree();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setIdxTree( CIdxTree* i_pIdxTree );
    CIdxTree* idxTree() { return m_pIdxTree; }
public: // instance methods
    QString nodeSeparator() const;
    CModelRootTreeEntry* modelRoot() const { return m_pModelRoot; }
    QMap<QString, CModelAbstractTreeEntry*> treeEntriesMap() const { return m_mappModelTreeEntries; }
public: // instance methods
    void setFilter( EIdxTreeEntryType i_entryType );
    EIdxTreeEntryType getFilter() const { return m_entryTypeFilter; }
protected: // instance methods
    void setFilter( CModelBranchTreeEntry* i_pModelBranch, EIdxTreeEntryType i_entryType, bool i_bRecursive );
public: // instance methods
    void setSortOrder( EIdxTreeSortOrder i_sortOrder );
    EIdxTreeSortOrder sortOrder() const { return m_sortOrder; }
protected: // instance methods
    void setSortOrder( CModelBranchTreeEntry* i_pModelBranch, EIdxTreeSortOrder i_sortOrder, bool i_bRecursive );
public: // instance methods
    void setIsExpanded( CModelBranchTreeEntry* i_pModelBranch, bool i_bIsExpanded );
    bool areAllParentBranchesExpanded( CModelBranchTreeEntry* i_pModelBranch ) const;
public: // instance methods
    void setIsSelected( CModelAbstractTreeEntry* i_pModelTreeEntry, bool i_bIsSelected, bool i_bRecursive = false );
public: // instance methods
    CModelBranchTreeEntry* findModelBranch( CBranchIdxTreeEntry* i_pBranch );
    CModelLeaveTreeEntry* findModelLeave( CLeaveIdxTreeEntry* i_pLeave );
    CModelAbstractTreeEntry* findModelEntry( CAbstractIdxTreeEntry* i_pTreeEntry );
public: // instance methods
    QModelIndex index( const QString& i_strKeyInTree, int i_iClm ) const;
public: // iterator methods
    iterator begin( iterator::ETraversalOrder i_traversalOrder = iterator::ETraversalOrderIndex );
    iterator end();
protected slots:
    void onIdxTreeAboutToBeDestroyed( QObject* i_pIdxTree );
protected slots:
    void onIdxTreeEntryAdded( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry );
    void onIdxTreeEntryChanged( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry );
    void onIdxTreeEntryAboutToBeRemoved( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry );
    void onIdxTreeEntryMoved( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strKeyInTreePrev, ZS::System::CBranchIdxTreeEntry* i_pTargetBranch );
    void onIdxTreeEntryKeyInTreeChanged( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CAbstractIdxTreeEntry* i_pTreeEntry, const QString& i_strKeyInTreePrev );
protected: // instance methods
    void clear( CModelBranchTreeEntry* i_pModelBranch, bool i_bDestroyTreeEntries = true );
    void remove( CModelAbstractTreeEntry* i_pModelTreeEntry );
public: // overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QModelIndex index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& i_modelIdx ) const;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags( const QModelIndex& i_modelIdx ) const;
    virtual Qt::DropActions supportedDropActions() const;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
public: // overridables of base class QAbstractItemModel
    virtual QStringList mimeTypes() const;
    virtual QMimeData* mimeData( const QModelIndexList& i_arModelIdxs ) const;
    virtual bool canDropMimeData( const QMimeData* i_pMimeData, Qt::DropAction i_dropAction, int i_iRow, int i_iClm, const QModelIndex& i_modelIdxParent ) const; //; virtual method of base class since Qt version ?
    virtual bool dropMimeData( const QMimeData* i_pMimeData, Qt::DropAction i_dropAction, int i_iRow, int i_iClm, const QModelIndex& i_modelIdxParent );
public: // instance methods for editing data
    SErrResultInfo canSetData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) const;
public: // overridables of base class QAbstractItemModel (just overwritten to trace the method calls for debugging purposes)
    virtual QModelIndex sibling( int i_iRow, int i_iCol, const QModelIndex& i_modelIdx ) const;
    virtual bool hasChildren( const QModelIndex& i_modelIdxParent = QModelIndex() ) const;
    virtual bool insertRows( int i_iRow, int i_iRowCount, const QModelIndex& i_modelIdxParent = QModelIndex() );
    virtual bool insertColumns( int i_iCol, int i_iColCount, const QModelIndex& i_modelIdxParent = QModelIndex() );
    virtual bool removeRows( int i_iRow, int i_iRowCount, const QModelIndex& i_modelIdxParent = QModelIndex() );
    virtual bool removeColumns( int i_iCol, int i_iColCount, const QModelIndex& i_modelIdxParent = QModelIndex() );
    virtual bool moveRows( const QModelIndex& i_modelIdxSourceParent, int i_iRowSource, int i_iRowCount, const QModelIndex& i_modelIdxDestParent, int i_iRowDestChild );
    virtual bool moveColumns( const QModelIndex& i_modelIdxSourceParent, int i_iColSource, int i_iColCount, const QModelIndex& i_modelIdxDestParent, int i_iColDestChild );
protected: // to trace emitting signals for debugging purposes
    void emit_dataChanged( const QModelIndex& i_modelIdxTL, const QModelIndex& i_modelIdxBR, const QVector<int>& i_ariRoles = QVector<int>() );
    void emit_headerDataChanged( Qt::Orientation i_orientation, int i_iFirstSection, int i_iLastSection );
protected: // reimplemented to trace emitting signals for debugging purposes
    inline QModelIndex _createIndex( int i_iRow, int i_iCol, void* i_pvData = nullptr ) const;
    inline QModelIndex _createIndex( int i_iRow, int i_iCol, quintptr i_uId ) const;
    void _beginInsertRows( const QModelIndex& i_modelIdxParent, int i_iFirstRow, int i_iLastRow );
    void _endInsertRows();
    void _beginRemoveRows( const QModelIndex &i_modelIdxParent, int i_iFirstRow, int i_iLastRow );
    void _endRemoveRows();
    bool _beginMoveRows( const QModelIndex& i_modelIdxSourceParent, int i_iRowSourceStart, int i_iRowSourceEnd, const QModelIndex& i_modelIdxDestParent, int i_iRowDest );
    void _endMoveRows();
    void _beginInsertColumns( const QModelIndex& i_modelIdxParent, int i_iFirstCol, int i_iLastCol );
    void _endInsertColumns();
    void _beginRemoveColumns( const QModelIndex& i_modelIdxParent, int i_iFirstCol, int i_iLastCol );
    void _endRemoveColumns();
    bool _beginMoveColumns( const QModelIndex& i_modelIdxSourceParent, int i_iColSourceStart, int i_iColSourceEnd, const QModelIndex& i_modelIdxDestParent, int i_iColDest );
    void _endMoveColumns();
    void _beginResetModel();
    void _endResetModel();
protected: // class members
    static int      s_iInstCount;
    static bool     s_bIconsCreated;
    static QPixmap* s_pPxmRoot;
    static QPixmap* s_pPxmBranch;
    static QPixmap* s_pPxmLeave;
    static QIcon*   s_pIconRoot;
    static QIcon*   s_pIconBranch;
    static QIcon*   s_pIconLeave;
protected: // instance members
    CIdxTree*                               m_pIdxTree;
    EIdxTreeEntryType                       m_entryTypeFilter;
    EIdxTreeSortOrder                       m_sortOrder;
    QMap<QString, CModelAbstractTreeEntry*> m_mappModelTreeEntries;   // Key is: <EntryTypeSymbol>:<ParentPath>/<Name> (e.g. "L:ZS::Data::CDataTable::FDAC::RF1In")
    CModelRootTreeEntry*                    m_pModelRoot;
    int                                     m_iTrcDetailLevel;

}; // class CModelIdxTree

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_IdxTreeModel_h
