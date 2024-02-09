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

#include "ZSDraw/Widgets/TreeView/ZSDrawGraphObjsTreeView.h"
#include "ZSDraw/Widgets/TreeView/ZSDrawGraphObjsTreeModel.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Widgets/GraphObjFormat/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSysGUI/ZSSysErrDlg.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Draw;


/*******************************************************************************
class CDelegateGraphObjsTreeView : public QStyledItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDelegateGraphObjsTreeView::CDelegateGraphObjsTreeView( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QStyledItemDelegate(i_pObjParent),
    m_pEdtName(nullptr),
    m_bEdtNameDestroyedSignalConnected(false),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( QString(i_pObjParent == nullptr ? "Drawing" : i_pObjParent->objectName()) );

    QString strMthInArgs;

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::TreeView", ClassName(), objectName());

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Parent: " + QString(i_pObjParent == nullptr ? "nullptr" : i_pObjParent->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CDelegateGraphObjsTreeView::~CDelegateGraphObjsTreeView()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pEdtName = nullptr;
    m_bEdtNameDestroyedSignalConnected = false;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QLineEdit* CDelegateGraphObjsTreeView::nameColumnEditor()
//------------------------------------------------------------------------------
{
    return m_pEdtName;
}

/*==============================================================================
public: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
QWidget* CDelegateGraphObjsTreeView::createEditor(
    QWidget*                    i_pWdgtParent,
    const QStyleOptionViewItem& i_option,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createEditor",
        /* strMthInArgs */ strMthInArgs );

    QWidget* pWdgtEditor = nullptr;

    CDelegateGraphObjsTreeView* pVThis = const_cast<CDelegateGraphObjsTreeView*>(this);

    if( i_modelIdx.column() == CModelIdxTree::EColumnTreeEntryName )
    {
        pWdgtEditor = QStyledItemDelegate::createEditor(i_pWdgtParent, i_option, i_modelIdx);
        //pWdgtEditor = new CLineEditIdxTreeClmName(m_pTrcMthFile, m_mthTrcDetailLevel, i_pWdgtParent);

        pVThis->m_pEdtName = dynamic_cast<QLineEdit*>(pWdgtEditor);

        if( !m_bEdtNameDestroyedSignalConnected && pVThis->m_pEdtName != nullptr )
        {
            //pVThis->m_pEdtName->installEventFilter(pVThis);

            QObject::connect(
                m_pEdtName, &QLineEdit::destroyed,
                this, &CDelegateGraphObjsTreeView::onEdtNameDestroyed );
            pVThis->m_bEdtNameDestroyedSignalConnected = true;
        }
    } // if( i_modelIdx.column() == CModelIdxTree::EColumnTreeEntryName )

    else
    {
        pWdgtEditor = QStyledItemDelegate::createEditor(i_pWdgtParent, i_option, i_modelIdx);
    }

    return pWdgtEditor;

} // createEditor

//------------------------------------------------------------------------------
void CDelegateGraphObjsTreeView::setEditorData(
    QWidget*            i_pWdgtEditor,
    const QModelIndex&  i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setEditorData",
        /* strMthInArgs */ strMthInArgs );

    return QStyledItemDelegate::setEditorData(i_pWdgtEditor, i_modelIdx);

} // setEditorData

//------------------------------------------------------------------------------
void CDelegateGraphObjsTreeView::setModelData(
    QWidget*            i_pWdgtEditor,
    QAbstractItemModel* i_pModel,
    const QModelIndex&  i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Model: " + QString(i_pModel == nullptr ? "nullptr" : i_pModel->objectName());
        strMthInArgs += ", ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setModelData",
        /* strMthInArgs */ strMthInArgs );

    if( i_pModel == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pModel == nullptr");
    }

    if( i_modelIdx.column() == CModelIdxTree::EColumnTreeEntryName )
    {
        CDelegateGraphObjsTreeView* pVThis = const_cast<CDelegateGraphObjsTreeView*>(this);

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
void CDelegateGraphObjsTreeView::updateEditorGeometry(
    QWidget*                    i_pWdgtEditor,
    const QStyleOptionViewItem& i_option,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateEditorGeometry",
        /* strMthInArgs */ strMthInArgs );

    return QStyledItemDelegate::updateEditorGeometry(i_pWdgtEditor, i_option, i_modelIdx);

} // updateEditorGeometry

