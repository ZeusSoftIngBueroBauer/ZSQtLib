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

#ifndef ZSDiagram_ItemsTreeView_h
#define ZSDiagram_ItemsTreeView_h

#include "ZSDiagramGUI/ZSDiagramGUIDllMain.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qstyleditemdelegate.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qstyleditemdelegate.h>
#include <QtWidgets/qtreeview.h>
#endif

class QLineEdit;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Diagram
{
class CWdgtDiagram;

namespace GUI
{
class CModelDiagramItems;

//******************************************************************************
class ZSDIAGRAMGUIDLL_API CDelegateDiagramObjsTreeView : public QStyledItemDelegate
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Diagram::GUI"; }
    static QString ClassName() { return "CDelegateDiagramObjsTreeView"; }
public: // ctors and dtor
    CDelegateDiagramObjsTreeView( QObject* i_pObjParent = nullptr );
    virtual ~CDelegateDiagramObjsTreeView();
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
    CDelegateDiagramObjsTreeView( const CDelegateDiagramObjsTreeView& );
private: // assignment operator not implemented
    CDelegateDiagramObjsTreeView& operator = ( const CDelegateDiagramObjsTreeView& );
private: // instance members
    QLineEdit* m_pEdtName;
    bool m_bEdtNameDestroyedSignalConnected;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDelegateDiagramObjsTreeView

//******************************************************************************
class ZSDIAGRAMGUIDLL_API CTreeViewDiagramItems : public QTreeView
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Diagram::GUI"; }
    static QString ClassName() { return "CTreeViewDiagramItems"; }
public: // ctors and dtor
    CTreeViewDiagramItems(CWdgtDiagram* i_pDiagram, QWidget* i_pWdgtParent = nullptr );
    virtual ~CTreeViewDiagramItems();
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
    //void onDiagramSelectionChanged();
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
    void onActionDiagramObjExpandTriggered( bool i_bChecked );
    void onActionDiagramObjCollapseTriggered( bool i_bChecked );
    void onActionDiagramObjDeleteTriggered( bool i_bChecked );
protected: // instance members
    CWdgtDiagram* m_pDiagram;
    CDelegateDiagramObjsTreeView* m_pDelegate;
    CModelDiagramItems* m_pModel;
    QMenu* m_pMenuDiagramObjContext;
    QAction* m_pActionDiagramObjTitle;
    QAction* m_pActionDiagramObjExpand;
    QAction* m_pActionDiagramObjCollapse;
    QAction* m_pActionDiagramObjDelete;
    QModelIndex m_modelIdxSelectedOnMousePressEvent;
    QModelIndex m_modelIdxSelectedOnMouseReleaseEvent;
    bool m_bSilentlyExecuteDeleteRequests;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjEvent;

}; // class CTreeViewDiagramItems

} // namespace GUI

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ItemsTreeView_h
