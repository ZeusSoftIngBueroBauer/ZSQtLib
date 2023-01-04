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

#ifndef ZSPhysValGUI_PhysSizesIdxTreeModelBranchContent_h
#define ZSPhysValGUI_PhysSizesIdxTreeModelBranchContent_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"

namespace ZS
{
namespace PhysVal
{
class CIdxTreePhysSizes;

namespace GUI
{
//******************************************************************************
class ZSPHYSVALGUIDLL_API CModelIdxTreePhysSizesBranchContent : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CModelIdxTreePhysSizesBranchContent"; }
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
    CModelIdxTreePhysSizesBranchContent( QObject* i_pObjParent = nullptr );
    CModelIdxTreePhysSizesBranchContent( CIdxTreePhysSizes* i_pIdxTree, QObject*  i_pObjParent = nullptr );
    virtual ~CModelIdxTreePhysSizesBranchContent();
signals:
    /*! Signal which will be emitted if the index tree instance has been changed.
        @param i_pIdxTree Newly referenced index tree. */
    void idxTreeChanged( QObject* i_pIdxTree );
    /*! Signal which will be emitted if the root entries key in the index tree has been changed
        and therefore the content of the model will be complete exchanged.
        @param i_strKeyInTree Key of the root entry in the index tree. */
    void keyInTreeOfRootEntryChanged(const QString& i_strKeyInTree);
public: // overridables
    virtual QString nameSpace() const { return CModelIdxTreePhysSizesBranchContent::NameSpace(); }
    virtual QString className() const { return CModelIdxTreePhysSizesBranchContent::ClassName(); }
public: // instance methods
    Q_PROPERTY(QObject* idxTree READ idxTree WRITE setIdxTree NOTIFY idxTreeChanged)
    void setIdxTree( QObject* i_pIdxTree );
    QObject* idxTree();
public: // instance methods
    Q_PROPERTY(QString keyInTreeOfRootEntry READ getKeyInTreeOfRootEntry WRITE setKeyInTreeOfRootEntry NOTIFY keyInTreeOfRootEntryChanged)
    void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree );
    QString getKeyInTreeOfRootEntry() const;
    ZS::System::GUI::CModelIdxTreeEntry* modelRootEntry() const { return m_pModelRootEntry; }
public: // instance methods
    ZS::System::GUI::CModelIdxTreeEntry* findEntry( const QString& i_strKeyInTree );
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
    void remove( ZS::System::GUI::CModelIdxTreeEntry* i_pModelTreeEntry );
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
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const override;
protected: // auxiliary instance methods
    void fillRoleNames();
protected: // class members
    static QHash<int, QByteArray> s_roleNames;
    static QHash<QByteArray, int> s_roleValues;
    static QHash<int, QByteArray> s_clm2Name;
protected: // instance members
    /*!< Pointer to index tree instance.
         Please note that entries may be added from within different thread contexts
         to the index tree. If so before accessing the entries the index tree
         instance the index tree instance must be locked - and unlocked afterwards. */
    CIdxTreePhysSizes* m_pIdxTree;
    /*!< Need a copy of the index tree entries as entries may be added, changed
         or removed from different threads. When removing an entry the signal
         entryRemoved is emitted and may be queued. The model cannot access the
         removed entry as it is already deleted. But the model is able to find the
         entry in the internal list and can inform the views about the removed entry.
         @Note Key is: <EntryTypeSymbol>:<ParentPath>/<Name>
               (e.g. "L:ZS::Data::CDataTable::FDAC::RF1In") */
    QMap<QString, ZS::System::GUI::CModelIdxTreeEntry*> m_mappModelTreeEntries;
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
    /*!< The model entry of the branch whose childs should be exposed by the model to views. */
    ZS::System::GUI::CModelIdxTreeEntry* m_pModelRootEntry;
    /*!< The QMLs table view don't take the header labels into account when resizing the
         columns to fit their contents. The model provides methods to calculate the maximum
         width of a column depending on the used font. To speed up the calculation the
         recently calculated widths are stored in this member. Only if an indicated
         entry is added/removed/modified the widths need to be recalculated.
         2022-12-23: TODO: The optimization is not yet implemented. */
    QVector<int> m_ariClmWidths;

}; // class CModelIdxTreePhysSizesBranchContent

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysSizesIdxTreeModelBranchContent_h
