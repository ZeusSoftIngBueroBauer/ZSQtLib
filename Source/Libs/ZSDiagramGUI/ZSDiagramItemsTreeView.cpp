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

#include "ZSDiagramGUI/ZSDiagramItemsTreeView.h"
#include "ZSDiagramGUI/ZSDiagramItemsTreeModel.h"
#include "ZSDiagram/ZSDiagramProcWdgt.h"
#include "ZSSysGUI/ZSSysGUIAux.h"
#include "ZSSysGUI/ZSSysErrDlg.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

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

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Diagram;
using namespace ZS::Diagram::GUI;


/*******************************************************************************
class CDelegateDiagramObjsTreeView : public QStyledItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDelegateDiagramObjsTreeView::CDelegateDiagramObjsTreeView( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QStyledItemDelegate(i_pObjParent),
    m_pEdtName(nullptr),
    m_bEdtNameDestroyedSignalConnected(false),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( QString(i_pObjParent == nullptr ? "Drawing" : i_pObjParent->objectName()) );

    QString strMthInArgs;

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

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
CDelegateDiagramObjsTreeView::~CDelegateDiagramObjsTreeView()
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
QLineEdit* CDelegateDiagramObjsTreeView::nameColumnEditor()
//------------------------------------------------------------------------------
{
    return m_pEdtName;
}

/*==============================================================================
public: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
QWidget* CDelegateDiagramObjsTreeView::createEditor(
    QWidget*                    i_pWdgtParent,
    const QStyleOptionViewItem& i_option,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + CModelDiagramItems::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "createEditor",
        /* strMthInArgs */ strMthInArgs );

    QWidget* pWdgtEditor = nullptr;

    CDelegateDiagramObjsTreeView* pVThis = const_cast<CDelegateDiagramObjsTreeView*>(this);

    if( i_modelIdx.column() == CModelDiagramItems::EColumnDiagObjName )
    {
        pWdgtEditor = QStyledItemDelegate::createEditor(i_pWdgtParent, i_option, i_modelIdx);

        pVThis->m_pEdtName = dynamic_cast<QLineEdit*>(pWdgtEditor);

        if( !m_bEdtNameDestroyedSignalConnected && pVThis->m_pEdtName != nullptr )
        {
            //pVThis->m_pEdtName->installEventFilter(pVThis);

            QObject::connect(
                m_pEdtName, &QLineEdit::destroyed,
                this, &CDelegateDiagramObjsTreeView::onEdtNameDestroyed );
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
void CDelegateDiagramObjsTreeView::setEditorData(
    QWidget*            i_pWdgtEditor,
    const QModelIndex&  i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + CModelDiagramItems::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setEditorData",
        /* strMthInArgs */ strMthInArgs );

    return QStyledItemDelegate::setEditorData(i_pWdgtEditor, i_modelIdx);

} // setEditorData

//------------------------------------------------------------------------------
void CDelegateDiagramObjsTreeView::setModelData(
    QWidget*            i_pWdgtEditor,
    QAbstractItemModel* i_pModel,
    const QModelIndex&  i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs  = "Model: " + QString(i_pModel == nullptr ? "nullptr" : i_pModel->objectName());
        strMthInArgs += ", ModelIdx {" + CModelDiagramItems::modelIdx2Str(i_modelIdx) + "}";
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

    if( i_modelIdx.column() == CModelDiagramItems::EColumnDiagObjName )
    {
        CDelegateDiagramObjsTreeView* pVThis = const_cast<CDelegateDiagramObjsTreeView*>(this);

        pVThis->m_pEdtName = dynamic_cast<QLineEdit*>(i_pWdgtEditor);

        if( pVThis->m_pEdtName != nullptr )
        {
            //QString strName = pVThis->m_pEdtName->text();

            //CModelDiagramItems* pModel = dynamic_cast<CModelDiagramItems*>(i_pModel);

            //SErrResultInfo errResultInfo = pModel->canSetData(i_modelIdx, strName, Qt::EditRole);

            //if( errResultInfo.isErrorResult() )
            //{
            //    CDlgErr* pDlgErr = new CDlgErr("Error");
            //    pDlgErr->setErrResultInfo(errResultInfo);
            //    pDlgErr->show();
            //}
            //else // if( !errResultInfo.isErrorResult() )
            //{
            //    pModel->setData(i_modelIdx, strName, Qt::EditRole);
            //}
        } // if( pVThis->m_pEdtName != nullptr )
    } // if( i_modelIdx.column() == CModelIdxTree::EColumnTreeEntryName )

} // setModelData

//------------------------------------------------------------------------------
void CDelegateDiagramObjsTreeView::updateEditorGeometry(
    QWidget*                    i_pWdgtEditor,
    const QStyleOptionViewItem& i_option,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + CModelDiagramItems::modelIdx2Str(i_modelIdx) + "}";
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
void CDelegateDiagramObjsTreeView::onEdtNameDestroyed( QObject* /*i_pWdgtEditor*/ )
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
class CTreeViewDiagramItems : public QTreeView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewDiagramItems::CTreeViewDiagramItems(CWdgtDiagram* i_pDiagram, QWidget* i_pWdgtParent) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pDiagram(i_pDiagram),
    m_pDelegate(nullptr),
    m_pModel(nullptr),
    m_pMenuDiagramObjContext(nullptr),
    m_pActionDiagramObjTitle(nullptr),
    m_pActionDiagramObjExpand(nullptr),
    m_pActionDiagramObjCollapse(nullptr),
    m_pActionDiagramObjDelete(nullptr),
    m_modelIdxSelectedOnMousePressEvent(),
    m_modelIdxSelectedOnMouseReleaseEvent(),
    m_bSilentlyExecuteDeleteRequests(false),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjEvent(nullptr)
{
    setObjectName( QString(i_pDiagram == nullptr ? "Diagram" : i_pDiagram->objectName()) );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjEvent = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName() + "-Event");

    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = QString(i_pDiagram == nullptr ? "nullptr" : i_pDiagram->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    m_pModel = new CModelDiagramItems(m_pDiagram);
    setModel(m_pModel);

    //m_pDelegate = new CDelegateDiagramObjsTreeView(this);

    //setItemDelegate(m_pDelegate);

    for( int idxClm = 0; idxClm < CModelDiagramItems::EColumnCount; ++idxClm ) {
        if (idxClm != CModelDiagramItems::EColumnDiagObjClassName
         && idxClm != CModelDiagramItems::EColumnDiagObjName) {
            hideColumn(idxClm);
        }
    }

    // Context Popup Menu of Branches
    //-------------------------------

    m_pMenuDiagramObjContext = new QMenu("Object", this);

    m_pActionDiagramObjTitle = new QAction("Object:", this);

    QFont fntActionTitle = m_pActionDiagramObjTitle->font();
    fntActionTitle.setBold(true);
    m_pActionDiagramObjTitle->setFont(fntActionTitle);
    m_pMenuDiagramObjContext->addAction(m_pActionDiagramObjTitle);

    m_pMenuDiagramObjContext->addSeparator();

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.bmp");
    pxmExpandAll.setMask(pxmExpandAll.createHeuristicMask());
    //pxmExpandAll = pxmExpandAll.scaled(16,16);
    m_pActionDiagramObjExpand = new QAction(pxmExpandAll, "Expand", this);
    m_pMenuDiagramObjContext->addAction(m_pActionDiagramObjExpand);

    QObject::connect(
        m_pActionDiagramObjExpand, &QAction::triggered,
        this, &CTreeViewDiagramItems::onActionDiagramObjExpandTriggered );

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.bmp");
    pxmCollapseAll.setMask(pxmCollapseAll.createHeuristicMask());

    m_pActionDiagramObjCollapse = new QAction(pxmCollapseAll, "Collapse", this);
    m_pMenuDiagramObjContext->addAction(m_pActionDiagramObjCollapse);

    QObject::connect(
        m_pActionDiagramObjCollapse, &QAction::triggered,
        this, &CTreeViewDiagramItems::onActionDiagramObjCollapseTriggered );

    m_pMenuDiagramObjContext->addSeparator();

    m_pActionDiagramObjDelete = new QAction("Delete", this);
    m_pMenuDiagramObjContext->addAction(m_pActionDiagramObjDelete);

    QObject::connect(
        m_pActionDiagramObjDelete, &QAction::triggered,
        this, &CTreeViewDiagramItems::onActionDiagramObjDeleteTriggered );

    m_pMenuDiagramObjContext->addSeparator();

    // Connect to signals of the tree view
    //------------------------------------

    QObject::connect(
        this, &CTreeViewDiagramItems::collapsed,
        this, &CTreeViewDiagramItems::onCollapsed );
    QObject::connect(
        this, &CTreeViewDiagramItems::expanded,
        this, &CTreeViewDiagramItems::onExpanded );

} // ctor

//------------------------------------------------------------------------------
CTreeViewDiagramItems::~CTreeViewDiagramItems()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strMthInArgs */ strMthInArgs );

    mthTracer.onAdminObjAboutToBeReleased();

    try
    {
        delete m_pModel;
    }
    catch(...)
    {
    }
    m_pModel = nullptr;

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjEvent);

    m_pDiagram = nullptr;
    m_pDelegate = nullptr;
    m_pMenuDiagramObjContext = nullptr;
    m_pActionDiagramObjTitle = nullptr;
    m_pActionDiagramObjExpand = nullptr;
    m_pActionDiagramObjCollapse = nullptr;
    m_pActionDiagramObjDelete = nullptr;
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
void CTreeViewDiagramItems::setSilentlyExecuteDeleteRequests( bool i_bExecuteSilently )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ExecuteSilently: " + bool2Str(i_bExecuteSilently);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSilentlyExecuteDeleteRequests",
        /* strMthInArgs */ strMthInArgs );

    m_bSilentlyExecuteDeleteRequests = i_bExecuteSilently;

} // setSilentlyExecuteDeleteRequests

