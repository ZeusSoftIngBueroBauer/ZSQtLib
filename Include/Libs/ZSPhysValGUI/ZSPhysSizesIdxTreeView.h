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

#ifndef ZSPhysValGUI_PhysSizesIdxTreeView_h
#define ZSPhysValGUI_PhysSizesIdxTreeView_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qtreeview.h>
#else
#include <QtWidgets/qtreeview.h>
#endif

#include "ZSPhysValGUI/ZSPhysValGUIDllMain.h"
#include "ZSSys/ZSSysCommon.h"

namespace ZS
{
namespace PhysVal
{
class CIdxTreePhysSizes;

namespace GUI
{
class CModelIdxTreePhysSizes;

//******************************************************************************
class ZSPHYSVALGUIDLL_API CTreeViewIdxTreePhysSizes : public QTreeView
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CTreeViewIdxTreePhysSizes"; }
public: // ctors and dtor
    CTreeViewIdxTreePhysSizes(
        CIdxTreePhysSizes* i_pIdxTree,
        QWidget* i_pWdgtParent = nullptr );
    virtual ~CTreeViewIdxTreePhysSizes();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setIdxTree(CIdxTreePhysSizes* i_pIdxTree);
    CIdxTreePhysSizes* idxTree() { return m_pIdxTree; }
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
    virtual void mousePressEvent( QMouseEvent* i_pEv );
protected slots:
    void onActionNameSpaceExpandTriggered( bool i_bChecked );
    void onActionNameSpaceCollapseTriggered( bool i_bChecked );
protected slots:
    void onIdxTreeAboutToBeDestroyed();
protected: // instance members
    CIdxTreePhysSizes* m_pIdxTree;
    CModelIdxTreePhysSizes* m_pModel;
    QMenu* m_pMenuNameSpaceContext;
    QAction* m_pActionNameSpaceTitle;
    QAction* m_pActionNameSpaceExpand;
    QAction* m_pActionNameSpaceCollapse;
    QModelIndex m_modelIdxSelectedOnMousePressEvent;
    QModelIndex m_modelIdxSelectedOnMouseReleaseEvent;

}; // class CTreeViewIdxTreePhysSizes

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysSizesIdxTreeView_h
