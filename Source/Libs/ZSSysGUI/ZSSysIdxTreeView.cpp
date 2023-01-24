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

#include <QtCore/qglobal.h>
#include <QtGui/qevent.h>
#include <QtGui/qdrag.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlineedit.h>
#include <QtGui/qmenu.h>
#include <QtGui/qmessagebox.h>
#else
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qmessagebox.h>
#endif

#include "ZSSysGUI/ZSSysIdxTreeView.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSysGUI/ZSSysErrDlg.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CDelegateIdxTree : public QStyledItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDelegateIdxTree::CDelegateIdxTree( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QStyledItemDelegate(i_pObjParent),
    m_pEdtName(nullptr),
    m_bEdtNameDestroyedSignalConnected(false),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( QString(i_pObjParent == nullptr ? "IdxTree" : i_pObjParent->objectName()) );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Parent: " + QString(i_pObjParent == nullptr ? "nullptr" : i_pObjParent->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CDelegateIdxTree::~CDelegateIdxTree()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }

    m_pEdtName = nullptr;
    m_bEdtNameDestroyedSignalConnected = false;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QLineEdit* CDelegateIdxTree::nameColumnEditor()
//------------------------------------------------------------------------------
{
    return m_pEdtName;
}

/*==============================================================================
public: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
QWidget* CDelegateIdxTree::createEditor(
    QWidget*                    i_pWdgtParent,
    const QStyleOptionViewItem& i_option,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "createEditor",
        /* strMethodInArgs    */ strMthInArgs );

    QWidget* pWdgtEditor = nullptr;

    CDelegateIdxTree* pVThis = const_cast<CDelegateIdxTree*>(this);

    if( i_modelIdx.column() == CModelIdxTree::EColumnTreeEntryName )
    {
        pWdgtEditor = QStyledItemDelegate::createEditor(i_pWdgtParent, i_option, i_modelIdx);
        //pWdgtEditor = new CLineEditIdxTreeClmName(m_pTrcMthFile, m_mthTrcDetailLevel, i_pWdgtParent);

        pVThis->m_pEdtName = dynamic_cast<QLineEdit*>(pWdgtEditor);

        if( !m_bEdtNameDestroyedSignalConnected && pVThis->m_pEdtName != nullptr )
        {
            //pVThis->m_pEdtName->installEventFilter(pVThis);

            pVThis->m_bEdtNameDestroyedSignalConnected = QObject::connect(
                m_pEdtName, &QLineEdit::destroyed,
                this, &CDelegateIdxTree::onEdtNameDestroyed);
        }
    } // if( i_modelIdx.column() == CModelIdxTree::EColumnTreeEntryName )

    else
    {
        pWdgtEditor = QStyledItemDelegate::createEditor(i_pWdgtParent, i_option, i_modelIdx);
    }

    return pWdgtEditor;

} // createEditor

//------------------------------------------------------------------------------
void CDelegateIdxTree::setEditorData(
    QWidget*           i_pWdgtEditor,
    const QModelIndex& i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setEditorData",
        /* strMethodInArgs    */ strMthInArgs );

    return QStyledItemDelegate::setEditorData(i_pWdgtEditor, i_modelIdx);

} // setEditorData

//------------------------------------------------------------------------------
void CDelegateIdxTree::setModelData(
    QWidget*            i_pWdgtEditor,
    QAbstractItemModel* i_pModel,
    const QModelIndex&  i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs  = "Model: " + QString(i_pModel == nullptr ? "nullptr" : i_pModel->objectName());
        strMthInArgs += ", ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setModelData",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_pModel == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pModel == nullptr");
    }

    if( i_modelIdx.column() == CModelIdxTree::EColumnTreeEntryName )
    {
        CDelegateIdxTree* pVThis = const_cast<CDelegateIdxTree*>(this);

        pVThis->m_pEdtName = dynamic_cast<QLineEdit*>(i_pWdgtEditor);

        if( pVThis->m_pEdtName != nullptr )
        {
            QString strName = pVThis->m_pEdtName->text();

            CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(i_pModel);

            SErrResultInfo errResultInfo = pModelIdxTree->canSetData(i_modelIdx, strName, Qt::EditRole);

            if( errResultInfo.isErrorResult() )
            {
                CDlgErr* pDlgErr = new CDlgErr("Error");
                pDlgErr->setErrResultInfo(errResultInfo);
                pDlgErr->show();
            }
            else // if( !errResultInfo.isErrorResult() )
            {
                pModelIdxTree->setData(i_modelIdx, strName, Qt::EditRole);
            }
        } // if( pVThis->m_pEdtName != nullptr )
    } // if( i_modelIdx.column() == CModelIdxTree::EColumnTreeEntryName )

} // setModelData

//------------------------------------------------------------------------------
void CDelegateIdxTree::updateEditorGeometry(
    QWidget*                    i_pWdgtEditor,
    const QStyleOptionViewItem& i_option,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "updateEditorGeometry",
        /* strMethodInArgs    */ strMthInArgs );

    return QStyledItemDelegate::updateEditorGeometry(i_pWdgtEditor, i_option, i_modelIdx);

} // updateEditorGeometry

/*==============================================================================
protected: // slots
==============================================================================*/

