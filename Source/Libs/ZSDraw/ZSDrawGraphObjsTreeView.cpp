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

#include "ZSDraw/ZSDrawGraphObjsTreeView.h"
#include "ZSDraw/ZSDrawGraphObjsTreeModel.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawDlgFormatGraphObjs.h"
#include "ZSDraw/ZSDrawGraphObjSelectionPoint.h"
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
using namespace ZS::Trace;


/*******************************************************************************
class CDelegateIdxTreeGraphObjs : public QStyledItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDelegateIdxTreeGraphObjs::CDelegateIdxTreeGraphObjs( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QStyledItemDelegate(i_pObjParent),
    m_pEdtName(nullptr),
    m_bEdtNameDestroyedSignalConnected(false),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( QString(i_pObjParent == nullptr ? "IdxTree" : i_pObjParent->objectName()) );

    QString strMthInArgs;

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "Parent: " + QString(i_pObjParent == nullptr ? "nullptr" : i_pObjParent->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strMthInArgs */ strMthInArgs );

} // ctor

//------------------------------------------------------------------------------
CDelegateIdxTreeGraphObjs::~CDelegateIdxTreeGraphObjs()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
QLineEdit* CDelegateIdxTreeGraphObjs::nameColumnEditor()
//------------------------------------------------------------------------------
{
    return m_pEdtName;
}

/*==============================================================================
public: // overridables of base class QStyledItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
QWidget* CDelegateIdxTreeGraphObjs::createEditor(
    QWidget*                    i_pWdgtParent,
    const QStyleOptionViewItem& i_option,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createEditor",
        /* strMthInArgs */ strMthInArgs );

    QWidget* pWdgtEditor = nullptr;

    CDelegateIdxTreeGraphObjs* pVThis = const_cast<CDelegateIdxTreeGraphObjs*>(this);

    if( i_modelIdx.column() == CModelIdxTree::EColumnTreeEntryName )
    {
        pWdgtEditor = QStyledItemDelegate::createEditor(i_pWdgtParent, i_option, i_modelIdx);
        //pWdgtEditor = new CLineEditIdxTreeClmName(m_pTrcMthFile, m_mthTrcDetailLevel, i_pWdgtParent);

        pVThis->m_pEdtName = dynamic_cast<QLineEdit*>(pWdgtEditor);

        if( !m_bEdtNameDestroyedSignalConnected && pVThis->m_pEdtName != nullptr )
        {
            //pVThis->m_pEdtName->installEventFilter(pVThis);

            pVThis->m_bEdtNameDestroyedSignalConnected = QObject::connect(
                /* pObjSender   */ m_pEdtName,
                /* szSignal     */ SIGNAL( destroyed(QObject*) ),
                /* pObjReceiver */ this,
                /* szSlot       */ SLOT( onEdtNameDestroyed(QObject*) ) );
            if( !m_bEdtNameDestroyedSignalConnected )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
    } // if( i_modelIdx.column() == CModelIdxTree::EColumnTreeEntryName )

    else
    {
        pWdgtEditor = QStyledItemDelegate::createEditor(i_pWdgtParent, i_option, i_modelIdx);
    }

    return pWdgtEditor;

} // createEditor

//------------------------------------------------------------------------------
void CDelegateIdxTreeGraphObjs::setEditorData(
    QWidget*            i_pWdgtEditor,
    const QModelIndex&  i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setEditorData",
        /* strMthInArgs */ strMthInArgs );

    return QStyledItemDelegate::setEditorData(i_pWdgtEditor, i_modelIdx);

} // setEditorData

//------------------------------------------------------------------------------
void CDelegateIdxTreeGraphObjs::setModelData(
    QWidget*            i_pWdgtEditor,
    QAbstractItemModel* i_pModel,
    const QModelIndex&  i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs  = "Model: " + QString(i_pModel == nullptr ? "nullptr" : i_pModel->objectName());
        strMthInArgs += ", ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setModelData",
        /* strMthInArgs */ strMthInArgs );

    if( i_pModel == nullptr )
    {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange, "i_pModel == nullptr");
    }

    if( i_modelIdx.column() == CModelIdxTree::EColumnTreeEntryName )
    {
        CDelegateIdxTreeGraphObjs* pVThis = const_cast<CDelegateIdxTreeGraphObjs*>(this);

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
void CDelegateIdxTreeGraphObjs::updateEditorGeometry(
    QWidget*                    i_pWdgtEditor,
    const QStyleOptionViewItem& i_option,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "updateEditorGeometry",
        /* strMthInArgs */ strMthInArgs );

    return QStyledItemDelegate::updateEditorGeometry(i_pWdgtEditor, i_option, i_modelIdx);

} // updateEditorGeometry