/*==============================================================================
public: // overridable slots of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::expandAll()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "expandAll",
        /* strMthInArgs */ strMthInArgs );

    QTreeView::expandAll();

    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());

    //expand(modelIdxRoot);

    expandRecursive(modelIdxRoot);

} // expandAll

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::collapseAll()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "collapseAll",
        /* strMthInArgs */ strMthInArgs );

    QTreeView::collapseAll();

    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());

    collapseRecursive(modelIdxRoot);

} // collapseAll

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::expandRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + CModelDiagramItems::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "expandRecursive",
        /* strMthInArgs */ strMthInArgs );

    //CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

    //if( pModelTreeEntry != nullptr )
    //{
    //    if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
    //    {
    //        expand(i_modelIdx);

    //        CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
    //        CModelIdxTreeEntry* pModelTreeEntryChild;

    //        QModelIndex modelIdxChild;

    //        for( int idxEntry = 0; idxEntry < pModelBranch->count(); ++idxEntry )
    //        {
    //            pModelTreeEntryChild = pModelBranch->at(idxEntry);

    //            if( pModelTreeEntryChild->isRoot() || pModelTreeEntryChild->isBranch() )
    //            {
    //                modelIdxChild = model()->index(idxEntry, 0, i_modelIdx);
    //                expandRecursive(modelIdxChild);
    //            }
    //        }
    //    } // if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
    //} // if( pModelTreeEntry != nullptr )

} // expandRecursive

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::collapseRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + CModelDiagramItems::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "collapseRecursive",
        /* strMthInArgs */ strMthInArgs );

    //CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

    //if( pModelTreeEntry != nullptr )
    //{
    //    if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
    //    {
    //        CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
    //        QModelIndex modelIdx;

    //        for( int idxEntry = 0; idxEntry < pModelBranch->count(); ++idxEntry )
    //        {
    //            pModelTreeEntry = pModelBranch->at(idxEntry);

    //            if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
    //            {
    //                modelIdx = model()->index(idxEntry, 0, i_modelIdx);
    //                collapseRecursive(modelIdx);
    //            }
    //        }

    //        collapse(i_modelIdx);

    //    } // if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
    //} // if( pModelTreeEntry != nullptr )

} // collapseRecursive