//------------------------------------------------------------------------------
void CDelegateIdxTree::onEdtNameDestroyed( QObject* /*i_pWdgtEditor*/ )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onEdtNameDestroyed",
        /* strMethodInArgs    */ "" );

    m_bEdtNameDestroyedSignalConnected = false;
    m_pEdtName = nullptr;

} // onEdtNameDestroyed


/*******************************************************************************
class CTreeViewIdxTree : public QTreeView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewIdxTree::CTreeViewIdxTree( CIdxTree* i_pIdxTree, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pIdxTree(i_pIdxTree),
    m_pDelegate(nullptr),
    m_pModel(nullptr),
    m_pMenuBranchContext(nullptr),
    m_pActionBranchTitle(nullptr),
    m_pActionBranchExpand(nullptr),
    m_pActionBranchCollapse(nullptr),
    m_pActionBranchCreateNewBranch(nullptr),
    m_pActionBranchCreateNewLeave(nullptr),
    m_pActionBranchDelete(nullptr),
    m_pActionBranchCut(nullptr),
    m_pActionBranchCopy(nullptr),
    m_pActionBranchPaste(nullptr),
    m_pMenuLeaveContext(nullptr),
    m_pActionLeaveTitle(nullptr),
    m_pActionLeaveDelete(nullptr),
    m_pActionLeaveCut(nullptr),
    m_pActionLeaveCopy(nullptr),
    m_pActionLeavePaste(nullptr),
    m_modelIdxSelectedOnMousePressEvent(),
    m_modelIdxSelectedOnMouseReleaseEvent(),
    m_modelIdxSelectedForPaste(),
    m_pasteMode(EPasteMode::Undefined),
    m_bSilentlyExecuteDeleteRequests(false),
    m_bSilentlyIgnoreInvalidCopyRequests(false),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    m_pModel = new CModelIdxTree(nullptr, true);

    setModel(m_pModel);

    QObject::connect(
        m_pModel, static_cast<void (CModelIdxTree::*)(EIdxTreeSortOrder)>(&CModelIdxTree::sortOrderChanged),
        this, &CTreeViewIdxTree::onModelSortOrderChanged );

    m_pDelegate = new CDelegateIdxTree(this);

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAllColumnsShowFocus(true);

    setItemDelegate(m_pDelegate);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);

    // Context Popup Menu of Branches
    //-------------------------------

    m_pMenuBranchContext = new QMenu("Branch", this);

    m_pActionBranchTitle = new QAction("Branch:", this);

    QFont fntActionTitle = m_pActionBranchTitle->font();
    fntActionTitle.setBold(true);
    m_pActionBranchTitle->setFont(fntActionTitle);
    m_pMenuBranchContext->addAction(m_pActionBranchTitle);

    m_pMenuBranchContext->addSeparator();

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.png");
    m_pActionBranchExpand = new QAction(pxmExpandAll, "Expand", this);
    m_pMenuBranchContext->addAction(m_pActionBranchExpand);

    QObject::connect(
        m_pActionBranchExpand, &QAction::triggered,
        this, &CTreeViewIdxTree::onActionBranchExpandTriggered );

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.png");
    m_pActionBranchCollapse = new QAction(pxmCollapseAll, "Collapse", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCollapse);

    QObject::connect(
        m_pActionBranchCollapse, &QAction::triggered,
        this, &CTreeViewIdxTree::onActionBranchCollapseTriggered );

    m_pMenuBranchContext->addSeparator();

    m_pActionBranchCreateNewBranch = new QAction("Create New Branch", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCreateNewBranch);

    QObject::connect(
        m_pActionBranchCreateNewBranch, &QAction::triggered,
        this, &CTreeViewIdxTree::onActionBranchCreateNewBranchTriggered );

    m_pActionBranchCreateNewLeave = new QAction("Create New Leave", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCreateNewLeave);

    QObject::connect(
        m_pActionBranchCreateNewLeave, &QAction::triggered,
        this, &CTreeViewIdxTree::onActionBranchCreateNewLeaveTriggered );

    m_pActionBranchDelete = new QAction("Delete", this);
    m_pMenuBranchContext->addAction(m_pActionBranchDelete);

    QObject::connect(
        m_pActionBranchDelete, &QAction::triggered,
        this, &CTreeViewIdxTree::onActionBranchDeleteTriggered );

    m_pMenuBranchContext->addSeparator();

    m_pActionBranchCut = new QAction("Cut", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCut);

    QObject::connect(
        m_pActionBranchCut, &QAction::triggered,
        this, &CTreeViewIdxTree::onActionBranchCutTriggered );

    m_pActionBranchCopy = new QAction("Copy", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCopy);

    QObject::connect(
        m_pActionBranchCopy, &QAction::triggered,
        this, &CTreeViewIdxTree::onActionBranchCopyTriggered );

    m_pActionBranchPaste = new QAction("Paste", this);
    m_pMenuBranchContext->addAction(m_pActionBranchPaste);

    QObject::connect(
        m_pActionBranchPaste, &QAction::triggered,
        this, &CTreeViewIdxTree::onActionBranchPasteTriggered );

    // Context Popup Menu of Leaves
    //-------------------------------

    m_pMenuLeaveContext = new QMenu("Leave", this);

    m_pActionLeaveTitle = new QAction("Leave:", this);

    m_pActionLeaveTitle->setFont(fntActionTitle);
    m_pMenuLeaveContext->addAction(m_pActionLeaveTitle);

    m_pMenuLeaveContext->addSeparator();

    m_pActionLeaveDelete = new QAction("Delete", this);
    m_pMenuLeaveContext->addAction(m_pActionLeaveDelete);

    QObject::connect(
        m_pActionLeaveDelete, &QAction::triggered,
        this, &CTreeViewIdxTree::onActionLeaveDeleteTriggered );

    m_pMenuLeaveContext->addSeparator();

    m_pActionLeaveCut = new QAction("Cut", this);
    m_pMenuLeaveContext->addAction(m_pActionLeaveCut);

    QObject::connect(
        m_pActionLeaveCut, &QAction::triggered,
        this, &CTreeViewIdxTree::onActionLeaveCutTriggered );

    m_pActionLeaveCopy = new QAction("Copy", this);
    m_pMenuLeaveContext->addAction(m_pActionLeaveCopy);

    QObject::connect(
        m_pActionLeaveCopy, &QAction::triggered,
        this, &CTreeViewIdxTree::onActionLeaveCopyTriggered );

    m_pActionLeavePaste = new QAction("Paste", this);
    m_pMenuLeaveContext->addAction(m_pActionLeavePaste);

    QObject::connect(
        m_pActionLeavePaste, &QAction::triggered,
        this, &CTreeViewIdxTree::onActionLeavePasteTriggered );

    // Connect to signals of the tree view
    //------------------------------------

    QObject::connect(
        this, &CTreeViewIdxTree::collapsed,
        this, &CTreeViewIdxTree::onCollapsed );
    QObject::connect(
        this, &CTreeViewIdxTree::expanded,
        this, &CTreeViewIdxTree::onExpanded );

    if( i_pIdxTree != nullptr )
    {
        setIdxTree(i_pIdxTree);
    }
} // ctor

//------------------------------------------------------------------------------
CTreeViewIdxTree::~CTreeViewIdxTree()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    try
    {
        delete m_pModel;
    }
    catch(...)
    {
    }

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }

    m_pIdxTree = nullptr;
    m_pDelegate = nullptr;
    m_pModel = nullptr;
    m_pMenuBranchContext = nullptr;
    m_pActionBranchTitle = nullptr;
    m_pActionBranchExpand = nullptr;
    m_pActionBranchCollapse = nullptr;
    m_pActionBranchCreateNewBranch = nullptr;
    m_pActionBranchCreateNewLeave = nullptr;
    m_pActionBranchDelete = nullptr;
    m_pActionBranchCut = nullptr;
    m_pActionBranchCopy = nullptr;
    m_pActionBranchPaste = nullptr;
    m_pMenuLeaveContext = nullptr;
    m_pActionLeaveTitle = nullptr;
    m_pActionLeaveDelete = nullptr;
    m_pActionLeaveCut = nullptr;
    m_pActionLeaveCopy = nullptr;
    m_pActionLeavePaste = nullptr;
    //m_modelIdxSelectedOnMousePressEvent;
    //m_modelIdxSelectedOnMouseReleaseEvent;
    //m_modelIdxSelectedForPaste;
    m_pasteMode = static_cast<EPasteMode>(0);
    m_bSilentlyExecuteDeleteRequests = false;
    m_bSilentlyIgnoreInvalidCopyRequests = false;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::setIdxTree( CIdxTree* i_pIdxTree )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = QString(i_pIdxTree == nullptr ? "null" : i_pIdxTree->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setIdxTree",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pIdxTree != i_pIdxTree )
    {
        if( m_pIdxTree != nullptr )
        {
            QObject::disconnect(
                m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
                this, &CTreeViewIdxTree::onIdxTreeAboutToBeDestroyed);
        }

        m_pIdxTree = i_pIdxTree;

        if( m_pIdxTree != nullptr )
        {
            QObject::connect(
                m_pIdxTree, &CIdxTree::aboutToBeDestroyed,
                this, &CTreeViewIdxTree::onIdxTreeAboutToBeDestroyed);
        }

        m_pModel->setIdxTree(i_pIdxTree);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::setExcludeLeaves( bool i_bExcludeLeaves )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bExcludeLeaves);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setExcludeLeaves",
        /* strMethodInArgs    */ strMthInArgs );

    m_pModel->setExcludeLeaves(i_bExcludeLeaves);

} // setExcludeLeaves