/*==============================================================================
protected: // slots
==============================================================================*/

//------------------------------------------------------------------------------
void CDelegateIdxTreeGraphObjs::onEdtNameDestroyed( QObject* /*i_pWdgtEditor*/ )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onEdtNameDestroyed",
        /* strMthInArgs */ strMthInArgs );

    m_bEdtNameDestroyedSignalConnected = false;
    m_pEdtName = nullptr;

} // onEdtNameDestroyed


/*******************************************************************************
class CTreeViewIdxTreeGraphObjs : public QTreeView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewIdxTreeGraphObjs::CTreeViewIdxTreeGraphObjs(
    CModelIdxTreeGraphObjs* i_pModel,
    QWidget*                i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pDrawingScene(i_pModel->getDrawingScene()),
    m_pDelegate(nullptr),
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
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjEvent(nullptr)
{
    setObjectName( QString(i_pModel == nullptr ? "IdxTree" : i_pModel->objectName()) );

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjEvent = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName() + "-Event");

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "Model: " + QString(i_pModel == nullptr ? "nullptr" : i_pModel->objectName());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iFilterDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    setModel(i_pModel);

    m_pDelegate = new CDelegateIdxTreeGraphObjs(this);

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

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.bmp");
    pxmExpandAll.setMask(pxmExpandAll.createHeuristicMask());
    //pxmExpandAll = pxmExpandAll.scaled(16,16);
    m_pActionBranchExpand = new QAction(pxmExpandAll, "Expand", this);
    m_pMenuBranchContext->addAction(m_pActionBranchExpand);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionBranchExpand,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionBranchExpandTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.bmp");
    pxmCollapseAll.setMask(pxmCollapseAll.createHeuristicMask());

    m_pActionBranchCollapse = new QAction(pxmCollapseAll, "Collapse", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCollapse);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionBranchCollapse,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionBranchCollapseTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pMenuBranchContext->addSeparator();

    m_pActionBranchCreateNewBranch = new QAction("Create New Branch", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCreateNewBranch);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionBranchCreateNewBranch,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionBranchCreateNewBranchTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionBranchCreateNewLeave = new QAction("Create New Leave", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCreateNewLeave);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionBranchCreateNewLeave,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionBranchCreateNewLeaveTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionBranchDelete = new QAction("Delete", this);
    m_pMenuBranchContext->addAction(m_pActionBranchDelete);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionBranchDelete,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionBranchDeleteTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pMenuBranchContext->addSeparator();

    m_pActionBranchCut = new QAction("Cut", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCut);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionBranchCut,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionBranchCutTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionBranchCopy = new QAction("Copy", this);
    m_pMenuBranchContext->addAction(m_pActionBranchCopy);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionBranchCopy,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionBranchCopyTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionBranchPaste = new QAction("Paste", this);
    m_pMenuBranchContext->addAction(m_pActionBranchPaste);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionBranchPaste,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionBranchPasteTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Context Popup Menu of Leaves
    //-------------------------------

    m_pMenuLeaveContext = new QMenu("Leave", this);

    m_pActionLeaveTitle = new QAction("Leave:", this);

    m_pActionLeaveTitle->setFont(fntActionTitle);
    m_pMenuLeaveContext->addAction(m_pActionLeaveTitle);

    m_pMenuLeaveContext->addSeparator();

    m_pActionLeaveDelete = new QAction("Delete", this);
    m_pMenuLeaveContext->addAction(m_pActionLeaveDelete);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionLeaveDelete,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionLeaveDeleteTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pMenuLeaveContext->addSeparator();

    m_pActionLeaveCut = new QAction("Cut", this);
    m_pMenuLeaveContext->addAction(m_pActionLeaveCut);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionLeaveCut,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionLeaveCutTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionLeaveCopy = new QAction("Copy", this);
    m_pMenuLeaveContext->addAction(m_pActionLeaveCopy);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionLeaveCopy,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionLeaveCopyTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionLeavePaste = new QAction("Paste", this);
    m_pMenuLeaveContext->addAction(m_pActionLeavePaste);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionLeavePaste,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionLeavePasteTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Connect to signals of the tree view
    //------------------------------------

    if( !QObject::connect(
        /* pObjSender   */ this,
        /* szSignal     */ SIGNAL( collapsed(const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onCollapsed(const QModelIndex&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ this,
        /* szSignal     */ SIGNAL( expanded(const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onExpanded(const QModelIndex&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ selectionModel(),
        /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onCurrentChanged(const QModelIndex&, const QModelIndex&) ) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }
    if( !QObject::connect(
        /* pObjSender   */ this,
        /* szSignal     */ SIGNAL( doubleClicked(const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onDoubleClicked(const QModelIndex&) ) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }


    // Connect to signals of the drawing scene
    //----------------------------------------

    if( !QObject::connect(
        /* pObjSender   */ m_pDrawingScene,
        /* szSignal     */ SIGNAL(selectionChanged()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onDrawingSceneSelectionChanged()) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

} // ctor

//------------------------------------------------------------------------------
CTreeViewIdxTreeGraphObjs::~CTreeViewIdxTreeGraphObjs()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strMthInArgs */ strMthInArgs );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_pDrawingScene = nullptr;
    m_pDelegate = nullptr;
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
    m_pTrcAdminObjEvent = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::setSortOrder( EIdxTreeSortOrder i_sortOrder )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "SortOrder: " + idxTreeSortOrder2Str(i_sortOrder);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setSortOrder",
        /* strMthInArgs */ strMthInArgs );

    if( static_cast<int>(i_sortOrder) < 0 || i_sortOrder >= EIdxTreeSortOrder::Count )
    {
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

    for( auto pModelTreeEntry : mappTreeEntries )
    {
        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root
         || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            pModelBranch = pModelTreeEntry;

            if( pModelBranch->isExpanded() && pModel->areAllParentBranchesExpanded(pModelBranch) )
            {
                strlstKeysOfExpandedModelEntries << pModelTreeEntry->keyInTree();
            }
        }
    }

    pModel->setSortOrder(i_sortOrder);

    QModelIndex modelIdx;

    for( auto strKey : strlstKeysOfExpandedModelEntries )
    {
        modelIdx = pModel->index(strKey, 0);
        expand(modelIdx);
    }

} // setSortOrder

//------------------------------------------------------------------------------
EIdxTreeSortOrder CTreeViewIdxTreeGraphObjs::sortOrder() const
//------------------------------------------------------------------------------
{
    CModelIdxTree* pModel = dynamic_cast<CModelIdxTree*>(model());
    return pModel->sortOrder();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::setSilentlyExecuteDeleteRequests( bool i_bExecuteSilently )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "ExecuteSilently: " + bool2Str(i_bExecuteSilently);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setSilentlyExecuteDeleteRequests",
        /* strMthInArgs */ strMthInArgs );

    m_bSilentlyExecuteDeleteRequests = i_bExecuteSilently;

} // setSilentlyExecuteDeleteRequests

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::setSilentlyIgnoreInvalidCopyRequests( bool i_bExecuteSilently )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "ExecuteSilently: " + bool2Str(i_bExecuteSilently);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setSilentlyIgnoreInvalidCopyRequests",
        /* strMthInArgs */ strMthInArgs );

    m_bSilentlyIgnoreInvalidCopyRequests = i_bExecuteSilently;

} // setSilentlyIgnoreInvalidCopyRequests

/*==============================================================================
public: // overridable slots of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::expandAll()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "expandAll",
        /* strMthInArgs */ strMthInArgs );

    QTreeView::expandAll();

    QModelIndex modelIdxRoot = model()->index(0, 0, QModelIndex());

    //expand(modelIdxRoot);

    expandRecursive(modelIdxRoot);

} // expandAll

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::collapseAll()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
void CTreeViewIdxTreeGraphObjs::expandRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "expandRecursive",
        /* strMthInArgs */ strMthInArgs );

    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            expand(i_modelIdx);

            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            CModelIdxTreeEntry* pModelTreeEntryChild;

            QModelIndex modelIdxChild;

            for( int idxEntry = 0; idxEntry < pModelBranch->count(); ++idxEntry )
            {
                pModelTreeEntryChild = pModelBranch->at(idxEntry);

                if( pModelTreeEntryChild->entryType() == EIdxTreeEntryType::Root || pModelTreeEntryChild->entryType() == EIdxTreeEntryType::Branch )
                {
                    modelIdxChild = model()->index(idxEntry, 0, i_modelIdx);
                    expandRecursive(modelIdxChild);
                }
            }
        } // if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
    } // if( pModelTreeEntry != nullptr )

} // expandRecursive

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::collapseRecursive( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "collapseRecursive",
        /* strMthInArgs */ strMthInArgs );

    CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

    if( pModelTreeEntry != nullptr )
    {
        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            QModelIndex modelIdx;

            for( int idxEntry = 0; idxEntry < pModelBranch->count(); ++idxEntry )
            {
                pModelTreeEntry = pModelBranch->at(idxEntry);

                if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
                {
                    modelIdx = model()->index(idxEntry, 0, i_modelIdx);
                    collapseRecursive(modelIdx);
                }
            }

            collapse(i_modelIdx);

        } // if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
    } // if( pModelTreeEntry != nullptr )

} // collapseRecursive

