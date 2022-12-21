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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#endif

#include "ZSSysGUI/ZSSysIdxTreeWdgt.h"
#include "ZSSysGUI/ZSSysIdxTreeView.h"
#include "ZSSysGUI/ZSSysIdxTreeTableViewBranchContent.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSSysGUI/ZSSysIdxTreeModel.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CWdgtIdxTree : public QWidget
*******************************************************************************/

/*==============================================================================
public: // type definitions and constants
==============================================================================*/

/* enum class EViewMode
==============================================================================*/

static const SEnumEntry s_arEnumStrWdgtIdxTreeViewModes[] = {                              // IdxName,                     Symbol
    /*  0 */ SEnumEntry( static_cast<int>(CWdgtIdxTree::EViewMode::NavPanelOnly),             "NavPanelOnly",              "NP"   ),
    /*  1 */ SEnumEntry( static_cast<int>(CWdgtIdxTree::EViewMode::NavPanelAndBranchContent), "NavPanelAndBranchContent",  "NPBC" )
};

//------------------------------------------------------------------------------
QString CWdgtIdxTree::viewMode2Str( EViewMode i_eVal, int i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrWdgtIdxTreeViewModes, _ZSArrLen(s_arEnumStrWdgtIdxTreeViewModes), static_cast<int>(i_eVal), i_alias);
}