/*==============================================================================
protected: // slots
==============================================================================*/

//------------------------------------------------------------------------------
void CDelegateGraphObjsTreeView::onEdtNameDestroyed( QObject* /*i_pWdgtEditor*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onEdtNameDestroyed",
        /* strMthInArgs */ strMthInArgs );

    m_bEdtNameDestroyedSignalConnected = false;
    m_pEdtName = nullptr;

} // onEdtNameDestroyed


/*******************************************************************************
class CTreeViewGraphObjs : public QTreeView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewGraphObjs::CTreeViewGraphObjs(
    CDrawingScene* i_pDrawingScene,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pDelegate(nullptr),
    m_pDrawingScene(i_pDrawingScene),
    m_pModelIdxTreeGraphObjs(nullptr),
    m_pMenuGraphObjContext(nullptr),
    m_pActionGraphObjTitle(nullptr),
    m_pActionGraphObjExpand(nullptr),
    m_pActionGraphObjCollapse(nullptr),
    m_pActionGraphObjDelete(nullptr),
    m_modelIdxSelectedOnMousePressEvent(),
    m_modelIdxSelectedOnMouseReleaseEvent(),
    m_bSilentlyExecuteDeleteRequests(false),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjEvent(nullptr)
{
    setObjectName( QString(i_pDrawingScene == nullptr ? "Drawing" : i_pDrawingScene->objectName()) );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::TreeView", ClassName(), objectName());
    m_pTrcAdminObjEvent = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Widgets::TreeView", ClassName() + "::Event", objectName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString(i_pDrawingScene == nullptr ? "nullptr" : i_pDrawingScene->objectName());
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    m_pModelIdxTreeGraphObjs = new CModelIdxTreeGraphObjs(m_pDrawingScene);
    setModel(m_pModelIdxTreeGraphObjs);

    //m_pDelegate = new CDelegateGraphObjsTreeView(this);

    //setItemDelegate(m_pDelegate);

    for( int idxClm = 0; idxClm < CModelIdxTreeGraphObjs::EColumnCount; ++idxClm ) {
        if (idxClm != CModelIdxTreeGraphObjs::EColumnTreeEntryName ) {
            hideColumn(idxClm);
        }
    }

    // Context Popup Menu of Branches
    //-------------------------------

    m_pMenuGraphObjContext = new QMenu("Object", this);

    m_pActionGraphObjTitle = new QAction("Object:", this);

    QFont fntActionTitle = m_pActionGraphObjTitle->font();
    fntActionTitle.setBold(true);
    m_pActionGraphObjTitle->setFont(fntActionTitle);
    m_pMenuGraphObjContext->addAction(m_pActionGraphObjTitle);

    m_pMenuGraphObjContext->addSeparator();

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.png");
    //pxmExpandAll = pxmExpandAll.scaled(16,16);
    m_pActionGraphObjExpand = new QAction(pxmExpandAll, "Expand", this);
    m_pMenuGraphObjContext->addAction(m_pActionGraphObjExpand);

    QObject::connect(
        m_pActionGraphObjExpand, &QAction::triggered,
        this, &CTreeViewGraphObjs::onActionGraphObjExpandTriggered );

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.png");
    m_pActionGraphObjCollapse = new QAction(pxmCollapseAll, "Collapse", this);
    m_pMenuGraphObjContext->addAction(m_pActionGraphObjCollapse);

    QObject::connect(
        m_pActionGraphObjCollapse, &QAction::triggered,
        this, &CTreeViewGraphObjs::onActionGraphObjCollapseTriggered );

    m_pMenuGraphObjContext->addSeparator();

    m_pActionGraphObjDelete = new QAction("Delete", this);
    m_pMenuGraphObjContext->addAction(m_pActionGraphObjDelete);

    QObject::connect(
        m_pActionGraphObjDelete, &QAction::triggered,
        this, &CTreeViewGraphObjs::onActionGraphObjDeleteTriggered );

    m_pMenuGraphObjContext->addSeparator();

    // Connect to signals of the tree view
    //------------------------------------

    QObject::connect(
        this, &CTreeViewGraphObjs::collapsed,
        this, &CTreeViewGraphObjs::onCollapsed );
    QObject::connect(
        this, &CTreeViewGraphObjs::expanded,
        this, &CTreeViewGraphObjs::onExpanded );
    //QObject::connect(
    //    selectionModel(), currentChanged,
    //    this, &CTreeViewGraphObjs::onCurrentChanged );

    // Connect to signals of the drawing scene
    //----------------------------------------

    QObject::connect(
        m_pDrawingScene, &CDrawingScene::selectionChanged,
        this, &CTreeViewGraphObjs::onDrawingSceneSelectionChanged );

} // ctor

//------------------------------------------------------------------------------
CTreeViewGraphObjs::~CTreeViewGraphObjs()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strMthInArgs */ strMthInArgs );

    mthTracer.onAdminObjAboutToBeReleased();

    try {
        delete m_pModelIdxTreeGraphObjs;
    }
    catch(...) {
    }
    m_pModelIdxTreeGraphObjs = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjEvent);

    m_pDrawingScene = nullptr;
    m_pDelegate = nullptr;
    m_pMenuGraphObjContext = nullptr;
    m_pActionGraphObjTitle = nullptr;
    m_pActionGraphObjExpand = nullptr;
    m_pActionGraphObjCollapse = nullptr;
    m_pActionGraphObjDelete = nullptr;
    //m_modelIdxSelectedOnMousePressEvent;
    //m_modelIdxSelectedOnMouseReleaseEvent;
    m_bSilentlyExecuteDeleteRequests = false;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjEvent = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::setSortOrder( EIdxTreeSortOrder i_sortOrder )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "SortOrder: " + idxTreeSortOrder2Str(i_sortOrder);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSortOrder",
        /* strMthInArgs */ strMthInArgs );

    if (static_cast<int>(i_sortOrder) < 0 || i_sortOrder >= EIdxTreeSortOrder::Count) {
        QString strAddErrInfo = "SortOrder: " + idxTreeSortOrder2Str(i_sortOrder);
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, strAddErrInfo);
    }

    CModelIdxTreeGraphObjs* pModel = dynamic_cast<CModelIdxTreeGraphObjs*>(model());

    // On changing the sort order the model will be changed.
    // The childs of the branches will be removed and inserted again
    // wherupon the tree view nodes will be collapsed.
    // After sorting the model formerly expanded tree nodes may be collapsed.
    // This is not what I want. To restore the tree view concerning the
    // expanded states of the nodes the exanded state of the model nodes
    // is saved. After the model has been sorted the expanded states are recalled.

    QMap<QString, CModelIdxTreeEntry*> mappTreeEntries = pModel->treeEntriesMap();
    QStringList strlstKeysOfExpandedModelEntries;
    CModelIdxTreeEntry* pModelBranch;

    for (auto pModelTreeEntry : mappTreeEntries) {
        if (pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch()) {
            pModelBranch = pModelTreeEntry;
            if (pModelBranch->isExpanded() && pModel->areAllParentBranchesExpanded(pModelBranch)) {
                strlstKeysOfExpandedModelEntries << pModelTreeEntry->keyInTree();
            }
        }
    }
    pModel->setSortOrder(i_sortOrder);

    QModelIndex modelIdx;
    for (auto strKey : strlstKeysOfExpandedModelEntries) {
        modelIdx = pModel->index(strKey, 0);
        expand(modelIdx);
    }
}