/*==============================================================================
public: // slots (hiding not overridable slots with same name in QTreeView)
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::expand( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
void CTreeViewIdxTreeGraphObjs::collapse( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
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
void CTreeViewIdxTreeGraphObjs::onCollapsed( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onCollapsed",
        /* strMthInArgs */ strMthInArgs );

    if( i_modelIdx.isValid() )
    {
        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            pModelIdxTree->setIsExpanded(pModelBranch, false);
        }
    } // if( i_modelIdx.isValid() )

} // onCollapsed

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onExpanded( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
    {
        strMthInArgs = "ModelIdx {" + CModelIdxTree::ModelIdx2Str(i_modelIdx) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onExpanded",
        /* strMthInArgs */ strMthInArgs );

    if( i_modelIdx.isValid() )
    {
        #if QT_VERSION >= 0x040000 && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        //resizeColumnToContents(i_modelIdx.column());
        #elif QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        // Cannot invoke resizeColumnToContents as this leads to an exception.
        #endif

        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
        {
            CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;
            pModelIdxTree->setIsExpanded(pModelBranch, true);
        }
    } // if( i_modelIdx.isValid() )

} // onExpanded

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onCurrentChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& /*i_modelIdxPrev*/ )
//------------------------------------------------------------------------------
{
    CGraphObj* pGraphObj = nullptr;

    if( i_modelIdxCurr.isValid() )
    {
        CModelIdxTreeEntry* pModelEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdxCurr.internalPointer());

        if( pModelEntry != nullptr )
        {
            pGraphObj = dynamic_cast<CGraphObj*>(pModelEntry->treeEntry());
        }
    }

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "GraphObj: " + QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onCurrentChanged",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);

    if( pGraphicsItem != nullptr )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

        if( pGraphObj != nullptr )
        {
            QGraphicsItem* pGraphicsItemSelected = pGraphicsItem;
            CGraphObj*     pGraphObjSelected = pGraphObj;

            // Selection Points cannot be selected. When clicking on a selection point
            // the parent got to be selected.
            if( pGraphObj->getType() == EGraphObjTypeSelectionPoint )
            {
                CGraphObjSelectionPoint* pSelectionPoint = dynamic_cast<CGraphObjSelectionPoint*>(pGraphObj);

                if( pSelectionPoint != nullptr )
                {
                    pGraphObjSelected = pSelectionPoint->getSelectedGraphObj();
                    pGraphicsItemSelected = dynamic_cast<QGraphicsItem*>(pGraphObjSelected);
                }
            }

            // On clicking on a selection point and removing the selection of it's parent object
            // the parent object will hide and destroy the selection point - which has currently
            // beeing clicked. The selection may only be cleared therefore if a new object has
            // been selected. Otherwise the selected tree view entry would be destroyed while it
            // has been selected. And afterwards created again. And so on. This will either end up
            // in a deadlock (endless loop) or at least with access violations,
            if( !pGraphicsItemSelected->isSelected() )
            {
                QObject::disconnect(
                    /* pObjSender   */ m_pDrawingScene,
                    /* szSignal     */ SIGNAL(selectionChanged()),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onDrawingSceneSelectionChanged()) );

                m_pDrawingScene->clearSelection();

                pGraphicsItemSelected->setSelected(true);

                if( !QObject::connect(
                    /* pObjSender   */ m_pDrawingScene,
                    /* szSignal     */ SIGNAL(selectionChanged()),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onDrawingSceneSelectionChanged()) ) )
                {
                    throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
                }
            }
        }
    }

} // onCurrentChanged

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onDoubleClicked( const QModelIndex& i_modelIdx )
//------------------------------------------------------------------------------
{
    CGraphObj* pGraphObj = nullptr;

    if( i_modelIdx.isValid() )
    {
        CModelIdxTreeEntry* pModelEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdx.internalPointer());

        if( pModelEntry != nullptr )
        {
            pGraphObj = dynamic_cast<CGraphObj*>(pModelEntry->treeEntry());
        }
    }

    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "GraphObj: " + QString(pGraphObj == nullptr ? "nullptr" : pGraphObj->path());
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDoubleClicked",
        /* strAddInfo   */ strMthInArgs );

    QGraphicsItem* pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);

    if( pGraphicsItem != nullptr )
    {
        m_pDrawingScene->clearSelection();

        pGraphicsItem->setSelected(true);

        CDlgFormatGraphObjs* pDlgFormatGraphObjs = new CDlgFormatGraphObjs(m_pDrawingScene,pGraphObj);

        pDlgFormatGraphObjs->setCurrentWidget(CDlgFormatGraphObjs::c_strWdgtObjName);

        pDlgFormatGraphObjs->exec();

        delete pDlgFormatGraphObjs;
        pDlgFormatGraphObjs = nullptr;
    }

} // onDoubleClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onDrawingSceneSelectionChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onDrawingSceneSelectionChanged",
        /* strAddInfo   */ "" );

    QObject::disconnect(
        /* pObjSender   */ selectionModel(),
        /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onCurrentChanged(const QModelIndex&, const QModelIndex&) ) );

    clearSelection();

    QList<QGraphicsItem*> arpGraphicItems = m_pDrawingScene->selectedItems();
    QModelIndex           modelIdx;

    for( auto& pGraphicsItem : arpGraphicItems )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

        modelIdx = dynamic_cast<CModelIdxTreeGraphObjs*>(model())->index(pGraphObj->keyInTree(), 0);

        if( modelIdx.isValid() )
        {
            setCurrentIndex(modelIdx);
            scrollTo(modelIdx);
        }
    }

    if( !QObject::connect(
        /* pObjSender   */ selectionModel(),
        /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onCurrentChanged(const QModelIndex&, const QModelIndex&) ) ) )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultSignalSlotConnectionFailed);
    }

} // onDrawingSceneSelectionChanged

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CTreeViewIdxTreeGraphObjs::event( QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObjEvent != nullptr && m_pTrcAdminObjEvent->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iEventDetails = m_pTrcAdminObjEvent->getTraceDetailLevel() >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event {" + qEvent2Str(i_pEv, iEventDetails) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjEvent,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "event",
        /* strMthInArgs */ strMthInArgs );

    return QTreeView::event(i_pEv);

} // event

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iEventDetails = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event {" + qKeyEvent2Str(i_pEv, iEventDetails) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "keyPressEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;

    QItemSelectionModel* pSelModel = selectionModel();

    QModelIndex modelIdxSelected = pSelModel->currentIndex();

    if( modelIdxSelected.isValid() )
    {
        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());
        CIdxTree*      pIdxTree = pModelIdxTree->idxTree();

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdxSelected.internalPointer());

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::ModelIdx2Str(modelIdxSelected) + "}";
            mthTracer.trace(strMthInArgs);
        }

        if( pIdxTree != nullptr && pModelTreeEntry != nullptr )
        {
            switch( i_pEv->key() )
            {
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
                        QString strMsg = "Do you really want to delete \"" + pModelTreeEntry->keyInTree() + "\"?";
                        iRet = QMessageBox::question(
                            /* pWdgtParent     */ this,
                            /* strTitle        */ getMainWindowTitle(),
                            /* strText         */ strMsg,
                            /* standardButtons */ QMessageBox::Yes | QMessageBox::No,
                            /* defaultButton   */ QMessageBox::No );
                    }
                    if( iRet == QMessageBox::Yes )
                    {
                        CIdxTreeEntry* pTreeEntry = pModelTreeEntry->treeEntry();
                        delete pTreeEntry;
                        pTreeEntry = nullptr;
                        pModelTreeEntry = nullptr;
                        //pIdxTree->remove(pModelTreeEntry->keyInTree());
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

                            if( pModelTreeEntry->entryType() != EIdxTreeEntryType::Root && pModelTreeEntry->entryType() != EIdxTreeEntryType::Branch )
                            {
                                pModelBranchTrg = pModelTreeEntry->modelParentBranch();
                            }
                            else
                            {
                                pModelBranchTrg = pModelTreeEntry;
                            }
                            if( pModelBranchTrg != nullptr )
                            {
                                CModelIdxTreeEntry* pModelTreeEntrySrc = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedForPaste.internalPointer());
                                CIdxTreeEntry*      pTreeEntrySrc = pModelTreeEntrySrc->treeEntry();
                                CIdxTreeEntry*      pBranchTrg    = pModelBranchTrg->treeEntry();

                                int idxInTargetBranch = -1;

                                if( pModelIdxTree->sortOrder() == EIdxTreeSortOrder::Config )
                                {
                                    idxInTargetBranch = m_modelIdxSelectedForPaste.row();
                                }
                                else // if( pModelIdxTree->sortOrder() != EIdxTreeSortOrder::Config )
                                {
                                    idxInTargetBranch = pTreeEntrySrc->indexInParentBranch();
                                }

                                if( m_pasteMode == EPasteMode::Copy )
                                {
                                    SErrResultInfo errResultInfo = pIdxTree->canCopy(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);

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
                                        pIdxTree->copy(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);
                                    }
                                }
                                else if( m_pasteMode == EPasteMode::Cut )
                                {
                                    SErrResultInfo errResultInfo = pIdxTree->canMove(pTreeEntrySrc, pBranchTrg, idxInTargetBranch);

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
                                        pIdxTree->move(pTreeEntrySrc, pBranchTrg);
                                    }
                                }

                                m_modelIdxSelectedForPaste = QModelIndex();
                                m_pasteMode = EPasteMode::Undefined;

                            } // if( pModelBranchTrg != nullptr )
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
        } // if( pIdxTree != nullptr && pModelTreeEntry != nullptr )
    } // if( modelIdxSelected.isValid() )

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

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iEventDetails = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv, iEventDetails) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mousePressEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMousePressEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
        {
            strMthInArgs = "ModelIdxPressed {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs);
        }

        if( pModelTreeEntry != nullptr )
        {
            if( i_pEv->buttons() & Qt::RightButton )
            {
                if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )
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

                } // if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Root || pModelTreeEntry->entryType() == EIdxTreeEntryType::Branch )

                else if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Leave )
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

                } // if( pModelTreeEntry->entryType() == EIdxTreeEntryType::Leave )
            } // if( i_pEv->buttons() & Qt::RightButton )
        } // if( pModelTreeEntry != nullptr )
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

    if( !bEventHandled )
    {
        QTreeView::mousePressEvent(i_pEv);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iEventDetails = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv, iEventDetails) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mouseReleaseEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;

    m_modelIdxSelectedOnMouseReleaseEvent = indexAt(i_pEv->pos());

    if( m_modelIdxSelectedOnMouseReleaseEvent.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMouseReleaseEvent.internalPointer());

        if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs ) )
        {
            strMthInArgs = "ModelIdxReleased {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedOnMouseReleaseEvent) + "}";
            mthTracer.trace(strMthInArgs);
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
void CTreeViewIdxTreeGraphObjs::mouseDoubleClickEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iEventDetails = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv, iEventDetails) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mouseDoubleClickEvent",
        /* strMthInArgs */ strMthInArgs );

    bool bEventHandled = false;

    if( !bEventHandled )
    {
        QTreeView::mouseDoubleClickEvent(i_pEv);
    }

} // mouseDoubleClickEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::mouseMoveEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iEventDetails = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event {" + qMouseEvent2Str(i_pEv, iEventDetails) + "}";
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "mouseMoveEvent",
        /* strMthInArgs */ strMthInArgs );

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
void CTreeViewIdxTreeGraphObjs::startDrag( Qt::DropActions i_supportedActions )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "SupportedActions: " + qDropActions2Str(i_supportedActions);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "startDrag",
        /* strMthInArgs */ strMthInArgs );

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // See Qt Centre
    // QUESTION: Where can I fix the QTreeView's default drop action?
    // Is there some kind of compatibility switch at Application level?
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    QModelIndexList indexes = selectedIndexes();
    QList<QPersistentModelIndex> persistentIndexes;
 
    if( indexes.count() > 0 )
    {
        QMimeData* data = model()->mimeData(indexes);

        if( data == nullptr )
        {
            return;
        }

        for( int i = 0; i<indexes.count(); i++ )
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

        if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) )
        {
            strMthInArgs = "SupportedActions: " + qDropActions2Str(i_supportedActions);
            strMthInArgs += ", DefaultAction: " + qDropAction2Str(defaultDropAction);
            mthTracer.trace("-> drag.exec(" + strMthInArgs + ")");
        }

        Qt::DropAction dropActionDragExec = drag->exec(i_supportedActions, defaultDropAction);

        if( mthTracer.isActive(ETraceDetailLevelRuntimeInfo) )
        {
            strMthInArgs = qDropAction2Str(dropActionDragExec);
            mthTracer.trace("<- drag.exec(" + strMthInArgs + ")");
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
                    model()->removeRow(idx.row(), idx.parent());
                }
                else
                {
                    model()->removeRow(idx.row(), QModelIndex());
                }
            } // for( int i = 0; i < indexes.count(); i++ )
        } // if( drag->exec(i_supportedActions, defaultDropAction) == Qt::MoveAction )
    } // if( indexes.count() > 0 )

} // startDrag

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::dragEnterEvent( QDragEnterEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iEventDetails = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event: " + qDragEnterEvent2Str(i_pEv, iEventDetails);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dragEnterEvent",
        /* strMthInArgs */ strMthInArgs );

    QTreeView::dragEnterEvent(i_pEv);

} // dragEnterEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::dragLeaveEvent( QDragLeaveEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iEventDetails = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event: " + qDragLeaveEvent2Str(i_pEv, iEventDetails);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dragLeaveEvent",
        /* strMthInArgs */ strMthInArgs );

    QTreeView::dragLeaveEvent(i_pEv);

} // dragLeaveEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::dragMoveEvent( QDragMoveEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iEventDetails = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event: " + qDragMoveEvent2Str(i_pEv, iEventDetails);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dragMoveEvent",
        /* strMthInArgs */ strMthInArgs );

    QTreeView::dragMoveEvent(i_pEv);

} // dragMoveEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::dropEvent( QDropEvent* i_pEv )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        int iEventDetails = m_pTrcAdminObj->getTraceDetailLevel() >= ETraceDetailLevelInternalStates ? 1 : 0;
        strMthInArgs = "Event: " + qDropEvent2Str(i_pEv, iEventDetails);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dropEvent",
        /* strMthInArgs */ strMthInArgs );

    QTreeView::dropEvent(i_pEv);

} // dropEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onActionBranchExpandTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionBranchExpandTriggered",
        /* strMthInArgs */ strMthInArgs );

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
void CTreeViewIdxTreeGraphObjs::onActionBranchCollapseTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionBranchCollapseTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        collapseRecursive(m_modelIdxSelectedOnMousePressEvent);
    }
} // onActionBranchCollapseTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onActionBranchCreateNewBranchTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionBranchCreateNewBranchTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());
        CIdxTree*      pIdxTree = pModelIdxTree->idxTree();

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs);
        }

        CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

        if( pIdxTree != nullptr && pModelBranch != nullptr )
        {
            CIdxTreeEntry* pBranch = pModelBranch->treeEntry();

            QString strName = "New Branch";
            QString strUniqueName = strName;
            int iCopies = 1;

            while( pBranch->indexOf(EIdxTreeEntryType::Branch, strUniqueName) >= 0 )
            {
                strUniqueName = strName + QString::number(++iCopies);
            }
            strName = strUniqueName;

            CIdxTreeEntry* pBranchNew = pIdxTree->createBranch(strName);

            pIdxTree->add(pBranchNew, pBranch);

        } // if( pIdxTree != nullptr && pModelBranch != nullptr )
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionBranchCreateNewBranchTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onActionBranchCreateNewLeaveTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionBranchCreateNewLeaveTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());
        CIdxTree*      pIdxTree = pModelIdxTree->idxTree();

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs);
        }

        CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

        if( pIdxTree != nullptr && pModelBranch != nullptr )
        {
            CIdxTreeEntry* pBranch = pModelBranch->treeEntry();

            QString strName = "New Leave";
            QString strUniqueName = strName;
            int iCopies = 1;

            while( pBranch->indexOf(EIdxTreeEntryType::Leave, strUniqueName) >= 0 )
            {
                strUniqueName = strName + QString::number(++iCopies);
            }
            strName = strUniqueName;

            CIdxTreeEntry* pLeaveNew = pIdxTree->createLeave(strName);

            pIdxTree->add(pLeaveNew, pBranch);

        } // if( pIdxTree != nullptr && pModelBranch != nullptr )
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionBranchCreateNewLeaveTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onActionBranchDeleteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionBranchDeleteTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());
        CIdxTree*      pIdxTree = pModelIdxTree->idxTree();

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs);
        }

        CModelIdxTreeEntry* pModelBranch = pModelTreeEntry;

        if( pIdxTree != nullptr && pModelBranch != nullptr )
        {
            CIdxTreeEntry* pBranch = pModelBranch->treeEntry();

            delete pBranch;
            pBranch = nullptr;

        } // if( pIdxTree != nullptr && pModelBranch != nullptr )
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionBranchDeleteTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onActionBranchCutTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionBranchCutTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        m_modelIdxSelectedForPaste = m_modelIdxSelectedOnMousePressEvent;
        m_pasteMode = EPasteMode::Cut;

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedForPaste) + "}";
            mthTracer.trace(strMthInArgs);
        }
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionBranchCutTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onActionBranchCopyTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionBranchCopyTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        m_modelIdxSelectedForPaste = m_modelIdxSelectedOnMousePressEvent;
        m_pasteMode = EPasteMode::Copy;

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedForPaste) + "}";
            mthTracer.trace(strMthInArgs);
        }
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionBranchCopyTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onActionBranchPasteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionBranchPasteTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() && m_modelIdxSelectedForPaste.isValid() )
    {
        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strMthInArgs  = "ModelIdxSrc {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedForPaste) + "}";
            strMthInArgs += ", ModelIdxTrg {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs);
        }

        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());
        CIdxTree*      pIdxTree = pModelIdxTree->idxTree();

        CModelIdxTreeEntry* pModelTreeEntrySrc = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedForPaste.internalPointer());
        CModelIdxTreeEntry* pModelTreeEntryTrg = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());
        CModelIdxTreeEntry* pModelBranchTrg    = nullptr;

        if( pModelTreeEntryTrg->entryType() != EIdxTreeEntryType::Root && pModelTreeEntryTrg->entryType() != EIdxTreeEntryType::Branch )
        {
            pModelBranchTrg = pModelTreeEntryTrg->modelParentBranch();
        }
        else
        {
            pModelBranchTrg = pModelTreeEntryTrg;
        }

        if( pModelBranchTrg != nullptr )
        {
            CIdxTreeEntry* pTreeEntrySrc = pModelTreeEntrySrc->treeEntry();
            CIdxTreeEntry* pBranchTrg    = pModelBranchTrg->treeEntry();

            if( m_pasteMode == EPasteMode::Copy )
            {
                pIdxTree->copy(pTreeEntrySrc, pBranchTrg);
            }
            else if( m_pasteMode == EPasteMode::Cut )
            {
                pIdxTree->move(pTreeEntrySrc, pBranchTrg);
                m_modelIdxSelectedForPaste = QModelIndex();
                m_pasteMode = EPasteMode::Undefined;
            }
        } // if( pModelBranchTrg != nullptr )
    } // if( m_modelIdxSelectedForPaste.isValid() )

} // onActionBranchPasteTriggered

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onActionLeaveDeleteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionLeaveDeleteTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());
        CIdxTree*      pIdxTree = pModelIdxTree->idxTree();

        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs);
        }

        CModelIdxTreeEntry* pModelLeave = pModelTreeEntry;

        if( pIdxTree != nullptr && pModelLeave != nullptr )
        {
            CIdxTreeEntry* pLeave = pModelLeave->treeEntry();

            delete pLeave;
            pLeave = nullptr;

        } // if( pIdxTree != nullptr && pModelLeave != nullptr )
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionLeaveDeleteTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onActionLeaveCutTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionLeaveCutTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        m_modelIdxSelectedForPaste = m_modelIdxSelectedOnMousePressEvent;
        m_pasteMode = EPasteMode::Cut;

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedForPaste) + "}";
            mthTracer.trace(strMthInArgs);
        }
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionLeaveCutTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onActionLeaveCopyTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionLeaveCopyTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        m_modelIdxSelectedForPaste = m_modelIdxSelectedOnMousePressEvent;
        m_pasteMode = EPasteMode::Copy;

        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strMthInArgs = "ModelIdxSelected {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedForPaste) + "}";
            mthTracer.trace(strMthInArgs);
        }
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionLeaveCopyTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeGraphObjs::onActionLeavePasteTriggered( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "onActionLeavePasteTriggered",
        /* strMthInArgs */ strMthInArgs );

    if( m_modelIdxSelectedOnMousePressEvent.isValid() )
    {
        if( mthTracer.isActive(ETraceDetailLevelInternalStates) )
        {
            strMthInArgs  = "ModelIdxSrc {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedForPaste) + "}";
            strMthInArgs += ", ModelIdxTrg {" + CModelIdxTree::ModelIdx2Str(m_modelIdxSelectedOnMousePressEvent) + "}";
            mthTracer.trace(strMthInArgs);
        }

        CModelIdxTree* pModelIdxTree = dynamic_cast<CModelIdxTree*>(model());
        CIdxTree*      pIdxTree = pModelIdxTree->idxTree();

        CModelIdxTreeEntry* pModelTreeEntrySrc = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedForPaste.internalPointer());
        CModelIdxTreeEntry* pModelTreeEntryTrg = static_cast<CModelIdxTreeEntry*>(m_modelIdxSelectedOnMousePressEvent.internalPointer());
        CModelIdxTreeEntry* pModelBranchTrg    = nullptr;

        if( pModelTreeEntryTrg->entryType() != EIdxTreeEntryType::Root && pModelTreeEntryTrg->entryType() != EIdxTreeEntryType::Branch )
        {
            pModelBranchTrg = pModelTreeEntryTrg->modelParentBranch();
        }
        else
        {
            pModelBranchTrg = pModelTreeEntryTrg;
        }

        if( pModelBranchTrg != nullptr )
        {
            CIdxTreeEntry* pTreeEntrySrc = pModelTreeEntrySrc->treeEntry();
            CIdxTreeEntry* pBranchTrg    = pModelBranchTrg->treeEntry();

            if( m_pasteMode == EPasteMode::Copy )
            {
                pIdxTree->copy(pTreeEntrySrc, pBranchTrg);
            }
            else if( m_pasteMode == EPasteMode::Cut )
            {
                pIdxTree->move(pTreeEntrySrc, pBranchTrg);
                m_modelIdxSelectedForPaste = QModelIndex();
                m_pasteMode = EPasteMode::Undefined;
            }
        } // if( pModelBranchTrg != nullptr )
    } // if( m_modelIdxSelectedOnMousePressEvent.isValid() )

} // onActionLeavePasteTriggered
