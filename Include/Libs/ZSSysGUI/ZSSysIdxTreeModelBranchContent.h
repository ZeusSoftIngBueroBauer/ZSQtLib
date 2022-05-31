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
namespace Trace
{
}
namespace System
{
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
        EColumnInternalId        = 1,
        EColumnTreeEntryType     = 2,
        EColumnIdxInTree         = 3,
        EColumnIdxInParentBranch = 4,
        EColumnKeyInTree         = 5,
        EColumnKeyInParentBranch = 6,
        EColumnCount
    };
public: // ctors and dtor
    CModelIdxTreeBranchContent(
        CIdxTree* i_pIdxTree,
        QObject*  i_pObjParent = nullptr,
        int       i_iTrcDetailLevel = ZS::Trace::ETraceDetailLevelNone );
    virtual ~CModelIdxTreeBranchContent();
public: // overridables
    virtual QString nameSpace() const { return CModelIdxTreeBranchContent::NameSpace(); }
    virtual QString className() const { return CModelIdxTreeBranchContent::ClassName(); }
public: // instance methods
    CIdxTree* getIdxTree() { return m_pIdxTree; }
public: // instance methods
    void setBranch( CIdxTreeEntry* i_pBranch );
    CIdxTreeEntry* branch() { return m_pBranch; }
public: // instance methods
    // Please note that the model branch entry as the root of the branch content model is a clone
    // of the model branch of idx tree model. The branch node of the index tree model may not contain
    // leaves whereas the root branch (the clone) of the content model may.
    CModelIdxTreeEntry* modelBranch() const { return m_pModelBranch; }
public: // instance methods
    EIdxTreeSortOrder sortOrder() const { return m_sortOrder; }
    void setSortOrder( EIdxTreeSortOrder i_sortOrder );
public: // instance methods
    CModelIdxTreeEntry* findModelEntry( CIdxTreeEntry* i_pTreeEntry );
public: // instance methods
    //QModelIndex index( const QString& i_strKeyInParentBranch ) const;
protected slots:
    void onIdxTreeAboutToBeDestroyed( QObject* i_pIdxTree );
protected slots:
    void onIdxTreeEntryAdded( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CIdxTreeEntry* i_pTreeEntry );
    void onIdxTreeEntryChanged( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CIdxTreeEntry* i_pTreeEntry );
    void onIdxTreeEntryAboutToBeRemoved( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CIdxTreeEntry* i_pTreeEntry );
    void onIdxTreeEntryMoved( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CIdxTreeEntry* i_pTreeEntry, const QString& i_strKeyInTreePrev, ZS::System::CIdxTreeEntry* i_pTargetBranch );
    void onIdxTreeEntryKeyInTreeChanged( ZS::System::CIdxTree* i_pIdxTree, ZS::System::CIdxTreeEntry* i_pTreeEntry, const QString& i_strKeyInTreePrev );
protected: // instance methods
    void clear( bool i_bDestroyTreeEntries = true );
    void remove( CModelIdxTreeEntry* i_pModelTreeEntry );
public: // overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
public: // overridables of base class QAbstractItemModel
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags( const QModelIndex& i_modelIdx ) const override;
    virtual Qt::DropActions supportedDropActions() const override;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
public: // instance methods for editing data
    SErrResultInfo canSetData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) const;
protected: // instance members
    CIdxTree*           m_pIdxTree;
    EIdxTreeSortOrder   m_sortOrder;
    CIdxTreeEntry*      m_pBranch;
    // Please note that the model branch entry as the root of the branch content model is a clone
    // of the model branch of idx tree model. The branch node of the index tree model may not contain
    // leaves whereas the root branch (the clone) of the content model may.
    CModelIdxTreeEntry* m_pModelBranch;
    int                 m_iTrcDetailLevel;

}; // class CModelIdxTreeBranchContent

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_IdxTreeModelBranchContent_h
