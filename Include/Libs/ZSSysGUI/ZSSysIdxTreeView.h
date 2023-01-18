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

#ifndef ZSSysGUI_IdxTreeView_h
#define ZSSysGUI_IdxTreeView_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qstyleditemdelegate.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qstyleditemdelegate.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "ZSSysGUI/ZSSysIdxTreeModel.h"

class QLineEdit;

namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CDelegateIdxTree : public QStyledItemDelegate
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CDelegateIdxTree"; }
public: // ctors and dtor
    CDelegateIdxTree(QObject* i_pObjParent = nullptr);
    virtual ~CDelegateIdxTree();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    QLineEdit* nameColumnEditor();
public: // overridables of base class QStyledItemDelegate
    QWidget* createEditor(
        QWidget*                    i_pWdgtParent,
        const QStyleOptionViewItem& i_option,
        const QModelIndex&          i_modelIdx ) const override;
    void setEditorData(
        QWidget*           i_pWdgtEditor,
        const QModelIndex& i_modelIdx ) const override;
    void setModelData(
        QWidget*            i_pWdgtEditor,
        QAbstractItemModel* i_pModel,
        const QModelIndex&  i_modelIdx ) const override;
    void updateEditorGeometry(
        QWidget*                    i_pWdgtEditor,
        const QStyleOptionViewItem& i_option,
        const QModelIndex&          i_modelIdx ) const override;
protected slots:
    void onEdtNameDestroyed( QObject* i_pWdgtEditor );
private: // copy ctor not implemented
    CDelegateIdxTree( const CDelegateIdxTree& );
private: // assignment operator not implemented
    CDelegateIdxTree& operator = ( const CDelegateIdxTree& );
private: // instance members
    /*!< Line edit to edit the name of the tree entry. */
    QLineEdit* m_pEdtName;
    /*!< true if the destroyed signal of the line editor is connected to the onEdtNameDestroyed slot. */
    bool m_bEdtNameDestroyedSignalConnected;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDelegateIdxTree


//******************************************************************************
class ZSSYSGUIDLL_API CTreeViewIdxTree : public QTreeView
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum class EPasteMode {
        Copy = 0,
        Cut  = 1,
        Count,
        Undefined
    };
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CTreeViewIdxTree"; }
public: // ctors and dtor
    CTreeViewIdxTree( CIdxTree* i_pIdxTree, QWidget* i_pWdgtParent = nullptr );
    virtual ~CTreeViewIdxTree();
signals:
    void sortOrderChanged(const QString& i_strSortOrder);
    void sortOrderChanged(EIdxTreeSortOrder i_sortOrder);
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setIdxTree(CIdxTree* i_pIdxTree);
    CIdxTree* idxTree() { return m_pIdxTree; }
public: // instance methods
    void setExcludeLeaves(bool i_bExcludeLeaves);
    bool areLeavesExcluded() const;
public: // instance methods
    void setSortOrder( EIdxTreeSortOrder i_sortOrder );
    EIdxTreeSortOrder sortOrder() const;
public: // instance methods
    void setSilentlyExecuteDeleteRequests( bool i_bExecuteSilently );
    bool silentlyExecuteDeleteRequests() const { return m_bSilentlyExecuteDeleteRequests; }
public: // instance methods
    void setSilentlyIgnoreInvalidCopyRequests( bool i_bExecuteSilently );
    bool silentlyIgnoreInvalidCopyRequests() const { return m_bSilentlyIgnoreInvalidCopyRequests; }
public: // instance methods (hiding not overridable methods with same name in QTreeView)
    virtual void expandAll();
    virtual void collapseAll();
public: // overridables
    virtual void expandRecursive( const QModelIndex& i_modelIdx );      // to distinguish from QTreeView method "expand".
    virtual void collapseRecursive( const QModelIndex& i_modelIdx );    // to distinguish from QTreeView method "collapse".
public: // slots (hiding not overridable slots with same name in QTreeView)
    virtual void expand( const QModelIndex& i_modelIdx );
    virtual void collapse( const QModelIndex& i_modelIdx );
public: // overridables of base class QTreeView (access from protect to public for test)
    QModelIndexList selectedIndexes() const override { return QTreeView::selectedIndexes(); }
protected slots:
    void onCollapsed( const QModelIndex& i_modelIdx );
    void onExpanded( const QModelIndex& i_modelIdx );
protected: // overridables of base class QTreeView
    virtual bool event( QEvent* i_pEv ) override;
protected: // overridables of base class QTreeView
    virtual void keyPressEvent( QKeyEvent* i_pEv ) override;
public: // overridables of base class QTreeView (public to pass mouse events from test steps)
    virtual void mousePressEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseReleaseEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseDoubleClickEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseMoveEvent( QMouseEvent* i_pEv ) override;
protected: // overridables of base class QTreeView
    virtual void startDrag( Qt::DropActions i_supportedActions );
    virtual void dragEnterEvent( QDragEnterEvent* i_pEv ) override;
    virtual void dragLeaveEvent( QDragLeaveEvent* i_pEv ) override;
    virtual void dragMoveEvent( QDragMoveEvent* i_pEv ) override;
    virtual void dropEvent( QDropEvent* i_pEv ) override;
protected slots:
    void onModelSortOrderChanged(EIdxTreeSortOrder i_sortOrder);
protected slots:
    void onActionBranchExpandTriggered( bool i_bChecked );
    void onActionBranchCollapseTriggered( bool i_bChecked );
protected slots:
    void onActionBranchCreateNewBranchTriggered( bool i_bChecked );
    void onActionBranchCreateNewLeaveTriggered( bool i_bChecked );
    void onActionBranchDeleteTriggered( bool i_bChecked );
    void onActionBranchCutTriggered( bool i_bChecked );
    void onActionBranchCopyTriggered( bool i_bChecked );
    void onActionBranchPasteTriggered( bool i_bChecked );
protected slots:
    void onActionLeaveDeleteTriggered( bool i_bChecked );
    void onActionLeaveCutTriggered( bool i_bChecked );
    void onActionLeaveCopyTriggered( bool i_bChecked );
    void onActionLeavePasteTriggered( bool i_bChecked );
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CIdxTree*         m_pIdxTree;
    CDelegateIdxTree* m_pDelegate;
    CModelIdxTree*    m_pModel;
    QMenu*      m_pMenuBranchContext;
    QAction*    m_pActionBranchTitle;
    QAction*    m_pActionBranchExpand;
    QAction*    m_pActionBranchCollapse;
    QAction*    m_pActionBranchCreateNewBranch;
    QAction*    m_pActionBranchCreateNewLeave;
    QAction*    m_pActionBranchDelete;
    QAction*    m_pActionBranchCut;
    QAction*    m_pActionBranchCopy;
    QAction*    m_pActionBranchPaste;
    QMenu*      m_pMenuLeaveContext;
    QAction*    m_pActionLeaveTitle;
    QAction*    m_pActionLeaveDelete;
    QAction*    m_pActionLeaveCut;
    QAction*    m_pActionLeaveCopy;
    QAction*    m_pActionLeavePaste;
    QModelIndex m_modelIdxSelectedOnMousePressEvent;
    QModelIndex m_modelIdxSelectedOnMouseReleaseEvent;
    QModelIndex m_modelIdxSelectedForPaste;
    EPasteMode  m_pasteMode;
    bool        m_bSilentlyExecuteDeleteRequests;
    bool        m_bSilentlyIgnoreInvalidCopyRequests;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTreeViewIdxTree

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_IdxTreeView_h
