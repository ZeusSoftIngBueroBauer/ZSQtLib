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

#ifndef ZSPhysValGUI_PhysUnitsTreeView_h
#define ZSPhysValGUI_PhysUnitsTreeView_h

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
namespace System
{
namespace GUI
{
class CModelIdxTree;
}
}
namespace PhysVal
{
class CIdxTreeUnits;

namespace GUI
{
//******************************************************************************
class ZSPHYSVALGUIDLL_API CTreeViewUnits : public QTreeView
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CTreeViewUnits"; }
public: // ctors and dtor
    CTreeViewUnits(QWidget* i_pWdgtParent = nullptr);
    virtual ~CTreeViewUnits();
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
    CIdxTreeUnits* m_pIdxTree;
    ZS::System::GUI::CModelIdxTree* m_pModel;
    QMenu* m_pMenuNameSpaceContext;
    QAction* m_pActionNameSpaceTitle;
    QAction* m_pActionNameSpaceExpand;
    QAction* m_pActionNameSpaceCollapse;
    QModelIndex m_modelIdxSelectedOnMousePressEvent;
    QModelIndex m_modelIdxSelectedOnMouseReleaseEvent;

}; // class CTreeViewUnits

} // namespace GUI

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysValGUI_PhysUnitsTreeView_h
