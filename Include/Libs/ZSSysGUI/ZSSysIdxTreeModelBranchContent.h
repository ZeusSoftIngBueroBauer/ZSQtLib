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

#ifndef ZSSysGUI_IdxTreeModelBranchContent_h
#define ZSSysGUI_IdxTreeModelBranchContent_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"

namespace ZS
{
namespace System
{
#ifdef ZS_TRACE_GUI_MODELS
class CTrcAdminObj;
#endif

namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CModelIdxTreeBranchContent : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CModelIdxTreeBranchContent"; }
public: // type definitions and constants
    enum EColumn {
        EColumnTreeEntryName     = 0,
        EColumnTreeEntryType     = 1,
        EColumnInternalId        = 2,
        EColumnIdxInTree         = 3,
        EColumnIdxInParentBranch = 4,
        EColumnKeyInTree         = 5,
        EColumnKeyInParentBranch = 6,
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
public: // ctors and dtor
    CModelIdxTreeBranchContent(
        CIdxTree* i_pIdxTree,
        QObject*  i_pObjParent = nullptr,
        EMethodTraceDetailLevel i_eTrcDetailLevel = EMethodTraceDetailLevel::None,
        EMethodTraceDetailLevel i_eTrcDetailLevelNoisyMethods = EMethodTraceDetailLevel::None );
    virtual ~CModelIdxTreeBranchContent();
signals:
    void keyInTreeOfRootEntryChanged(const QString& i_strKeyInTree);
    void sortOrderChanged(const QString& i_strSortOrder);
    void sortOrderChanged(EIdxTreeSortOrder i_sortOrder);
public: // overridables
    virtual QString nameSpace() const { return CModelIdxTreeBranchContent::NameSpace(); }
    virtual QString className() const { return CModelIdxTreeBranchContent::ClassName(); }
public: // instance methods
    //CIdxTree* getIdxTree() { return m_pIdxTree; }
public: // instance methods
    Q_PROPERTY(QString keyInTreeOfRootEntry READ getKeyInTreeOfRootEntry WRITE setKeyInTreeOfRootEntry NOTIFY keyInTreeOfRootEntryChanged)
    void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree );
    QString getKeyInTreeOfRootEntry() const;
    CModelIdxTreeEntry* modelRootEntry() const { return m_pModelRootEntry; }
public: // instance methods
    Q_PROPERTY(QString sortOrder READ sortOrderAsString WRITE setSortOrder NOTIFY sortOrderChanged)
    void setSortOrder( EIdxTreeSortOrder i_sortOrder );
    void setSortOrder( const QString& i_strSortOrder );
    EIdxTreeSortOrder sortOrder() const;
    QString sortOrderAsString() const;
public: // instance methods
    CModelIdxTreeEntry* findEntry( const QString& i_strKeyInTree );
public: // instance methods
    QModelIndex index( const QString& i_strKeyInTree, int i_iClm ) const;
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected slots:
    void onIdxTreeEntryAdded( const QString& i_strKeyInTree );
    void onIdxTreeEntryChanged( const QString& i_strKeyInTree );
    void onIdxTreeEntryAboutToBeRemoved( ZS::System::EIdxTreeEntryType i_entryType, const QString& i_strKeyInTree, int i_idxInTree );
    void onIdxTreeEntryMoved( const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree, const QString& i_strKeyInTreeOfTargetBranch );
    void onIdxTreeEntryKeyInTreeChanged( const QString& i_strNewKeyInTree, const QString& i_strOrigKeyInTree );
protected: // instance methods
    void clear();
    void remove( CModelIdxTreeEntry* i_pModelTreeEntry );
public: // instance methods
    Q_INVOKABLE int columnWidthByColumn(int i_iClm, int i_iFontPixelSize = 0);
    Q_INVOKABLE int columnWidthByRole(const QByteArray& i_byteArrRole, int i_iFontPixelSize = 0);
public: // overridables of base class QAbstractItemModel
    virtual QHash<int, QByteArray> roleNames() const override;
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual QModelIndex index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags( const QModelIndex& i_modelIdx ) const override;
    virtual Qt::DropActions supportedDropActions() const override;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
public: // instance methods for editing data
    SErrResultInfo canSetData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) const;
protected slots:
    void onTrcAdminObjChanged( QObject* i_pTrcAdminObj );
    void onTrcAdminObjNoisyMethodsChanged( QObject* i_pTrcAdminObj );
protected: // auxiliary instance methods
    void fillRoleNames();
protected: // class members
    static QHash<int, QByteArray> s_roleNames;
    static QHash<QByteArray, int> s_roleValues;
    static QHash<int, QByteArray> s_clm2Name;
protected: // instance members
    CIdxTree* m_pIdxTree;
    EIdxTreeSortOrder m_sortOrder;
    /*!< Need a copy of the index tree entries as entries may be added, changed
         or removed from different threads. When removing an entry the signal
         entryRemoved is emitted and may be queued. The model cannot access the
         removed entry as it is already deleted. But the model is able to find the
         entry in the internal list and can inform the views about the removed entry.
         @Note Key is: <EntryTypeSymbol>:<ParentPath>/<Name>
               (e.g. "L:ZS::Data::CDataTable::FDAC::RF1In") */
    QMap<QString, CModelIdxTreeEntry*> m_mappModelTreeEntries;
    /*!< Model tree entry of the branch to be indicated by the table view.
         In contrast to the branch model entries of the index tree model the branch entries
         of this root entry may not contain further childs.
         Please also note to indicate the nodes of the index tree a copy of the index tree
         entries must be created as entries in the index may be added, changed or removed
         from different threads. When removing an entry the signal entryRemoved is emitted
         and may be queued. The model cannot access the removed entry as it is already deleted.
         But the model is able to find the entry in the internal list and can inform the views
         about the removed entry. */
    QString m_strKeyInTreeOfRootEntry;
    CModelIdxTreeEntry* m_pModelRootEntry;
    QVector<int> m_ariClmWidths;
    #ifdef ZS_TRACE_GUI_MODELS
    /*!< Trace detail level for method tracing.
         Trace output may not be controlled by trace admin objects
         if the index tree belongs the trace server. */
    EMethodTraceDetailLevel m_eTrcDetailLevel;
    /*!< Trace detail level for method tracing.
         This detail level is used by very often called methods like "data".
         Trace output may not be controlled by trace admin objects
         if the index tree belongs the trace server. */
    EMethodTraceDetailLevel m_eTrcDetailLevelNoisyMethods;
    /*!< Trace admin object to control trace outputs of the class.
         The object will not be created if the index tree's belongs to the trace server. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    /*!< Trace admin object to control trace outputs of the class.
         This trace admin object is used by very often called methods like "data".
         The object will not be created if the index tree's belongs to the trace server. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjNoisyMethods;
    #endif

}; // class CModelIdxTreeBranchContent

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_IdxTreeModelBranchContent_h