//------------------------------------------------------------------------------
QPixmap CWdgtIdxTree::viewMode2Pixmap( EViewMode i_eVal, const QSize& i_sz )
//------------------------------------------------------------------------------
{
    QString str = viewMode2Str(i_eVal);
    QPixmap pxm = QPixmap( ":/ZS/TreeView/TreeViewViewMode" + str + ".png" );
    pxm = pxm.scaled(i_sz);
    return pxm;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtIdxTree::CWdgtIdxTree(
    CModelIdxTree* i_pModel,
    CModelIdxTreeBranchContent* i_pModelBranchContent,
    QWidget* i_pWdgtParent,
    Qt::WindowFlags i_wflags,
    EMethodTraceDetailLevel i_eTrcDetailLevel ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_viewMode(EViewMode::NavPanelOnly),
    m_szBtns(24, 24),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pBtnViewMode(nullptr),
    m_pBtnTreeViewResizeRowsAndColumnsToContents(nullptr),
    m_pBtnTreeViewExpandAll(nullptr),
    m_pBtnTreeViewCollapseAll(nullptr),
    m_pBtnSortOrder(nullptr),
    m_pEdtBranch(nullptr),
    m_pSpcHeadLine(nullptr),
    m_pModel(i_pModel),
    m_pModelBranchContent(i_pModelBranchContent),
    m_pSplitter(nullptr),
    m_pTreeView(nullptr),
    m_pTableViewBranchContent(nullptr),
    m_eTrcDetailLevel(i_eTrcDetailLevel),
    m_pTrcAdminObj(nullptr)
{
    setObjectName( i_pModel == nullptr ? "IdxTree" : i_pModel->objectName() );

    QString strMthInArgs;

    // If the tree's parent is the trace server the detail level of trace outputs
    // may not be controlled by trace admin objects as the belong to the index tree
    // of the trace server.
    if( i_pModel != nullptr && i_pModel->idxTree() != nullptr )
    {
        if( dynamic_cast<CTrcServer*>(i_pModel->idxTree()->parent()) == nullptr )
        {
            m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), objectName());

            if( m_pTrcAdminObj != nullptr )
            {
                m_eTrcDetailLevel = m_pTrcAdminObj->getMethodCallsTraceDetailLevel();

                if( !QObject::connect(
                    /* pObjSender   */ m_pTrcAdminObj,
                    /* szSignal     */ SIGNAL(changed(QObject*)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onTrcAdminObjChanged(QObject*)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
        }
    }

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Model: " + QString(i_pModel == nullptr ? "nullptr" : i_pModel->objectName());
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "ctor",
        /* strMethodInArgs    */ strMthInArgs );

    m_pLytMain = new QVBoxLayout();
    setLayout(m_pLytMain);

    // Line with controls to modify optic of tree view
    //================================================

    m_pLytHeadLine = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytHeadLine);

    // <Button> View Mode
    //-------------------

    QPixmap pxmViewMode = viewMode2Pixmap(m_viewMode, m_szBtns);
    pxmViewMode.setMask(pxmViewMode.createHeuristicMask());

    m_pBtnViewMode = new QPushButton();
    m_pBtnViewMode->setFixedSize(m_szBtns);
    m_pBtnViewMode->setIcon(pxmViewMode);
    m_pBtnViewMode->setToolTip("Press to toggle view mode between NavPanelOnly and NavPanelAndBranchContent");
    m_pLytHeadLine->addWidget(m_pBtnViewMode);
    m_pLytHeadLine->addSpacing(10);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnViewMode,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnViewModeClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Resize Columns To Contents
    //------------------------------------

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.png");

    m_pBtnTreeViewResizeRowsAndColumnsToContents = new QPushButton();
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setFixedSize(m_szBtns);
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setToolTip("Press to resize the columns to their contents");
    m_pLytHeadLine->addWidget(m_pBtnTreeViewResizeRowsAndColumnsToContents);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnTreeViewResizeRowsAndColumnsToContents,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTreeViewResizeRowsAndColumnsToContentsClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Expand All
    //--------------------

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.png");

    m_pBtnTreeViewExpandAll = new QPushButton();
    m_pBtnTreeViewExpandAll->setIcon(pxmExpandAll);
    m_pBtnTreeViewExpandAll->setFixedSize(m_szBtns);
    m_pBtnTreeViewExpandAll->setToolTip("Press to expand all branches of the tree");
    m_pLytHeadLine->addWidget(m_pBtnTreeViewExpandAll);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnTreeViewExpandAll,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTreeViewExpandAllClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Collapse All
    //----------------------

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.png");

    m_pBtnTreeViewCollapseAll = new QPushButton();
    m_pBtnTreeViewCollapseAll->setIcon(pxmCollapseAll);
    m_pBtnTreeViewCollapseAll->setFixedSize(m_szBtns);
    m_pBtnTreeViewCollapseAll->setToolTip("Press to collapse all branches of the tree");
    m_pLytHeadLine->addWidget(m_pBtnTreeViewCollapseAll);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnTreeViewCollapseAll,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTreeViewCollapseAllClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Sort Order
    //----------------------

    QPixmap pxmSortOrder = idxTreeSortOrder2Pixmap(EIdxTreeSortOrder::Config, m_szBtns);

    m_pBtnSortOrder = new QPushButton();
    m_pBtnSortOrder->setFixedSize(m_szBtns);
    m_pBtnSortOrder->setIcon(pxmSortOrder);
    m_pBtnSortOrder->setProperty("SortOrderCurr", QVariant(static_cast<int>(EIdxTreeSortOrder::Config)));
    m_pBtnSortOrder->setToolTip("Press to toggle the sort order between \"As Configured\" and \"Alphabetically Sorted\"");
    m_pLytHeadLine->addWidget(m_pBtnSortOrder);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnSortOrder,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnSortOrderClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <LineEdit> Branch
    //------------------

    // Only visible in view mode "Show Branch Content Pane"
    m_pSpcHeadLine = new QSpacerItem(0, 24, QSizePolicy::Expanding);
    m_pLytHeadLine->addSpacerItem(m_pSpcHeadLine);

    // <TreeView>
    //===========

    m_pTreeView = new CTreeViewIdxTree(m_pModel);
    m_pTreeView->setAlternatingRowColors(true);
    m_pLytMain->addWidget(m_pTreeView, 1);

    if( !QObject::connect(
        /* pObjSender   */ m_pTreeView->selectionModel(),
        /* szSignal     */ SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTreeViewSelectionModelCurrentRowChanged(const QModelIndex&, const QModelIndex&)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( m_pTreeView != nullptr )
    {
        m_pTreeView->resizeColumnToContents(CModelIdxTree::EColumnTreeEntryName);
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtIdxTree::~CWdgtIdxTree()
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "dtor",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pTrcAdminObj != nullptr )
    {
        mthTracer.onAdminObjAboutToBeReleased();

        CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    }

    m_szBtns = QSize(0, 0);
    m_viewMode = static_cast<EViewMode>(0);
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pBtnViewMode = nullptr;
    m_pBtnTreeViewResizeRowsAndColumnsToContents = nullptr;
    m_pBtnTreeViewExpandAll = nullptr;
    m_pBtnTreeViewCollapseAll = nullptr;
    m_pBtnSortOrder = nullptr;
    m_pEdtBranch = nullptr;
    m_pSpcHeadLine = nullptr;
    m_pModel = nullptr;
    m_pModelBranchContent = nullptr;
    m_pSplitter = nullptr;
    m_pTreeView = nullptr;
    m_pTableViewBranchContent = nullptr;
    m_eTrcDetailLevel = static_cast<EMethodTraceDetailLevel>(0);
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTree::setViewMode( EViewMode i_viewMode )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "ViewMode: " + viewMode2Str(i_viewMode);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "setViewMode",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_viewMode != m_viewMode )
    {
        m_viewMode = i_viewMode;

        if( m_viewMode == EViewMode::NavPanelOnly )
        {
            if( m_pSplitter != nullptr )
            {
                // Reparent tree view. This will remove the tree view
                // from the splitter but does not destroy the tree view.
                m_pLytMain->addWidget(m_pTreeView);

                delete m_pSplitter;
                m_pSplitter = nullptr;
                m_pTableViewBranchContent = nullptr;
            }

            m_pLytHeadLine->removeWidget(m_pEdtBranch);
            delete m_pEdtBranch;
            m_pEdtBranch = nullptr;

            m_pSpcHeadLine = new QSpacerItem(0, 24, QSizePolicy::Expanding);
            m_pLytHeadLine->addSpacerItem(m_pSpcHeadLine);

            m_pModel->setExcludeLeaves(false);
        }
        else if( m_viewMode == EViewMode::NavPanelAndBranchContent )
        {
            // The dtor of the spacer item does not inform the layout of its destruction.
            m_pLytHeadLine->removeItem(m_pSpcHeadLine);
            delete m_pSpcHeadLine;
            m_pSpcHeadLine = nullptr;

            m_pEdtBranch = new QLineEdit();
            m_pLytHeadLine->addWidget(m_pEdtBranch, 1);

            if( m_pSplitter == nullptr )
            {
                if( m_pTreeView != nullptr )
                {
                    m_pLytMain->removeWidget(m_pTreeView);
                }

                m_pSplitter = new QSplitter(Qt::Horizontal);
                m_pLytMain->addWidget(m_pSplitter, 1);

                if( m_pTreeView != nullptr )
                {
                    m_pSplitter->addWidget(m_pTreeView);
                }
                if( m_pTableViewBranchContent == nullptr )
                {
                    m_pTableViewBranchContent = new CTableViewIdxTreeBranchContent(m_pModelBranchContent);
                }
                m_pSplitter->addWidget(m_pTableViewBranchContent);
            }
            m_pModel->setExcludeLeaves(true);
        }

        QPixmap pxmViewMode = viewMode2Pixmap(m_viewMode, m_szBtns);
        pxmViewMode.setMask(pxmViewMode.createHeuristicMask());

        m_pBtnViewMode->setIcon(pxmViewMode);

    } // if( i_viewMode != m_viewMode )

} // setViewMode

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTree::onBtnViewModeClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onBtnViewModeClicked",
        /* strMethodInArgs    */ strMthInArgs );

    EViewMode viewModeNew = m_viewMode;

    if( m_viewMode == EViewMode::NavPanelOnly )
    {
        viewModeNew = EViewMode::NavPanelAndBranchContent;
    }
    else // if( m_viewMode == EViewMode::NavPanelAndBranchContent )
    {
        viewModeNew = EViewMode::NavPanelOnly;
    }

    setViewMode(viewModeNew);

} // onBtnViewModeClicked

//------------------------------------------------------------------------------
void CWdgtIdxTree::onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onBtnTreeViewResizeRowsAndColumnsToContentsClicked",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pTreeView != nullptr )
    {
        for( int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++ )
        {
            m_pTreeView->resizeColumnToContents(idxClm);
        }
    }

} // onBtnTreeViewResizeRowsAndColumnsToContentsClicked