//------------------------------------------------------------------------------
EIdxTreeSortOrder CTreeViewGraphObjs::sortOrder() const
//------------------------------------------------------------------------------
{
    CModelIdxTree* pModel = dynamic_cast<CModelIdxTree*>(model());
    return pModel->sortOrder();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::setSilentlyExecuteDeleteRequests( bool i_bExecuteSilently )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ExecuteSilently: " + bool2Str(i_bExecuteSilently);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSilentlyExecuteDeleteRequests",
        /* strMthInArgs */ strMthInArgs );
    m_bSilentlyExecuteDeleteRequests = i_bExecuteSilently;
}

/*==============================================================================
public: // overridable slots of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::expandAll()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "expandAll",
        /* strMthInArgs */ "" );
    QTreeView::expandAll();
    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());
    //expand(modelIdxRoot);
    expandRecursive(modelIdxRoot);
}

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::collapseAll()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "collapseAll",
        /* strMthInArgs */ "" );
    QTreeView::collapseAll();
    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());
    collapseRecursive(modelIdxRoot);
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::expandRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "expandRecursive",
        /* strMthInArgs */ strMthInArgs );

    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    if (pModelTreeEntry != nullptr) {
        if (pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch()) {
            expand(i_modelIdx);
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            CModelIdxTreeEntry* pModelTreeEntryChild;
            QModelIndex modelIdxChild;
            for (int idxEntry = 0; idxEntry < pModelBranch->count(); ++idxEntry) {
                pModelTreeEntryChild = pModelBranch->at(idxEntry);
                if (pModelTreeEntryChild->isRoot() || pModelTreeEntryChild->isBranch()) {
                    modelIdxChild = model()->index(idxEntry, 0, i_modelIdx);
                    expandRecursive(modelIdxChild);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::collapseRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "collapseRecursive",
        /* strMthInArgs */ strMthInArgs );

    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    if (pModelTreeEntry != nullptr) {
        if (pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch()) {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            QModelIndex modelIdx;
            for (int idxEntry = 0; idxEntry < pModelBranch->count(); ++idxEntry) {
                pModelTreeEntry = pModelBranch->at(idxEntry);
                if (pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch()) {
                    modelIdx = model()->index(idxEntry, 0, i_modelIdx);
                    collapseRecursive(modelIdx);
                }
            }
            collapse(i_modelIdx);
        }
    }
}

/*==============================================================================
public: // slots (hiding not overridable slots with same name in QTreeView)
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::expand( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "expand",
        /* strMthInArgs */ strMthInArgs );

    // If calling "expandAll" the signal "expanded" is not invoked for all
    // nodes of the tree view. This is a workaround to store the "expanded"
    // state of the model tree node in order to restore this state on changing
    // the sort order of the model.

    QTreeView::expand(i_modelIdx);
    if (i_modelIdx.isValid()) {
        onExpanded(i_modelIdx);
    }
}

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::collapse( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "collapse",
        /* strMthInArgs */ strMthInArgs );

    // If calling "collapseAll" the signal "collapsed" is not invoked for all
    // nodes of the tree view. This is a workaround to store the "expanded"
    // state of the model tree node in order to restore this state on changing
    // the sort order of the model.

    QTreeView::collapse(i_modelIdx);
    if (i_modelIdx.isValid()) {
        onCollapsed(i_modelIdx);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::onCollapsed( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCollapsed",
        /* strMthInArgs */ strMthInArgs );

    if (i_modelIdx.isValid()) {
        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        if (pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch()) {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            pModelIdxTree->setIsExpanded(pModelBranch, false);
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::onExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onExpanded",
        /* strMthInArgs */ strMthInArgs );

    if (i_modelIdx.isValid()) {
        #if QT_VERSION >= 0x040000 && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        //resizeColumnToContents(i_modelIdx.column());
        #elif QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        // Cannot invoke resizeColumnToContents as this leads to an exception.
        #endif

        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
        if (pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch()) {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            pModelIdxTree->setIsExpanded(pModelBranch, true);
        }
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::onDrawingSceneSelectionChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawingSceneSelectionChanged",
        /* strAddInfo   */ "" );

    //QObject::disconnect(
    //    selectionModel(), currentChanged,
    //    this, &CTreeViewGraphObjs::onCurrentChanged );

    //clearSelection();

    QList<QGraphicsItem*> arpSelectedItems = m_pDrawingScene->selectedItems();
    if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
        QString strRuntimeInfo = "SelectedItems [" + QString::number(arpSelectedItems.size()) + "]";
        if (arpSelectedItems.size() > 0) {
            strRuntimeInfo += "(";
            for (auto& pGraphicsItem : arpSelectedItems) {
                CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
                if( !strRuntimeInfo.endsWith("(") ) strRuntimeInfo += ", ";
                strRuntimeInfo += pGraphObj->path();
            }
            strRuntimeInfo += ")";
        }
        mthTracer.trace(strRuntimeInfo);
    }

    //if( arpSelectedItems.size() == 1 )
    //{
    //    QGraphicsItem* pGraphicsItem = arpSelectedItems[0];
    //    CGraphObj*     pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

    //    if( pGraphObj != nullptr )
    //    {
    //        QModelIndex modelIdx = dynamic_cast<CModelIdxTreeGraphObjs*>(model())->index(pGraphObj->keyInTree(), 0);

    //        if( modelIdx.isValid() )
    //        {
    //            setCurrentIndex(modelIdx);
    //            scrollTo(modelIdx);
    //        }
    //    }
    //}

    //QObject::connect(
    //    selectionModel(), currentChanged,
    //    this, &CTreeViewGraphObjs::onCurrentChanged );

} // onDrawingSceneSelectionChanged

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CTreeViewGraphObjs::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjEvent, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Event {" + qEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvent,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "event",
        /* strMthInArgs */ strMthInArgs );
    return QTreeView::event(i_pEv);
}

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Event {" + qKeyEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "keyPressEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;

    QItemSelectionModel* pSelModel = selectionModel();
    QModelIndex modelIdxSelected = pSelModel->currentIndex();
    if (modelIdxSelected.isValid()) {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdxSelected.internalPointer());
        if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::modelIdx2Str(modelIdxSelected) + "}";
            mthTracer.trace(strMthInArgs);
        }

        if (pModelTreeEntry != nullptr) {
            switch (i_pEv->key()) {
                case Qt::Key_F2: { // Rename
                    // Handled by delegate.
                    break;
                }
                case Qt::Key_Escape: { // Undo selection
                    // Not handled by TreeView but BranchContentView.
                    break;
                }
                case Qt::Key_Delete: {
                    CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pModelTreeEntry->getIdxTreeEntry());
                    if (pGraphObj != nullptr && !pGraphObj->isRoot()) {
                        // Selection points and labels cannot be deleted by the user
                        // but will be implicitly deleted if the item is deselected.
                        if (!pGraphObj->isSelectionPoint() && !pGraphObj->isLabel()) {
                            int iRet = QMessageBox::Yes;
                            if (!m_bSilentlyExecuteDeleteRequests) {
                                QString strMsg = "Do you really want to delete \"" + pModelTreeEntry->keyInTree() + "\"?";
                                iRet = QMessageBox::question(
                                    /* pWdgtParent     */ this,
                                    /* strTitle        */ ZS::System::GUI::getMainWindowTitle(),
                                    /* strText         */ strMsg,
                                    /* standardButtons */ QMessageBox::Yes | QMessageBox::No,
                                    /* defaultButton   */ QMessageBox::No );
                            }
                            if (iRet == QMessageBox::Yes) {
                                delete pGraphObj;
                                pGraphObj = nullptr;
                                pModelTreeEntry = nullptr;
                            }
                        }
                    }
                    bEventHandled = true;
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    if (!bEventHandled) {
        if (i_pEv->key() != Qt::Key_F2) {
            QTreeView::keyPressEvent(i_pEv);
        }
        else {
            QTreeView::keyPressEvent(i_pEv);
        }
    }
} // keyPressEvent

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mousePressEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMousePressEvent = indexAt(i_pEv->pos());

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            QString strMthInArgs = "ModelIdxPressed {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs);
        }
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());
        if (pModelTreeEntry != nullptr) {
            CGraphObj* pGraphObjClicked = dynamic_cast<CGraphObj*>(pModelTreeEntry->getIdxTreeEntry());
            if (pGraphObjClicked != nullptr && !pGraphObjClicked->isRoot()) {
                QGraphicsItem* pGraphicsItemClicked = dynamic_cast<QGraphicsItem*>(pGraphObjClicked);
                CGraphObj*     pGraphObj = pGraphObjClicked;
                QGraphicsItem* pGraphicsItem = pGraphicsItemClicked;

                // Ensure that only the clicked graphic item is selected taking into accout that selection points
                // and labels cannot be selected but instead select their parents (if not already selected).
                if (pGraphObjClicked->isSelectionPoint() || pGraphObjClicked->isLabel()) {
                    pGraphObj = pGraphObjClicked->parentGraphObj();
                    // Selection points and labels don't have graphics item as a parent.
                    // Instead we must use the parent graph object in the index tree.
                    pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);
                }
                if (pGraphicsItem != nullptr) {
                    if (i_pEv->buttons() & Qt::LeftButton) {
                    }
                    else if (i_pEv->buttons() & Qt::RightButton) {
                        if (m_pMenuGraphObjContext != nullptr) {
                            m_pActionGraphObjTitle->setText( "Object: " + pGraphObj->name() );
                            m_pMenuGraphObjContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                        }
                        bEventHandled = true;
                    }
                }
            }
        }
    }
    if (!bEventHandled) {
        QTreeView::mousePressEvent(i_pEv);
    }
} // mousePressEvent

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseReleaseEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMouseReleaseEvent = indexAt(i_pEv->pos());

    if (m_modelIdxSelectedOnMouseReleaseEvent.isValid()) {
        if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            QString strMthInArgs = "ModelIdxReleased {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMouseReleaseEvent) + "}";
            mthTracer.trace(strMthInArgs);
        }
    }
    if (!bEventHandled) {
        QTreeView::mouseReleaseEvent(i_pEv);
    }
} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::mouseDoubleClickEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;

    QModelIndex modelIdx = indexAt(i_pEv->pos());

    if (modelIdx.isValid()) {
        if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            QString strMthInArgs = "ModelIdxClicked {" + CModelIdxTree::modelIdx2Str(modelIdx) + "}";
            mthTracer.trace(strMthInArgs);
        }
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdx.internalPointer());
        if (pModelTreeEntry != nullptr) {
            CGraphObj* pGraphObjClicked = dynamic_cast<CGraphObj*>(pModelTreeEntry->getIdxTreeEntry());
            if (pGraphObjClicked != nullptr && !pGraphObjClicked->isRoot()) {
                QGraphicsItem* pGraphicsItemClicked = dynamic_cast<QGraphicsItem*>(pGraphObjClicked);
                if (pGraphicsItemClicked != nullptr) {
                    if (i_pEv->buttons() & Qt::LeftButton) {
                    }
                    else if (i_pEv->buttons() & Qt::RightButton) {
                        if (m_pMenuGraphObjContext != nullptr) {
                            m_pActionGraphObjTitle->setText( "Object: " + pModelTreeEntry->name() );
                            m_pMenuGraphObjContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                        }
                        bEventHandled = true;
                    }
                }
            }
        }
    }
    if (!bEventHandled) {
        QTreeView::mouseDoubleClickEvent(i_pEv);
    }
}

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::mouseMoveEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv) + "}";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseMoveEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;
    if (!bEventHandled) {
        QTreeView::mouseMoveEvent(i_pEv);
    }
}

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::selectionChanged(
    const QItemSelection& i_selected,
    const QItemSelection& i_deselected )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Selected [" + QString::number(i_selected.indexes().size()) + "]";
        if (i_selected.indexes().size() > 0) {
            strMthInArgs += "(";
            for (const auto& modelIdx : i_selected.indexes()) {
                if (!strMthInArgs.endsWith("(")) strMthInArgs += ", ";
                strMthInArgs += "{" + CModelIdxTree::modelIdx2Str(modelIdx) + "}";
            }
            strMthInArgs += ")";
        }
        strMthInArgs += ", Deselected [" + QString::number(i_deselected.indexes().size()) + "]";
        if (i_deselected.indexes().size() > 0) {
            strMthInArgs += "(";
            for (const auto& modelIdx : i_deselected.indexes()) {
                if (!strMthInArgs.endsWith("(")) strMthInArgs += ", ";
                strMthInArgs += "{" + CModelIdxTree::modelIdx2Str(modelIdx) + "}";
            }
            strMthInArgs += ")";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "selectionChanged",
        /* strMthInArgs */ strMthInArgs );

    // The selection contains each cell (each column).
    // We are only interested in the rows and duplicate entries will be removed.

    QHash<QString, CGraphObj*> hshpGraphObjsSelected;
    for (const auto& modelIdx : i_selected.indexes()) {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdx.internalPointer());
        if (pModelTreeEntry != nullptr && !pModelTreeEntry->getIdxTreeEntry()->isAboutToBeDestroyed()) {
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pModelTreeEntry->getIdxTreeEntry());
            if (pGraphObj != nullptr && !pGraphObj->isRoot()) {
                // Selection points and labels cannot be selected but instead select their parents (if not already selected).
                if (pGraphObj->isSelectionPoint() || pGraphObj->isLabel()) {
                    // Selection points and labels don't have graphics item as a parent.
                    // Instead we must use the parent graph object in the index tree.
                    pGraphObj = pGraphObj->parentGraphObj();
                }
                if (pGraphObj != nullptr) {
                    if (!hshpGraphObjsSelected.contains(pGraphObj->keyInTree())) {
                        hshpGraphObjsSelected.insert(pGraphObj->keyInTree(), pGraphObj);
                    }
                }
            }
        }
    }

    QHash<QString, CGraphObj*> hshpGraphObjsDeselected;
    for (const auto& modelIdx : i_deselected.indexes()) {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdx.internalPointer());
        if (pModelTreeEntry != nullptr && !pModelTreeEntry->getIdxTreeEntry()->isAboutToBeDestroyed()) {
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pModelTreeEntry->getIdxTreeEntry());
            if (pGraphObj != nullptr && !pGraphObj->isRoot()) {
                // Selection points and labels can neither be selected nor deselected.
                if (pGraphObj->isSelectionPoint() || pGraphObj->isLabel()) {
                    // Selection points and labels don't have graphics item as a parent.
                    // Instead we must use the parent graph object in the index tree.
                    pGraphObj = pGraphObj->parentGraphObj();
                }
                if (pGraphObj != nullptr) {
                    // If the tree view entry for a selection point or label is deselected it might be that
                    // the parent is still selected or has been selected by selecting one of its child
                    // selection points or labels. So we add the parent object only to the hash of items
                    // to be deselected if the item has not inserted into the hash of selected items.
                    if (!hshpGraphObjsSelected.contains(pGraphObj->keyInTree())
                     && !hshpGraphObjsDeselected.contains(pGraphObj->keyInTree())) {
                        hshpGraphObjsDeselected.insert(pGraphObj->keyInTree(), pGraphObj);
                    }
                }
            }
        }
    }
    for (CGraphObj* pGraphObj : hshpGraphObjsDeselected) {
        pGraphObj->setIsHighlighted(false);
    }
    for (CGraphObj* pGraphObj : hshpGraphObjsSelected) {
        pGraphObj->setIsHighlighted(true);
    }
} // selectionChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::onActionGraphObjExpandTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionGraphObjExpandTriggered",
        /* strMthInArgs */ strMthInArgs );

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        expandRecursive(m_modelIdxSelectedOnMousePressEvent);
        for (int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++) {
            resizeColumnToContents(idxClm);
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::onActionGraphObjCollapseTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionGraphObjCollapseTriggered",
        /* strMthInArgs */ strMthInArgs );

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        collapseRecursive(m_modelIdxSelectedOnMousePressEvent);
    }
}

