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

#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeView.h"
#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeModel.h"
#include "ZSSysGUI/ZSSysTrcAdminObjEditFilterExpressionsDlg.h"
#include "ZSSysGUI/ZSSysCheckBoxItemDelegate.h"
#include "ZSSysGUI/ZSSysComboBoxItemDelegate.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSysGUI/ZSSysEditEnumValueDlg.h"
#include "ZSSysGUI/ZSSysEditStringValueDlg.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include <QtWidgets/qapplication.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qmenu.h>
#include <QtGui/qinputdialog.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qinputdialog.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CTreeViewIdxTreeTrcAdminObjs : public QTreeView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewIdxTreeTrcAdminObjs::CTreeViewIdxTreeTrcAdminObjs(
    CIdxTreeTrcAdminObjs* i_pIdxTree,
    QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pIdxTree(i_pIdxTree),
    m_pModel(nullptr),
    m_pMenuNameSpaceContext(nullptr),
    m_pActionNameSpaceTitle(nullptr),
    m_pActionNameSpaceExpand(nullptr),
    m_pActionNameSpaceCollapse(nullptr),
    m_pActionNameSpaceEnableAdminObjs(nullptr),
    m_pActionNameSpaceDisableAdminObjs(nullptr),
    m_pActionNameSpaceSetAdminObjsMethodCallsDetailLevel(nullptr),
    m_pActionNameSpaceSetAdminObjsRuntimeInfoDetailLevel(nullptr),
    m_pActionNameSpaceSetAdminObjsObjNameFilter(nullptr),
    m_pActionNameSpaceSetAdminObjsMethodNameFilter(nullptr),
    m_pActionNameSpaceSetAdminObjsTraceDataFilter(nullptr),
    m_modelIdxSelectedOnMousePressEvent(),
    m_modelIdxSelectedOnMouseReleaseEvent(),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( QString(i_pIdxTree == nullptr ? "IdxTree" : i_pIdxTree->objectName()) );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "IdxTree: " + QString(i_pIdxTree == nullptr ? "nullptr" : i_pIdxTree->objectName());
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    m_pModel = new CModelIdxTreeTrcAdminObjs(m_pIdxTree, nullptr);

    setModel(m_pModel);

    setItemDelegateForColumn(
        CModelIdxTreeTrcAdminObjs::EColumnEnabled, new CCheckBoxItemDelegate(m_pModel, this));
    setItemDelegateForColumn(
        CModelIdxTreeTrcAdminObjs::EColumnMethodCallsDetailLevel, new CComboBoxItemDelegate(m_pModel, this));
    setItemDelegateForColumn(
        CModelIdxTreeTrcAdminObjs::EColumnRuntimeInfoDetailLevel, new CComboBoxItemDelegate(m_pModel, this));

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAlternatingRowColors(true);
    setAllColumnsShowFocus(true);
    setEditTriggers(
        QAbstractItemView::CurrentChanged|QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked|
        QAbstractItemView::EditKeyPressed|QAbstractItemView::AnyKeyPressed);

    //hideColumn(CModelIdxTree::EColumnTreeEntryName);
    hideColumn(CModelIdxTree::EColumnTreeEntryNameDecorated);
    hideColumn(CModelIdxTree::EColumnTreeEntryTypeImageUrl);
    hideColumn(CModelIdxTree::EColumnTreeEntryTypeIcon);
    hideColumn(CModelIdxTree::EColumnTreeEntryType);
    hideColumn(CModelIdxTree::EColumnInternalId);
    hideColumn(CModelIdxTree::EColumnIdxInTree);
    hideColumn(CModelIdxTree::EColumnIdxInParentBranch);
    hideColumn(CModelIdxTree::EColumnKeyInTree);
    hideColumn(CModelIdxTree::EColumnKeyInParentBranch);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnRefCount);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnEnabled);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnMethodCallsDetailLevel);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnRuntimeInfoDetailLevel);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnObjNameFilter);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnMethodNameFilter);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnDataFilter);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnNameSpace);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnClassName);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnObjName);
    //hideColumn(CModelIdxTreeTrcAdminObjs::EColumnObjThreadName);
    hideColumn(CModelIdxTreeTrcAdminObjs::EColumnObjAddress);

    // Context Popup Menu of Name Space Branches
    //------------------------------------------

    m_pMenuNameSpaceContext = new QMenu("Name Space", this);

    m_pActionNameSpaceTitle = new QAction("Name Space:", this);

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.png");
    m_pActionNameSpaceExpand = new QAction(pxmExpandAll, "Expand", this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceExpand);
    QObject::connect(
        m_pActionNameSpaceExpand, &QAction::triggered,
        this, &CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceExpandTriggered);

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.png");
    m_pActionNameSpaceCollapse = new QAction(pxmCollapseAll, "Collapse", this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceCollapse);
    QObject::connect(
        m_pActionNameSpaceCollapse, &QAction::triggered,
        this, &CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceCollapseTriggered);

    m_pMenuNameSpaceContext->addSeparator();

    QFont fntActionTitle = m_pActionNameSpaceTitle->font();
    fntActionTitle.setBold(true);
    m_pActionNameSpaceTitle->setFont(fntActionTitle);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceTitle);

    m_pActionNameSpaceEnableAdminObjs = new QAction("Recursively Enable Admin Objects", this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceEnableAdminObjs);
    QObject::connect(
        m_pActionNameSpaceEnableAdminObjs, &QAction::triggered,
        this, &CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceEnableAdminObjsTriggered);

    m_pActionNameSpaceDisableAdminObjs = new QAction("Recursively Disable Admin Objects",this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceDisableAdminObjs);
    QObject::connect(
        m_pActionNameSpaceDisableAdminObjs, &QAction::triggered,
        this, &CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceDisableAdminObjsTriggered);

    m_pMenuNameSpaceContext->addSeparator();

    m_pActionNameSpaceSetAdminObjsMethodCallsDetailLevel = new QAction("Recursively Set Method Calls Detail Level of Admin Objects", this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceSetAdminObjsMethodCallsDetailLevel);
    QObject::connect(
        m_pActionNameSpaceSetAdminObjsMethodCallsDetailLevel, &QAction::triggered,
        this, &CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsMethodCallsDetailLevelTriggered);

    m_pActionNameSpaceSetAdminObjsRuntimeInfoDetailLevel = new QAction("Recursively Set Runtime Info Detail Level of Admin Objects", this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceSetAdminObjsRuntimeInfoDetailLevel);
    QObject::connect(
        m_pActionNameSpaceSetAdminObjsRuntimeInfoDetailLevel, &QAction::triggered,
        this, &CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsRuntimeInfoDetailLevelTriggered);

    m_pActionNameSpaceSetAdminObjsObjNameFilter = new QAction("Recursively Set Object Name Filter of Admin Objects", this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceSetAdminObjsObjNameFilter);
    QObject::connect(
        m_pActionNameSpaceSetAdminObjsObjNameFilter, &QAction::triggered,
        this, &CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsObjectNameFilterTriggered);

    m_pActionNameSpaceSetAdminObjsMethodNameFilter = new QAction("Recursively Set Method Name Filter of Admin Objects", this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceSetAdminObjsMethodNameFilter);
    QObject::connect(
        m_pActionNameSpaceSetAdminObjsMethodNameFilter, &QAction::triggered,
        this, &CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsMethodNameFilterTriggered);

    m_pActionNameSpaceSetAdminObjsTraceDataFilter = new QAction("Recursively Set Data Filter of Admin Objects", this);
    m_pMenuNameSpaceContext->addAction(m_pActionNameSpaceSetAdminObjsTraceDataFilter);
    QObject::connect(
        m_pActionNameSpaceSetAdminObjsTraceDataFilter, &QAction::triggered,
        this, &CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsTraceDataFilterTriggered);

} // ctor