//------------------------------------------------------------------------------
void CWdgtIdxTree::onBtnTreeViewExpandAllClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onBtnTreeViewExpandAllClicked",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pTreeView != nullptr )
    {
        m_pTreeView->expandAll();

        for( int idxClm = 0; idxClm < CModelIdxTree::EColumnCount; idxClm++ )
        {
            m_pTreeView->resizeColumnToContents(idxClm);
        }
    }

} // onBtnTreeViewExpandAllClicked

//------------------------------------------------------------------------------
void CWdgtIdxTree::onBtnTreeViewCollapseAllClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onBtnTreeViewCollapseAllClicked",
        /* strMethodInArgs    */ strMthInArgs );

    if( m_pTreeView != nullptr )
    {
        m_pTreeView->collapseAll();
    }

} // onBtnTreeViewCollapseAllClicked

//------------------------------------------------------------------------------
void CWdgtIdxTree::onBtnSortOrderClicked( bool i_bChecked )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs = "Checked: " + bool2Str(i_bChecked);
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onBtnSortOrderClicked",
        /* strMethodInArgs    */ strMthInArgs );

    EIdxTreeSortOrder sortOrderCurr = static_cast<EIdxTreeSortOrder>(m_pBtnSortOrder->property("SortOrderCurr").toInt());
    EIdxTreeSortOrder sortOrderNew;

    if( sortOrderCurr == EIdxTreeSortOrder::Config )
    {
        sortOrderNew = EIdxTreeSortOrder::Ascending;
    }
    else // if( sortOrderCurr == EIdxTreeSortOrder::Ascending )
    {
        sortOrderNew = EIdxTreeSortOrder::Config;
    }

    if( m_pTreeView != nullptr )
    {
        m_pTreeView->setSortOrder(sortOrderNew);
    }
    if( m_pTableViewBranchContent != nullptr )
    {
        m_pTableViewBranchContent->setSortOrder(sortOrderNew);
    }

    m_pBtnSortOrder->setProperty("SortOrderCurr", static_cast<int>(sortOrderNew));

    QPixmap pxmSortOrder = idxTreeSortOrder2Pixmap(sortOrderNew, m_szBtns);
    m_pBtnSortOrder->setIcon(pxmSortOrder);

} // onBtnSortOrderClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTree::onTreeViewSelectionModelCurrentRowChanged(
    const QModelIndex& i_modelIdxCurr,
    const QModelIndex& i_modelIdxPrev )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_eTrcDetailLevel >= EMethodTraceDetailLevel::ArgsNormal )
    {
        strMthInArgs  = "Curr {" + CModelIdxTree::modelIdx2Str(i_modelIdxCurr) + "}";
        strMthInArgs += ", Prev {" + CModelIdxTree::modelIdx2Str(i_modelIdxPrev) + "}";
    }

    CMethodTracer mthTracer(
        /* pTrcAdminObj       */ m_pTrcAdminObj,
        /* pTrcServer         */ CTrcServer::GetInstance(),
        /* eTrcDetailLevel    */ m_eTrcDetailLevel,
        /* eFilterDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strNameSpace       */ NameSpace(),
        /* strClassName       */ ClassName(),
        /* strObjName         */ objectName(),
        /* strMethod          */ "onTreeViewSelectionModelCurrentRowChanged",
        /* strMethodInArgs    */ strMthInArgs );

    if( i_modelIdxCurr.isValid() )
    {
        CModelIdxTreeEntry* pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(i_modelIdxCurr.internalPointer());
        CModelIdxTreeEntry* pModelTreeEntryBranch = nullptr;

        if( pModelTreeEntry != nullptr )
        {
            if( pModelTreeEntry->isRoot() || pModelTreeEntry->isBranch() )
            {
                pModelTreeEntryBranch = pModelTreeEntry;
            }
        }
        if( pModelTreeEntryBranch != nullptr )
        {
            if( m_pEdtBranch != nullptr )
            {
                m_pEdtBranch->setText(pModelTreeEntryBranch->keyInTree());
            }
            if( m_pTableViewBranchContent != nullptr )
            {
                m_pTableViewBranchContent->setKeyInTreeOfRootEntry(pModelTreeEntryBranch->keyInTree());
            }
        }
        else
        {
            if( m_pEdtBranch != nullptr )
            {
                m_pEdtBranch->setText("");
            }
            if( m_pTableViewBranchContent != nullptr )
            {
                m_pTableViewBranchContent->setKeyInTreeOfRootEntry("");
            }
        }
    } // if( i_modelIdxCurr.isValid() )

} // onTreeViewSelectionModelCurrentRowChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtIdxTree::onTrcAdminObjChanged( QObject* i_pTrcAdminObj )
//------------------------------------------------------------------------------
{
    CTrcAdminObj* pTrcAdminObj = dynamic_cast<CTrcAdminObj*>(i_pTrcAdminObj);

    if( pTrcAdminObj != nullptr && m_pTrcAdminObj == pTrcAdminObj )
    {
        m_eTrcDetailLevel = pTrcAdminObj->getMethodCallsTraceDetailLevel();
    }
}