//------------------------------------------------------------------------------
bool CTreeViewIdxTree::areLeavesExcluded() const
//------------------------------------------------------------------------------
{
    return m_pModel->areLeavesExcluded();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::setSortOrder( EIdxTreeSortOrder i_sortOrder )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = idxTreeSortOrder2Str(i_sortOrder);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setSortOrder",
        /* strMethodInArgs    */ strMthInArgs );

    if( static_cast<int>(i_sortOrder) < 0 || i_sortOrder >= EIdxTreeSortOrder::Count )
    {
        QString strAddErrInfo = "SortOrder: " + idxTreeSortOrder2Str(i_sortOrder);
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo);
    }

    // On changing the sort order the model will be changed.
    // The childs of the branches will be removed and inserted again
    // wherupon the tree view nodes will be collapsed.
    // After sorting the model formerly expanded tree nodes may be collapsed.
    // This is not what I want. To restore the tree view concerning the
    // expanded states of the nodes the exanded state of the model nodes
    // is saved. After the model has been sorted the expanded states are recalled.

    QMap<QString, CModelIdxTreeEntry*> mappTreeEntries = m_pModel->treeEntriesMap();

    QStringList strlstKeysOfExpandedModelEntries;

    CModelIdxTreeEntry* pModelBranch;

    for( auto pModelTreeEntry : mappTreeEntries )
    {
        if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
        {
            pModelBranch = pModelTreeEntry;

            if( pModelBranch->isExpanded() && m_pModel->areAllParentBranchesExpanded(pModelBranch) )
            {
                strlstKeysOfExpandedModelEntries << pModelTreeEntry->keyInTree();
            }
        }
    }

    m_pModel->setSortOrder(i_sortOrder);

    QModelIndex modelIdx;

    for( auto strKey : strlstKeysOfExpandedModelEntries )
    {
        modelIdx = m_pModel->index(strKey, 0);
        expand(modelIdx);
    }

} // setSortOrder

