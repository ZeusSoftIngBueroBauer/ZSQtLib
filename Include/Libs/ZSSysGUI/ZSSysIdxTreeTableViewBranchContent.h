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

#ifndef ZSSysGUI_IdxTreeTableViewBranchContent_h
#define ZSSysGUI_IdxTreeTableViewBranchContent_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qtableview.h>
#endif

#include "ZSSysGUI/ZSSysIdxTreeModel.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;

namespace GUI
{
class CModelIdxTreeBranchContent;

//******************************************************************************
class ZSSYSGUIDLL_API CTableViewIdxTreeBranchContent : public QTableView
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::System::GUI"; }
    static QString ClassName() { return "CTableViewIdxTreeBranchContent"; }
public: // ctors and dtor
    CTableViewIdxTreeBranchContent( CIdxTree* i_pIdxTree, QWidget* i_pWdgtParent = nullptr );
    virtual ~CTableViewIdxTreeBranchContent();
public: // overridables
    virtual QString nameSpace() const { return CTableViewIdxTreeBranchContent::NameSpace(); }
    virtual QString className() const { return CTableViewIdxTreeBranchContent::ClassName(); }
public: // instance methods
    void setIdxTree(CIdxTree* i_pIdxTree);
    CIdxTree* idxTree() { return m_pIdxTree; }
public: // overridables
    virtual void setKeyInTreeOfRootEntry( const QString& i_strKeyInTree );
    QString getKeyInTreeOfRootEntry() const;
public: // instance methods
    void setSortOrder( EIdxTreeSortOrder i_sortOrder );
    EIdxTreeSortOrder sortOrder() const;
protected: // overridables of base class QTreeView
    virtual void keyPressEvent( QKeyEvent* i_pEv );
    virtual void mousePressEvent( QMouseEvent* i_pEv );
    virtual void mouseReleaseEvent( QMouseEvent* i_pEv );
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
protected: // instance members
    CIdxTree* m_pIdxTree;
    CModelIdxTreeBranchContent* m_pModel;
    //QString m_strKeyInTreeOfRootEntry;
    QMenu*   m_pMenuBranchContext;
    QAction* m_pActionBranchTitle;
    QAction* m_pActionBranchCreateNewBranch;
    QAction* m_pActionBranchCreateNewLeave;
    QAction* m_pActionBranchDelete;
    QAction* m_pActionBranchCut;
    QAction* m_pActionBranchCopy;
    QAction* m_pActionBranchPaste;
    QMenu*   m_pMenuLeaveContext;
    QAction* m_pActionLeaveTitle;
    QAction* m_pActionLeaveDelete;
    QAction* m_pActionLeaveCut;
    QAction* m_pActionLeaveCopy;
    QAction* m_pActionLeavePaste;
    QModelIndex m_modelIdxSelectedOnMousePressEvent;
    QModelIndex m_modelIdxSelectedOnMouseReleaseEvent;
    /*!< Trace admin object to control trace outputs of the class. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CTableViewIdxTreeBranchContent

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_IdxTreeTableViewBranchContent_h