//------------------------------------------------------------------------------
CTreeViewIdxTreeTrcAdminObjs::~CTreeViewIdxTreeTrcAdminObjs()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ "" );

    try {
        delete m_pModel;
    }
    catch (...) {
    }

    if (m_pTrcAdminObj != nullptr) {
        mthTracer.onAdminObjAboutToBeReleased();
        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }

    m_pIdxTree = nullptr;
    m_pModel = nullptr;
    m_pMenuNameSpaceContext = nullptr;
    m_pActionNameSpaceTitle = nullptr;
    m_pActionNameSpaceExpand = nullptr;
    m_pActionNameSpaceCollapse = nullptr;
    m_pActionNameSpaceEnableAdminObjs = nullptr;
    m_pActionNameSpaceDisableAdminObjs = nullptr;
    m_pActionNameSpaceSetAdminObjsMethodCallsDetailLevel = nullptr;
    m_pActionNameSpaceSetAdminObjsRuntimeInfoDetailLevel = nullptr;
    m_pActionNameSpaceSetAdminObjsObjNameFilter = nullptr;
    m_pActionNameSpaceSetAdminObjsMethodNameFilter = nullptr;
    m_pActionNameSpaceSetAdminObjsTraceDataFilter = nullptr;
    //m_modelIdxSelectedOnMousePressEvent;
    //m_modelIdxSelectedOnMouseReleaseEvent;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // overridable slots of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::expandAll()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "expandAll",
        /* strMethodInArgs    */ "" );

    QTreeView::expandAll();
    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());
    expandRecursive(modelIdxRoot);
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::collapseAll()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "collapseAll",
        /* strMethodInArgs    */ "" );

    QTreeView::collapseAll();
    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());
    collapseRecursive(modelIdxRoot);
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::expandRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "expandRecursive",
        /* strMethodInArgs    */ strMthInArgs );

    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    if (pModelTreeEntry != nullptr) {
        if (pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch()) {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            for (int idxEntry = 0; idxEntry < pModelBranch->count(); ++idxEntry) {
                CModelIdxTreeEntry* pModelTreeEntryChild = pModelBranch->at(idxEntry);
                if (pModelTreeEntryChild->isRoot() || pModelTreeEntryChild->isBranch()) {
                    QModelIndex modelIdxChild = model()->index(idxEntry, 0, i_modelIdx);
                    expandRecursive(modelIdxChild);
                }
            }
            expand(i_modelIdx);
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::collapseRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "collapsRecursive",
        /* strMethodInArgs    */ strMthInArgs );

    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    if (pModelTreeEntry != nullptr) {
        if (pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch()) {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            for ( int idxEntry = 0; idxEntry < pModelBranch->count(); ++idxEntry) {
                pModelTreeEntry = pModelBranch->at(idxEntry);
                if (pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch()) {
                    QModelIndex modelIdx = model()->index(idxEntry, 0, i_modelIdx);
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
void CTreeViewIdxTreeTrcAdminObjs::expand( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
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
    if (i_modelIdx.isValid()) {
        onExpanded(i_modelIdx);
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::collapse( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
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
    if (i_modelIdx.isValid()) {
        onCollapsed(i_modelIdx);
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onCollapsed( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onCollapsed",
        /* strMethodInArgs    */ strMthInArgs );

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
void CTreeViewIdxTreeTrcAdminObjs::onExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "ModelIdx {" + CModelIdxTreeTrcAdminObjs::modelIdx2Str(i_modelIdx) + "}";
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onExpanded",
        /* strMethodInArgs    */ strMthInArgs );

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
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qKeyEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "keyPressEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    if( i_pEv->key() == Qt::Key_Return
     || i_pEv->key() == Qt::Key_Enter
     || i_pEv->key() == Qt::Key_Space
     || i_pEv->key() == Qt::Key_F5 ) {
        QModelIndex modelIdxSelected = selectionModel()->currentIndex();
        if (modelIdxSelected.isValid()) {
            QAbstractItemModel* pModelAbstract = const_cast<QAbstractItemModel*>(modelIdxSelected.model());
            if (pModelAbstract != nullptr) {
                if (i_pEv->key() == Qt::Key_F5) {
                    //pModelAbstract->beginReset();
                    //pModelAbstract->endReset();
                }
                else {
                    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());
                    if (pModelTreeEntry->isLeave()) {
                        if (modelIdxSelected.column() == CModelIdxTreeTrcAdminObjs::EColumnEnabled) {
                            QVariant val = pModelAbstract->data(modelIdxSelected, Qt::DisplayRole);
                            #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            if (val.canConvert(QVariant::Bool)) {
                            #else
                            // static_cast to avoid deprecation warning
                            if (val.canConvert(static_cast<QMetaType>(QMetaType::Bool))) {
                            #endif
                                pModelAbstract->setData(modelIdxSelected, !val.toBool(), Qt::EditRole);
                            }
                        }
                    }
                }
            }
        }
    }
    if (!bEventHandled) {
        QTreeView::keyPressEvent(i_pEv);
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "mousePressEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;
    QPoint ptPosThis = pos();
    QPoint ptEvLocalPos = i_pEv->localPos().toPoint();
    QPoint ptEvGlobalPos = i_pEv->globalPos();
    QPoint ptEvSceenPos = i_pEv->screenPos().toPoint();
    QPoint ptEvWindowPos = i_pEv->windowPos().toPoint();
    m_modelIdxSelectedOnMousePressEvent = indexAt(i_pEv->pos());
    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());
        if (pModelTreeEntry != nullptr) {
            if (i_pEv->buttons() & Qt::RightButton) {
                if (pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch()) {
                    if (m_modelIdxSelectedOnMousePressEvent.column() == CModelIdxTreeTrcAdminObjs::EColumnTreeEntryName) {
                        if (m_pMenuNameSpaceContext != nullptr) {
                            m_pActionNameSpaceTitle->setText( "Name Space: " + pModelTreeEntry->name() );
                            m_pMenuNameSpaceContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                        }
                        bEventHandled = true;
                    }
                }
            }
            else if (i_pEv->buttons() & Qt::LeftButton) {
                if (pModelTreeEntry->isLeave()) {
                    CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(pModelTreeEntry->getIdxTreeEntry());
                    if (pTrcAdminObj != nullptr) {
                        if (m_modelIdxSelectedOnMousePressEvent.column() == CModelIdxTreeTrcAdminObjs::EColumnObjNameFilter
                         || m_modelIdxSelectedOnMousePressEvent.column() == CModelIdxTreeTrcAdminObjs::EColumnMethodNameFilter
                         || m_modelIdxSelectedOnMousePressEvent.column() == CModelIdxTreeTrcAdminObjs::EColumnDataFilter) {
                            CDlgTrcAdminObjEditFilterExpressions* pDlg =
                                CDlgTrcAdminObjEditFilterExpressions::GetInstance("TrcAdminObjEditFilterDialog");
                            if (pDlg == nullptr) {
                                QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Edit Filter Expressions";
                                pDlg = CDlgTrcAdminObjEditFilterExpressions::CreateInstance(
                                    /* strDlgTitle */ strDlgTitle,
                                    /* strObjName  */ "TrcAdminObjEditFilterDialog",
                                    /* pIdxTree    */ m_pIdxTree,
                                    /* pWdgtParent */ this );
                                if (m_modelIdxSelectedOnMousePressEvent.column() == CModelIdxTreeTrcAdminObjs::EColumnObjNameFilter) {
                                    pDlg->setKeyEntryToEdit(pTrcAdminObj->keyInTree(), EMethodTraceFilterProperty::ObjectName);
                                }
                                else if (m_modelIdxSelectedOnMousePressEvent.column() == CModelIdxTreeTrcAdminObjs::EColumnMethodNameFilter) {
                                    pDlg->setKeyEntryToEdit(pTrcAdminObj->keyInTree(), EMethodTraceFilterProperty::MethodName);
                                }
                                else if (m_modelIdxSelectedOnMousePressEvent.column() == CModelIdxTreeTrcAdminObjs::EColumnDataFilter) {
                                    pDlg->setKeyEntryToEdit(pTrcAdminObj->keyInTree(), EMethodTraceFilterProperty::TraceData);
                                }
                                pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
                                pDlg->adjustSize();
                                pDlg->show();
                                pDlg->move(ptEvGlobalPos);
                            }
                            else {
                                if (pDlg->isHidden()) {
                                    pDlg->show();
                                }
                                pDlg->raise();
                                pDlg->activateWindow();
                            }
                        }
                    }
                }
            }
        }
    }
    if (!bEventHandled) {
        QTreeView::mousePressEvent(i_pEv);
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = qMouseEvent2Str(i_pEv);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "mouseReleaseEvent",
        /* strMethodInArgs    */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMouseReleaseEvent = indexAt(i_pEv->pos());
    if (m_modelIdxSelectedOnMouseReleaseEvent.isValid()) {
    }
    if (!bEventHandled) {
        QTreeView::mouseReleaseEvent(i_pEv);
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::paintEvent( QPaintEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QTreeView::paintEvent(i_pEv);
    QPainter painter(viewport());
    QColor lineColor(Qt::lightGray);
    lineColor.setAlpha(127);
    painter.setPen(lineColor);
    for (int idxClm = 0; idxClm < header()->count(); ++idxClm) {
        // draw only visible sections starting from second column
        if (header()->isSectionHidden(idxClm) || header()->visualIndex(idxClm) <= 0)
            continue;
        // position mapped to viewport
        int xPos = header()->sectionViewportPosition(idxClm) - 1;
        if (xPos > 0) {
            painter.drawLine(QPoint(xPos, 0), QPoint(xPos, height()));
        }
    }
}

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceExpandTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionNameSpaceExpandTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        expandRecursive(m_modelIdxSelectedOnMousePressEvent);
        for (int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++) {
            resizeColumnToContents(idxClm);
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceCollapseTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionNameSpaceCollapseTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        collapseRecursive(m_modelIdxSelectedOnMousePressEvent);
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceEnableAdminObjsTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionNameSpaceEnableAdminObjsTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());
        if (pModelIdxTree != nullptr) {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();
            CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());
            if (pIdxTree != nullptr && pModelTreeEntry != nullptr) {
                CIdxTreeLocker idxTreeLocker(pIdxTree);
                CIdxTreeEntry* pBranch = pModelTreeEntry->getIdxTreeEntry();
                if (pBranch != nullptr) {
                    pIdxTree->setEnabled(pBranch, EEnabled::Yes);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceDisableAdminObjsTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionNameSpaceDisableAdminObjsTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());
        if (pModelIdxTree != nullptr) {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();
            CModelIdxTreeEntry* pModelTreeEntry =
                static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());
            if (pIdxTree != nullptr && pModelTreeEntry != nullptr) {
                CIdxTreeLocker idxTreeLocker(pIdxTree);
                CIdxTreeEntry* pBranch = pModelTreeEntry->getIdxTreeEntry();
                if (pBranch != nullptr) {
                    pIdxTree->setEnabled(pBranch, EEnabled::No);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsMethodCallsDetailLevelTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionNameSpaceSetAdminObjsMethodCallsDetailLevelTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());
        if (pModelIdxTree != nullptr) {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();
            CModelIdxTreeEntry* pModelTreeEntry =
                static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());
            if (pIdxTree != nullptr && pModelTreeEntry != nullptr) {
                CIdxTreeLocker idxTreeLocker(pIdxTree);
                CIdxTreeEntry* pBranch = pModelTreeEntry->getIdxTreeEntry();
                if (pBranch != nullptr) {
                    CDlgEditEnumValue* pDlg = CDlgEditEnumValue::CreateInstance(
                        /* strTitle    */ QCoreApplication::applicationName(),
                        /* strObjName  */ "MethodCallsDetailLevel",
                        /* pWdgtParent */ this );
                    pDlg->setValueName("MethodCalls");
                    pDlg->setComboItems(CEnumMethodTraceDetailLevel::s_arEnumEntries);
                    pDlg->setEnumerator(0);
                    if (pDlg->exec() == QDialog::Accepted) {
                        try {
                            int iDetailLevel = pDlg->getEnumerator();
                            CEnumMethodTraceDetailLevel eDetailLevel(iDetailLevel);
                            pIdxTree->setMethodCallsTraceDetailLevel(pBranch, eDetailLevel.enumerator());
                        }
                        catch (CException&) {
                        }
                    }
                    CDlgEditEnumValue::DestroyInstance(pDlg);
                    pDlg = nullptr;
                }
            }
        }
    }
} // onActionNameSpaceSetAdminObjsMethodCallsDetailLevelTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsRuntimeInfoDetailLevelTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionNameSpaceSetAdminObjsRuntimeInfoDetailLevelTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());
        if (pModelIdxTree != nullptr) {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();
            CModelIdxTreeEntry* pModelTreeEntry =
                static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());
            if (pIdxTree != nullptr && pModelTreeEntry != nullptr) {
                CIdxTreeLocker idxTreeLocker(pIdxTree);
                CIdxTreeEntry* pBranch = pModelTreeEntry->getIdxTreeEntry();
                if (pBranch != nullptr) {
                    CDlgEditEnumValue* pDlg = CDlgEditEnumValue::CreateInstance(
                        /* strTitle    */ QCoreApplication::applicationName(),
                        /* strObjName  */ "RuntimeInfoDetailLevel",
                        /* pWdgtParent */ this );
                    pDlg->setValueName("RuntimeInfo");
                    pDlg->setComboItems(CEnumLogDetailLevel::s_arEnumEntries);
                    pDlg->setEnumerator(0);
                    if (pDlg->exec() == QDialog::Accepted) {
                        try {
                            int iDetailLevel = pDlg->getEnumerator();
                            CEnumLogDetailLevel eDetailLevel(iDetailLevel);
                            pIdxTree->setRuntimeInfoTraceDetailLevel(pBranch, eDetailLevel.enumerator());
                        }
                        catch (CException&) {
                        }
                    }
                    CDlgEditEnumValue::DestroyInstance(pDlg);
                    pDlg = nullptr;
                }
            }
        }
    }
} // onActionNameSpaceSetAdminObjsRuntimeInfoDetailLevelTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsObjectNameFilterTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionNameSpaceSetAdminObjsObjectNameFilterTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());
        if (pModelIdxTree != nullptr) {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();
            CModelIdxTreeEntry* pModelTreeEntry =
                static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());
            if (pIdxTree != nullptr && pModelTreeEntry != nullptr) {
                CIdxTreeLocker idxTreeLocker(pIdxTree);
                CIdxTreeEntry* pBranch = pModelTreeEntry->getIdxTreeEntry();
                if (pBranch != nullptr) {
                    CDlgTrcAdminObjEditFilterExpressions* pDlg =
                        CDlgTrcAdminObjEditFilterExpressions::GetInstance("TrcAdminObjEditFilterDialog");
                    if (pDlg == nullptr) {
                        QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Edit Filter Expressions";
                        pDlg = CDlgTrcAdminObjEditFilterExpressions::CreateInstance(
                            /* strDlgTitle */ strDlgTitle,
                            /* strObjName  */ "TrcAdminObjEditFilterDialog",
                            /* pIdxTree    */ pIdxTree,
                            /* pWdgtParent */ this );
                        pDlg->setKeyEntryToEdit(pBranch->keyInTree(), EMethodTraceFilterProperty::ObjectName);
                        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
                        pDlg->adjustSize();
                        pDlg->show();
                        QRect rectVisual = visualRect(m_modelIdxSelectedOnMousePressEvent);
                        QPoint ptMousePos = rectVisual.topLeft();
                        QPoint ptEvGlobalPos = mapToGlobal(ptMousePos);
                        pDlg->move(ptEvGlobalPos);
                    }
                    else {
                        if (pDlg->isHidden()) {
                            pDlg->show();
                        }
                        pDlg->raise();
                        pDlg->activateWindow();
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsMethodNameFilterTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionNameSpaceSetAdminObjsMethodNameFilterTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());
        if (pModelIdxTree != nullptr) {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();
            CModelIdxTreeEntry* pModelTreeEntry =
                static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());
            if (pIdxTree != nullptr && pModelTreeEntry != nullptr) {
                CIdxTreeLocker idxTreeLocker(pIdxTree);
                CIdxTreeEntry* pBranch = pModelTreeEntry->getIdxTreeEntry();
                if (pBranch != nullptr) {
                    CDlgTrcAdminObjEditFilterExpressions* pDlg =
                        CDlgTrcAdminObjEditFilterExpressions::GetInstance("TrcAdminObjEditFilterDialog");
                    if (pDlg == nullptr) {
                        QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Edit Filter Expressions";
                        pDlg = CDlgTrcAdminObjEditFilterExpressions::CreateInstance(
                            /* strDlgTitle */ strDlgTitle,
                            /* strObjName  */ "TrcAdminObjEditFilterDialog",
                            /* pIdxTree    */ pIdxTree,
                            /* pWdgtParent */ this );
                        pDlg->setKeyEntryToEdit(pBranch->keyInTree(), EMethodTraceFilterProperty::MethodName);
                        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
                        pDlg->adjustSize();
                        pDlg->show();
                        QRect rectVisual = visualRect(m_modelIdxSelectedOnMousePressEvent);
                        QPoint ptMousePos = rectVisual.topLeft();
                        QPoint ptEvGlobalPos = mapToGlobal(ptMousePos);
                        pDlg->move(ptEvGlobalPos);
                    }
                    else {
                        if (pDlg->isHidden()) {
                            pDlg->show();
                        }
                        pDlg->raise();
                        pDlg->activateWindow();
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTrcAdminObjs::onActionNameSpaceSetAdminObjsTraceDataFilterTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }
    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "onActionNameSpaceSetAdminObjsTraceDataFilterTriggered",
        /* strMethodInArgs    */ strMthInArgs );

    if (m_modelIdxSelectedOnMousePressEvent.isValid()) {
        CModelIdxTreeTrcAdminObjs* pModelIdxTree = dynamic_cast<CModelIdxTreeTrcAdminObjs*>(model());
        if (pModelIdxTree != nullptr) {
            CIdxTreeTrcAdminObjs* pIdxTree = pModelIdxTree->idxTree();
            CModelIdxTreeEntry* pModelTreeEntry =
                static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());
            if (pIdxTree != nullptr && pModelTreeEntry != nullptr) {
                CIdxTreeLocker idxTreeLocker(pIdxTree);
                CIdxTreeEntry* pBranch = pModelTreeEntry->getIdxTreeEntry();
                if (pBranch != nullptr) {
                    CDlgTrcAdminObjEditFilterExpressions* pDlg =
                        CDlgTrcAdminObjEditFilterExpressions::GetInstance("TrcAdminObjEditFilterDialog");
                    if (pDlg == nullptr) {
                        QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Edit Filter Expressions";
                        pDlg = CDlgTrcAdminObjEditFilterExpressions::CreateInstance(
                            /* strDlgTitle */ strDlgTitle,
                            /* strObjName  */ "TrcAdminObjEditFilterDialog",
                            /* pIdxTree    */ pIdxTree,
                            /* pWdgtParent */ this );
                        pDlg->setKeyEntryToEdit(pBranch->keyInTree(), EMethodTraceFilterProperty::TraceData);
                        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
                        pDlg->adjustSize();
                        pDlg->show();
                        QRect rectVisual = visualRect(m_modelIdxSelectedOnMousePressEvent);
                        QPoint ptMousePos = rectVisual.topLeft();
                        QPoint ptEvGlobalPos = mapToGlobal(ptMousePos);
                        pDlg->move(ptEvGlobalPos);
                    }
                    else {
                        if (pDlg->isHidden()) {
                            pDlg->show();
                        }
                        pDlg->raise();
                        pDlg->activateWindow();
                    }
                }
            }
        }
    }
}