//------------------------------------------------------------------------------
EIdxTreeSortOrder CTreeViewIdxTree::sortOrder() const
//------------------------------------------------------------------------------
{
    return m_pModel->sortOrder();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::setSilentlyExecuteDeleteRequests( bool i_bExecuteSilently )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bExecuteSilently);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setSilentlyExecuteDeleteRequests",
        /* strMethodInArgs    */ strMthInArgs );

    m_bSilentlyExecuteDeleteRequests = i_bExecuteSilently;
}

//------------------------------------------------------------------------------
void CTreeViewIdxTree::setSilentlyIgnoreInvalidCopyRequests( bool i_bExecuteSilently )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bExecuteSilently);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "setSilentlyIgnoreInvalidCopyRequests",
        /* strMethodInArgs    */ strMthInArgs );

    m_bSilentlyIgnoreInvalidCopyRequests = i_bExecuteSilently;
}

/*==============================================================================
public: // overridable slots of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::expandAll()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "expandAll",
        /* strMethodInArgs    */ strMthInArgs );

    QTreeView::expandAll();

    QModelIndex modelIdxRoot = m_pModel->index(0, 0, QModelIndex());

    expandRecursive(modelIdxRoot);
}

//------------------------------------------------------------------------------
void CTreeViewIdxTree::collapseAll()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "collapseAll",
        /* strMethodInArgs    */ "" );

    QTreeView::collapseAll();

    QModelIndex modelIdxRoot = m_pModel->index(0, 0, QModelIndex());

    collapseRecursive(modelIdxRoot);
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::expandRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "expandRecursive",
        /* strMethodInArgs    */ strMthInArgs );

    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
        {
            expand(i_modelIdx);

            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            CModelIdxTreeEntry* pModelTreeEntryChild;

            QModelIndex modelIdxChild;

            for( int idxEntry = 0; idxEntry < pModelBranch->count(); ++idxEntry )
            {
                pModelTreeEntryChild = pModelBranch->at(idxEntry);

                if( pModelTreeEntryChild->isRoot() || pModelTreeEntryChild->isBranch() )
                {
                    modelIdxChild = m_pModel->index(idxEntry, 0, i_modelIdx);
                    expandRecursive(modelIdxChild);
                }
            }
        } // if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
    } // if( pModelTreeEntry != nullptr )
} // expandRecursive

//------------------------------------------------------------------------------
void CTreeViewIdxTree::collapseRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "collapseRecursive",
        /* strMethodInArgs    */ strMthInArgs );

    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
        {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            QModelIndex modelIdx;

            for( int idxEntry = 0; idxEntry < pModelBranch->count(); ++idxEntry )
            {
                pModelTreeEntry = pModelBranch->at(idxEntry);

                if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
                {
                    modelIdx = m_pModel->index(idxEntry, 0, i_modelIdx);
                    collapseRecursive(modelIdx);
                }
            }

            collapse(i_modelIdx);

        } // if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
    } // if( pModelTreeEntry != nullptr )
} // collapseRecursive

