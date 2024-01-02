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

#ifndef ZSSysGUI_TrcAdminObjIdxTreeView_h
#define ZSSysGUI_TrcAdminObjIdxTreeView_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qstyleditemdelegate.h>
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qstyleditemdelegate.h>
#include <QtWidgets/qtreeview.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"
#include "ZSSys/ZSSysCommon.h"

namespace ZS
{
namespace System
{
class CIdxTreeTrcAdminObjs;
class CTrcAdminObj;

namespace GUI
{
class CModelIdxTreeTrcAdminObjs;

//******************************************************************************
class ZSSYSGUIDLL_API CDelegateIdxTreeTrcAdminObjs : public QStyledItemDelegate
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CDelegateIdxTreeTrcAdminObjs"; }
public: // ctors and dtor
    CDelegateIdxTreeTrcAdminObjs(QObject* i_pObjParent = nullptr);
    virtual ~CDelegateIdxTreeTrcAdminObjs();
public: // instance methods
    bool isCheckBoxEnabledHit(
        const QRect&       i_rectVisual,
        const QPoint&      i_ptPos,
        const QModelIndex& i_modelIdx );
public: // overridables of base class QItemDelegate
    virtual void paint(
        QPainter*                   i_pPainter,
        const QStyleOptionViewItem& i_styleOption,
        const QModelIndex&          i_modelIdx ) const;
    QWidget* createEditor( QWidget* i_pWdgtParent, const QStyleOptionViewItem& i_styleOption, const QModelIndex& i_modelIdx ) const;
    void setEditorData( QWidget* i_pWdgtEditor, const QModelIndex& i_modelIdx ) const;
    void setModelData(QWidget* i_pWdgtEditor, QAbstractItemModel* i_pModel, const QModelIndex& i_modelIdx ) const;
    void updateEditorGeometry( QWidget* i_pWdgtEditor, const QStyleOptionViewItem& i_styleOption, const QModelIndex& i_modelIdx ) const;
protected slots:
    void onComboDetailLevelActivated( int i_idx );
    void onEdtObjectNameFilterEditingFinished();
    void onEdtMethodNameFilterEditingFinished();
    void onEdtDataFilterEditingFinished();
private: // copy ctor not implemented
    CDelegateIdxTreeTrcAdminObjs( const CDelegateIdxTreeTrcAdminObjs& );
private: // assignment operator not implemented
    CDelegateIdxTreeTrcAdminObjs& operator = ( const CDelegateIdxTreeTrcAdminObjs& );
protected: // instance members
    mutable QRect m_rectChkBoxTraceEnabled;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDelegateIdxTreeTrcAdminObjs

//******************************************************************************
class ZSSYSGUIDLL_API CTreeViewIdxTreeTrcAdminObjs : public QTreeView
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CTreeViewIdxTreeTrcAdminObjs"; }
public: // ctors and dtor
    CTreeViewIdxTreeTrcAdminObjs(
        CIdxTreeTrcAdminObjs* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr );
    virtual ~CTreeViewIdxTreeTrcAdminObjs();
public: // instance methods (hiding not overridable methods with same name in QTreeView)
    virtual void expandAll();
    virtual void collapseAll();
public: // overridables
    virtual void expandRecursive( const QModelIndex& i_modelIdx );      // to distinguish from QTreeView method "expand".
    virtual void collapseRecursive( const QModelIndex& i_modelIdx );    // to distinguish from QTreeView method "collapse".
public: // slots (hiding not overridable slots with same name in QTreeView)
    virtual void expand( const QModelIndex& i_modelIdx );
    virtual void collapse( const QModelIndex& i_modelIdx );
protected slots:
    void onCollapsed( const QModelIndex& i_modelIdx );
    void onExpanded( const QModelIndex& i_modelIdx );
protected: // overridables of base class QTreeView
    virtual void keyPressEvent( QKeyEvent* i_pEv );
    virtual void mousePressEvent( QMouseEvent* i_pEv );
    virtual void mouseReleaseEvent( QMouseEvent* i_pEv );
protected slots:
    void onActionNameSpaceExpandTriggered( bool i_bChecked );
    void onActionNameSpaceCollapseTriggered( bool i_bChecked );
    void onActionNameSpaceEnableAdminObjsTriggered( bool i_bChecked );
    void onActionNameSpaceDisableAdminObjsTriggered( bool i_bChecked );
    void onActionNameSpaceSetAdminObjsMethodCallsDetailLevelTriggered( bool i_bChecked );
    void onActionNameSpaceSetAdminObjsRuntimeInfoDetailLevelTriggered( bool i_bChecked );
    void onActionNameSpaceSetAdminObjsObjectNameFilterTriggered( bool i_bChecked );
    void onActionNameSpaceSetAdminObjsMethodNameFilterTriggered( bool i_bChecked );
    void onActionNameSpaceSetAdminObjsTraceDataFilterTriggered( bool i_bChecked );
protected: // instance members
    CIdxTreeTrcAdminObjs* m_pIdxTree;
    CDelegateIdxTreeTrcAdminObjs* m_pDelegate;
    CModelIdxTreeTrcAdminObjs* m_pModel;
    QMenu* m_pMenuNameSpaceContext;
    QAction* m_pActionNameSpaceTitle;
    QAction* m_pActionNameSpaceExpand;
    QAction* m_pActionNameSpaceCollapse;
    QAction* m_pActionNameSpaceEnableAdminObjs;
    QAction* m_pActionNameSpaceDisableAdminObjs;
    QAction* m_pActionNameSpaceSetAdminObjsMethodCallsDetailLevel;
    QAction* m_pActionNameSpaceSetAdminObjsRuntimeInfoDetailLevel;
    QAction* m_pActionNameSpaceSetAdminObjsObjNameFilter;
    QAction* m_pActionNameSpaceSetAdminObjsMethodNameFilter;
    QAction* m_pActionNameSpaceSetAdminObjsTraceDataFilter;
    QModelIndex m_modelIdxSelectedOnMousePressEvent;
    QModelIndex m_modelIdxSelectedOnMouseReleaseEvent;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTreeViewIdxTreeTrcAdminObjs

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_TrcAdminObjIdxTreeView_h