//------------------------------------------------------------------------------
void CTreeViewGraphObjs::onActionGraphObjDeleteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionGraphObjDeleteTriggered",
        /* strMthInArgs */ strMthInArgs );

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        if (mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug)) {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs);
        }

        CModelIdxTreeEntry* pModelTreeEntry =
            static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());
        if (pModelTreeEntry != nullptr) {
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pModelTreeEntry->getIdxTreeEntry());
            if (pGraphObj != nullptr && !pGraphObj->isRoot()) {
                // Selection points and labels cannot be deleted by the user
                // but will be implicitly deleted if the item is deselected.
                if (!pGraphObj->isSelectionPoint() && !pGraphObj->isLabel()) {
                    int iRet = QMessageBox::Yes;
                    if (!m_bSilentlyExecuteDeleteRequests) {
                        QString strMsg = "Do you really want to delete \"" + pModelTreeEntry->keyInTree() + "\"?";
                        iRet = QMessageBox::question(
                            /* pWdgtParent     */ this,
                            /* strTitle        */ ZS::System::GUI::getMainWindowTitle(),
                            /* strText         */ strMsg,
                            /* standardButtons */ QMessageBox::Yes | QMessageBox::No,
                            /* defaultButton   */ QMessageBox::No );
                    }
                    if (iRet == QMessageBox::Yes) {
                        delete pGraphObj;
                        pGraphObj = nullptr;
                        pModelTreeEntry = nullptr;
                    }
                }
            }
        }
    }
} // onActionGraphObjDeleteTriggered