/*==============================================================================
public: // slots (hiding not overridable slots with same name in QTreeView)
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::expand( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "expand",
        /* strMethodInArgs    */ strMthInArgs );

    // If calling "expandAll" the signal "expanded" is not invoked for all
    // nodes of the tree view. This is a workaround to store the "expanded"
    // state of the model tree node in order to restore this state on changing
    // the sort order of the model.

    QTreeView::expand(i_modelIdx);

    if( i_modelIdx.isValid() )
    {
        onExpanded(i_modelIdx);
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTree::collapse( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "collapse",
        /* strMethodInArgs    */ strMthInArgs );

    // If calling "collapseAll" the signal "collapsed" is not invoked for all
    // nodes of the tree view. This is a workaround to store the "expanded"
    // state of the model tree node in order to restore this state on changing
    // the sort order of the model.

    QTreeView::collapse(i_modelIdx);

    if( i_modelIdx.isValid() )
    {
        onCollapsed(i_modelIdx);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onCollapsed( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onCollapsed",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_modelIdx.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
        {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            m_pModel->setIsExpanded(pModelBranch, false);
        }
    } // if( i_modelIdx.isValid() )
} // onCollapsed

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onExpanded",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_modelIdx.isValid() )
    {
        #if QT_VERSION >= 0x040000 && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        //resizeColumnToContents(i_modelIdx.column());
        #elif QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        // Cannot invoke resizeColumnToContents as this leads to an exception.
        #endif

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
        {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            m_pModel->setIsExpanded(pModelBranch, true);
        }
    } // if( i_modelIdx.isValid() )
} // onExpanded

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CTreeViewIdxTree::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "event",
        /* strMethodInArgs    */ strMthInArgs );

    return QTreeView::event(i_pEv);
}

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qKeyEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "keyPressEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    QItemSelectionModel* pSelModel = selectionModel();

    QModelIndex modelIdxSelected = pSelModel->currentIndex();

    if( modelIdxSelected.isValid() )
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::modelIdx2Str(modelIdxSelected) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }

        CModelIdxTreeEntry* pModelTreeEntrySelected = static_cast<CModelIdxTreeEntry*>(modelIdxSelected.internalPointer());

        if( pModelTreeEntrySelected != nullptr )
        {
            switch( i_pEv->key() )
            {
                case Qt::Key_Enter:
                {
                    break;
                }
                case Qt::Key_F2: // Rename
                {
                    // Handled by delegate.
                    break;
                }
                case Qt::Key_Escape: // Undo selection
                {
                    // Not handled by TreeView but BranchContentView.
                    break;
                }
                case Qt::Key_Delete:
                {
                    int iRet = QMessageBox::Yes;

                    if( !m_bSilentlyExecuteDeleteRequests )
                    {
                        QString strMsg = "Do you really want to delete \"" + pModelTreeEntrySelected->keyInTree() + "\"?";
                        iRet = QMessageBox::question(
                            /* pWdgtParent     */ this,
                            /* strTitle        */ getMainWindowTitle(),
                            /* strText         */ strMsg,
                            /* standardButtons */ QMessageBox::Yes | QMessageBox::No,
                            /* defaultButton   */ QMessageBox::No );
                    }
                    if( iRet == QMessageBox::Yes )
                    {
                        CIdxTreeEntry* pTreeEntry = pModelTreeEntrySelected->getIdxTreeEntry();
                        // If the tree entry is a branch the dtor also destroys all childs.
                        // The dtor will also remove the tree entry from the index tree emitting
                        // the "aboutToBeDestroyed" signal wherupon the model destroys the model entry.
                        delete pTreeEntry;
                        pTreeEntry = nullptr;
                        pModelTreeEntrySelected = nullptr;
                    }
                    bEventHandled = true;
                    break;
                }
                case Qt::Key_C: // Copy
                {
                    if( i_pEv->modifiers() & Qt::ControlModifier )
                    {
                        m_modelIdxSelectedForPaste = modelIdxSelected;
                        m_pasteMode = EPasteMode::Copy;
                        bEventHandled = true;
                    }
                    break;
                }
                case Qt::Key_X: // Cut
                {
                    if( i_pEv->modifiers() & Qt::ControlModifier )
                    {
                        m_modelIdxSelectedForPaste = modelIdxSelected;
                        m_pasteMode = EPasteMode::Cut;
                        bEventHandled = true;
                    }
                    break;
                }
                case Qt::Key_V: // Paste
                {
                    if( i_pEv->modifiers() & Qt::ControlModifier )
                    {
                        if( m_modelIdxSelectedForPaste.isValid() )
                        {
                            CModelIdxTreeEntry* pModelBranchTrg = nullptr;

                            if( pModelTreeEntrySelected->isLeave() )
                            {
                                pModelBranchTrg = pModelTreeEntrySelected->parentBranch();
                            }
                            else
                            {
                                pModelBranchTrg = pModelTreeEntrySelected;
                            }

                            if( pModelBranchTrg != nullptr )
                            {
                                CIdxTreeLocker idxTreeLocker(m_pIdxTree);

                                CModelIdxTreeEntry* pModelTreeEntrySrc = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedForPaste.internalPointer());
                                CIdxTreeEntry*      pTreeEntrySrc = pModelTreeEntrySrc->getIdxTreeEntry();
                                CIdxTreeEntry*      pBranchTrg    = pModelBranchTrg->getIdxTreeEntry();

                                int idxInTargetBranch = -1;

                                if( m_pModel->sortOrder() == EIdxTreeSortOrder::Config )
                                {
                                    idxInTargetBranch = m_modelIdxSelectedForPaste.row();
                                }
                                else
                                {
                                    idxInTargetBranch = pTreeEntrySrc->indexInParentBranch();
                                }

                                if( m_pasteMode == EPasteMode::Copy )
                                {
                                    SErrResultInfo errResultInfo = m_pIdxTree->canCopy(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);

                                    if( errResultInfo.isErrorResult() )
                                    {
                                        if( !m_bSilentlyIgnoreInvalidCopyRequests )
                                        {
                                            CDlgErr* pDlgErr = new CDlgErr("Error");
                                            pDlgErr->setErrResultInfo(errResultInfo);
                                            pDlgErr->show();
                                        }
                                    }
                                    else // if( !errResultInfo.isErrorResult() )
                                    {
                                        m_pIdxTree->copy(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);
                                    }
                                }
                                else if( m_pasteMode == EPasteMode::Cut )
                                {
                                    SErrResultInfo errResultInfo = m_pIdxTree->canMove(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);

                                    if( errResultInfo.isErrorResult() )
                                    {
                                        if( !m_bSilentlyIgnoreInvalidCopyRequests )
                                        {
                                            CDlgErr* pDlgErr = new CDlgErr("Error");
                                            pDlgErr->setErrResultInfo(errResultInfo);
                                            pDlgErr->show();
                                        }
                                    }
                                    else // if( !errResultInfo.isErrorResult() )
                                    {
                                        m_pIdxTree->move(pTreeEntrySrc, pBranchTrg);
                                    }
                                }
                            } // if( pModelBranchTrg != nullptr )

                            m_modelIdxSelectedForPaste = QModelIndex();
                            m_pasteMode = EPasteMode::Undefined;

                        } // if( m_modelIdxSelectedForPaste.isValid() )
                        bEventHandled = true;
                    } // if( i_pEv->modifiers() & Qt::ControlModifier )
                    break;
                } // case Qt::Key_V
                default:
                {
                    break;
                }
            } // switch( i_pEv->key() )
        } // if( pModelTreeEntrySelected != nullptr )
    } // if( modelIdxSelected.isValid() )

    if( !bEventHandled )
    {
        QTreeView::keyPressEvent(i_pEv);
    }

} // keyPressEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTree::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qMouseEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "mousePressEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMousePressEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxPressed {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }

        if( pModelTreeEntry != nullptr )
        {
            if( i_pEv->buttons() & Qt::RightButton )
            {
                if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
                {
                    if( m_pMenuBranchContext != nullptr )
                    {
                        m_pActionBranchTitle->setText( "Branch: " + pModelTreeEntry->name() );

                        if( m_modelIdxSelectedForPaste.isValid() )
                        {
                            m_pActionBranchPaste->setEnabled(true);
                        }
                        else
                        {
                            m_pActionBranchPaste->setEnabled(false);
                        }
                        m_pMenuBranchContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                    }
                    bEventHandled = true;

                } // if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )

                else if( pModelTreeEntry->isLeave() )
                {
                    if( m_pMenuLeaveContext != nullptr )
                    {
                        m_pActionLeaveTitle->setText( "Leave: " + pModelTreeEntry->name() );

                        if( m_modelIdxSelectedForPaste.isValid() )
                        {
                            m_pActionLeavePaste->setEnabled(true);
                        }
                        else
                        {
                            m_pActionLeavePaste->setEnabled(false);
                        }
                        m_pMenuLeaveContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                    }
                    bEventHandled = true;

                } // if( pModelTreeEntry->isLeave() )
            } // if( i_pEv->buttons() & Qt::RightButton )
        } // if( pModelTreeEntry != nullptr )
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

    if( !bEventHandled )
    {
        QTreeView::mousePressEvent(i_pEv);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTree::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qMouseEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "mouseReleaseEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMouseReleaseEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMouseReleaseEvent.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxReleased {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMouseReleaseEvent) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }

        if( pModelTreeEntry != nullptr )
        {
            if( i_pEv->buttons() & Qt::LeftButton )
            {
            } // if( i_pEv->buttons() & Qt::LeftButton )
        } // if( pModelTreeEntry != nullptr )
    } // if( m_modelIdxSelectedOnMouseReleaseEvent.isValid() )

    if( !bEventHandled )
    {
        QTreeView::mouseReleaseEvent(i_pEv);
    }
} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTree::mouseDoubleClickEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qMouseEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "mouseDoubleClickEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    if( !bEventHandled )
    {
        QTreeView::mouseDoubleClickEvent(i_pEv);
    }
} // mouseDoubleClickEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTree::mouseMoveEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qMouseEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "mouseMoveEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    if( !bEventHandled )
    {
        QTreeView::mouseMoveEvent(i_pEv);
    }
} // mouseMoveEvent

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::startDrag( Qt::DropActions i_supportedActions )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "SupportedActions: " + qDropActions2Str(i_supportedActions);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "startDrag",
        /* strMethodInArgs    */ strMthInArgs );

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // See Qt Centre
    // QUESTION: Where can I fix the QTreeView's default drop action?
    // Is there some kind of compatibility switch at Application level?
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    QModelIndexList indexes = selectedIndexes();
    QList<QPersistentModelIndex> persistentIndexes;
 
    if( indexes.count() > 0 )
    {
        QMimeData* data = m_pModel->mimeData(indexes);

        if( data == nullptr )
        {
            return;
        }

        for( int i = 0; i < indexes.count(); i++ )
        {
            QModelIndex idx = indexes.at(i);
            //qDebug() << "\tDragged item to delete" << i << " is: \"" << idx.data(NODE_TITLE).toString() << "\"";
            //qDebug() << "Row is: " << idx.row();
            persistentIndexes.append(QPersistentModelIndex(idx));
        }
 
        QPixmap pixmap = indexes.first().data(Qt::DecorationRole).value<QPixmap>();

        QDrag* drag = new QDrag(this);

        drag->setPixmap(pixmap);
        drag->setMimeData(data);
        drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
 
        Qt::DropAction defaultDropAction = Qt::IgnoreAction;
        if( i_supportedActions & Qt::MoveAction && dragDropMode() != QAbstractItemView::InternalMove )
        {
            defaultDropAction = Qt::MoveAction; //was Qt::CopyAction THIS WAS THE CULPRIT!
        }

        if( mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsVerbose) )
        {
            strMthInArgs = "SupportedActions: " + qDropActions2Str(i_supportedActions);
            strMthInArgs += ", DefaultAction: " + qDropAction2Str(defaultDropAction);
            mthTracer.trace("-> drag.exec(" + strMthInArgs + ")");
        }

        Qt::DropAction dropActionDragExec = drag->exec(i_supportedActions, defaultDropAction);

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = qDropAction2Str(dropActionDragExec);
            mthTracer.trace("<- drag.exec(" + strMthInArgs + ")", ELogDetailLevel::Debug);
        }

        if( dropActionDragExec == Qt::MoveAction )
        {
            //when we get here any copying done in dropMimeData has messed up our selected indexes
            //that's why we use persistent indexes
            for( int i = 0; i < indexes.count(); i++ )
            {
                QPersistentModelIndex idx = persistentIndexes.at(i);
                //qDebug() << "\tDragged item to delete" << i << " is: " << idx.data(NODE_TITLE).toString();
                //qDebug() << "Row is: " << idx.row();
                if (idx.isValid()) //the item is not top level
                {
                    m_pModel->removeRow(idx.row(), idx.parent());
                }
                else
                {
                    m_pModel->removeRow(idx.row(), QModelIndex());
                }
            } // for( int i = 0; i < indexes.count(); i++ )
        } // if( drag->exec(i_supportedActions, defaultDropAction) == Qt::MoveAction )
    } // if( indexes.count() > 0 )

} // startDrag