/*==============================================================================
public: // slots (hiding not overridable slots with same name in QTreeView)
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::expand( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + CModelDiagramItems::modelIdx2Str(i_modelIdx) + "}";
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

    if( i_modelIdx.isValid() )
    {
        onExpanded(i_modelIdx);
    }

} // expand

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::collapse( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + CModelDiagramItems::modelIdx2Str(i_modelIdx) + "}";
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

    if( i_modelIdx.isValid() )
    {
        onCollapsed(i_modelIdx);
    }

} // collapse

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::onCollapsed( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + CModelDiagramItems::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onCollapsed",
        /* strMthInArgs */ strMthInArgs );

    //if( i_modelIdx.isValid() )
    //{
    //    CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());

    //    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

    //    if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
    //    {
    //        CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
    //        pModelIdxTree->setIsExpanded(pModelBranch, false);
    //    }
    //} // if( i_modelIdx.isValid() )

} // onCollapsed

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::onExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "ModelIdx {" + CModelDiagramItems::modelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onExpanded",
        /* strMthInArgs */ strMthInArgs );

    //if( i_modelIdx.isValid() )
    //{
    //    #if QT_VERSION >= 0x040000 && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    //    //resizeColumnToContents(i_modelIdx.column());
    //    #elif QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    //    // Cannot invoke resizeColumnToContents as this leads to an exception.
    //    #endif

    //    CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());

    //    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

    //    if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
    //    {
    //        CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
    //        pModelIdxTree->setIsExpanded(pModelBranch, true);
    //    }
    //} // if( i_modelIdx.isValid() )

} // onExpanded

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CTreeViewDiagramItems::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjEvent != nullptr && m_pTrcAdminObjEvent->areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal) )
    {
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
void CTreeViewDiagramItems::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
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

    //if( modelIdxSelected.isValid() )
    //{
    //    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdxSelected.internalPointer());

    //    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    //    {
    //        strAddTrcInfo = "ModelIdxSelected {" + CModelIdxTree::modelIdx2Str(modelIdxSelected) + "}";
    //        mthTracer.trace(strAddTrcInfo);
    //    }

    //    if( pModelTreeEntry != nullptr )
    //    {
    //        switch( i_pEv->key() )
    //        {
    //            case Qt::Key_F2: // Rename
    //            {
    //                // Handled by delegate.
    //                break;
    //            }
    //            case Qt::Key_Escape: // Undo selection
    //            {
    //                // Not handled by TreeView but BranchContentView.
    //                break;
    //            }
    //            case Qt::Key_Delete:
    //            {
    //                CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pModelTreeEntry->getIdxTreeEntry());

    //                if( pGraphObj != nullptr && !pGraphObj->isRoot() )
    //                {
    //                    // Selection points and labels cannot be deleted by the user
    //                    // but will be implicitly deleted if the item is deselected.
    //                    if( !pGraphObj->isSelectionPoint() && !pGraphObj->isLabel() )
    //                    {
    //                        int iRet = QMessageBox::Yes;

    //                        if( !m_bSilentlyExecuteDeleteRequests )
    //                        {
    //                            QString strMsg = "Do you really want to delete \"" + pModelTreeEntry->keyInTree() + "\"?";
    //                            iRet = QMessageBox::question(
    //                                /* pWdgtParent     */ this,
    //                                /* strTitle        */ ZS::System::GUI::getMainWindowTitle(),
    //                                /* strText         */ strMsg,
    //                                /* standardButtons */ QMessageBox::Yes | QMessageBox::No,
    //                                /* defaultButton   */ QMessageBox::No );
    //                        }
    //                        if( iRet == QMessageBox::Yes )
    //                        {
    //                            delete pGraphObj;
    //                            pGraphObj = nullptr;
    //                            pModelTreeEntry = nullptr;
    //                        }
    //                    } // if( pGraphObj->getType() != EGraphObjTypeSelectionPoint && pGraphObj->getType() != EGraphObjTypeLabel )
    //                } // if( pGraphObj != nullptr && !pGraphObj->isRoot() )
    //                bEventHandled = true;
    //                break;
    //            }
    //            default:
    //            {
    //                break;
    //            }
    //        } // switch( i_pEv->key() )
    //    } // if( pModelTreeEntry != nullptr )
    //} // if( modelIdxSelected.isValid() )

    if( !bEventHandled )
    {
        if( i_pEv->key() != Qt::Key_F2 )
        {
            QTreeView::keyPressEvent(i_pEv);
        }
        else
        {
            QTreeView::keyPressEvent(i_pEv);
        }
    }

} // keyPressEvent

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mousePressEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMousePressEvent = indexAt(i_pEv->pos());

    //if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    //{
    //    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug ) )
    //    {
    //        strAddTrcInfo = "ModelIdxPressed {" + CModelIdxTree::modelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
    //        mthTracer.trace(strAddTrcInfo);
    //    }

    //    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

    //    if( pModelTreeEntry != nullptr )
    //    {
    //        CGraphObj* pGraphObjClicked = dynamic_cast<CGraphObj*>(pModelTreeEntry->getIdxTreeEntry());

    //        if( pGraphObjClicked != nullptr && !pGraphObjClicked->isRoot() )
    //        {
    //            QGraphicsItem* pGraphicsItemClicked = dynamic_cast<QGraphicsItem*>(pGraphObjClicked);

    //            CGraphObj*     pGraphObj = pGraphObjClicked;
    //            QGraphicsItem* pGraphicsItem = pGraphicsItemClicked;

    //            // Ensure that only the clicked graphic item is selected taking into accout that selection points
    //            // and labels cannot be selected but instead select their parents (if not already selected).
    //            if( pGraphObjClicked->isSelectionPoint() || pGraphObjClicked->isLabel() )
    //            {
    //                pGraphObj = pGraphObjClicked->parentGraphObj();
    //                // Selection points and labels don't have graphics item as a parent.
    //                // Instead we must use the parent graph object in the index tree.
    //                pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);
    //            }

    //            if( pGraphicsItem != nullptr )
    //            {
    //                if( i_pEv->buttons() & Qt::LeftButton )
    //                {
    //                }
    //                else if( i_pEv->buttons() & Qt::RightButton )
    //                {
    //                    if( m_pMenuDiagramObjContext != nullptr )
    //                    {
    //                        m_pActionDiagramObjTitle->setText( "Object: " + pGraphObj->name() );
    //                        m_pMenuDiagramObjContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
    //                    }
    //                    bEventHandled = true;
    //                }
    //            } // if( pGraphicsItemClicked != nullptr )
    //        } // if( pGraphObjClicked != nullptr && !pGraphObjClicked->isRoot() )
    //    } // if( pModelTreeEntry != nullptr )
    //} // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

    if( !bEventHandled )
    {
        QTreeView::mousePressEvent(i_pEv);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseReleaseEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMouseReleaseEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMouseReleaseEvent.isValid() )
    {
        if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug ) )
        {
            strAddTrcInfo = "ModelIdxReleased {" + CModelDiagramItems::modelIdx2Str(m_modelIdxSelectedOnMouseReleaseEvent) + "}";
            mthTracer.trace(strAddTrcInfo);
        }
    }

    if( !bEventHandled )
    {
        QTreeView::mouseReleaseEvent(i_pEv);
    }

} // mouseReleaseEvent

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::mouseDoubleClickEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;

    QModelIndex modelIdx = indexAt(i_pEv->pos());

    //if( modelIdx.isValid() )
    //{
    //    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    //    {
    //        strAddTrcInfo = "ModelIdxClicked {" + CModelIdxTree::modelIdx2Str(modelIdx) + "}";
    //        mthTracer.trace(strAddTrcInfo);
    //    }

    //    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdx.internalPointer());

    //    if( pModelTreeEntry != nullptr )
    //    {
    //        CGraphObj* pGraphObjClicked = dynamic_cast<CGraphObj*>(pModelTreeEntry->getIdxTreeEntry());

    //        if( pGraphObjClicked != nullptr && !pGraphObjClicked->isRoot() )
    //        {
    //            QGraphicsItem* pGraphicsItemClicked = dynamic_cast<QGraphicsItem*>(pGraphObjClicked);

    //            if( pGraphicsItemClicked != nullptr )
    //            {
    //                if( i_pEv->buttons() & Qt::LeftButton )
    //                {
    //                    CGraphObj*     pGraphObjSelected = pGraphObjClicked;
    //                    QGraphicsItem* pGraphicsItemSelected = pGraphicsItemClicked;

    //                    // Ensure that only the clicked graphic item is selected taking into accout that selection points
    //                    // and labels cannot be selected but instead select their parents (if not already selected).
    //                    if( pGraphObjClicked->isSelectionPoint() || pGraphObjClicked->isLabel() )
    //                    {
    //                        pGraphObjSelected = pGraphObjClicked->parentGraphObj();
    //                        // Selection points and labels don't have graphics item as a parent.
    //                        // Instead we must use the parent graph object in the index tree.
    //                        pGraphicsItemSelected = dynamic_cast<QGraphicsItem*>(pGraphObjSelected);
    //                    }

    //                    CDlgFormatGraphObjs* pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDiagram, pGraphObjSelected);

    //                    pDlgFormatGraphObjs->setCurrentWidget(CDlgFormatGraphObjs::c_strWdgtLabels);

    //                    pDlgFormatGraphObjs->exec();

    //                    delete pDlgFormatGraphObjs;
    //                    pDlgFormatGraphObjs = nullptr;
    //                }
    //                else if( i_pEv->buttons() & Qt::RightButton )
    //                {
    //                    if( m_pMenuDiagramObjContext != nullptr )
    //                    {
    //                        m_pActionDiagramObjTitle->setText( "Object: " + pModelTreeEntry->name() );
    //                        m_pMenuDiagramObjContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
    //                    }
    //                    bEventHandled = true;
    //                }
    //            } // if( pGraphicsItem != nullptr )
    //        } // if( pGraphObjClicked != nullptr && !pGraphObjClicked->isRoot() )
    //    } // if( pModelTreeEntry != nullptr )
    //} // if( modelIdx.isValid() )

    if( !bEventHandled )
    {
        QTreeView::mouseDoubleClickEvent(i_pEv);
    }

} // mouseDoubleClickEvent

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::mouseMoveEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "mouseMoveEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;

    if( !bEventHandled )
    {
        QTreeView::mouseMoveEvent(i_pEv);
    }
}

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::selectionChanged(
    const QItemSelection& i_selected,
    const QItemSelection& i_deselected )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Selected: " + QString::number(i_selected.indexes().size());
        if( i_selected.indexes().size() > 0 )
        {
            strMthInArgs += " [";
            for( const auto& modelIdx : i_selected.indexes() )
            {
                if( !strMthInArgs.endsWith("[") ) strMthInArgs += ", ";
                strMthInArgs += CModelDiagramItems::modelIdx2Str(modelIdx);
            }
            strMthInArgs += "]";
        }
        strMthInArgs += ", Deselected: " + QString::number(i_deselected.indexes().size());
        if( i_deselected.indexes().size() > 0 )
        {
            strMthInArgs += " [";
            for( const auto& modelIdx : i_deselected.indexes() )
            {
                if( !strMthInArgs.endsWith("[") ) strMthInArgs += ", ";
                strMthInArgs += CModelDiagramItems::modelIdx2Str(modelIdx);
            }
            strMthInArgs += "]";
        }
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "selectionChanged",
        /* strMthInArgs */ strMthInArgs );

    //// The selection contains each cell (each column).
    //// We are only interested in the rows and duplicate entries will be removed.

    //QHash<QString, QGraphicsItem*> hshGraphicsItemsSelected;

    //for( const auto& modelIdx : i_selected.indexes() )
    //{
    //    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdx.internalPointer());

    //    if( pModelTreeEntry != nullptr && !pModelTreeEntry->getIdxTreeEntry()->isAboutToBeDestroyed() )
    //    {
    //        CGraphObj* pGraphObjSelected = dynamic_cast<CGraphObj*>(pModelTreeEntry->getIdxTreeEntry());

    //        if( pGraphObjSelected != nullptr && !pGraphObjSelected->isRoot() )
    //        {
    //            QGraphicsItem* pGraphicsItemSelected = dynamic_cast<QGraphicsItem*>(pGraphObjSelected);

    //            CGraphObj*     pGraphObj = pGraphObjSelected;
    //            QGraphicsItem* pGraphicsItem = pGraphicsItemSelected;

    //            // Selection points and labels cannot be selected but instead select their parents (if not already selected).
    //            if( pGraphObj->isSelectionPoint() || pGraphObj->isLabel() )
    //            {
    //                pGraphObj = pGraphObj->parentGraphObj();
    //                // Selection points and labels don't have graphics item as a parent.
    //                // Instead we must use the parent graph object in the index tree.
    //                pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);
    //            }

    //            if( pGraphicsItem != nullptr )
    //            {
    //                if( !hshGraphicsItemsSelected.contains(pGraphObj->keyInTree()) )
    //                {
    //                    hshGraphicsItemsSelected.insert(pGraphObj->keyInTree(), pGraphicsItem);
    //                }
    //            }
    //        } // if( pGraphObjSelected != nullptr && !pGraphObjSelected->isRoot() )
    //    } // if( pModelTreeEntry != nullptr && !pModelTreeEntry->isAboutToBeDestroyed() )
    //} // for( const auto& modelIdx : i_selected.indexes() )

    //QHash<QString, QGraphicsItem*> hshGraphicsItemsDeselected;

    //for( const auto& modelIdx : i_deselected.indexes() )
    //{
    //    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdx.internalPointer());

    //    if( pModelTreeEntry != nullptr && !pModelTreeEntry->getIdxTreeEntry()->isAboutToBeDestroyed() )
    //    {
    //        CGraphObj* pGraphObjSelected = dynamic_cast<CGraphObj*>(pModelTreeEntry->getIdxTreeEntry());

    //        if( pGraphObjSelected != nullptr && !pGraphObjSelected->isRoot() )
    //        {
    //            QGraphicsItem* pGraphicsItemSelected = dynamic_cast<QGraphicsItem*>(pGraphObjSelected);

    //            CGraphObj*     pGraphObj = pGraphObjSelected;
    //            QGraphicsItem* pGraphicsItem = pGraphicsItemSelected;

    //            // Selection points and labels can neither be selected nor deselected.
    //            if( pGraphObj->isSelectionPoint() || pGraphObj->isLabel() )
    //            {
    //                pGraphObj = pGraphObj->parentGraphObj();
    //                // Selection points and labels don't have graphics item as a parent.
    //                // Instead we must use the parent graph object in the index tree.
    //                pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);
    //            }

    //            if( pGraphicsItem != nullptr )
    //            {
    //                // If the tree view entry for a selection point or label is deselected it might be that
    //                // the parent is still selected or has been selected by selecting one of its child
    //                // selection points or labels. So we add the parent object only to the hash of items
    //                // to be deselected if the item has not inserted into the hash of selected items.
    //                if( !hshGraphicsItemsSelected.contains(pGraphObj->keyInTree()) && !hshGraphicsItemsDeselected.contains(pGraphObj->keyInTree()) )
    //                {
    //                    hshGraphicsItemsDeselected.insert(pGraphObj->keyInTree(), pGraphicsItem);
    //                }
    //            }
    //        } // if( pGraphObjSelected != nullptr && !pGraphObjSelected->isRoot() )
    //    } // if( pModelTreeEntry != nullptr && !pModelTreeEntry->isAboutToBeDestroyed() )
    //} // for( const auto& modelIdx : i_deselected.indexes() )

    //for( auto* pGraphicsItem : hshGraphicsItemsDeselected )
    //{
    //    pGraphicsItem->setSelected(false);
    //}

    //for( auto* pGraphicsItem : hshGraphicsItemsSelected )
    //{
    //    pGraphicsItem->setSelected(true);
    //}

} // selectionChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::onActionDiagramObjExpandTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDiagramObjExpandTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        expandRecursive(m_modelIdxSelectedOnMousePressEvent);

        for( int idxClm = 0; idxClm < CModelDiagramItems::EColumnCount; idxClm++ )
        {
            resizeColumnToContents(idxClm);
        }
    }
} // onActionDiagramObjExpandTriggered

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::onActionDiagramObjCollapseTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDiagramObjCollapseTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        collapseRecursive(m_modelIdxSelectedOnMousePressEvent);
    }
} // onActionDiagramObjCollapseTriggered

