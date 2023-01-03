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

#ifndef ZSSysGUI_IdxTreeModel_h
#define ZSSysGUI_IdxTreeModel_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysIdxTree.h"

namespace ZS
{
namespace System
{
#ifdef ZS_TRACE_GUI_MODELS
class CTrcAdminObj;
#endif

namespace GUI
{
class CModelIdxTreeEntry;

/*******************************************************************************
Type definitions and constants
*******************************************************************************/

//------------------------------------------------------------------------------
enum class EIdxTreeSortOrder
//------------------------------------------------------------------------------
{
    Config     = 0,    // As provided (configured) by the referenced index tree branches.
    Ascending  = 1,    // Childs alphabetically sorted in ascending order.
    Descending = 2,    // Childs alphabetically sorted in descending order.
    Count,
    Undefined
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
    /*! @brief Iterator for iterating through the tree entries.
        The iterator may be used either to iterate through the trees vector of entries by index or
        through the tree in preorder.
        @note If the index tree is modified by different threads you need to lock the
              index tree when iterating through the entries. */
    class ZSSYSGUIDLL_API iterator
    {
    friend class CModelIdxTree;
    public:
        enum ETraversalOrder {
            Index,      // iterate throug the vector (index based)
            PreOrder,   // traverse through the tree in preorder (top to bottom, first to last child index of branches)
            Count,
            Undefined
        };
    public:
        iterator() {}
    protected:
        iterator( CModelIdxTree* i_pModel, CModelIdxTree::iterator::ETraversalOrder i_traversalOrder ) :
            m_pModel(i_pModel),
            m_pModelTreeEntryCurr(nullptr),
            m_traversalOrder(i_traversalOrder)
        {
        }
    public:
        CModelIdxTreeEntry* operator * () const;
        bool operator == ( const iterator& i_other ) const;
        bool operator == ( iterator& i_other ) const;
        bool operator != ( const iterator& i_other ) const;
        bool operator != ( iterator& i_other ) const;
        iterator& operator ++ ();
    private:
        CModelIdxTree*      m_pModel = nullptr;
        CModelIdxTreeEntry* m_pModelTreeEntryCurr = nullptr;
        ETraversalOrder     m_traversalOrder = ETraversalOrder::Index;
    };
public: // type definitions and constants
    enum EColumn {
        EColumnTreeEntryName          = 0,
        EColumnTreeEntryNameDecorated = 1,
        EColumnTreeEntryTypeImageUrl  = 2,
        EColumnTreeEntryTypeIcon      = 3,
        EColumnTreeEntryType          = 4,
        EColumnInternalId             = 5,
        EColumnIdxInTree              = 6,
        EColumnIdxInParentBranch      = 7,
        EColumnKeyInTree              = 8,
        EColumnKeyInParentBranch      = 9,
        EColumnCount,
        EColumnUndefined
    };
    static QString column2Str(EColumn i_clm);
public: // type definitions and constants
    enum class ERole {
        Sort = Qt::UserRole,
        ImageUrl,
        Type,
        FirstDataColumnRole
    };
    Q_ENUM(ERole);
public: // class methods
    static EColumn role2Column(int i_iRole);
    static QString role2Str(int i_iRole);
    static int byteArr2Role(const QByteArray& i_byteArrRole);
    static int column2Role(EColumn i_clm);
    Q_INVOKABLE static QString modelIdx2Str( const QModelIndex& i_modelIdx, int i_iRole = -1, bool i_bIncludeId = false );
public: // class methods
    static QString getImageUrl( EIdxTreeEntryType i_entryType );
    static QIcon getIcon( EIdxTreeEntryType i_entryType );
protected: // class methods
    static bool areIconsCreated();
public: // ctors and dtor
    CModelIdxTree( QObject* i_pObjParent = nullptr );
    CModelIdxTree( CIdxTree* i_pIdxTree, QObject* i_pObjParent = nullptr );
    virtual ~CModelIdxTree();
signals:
    void idxTreeChanged( QObject* i_pIdxTree );
    void sortOrderChanged(const QString& i_strSortOrder);
    void sortOrderChanged(EIdxTreeSortOrder i_sortOrder);
    void excludeLeavesChanged(bool i_bExcludeLeaves);
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    Q_PROPERTY(QObject* idxTree READ idxTree WRITE setIdxTree NOTIFY idxTreeChanged)
    void setIdxTree( QObject* i_pIdxTree );
    QObject* idxTree();
public: // instance methods
    Q_PROPERTY(QString nodeSeparator READ nodeSeparator CONSTANT)
    QString nodeSeparator() const;
    CModelIdxTreeEntry* modelRootEntry() const { return m_pModelRootEntry; }
    QMap<QString, CModelIdxTreeEntry*> treeEntriesMap() const { return m_mappModelTreeEntries; }
public: // instance methods
    Q_PROPERTY(bool excludeLeaves READ areLeavesExcluded WRITE setExcludeLeaves NOTIFY excludeLeavesChanged)
    void setExcludeLeaves(bool i_bExcludeLeaves);
    bool areLeavesExcluded() const;
protected: // instance methods
    void setExcludeLeaves( CModelIdxTreeEntry* i_pModelBranch, bool i_bExcludeLeaves, bool i_bRecursive );
public: // instance methods
    Q_PROPERTY(QString sortOrder READ sortOrderAsString WRITE setSortOrder NOTIFY sortOrderChanged)
    void setSortOrder( EIdxTreeSortOrder i_sortOrder );
    void setSortOrder( const QString& i_strSortOrder );
    EIdxTreeSortOrder sortOrder() const;
    QString sortOrderAsString() const;
protected: // instance methods
    void setSortOrder( CModelIdxTreeEntry* i_pModelBranch, EIdxTreeSortOrder i_sortOrder, bool i_bRecursive );
public: // instance methods
    Q_INVOKABLE QString getTreeEntryTypeAsString( const QModelIndex& i_modelIdx ) const;
    Q_INVOKABLE QString getTreeEntryName( const QModelIndex& i_modelIdx ) const;
    Q_INVOKABLE QString getTreeEntryPath( const QModelIndex& i_modelIdx ) const;
    Q_INVOKABLE QString getTreeEntryAbsoluteNodePath( const QModelIndex& i_modelIdx ) const;
    Q_INVOKABLE QString getTreeEntryKeyInTree( const QModelIndex& i_modelIdx ) const;
    Q_INVOKABLE int getTreeEntryIndexInTree( const QModelIndex& i_modelIdx ) const;
    Q_INVOKABLE QString getTreeEntryKeyInParentPath( const QModelIndex& i_modelIdx ) const;
    Q_INVOKABLE int getTreeEntryKeyIndexInParentPath( const QModelIndex& i_modelIdx ) const;
public: // instance methods
    void setIsExpanded( CModelIdxTreeEntry* i_pModelBranch, bool i_bIsExpanded );
    bool areAllParentBranchesExpanded( CModelIdxTreeEntry* i_pModelBranch ) const;
public: // instance methods
    void setIsSelected( CModelIdxTreeEntry* i_pModelTreeEntry, bool i_bIsSelected, bool i_bRecursive = false );
public: // instance methods
    CModelIdxTreeEntry* findEntry( const QString& i_strKeyInTree ) const;
public: // instance methods
    QModelIndex index( const QString& i_strKeyInTree, int i_iClm ) const;
public: // iterator methods
    iterator begin( iterator::ETraversalOrder i_traversalOrder = iterator::ETraversalOrder::Index );
    iterator end();
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected slots: // overridables
    virtual void onIdxTreeEntryAdded( const QString& i_strKeyInTree );
    virtual void onIdxTreeEntryChanged( const QString& i_strKeyInTree );
    virtual void onIdxTreeEntryAboutToBeRemoved( ZS::System::EIdxTreeEntryType i_entryType, const QString& i_strKeyInTree, int i_idxInTree );
    virtual void onIdxTreeEntryMoved( const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strKeyInTreeOfTargetBranch );
    virtual void onIdxTreeEntryKeyInTreeChanged( const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree );
protected: // instance methods
    void clear( CModelIdxTreeEntry* i_pModelBranch, bool i_bDestroyTreeEntries = true );
    void remove( CModelIdxTreeEntry* i_pModelTreeEntry );
    //void updateKeyInTree( CModelIdxTreeEntry* i_pModelTreeEntry );
public: // instance methods
    Q_INVOKABLE int columnWidthByColumn(int i_iClm, int i_iFontPixelSize = 0, const QModelIndex& i_modelIdxParent = QModelIndex());
    Q_INVOKABLE int columnWidthByRole(const QByteArray& i_byteArrRole, int i_iFontPixelSize = 0, const QModelIndex& i_modelIdxParent = QModelIndex());
public: // overridables of base class QAbstractItemModel
    virtual QHash<int, QByteArray> roleNames() const override;
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual QModelIndex index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual QModelIndex parent( const QModelIndex& i_modelIdx ) const override;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags( const QModelIndex& i_modelIdx ) const override;
    virtual Qt::DropActions supportedDropActions() const override;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const override;
    virtual bool setData(const QModelIndex& i_modelIdx, const QVariant& i_value, int i_iRole = Qt::EditRole) override;
public: // overridables of base class QAbstractItemModel
    virtual QStringList mimeTypes() const override;
    virtual QMimeData* mimeData( const QModelIndexList& i_arModelIdxs ) const override;
    virtual bool canDropMimeData( const QMimeData* i_pMimeData, Qt::DropAction i_dropAction, int i_iRow, int i_iClm, const QModelIndex& i_modelIdxParent ) const; // override; virtual method of base class since Qt version ?
    virtual bool dropMimeData( const QMimeData* i_pMimeData, Qt::DropAction i_dropAction, int i_iRow, int i_iClm, const QModelIndex& i_modelIdxParent ) override;
public: // instance methods for editing data
    SErrResultInfo canSetData(const QModelIndex& i_modelIdx, const QVariant& i_value, int i_iRole = Qt::EditRole) const;
//public: // overridables of base class QAbstractItemModel (just overwritten to trace the method calls for debugging purposes)
//    virtual QModelIndex sibling( int i_iRow, int i_iCol, const QModelIndex& i_modelIdx ) const override;
//    virtual bool hasChildren( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
//    virtual bool insertRows( int i_iRow, int i_iRowCount, const QModelIndex& i_modelIdxParent = QModelIndex() ) override;
//    virtual bool insertColumns( int i_iCol, int i_iColCount, const QModelIndex& i_modelIdxParent = QModelIndex() ) override;
//    virtual bool removeRows( int i_iRow, int i_iRowCount, const QModelIndex& i_modelIdxParent = QModelIndex() ) override;
//    virtual bool removeColumns( int i_iCol, int i_iColCount, const QModelIndex& i_modelIdxParent = QModelIndex() ) override;
//    virtual bool moveRows( const QModelIndex& i_modelIdxSourceParent, int i_iRowSource, int i_iRowCount, const QModelIndex& i_modelIdxDestParent, int i_iRowDestChild ) override;
//    virtual bool moveColumns( const QModelIndex& i_modelIdxSourceParent, int i_iColSource, int i_iColCount, const QModelIndex& i_modelIdxDestParent, int i_iColDestChild ) override;
protected: // to trace emitting signals for debugging purposes
    void emit_dataChanged( const QModelIndex& i_modelIdxTL, const QModelIndex& i_modelIdxBR, const QVector<int>& i_ariRoles = QVector<int>() );
    void emit_headerDataChanged( Qt::Orientation i_orientation, int i_iFirstSection, int i_iLastSection );
    void emit_layoutChanged( const QList<QPersistentModelIndex>& i_arModelIdxsParents = QList<QPersistentModelIndex>(), QAbstractItemModel::LayoutChangeHint i_hint = QAbstractItemModel::NoLayoutChangeHint );
    void emit_layoutAboutToBeChanged( const QList<QPersistentModelIndex>& i_arModelIdxsParents = QList<QPersistentModelIndex>(), QAbstractItemModel::LayoutChangeHint i_hint = QAbstractItemModel::NoLayoutChangeHint );
    void emit_sortOrderChanged(const QString& i_strSortOrder);
    void emit_sortOrderChanged(EIdxTreeSortOrder i_sortOrder);
    void emit_excludeLeavesChanged(bool i_bExcludeLeaves);
protected: // reimplemented to trace emitting signals for debugging purposes
    inline QModelIndex _createIndex( int i_iRow, int i_iCol, void* i_pvData = Q_NULLPTR ) const;
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
protected: // auxiliary instance methods
    void fillRoleNames();
protected: // class members
    static int      s_iInstCount;
    static bool     s_bIconsCreated;
    static QPixmap* s_pPxmRoot;
    static QPixmap* s_pPxmBranch;
    static QPixmap* s_pPxmLeave;
    static QIcon*   s_pIconRoot;
    static QIcon*   s_pIconBranch;
    static QIcon*   s_pIconLeave;
protected: // class members
    static QHash<int, QByteArray> s_roleNames;
    static QHash<QByteArray, int> s_roleValues;
    static QHash<int, QByteArray> s_clm2Name;
protected: // instance members
    CIdxTree* m_pIdxTree;
    bool m_bExcludeLeaves;
    EIdxTreeSortOrder m_sortOrder;
    /*!< Need a copy of the index tree entries as entries may be added, changed
         or removed from different threads. When removing an entry the signal
         entryRemoved is emitted and may be queued. The model cannot access the
         removed entry as it is already deleted. But the model is able to find the
         entry in the internal list and can inform the views about the removed entry.
         @Note Key is: <EntryTypeSymbol>:<ParentPath>/<Name>
               (e.g. "L:ZS::Data::CDataTable::FDAC::RF1In") */
    QMap<QString, CModelIdxTreeEntry*> m_mappModelTreeEntries;
    CModelIdxTreeEntry* m_pModelRootEntry;
    QVector<int> m_ariClmWidths;
    #ifdef ZS_TRACE_GUI_MODELS
    /*!< Trace admin object to control trace outputs of the class.
         The object will not be created if the index tree's belongs to the trace server. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    /*!< Trace admin object to control trace outputs of the class.
         This trace admin object is used by very often called methods like "data".
         The object will not be created if the index tree's belongs to the trace server. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjNoisyMethods;
    #endif

}; // class CModelIdxTree

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_IdxTreeModel_h