//------------------------------------------------------------------------------
void CTreeViewIdxTree::dragEnterEvent( QDragEnterEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qDragEnterEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dragEnterEvent",
        /* strMethodInArgs    */ strMthInArgs );

    QTreeView::dragEnterEvent(i_pEv);
}

//------------------------------------------------------------------------------
void CTreeViewIdxTree::dragLeaveEvent( QDragLeaveEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qDragLeaveEvent2Str(i_pEv)  + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dragLeaveEvent",
        /* strMethodInArgs    */ strMthInArgs );

    QTreeView::dragLeaveEvent(i_pEv);
}

//------------------------------------------------------------------------------
void CTreeViewIdxTree::dragMoveEvent( QDragMoveEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qDragMoveEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dragMoveEvent",
        /* strMethodInArgs    */ strMthInArgs );

    QTreeView::dragMoveEvent(i_pEv);
}

//------------------------------------------------------------------------------
void CTreeViewIdxTree::dropEvent( QDropEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "{" + qDropEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dropEvent",
        /* strMethodInArgs    */ strMthInArgs );

    QTreeView::dropEvent(i_pEv);

} // dropEvent

/*==============================================================================
protected: // slots
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onModelSortOrderChanged(EIdxTreeSortOrder i_sortOrder)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = idxTreeSortOrder2Str(i_sortOrder);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onModelSortOrderChanged",
        /* strMethodInArgs    */ strMthInArgs );

    emit sortOrderChanged(i_sortOrder);
    emit sortOrderChanged(idxTreeSortOrder2Str(i_sortOrder));
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onActionBranchExpandTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchExpandTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        expandRecursive(m_modelIdxSelectedOnMousePressEvent);

        for( int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++ )
        {
            resizeColumnToContents(idxClm);
        }
    }
} // onActionBranchExpandTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onActionBranchCollapseTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchCollapseTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        collapseRecursive(m_modelIdxSelectedOnMousePressEvent);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onActionBranchCreateNewBranchTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchCreateNewBranchTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

        if( m_pIdxTree != nullptr && pModelBranch != nullptr )
        {
            CIdxTreeLocker idxTreeLocker(m_pIdxTree);

            CIdxTreeEntry* pBranch = pModelBranch->getIdxTreeEntry();

            QString strName = "New Branch";
            QString strUniqueName = strName;
            int iCopies = 1;

            QString strEntryTypeSymbol =
                CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Branch, EEnumEntryAliasStrSymbol);
            while( pBranch->indexOf(strEntryTypeSymbol, strUniqueName) >= 0 )
            {
                strUniqueName = strName + QString::number(++iCopies);
            }
            strName = strUniqueName;

            CIdxTreeEntry* pBranchNew = m_pIdxTree->createBranch(strName);

            m_pIdxTree->add(pBranchNew, pBranch);
        }
    }
} // onActionBranchCreateNewBranchTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onActionBranchCreateNewLeaveTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchCreateNewLeaveTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

        if( m_pIdxTree != nullptr && pModelBranch != nullptr )
        {
            CIdxTreeLocker idxTreeLocker(m_pIdxTree);

            CIdxTreeEntry* pBranch = pModelBranch->getIdxTreeEntry();

            QString strName = "New Leave";
            QString strUniqueName = strName;
            int iCopies = 1;

            QString strEntryTypeSymbol =
                CIdxTreeEntry::entryType2Str(CIdxTreeEntry::EEntryType::Leave, EEnumEntryAliasStrSymbol);
            while( pBranch->indexOf(strEntryTypeSymbol, strUniqueName) >= 0 )
            {
                strUniqueName = strName + QString::number(++iCopies);
            }
            strName = strUniqueName;

            CIdxTreeEntry* pLeaveNew = m_pIdxTree->createLeave(strName);

            m_pIdxTree->add(pLeaveNew, pBranch);
        }
    }
} // onActionBranchCreateNewLeaveTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onActionBranchDeleteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchDeleteTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

        if( m_pIdxTree != nullptr && pModelBranch != nullptr )
        {
            CIdxTreeLocker idxTreeLocker(m_pIdxTree);

            CIdxTreeEntry* pBranch = pModelBranch->getIdxTreeEntry();

            delete pBranch;
            pBranch = nullptr;
        }
    }
} // onActionBranchDeleteTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onActionBranchCutTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchCutTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        m_modelIdxSelectedForPaste = m_modelIdxSelectedOnMousePressEvent;
        m_pasteMode = EPasteMode::Cut;

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedForPaste) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }
    }
} // onActionBranchCutTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onActionBranchCopyTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchCopyTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        m_modelIdxSelectedForPaste = m_modelIdxSelectedOnMousePressEvent;
        m_pasteMode = EPasteMode::Copy;

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedForPaste) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }
    }
} // onActionBranchCopyTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onActionBranchPasteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionBranchPasteTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() && m_modelIdxSelectedForPaste.isValid() )
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs  = "ModelIdxSrc {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedForPaste) + "}";
            strMthInArgs += ", ModelIdxTrg {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }

        CModelIdxTreeEntry* pModelTreeEntrySrc = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedForPaste.internalPointer());
        CModelIdxTreeEntry* pModelTreeEntryTrg = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());
        CModelIdxTreeEntry* pModelBranchTrg    = nullptr;

        if( !pModelTreeEntryTrg->isRoot() && !pModelTreeEntryTrg->isBranch() )
        {
            pModelBranchTrg = pModelTreeEntryTrg->parentBranch();
        }
        else
        {
            pModelBranchTrg = pModelTreeEntryTrg;
        }

        if( pModelBranchTrg != nullptr )
        {
            CIdxTreeLocker idxTreeLocker(m_pIdxTree);

            CIdxTreeEntry* pTreeEntrySrc = pModelTreeEntrySrc->getIdxTreeEntry();
            CIdxTreeEntry* pBranchTrg    = pModelBranchTrg->getIdxTreeEntry();

            if( m_pasteMode == EPasteMode::Copy )
            {
                m_pIdxTree->copy(pTreeEntrySrc, pBranchTrg);
            }
            else if( m_pasteMode == EPasteMode::Cut )
            {
                m_pIdxTree->move(pTreeEntrySrc, pBranchTrg);
                m_modelIdxSelectedForPaste = QModelIndex();
                m_pasteMode = EPasteMode::Undefined;
            }
        }
    }
} // onActionBranchPasteTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onActionLeaveDeleteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionLeaveDeleteTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        CModelIdxTreeEntry* pModelLeave = pModelTreeEntry;

        if( m_pIdxTree != nullptr && pModelLeave != nullptr )
        {
            CIdxTreeLocker idxTreeLocker(m_pIdxTree);

            CIdxTreeEntry* pLeave = pModelLeave->getIdxTreeEntry();

            delete pLeave;
            pLeave = nullptr;
        }
    }
} // onActionLeaveDeleteTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onActionLeaveCutTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionLeaveCutTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        m_modelIdxSelectedForPaste = m_modelIdxSelectedOnMousePressEvent;
        m_pasteMode = EPasteMode::Cut;

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedForPaste) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }
    }
} // onActionLeaveCutTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onActionLeaveCopyTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionLeaveCopyTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        m_modelIdxSelectedForPaste = m_modelIdxSelectedOnMousePressEvent;
        m_pasteMode = EPasteMode::Copy;

        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedForPaste) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }
    }
} // onActionLeaveCopyTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onActionLeavePasteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
    {
        strMthInArgs = bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionLeavePasteTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
        {
            strMthInArgs  = "ModelIdxSrc {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedForPaste) + "}";
            strMthInArgs += ", ModelIdxTrg {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs, ELogDetailLevel::Debug);
        }

        CModelIdxTreeEntry* pModelTreeEntrySrc = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedForPaste.internalPointer());
        CModelIdxTreeEntry* pModelTreeEntryTrg = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());
        CModelIdxTreeEntry* pModelBranchTrg    = nullptr;

        if( !pModelTreeEntryTrg->isRoot() && !pModelTreeEntryTrg->isBranch() )
        {
            pModelBranchTrg = pModelTreeEntryTrg->parentBranch();
        }
        else
        {
            pModelBranchTrg = pModelTreeEntryTrg;
        }

        if( pModelBranchTrg != nullptr )
        {
            CIdxTreeLocker idxTreeLocker(m_pIdxTree);

            CIdxTreeEntry* pTreeEntrySrc = pModelTreeEntrySrc->getIdxTreeEntry();
            CIdxTreeEntry* pBranchTrg    = pModelBranchTrg->getIdxTreeEntry();

            if( m_pasteMode == EPasteMode::Copy )
            {
                m_pIdxTree->copy(pTreeEntrySrc, pBranchTrg);
            }
            else if( m_pasteMode == EPasteMode::Cut )
            {
                m_pIdxTree->move(pTreeEntrySrc, pBranchTrg);
                m_modelIdxSelectedForPaste = QModelIndex();
                m_pasteMode = EPasteMode::Undefined;
            }
        }
    }
} // onActionLeavePasteTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTree::onIdxTreeAboutToBeDestroyed()
//------------------------------------------------------------------------------
{
    #ifdef ZS_TRACE_GUI_MODELS
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onIdxTreeAboutToBeDestroyed",
        /* strMethodInArgs    */ "" );
    #endif

    m_pIdxTree = nullptr;
}
