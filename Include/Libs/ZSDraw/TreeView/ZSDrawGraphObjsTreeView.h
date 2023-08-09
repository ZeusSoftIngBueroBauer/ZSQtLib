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

#ifndef ZSDraw_GraphObjsTreeView_h
#define ZSDraw_GraphObjsTreeView_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qstyleditemdelegate.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qstyleditemdelegate.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "ZSDraw/TreeView/ZSDrawGraphObjsTreeModel.h"

class QLineEdit;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}
namespace Draw
{
class CGraphObj;
class CModelIdxTreeGraphObjs;

//******************************************************************************
class ZSDRAWDLL_API CDelegateGraphObjsTreeView : public QStyledItemDelegate
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CDelegateGraphObjsTreeView"; }
public: // ctors and dtor
    CDelegateGraphObjsTreeView( QObject* i_pObjParent = nullptr );
    virtual ~CDelegateGraphObjsTreeView();
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
    CDelegateGraphObjsTreeView( const CDelegateGraphObjsTreeView& );
private: // assignment operator not implemented
    CDelegateGraphObjsTreeView& operator = ( const CDelegateGraphObjsTreeView& );
private: // instance members
    QLineEdit* m_pEdtName;
    bool       m_bEdtNameDestroyedSignalConnected;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDelegateGraphObjsTreeView

//******************************************************************************
class ZSDRAWDLL_API CTreeViewGraphObjs : public QTreeView
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CTreeViewGraphObjs"; }
public: // ctors and dtor
    CTreeViewGraphObjs(
        CDrawingScene* i_pDrawingScene,
        QWidget*       i_pWdgtParent = nullptr );
    virtual ~CTreeViewGraphObjs();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setSortOrder( ZS::System::GUI::EIdxTreeSortOrder i_sortOrder );
    ZS::System::GUI::EIdxTreeSortOrder sortOrder() const;
public: // instance methods
    void setSilentlyExecuteDeleteRequests( bool i_bExecuteSilently );
    bool silentlyExecuteDeleteRequests() const { return m_bSilentlyExecuteDeleteRequests; }
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
protected slots:
    void onDrawingSceneSelectionChanged();
protected: // overridables of base class QTreeView
    virtual bool event( QEvent* i_pEv ) override;
protected: // overridables of base class QTreeView
    virtual void keyPressEvent( QKeyEvent* i_pEv ) override;
protected: // overridables of base class QTreeView
    virtual void mousePressEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseReleaseEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseDoubleClickEvent( QMouseEvent* i_pEv ) override;
    virtual void mouseMoveEvent( QMouseEvent* i_pEv ) override;
protected: // overridables of base class QTreeView
    virtual void selectionChanged( const QItemSelection& i_selected, const QItemSelection& i_deselected ) override;
protected slots:
    void onActionGraphObjExpandTriggered( bool i_bChecked );
    void onActionGraphObjCollapseTriggered( bool i_bChecked );
    void onActionGraphObjDeleteTriggered( bool i_bChecked );
protected: // instance members
    CDrawingScene* m_pDrawingScene;
    CDelegateGraphObjsTreeView* m_pDelegate;
    ZS::Draw::CModelIdxTreeGraphObjs* m_pModelIdxTreeGraphObjs;
    QMenu* m_pMenuGraphObjContext;
    QAction* m_pActionGraphObjTitle;
    QAction* m_pActionGraphObjExpand;
    QAction* m_pActionGraphObjCollapse;
    QAction* m_pActionGraphObjDelete;
    QModelIndex m_modelIdxSelectedOnMousePressEvent;
    QModelIndex m_modelIdxSelectedOnMouseReleaseEvent;
    bool m_bSilentlyExecuteDeleteRequests;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjEvent;

}; // class CTreeViewGraphObjs

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjsTreeView_h