//------------------------------------------------------------------------------
void CTreeViewDiagramItems::onActionDiagramObjDeleteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strAddTrcInfo;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onActionDiagramObjDeleteTriggered",
        /* strMthInArgs */ strMthInArgs );

    //if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    //{
    //    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    //    {
    //        strAddTrcInfo = "ModelIdxSelected {" + CModelDiagramItems::modelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
    //        mthTracer.trace(strAddTrcInfo);
    //    }

    //    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

    //    if( pModelTreeEntry != nullptr )
    //    {
    //        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pModelTreeEntry->getIdxTreeEntry());

    //        if( pGraphObj != nullptr && !pGraphObj->isRoot() )
    //        {
    //            // Selection points and labels cannot be deleted by the user
    //            // but will be implicitly deleted if the item is deselected.
    //            if( !pGraphObj->isSelectionPoint() && !pGraphObj->isLabel() )
    //            {
    //                int iRet = QMessageBox::Yes;

    //                if( !m_bSilentlyExecuteDeleteRequests )
    //                {
    //                    QString strMsg = "Do you really want to delete \"" + pModelTreeEntry->keyInTree() + "\"?";
    //                    iRet = QMessageBox::question(
    //                        /* pWdgtParent     */ this,
    //                        /* strTitle        */ ZS::System::GUI::getMainWindowTitle(),
    //                        /* strText         */ strMsg,
    //                        /* standardButtons */ QMessageBox::Yes | QMessageBox::No,
    //                        /* defaultButton   */ QMessageBox::No );
    //                }
    //                if( iRet == QMessageBox::Yes )
    //                {
    //                    delete pGraphObj;
    //                    pGraphObj = nullptr;
    //                    pModelTreeEntry = nullptr;
    //                }
    //            } // if( pGraphObj->getType() != EGraphObjTypeSelectionPoint && pGraphObj->getType() != EGraphObjTypeLabel )
    //        } // if( pGraphObj != nullptr && !pGraphObj->isRoot() )
    //    } // if( pModelTreeEntry != nullptr )
    //} // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionDiagramObjDeleteTriggered
